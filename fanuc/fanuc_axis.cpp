//
// Copyright Copyright 2012, System Insights, Inc.
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.
//
#include "internal.hpp"
#include "fanuc_axis.hpp"
#include <logger.hpp>
#include "fanuc_path.hpp"

using namespace std;


FanucAxis::FanucAxis(Adapter *adapter, string prefix, int index) :
	mIndex(index),
	mDivisor(1.0)
{
	mActual.setName((prefix + "act").c_str());
	adapter->addDatum(mActual);
	mLoad.setName((prefix + "load").c_str());
	adapter->addDatum(mLoad);
	mTravel.setName((prefix + "travel").c_str());
	adapter->addDatum(mTravel);
	mOverheat.setName((prefix + "overheat").c_str());
	adapter->addDatum(mOverheat);
	mServo.setName((prefix + "servo").c_str());
	adapter->addDatum(mServo);
}


bool FanucAxis::gatherData(ODBDY2 *dynamic, ODBSVLOAD *loads)
{
	mActual.setValue(dynamic->pos.faxis.machine[mIndex] / mDivisor);
	mLoad.setValue(loads[mIndex].svload.data /
					pow((long double) 10.0, (long double) loads[mIndex].svload.dec));
	return true;
}


FanucSpindle::FanucSpindle(Adapter *adapter, string prefix, int index) :
	mIndex(index)
{
	mSpeed.setName((prefix + "speed").c_str());
	adapter->addDatum(mSpeed);
	mLoad.setName((prefix + "load").c_str());
	adapter->addDatum(mLoad);
	mServo.setName((prefix + "servo").c_str());
	adapter->addDatum(mServo);
}


bool FanucSpindle::gatherData(ODBSPLOAD *loads, ODBACT2 *aSpeeds)
{
	mLoad.setValue(loads[mIndex].spload.data /
					pow((long double) 10.0, (long double) loads[mIndex].spload.dec));
	mSpeed.setValue(aSpeeds->data[mIndex]);
	return true;
}
