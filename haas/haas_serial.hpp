
#ifndef HAAS_SERIAL
#define HAAS_SERIAL

#include "serial.hpp"
#include <vector>
#include <string>
#include <iostream>

class HaasSerial : public Serial {
public:
  HaasSerial(const char *aDevice,
	     int aBaud, const char *aParity, int aDataBit,
	     int aStopBit, bool aDebug = false);
  std::vector<std::string> *sendCommand(const char *aCommand);
  bool getVariable(int aVariableNumber, double &aResult);

protected:
  std::vector<std::string> *split(const char *aString);
};

#endif
