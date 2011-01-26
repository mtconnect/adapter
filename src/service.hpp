#ifndef SERVICE_HPP
#define SERVICE_HPP

#include "logger.hpp"

#define SERVICE_NAME_LEN 80

class MTConnectService {
public:
  MTConnectService();
  virtual int main(int aArgc, const char *aArgv[]);
  virtual void initialize(int aArgc, const char *aArgv[]) = 0;

  void setName(const char *aName);
  virtual void stop() = 0;
  virtual void start() = 0;
  const char *name() { return mName; }
  
protected:
  char mName[SERVICE_NAME_LEN];
  bool mIsService;
  bool mDebug;
  
  void install(int argc, const char *argv[]);
  void remove();
};

#ifdef WIN32
class ServiceLogger : public Logger {
public:
  virtual void error(const char *aFormat, ...);
  virtual void warning(const char *aFormat, ...);
  virtual void info(const char *aFormat, ...);  
  virtual void debug(const char *aFormat, ...);  

protected:
};
#else
class ServiceLogger : public Logger {};
#endif
#endif
