bool Serial::connect()
{
  if (mConnected)
  {
    printf("You are already connected, please disconnect first\n");
    return false;
  }

  struct termios tios;
  speed_t speed;

  if (mDebug) {
    printf("Opening %s at %d bauds (%s) data: %d, stop: %d\n",
	   mDevice, mBaud, mParity, mDataBit, mStopBit);
  }

  /* The O_NOCTTY flag tells UNIX that this program doesn't want
     to be the "controlling terminal" for that port. If you
     don't specify this then any input (such as keyboard abort
     signals and so forth) will affect your process

     Timeouts are ignored in canonical input mode or when the
     NDELAY option is set on the file via open or fcntl */
  mFd = open(mDevice, O_RDWR | O_NOCTTY | O_NDELAY);
  if (mFd < 0) {
    perror("open");
    printf("ERROR Can't open the device %s (errno %d)\n",
	   mDevice, errno);
    return -1;
  }

  /* Save */
  tcgetattr(mFd, &(mOldTios));

  memset(&tios, 0, sizeof(struct termios));
        
  /* C_ISPEED     Input baud (new interface)
     C_OSPEED     Output baud (new interface)
  */
  switch (mBaud) {
  case 110:
    speed = B110;
    break;
  case 300:
    speed = B300;
    break;
  case 600:
    speed = B600;
    break;
  case 1200:
    speed = B1200;
    break;
  case 2400:
    speed = B2400;
    break;
  case 4800:
    speed = B4800;
    break;
  case 9600: 
    speed = B9600;
    break;
  case 19200:
    speed = B19200;
    break;
  case 38400:
    speed = B38400;
    break;
  case 57600:
    speed = B57600;
    break;
  case 115200:
    speed = B115200;
    break;
  default:
    speed = B9600;
    printf("WARNING Unknown baud rate %d for %s (B9600 used)\n",
	   mBaud, mDevice);
  }

  /* Set the baud rate */
  if ((cfsetispeed(&tios, speed) < 0) ||
      (cfsetospeed(&tios, speed) < 0)) {
    perror("cfsetispeed/cfsetospeed\n");
    close(mFd);
    return false;
  }
        
  /* C_CFLAG      Control options
     CLOCAL       Local line - do not change "owner" of port
     CREAD        Enable receiver
  */
  tios.c_cflag |= (CREAD | CLOCAL);

  /* CSIZE, HUPCL, CRTSCTS (hardware flow control) */

  /* Set data bits (5, 6, 7, 8 bits)
     CSIZE        Bit mask for data bits
  */
  tios.c_cflag &= ~CSIZE;
  switch (mDataBit) {
  case 5:
    tios.c_cflag |= CS5;
    break;
  case 6:
    tios.c_cflag |= CS6;
    break;
  case 7:
    tios.c_cflag |= CS7;
    break;
  case 8:
  default:
    tios.c_cflag |= CS8;
    break;
  }

  /* Stop bit (1 or 2) */
  if (mStopBit == 1)
    tios.c_cflag &=~ CSTOPB;
  else /* 2 */
    tios.c_cflag |= CSTOPB;

  // rts/cts flow control
  tios.c_cflag &= ~CRTSCTS;
  
  /* PARENB       Enable parity bit
     PARODD       Use odd parity instead of even */
  if (strncmp(mParity, "none", 4) == 0) {
    tios.c_cflag &=~ PARENB;
  } else if (strncmp(mParity, "even", 4) == 0) {
    tios.c_cflag |= PARENB;
    tios.c_cflag &=~ PARODD;
  } else {
    /* odd */
    tios.c_cflag |= PARENB;
    tios.c_cflag |= PARODD;
  }
        
  /* Read the man page of termios if you need more information. */

  /* This field isn't used on POSIX systems 
     tios.c_line = 0; 
  */

  /* C_LFLAG      Line options 

     ISIG Enable SIGINTR, SIGSUSP, SIGDSUSP, and SIGQUIT signals
     ICANON       Enable canonical input (else raw)
     XCASE        Map uppercase \lowercase (obsolete)
     ECHO Enable echoing of input characters
     ECHOE        Echo erase character as BS-SP-BS
     ECHOK        Echo NL after kill character
     ECHONL       Echo NL
     NOFLSH       Disable flushing of input buffers after
     interrupt or quit characters
     IEXTEN       Enable extended functions
     ECHOCTL      Echo control characters as ^char and delete as ~?
     ECHOPRT      Echo erased character as character erased
     ECHOKE       BS-SP-BS entire line on line kill
     FLUSHO       Output being flushed
     PENDIN       Retype pending input at next read or input char
     TOSTOP       Send SIGTTOU for background output

     Canonical input is line-oriented. Input characters are put
     into a buffer which can be edited interactively by the user
     until a CR (carriage return) or LF (line feed) character is
     received.  

     Raw input is unprocessed. Input characters are passed
     through exactly as they are received, when they are
     received. Generally you'll deselect the ICANON, ECHO,
     ECHOE, and ISIG options when using raw input
  */

  /* Raw input */
  tios.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

  /* C_IFLAG      Input options 

     Constant     Description
     INPCK        Enable parity check
     IGNPAR       Ignore parity errors
     PARMRK       Mark parity errors
     ISTRIP       Strip parity bits
     IXON Enable software flow control (outgoing)
     IXOFF        Enable software flow control (incoming)
     IXANY        Allow any character to start flow again
     IGNBRK       Ignore break condition
     BRKINT       Send a SIGINT when a break condition is detected
     INLCR        Map NL to CR
     IGNCR        Ignore CR
     ICRNL        Map CR to NL
     IUCLC        Map uppercase to lowercase
     IMAXBEL      Echo BEL on input line too long
  */
  if (strncmp(mParity, "none", 4) == 0) {
    tios.c_iflag &= ~INPCK;
  } else {
    tios.c_iflag |= INPCK;
  }

  /* Software flow control is enabled */
  tios.c_iflag |= (IXON | IXOFF);
        
  /* C_OFLAG      Output options
     OPOST        Postprocess output (not set = raw output)
     ONLCR        Map NL to CR-NL

     ONCLR and others needs OPOST to be enabled
  */         

  /* Raw ouput */
  tios.c_oflag &=~ OPOST;

  /* C_CC         Control characters 
     VMIN         Minimum number of characters to read
     VTIME        Time to wait for data (tenths of seconds)

     UNIX serial interface drivers provide the ability to
     specify character and packet timeouts. Two elements of the
     c_cc array are used for timeouts: VMIN and VTIME. Timeouts
     are ignored in canonical input mode or when the NDELAY
     option is set on the file via open or fcntl.

     VMIN specifies the minimum number of characters to read. If
     it is set to 0, then the VTIME value specifies the time to
     wait for every character read. Note that this does not mean
     that a read call for N bytes will wait for N characters to
     come in. Rather, the timeout will apply to the first
     character and the read call will return the number of
     characters immediately available (up to the number you
     request).

     If VMIN is non-zero, VTIME specifies the time to wait for
     the first character read. If a character is read within the
     time given, any read will block (wait) until all VMIN
     characters are read. That is, once the first character is
     read, the serial interface driver expects to receive an
     entire packet of characters (VMIN bytes total). If no
     character is read within the time allowed, then the call to
     read returns 0. This method allows you to tell the serial
     driver you need exactly N bytes and any read call will
     return 0 or N bytes. However, the timeout only applies to
     the first character read, so if for some reason the driver
     misses one character inside the N byte packet then the read
     call could block forever waiting for additional input
     characters.

     VTIME specifies the amount of time to wait for incoming
     characters in tenths of seconds. If VTIME is set to 0 (the
     default), reads will block (wait) indefinitely unless the
     NDELAY option is set on the port with open or fcntl.
  */
  /* Unused because we use open with the NDELAY option */
  tios.c_cc[VMIN] = 0;
  tios.c_cc[VTIME] = 5;

  if (tcsetattr(mFd, TCSANOW, &tios) < 0) {
    perror("tcsetattr\n");
    close(mFd);
    return false;
  }

  mConnected = true;
  
  return true;
}

bool Serial::disconnect()
{
  if (mConnected)
  {
    tcsetattr(mFd, TCSANOW, &mOldTios);
    close(mFd);
    mConnected = false;
    return true;
  }
  else
  {
    return false;
  }
}

int Serial::read(char *aBuffer, int aLen)
{
  return ::read(mFd, aBuffer, aLen);
}

int Serial::write(const char *aBuffer, int aLen)
{
  return ::write(mFd, aBuffer, aLen);
}

