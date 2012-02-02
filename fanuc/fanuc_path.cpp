
#include "internal.hpp"
#include "Fwlib32.h"
#include "fanuc_path.hpp"
#include "logger.hpp"

#include <string>

using namespace std;

void FanucPath::addDatum(DeviceDatum &aDatum, const char *aName, const char *aSuffix)
{
  char name[32];
  strcpy(name, aName); strcat(name, aSuffix);
  aDatum.setName(name);
  mAdapter->addDatum(aDatum);
}

FanucPath::FanucPath(Adapter *anAdapter, short aPathNumber)
  : mAdapter(anAdapter), mPathNumber(aPathNumber), mXAxisPos(NULL), mYAxisPos(NULL),
    mZAxisPos(NULL)
{
  char number[2];
  if (aPathNumber > 1)
    sprintf(number, "%d", aPathNumber);
  else
    number[0] = '\0';

  addDatum(mToolId, "tool_id", number);
  addDatum(mToolGroup, "tool_group", number);
  addDatum(mProgramName, "program", number);
  addDatum(mProgramComment, "program_comment", number);
  addDatum(mLine, "line", number);
  addDatum(mBlock, "block", number);
  addDatum(mPathFeedrate, "path_feedrate", number);
  addDatum(mPathPosition, "path_position", number);
  addDatum(mActiveAxes, "active_axes", number);
  addDatum(mMode, "mode", number);
  addDatum(mServo, "servo", number);
  addDatum(mComms, "comms", number);
  addDatum(mLogic, "logic", number);
  addDatum(mMotion, "motion", number);
  addDatum(mSystem, "system", number);
  addDatum(mSpindle, "spindle", number);

  // Only track estop on the first path. Should be the same for all
  // paths, only one estop per machine.
  if (aPathNumber == 1)
    addDatum(mEStop, "estop", number);
}

FanucPath::~FanucPath()
{
  size_t i;
  for (i = 0; i < mAxes.size(); i++)
    delete mAxes[i];
  mAxes.clear();

  i;
  for (i = 0; i < mSpindles.size(); i++)
    delete mSpindles[i];
  mSpindles.clear();
}


bool FanucPath::configure(unsigned short aFlibhndl)
{
  int ret = cnc_setpath(aFlibhndl, mPathNumber);
  if (ret != EW_OK)
  {
    gLogger->error("Could not cnc_setpath: %d\n");
    return false;
  }
  
  return configureAxes(aFlibhndl) &&
    configureSpindles(aFlibhndl);
}

bool FanucPath::configureSpindles(unsigned short aFlibhndl)
{
  ODBSPDLNAME spindles[MAX_SPINDLE];
  mSpindleCount = MAX_SPINDLE;
  short ret = cnc_rdspdlname(aFlibhndl, &mSpindleCount, spindles);
  if (ret == EW_OK)
  {
    int i = 0;
    for (i = 0; i < mSpindleCount; i++)
    {
      printf("Spindle %d : %c%c%c\n", i, spindles[i].name, spindles[i].suff1, spindles[i].suff2);
      char name[12];
      int j = 0;
      name[j++] = spindles[i].name;
      if (spindles[i].suff1 > 0)
        name[j++] =  spindles[i].suff1;
      name[j] = '\0';

      mSpindles.push_back(new FanucSpindle(mAdapter, name, i));
    }
    
    return true;
  }
  else
  {
    printf("Failed to get splindle names: %d\n", ret);
    return false;
  }
}

bool FanucPath::configureAxes(unsigned short aFlibhndl)
{
  ODBAXISNAME axes[MAX_AXIS];
  mAxisCount = MAX_AXIS;
  short ret = cnc_rdaxisname(aFlibhndl, &mAxisCount, axes);
  if (ret == EW_OK)
  {
    int i = 0;
    string activeAxes;
    for (i = 0; i < mAxisCount; i++)
    {
      if (i > 0)
        activeAxes += " ";
      
      printf("Axis %d : %c%c\n", i, axes[i].name, axes[i].suff);
      char name[12];
      int j = 0;
      name[j++] = axes[i].name;
      if (axes[i].suff > 0)
        name[j++] =  axes[i].suff;
      name[j] = '\0';
      
      activeAxes += name;

      FanucAxis *axis = new FanucAxis(mAdapter, name, i);
      mAxes.push_back(axis);

      if (axes[i].name == 'X' && (axes[i].suff == 0 || mXAxisPos == NULL))
        mXAxisPos = &(axis->mActual);
      else if (axes[i].name == 'Y' && (axes[i].suff == 0 || mYAxisPos == NULL))
        mYAxisPos = &(axis->mActual);
      else if (axes[i].name == 'Z' && (axes[i].suff == 0 || mZAxisPos == NULL))
        mZAxisPos = &(axis->mActual);

      
    }
    mActiveAxes.setValue(activeAxes.c_str());
  }
  else
  {
    printf("Failed to get axis names: %d\n", ret);
    exit(999);
  }

  short count, inprec[MAX_AXIS], outprec[MAX_AXIS];
  ret = cnc_getfigure(aFlibhndl, 0, &count, inprec, outprec);
  if (ret == EW_OK)
  {
    for (int i = 0; i < mAxes.size(); i++)
      mAxes[i]->mDivisor = pow((long double) 10.0, (long double) inprec[i]);
  }
  else
  {
    printf("Failed to get axis scale: %d\n", ret);
    return false;
  }

  return true;
}

