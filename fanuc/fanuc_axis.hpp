/*
 * Copyright Copyright 2012, System Insights, Inc.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */
 
 
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
