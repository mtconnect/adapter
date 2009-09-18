
#ifndef SERIAL_HPP
#define SERIAL_HPP

class Serial {
public:
  class SerialError
  {
  private:
    char mMessage[1024];

  public:
    SerialError(const char *aMessage);
    const char *message() const { return mMessage; }
  };
private:

  
protected:
  /* Descriptor (tty or socket) */
#ifdef WIN32
  HANDLE mFd;
#else
  int mFd;
#endif
  
  /* Flag debug */
  bool mDebug;
  
  /* TCP port */
  int mPort;
  
  /* Device: "/dev/ttyS0", "/dev/ttyUSB0" or "/dev/tty.USA19*"
     on Mac OS X for KeySpan USB<->Serial adapters this string
     had to be made bigger on OS X as the directory+file name
     was bigger than 19 bytes. Making it 67 bytes for now, but
     OS X does support 256 byte file names. May become a problem
     in the future. */
  
#ifdef __APPLE_CC__
  char mDevice[64];
#else
  char mDevice[16];
#endif
  
  /* Bauds: 9600, 19200, 57600, 115200, etc */
  int mBaud;
  
  /* Data bit */
  unsigned char mDataBit;
  
  /* Stop bit */
  unsigned char mStopBit;
  
  /* Parity: "even", "odd", "none" */
  char mParity[5];
  
  /* In error_treat with TCP, do a reconnect or just dump the error */
  bool mErrorHandling;
  
  /* IP address */
  char mIp[16];
  
#ifndef WIN32
  /* Save old termios settings */
  struct termios mOldTios;
#endif

  bool mConnected;

private:
  int read(char *aBuffer, int len);
  int write(const char *aBuffer, int len);

public:
  Serial(const char *aDevice,
	 int aBaud, const char *aParity, int aDataBit,
	 int aStopBit, bool aDebug = false);
  ~Serial();

  bool debug() { return mDebug; }
  bool connected() { return mConnected; }

  bool connect();
  bool disconnect();
  int  readUntil(const char *aUntil, char *aBuffer, int aLength);
  int  write(char *aBuffer);
  bool flushInput();
};

#endif
