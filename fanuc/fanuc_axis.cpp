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
 
#include "internal.hpp"
#include "Fwlib32.h"
#include "fanuc_path.hpp"
#include "logger.hpp"

using namespace std;

FanucAxis::FanucAxis(Adapter *anAdapter, string aPrefix, int aIndex)
  : mIndex(aIndex), mDivisor(1.0)
{
  mActual.setName((aPrefix + "act").c_str());
  anAdapter->addDatum(mActual);
  mLoad.setName((aPrefix + "load").c_str());
  anAdapter->addDatum(mLoad);
  mTravel.setName((aPrefix + "travel").c_str());
  anAdapter->addDatum(mTravel);
  mOverheat.setName((aPrefix + "overheat").c_str());
  anAdapter->addDatum(mOverheat);
  mServo.setName((aPrefix + "servo").c_str());
  anAdapter->addDatum(mServo);
}

bool FanucAxis::gatherData(ODBDY2 *aDynamic, ODBSVLOAD *aLoads)
{
  mActual.setValue(aDynamic->pos.faxis.machine[mIndex] / mDivisor);
  mLoad.setValue(aLoads[mIndex].svload.data /
                  pow((long double) 10.0, (long double) aLoads[mIndex].svload.dec));
  return true;
}

FanucSpindle::FanucSpindle(Adapter *anAdapter, string aPrefix, int aIndex)
  : mIndex(aIndex)
{
  mSpeed.setName((aPrefix + "speed").c_str());
  anAdapter->addDatum(mSpeed);
  mLoad.setName((aPrefix + "load").c_str());
  anAdapter->addDatum(mLoad);
  mServo.setName((aPrefix + "servo").c_str());
  anAdapter->addDatum(mServo);
}

bool FanucSpindle::gatherData(ODBSPLOAD *aLoads, ODBACT2 *aSpeeds)
{
  mLoad.setValue(aLoads[mIndex].spload.data /
                  pow((long double) 10.0, (long double) aLoads[mIndex].spload.dec));
  mSpeed.setValue(aSpeeds->data[mIndex]);
  return true;
}

