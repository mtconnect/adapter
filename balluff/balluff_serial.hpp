
#ifndef BALLUFF_SERIAL
#define BALLUFF_SERIAL

#include "serial.hpp"
#include <vector>
#include <string>
#include <iostream>

#define MAX_RFID_SIZE 8192

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
  char computeBCC(const  uint8_t *aText, uint16_t aSize);
  int writeWithBCC(const  uint8_t *aData, uint16_t aSize, int aTimeout = 1000);
  int readWithBCC(uint8_t *aBuffer, uint16_t aLen, int aTimeout = 1000);
  EResult sendCommand(const char *aCommand, int aTimeout = 5000);
  
public:  
  BalluffSerial(const char *aDevice,
	     int aBaud = 9600, const char *aParity = "none", int aDataBit = 8,
	     int aStopBit = 1, bool aDebug = false);
  const char *translateError(EResult aError);

  EResult getStatus(char &aStatus);
  EResult selectHead(int aHead);
  EResult readRFID(uint8_t *aText, uint16_t aSize);
  EResult readHeader(uint16_t &aSize, uint32_t &aHash);
  EResult writeRFID(uint32_t aKey, const uint8_t *aText, uint16_t aLen);
  EResult checkForData(int &aHead, uint32_t &aLead);
  EResult reset();
  
  
};

#endif
