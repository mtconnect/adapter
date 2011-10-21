//
//  main.c
//  cstream
//
//  Created by William Sobel on 10/17/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <fstream>
#include <sstream>
#include <internal.hpp>
#include <stdio.h>
#include <string.h>
#include "stream.h"
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <b64/cencode.h>
#include <b64/cdecode.h>
#include <zlib.h>
#include "balluff_adapter.hpp"
#include "balluff_serial.hpp"

using namespace std;

#define CHUNK 16384

int BalluffAdapter::HandleXmlChunk(const char *xml, void *aObj)
{
  BalluffAdapter *adapter = (BalluffAdapter*) aObj;
  
  xmlDocPtr document;
  const char *path;
  xmlXPathContextPtr xpathCtx;
  xmlNodePtr root;
  xmlXPathObjectPtr nodes;
  xmlNodeSetPtr nodeset;
  int i;
  
  document = xmlReadDoc(BAD_CAST xml, "file://node.xml",
                        NULL, XML_PARSE_NOBLANKS);
  if (document == NULL) 
  {
    gLogger->error("Cannot parse document: %s\n", xml);
    xmlFreeDoc(document);
    return 0;
  }
  
  path = "//m:Events/*";
  xpathCtx = xmlXPathNewContext(document);
  
  root = xmlDocGetRootElement(document);
  if (root->ns != NULL)
  {
    xmlXPathRegisterNs(xpathCtx, BAD_CAST "m", root->ns->href);
  }
  else
  {
    gLogger->error("Document does not have a namespace: %s\n", xml);
    xmlFreeDoc(document);
    return 0;
  }
  
  // Evaluate the xpath.
  nodes = xmlXPathEval(BAD_CAST path, xpathCtx);
  if (nodes == NULL || nodes->nodesetval == NULL)
  {
    printf("No nodes found matching XPath\n");
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(document);
    return 1;
  }
  
  // Spin through all the events, samples and conditions.
  nodeset = nodes->nodesetval;
  for (i = 0; i != nodeset->nodeNr; ++i)
  {
    xmlNodePtr n = nodeset->nodeTab[i];
    xmlChar *name = xmlGetProp(n, BAD_CAST "name");
    xmlChar *value;
    
    if (name == NULL)
      name = xmlGetProp(n, BAD_CAST "dataItemId");
    value = xmlNodeGetContent(n);
    
    if (xmlStrcmp(n->name, BAD_CAST "AssetChanged") == 0 && 
        xmlStrcmp(value, BAD_CAST "UNAVAILABLE") != 0) {
      adapter->sendAssetToAgent((const char*) value);
    }    
    xmlFree(value);
    xmlFree(name);
  }
  
  xmlXPathFreeObject(nodes);    
  xmlXPathFreeContext(xpathCtx);
  xmlFreeDoc(document);
  
  return 1;
}

void BalluffAdapter::sendAssetToAgent(const char *aId)
{
  // Get the XML for the asset from the server.
  string str = mBaseUrl + "assets/" + aId;
  void *context = MTCWebRequest(str.c_str());
  const char *result;
  if (MTCWebExecute(context, &result) == 0) {
    map<string,string> attrs = getAttributes(result, "//m:CuttingTool");
    
    if (attrs.count("deviceUuid") > 0 && mDevice != attrs["deviceUuid"]) {
      char *encoded = encodeResult(result);
      mOutgoing = encoded;
      mOutgoingId = aId;
      addAsset(aId, attrs["element"].c_str(), result); 
    }
  }
  MTCWebFree(context);
}

