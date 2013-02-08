
#include "internal.hpp"
#include "condition.hpp"
#include "string_buffer.hpp"

// Condition

Condition::Condition(const char *aName, bool aSimple) :
  DeviceDatum(aName), mBegun(false), mSimple(aSimple)
{
  mActiveSize = mInitialActiveListSize;
  mActiveCount = 0;
  mActiveList = new ActiveCondition*[mActiveSize];
  mHasValue = true;
  unavailable();
}

Condition::~Condition()
{
  removeAll();
  delete[] mActiveList;
}

void Condition::removeAll()
{
  for (int i = 0; i < mActiveCount; i++)
    delete mActiveList[i];
  mActiveCount = 0;
}

bool Condition::requiresFlush()
{
  return true;
}

bool Condition::unavailable()
{
  return add(eUNAVAILABLE);
}

char *Condition::toString(char *aBuffer, int aMaxLen)
{
  return NULL;
}

void Condition::begin()
{
  if (!mSimple) {
    for (int i = 0; i < mActiveCount; i++) {
      mActiveList[i]->clear();
    }
  }
  mPrepared = false;
  mBegun = true;
  mChanged = false;
}

void Condition::cleanup()
{
  mBegun = false;
  mPrepared = false;
}

void Condition::initialize()
{
  normal();
}

void Condition::append(StringBuffer &aStringBuffer, char *aBuffer,
                       ActiveCondition *aCond, bool &aFirst, int aMaxLen)
{
  if (!aFirst)
    aStringBuffer.newline();
  else
    aFirst = false;

  char *cp = aBuffer + strlen(aBuffer);
      
  aCond->toString(cp, aMaxLen);
  appendText(aBuffer, (char*) aCond->getText(), aMaxLen);
  aStringBuffer.append(aBuffer);
}

void Condition::prepare()
{
  if (mBegun)
  {
    bool marked = false;

    // Check to see if we have no marked conditions
    for (int i = 0; !marked && i < mActiveCount; i++)
    {
      if (mActiveList[i]->isPlaceHolder() || mActiveList[i]->isMarked())
        marked = true;
    }

    if (!marked)
      normal();
  
    // Sweep old conditions
    for (int i = mActiveCount - 1; i >= 0; i--)
    {
      ActiveCondition *cond = mActiveList[i];
      if (!cond->isPlaceHolder() && !cond->isMarked())
      {
        cond->setValue(eNORMAL, "", cond->getNativeCode());
      }
    
      if (cond->hasChanged()) {
        mChanged = true;
      }
    }
    mPrepared = true;
  }
}

bool Condition::append(StringBuffer &aBuffer)
{
  if (!mBegun) 
  {
    char buffer[1024];
    bool first = true;
    buffer[0] = '|';
    strcpy(buffer + 1, mName);
    char *cp = buffer + strlen(buffer);
    int max = 1024 - strlen(buffer);
    for (int i = 0; i < mActiveCount; i++)
    {
      *cp = '\0';
      append(aBuffer, buffer, mActiveList[i], first, max);
    }
  } else if (mBegun && mPrepared) {
    char buffer[1024];
    bool first = true;
    buffer[0] = '|';
    strcpy(buffer + 1, mName);
    char *cp = buffer + strlen(buffer);
    int max = 1024 - strlen(buffer);
  
    // Sweep old conditions
    for (int i = mActiveCount - 1; i >= 0; i--)
    {
      ActiveCondition *cond = mActiveList[i];
      if (cond->hasChanged()) {
        *cp = '\0';
        append(aBuffer, buffer, cond, first, max);
      }
      
      // Remove stale conditions since they have now been generated.
      if (!cond->isPlaceHolder() && !cond->isMarked())
        removeAt(i);
    }
  }
  
  return mChanged;
}

bool Condition::isActive(const char *aNativeCode)
{
  for (int i = 0; i < mActiveCount; i++) {
    if (strcmp(aNativeCode, mActiveList[i]->getNativeCode()) == 0)
      return true;
  }

  return false;
}

