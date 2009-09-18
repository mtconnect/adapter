
#include "internal.hpp"
#include "string_array.hpp"

#define BLOCK_SIZE 256

StringArray::StringArray()
{
  mLength = 0;
  mSize = BLOCK_SIZE;
  mArray = (char **) malloc(sizeof(char*) * mSize);
  mArray[mLength = 0];
}

StringArray::~StringArray()
{
  clear();
  free(mArray);
}

void StringArray::clear()
{
  for (int i = 0; i < mLength; i++)
    free(mArray[i]);
  mLength = 0;
}

void StringArray::append(const char *aString)
{
  char *dup = strdup(aString);
  if (dup == 0)
  {
    perror("StringArray::append");
    exit(2);
  }

  if (mLength >= mSize - 1)
  {
    mSize += BLOCK_SIZE;
    mArray = (char**) realloc(mArray, mSize * sizeof(char*));
    if (mArray == 0)
    {
      perror("StringArray::append");
      exit(2);
    }
  }
  
  mArray[mLength++] = dup;
}

int StringArray::readFile(const char *aFileName)
{
  // First clear out existing contents
  clear();

  // Parse file.
  FILE *file = fopen(aFileName, "r");
  if (file == 0)
  {
    printf("Could not open file: %s\n", aFileName);
  }
  else
  {
    printf("Parsing file: %s\n", aFileName);
    char buffer[1024];
    while (fgets(buffer, 1024, file) != 0)
    {
      int last = strlen(buffer) - 1;
      if (last > 0 && buffer[last] == '\n')
	buffer[last] = 0;
      append(buffer);
    }
  }
  return mLength;
}




  
