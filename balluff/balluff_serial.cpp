
#include "internal.hpp"
#include "balluff_serial.hpp"
#include "logger.hpp"

using namespace std;

const char ACK = 0x06;
const char NAK = 0x15;
const char STX = 0x02;
const char SOH = 0x01;

BalluffSerial::BalluffSerial(const char *aDevice,
	     int aBaud, const char *aParity, int aDataBit,
	     int aStopBit, bool aDebug)
  : Serial(aDevice, aBaud, aParity, aDataBit, aStopBit)
{
}

char BalluffSerial::computeBCC(const uint8_t *aText, uint16_t aSize)
{
  unsigned char bcc = 0;
  uint16_t i;
  for (i = 0; i < aSize; i++)
    bcc ^= aText[i];
  return (char) bcc;
}

const char *BalluffSerial::translateError(BalluffSerial::EResult aError)
{
  switch (aError) {
    case SUCCESS: return "SUCCESS";
    case NO_DATA_CARRIER: return "NO_DATA_CARRIER";
    case READ_ERROR: return "READ_ERROR";
    case READ_ABORTED: return "READ_ABORTED";
    case WRITE_ERROR: return "WRITE_ERROR";
    case WRITE_ABORTED: return "WRITE_ABORTED";
    case INTERFACE_ERROR: return "INTERFACE_ERROR";
    case TELEGRAM_FORMAT_ERROR: return "TELEGRAM_FORMAT_ERROR";
    case BCC_ERROR: return "BCC_ERROR";
    case CABLE_BREAK: return "CABLE_BREAK";
    case READ_IN_PROGRESS: return "READ_IN_PROGRESS";
    case WRITE_IN_PROGRESS: return "WRITE_IN_PROGRESS";
    case HEAD_SELECT_IN_PROGRESS: return "HEAD_SELECT_IN_PROGRESS";
    case CRC_ERROR: return "CRC_ERROR";
    case EEPROM_ERROR: return "EEPROM_ERROR";
  }
  return "Unknown Result";
}

int BalluffSerial::writeWithBCC(const uint8_t *aData, uint16_t aSize, int aTimeout)
{
  uint8_t outgoing[MAX_RFID_SIZE];
  memcpy(outgoing, aData, aSize);
  outgoing[aSize] = computeBCC(aData, aSize);
  aSize++;
  
  int count = writeFully((const char*) outgoing, aSize, aTimeout);
  // Remove one for the bcc byte we added.
  if (count > 0) count -= 1;
  return count;
}

int BalluffSerial::readWithBCC(uint8_t *aBuffer, uint16_t aLen, int aTimeout)
{
  int len = aLen + 1;
  int count = readFully((char*) aBuffer, len, aTimeout);
  if (count < len)
    return count;
  
  char bcc = aBuffer[aLen];
  if (bcc != computeBCC(aBuffer, aLen))
    return -1;
  
  // Remove one for the bcc
  return count - 1;
}

BalluffSerial::EResult BalluffSerial::selectHead(int head)
{
  char buffer[32];
  sprintf(buffer, "H%d", head);
  return sendCommand(buffer);
}

BalluffSerial::EResult BalluffSerial::getStatus(char &aStatus)
{
  char command[3];
  command[0] = 'S';
  command[1] = 'S';
  command[2] = '\0';
  
  int count = writeFully(command, 2);
  // Remove one for the bcc byte we added.
  if (count != 2) return READ_ERROR;

  EResult res = SUCCESS;
  uint8_t response[3];
  count = readWithBCC(response, 2);
  if (count == 2 && response[0] == 'S')
    aStatus = response[1];
  else
    res = READ_ERROR;
  
  return res;
}

BalluffSerial::EResult BalluffSerial::reset()
{
  char command[3];
  command[0] = 'Q';
  command[1] = 'Q';
  command[2] = '\0';
  
  int count = writeFully(command, 2);
  // Remove one for the bcc byte we added.
  if (count != 2) return READ_ERROR;
  
  uint8_t response[3];
  count = readWithBCC(response, 1);
  if (count != 1 && response[0] != 'Q')
    return READ_ERROR;
  sleep(2);
  
  return SUCCESS;
}

