
#include "internal.hpp"
#include "logger.hpp"

Logger *gLogger = NULL;

void Logger::error(const char *aFormat, ...)
{
  char buffer[LOGGER_BUFFER_SIZE];
  char ts[32];
  va_list args;
  va_start (args, aFormat);
  fprintf(mFile, "%s - Error: %s\n", timestamp(ts), format(buffer, LOGGER_BUFFER_SIZE, aFormat, args));
  fflush(mFile);
  va_end (args);
}

void Logger::warning(const char *aFormat, ...)
{
  if (mLogLevel > eWARNING) return;
  
  char buffer[LOGGER_BUFFER_SIZE];
  char ts[32];
  va_list args;
  va_start (args, aFormat);
  fprintf(mFile, "%s - Warning: %s\n", timestamp(ts), format(buffer, LOGGER_BUFFER_SIZE, aFormat, args));
  fflush(mFile);
  va_end (args);
}

void Logger::info(const char *aFormat, ...)
{
  if (mLogLevel > eINFO) return;
  
  char buffer[LOGGER_BUFFER_SIZE];
  char ts[32];
  va_list args;
  va_start (args, aFormat);
  fprintf(mFile, "%s - Info: %s\n", timestamp(ts), format(buffer, LOGGER_BUFFER_SIZE, aFormat, args));
  fflush(mFile);
  va_end (args);
}

void Logger::debug(const char *aFormat, ...)
{
  if (mLogLevel > eDEBUG) return;
  
  char buffer[LOGGER_BUFFER_SIZE];
  char ts[32];
  va_list args;
  va_start (args, aFormat);
  fprintf(mFile, "%s - Debug: %s\n", timestamp(ts), format(buffer, LOGGER_BUFFER_SIZE, aFormat, args));
  fflush(mFile);
  va_end (args);
}


const char *Logger::format(char *aBuffer, int aLen, const char *aFormat, va_list args)
{
  vsprintf(aBuffer, aFormat, args);
  aBuffer[aLen - 1] = '\0';
  return aBuffer;
}

const char *Logger::timestamp(char *aBuffer)
{
#ifdef WIN32
  SYSTEMTIME st;
  GetSystemTime(&st);
  sprintf(aBuffer, "%4d-%02d-%02dT%02d:%02d:%02d.%03dZ", st.wYear, st.wMonth, st.wDay, st.wHour, 
          st.wMinute, st.wSecond, st.wMilliseconds);
#else
  struct timeval tv;
  struct timezone tz;

  gettimeofday(&tv, &tz);

  strftime(aBuffer, 64, "%Y-%m-%dT%H:%M:%S", gmtime(&tv.tv_sec));
  sprintf(aBuffer + strlen(aBuffer), ".%06ldZ", tv.tv_usec);
#endif
  
  return aBuffer;
}
