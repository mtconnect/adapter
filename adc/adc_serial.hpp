
#ifndef ADC_SERIAL
#define ADC_SERIAL

#include "serial.hpp"
#include <vector>
#include <string>
#include <iostream>

class AdcSerial : public Serial {
public:
  AdcSerial(const char *aDevice,
	     int aBaud, const char *aParity, int aDataBit,
	     int aStopBit, bool aDebug = false);
  std::string sendCommand(const char *aCommand);
};

#endif
