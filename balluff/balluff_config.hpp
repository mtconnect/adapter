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
public: 
  BalluffConfiguration();
  virtual ~BalluffConfiguration();
  
  virtual void parse(std::istream &aStream, int aPort = 7878, int aDelay = 100,
                     int aTimeout = 10000, const char *aService = "Balluff Adapter")
  { Configuration::parse(aStream, aPort, aDelay, aTimeout, aService); }
  
  std::string &getSerialPort() { return mSerialPort; }
  std::string &getAgentUrl() { return mAgentUrl; }
  std::string &getBaseAgentUrl() { return mBaseAgentUrl; }
  std::string &getDeviceUuid() { return mDeviceUuid; }
  std::string &getParity() { return mParity; }
  int getBaud() { return mBaud; }
  int getStopBits() { return mStopBits; }
  int getDataBits() { return mDataBits; }
  
protected:
  virtual void parse(YAML::Node &aDoc, int aPort, int aDelay, 
                     int aTimeout, const char *aService);

protected:
  std::string mSerialPort;
  std::string mAgentUrl;
  std::string mDeviceUuid;
  std::string mBaseAgentUrl;
  std::string mParity;
  int mBaud;
  int mStopBits;
  int mDataBits;
};

#endif