map<string,string> BalluffAdapter::getAttributes(const string &aXml, 
                                                const string &aXPath)
{
  std::map<std::string,std::string> res;
  
  xmlDocPtr document = xmlReadDoc(BAD_CAST aXml.c_str(), "file://node.xml",
                                    NULL, XML_PARSE_NOBLANKS);
  if (document == NULL) {
    gLogger->error("Cannot parse document: %s\n", aXml.c_str());
  } else {
    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(document);
    
    xmlNodePtr root = xmlDocGetRootElement(document);
    if (root->ns != NULL)
    {
      xmlXPathRegisterNs(xpathCtx, BAD_CAST "m", root->ns->href);
      
      xmlXPathObjectPtr nodes;
      xmlNodeSetPtr nodeset;
      
      // Evaluate the xpath.
      nodes = xmlXPathEval(BAD_CAST aXPath.c_str(), xpathCtx);
      if (nodes == NULL || nodes->nodesetval == NULL)
      {
        printf("No nodes found matching XPath\n");
      } else {
        // Spin through all the events, samples and conditions.
        nodeset = nodes->nodesetval;
        xmlNodePtr node = nodeset->nodeTab[0];
        
        res["element"] = (const char*) (node->name);
        for (xmlAttrPtr attr = node->properties; attr != NULL; attr = attr->next)
        {
          if (attr->type == XML_ATTRIBUTE_NODE) {
            res[(const char *) attr->name] = (const char*) attr->children->content;
          }
        }
        
        xmlXPathFreeObject(nodes);
      }
      
      xmlXPathFreeContext(xpathCtx);
    }
    else
    {
      gLogger->error("Document does not have a namespace: %s\n", aXml.c_str());
    }
  }

  if (document != NULL)    
    xmlFreeDoc(document);

  return res;
}

static void StreamXMLErrorFunc(void *ctx ATTRIBUTE_UNUSED, const char *msg, ...)
{
  va_list args;
  char buffer[2048];
  va_start(args, msg);
  vsnprintf(buffer, 2046, msg, args);
  buffer[2047] = '0';
  va_end(args);
  
  gLogger->error("XML Error: %s\n", buffer);   
}

char *BalluffAdapter::encodeResult(const char *aData)
{
  char *encoded;
  base64_encodestate state;
  int ret;
  unsigned char *out;
  uLongf len;

  len = compressBound(strlen(aData)) + sizeof(uint32_t);
  out = (unsigned char*) malloc(len);
  *((uint32_t*) out) = htonl(strlen(aData));
  
  len -= sizeof(uint32_t);

  ret = compress2(out + sizeof(uint32_t), &len, 
                  (Bytef*) aData, strlen(aData), 9);
  if (ret != Z_OK)
  {
    free(out);
    return NULL;
  }
    
  encoded = (char*) calloc(1, (sizeof(uint32_t) + len) * 2);
  
  base64_init_encodestate(&state);
  base64_encode_block((char*) out, len + sizeof(uint32_t), encoded, &state);
  base64_encode_blockend(encoded + strlen(encoded), &state);
  printf("encoded: \n%s\n", encoded); 
  
  free(out);
  
  return encoded;
}

char *BalluffAdapter::reconstitute(const char *aText)
{
  char *decoded;
  base64_decodestate state;
  int ret, len;
  unsigned char *xml;
  uLongf size;
  
  decoded = (char*) malloc(strlen(aText));
  
  base64_init_decodestate(&state);
  len = base64_decode_block(aText, strlen(aText), decoded, &state);

  size = ntohl(*((uint32_t*) decoded)) + 1;
  xml = (unsigned char*) malloc(size);

  ret = uncompress(xml, &size, (Bytef*) decoded + sizeof(uint32_t), len - 4);
  free(decoded);
  
  if (ret != Z_OK)
  {
    free(xml);
    return NULL;
  }

  xml[size] = '\0';
  return (char*) xml;
}

BalluffAdapter::BalluffAdapter(int aPort)
  : Adapter(aPort, 1000), mAvailability("avail")
{
  addDatum(mAvailability);
}

BalluffAdapter::~BalluffAdapter()
{
  delete mSerial;
}

void BalluffAdapter::initialize(int aArgc, const char *aArgv[])
{
  MTConnectService::initialize(aArgc, aArgv);
  
  string file = "adapter.yaml";
  if (aArgc > 0)
  {
    file = aArgv[0];
  }
  
  ifstream str(file.c_str());
  mConfiguration.parse(str);
  
  mDevice = mConfiguration.getDeviceUuid();
  mUrl = mConfiguration.getAgentUrl();
  mBaseUrl = mConfiguration.getBaseAgentUrl();
    
  mPort = mConfiguration.getPort();
  mScanDelay = mConfiguration.getScanDelay();
  mHeartbeatFrequency = mConfiguration.getTimeout();

  mSerial = new BalluffSerial(mConfiguration.getSerialPort().c_str(),
                              mConfiguration.getBaud(),
                              mConfiguration.getParity().c_str(), 
                              mConfiguration.getDataBits(),
                              mConfiguration.getStopBits());

  gLogger->setLogLevel(Logger::eDEBUG);
}

