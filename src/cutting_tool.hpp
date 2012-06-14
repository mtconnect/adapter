
#ifndef CUTTING_TOOL_HPP
#define CUTTING_TOOL_HPP

#include <string>
#include <map>
#include <vector>
#include <float.h>

class CuttingToolProperty {
public:
  CuttingToolProperty(const std::string &aName, std::map<std::string, std::string> &aAttributes, const std::string &aValue) 
    : mName(aName), mAttributes(aAttributes), mValue(aValue)
  {}
  
  CuttingToolProperty(std::string aName)
    : mName(aName)
  {}

  CuttingToolProperty(const CuttingToolProperty &aProp)
    : mName(aProp.mName), mAttributes(aProp.mAttributes), mValue(aProp.mValue)
  {}

  virtual std::string toXML();
  
public:
  std::string mName;
  std::map<std::string,std::string> mAttributes;
  std::string mValue;
};

class CuttingToolStatus : public CuttingToolProperty {
public:
  CuttingToolStatus(std::vector<std::string> &aStatus) 
    : CuttingToolProperty("CutterStatus"), mStatus(aStatus)
  { }

  CuttingToolStatus(const CuttingToolStatus &aStatus)
    : CuttingToolProperty(aStatus), mStatus(aStatus.mStatus)
  {}

  virtual std::string toXML();

public:
  std::vector<std::string> mStatus;  
};

static const double CT_NO_VALUE = DBL_MAX;

class CuttingToolMeasurement : public CuttingToolProperty {
public:
  CuttingToolMeasurement(std::string aName, std::string aCode, double aValue, double aNominal = CT_NO_VALUE, 
                         double aMin = CT_NO_VALUE, double aMax = CT_NO_VALUE,  
                         std::string aNativeUnits = "", 
                         std::string aUnits = "");

  CuttingToolMeasurement(const CuttingToolMeasurement &aMeasure)
    : CuttingToolProperty(aMeasure)
  {}
};

class CuttingItem {
public:
  std::vector<CuttingToolProperty> mProperties;
  std::vector<CuttingToolMeasurement> mMeasurements;
};

class CuttingTool {
public:
  CuttingTool(std::string &aAssetId, int aToolNumber, std::string &aDescription,
    CuttingToolStatus &aStatus);

  void add(CuttingToolProperty &aProp) { mProperties.push_back(aProp); }
  void add(CuttingToolMeasurement &aProp) { mMeasurements.push_back(aProp); }
  
  virtual std::string toString();
  
  const std::string &getAssetId() const { return mAssetId; }
  int getToolNumber() const { return mToolNumber; }

  bool isValid() const { return !mAssetId.empty(); }

protected:
  CuttingToolStatus mStatus;
  std::vector<CuttingToolProperty> mProperties;
  std::vector<CuttingToolMeasurement> mMeasurements;
  std::string mAssetId;
  int mToolNumber;
  std::string mDescription;
};

#endif