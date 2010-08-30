
#include "internal.hpp"
#include "serial.hpp"

Serial::SerialError::SerialError(const char *aMessage)
{
  strncpy(mMessage, aMessage, 1023);
  mMessage[1023] = '\0';
}

Serial::Serial(const char *aDevice,
	       int aBaud, const char *aParity, int aDataBit,
	       int aStopBit, bool aDebug)
{
  mBaud = aBaud;
  strncpy(mDevice, aDevice, sizeof(mDevice) - 1);
  mParity[sizeof(mDevice) - 1] = '\0';
  strncpy(mParity, aParity, sizeof(mParity) - 1);
  mParity[sizeof(mParity) - 1] = '\0';
  mDataBit = aDataBit;
  mStopBit = aStopBit;
  mDebug = aDebug;

#ifdef WIN32
  mFd = INVALID_HANDLE_VALUE;
#else
  mFd = -1;
#endif

  mConnected = false;
}

Serial::~Serial()
{
  disconnect();
}

int Serial::readUntil(const char *aUntil, char *aBuffer, int aLength)
{
  if (!mConnected)
  {
    printf("Trying to read when not connected");
    return -1;
  }
    
  if (mDebug) 
    printf("Reading upto %d bytes or we get a match\n", aLength, aUntil);

  int len = 0, count = 0;
  char *cp = aBuffer;
  const char *match = aUntil;
  do
  {
    int ret = read(cp, 1);
    if (ret == -1)
    {
      throw SerialError("Couldn't read");
    }
    if (ret == 0)
    {
      usleep(10 * 1000); // 10 msec
      if (count++ > 10)
      {
	printf("Read timed out\n");
	return -1;
      }
    }
    else
    {
      count = 0;
      
      //printf("Received: %d == match: %d\n", *cp, *match); 
      if (*match == *cp)
	match++;
      else
	match = aUntil;

      // See if we can match the beginnig of the string again.
      if (*match == *cp)
	match++;
      //printf("Match now: %d\n", *match);
      cp++;
      len++;
    }
  } while (len <= aLength && *match != '\0');

  *cp = '\0';

  if (mDebug) 
    printf("Read returned: %d - '%s'\n", len, aBuffer);

  return len;
}

int Serial::write(char *aBuffer)
{
  if (mDebug) 
    printf("Writing '%s'\n", aBuffer);

  int ret = write(aBuffer, (int) strlen(aBuffer));
  if (ret < 0)
    throw SerialError("Couldn't write");
  
  if (mDebug) 
    printf("Write returned: %d\n", ret);
  
  return ret;
}

bool Serial::flushInput()
{
  char buffer[2];
  int ret;
  do
  {
    ret = read(buffer, 1);
    if (ret < 0)
      throw SerialError("Couldn't read");
  } while (ret > 0);

  return true;
}

#ifdef WIN32
#include "serial.win32.inc"
#else
#include "serial.unix.inc"
#endif

