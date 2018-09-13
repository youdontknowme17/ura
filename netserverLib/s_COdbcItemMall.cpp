// Name		: s_COdbcItemMall.cpp
// Project	: Lib-NetServer

#include "StdAfx.h"
#include "s_COdbcManager.h"
#include "s_CDbAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// TopUp01
int COdbcManager::OdbcTopUp01(
	const TCHAR* szVocID,
	const TCHAR* szVocCodes,
	const TCHAR* szCaptcha1,
	const TCHAR* szCaptcha2,
	const TCHAR* szUserID
)
{
	CString strVocID = szVocID;
	strVocID.Trim(_T(" "));
	strVocID.Replace(_T("'"), _T("''"));

	CString strVocCodes = szVocCodes;
	strVocCodes.Trim(_T(" "));
	strVocCodes.Replace(_T("'"), _T("''"));

	CString strCaptcha1 = szCaptcha1;
	strCaptcha1.Trim(_T(" "));
	strCaptcha1.Replace(_T("'"), _T("''"));

	CString strCaptcha2 = szCaptcha2;
	strCaptcha2.Trim(_T(" "));
	strCaptcha2.Replace(_T("'"), _T("''"));

	CString strUserID = szUserID;
	strUserID.Trim(_T(" "));
	strUserID.Replace(_T("'"), _T("''"));

	TCHAR szTemp[512] = { 0 };
	_snprintf(szTemp, 512, "{call OdbcTopUp01('%s','%s','%s','%s','%s', ?)}",
		strVocID.GetString(),
		strVocCodes.GetString(),
		strCaptcha1.GetString(),
		strCaptcha2.GetString(),
		strUserID.GetString()
	);

	int nReturn = m_pUserDB->ExecuteSpInt(szTemp);

	return nReturn;
}

// TopUp02
int COdbcManager::OdbcTopUp02(
	STOPUP* sTopUp
)
{
	SQLRETURN sReturn = 0;
	ODBC_STMT* pConn = m_pUserDB->GetConnection();
	if (!pConn) return DB_ERROR;

	CString strVocID = sTopUp->m_szVocID;
	strVocID.Trim(_T(" "));
	strVocID.Replace(_T("'"), _T("''"));

	CString strVocCodes = sTopUp->m_szVocCodes;
	strVocCodes.Trim(_T(" "));
	strVocCodes.Replace(_T("'"), _T("''"));

	TCHAR szTemp[256] = { 0 };
	_snprintf(szTemp, 256, "SELECT Nominal "
		"FROM GeneratedVoucher WHERE VoucherID='%s' AND VoucherCodes='%s' ",
		strVocID.GetString(), strVocCodes.GetString());

	sReturn = ::SQLExecDirect(pConn->hStmt,
		(SQLCHAR*)szTemp,
		SQL_NTS);

	if ((sReturn != SQL_SUCCESS) && (sReturn != SQL_SUCCESS_WITH_INFO))
	{
		Print(szTemp);
		Print(GetErrorString(pConn->hStmt));
		m_pUserDB->FreeConnection(pConn);

		return DB_ERROR;
	}

	LONGLONG	llChaSaldo = 0;
	SQLINTEGER	cbChaSaldo = SQL_NTS;

	while (true)
	{
		sReturn = ::SQLFetch(pConn->hStmt);
		if (sReturn == SQL_ERROR || sReturn == SQL_SUCCESS_WITH_INFO)
		{
			Print(szTemp);
			Print(GetErrorString(pConn->hStmt));
			m_pUserDB->FreeConnection(pConn);

			return DB_ERROR;
		}

		if (sReturn == SQL_SUCCESS || sReturn == SQL_SUCCESS_WITH_INFO)
		{
			::SQLGetData(pConn->hStmt, 1, SQL_C_SBIGINT, &llChaSaldo, 0, &cbChaSaldo);

			sTopUp->m_lnSaldo = llChaSaldo;
		}
		else
		{
			break;
		}

		Sleep(0);
	}

	m_pUserDB->FreeConnection(pConn);

	return DB_OK;
}