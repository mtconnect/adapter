
#ifndef BALLUFF_ADAPTER_HPP
#define BALLUFF_ADAPTER_HPP

#include "adapter.hpp"
#include "device_datum.hpp"
#include "service.hpp"
#include "condition.hpp"
#include <sys/time.h>
#include <string>
#include <map>
#include "stream.h"
#include "balluff_serial.hpp"
#include "balluff_config.hpp"

class BalluffAdapter : public Adapter, public MTConnectService
{
protected:
  /* Define all the data values here */
  
  /* Events */
  Availability mAvailability; 
  BalluffSerial *mSerial;
  std::string mUrl;
  std::string mBaseUrl;
  std::string mDevice;
  
  std::string mOutgoing;
  std::string mOutgoingId;
  
  BalluffConfiguration mConfiguration;
  
protected:
  void startAgentMonitor();
  char *reconstitute(const char *aData);
  char *encodeResult(const char *aData);

  static int HandleXmlChunk(const char *xml, void *aObj);
  
  void sendAssetToAgent(const char *aId);
  
  static void *AgentMonitor(void *anAdapter);
  void agentMonitor();
  
  std::map<std::string,std::string> getAttributes(const std::string &aXml, 
                                                   const std::string &aXPath);

public:
  BalluffAdapter(int aPort);
  ~BalluffAdapter();
  
  virtual void initialize(int aArgc, const char *aArgv[]);
  virtual void start();
  virtual void stop();
  virtual void gatherDeviceData();
};

#endif

