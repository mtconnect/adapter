
#ifndef KRISHNA_DATA
#define KRISHNA_DATA

#include "device_datum.hpp"
#include <vector>

class KrishnaSample : public Sample {
public:
  KrishnaSample(const char *aName, int aOffset, double aScaler) :
    Sample(aName) { mOffset = aOffset; mScaler = aScaler; }

  int getOffset() { return mOffset; }
  double getScaler() { return mScaler; }

  bool setValue(double aValue) { return Sample::setValue(aValue * mScaler); }
  
protected:
  int mOffset;
  double mScaler;
};

class KrishnaData {
public:
  KrishnaData(int aAddress) : mAddress(aAddress), mRequestCount(0) {}
  
  ~KrishnaData() {
    delete[] mData;
  }
  
  int getAddress() { return mAddress; }
  int getCount() { return mSamples.size(); }
  uint16_t getRequestCount() { return mRequestCount; }

  void addSample(KrishnaSample *aSample) {
    mSamples.push_back(aSample);
    if (aSample->getOffset() > mRequestCount)
      mRequestCount = aSample->getOffset();
  }

  void createData() {
    mData = new uint16_t[getCount()];
  }
  void *getData() { return mData; }
  int  getDataLength() { return mRequestCount * sizeof(uint16_t); }

  void unavailable() {
    for (int i = 0; i < mSamples.size(); i++) {
      mSamples[i]->unavailable();
    }
  }
  
  virtual void writeValues() {
    for (int i = 0; i < mSamples.size(); i++) {
      mSamples[i]->setValue((double) mData[i]);
    }
  }

protected:
  std::vector<KrishnaSample*> mSamples;
  int mAddress;
  double mScaler;
  uint16_t *mData;
  uint16_t mRequestCount;
};


class KrishnaResponse : public Rx64Response 
{
public:
  uint8_t getDataLength() { return getFrameData()[Rx64Response::getDataOffset() + 5]; }
  uint8_t *getData() { return getFrameData() + Rx64Response::getDataOffset() + 6; }
  uint8_t getMeterId() { return getFrameData()[Rx64Response::getDataOffset() + 1]; }
  uint8_t getFunctionCode() { return getFrameData()[Rx64Response::getDataOffset() + 2]; }
  uint16_t getAddress() { return (getFrameData()[Rx64Response::getDataOffset() + 3] << 8) +
	                         (getFrameData()[Rx64Response::getDataOffset() + 4]); }
};

#endif
