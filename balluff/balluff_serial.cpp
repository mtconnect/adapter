
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

char BalluffSerial::computeBCC(const string &aText)
{
  unsigned char bcc = 0;
  string::const_iterator i;
  for (i = aText.begin(); i != aText.end(); i++)
    bcc ^= *i;
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

int BalluffSerial::writeWithBCC(const string &aData, int aTimeout)
{
  string outgoing = aData + computeBCC(aData);
  int count = writeFully(outgoing.c_str(), outgoing.length(), aTimeout);
  // Remove one for the bcc byte we added.
  if (count > 0) count -= 1;
  return count;
}

int BalluffSerial::readWithBCC(char *aBuffer, int aLen, int aTimeout)
{
  int len = aLen + 1;
  int count = readFully(aBuffer, len, aTimeout);
  if (count < len)
    return count;
  
  char bcc = aBuffer[aLen];
  aBuffer[aLen] = '\0';
  if (bcc != computeBCC(aBuffer))
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
  char response[3];
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
  
  char response[3];
  count = readWithBCC(response, 1);
  if (count != 1 && response[0] != 'Q')
    return READ_ERROR;
  sleep(2);
  
  return SUCCESS;
}

BalluffSerial::EResult BalluffSerial::sendCommand(const char *aCommand)
{
  EResult res = SUCCESS;
  
  gLogger->debug("Sending: %s", aCommand);
  writeWithBCC(aCommand);
  
  char resp[2];
  int count = readFully(resp, 2, 30000);
  if (resp[0] != ACK || count != 2) 
  {
    gLogger->debug("Send command '%s' failed: %s", aCommand, translateError((EResult) resp[1]));
    res = (EResult)resp[1];
  }
  
  return res;
}

BalluffSerial::EResult BalluffSerial::checkForData(int &aHead, string &aText)
{
  aHead = 0;
  
  EResult res = sendCommand("H?");
  if (res != SUCCESS) return res;

  char reply[16];
  int count = readWithBCC(reply, 6);
  if (count < 6) return READ_ERROR;
  
  reply[6] = '\0';
  
  // May have framing error...
  if (reply[0] == 'H') {
    if (reply[1] != '?') {
      aHead = reply[1] - '0';
      aText = reply + 2;
    } else {
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

BalluffSerial::EResult BalluffSerial::readRFID(int aSize, std::string &aText)
{
  char buffer[8 * 1024];
  char command[32];
  
  sprintf(command, "R0012%04d", aSize);
  EResult res = sendCommand(command);
  if (res != SUCCESS)
    return res;
    
  print(STX);
  int count = readWithBCC(buffer, aSize, 30000);
  if (count < aSize)
  {
    gLogger->warning("Error reading, only got: %d", count);
    return READ_ERROR;
  }
  
  buffer[aSize] = '\0';  
  gLogger->debug("Received: %s", buffer);
  
  aText = buffer;
    
  return SUCCESS;
}


BalluffSerial::EResult BalluffSerial::readHeader(int &aSize, uint32_t &aHash)
{
  char buffer[16];
  char command[32];
  
  strcpy(command, "R00000012");
  EResult res = sendCommand(command);
  if (res != SUCCESS)
    return res;
  
  print(STX);
  int count = readWithBCC(buffer, 12);
  if (count < 12)
  {
    gLogger->warning("Error reading, only got: %d", count);
    return READ_ERROR;
  }
  
  buffer[12] = '\0';  
  gLogger->debug("Received: %s", buffer);

  char *ep;
  aHash = strtol(buffer + 4, &ep, 16);
  buffer[4] = '\0';
  aSize = atoi(buffer);
  
  return SUCCESS;
}

BalluffSerial::EResult BalluffSerial::writeRFID(std::string aKey, std::string aText)
{
  char buffer[4 * 1024];
  char command[32];
  
  int len = (int) aText.length();
  sprintf(buffer, "%c%04d%8s", STX, len, aKey.c_str());
  
  // Add 13 for the leading size <STX><SIZE><KEY>.
  len += 1 + 4 + 8;
  
  // Append the data after the size
  memcpy(buffer + 13, aText.c_str(), aText.length());
  buffer[len] = '\0';
  
  // Create the command (length - 1 (-1 for <STX>))
  sprintf(command, "W0000%04d", len - 1);
  EResult res = sendCommand(command);
  if (res != SUCCESS) return res;
  
  int written = writeWithBCC(buffer, 10000);
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





