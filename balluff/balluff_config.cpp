//
//  balluff_config.cpp
//  balluff
//
//  Created by William Sobel on 10/20/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include <yaml.h>

#include "balluff_config.hpp"

using namespace std;

BalluffConfiguration::BalluffConfiguration()
{
}

BalluffConfiguration::~BalluffConfiguration()
{
}

void BalluffConfiguration::parse(YAML::Node &aDoc, int aPort, int aDelay, int aTimeout, const char *aService)
{
  Configuration::parse(aDoc, aPort, aDelay, aTimeout, aService);
  
  const YAML::Node &connection = aDoc["connection"];
  connection["deviceUuid"] >> mDeviceUuid;
  connection["serialPort"] >> mSerialPort;
  connection["agentUrl"] >> mAgentUrl;
  
  SET_WITH_DEFAULT(connection, "baud", mBaud, 9600);
  SET_WITH_DEFAULT(connection, "stopBits", mStopBits, 1);
  SET_WITH_DEFAULT(connection, "dataBits", mDataBits, 8);
  SET_WITH_DEFAULT(connection, "parity", mParity, "none");

  mBaseAgentUrl = mAgentUrl;
  size_t p = mBaseAgentUrl.find("://");
  if (p != string::npos) {
    size_t n = mBaseAgentUrl.find('/', p + 3);
    if (n != string::npos) mBaseAgentUrl.erase(n + 1);
  }
}