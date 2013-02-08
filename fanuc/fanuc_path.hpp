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
#include "fanuc_axis.hpp"
#include <vector>

class StaticEvent : public Event 
{
public:
  StaticEvent(const char *aName = "") : Event(aName) {}      

  virtual bool unavailable() {
    return false;
  }
};

class FanucPath
{
public:
  FanucPath(Adapter *anAdapter, short aPathNumber);
  ~FanucPath();

  void allowDNC(const bool aAllow) { mAllowDNC = aAllow; }
  bool configure(unsigned short mFlibhndl);
  bool gatherData(unsigned short mFlibhndl);

protected:
  bool configureAxes(unsigned short mFlibhndl);
  bool configureSpindles(unsigned short mFlibhndl);
  
  bool getToolData(unsigned short aFlibhndl);
  bool getProgramInfo(unsigned short mFlibhndl);
  bool getStatus(unsigned short mFlibhndl);
  bool getHeader(unsigned short aFlibhndl, int aProg);
  bool getAxisData(unsigned short aFlibhndl);
  bool getSpindleData(unsigned short aFlibhndl);

  void getCondition(unsigned short aFlibhndl, long aAlarm);
  Condition *translateAlarmNo(long aNum, int aAxis);


  void addDatum(DeviceDatum &aDatum, const char *aName, const char *aSuffix);

protected:
  bool mConfigured;
  Adapter *mAdapter;
  
  short mPathNumber;

  // Path specific data items
  Execution      mExecution;
  IntEvent       mToolId;
  IntEvent       mToolGroup;
  Event          mProgramName;
  Event          mProgramComment;
  IntEvent       mLine;
  Event          mBlock;
  StaticEvent    mActiveAxes;
  ControllerMode mMode;
  EmergencyStop  mEStop;
  
  Sample         mPathFeedrate;
  PathPosition   mPathPosition;
  Sample         mCommandedFeedrate;

  int            mProgramNum;
  short          mSpindleCount;
  short          mAxisCount;

  bool           mToolManagementEnabled;
  bool           mUseModalToolData;
  bool           mAllowDNC;
  
  // Path related conditions
  Condition mServo;
  Condition mComms;
  Condition mLogic;
  Condition mMotion;
  Condition mSystem;

  FanucAxis *mXAxis, *mYAxis, *mZAxis;

  std::vector<FanucAxis*> mAxes;
  std::vector<FanucSpindle*> mSpindles;
};
