// Name		: s_COdbcUserMenu.cpp
// Project	: Lib-NetServer

#include "StdAfx.h"
#include "s_COdbcManager.h"
#include "s_CDbAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// RegisterAccount
int COdbcManager::RegisterAccount(
	const TCHAR* szUserID,
	const TCHAR* szPassw,
	const TCHAR* szPinCode,
	const TCHAR* szEmail,
	const TCHAR* szCaptcha1,
	int nRandomNum
)
{
	CString strUserID = szUserID;
	strUserID.Trim(_T(" "));
	strUserID.Replace(_T("'"), _T("''"));

	CString strPassw = szPassw;
	strPassw.Trim(_T(" "));
	strPassw.Replace(_T("'"), _T("''"));

	CString strPinCode = szPinCode;
	strPinCode.Trim(_T(" "));
	strPinCode.Replace(_T("'"), _T("''"));

	CString strEmail = szEmail;
	strEmail.Trim(_T(" "));
	strEmail.Replace(_T("'"), _T("''"));

	CString strCaptcha1 = szCaptcha1;
	strCaptcha1.Trim(_T(" "));
	strCaptcha1.Replace(_T("'"), _T("''"));

	TCHAR szRandomNum[10] = { 0 };
	_itot(nRandomNum, szRandomNum, 10);

	TCHAR szTemp[512] = { 0 };

	_snprintf(szTemp, 512, "{call UserMenuRegisterAccount('%s','%s','%s','%s','%s','%s', ?)}",
		strUserID.GetString(),
		strPassw.GetString(),
		strPinCode.GetString(),
		strEmail.GetString(),
		strCaptcha1.GetString(),
		szRandomNum
	);

	int nReturn = m_pUserDB->ExecuteSpInt(szTemp);

	return nReturn;
}

// ForgotAccount
int COdbcManager::ForgotAccount(
	const TCHAR* szEmail,
	const TCHAR* szPinCode,
	const TCHAR* szCaptcha1,
	int nRandomNum
)
{
	CString strEmail = szEmail;
	strEmail.Trim(_T(" "));
	strEmail.Replace(_T("'"), _T("''"));

	CString strPinCode = szPinCode;
	strPinCode.Trim(_T(" "));
	strPinCode.Replace(_T("'"), _T("''"));

	CString strCaptcha1 = szCaptcha1;
	strCaptcha1.Trim(_T(" "));
	strCaptcha1.Replace(_T("'"), _T("''"));

	TCHAR szRandomNum[10] = { 0 };
	_itot(nRandomNum, szRandomNum, 10);

	TCHAR szTemp[512] = { 0 };

	_snprintf(szTemp, 512, "{call UserMenuForgotAccount('%s','%s','%s','%s', ?)}",
		strEmail.GetString(),
		strPinCode.GetString(),
		strCaptcha1.GetString(),
		szRandomNum
	);

	int nReturn = m_pUserDB->ExecuteSpInt(szTemp);

	return nReturn;
}

// ChangePassword
int COdbcManager::ChangePassword(
	const TCHAR* szUserID,
	const TCHAR* szOldPassw,
	const TCHAR* szNewPassw,
	const TCHAR* szCaptcha1,
	int nRandomNum
)
{
	CString strUserID = szUserID;
	strUserID.Trim(_T(" "));
	strUserID.Replace(_T("'"), _T("''"));

	CString strOldPassw = szOldPassw;
	strOldPassw.Trim(_T(" "));
	strOldPassw.Replace(_T("'"), _T("''"));

	CString strNewPassw = szNewPassw;
	strNewPassw.Trim(_T(" "));
	strNewPassw.Replace(_T("'"), _T("''"));

	CString strCaptcha1 = szCaptcha1;
	strCaptcha1.Trim(_T(" "));
	strCaptcha1.Replace(_T("'"), _T("''"));

	TCHAR szRandomNum[10] = { 0 };
	_itot(nRandomNum, szRandomNum, 10);

	TCHAR szTemp[512] = { 0 };

	_snprintf(szTemp, 512, "{call UserMenuChangePassword('%s','%s','%s','%s','%s', ?)}",
		strUserID.GetString(),
		strOldPassw.GetString(),
		strNewPassw.GetString(),
		strCaptcha1.GetString(),
		szRandomNum
	);

	int nReturn = m_pUserDB->ExecuteSpInt(szTemp);

	return nReturn;
}

// ChangePinCode
int COdbcManager::ChangePinCode(
	const TCHAR* szUserID,
	const TCHAR* szPassw,
	const TCHAR* szOldPinCode,
	const TCHAR* szNewPinCode,
	const TCHAR* szCaptcha1,
	int nRandomNum
)
{
	CString strUserID = szUserID;
	strUserID.Trim(_T(" "));
	strUserID.Replace(_T("'"), _T("''"));

	CString strPassw = szPassw;
	strPassw.Trim(_T(" "));
	strPassw.Replace(_T("'"), _T("''"));

	CString strOldPinCode = szOldPinCode;
	strOldPinCode.Trim(_T(" "));
	strOldPinCode.Replace(_T("'"), _T("''"));

	CString strNewPinCode = szNewPinCode;
	strNewPinCode.Trim(_T(" "));
	strNewPinCode.Replace(_T("'"), _T("''"));

	CString strCaptcha1 = szCaptcha1;
	strCaptcha1.Trim(_T(" "));
	strCaptcha1.Replace(_T("'"), _T("''"));

	TCHAR szRandomNum[10] = { 0 };
	_itot(nRandomNum, szRandomNum, 10);

	TCHAR szTemp[512] = { 0 };

	_snprintf(szTemp, 512, "{call UserMenuChangePinCode('%s','%s','%s','%s','%s','%s', ?)}",
		strUserID.GetString(),
		strPassw.GetString(),
		strOldPinCode.GetString(),
		strNewPinCode.GetString(),
		strCaptcha1.GetString(),
		szRandomNum
	);

	int nReturn = m_pUserDB->ExecuteSpInt(szTemp);

	return nReturn;
}
// FixUser
int COdbcManager::FixUser(
	const TCHAR* szUserID,
	const TCHAR* szPassw,
	const TCHAR* szCaptcha1,
	int nRandomNum
)
{
	CString strUserID = szUserID;
	strUserID.Trim(_T(" "));
	strUserID.Replace(_T("'"), _T("''"));

	CString strPassw = szPassw;
	strPassw.Trim(_T(" "));
	strPassw.Replace(_T("'"), _T("''"));

	CString strCaptcha1 = szCaptcha1;
	strCaptcha1.Trim(_T(" "));
	strCaptcha1.Replace(_T("'"), _T("''"));

	TCHAR szRandomNum[10] = { 0 };
	_itot(nRandomNum, szRandomNum, 10);

	TCHAR szTemp[512] = { 0 };

	_snprintf(szTemp, 512, "{call UserMenuFixUser('%s','%s','%s','%s', ?)}",
		strUserID.GetString(),
		strPassw.GetString(),
		strCaptcha1.GetString(),
		szRandomNum
	);

	int nReturn = m_pUserDB->ExecuteSpInt(szTemp);

	return nReturn;
}