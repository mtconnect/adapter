
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
  typedef std::map<std::string,std::string> Attributes;
  
  /* Define all the data values here */
  
  /* Events */
  Availability mAvailability; 
  Event        mTool;
  
  BalluffSerial *mSerial;
  std::string mUrl;
  std::string mBaseUrl;
  std::string mMyBaseUrl;
  std::string mDevice;

  // Outgoing info...
  std::string mOutgoingId;
  uint32_t mOutgoingHash;
  bool mOutgoingIsNew;
  
  // Incoming info
  std::string mCurrentDeviceUuid;
  std::string mCurrentAssetId;
  std::string mCurrentAssetTimestamp;
  std::string mLastAssetId;
  uint8_t mCurrentType;
  std::string mNextSequence;
  
  // Current asset checksum
  uint32_t mCurrentHash;
  bool mHasRead;
      
  BalluffConfiguration mConfiguration;
  
protected:
  enum EChanged {
    SAME,
    OLDER,
    NEWER,
    ERROR
  };
  
  void startAgentMonitor();
  char *reconstitute(const uint8_t *aData, uint16_t aSize);
  uint16_t compressResult(const char *aData, uint8_t *aEncoded);
  std::string getAsset(std::string &aUrl, const char *aId);
  std::string getContent(std::string &aUrl);
  EChanged hasAssetChanged(Attributes &aRFIDAttributes,
                           Attributes &aAgentAttributes);
                    
  static int HandleXmlChunk(const char *xml, void *aObj);
  
  void sendAssetToAgent(const char *aId, std::string &aUuid);
  
  static void *AgentMonitor(void *anAdapter);
  void agentMonitor();
  
  Attributes getAttributes(const std::string &aXml, 
                           const std::string &aXPath);
  
  bool checkForDataCarrier(uint32_t &aHash);
  bool checkForNewAsset(uint32_t aHash);
  bool checkNewOutgoingAsset();
  bool readAssetFromRFID(uint32_t aHash);
  void updateAssetFromRFID(uint32_t aHash, const char *aXml);
  
  bool writeAssetToRFID();
  
  uint32_t computeHash(const std::string &aKey);
  
  // Progress indicator
  enum EIndicator {
    IDLE,
    WORKING,
    SUCCESS,
    FAIL
  };
  void indicator(EIndicator aIndicator);

public:
  BalluffAdapter(int aPort);
  ~BalluffAdapter();
  
  virtual void initialize(int aArgc, const char *aArgv[]);
  virtual void start();
  virtual void stop();
  virtual void gatherDeviceData();
};

#endif