bool Condition::add(ELevels aLevel, const char *aText, const char *aCode,
                    const char *aQualifier, const char *aSeverity)
{
  ActiveCondition *cond = NULL;
  bool res;
  // First check for a unassociated normal or a unavailable.
  if ((aLevel == eNORMAL || aLevel == eUNAVAILABLE) &&
      aCode[0] == '\0')
  {
    // See if we are already in this state.
    if (mActiveCount == 1 && mActiveList[0]->getNativeCode()[0] == '\0' &&
        mActiveList[0]->getLevel() == aLevel)
    {
      mActiveList[0]->mark();
      res = false;
    }
    else
    {
      // Clear all existing conditions and add one with this state.
      removeAll();
      cond = new ActiveCondition(aLevel);
      add(cond);
      res = mChanged = true;
    }
  }
  else
  {
    if (mActiveCount == 1 &&
        (mActiveList[0]->getLevel() == eNORMAL ||
         mActiveList[0]->getLevel() == eUNAVAILABLE)) 
    {
      removeAll();
    }
    
    // We have a code specific condition or a ab-normal
    int i;
    for (i = 0; i < mActiveCount; i++)
      if (strcmp(aCode, mActiveList[i]->getNativeCode()) == 0)
        break;
    
    if (i < mActiveCount)
    {
      res = mChanged = mActiveList[i]->setValue(aLevel, aText, aCode, aQualifier, aSeverity);
      mActiveList[i]->mark();
    }
    else
    {
      // New condition
      cond = new ActiveCondition(aLevel, aText, aCode, aQualifier, aSeverity);
      add(cond);
      res = mChanged = true;
    }
  }
  
  return res;
}

void Condition::remove(const char *aCode)
{
  // We have a code specific condition or a ab-normal
  int i;
  for (i = 0; i < mActiveCount; i++) {
    if (strcmp(aCode, mActiveList[i]->getNativeCode()) == 0) {
      if (mActiveCount == 1)
        normal();
      else {
        mActiveList[i]->setValue(eNORMAL, "", aCode);
        mActiveList[i]->clear();
      }
      mChanged = true;
      break;      
    }
  }
}


void Condition::add(ActiveCondition *aCond)
{
  if (mActiveCount >= mActiveSize)
  {
    ActiveCondition **newList = new ActiveCondition*[mActiveSize * 2];
    memcpy(newList, mActiveList, sizeof(ActiveCondition*) * mActiveSize);
    delete mActiveList;
    mActiveList = newList;
    mActiveSize *= 2;
  }
  
  mActiveList[mActiveCount++] = aCond;
}

bool Condition::removeAt(int i)
{
  if (i < 0 || i >= mActiveCount)
    return false;

  delete mActiveList[i];
  mActiveCount--;
  memmove(mActiveList + i, mActiveList + i + 1,
          sizeof(ActiveCondition*) * (mActiveCount - i));
  
  return true;
}

char *Condition::ActiveCondition::toString(char *aBuffer, int aMaxLen)
{
  const char *text;
  switch(mLevel)
  {
  case eUNAVAILABLE: text = "UNAVAILABLE"; break;
  case eNORMAL: text = "NORMAL"; break;
  case eWARNING: text = "WARNING"; break;
  case eFAULT: text = "FAULT"; break;
  default: text = ""; break;
  }
  snprintf(aBuffer, aMaxLen, "|%s|%s|%s|%s|", text, mNativeCode, mNativeSeverity,
                mQualifier);

  mChanged = false;
  return aBuffer;
}

bool Condition::ActiveCondition::setValue(ELevels aLevel, const char *aText, const char *aCode,
                          const char *aQualifier, const char *aSeverity)
{
  if ((aLevel == eNORMAL || aLevel == eUNAVAILABLE) && aCode[0] == '\0')
    mPlaceHolder = true;
  
  if (mLevel != aLevel ||
      strncmp(aCode, mNativeCode, EVENT_VALUE_LEN) != 0 ||
      strncmp(aQualifier, mQualifier, EVENT_VALUE_LEN) != 0 ||
      strncmp(aSeverity, mNativeSeverity, EVENT_VALUE_LEN) != 0 ||
      strncmp(aText, mText, EVENT_VALUE_LEN) != 0)
  {
    mLevel = aLevel;
    
    strncpy(mNativeCode, aCode, EVENT_VALUE_LEN);
    mNativeCode[EVENT_VALUE_LEN - 1] = '\0';
    
    strncpy(mQualifier, aQualifier, EVENT_VALUE_LEN);
    mQualifier[EVENT_VALUE_LEN - 1] = '\0';

    strncpy(mNativeSeverity, aSeverity, EVENT_VALUE_LEN);
    mNativeSeverity[EVENT_VALUE_LEN - 1] = '\0';

    strncpy(mText, aText, EVENT_VALUE_LEN);
    mText[EVENT_VALUE_LEN - 1] = '\0';
    
    mChanged = true;
  }
  
  return mChanged;
}

