#ifndef _COPCITEM_HPP
#define _COPCITEM_HPP

/*##{class}#####################################################################

--------------------------------------------------------------------------------

  Description:


--------------------------------------------------------------------------------
  History:
#######################################################################{end}##*/
class DeviceDatum;

class COPCItem
{
    COPCItem() {};
public:

    COPCItem(CComPtr< IOPCServer > aServer, const char *aTag);
    virtual ~COPCItem();
	long OutPutError(HRESULT hError);
	long setCurrentResult(OPCITEMSTATE &tState, HRESULT hError);

	float getFloatValue() { return mResult.fltVal; }
	double getDoubleValue() { return mResult.dblVal; }
	BSTR getBSTRValue() { return mResult.bstrVal; }
	bool getBoolValue() { return mResult.boolVal; }
	long getLongValue() { return mResult.lVal; }
	short getShortValue() { return mResult.iVal; }
	char getByteValue() { return mResult.bVal; }
	int getIntValue() { return mResult.intVal; }
	unsigned int getUIntValue() { return mResult.uintVal; }


//
// Attributes
//
    OPCITEMDEF          mItemDef;         //Items Definitions Struktur für Gruppe
    OPCITEMRESULT       *mItemResult;    //Items Result Struktur von der Gruppe
    HRESULT             mError;           //Fehler des einzelnen Items    
    VARIANT             mResult;
    CComPtr< IOPCServer >   mOPCServer;
};

#endif //_COPCITEM_HPP