BalluffSerial::EResult BalluffSerial::sendCommand(const char *aCommand, int aTimeout)
{
  EResult res = SUCCESS;
  
  gLogger->debug("Sending: %s", aCommand);
  writeWithBCC((const uint8_t*) aCommand, strlen(aCommand));
  
  uint8_t resp[2];
  int count = readFully((char*) resp, 2, aTimeout);
  if (resp[0] != ACK || count != 2) 
  {
    gLogger->debug("Send command '%s' failed: %s", aCommand, translateError((EResult) resp[1]));
    res = (EResult)resp[1];
  }
  
  return res;
}

BalluffSerial::EResult BalluffSerial::checkForData(int &aHead, uint32_t &aText)
{
  aHead = 0;
  int times = 0;
  int count;  
  uint8_t reply[16];
  EResult res;
  do {
    res = sendCommand("H?");
    if (res != SUCCESS) return res;
    count = readWithBCC(reply, 6, 3000);
    if (count < 6) {
      times++;
      gLogger->debug("Read of header content failed: %d times", times);
      flush();
      wait(200);
    }
  } while (times < 2 && count < 6);
  if (count < 6) return READ_ERROR;
  
  reply[6] = '\0';
  
  // May have framing error...
  if (reply[0] == 'H') {
    if (reply[1] != '?') {
      aHead = reply[1] - '0';
      uint32_t hash;
      memcpy(&hash, reply + 2, sizeof(hash));
      aText = ntohl(hash);
      gLogger->debug("RFID responded with %d and %d", aHead, hash);
    } else {
      gLogger->debug("RFID responded with %c", reply[1]);
      aHead = 0;
    }
    
    return SUCCESS;
  }
  else
  {
    gLogger->debug("Looks like a framing error");
    return READ_ERROR;
  }
}

BalluffSerial::EResult BalluffSerial::readRFID(uint8_t *aText, uint16_t aSize)
{
  char command[32];
  
  sprintf(command, "R0007%04d", aSize);
  EResult res = sendCommand(command, 30000);
  if (res != SUCCESS)
    return res;
    
  print(STX);
  int count = readWithBCC(aText, aSize, 10000);
  if (count < aSize)
  {
    gLogger->warning("Error reading, only got: %d", count);
    return READ_ERROR;
  }
  
  //gLogger->debug("Received: %s", aText);
  
  return SUCCESS;
}


BalluffSerial::EResult BalluffSerial::readHeader(uint16_t &aSize, uint8_t &aType)
{
  uint8_t buffer[16];
  char command[32];
  
  strcpy(command, "R00040003");
  EResult res = sendCommand(command);
  if (res != SUCCESS)
    return res;
  
  print(STX);
  int count = readWithBCC(buffer, 3);
  if (count < 3)
  {
    gLogger->warning("Error reading, only got: %d", count);
    return READ_ERROR;
  }
  
  uint16_t size;
  memcpy(&size, buffer, sizeof(size));
  aSize = ntohs(size);
  aType = buffer[2];

  gLogger->debug("Header: size: %d type: %c", aSize, aType);
  
  return SUCCESS;
}

BalluffSerial::EResult BalluffSerial::writeRFID(uint32_t aKey, uint8_t aType, 
                                                const uint8_t *aText, uint16_t aLen)
{
  uint8_t buffer[MAX_RFID_SIZE];
  char command[32];
  
  buffer[0] = STX;
  
  uint32_t kno = htonl(aKey);
  uint16_t lno = htons(aLen);
  memcpy(buffer + 1, &kno, sizeof(kno));
  memcpy(buffer + 5, &lno, sizeof(lno));
  buffer[7] = aType;

  // Add 13 for the leading size <STX><KEY><SIZE><TYPE>.
  uint16_t offset = 1 + 2 + 4 + 1;
  uint16_t len = aLen + offset;

  
  // Append the data after the size
  memcpy(buffer + offset, aText, aLen);  

  // Remove one for <STX>
  sprintf(command, "W0000%04d", len - 1);
  EResult res = sendCommand(command);
  if (res != SUCCESS) return res;
  
  int written = writeWithBCC(buffer, len, 10000);
  if (written != len)
    res = WRITE_ERROR;
  
  char resp[2];
  int count = readFully(resp, 2, 20000);
  if (resp[0] != ACK || count != 2)
  {
    gLogger->warning("Write failed: %s", translateError((EResult) resp[1]));
    res = (EResult) resp[1];
  }

  return res;
}





