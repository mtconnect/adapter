#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <curl/curl.h>

#include "stream.h"

#define INITIAL_BLOCK_SIZE (512 * 1024)
#define BOUNDARY_SIZE 48
#define PADDING (8 * 1024)

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

#ifdef WIN32
#define strncasecmp _strnicmp
#else
#define strncpy_s(d, i, s, c) strncpy(d, s, c); 
#endif

struct _XmlBlock {
  char *block;
  size_t block_size;
  char boundary[BOUNDARY_SIZE];
  char *start;
  char *end;
  size_t length;
  size_t boundaryLength;
  char in_body;
  CURL *context;
  int stopped;
  MTCStreamXMLHandler handler;
  void *user_data;
};

typedef struct _XmlBlock XmlBlock;

#ifdef WIN32
/* Find a string in another string doing a case insensitive search */
static char *strcasestr(const char *s, const char *f)
{
  const char *sp = s;
  const char *fp = f;
  
  while (*sp != '\0')
  {
    if (tolower(*sp) == tolower(*fp))
      fp++;
    else
      fp = f;
    if (*fp == '\0')
      return (char*) (sp - strlen(f));
    sp++;
  }
  
  return NULL;
}
#endif


static size_t HandleHeader(char *ptr, size_t size, size_t nmemb, void *user)
{
  char line[512];
  XmlBlock *data = (XmlBlock*) user;
  
  int len = size * nmemb;
  if (len > 511) len = 511;
  memcpy(line, ptr, len);
  line[len] = '\0';
  
  /* All we care about is the content type. */
  if (strncasecmp(line, "content-type:", 12) == 0)
  {
    char *bp;
    
    if (strstr(line, "multipart/x-mixed-replace") == NULL)
    {
      fprintf(stderr, "Incorrect content type: '%s', must be multipart/x-mixed-replace\n",
              line);
      exit(1);
    }
    
    bp = strstr(ptr, "boundary=");
    if (bp == NULL)
    {
      fprintf(stderr, "Cannot find boundary in %s\n", line);
      exit(1);
    }
    
    data->boundary[0] = data->boundary[1] = '-';
    strncpy_s(data->boundary + 2, BOUNDARY_SIZE - 3, bp + 9, BOUNDARY_SIZE - 3);
    data->boundary[BOUNDARY_SIZE - 1] = '\0';
    bp = data->boundary + (strlen(data->boundary) - 1);
    while (!isalnum(*bp))
      *bp-- = '\0';
    
    data->boundaryLength = strlen(data->boundary);
    
    printf("Found boundary: %s\n", data->boundary);
  }
  return size * nmemb;
}

static size_t HandleData(char *ptr, size_t size, size_t nmemb, void *user)
{
  /* First find the boundary in the current block. */
  XmlBlock *data = (XmlBlock*) user;
  int need_data;
  
  if (data->stopped) return 0;
  
  if (data->boundary[0] == '\0')
  {
    fprintf(stderr, "Data arrived without boundary\n");
    exit(1);
  }
  
  /* append the new data to the end of the block and null terminate */
  memcpy(data->end, ptr, size * nmemb);
  data->end += size * nmemb;
  *(data->end) = '\0';
  
  do
  {
    need_data = TRUE;
    if (!data->in_body)
    {
      // Look for the boundary
      char *bp = strstr(data->start, data->boundary);
      if (bp != NULL)
      {
        char *ep = strstr(bp, "\r\n\r\n");
        if (ep != NULL)
        {
          // Parse the headers after the boundary for the content length.
          char *cp;
          bp += data->boundaryLength + 2;
          cp = strcasestr(bp, "Content-length:");
          if (cp != NULL) {
            data->length = atoi(cp + 16);
          }
          
          // Make sure we have enough room for the new block of data
          if (data->length + (data->end - data->block) > data->block_size) {
            // Put a small buffer on to the end of the buffer. This is to account for some overflow
            // of additional bytes. Hopefully this will be large enough for subsequent blocks as well.
            int new_size = data->length + (data->end - data->block) + PADDING;
            char *new_block = (char *) malloc(new_size);
            memcpy(new_block,  data->block, data->block_size);
            data->block_size = new_size;
            
            /* Reset the pointers to the correct positions on the new block of data */
            data->end = new_block + (data->end - data->block);
            
            // Start will be reset below, so just fix up ep.
            ep = new_block + (ep - data->block);
            
            free(data->block);
            data->block = new_block;
          }
          
          // Scan for the "\r\n\r\n"
          data->in_body = 1;
          data->start = ep + 4;
        }
      }
    }
    
    if (data->in_body && (size_t) (data->end - data->start) >= (size_t) data->length)
    {
      char *ep;
      size_t len;
      int res;

      *(data->start + data->length) = '\0';
      
      /* We have a new chunk of xml data... */
      res = data->handler(data->start, data->user_data);
      if (res == 0) return 0;
      
      /* Consume the block and reset the pointers. */
      ep = data->start + data->length;
      len = data->end - ep;
      if (len > 0) memcpy(data->block, ep, len);
      data->start = data->block;
      data->end = data->block + len;
      data->length = 0;
      data->in_body = 0;
      
      if (len > 60) need_data = FALSE;
    }
  } while (!need_data);
  
  return size * nmemb;
}


