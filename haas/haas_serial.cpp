
#include "internal.hpp"
#include "haas_serial.hpp"

using namespace std;

HaasSerial::HaasSerial(const char *aDevice,
	     int aBaud, const char *aParity, int aDataBit,
	     int aStopBit, bool aDebug)
  : Serial(aDevice, aBaud, aParity, aDataBit, aStopBit, aDebug)
{
}

vector<string> *HaasSerial::split(const char *aString)
{
  vector<string> *list = new vector<string>;

  const char *start = aString;
  const char *cp = aString;
  
  while (*cp)
  {
    if (*cp == ',' || *cp == 0x17)
    {
      const char *end = cp;
      while (end > start && (*end == ' '))
	end--;

      if (end > start)
      {
	string s(start, end);
	list->push_back(s);
      }

      // Strip , and spaces from end of string
      while (*cp != 0x17 && (*cp == ' ' || *cp == ','))
	cp++;

      // Start parsing the next block
      start = cp;
    }
    
    if (*cp == 0x17)
      break;
    
    cp++;
  }

  return list;
}

vector<string> *HaasSerial::sendCommand(const char *aCommand)
{
  // Wait a small amount of time.
  usleep(20 * 1000); // 20 msec
  
  char buffer[1024];
  sprintf(buffer, "%s\r\n", aCommand);
  write(buffer);

  if (readUntil("\r\n>", buffer, sizeof(buffer)) <= 0)
    return 0;

  // Find STX for the beginning start of data
  char *cp = buffer;
  while (*cp != 0x02 && *cp)
    cp++;

  if (*cp == 0)
    return 0;
  
  cp++;

  vector<string> *l = split(cp);
  return l;
}

bool HaasSerial::getVariable(int aVariableNumber, double &aResult)
{
  bool success = false;
  char command[64];

  bool again = false;
  do 
  {
    sprintf(command, "Q600 %d", aVariableNumber);
    vector<string> *res = this->sendCommand(command);
    if (res && res->size() == 3)
    {
      // Check to make sure the correct variable is received
      if (atoi(res->at(1).c_str()) == aVariableNumber)
      {
	string &v = res->back();
	aResult = atof(v.c_str());
	success = true;
      }
      else if (!again)
      {
	//  we'll try a second time after flushing the input stream.
	again = true;
	flushInput();
      }
      else // Only try twice.
	again = false;
    }
    if (res)
      delete res;

    
  } while (again);

  return success;
}



