
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
  
protected:
  /* Descriptor (tty or socket) */
#ifdef WIN32
  HANDLE mFd;
#else
  int mFd;
#endif
  
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
  bool mErrorHandling;
    
  enum FlowControl {
    eSOFT,
    eHARD,
    eNONE
  };
  FlowControl mFlow;
  
#ifndef WIN32
  /* Save old termios settings */
  struct termios mOldTios;
#endif

  bool mConnected;

public:
  enum WaitMode {
    READ, WRITE
  };
  
  Serial(const char *aDevice,
	 int aBaud, const char *aParity, int aDataBit,
	 int aStopBit);
  ~Serial();

  bool connected() { return mConnected; }
  bool available() { return mConnected; }

  bool connect();
  bool disconnect();

  int wait(int aTimeout, WaitMode mode = READ);
  static uint64_t getTimestamp();

  // Raw internal read
  int read(char *aBuffer, int len);
  
  int  readUntil(const char *aUntil, char *aBuffer, int aLength);
  int read(char &c) { 
    char buffer[2]; 
    int ret = read(buffer, 1);
    c = buffer[0]; 
    return ret;
  }
  int read(unsigned char &b) {
    char c;
    int ret = read(c);
    b = (unsigned char) c;
    return ret;
  }
  
  int readFully(char *aBuffer, int len, uint32_t timeout = 1000);
  
  // Raw internal write
  int write(const char *aBuffer, int len);
  int writeFully(const char *aBuffer, int len, uint32_t timeout = 1000);
  
  // Write cover methods
  int  write(const char *aBuffer);
  int print(char c) { char b[2]; b[0] = c; return write(b, 1); }
  
  bool flushInput();
  bool flush();
  bool printCommStatus();
};

#endif