void *BalluffAdapter::AgentMonitor(void *anAdapter)
{
  ((BalluffAdapter*) anAdapter)->agentMonitor();
  static int res = 0;
  return &res;
}

void BalluffAdapter::startAgentMonitor()
{
  int res = pthread_create(&mServerThread, NULL, AgentMonitor, this);
  if (res != 0) {
    gLogger->error("Cannot create server thread");
    exit(1);
  }
}

void BalluffAdapter::agentMonitor()
{
  xmlInitParser();
  xmlXPathInit();
  xmlSetGenericErrorFunc(NULL, StreamXMLErrorFunc);

  string url = mUrl;
  if (url[url.length() - 1] != '/') url.append("/");
  url.append("sample?interval=10&path=//DataItem[@type=\"ASSET_CHANGED\"]");
  void *context = MTCStreamInit(url.c_str(), HandleXmlChunk, this);
  MTCStreamStart(context);
  MTCStreamFree(context);
}


void BalluffAdapter::start()
{
  // Start agent heartbeat thread...
  startServerThread();
  
  startAgentMonitor();
  
  // Connect serial connection.
  mSerial->connect();
  mSerial->flush();
  
  begin();
  mBuffer.reset();
  mBuffer.timestamp();
  
  mAvailability.available();
  
  sendChangedData();
  cleanup();

  // Lets try some simple communication...
  while (mSerial->selectHead(1) != BalluffSerial::SUCCESS) {
    mSerial->flush();
    sleepMs(1000);
  }

  bool readData = false;
  while (true)
  {
    mSerial->flush();

    int head, size;
    BalluffSerial::EResult res = mSerial->checkForData(head, size);
    if (res == BalluffSerial::SUCCESS)
    {
      cout << "Successfully read: " << head << endl;
      
      if (head != 0)
      {
        if (size > 0 && !readData) {
          string incoming;
          res = mSerial->readRFID(head, size, incoming);
          cout << "Read: " << incoming << endl;
          if (res == BalluffSerial::SUCCESS) {
            char *decode = reconstitute(incoming.c_str());
            if (decode != NULL && decode[0] == '<') {
            // Check for XML
              map<string,string> attrs = getAttributes(decode, "//m:CuttingTool");
              if (attrs.count("assetId") > 0)
                addAsset(attrs["assetId"].c_str(), attrs["element"].c_str(), decode);
              else
                gLogger->warning("Could not find asset id in document");
            } else if (strncmp(incoming.c_str(), "http://", 7) == 0 && 
                       strncmp(incoming.c_str(), mBaseUrl.c_str(), mBaseUrl.length()) != 0) {
              // We have a URI and the uri is different from our own...
              sendAssetToAgent(incoming.c_str());
            } else {
              // Whatever else we will just assume is an ID...
            }
            if (decode != NULL)
              free(decode);
            readData = true;
          }          
        }
        
        if (!mOutgoing.empty() && (readData || size == 0)) {
          res= mSerial->writeRFID(head, mOutgoing);
          if (res == BalluffSerial::SUCCESS)
          {
            cout << "Succussfully wrote: " << mOutgoing << endl;
            mOutgoing.clear();
          } else {
            // We may not have had enough room...
            
            mOutgoing = mBaseUrl + "assets/" + mOutgoingId;
            res= mSerial->writeRFID(head, mOutgoing);
            if (res == BalluffSerial::SUCCESS)
            {
              cout << "Succussfully wrote: " << mOutgoing << endl;
              mOutgoing.clear();
            }
          }
        }
      } else {
        readData = false;
      }
    }
    sleepMs(1000);
  }    
}

void BalluffAdapter::stop()
{
  stopServer();
}

void BalluffAdapter::gatherDeviceData()
{
}

