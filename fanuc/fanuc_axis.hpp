
#include "device_datum.hpp"
#include "condition.hpp"
#include "adapter.hpp"
#include <string>
#include "Fwlib32.h"

class FanucAxis
{
public:
  FanucAxis(Adapter *anAdapter, std::string aPrefix, int anIndex);
  ~FanucAxis() {}
                                             
  bool gatherData(ODBDY2 *aDynamic, ODBSVLOAD *aLoads);

public:
  int mIndex;
  
  Sample mActual;
  Sample mLoad;

  double mDivisor;
  
  Condition mTravel;
  Condition mOverheat;
  Condition mServo;
};

class FanucSpindle
{
public:
  FanucSpindle(Adapter *anAdapter, std::string aPrefix, int anIndex);
  ~FanucSpindle() {}

  bool gatherData(ODBSPLOAD *aLoads, ODBACT2 *aSpeeds);

public:
  int mIndex;
  
  Sample mSpeed;
  Sample mLoad;

  Condition mServo;
};