// API Methods
void *MTCStreamInit(const char *aUrl, MTCStreamXMLHandler aHandler, void *aUserData)
{
  XmlBlock *data = (XmlBlock*) calloc(1, sizeof(XmlBlock));
  data->block = calloc(1, INITIAL_BLOCK_SIZE);
  data->start = data->end = data->block;
  data->block_size = INITIAL_BLOCK_SIZE;
  
  data->context = curl_easy_init();
  data->handler = aHandler;
  data->user_data = aUserData;

  //curl_easy_setopt(handle, CURLOPT_VERBOSE, 1);
  curl_easy_setopt(data->context, CURLOPT_URL, aUrl);
  curl_easy_setopt(data->context, CURLOPT_WRITEFUNCTION, HandleData);
  curl_easy_setopt(data->context, CURLOPT_WRITEDATA, data);
  curl_easy_setopt(data->context, CURLOPT_HEADERFUNCTION, HandleHeader);
  curl_easy_setopt(data->context, CURLOPT_WRITEHEADER, data);

  return data;
}

void MTCStreamStop(void *aContext)
{
  XmlBlock *data = (XmlBlock*) aContext;
  data->stopped = 1;
  
}

int MTCStreamStart(void *aContext)
{
  XmlBlock *data = (XmlBlock*) aContext;
  return curl_easy_perform(data->context);
}

void MTCStreamFree(void *aContext)
{
  XmlBlock *data = (XmlBlock*) aContext;
  free(data->block);
  
  /* Assumes the perform is now complete */
  free(aContext);
}

static size_t HandleReqHeader(char *ptr, size_t size, size_t nmemb, void *user)
{
  XmlBlock *data = (XmlBlock*) user;

  // All we care about is content length. See if we need to resize our buffer.
  if (strncasecmp(ptr, "content-length:", size * nmemb) == 0)
  {
    data->length = atoi(ptr + 16);

    // Simplier than in the streaming case since we don't have any data yet.
    if (data->length > data->block_size) {
      int new_size = data->length + PADDING;
      char *new_block = (char *) malloc(new_size);
      data->block_size = new_size;
            
      free(data->block);
      data->start = data->end = data->block = new_block;
    }
  }
  
  return size * nmemb;
}


static size_t HandleReqData(char *ptr, size_t size, size_t nmemb, void *user)
{
  XmlBlock *data = (XmlBlock*) user;
  
  // Make sure the content length is correctly stated.
  if ((size_t) (data->end - data->block) + size * nmemb > data->block_size)
    return 0;

  memcpy(data->end, ptr, size * nmemb);
  data->end += size * nmemb;
  *(data->end) = '\0';
  
  return size * nmemb;
}

void *MTCWebRequest(const char *aUrl)
{
  XmlBlock *data = (XmlBlock*) calloc(1, sizeof(XmlBlock));
  data->block = calloc(1, INITIAL_BLOCK_SIZE);
  data->start = data->end = data->block;
  data->block_size = INITIAL_BLOCK_SIZE;
  
  data->context = curl_easy_init();
  
  //curl_easy_setopt(handle, CURLOPT_VERBOSE, 1);
  curl_easy_setopt(data->context, CURLOPT_URL, aUrl);
  curl_easy_setopt(data->context, CURLOPT_WRITEFUNCTION, HandleReqData);
  curl_easy_setopt(data->context, CURLOPT_WRITEDATA, data);
  curl_easy_setopt(data->context, CURLOPT_HEADERFUNCTION, HandleReqHeader);
  curl_easy_setopt(data->context, CURLOPT_WRITEHEADER, data);
  
  return data;
}

// Returns 0 for success. aBuffer is owned by aContext and will be freed by MTCWebFree.
int MTCWebExecute(void *aContext, const char **aBuffer)
{
  XmlBlock *data = (XmlBlock*) aContext;
  int res = curl_easy_perform(data->context);
  if (res == 0)
    *aBuffer = data->block;
  return res;
}

void MTCWebFree(void *aContext)
{
  MTCStreamFree(aContext);
}


