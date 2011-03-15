
#ifndef CONNECTONE_DEVICE_HPP
#define CONNECTONE_DEVICE_HPP

#include "XBee.h"
#include <string>
#include <map>
#include <time.h>
#include "device_datum.hpp"

class ConnectOneDevice {
public:
  const static int MAX_FRAMES = 16;
  
  ConnectOneDevice(std::string aName, XBeeAddress64 &aAddr) :
    mAddress(aAddr), mName(aName), mAvailable(false), mPrefix(false),
    mSilenceTimeout(20), mHonorTimestamp(false),
    mHeartBeat(2), mLastHeartbeat(0)
  {
    memset(mFrames, 0, sizeof(mFrames));
  }
  
  ~ConnectOneDevice() {
    std::map<std::string, Sample*>::iterator iter;
    for (iter = mDataItems.begin(); iter != mDataItems.end(); ++iter)
      delete iter->second;
  }

  bool hasFrame(uint8_t aId) {
    for (int i = 0; i < MAX_FRAMES; i++) {
      if (aId == mFrames[i])
	return true;
    }
    return false;
  }

  void addFrame(uint8_t aId)
  {
    memmove(mFrames, mFrames + 1, sizeof(mFrames) - 1);
    mFrames[MAX_FRAMES - 1] = aId; 
  }
    
  bool mAvailable;
  XBeeAddress64 mAddress;
  std::string   mName;
  bool          mPrefix;
  std::map<std::string, Sample*> mDataItems;
  time_t        mLastMsgTime;
  time_t        mLastHeartbeat;

  time_t        mSilenceTimeout;
  bool          mHonorTimestamp;
  int           mHeartBeat;

protected:
  uint8_t       mFrames[MAX_FRAMES];
};
  

#endif
