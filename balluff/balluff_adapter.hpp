
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
  std::string mMyBaseUrl;
  std::string mDevice;

  // Outgoing info...
  std::string mOutgoing;
  std::string mOutgoingId;
  std::string mOutgoingTimestamp;
  bool mForceOverwrite;
  
  // Incoming info
  std::string mCurrentAssetId;
  std::string mCurrentAssetTimestamp;
  
  // Current asset checksum
  uint32_t mCurrentHash;
  bool mHasHash;
  
  BalluffConfiguration mConfiguration;
  
protected:
  enum EChanged {
    SAME,
    CHANGED,
    ERROR
  };
  
  void startAgentMonitor();
  char *reconstitute(const char *aData);
  char *encodeResult(const char *aData);
  std::string getAsset(std::string &aUrl, const char *aId);
  EChanged hasAssetChanged(std::map<std::string,std::string> &aAttributes);
                    
  static int HandleXmlChunk(const char *xml, void *aObj);
  
  void sendAssetToAgent(const char *aId);
  
  static void *AgentMonitor(void *anAdapter);
  void agentMonitor();
  
  std::map<std::string,std::string> getAttributes(const std::string &aXml, 
                                                   const std::string &aXPath);
  
  bool checkForDataCarrier();
  bool checkForNewAsset(int &aSize, uint32_t &aHash);
  bool readAssetFromRFID(int aSize, uint32_t aHash);
  bool checkNewOutgoingAsset(uint32_t &aHash);
  
  bool writeAssetToRFID(uint32_t aHash);
  
  uint32_t computeHash(const std::string &aKey);

public:
  BalluffAdapter(int aPort);
  ~BalluffAdapter();
  
  virtual void initialize(int aArgc, const char *aArgv[]);
  virtual void start();
  virtual void stop();
  virtual void gatherDeviceData();
};

#endif