bool FanucPath::gatherData(unsigned short aFlibhndl)
{
  int ret;
  ret = cnc_setpath(aFlibhndl, mPathNumber);
  if (ret != EW_OK)
  {
    gLogger->error("Cannot set path to: %d, %d", ret);
    return false;
  }

  return getProgramInfo(aFlibhndl) && 
    getStatus(aFlibhndl) &&
    getAxisData(aFlibhndl) &&
    getSpindleData(aFlibhndl) &&
    getToolData(aFlibhndl);
}

bool FanucPath::getProgramInfo(unsigned short aFlibhndl)
{
  int ret;
  char buf[1024];
  unsigned short len = sizeof(buf);
  short num;
  ret = cnc_rdexecprog(aFlibhndl, (unsigned short*) &len, &num, buf);
  if (ret == EW_OK)
  {
    buf[len] = '\0';
    for (int i = 0; i < len; i++)
    {
      if (buf[i] == '\n')
      {
        buf[i] = '\0';
        break;
      }
    }
    
    mBlock.setValue(buf);
    mLine.setValue(num);
    
    return true;
  }
  else
  {
    gLogger->error("Cannot cnc_rdexecprog for path %d: %d", mPathNumber, ret);
    return false;
  }
}

bool FanucPath::getStatus(unsigned short aFlibhndl)
{
  ODBST status;
  memset(&status, 0, sizeof(status));
  int ret = cnc_statinfo(aFlibhndl, &status);
  if (ret == EW_OK)
  {
    if (status.run == 3 || status.run == 4) // STaRT
      mExecution.setValue(Execution::eACTIVE);
    else 
    {
      if (status.run == 2 || status.motion == 2 || status.mstb != 0) // HOLD or motion is Wait
        mExecution.setValue(Execution::eINTERRUPTED);
      else if (status.run == 0) // STOP
        mExecution.setValue(Execution::eSTOPPED);
      else
        mExecution.setValue(Execution::eREADY);
    }

    // This will take care of JOG 
    if (status.aut == 5 || status.aut == 6) 
      mMode.setValue(ControllerMode::eMANUAL);
    else if (status.aut == 0 ||status.aut == 3) // MDI and EDIT
      mMode.setValue(ControllerMode::eMANUAL_DATA_INPUT);
    else // Otherwise AUTOMATIC
      mMode.setValue(ControllerMode::eAUTOMATIC);

    if (mPathNumber == 1) 
    {
      if (status.emergency == 1)
        mEStop.setValue(EmergencyStop::eTRIGGERED);
      else
        mEStop.setValue(EmergencyStop::eARMED);
    }
    return true;
  }
  else
  {
    gLogger->error("Cannot cnc_statinfo for path %d: %d", mPathNumber, ret);
    return false;
  }
}

bool FanucPath::getToolData(unsigned short aFlibhndl)
{
  ODBTLIFE3 toolId;
  short ret = cnc_rdntool(aFlibhndl, 0, &toolId);
  if (ret == EW_OK && toolId.data != 0)
  {
    mToolId.setValue(toolId.data);
    mToolGroup.setValue(toolId.datano);
  }
  else
  {
    gLogger->warning("Cannot cnc_rdntool for path %d: %d", mPathNumber, ret);
  }

  return true;
}

bool FanucPath::getHeader(unsigned short aFlibhndl, int aProg)
{
  /* This is not needed since we're getting the codes from
     macros now. */
  
  char program[2048];
  short ret = cnc_upstart(aFlibhndl, aProg);
  if (ret == EW_OK)
  {
    long len = sizeof(program);
    do 
    {
      ret = cnc_upload3(aFlibhndl, &len, program);
      if (ret == EW_OK)
      {
        bool nl = false;
        program[len] = '\0';
        int lineCount = 0;
        for (char *cp = program; *cp != '\0' && lineCount < 5; ++cp)
        {
          // When we get a new line, check for the first empty line
          // following with only spaces, ; or carriage returns. If 
          // a new line follows, then terminate the header and set the
          // program comment.
          if (*cp == '\n') 
          {
            char f = *(cp + 1);
            if (lineCount > 0 && f != '(')
            {
              *cp = '\0';
              break;
            }
            *cp = ' ';
            lineCount++;
          }
        }
        mProgramComment.setValue(program);
      }
    } while (ret == EW_BUFFER);
  }
  cnc_upend3(aFlibhndl);

  return true;
}

