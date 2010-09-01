
#ifndef KRISHNA_METER_HPP
#define KRISHNA_METER_HPP

#include "XBee.h"
#include <string>

class KrishnaMeter {
public:
  KrishnaMeter(std::string aName, XBeeAddress64 &aAddr) :
    mAddress(aAddr), mName(aName), mAvailable(false) { }
  
  bool mAvailable;
  XBeeAddress64 mAddress;
  std::string   mName;
  
  std::vector<KrishnaData*> mData;
};
  

#endif
