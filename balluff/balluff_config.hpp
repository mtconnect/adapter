//
//  balluff_config.hpp
//  balluff
//
//  Created by William Sobel on 10/20/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef balluff_balluff_config_hpp
#define balluff_balluff_config_hpp

#include "configuration.hpp"
#include <string>

class BalluffConfiguration : public Configuration {  
protected:
  std::string mSerialPort;
  std::string mAgentUrl;
  std::string mDeviceUuid;
  std::string mBaseAgentUrl;
  std::string mMyAgentUrl;
  std::string mMyBaseAgentUrl;
  std::string mParity;
  int mBaud;
  int mStopBits;
  int mDataBits;
  
protected:
  virtual void parse(YAML::Node &aDoc, int aPort, int aDelay, 
                     int aTimeout, const char *aService);
  std::string baseUrl(const std::string &aUrl)
  {
    std::string base = aUrl;
    size_t p = base.find("://");
    if (p != std::string::npos) {
      size_t n = base.find('/', p + 3);
      if (n != std::string::npos) base.erase(n + 1);
    }
    return base;
  }

public: 
  BalluffConfiguration();
  virtual ~BalluffConfiguration();
  
  virtual void parse(std::istream &aStream, int aPort = 7878, int aDelay = 100,
                     int aTimeout = 10000, const char *aService = "Balluff Adapter")
  { Configuration::parse(aStream, aPort, aDelay, aTimeout, aService); }
  
  std::string &getSerialPort() { return mSerialPort; }
  std::string &getAgentUrl() { return mAgentUrl; }
  std::string &getBaseAgentUrl() { return mBaseAgentUrl; }
  std::string &getMyAgentUrl() { return mBaseAgentUrl; }
  std::string &getMyBaseAgentUrl() { return mMyBaseAgentUrl; }
  std::string &getDeviceUuid() { return mDeviceUuid; }
  std::string &getParity() { return mParity; }
  int getBaud() { return mBaud; }
  int getStopBits() { return mStopBits; }
  int getDataBits() { return mDataBits; }

};

#endif
