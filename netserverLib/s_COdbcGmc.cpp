// Name		: s_COdbcGmc.cpp
// Project	: Lib-NetServer

#include "StdAfx.h"
#include "s_COdbcManager.h"
#include "s_CDbAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// GMCSndGenVoc
int COdbcManager::GMCSndGenVoc(
	const TCHAR* szUserID,
	const TCHAR* szVocID,
	const TCHAR* szVocCodes,
	const TCHAR* szNominal
)
{
	CString strUserID = szUserID;
	strUserID.Trim(_T(" "));
	strUserID.Replace(_T("'"), _T("''"));

	CString strVocID = szVocID;
	strVocID.Trim(_T(" "));
	strVocID.Replace(_T("'"), _T("''"));

	CString strVocCodes = szVocCodes;
	strVocCodes.Trim(_T(" "));
	strVocCodes.Replace(_T("'"), _T("''"));

	CString strNominal = szNominal;
	strNominal.Trim(_T(" "));
	strNominal.Replace(_T("'"), _T("''"));

	TCHAR szTemp[256] = { 0 };

	_snprintf(szTemp, 512, "{call GMCSndGenVoc('%s','%s','%s','%s', ?)}",
		strUserID.GetString(),
		strVocID.GetString(),
		strVocCodes.GetString(),
		strNominal.GetString()
	);

	int nReturn = m_pUserDB->ExecuteSpInt(szTemp);

	return nReturn;
}

// GMCSndSrchVoc
int COdbcManager::GMCSndSrchVoc(
	const TCHAR* szUserID,
	int nSrchCode,
	const TCHAR* szInput
)
{
	CString strUserID = szUserID;
	strUserID.Trim(_T(" "));
	strUserID.Replace(_T("'"), _T("''"));

	CString strInput = szInput;
	strInput.Trim(_T(" "));
	strInput.Replace(_T("'"), _T("''"));

	TCHAR szTemp[256] = { 0 };

	_snprintf(szTemp, 512, "{call GMCSndSrchVoc('%s','%d','%s', ?)}",
		strUserID.GetString(),
		nSrchCode,
		strInput.GetString()
	);

	int nReturn = m_pUserDB->ExecuteSpInt(szTemp);

	return nReturn;
}