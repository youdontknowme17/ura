// Name		: s_CDbActionUserMenu.cpp
// Project	: Lib-NetServer

#include "StdAfx.h"

#include "s_CDbAction.h"
#include "s_CSessionServer.h"
#include "s_CFieldServer.h"
#include "s_CAgentServer.h"

#include "../RanClientLib/G-Logic/GLCharData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// RegisterAccount
CAgentRegisterAccount::CAgentRegisterAccount(
	const TCHAR* szUserID,
	const TCHAR* szPassw,
	const TCHAR* szPinCode,
	const TCHAR* szEmail,
	const TCHAR* szCaptcha1,
	int nRandomNum,
	DWORD dwClient,
	const TCHAR* szUserIP,
	USHORT uPort
)
{
	m_strUserID		= szUserID;
	m_strPassw		= szPassw;
	m_strPinCode	= szPinCode;
	m_strEmail		= szEmail;
	m_strCaptcha1	= szCaptcha1;
	m_nRandomNum	= nRandomNum;
	m_dwClient		= dwClient;
	m_strUserIP		= szUserIP;
	m_nPort			= uPort;
}

int CAgentRegisterAccount::Execute(CServer* pServer)
{
	CAgentServer* pTemp = reinterpret_cast<CAgentServer*> (pServer);

	if (pServer == NULL) return NET_ERROR;

	int nResult = COdbcManager::GetInstance()->RegisterAccount(
		m_strUserID.GetString(),
		m_strPassw.GetString(),
		m_strPinCode.GetString(),
		m_strEmail.GetString(),
		m_strCaptcha1.GetString(),
		m_nRandomNum
	);

	pTemp->MsgSndRegisterAccountBack(
		nResult,
		m_dwClient,
		m_strUserIP.GetString(),
		m_uPort
	);

	return NET_OK;
}

// ForgotAccount
CAgentForgotAccount::CAgentForgotAccount(
	const TCHAR* szEmail,
	const TCHAR* szPinCode,
	const TCHAR* szCaptcha1,
	int nRandomNum,
	DWORD dwClient,
	const TCHAR* szUserIP,
	USHORT uPort
)
{
	m_strEmail		= szEmail;
	m_strPinCode	= szPinCode;
	m_strCaptcha1	= szCaptcha1;
	m_nRandomNum	= nRandomNum;
	m_dwClient		= dwClient;
	m_strUserIP		= szUserIP;
	m_nPort			= uPort;
}

int CAgentForgotAccount::Execute(CServer* pServer)
{
	CAgentServer* pTemp = reinterpret_cast<CAgentServer*> (pServer);

	if (pServer == NULL) return NET_ERROR;

	int nResult = COdbcManager::GetInstance()->ForgotAccount(
		m_strEmail.GetString(),
		m_strPinCode.GetString(),		
		m_strCaptcha1.GetString(),
		m_nRandomNum
	);

	pTemp->MsgSndForgotAccountBack(
		nResult,
		m_dwClient,
		m_strUserIP.GetString(),
		m_uPort
	);

	return NET_OK;
}

// ChangePassword
CAgentChangePassword::CAgentChangePassword(
	const TCHAR* szUserID,
	const TCHAR* szOldPassw,
	const TCHAR* szNewPassw,
	const TCHAR* szCaptcha1,
	int nRandomNum,
	DWORD dwClient,
	const TCHAR* szUserIP,
	USHORT uPort
)
{
	m_strUserID		= szUserID;
	m_strOldPassw	= szOldPassw;
	m_strNewPassw	= szNewPassw;
	m_strCaptcha1	= szCaptcha1;
	m_nRandomNum	= nRandomNum;
	m_dwClient		= dwClient;
	m_strUserIP		= szUserIP;
	m_nPort			= uPort;
}

int CAgentChangePassword::Execute(CServer* pServer)
{
	CAgentServer* pTemp = reinterpret_cast<CAgentServer*> (pServer);

	if (pServer == NULL) return NET_ERROR;

	int nResult = COdbcManager::GetInstance()->ChangePassword(
		m_strUserID.GetString(),
		m_strOldPassw.GetString(),
		m_strNewPassw.GetString(),
		m_strCaptcha1.GetString(),
		m_nRandomNum
	);

	pTemp->MsgSndChangePasswordBack(
		nResult,
		m_dwClient,
		m_strUserIP.GetString(),
		m_uPort
	);

	return NET_OK;
}

// ChangePinCode
CAgentChangePinCode::CAgentChangePinCode(
	const TCHAR* szUserID,
	const TCHAR* szPassw,
	const TCHAR* szOldPinCode,
	const TCHAR* szNewPinCode,
	const TCHAR* szCaptcha1,
	int nRandomNum,
	DWORD dwClient,
	const TCHAR* szUserIP,
	USHORT uPort
)
{
	m_strUserID = szUserID;
	m_strPassw = szPassw;
	m_strOldPinCode = szOldPinCode;
	m_strNewPinCode = szNewPinCode;
	m_strCaptcha1 = szCaptcha1;
	m_nRandomNum = nRandomNum;
	m_dwClient = dwClient;
	m_strUserIP = szUserIP;
	m_nPort = uPort;
}

int CAgentChangePinCode::Execute(CServer* pServer)
{
	CAgentServer* pTemp = reinterpret_cast<CAgentServer*> (pServer);

	if (pServer == NULL) return NET_ERROR;

	int nResult = COdbcManager::GetInstance()->ChangePinCode(
		m_strUserID.GetString(),
		m_strPassw.GetString(),
		m_strOldPinCode.GetString(),
		m_strNewPinCode.GetString(),
		m_strCaptcha1.GetString(),
		m_nRandomNum
	);

	pTemp->MsgSndChangePinCodeBack(
		nResult,
		m_dwClient,
		m_strUserIP.GetString(),
		m_uPort
	);

	return NET_OK;
}
// FixUser
CAgentFixUser::CAgentFixUser(
	const TCHAR* szUserID,
	const TCHAR* szPassw,
	const TCHAR* szCaptcha1,
	int nRandomNum,
	DWORD dwClient,
	const TCHAR* szUserIP,
	USHORT uPort
)
{
	m_strUserID		= szUserID;
	m_strPassw		= szPassw;
	m_strCaptcha1	= szCaptcha1;
	m_nRandomNum	= nRandomNum;
	m_dwClient		= dwClient;
	m_strUserIP		= szUserIP;
	m_nPort			= uPort;
}

int CAgentFixUser::Execute(CServer* pServer)
{
	CAgentServer* pTemp = reinterpret_cast<CAgentServer*> (pServer);

	if (pServer == NULL) return NET_ERROR;

	int nResult = COdbcManager::GetInstance()->FixUser(
		m_strUserID.GetString(),
		m_strPassw.GetString(),
		m_strCaptcha1.GetString(),
		m_nRandomNum
	);

	pTemp->MsgSndFixUserBack(
		nResult,
		m_dwClient,
		m_strUserIP.GetString(),
		m_uPort
	);

	return NET_OK;
}