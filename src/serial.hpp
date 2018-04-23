
#ifndef SERIAL_HPP
#define SERIAL_HPP

class Serial {
//
// Copyright (c) 2008, AMT - The Association For Manufacturing Technology ("AMT")
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the AMT nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// DISCLAIMER OF WARRANTY. ALL MTCONNECT MATERIALS AND SPECIFICATIONS PROVIDED
// BY AMT, MTCONNECT OR ANY PARTICIPANT TO YOU OR ANY PARTY ARE PROVIDED "AS IS"
// AND WITHOUT ANY WARRANTY OF ANY KIND. AMT, MTCONNECT, AND EACH OF THEIR
// RESPECTIVE MEMBERS, OFFICERS, DIRECTORS, AFFILIATES, SPONSORS, AND AGENTS
// (COLLECTIVELY, THE "AMT PARTIES") AND PARTICIPANTS MAKE NO REPRESENTATION OR
// WARRANTY OF ANY KIND WHATSOEVER RELATING TO THESE MATERIALS, INCLUDING, WITHOUT
// LIMITATION, ANY EXPRESS OR IMPLIED WARRANTY OF NONINFRINGEMENT,
// MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
//
// LIMITATION OF LIABILITY. IN NO EVENT SHALL AMT, MTCONNECT, ANY OTHER AMT
// PARTY, OR ANY PARTICIPANT BE LIABLE FOR THE COST OF PROCURING SUBSTITUTE GOODS
// OR SERVICES, LOST PROFITS, LOSS OF USE, LOSS OF DATA OR ANY INCIDENTAL,
// CONSEQUENTIAL, INDIRECT, SPECIAL OR PUNITIVE DAMAGES OR OTHER DIRECT DAMAGES,
// WHETHER UNDER CONTRACT, TORT, WARRANTY OR OTHERWISE, ARISING IN ANY WAY OUT OF
// THIS AGREEMENT, USE OR INABILITY TO USE MTCONNECT MATERIALS, WHETHER OR NOT
// SUCH PARTY HAD ADVANCE NOTICE OF THE POSSIBILITY OF SUCH DAMAGES.
//
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

	int readUntil(const char *aUntil, char *aBuffer, int aLength);
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
	int write(const char *aBuffer);
  int print(char c) { char b[2]; b[0] = c; return write(b, 1); }

	bool flushInput();
	bool flush();
	bool printCommStatus();
};

#endif