bool FanucPath::getAxisData(unsigned short aFlibhndl)
{
  ODBDY2 dyn;
  short ret = cnc_rddynamic2(aFlibhndl, -1, sizeof(dyn), &dyn);
  if (ret == EW_OK)
  {
    ODBSVLOAD axLoad[MAX_AXIS];
    short num = MAX_AXIS;
    ret = cnc_rdsvmeter(aFlibhndl, &num, axLoad);
    if (ret != EW_OK)
    {
      gLogger->error("cnc_rdsvmeter failed: %d", ret);
      return false;
    }

    char buf[32];
    if (dyn.prgnum != mProgramNum)
      getHeader(aFlibhndl, dyn.prgnum);

    mProgramNum = dyn.prgnum;
    sprintf(buf, "%d.%d", dyn.prgmnum, dyn.prgnum);
    mProgramName.setValue(buf);

    // Update all the axes
    vector<FanucAxis*>::iterator axis;
    for (axis = mAxes.begin(); axis != mAxes.end(); axis++)
    {
      (*axis)->gatherData(&dyn, axLoad);
    }

    mPathFeedrate.setValue(dyn.actf);

    double x = 0.0, y = 0.0, z = 0.0;
    if (mXAxisPos != NULL && mXAxisPos->hasValue())
      x = mXAxisPos->getValue();
    if (mYAxisPos != NULL && mYAxisPos->hasValue())
      y = mYAxisPos->getValue();
    if (mZAxisPos != NULL && mZAxisPos->hasValue())
      z = mZAxisPos->getValue();
    
    mPathPosition.setValue(x, y, z);

    getCondition(aFlibhndl, dyn.alarm);
  }

  return true;
}

bool FanucPath::getSpindleData(unsigned short aFlibhndl)
{
    // Handle spindle data...
  ODBACT2 speeds;
  int ret = cnc_acts2(aFlibhndl, ALL_SPINDLES, &speeds);
  if (ret != EW_OK)
  {
    gLogger->error("cnc_acts2 failed: %d", ret);
    return false;
  }
  
  ODBSPLOAD spLoad[MAX_SPINDLE];
  short num = MAX_SPINDLE;
  ret = cnc_rdspmeter(aFlibhndl, 0, &num, spLoad);
  if (ret != EW_OK)
  {
    gLogger->error("cnc_rdspmeter failed: %d", ret);
    return false;
  }
  
  if (num > mSpindleCount)
  {
    gLogger->error("spindle load has more spindles than names: %d > %d\n",
                   num, mSpindleCount);
    return false;
  }

  // Update all the axes
  vector<FanucSpindle*>::iterator spindle;
  for (spindle = mSpindles.begin(); spindle != mSpindles.end(); spindle++)
  {
    (*spindle)->gatherData(spLoad, &speeds);
  }
  
  return true;
}

Condition *FanucPath::translateAlarmNo(long aNum, int aAxis)
{
  switch(aNum) 
  {
  case 0: // Parameter Switch Off
    return &mLogic;

  case 2: // I/O
  case 7: // Data I/O
    return &mComms;

  case 4: // Overtravel
    if (aAxis > 0)
      return &(mAxes[aAxis - 1]->mTravel);
    else
      return &mSystem;

  case 5: // Overheat
    if (aAxis > 0)
      return &(mAxes[aAxis - 1]->mOverheat);
    else
      return &mSystem;

  case 6: // Servo
    if (aAxis > 0)
      return &(mAxes[aAxis - 1]->mServo);
    else
      return &mServo;

  case 12: // Background P/S
  case 3: // Forground P/S
  case 8: // Macro
    return &mMotion;

  case 9: // Spindle
    return &mSpindle;
    
  case 19: // PMC
    return &mLogic;
    
  default: // 10, 11, 13, 15.
    return &mSystem;
  }

  return NULL;
}

void FanucPath::getCondition(unsigned short aFlibhndl, long aAlarm)
{
  if (aAlarm != 0) 
  {
    for (int i = 0; i < 31; i++) 
    {
      if (aAlarm & (0x1 << i))
      {
        ODBALMMSG2 alarms[MAX_AXIS];
        short num = MAX_AXIS;
        
        short ret = cnc_rdalmmsg2(aFlibhndl, i, &num, alarms);
        if (ret != EW_OK)
          continue;

        for (int j = 0; j < num; j++) 
        {
          ODBALMMSG2 &alarm = alarms[j];
          char num[16];
          
          Condition *cond = translateAlarmNo(i, alarm.axis);
          if (cond == NULL)
            continue;

          sprintf(num, "%d", alarm.alm_no);
          cond->add(Condition::eFAULT, alarm.alm_msg, num);
        }
      }
    }       
  }
}
  
