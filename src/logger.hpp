
#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <stdarg.h>
#include <stdio.h>

#define LOGGER_BUFFER_SIZE 1024

class Logger {
public:
  enum LogLevel {
    eDEBUG,
    eINFO,
    eWARNING,
    eERROR
  };
  
  Logger(FILE *aFile = stderr) : mFile(aFile) { mLogLevel = eINFO; }
  void setLogLevel(LogLevel aLevel) { mLogLevel = aLevel; }
  LogLevel getLogLevel() { return mLogLevel; }
  
  virtual void error(const char *aFormat, ...);
  virtual void warning(const char *aFormat, ...);
  virtual void info(const char *aFormat, ...);
  virtual void debug(const char *aFormat, ...);
  
protected:
  const char *format(char *aBuffer, int aLen, const char *aFormat, va_list args);
  const char *timestamp(char *aBuffer);
  
  LogLevel mLogLevel;
  FILE *mFile;
};

extern Logger *gLogger;

#endif
