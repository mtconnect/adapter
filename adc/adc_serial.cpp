
#include "internal.hpp"
#include "adc_serial.hpp"

using namespace std;

AdcSerial::AdcSerial(const char *aDevice,
	     int aBaud, const char *aParity, int aDataBit,
	     int aStopBit, bool aDebug)
  : Serial(aDevice, aBaud, aParity, aDataBit, aStopBit, aDebug)
{
}

string AdcSerial::sendCommand(const char *aCommand)
{
  // Wait a small amount of time.
  usleep(20 * 1000); // 20 msec
  
  char buffer[1024];
  sprintf(buffer, "%s\r", aCommand);
  write(buffer);

  memset(buffer, 0, sizeof(buffer));
  if (readUntil("\r", buffer, sizeof(buffer)) <= 0)
    return string("");

  return string(buffer);
}




