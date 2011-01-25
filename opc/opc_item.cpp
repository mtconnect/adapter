#include "stdafx.h"
#include "opc_item.hpp"
#include "device_datum.hpp"

COPCItem :: COPCItem(CComPtr< IOPCServer > aServer, const char *aTag)
:   mError            (S_OK)
,   mItemResult      (NULL)
,   mOPCServer(aServer)
{
///////////////////////////////////
//
//  Initialisierung der OPCITEMDEF Struktur
    ZeroMemory(&mItemDef, sizeof(mItemDef));
    mItemDef.szAccessPath = new WCHAR[256];
    ZeroMemory(mItemDef.szAccessPath, sizeof(WCHAR)*256);

    mItemDef.szItemID     = new WCHAR[256];
    mbstowcs(mItemDef.szItemID, aTag, strlen(aTag)+1);

    mItemDef.bActive      = TRUE;

    mItemDef.hClient      = (OPCHANDLE)this;      //erleichtert später die datenausgabe

    mItemDef.dwBlobSize   = 0;

    mItemDef.pBlob        = NULL;

    mItemDef.vtRequestedDataType  = VT_EMPTY;

    ::VariantInit(&mResult);
}

COPCItem :: ~COPCItem()
{
    delete [] mItemDef.szAccessPath;
    delete [] mItemDef.szItemID;

    ::VariantClear(&mResult);

    if (NULL != mItemResult)
    {
        CoTaskMemFree(mItemResult);
        mItemResult = NULL;
    }
}


/*##{method}####################################################################

--------------------------------------------------------------------------------

  Description: Ausgabe eines Fehlers

   Parameters:

Return Values:

--------------------------------------------------------------------------------
  History:
#######################################################################{end}##*/
long COPCItem :: OutPutError(HRESULT hError)
{
    HRESULT hRes;
    WCHAR *pszError;
    hRes = mOPCServer->GetErrorString(hError, GetUserDefaultLCID(), &pszError);
    if (FAILED(hRes))
    {
        CString strError;
        strError.Format("Error: 0x%08X", hError);
        return 1;
    }
	printf("Error: %ls\n", pszError);
    CoTaskMemFree(pszError);
    return 0;
}

long COPCItem :: setCurrentResult(OPCITEMSTATE &tState, HRESULT hError)
{
    HRESULT hRes;
    if (S_OK != hError)
    {
        return OutPutError(hError);
    }

	mError = hError;
    VariantCopy(&mResult, &tState.vDataValue);
    VariantClear(&tState.vDataValue);

    return 0;
}
