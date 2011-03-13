
#ifndef CONNECTONE_DEVICE_HPP
#define CONNECTONE_DEVICE_HPP

#include "XBee.h"
#include <string>
#include <map>
#include <time.h>
#include "device_datum.hpp"

class ConnectOneDevice {
public:
  ConnectOneDevice(std::string aName, XBeeAddress64 &aAddr) :
    mAddress(aAddr), mName(aName), mAvailable(false), mPrefix(false) { }
  ~ConnectOneDevice() {
    std::map<std::string, Sample*>::iterator iter;
    for (iter = mDataItems.begin(); iter != mDataItems.end(); ++iter)
      delete iter->second;
  }
    
  bool mAvailable;
  XBeeAddress64 mAddress;
  std::string   mName;
  bool          mPrefix;
  std::map<std::string, Sample*> mDataItems;
  time_t       mLastMsgTime;
};
  

#endif
