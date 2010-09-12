
#ifndef KRISHNA_DATA
#define KRISHNA_DATA

#include "device_datum.hpp"
#include <vector>

class KrishnaSample : public Sample {
public:
  KrishnaSample(const char *aName, int aOffset, double aScaler, uint16_t aMin = 0, uint16_t aMax = 0) :
    Sample(aName), mMin(aMin), mMax(aMax), mOffset(aOffset), mScaler(aScaler) { }

  int getOffset() { return mOffset; }
  double getScaler() { return mScaler; }
  uint16_t minimum() { return mMin; }
  uint16_t maximum() { return mMax; }

  bool setValue(double aValue) { return Sample::setValue(aValue * mScaler); }
  
protected:
  int mOffset;
  double mScaler;
  bool mNonZero;
  uint16_t mMax, mMin;
};

class KrishnaData {
public:
  KrishnaData(int aAddress) : mAddress(aAddress), mRequestCount(0) {}
  
  ~KrishnaData() {
    delete[] mData;
    for (size_t i = 0; i < mSamples.size(); i++)
      delete mSamples[i];
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

  int  getDataLength() { return (mRequestCount + 1) * sizeof(uint16_t); }

  void unavailable() {
    for (size_t i = 0; i < mSamples.size(); i++) {
      mSamples[i]->unavailable();
    }
  }

  bool writeData(void *aData, int aLength) {
    int count = aLength / sizeof(uint16_t);
    uint16_t *data = (uint16_t*) aData;
    memset(mData, 0, getCount() * sizeof(uint16_t));
    for (int i = 0; i < count; i++) {
      mData[i] = ntohs(data[i]);
    }
    
    for (size_t i = 0; i < mSamples.size(); i++) {
      if ((mSamples[i]->minimum() > 0 && mData[i] < mSamples[i]->minimum()) || 
          (mSamples[i]->maximum() > 0 && mData[i] > mSamples[i]->maximum())) {
        gLogger->debug("%s must be >= %d and <= %d", 
			mSamples[i]->getName(), mSamples[i]->minimum(), 
			mSamples[i]->maximum());
        return false;
      }
    }
    
    return true;
  }
  
  void writeValues() {
    for (size_t i = 0; i < mSamples.size(); i++) {
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

class KrishnaRequest : public ZBTxRequest {
public:
  KrishnaRequest(XBeeAddress64 &addr64, uint16_t aAddress, uint16_t aCount) 
    :  ZBTxRequest(addr64, 0, 0), mAddress(aAddress), mCount(aCount)
  {
    mPayload[0] = htons(0x0103);
    mPayload[1] = htons(aAddress);
    mPayload[2] = htons(aCount);    
    
    setPayloadLength(sizeof(mPayload));
    setPayload((uint8_t*) mPayload);
  }
  
  uint16_t getCount() { return mCount; }
  uint16_t getDataAddress() { return mAddress; }
  
protected:
  uint16_t mPayload[3];
  uint16_t mAddress;
  uint16_t mCount;
};

#endif
