
#ifndef BALLUFF_SERIAL
#define BALLUFF_SERIAL

#include "serial.hpp"
#include <vector>
#include <string>
#include <iostream>

class BalluffSerial : public Serial {
public:
  enum EResult {
    SUCCESS = '0',
    NO_DATA_CARRIER = '1',
    READ_ERROR = '2',
    READ_ABORTED = '3',
    WRITE_ERROR = '4',
    WRITE_ABORTED = '5',
    INTERFACE_ERROR = '6',
    TELEGRAM_FORMAT_ERROR = '7',
    BCC_ERROR = '8',
    CABLE_BREAK = '9',
    READ_IN_PROGRESS = 'A',
    WRITE_IN_PROGRESS = 'B',
    HEAD_SELECT_IN_PROGRESS = 'C',
    CRC_ERROR = 'E',
    EEPROM_ERROR = 'I'
  };

protected:
  char computeBCC(const std::string &aText);
  int writeWithBCC(const std::string &aData, int aTimeout = 1000);
  int readWithBCC(char *aBuffer, int aLen, int aTimeout = 1000);
  EResult sendCommand(const char *aCommand);
  
public:  
  BalluffSerial(const char *aDevice,
	     int aBaud = 9600, const char *aParity = "none", int aDataBit = 8,
	     int aStopBit = 1, bool aDebug = false);
  const char *translateError(EResult aError);

  EResult getStatus(char &aStatus);
  EResult selectHead(int aHead);
  EResult readRFID(int aSize, std::string &aText);
  EResult readHeader(int &aSize, uint32_t &aHash);
  EResult writeRFID(std::string aKey, std::string aText);
  EResult checkForData(int &aHead, std::string &aLead);
  EResult reset();
  
  
};

#endif
