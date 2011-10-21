//
//  stream.h
//  cstream
//
//  Created by William Sobel on 10/17/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef cstream_stream_h
#define cstream_stream_h


#ifdef  __cplusplus
extern "C" {
#endif
  
#if 0
}
#endif
  
/* Return 1 if successful, 0 to stop. */
typedef int (*MTCStreamXMLHandler)(const char *aXML, void *aUserData);
  
void *MTCStreamInit(const char *aUrl, MTCStreamXMLHandler aHandler, void *aUserData);

int MTCStreamStart(void *aContext);

void MTCStreamStop(void *aContext);

void MTCStreamFree(void *aContext);

void *MTCWebRequest(const char *aUrl);

// Returns 0 for success. aBuffer is owned by aContext and will be freed by MTCWebFree.
int MTCWebExecute(void *aContext, const char **aBuffer);

void MTCWebFree(void *AContext);

  
#ifdef  __cplusplus
}
#endif

#endif
