
#include "device_datum.hpp"
#include "condition.hpp"
#include "adapter.hpp"
#include "fanuc_axis.hpp"
#include <vector>

class FanucPath
{
public:
  FanucPath(Adapter *anAdapter, short aPathNumber);
  ~FanucPath();

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
  Event          mActiveAxes;
  ControllerMode mMode;
  EmergencyStop  mEStop;
  
  Sample         mPathFeedrate;
  PathPosition   mPathPosition;

  int            mProgramNum;
  short          mSpindleCount;
  short          mAxisCount;
  
  // Path related conditions
  Condition mServo;
  Condition mComms;
  Condition mLogic;
  Condition mMotion;
  Condition mSystem;
  Condition mSpindle;

  Sample *mXAxisPos, *mYAxisPos, *mZAxisPos;

  std::vector<FanucAxis*> mAxes;
  std::vector<FanucSpindle*> mSpindles;
};
