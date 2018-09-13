// Name		: s_CAgentServerMsgUserMenu.cpp
// Project	: Lib-NetServer

#include "StdAfx.h"
#include "s_CAgentServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// RegisterAccount
void CAgentServer::MsgSndRegisterAccount(MSG_LIST* pMsg)
{
	if (pMsg == NULL) return;

	NET_REGISTERACCOUNT_DATA* nrad = reinterpret_cast<NET_REGISTERACCOUNT_DATA*> (pMsg->Buffer);

	if (sizeof(NET_REGISTERACCOUNT_DATA) != nrad->nmg.dwSize)
	{
		CConsoleMessage::GetInstance()->Write(
			_T("ERROR:NET_REGISTERACCOUNT_DATA Wrong Message Size"));
		return;
	}

	DWORD dwClient = pMsg->dwClient;

	m_Tea.decrypt(nrad->szUserID, USR_ID_LENGTH + 1);
	m_Tea.decrypt(nrad->szPassw, USR_PASS_LENGTH + 1);
	m_Tea.decrypt(nrad->szPinCode, USR_PASS_LENGTH + 1);
	m_Tea.decrypt(nrad->szEmail, CHR_ID_LENGTH + 1);
	m_Tea.decrypt(nrad->szCaptcha1, USR_RAND_PASS_LENGTH + 1);
	m_Tea.decrypt(nrad->szEnCrypt, ENCRYPT_KEY + 1);

	TCHAR szUserID[USR_ID_LENGTH + 1] = { 0 };
	TCHAR szPassw[USR_PASS_LENGTH + 1] = { 0 };
	TCHAR szPinCode[USR_PASS_LENGTH + 1] = { 0 };
	TCHAR szEmail[CHR_ID_LENGTH + 1] = { 0 };
	TCHAR szCaptcha1[USR_RAND_PASS_LENGTH + 1] = { 0 };
	TCHAR szEncrypt[ENCRYPT_KEY + 1] = { 0 };

	StringCchCopy(szUserID, USR_ID_LENGTH + 1, nrad->szUserID);
	StringCchCopy(szPassw, USR_PASS_LENGTH + 1, nrad->szPassw);
	StringCchCopy(szPinCode, USR_PASS_LENGTH + 1, nrad->szPinCode);
	StringCchCopy(szEmail, CHR_ID_LENGTH + 1, nrad->szEmail);
	StringCchCopy(szCaptcha1, USR_RAND_PASS_LENGTH + 1, nrad->szCaptcha1);
	StringCchCopy(szEncrypt, ENCRYPT_KEY + 1, nrad->szEnCrypt);

	if (_tcscmp(szEncrypt, m_szEncrypt))
	{

		NET_REGISTERACCOUNT_FB_DATA	nrafbd;
		nrafbd.nmg.nType = NET_MSG_REGISTERACCOUNT_FB;
		nrafbd.nResult = EM_REGISTERACCOUNT_FB_SUB_FAIL;
		SendClient(dwClient, &nrafbd);

		CConsoleMessage::GetInstance()->Write(
			_T("ERROR:SndMsgRegisterAccount Not Correct Encrypt UserID : %s"), nrad->szUserID);

		return;
	}

	CAgentRegisterAccount* pAction = new CAgentRegisterAccount(
		szUserID,
		szPassw,
		szPinCode,
		szEmail,
		szCaptcha1,
		m_pClientManager->GetRandomPassNumber(dwClient),
		dwClient,
		m_pClientManager->GetClientIP(dwClient),
		m_pClientManager->GetClientPort(dwClient)
	);

	COdbcManager::GetInstance()->AddJob((CDbAction*)pAction);
}

void CAgentServer::MsgSndRegisterAccountBack(
	int nResult,
	DWORD dwClient,
	const TCHAR* szUserIP,
	USHORT uPort
)
{
	switch (nResult)
	{
	case DB_ERROR:
	{
		NET_REGISTERACCOUNT_FB_DATA nrafbd;
		nrafbd.nmg.nType	= NET_MSG_REGISTERACCOUNT_FB;
		nrafbd.nResult		= EM_REGISTERACCOUNT_FB_SUB_FAIL;

		CConsoleMessage::GetInstance()->Write("CAgentRegisterAccount result %d", nResult);

		SendClient(dwClient, &nrafbd);
	}
	break;

	case 0:
	{
		NET_REGISTERACCOUNT_FB_DATA nrafbd;
		nrafbd.nmg.nType	= NET_MSG_REGISTERACCOUNT_FB;
		nrafbd.nResult		= EM_REGISTERACCOUNT_FB_SUB_FAIL;

		CConsoleMessage::GetInstance()->Write("CAgentRegisterAccount result %d", nResult);

		SendClient(dwClient, &nrafbd);
	}
	break;

	case 1:
	{
		NET_REGISTERACCOUNT_FB_DATA nrafbd;
		nrafbd.nmg.nType	= NET_MSG_REGISTERACCOUNT_FB;
		nrafbd.nResult		= EM_REGISTERACCOUNT_FB_SUB_INCORRECT;

		CConsoleMessage::GetInstance()->Write("CAgentRegisterAccount result %d", nResult);

		SendClient(dwClient, &nrafbd);
	}
	break;

	case 2:
	{
		NET_REGISTERACCOUNT_FB_DATA nrafbd;
		nrafbd.nmg.nType	= NET_MSG_REGISTERACCOUNT_FB;
		nrafbd.nResult		= EM_REGISTERACCOUNT_FB_SUB_RANDOM_PASS;

		CConsoleMessage::GetInstance()->Write("CAgentRegisterAccount result %d", nResult);

		SendClient(dwClient, &nrafbd);
	}
	break;

	case 3:
	{
		NET_REGISTERACCOUNT_FB_DATA nrafbd;
		nrafbd.nmg.nType	= NET_MSG_REGISTERACCOUNT_FB;
		nrafbd.nResult		= EM_REGISTERACCOUNT_FB_SUB_OK;

		CConsoleMessage::GetInstance()->Write("CAgentRegisterAccount result %d", nResult);

		SendClient(dwClient, &nrafbd);
	}
	break;
	//Add by mcalimosa 09/08/2018
	case 4:
	{
		NET_REGISTERACCOUNT_FB_DATA nrafbd;
		nrafbd.nmg.nType	= NET_MSG_REGISTERACCOUNT_FB;
		nrafbd.nResult		= EM_REGISTERACCOUNT_FB_SUB_INUSE_EMAIL;

		CConsoleMessage::GetInstance()->Write("CAgentRegisterAccount result %d", nResult);

		SendClient(dwClient, &nrafbd);
	}
	break;

	default:
	{
		NET_REGISTERACCOUNT_FB_DATA nrafbd;
		nrafbd.nmg.nType	= NET_MSG_REGISTERACCOUNT_FB;
		nrafbd.nResult		= EM_REGISTERACCOUNT_FB_SUB_FAIL;

		CConsoleMessage::GetInstance()->Write("CAgentRegisterAccount result %d", nResult);

		SendClient(dwClient, &nrafbd);
	}
	break;
	}
}

// ForgotAccount
void CAgentServer::MsgSndForgotAccount(MSG_LIST* pMsg)
{
	if (pMsg == NULL) return;

	NET_FORGOTACCOUNT_DATA* nfad = reinterpret_cast<NET_FORGOTACCOUNT_DATA*> (pMsg->Buffer);

	if (sizeof(NET_FORGOTACCOUNT_DATA) != nfad->nmg.dwSize)
	{
		CConsoleMessage::GetInstance()->Write(
			_T("ERROR:NET_FORGOTACCOUNT_DATA Wrong Message Size"));
		return;
	}

	DWORD dwClient = pMsg->dwClient;

	m_Tea.decrypt(nfad->szEmail, CHR_ID_LENGTH + 1);
	m_Tea.decrypt(nfad->szPinCode, USR_PASS_LENGTH + 1);
	m_Tea.decrypt(nfad->szCaptcha1, USR_RAND_PASS_LENGTH + 1);
	m_Tea.decrypt(nfad->szEnCrypt, ENCRYPT_KEY + 1);

	TCHAR szEmail[CHR_ID_LENGTH + 1] = { 0 };
	TCHAR szPinCode[USR_PASS_LENGTH + 1] = { 0 };	
	TCHAR szCaptcha1[USR_RAND_PASS_LENGTH + 1] = { 0 };
	TCHAR szEncrypt[ENCRYPT_KEY + 1] = { 0 };

	StringCchCopy(szEmail, CHR_ID_LENGTH + 1, nfad->szEmail);
	StringCchCopy(szPinCode, USR_PASS_LENGTH + 1, nfad->szPinCode);	
	StringCchCopy(szCaptcha1, USR_RAND_PASS_LENGTH + 1, nfad->szCaptcha1);
	StringCchCopy(szEncrypt, ENCRYPT_KEY + 1, nfad->szEnCrypt);

	if (_tcscmp(szEncrypt, m_szEncrypt))
	{

		NET_FORGOTACCOUNT_FB_DATA	nfafbd;
		nfafbd.nmg.nType	= NET_MSG_FORGOTACCOUNT_FB;
		nfafbd.nResult		= EM_FORGOTACCOUNT_FB_SUB_FAIL;
		SendClient(dwClient, &nfafbd);

		CConsoleMessage::GetInstance()->Write(
			_T("ERROR:SndMsgForgotAccount Not Correct Encrypt UserEmail : %s"), nfad->szEmail);

		return;
	}

	CAgentForgotAccount* pAction = new CAgentForgotAccount(
		szEmail,
		szPinCode,
		szCaptcha1,
		m_pClientManager->GetRandomPassNumber(dwClient),
		dwClient,
		m_pClientManager->GetClientIP(dwClient),
		m_pClientManager->GetClientPort(dwClient)
	);

	COdbcManager::GetInstance()->AddJob((CDbAction*)pAction);
}

void CAgentServer::MsgSndForgotAccountBack(
	int nResult,
	DWORD dwClient,
	const TCHAR* szUserIP,
	USHORT uPort
)
{
	switch (nResult)
	{
	case DB_ERROR:
	{
		NET_FORGOTACCOUNT_FB_DATA nfafbd;
		nfafbd.nmg.nType = NET_MSG_FORGOTACCOUNT_FB;
		nfafbd.nResult = EM_FORGOTACCOUNT_FB_SUB_FAIL;

		CConsoleMessage::GetInstance()->Write("CAgentForgotAccount result %d", nResult);

		SendClient(dwClient, &nfafbd);
	}
	break;

	case 0:
	{
		NET_FORGOTACCOUNT_FB_DATA nfafbd;
		nfafbd.nmg.nType = NET_MSG_FORGOTACCOUNT_FB;
		nfafbd.nResult = EM_FORGOTACCOUNT_FB_SUB_FAIL;

		CConsoleMessage::GetInstance()->Write("CAgentForgotAccount result %d", nResult);

		SendClient(dwClient, &nfafbd);
	}
	break;

	case 1:
	{
		NET_FORGOTACCOUNT_FB_DATA nfafbd;
		nfafbd.nmg.nType = NET_MSG_FORGOTACCOUNT_FB;
		nfafbd.nResult = EM_FORGOTACCOUNT_FB_SUB_INCORRECT;

		CConsoleMessage::GetInstance()->Write("CAgentForgotAccount result %d", nResult);

		SendClient(dwClient, &nfafbd);
	}
	break;

	case 2:
	{
		NET_FORGOTACCOUNT_FB_DATA nfafbd;
		nfafbd.nmg.nType = NET_MSG_FORGOTACCOUNT_FB;
		nfafbd.nResult = EM_FORGOTACCOUNT_FB_SUB_RANDOM_PASS;

		CConsoleMessage::GetInstance()->Write("CAgentForgotAccount result %d", nResult);

		SendClient(dwClient, &nfafbd);
	}
	break;

	case 3:
	{
		NET_FORGOTACCOUNT_FB_DATA nfafbd;
		nfafbd.nmg.nType = NET_MSG_FORGOTACCOUNT_FB;
		nfafbd.nResult = EM_FORGOTACCOUNT_FB_SUB_OK;

		CConsoleMessage::GetInstance()->Write("CAgentForgotAccount result %d", nResult);

		SendClient(dwClient, &nfafbd);
	}
	break;

	default:
	{
		NET_FORGOTACCOUNT_FB_DATA nfafbd;
		nfafbd.nmg.nType = NET_MSG_FORGOTACCOUNT_FB;
		nfafbd.nResult = EM_FORGOTACCOUNT_FB_SUB_FAIL;

		CConsoleMessage::GetInstance()->Write("CAgentForgotAccount result %d", nResult);

		SendClient(dwClient, &nfafbd);
	}
	break;
	}
}

// ChangePassword
void CAgentServer::MsgSndChangePassword(MSG_LIST* pMsg)
{
	if (pMsg == NULL) return;

	NET_CHANGEPASSWORD_DATA* ncpd = reinterpret_cast<NET_CHANGEPASSWORD_DATA*> (pMsg->Buffer);

	if (sizeof(NET_CHANGEPASSWORD_DATA) != ncpd->nmg.dwSize)
	{
		CConsoleMessage::GetInstance()->Write(
			_T("ERROR:NET_CHANGEPASSW_DATA Wrong Message Size"));
		return;
	}

	DWORD dwClient = pMsg->dwClient;

	m_Tea.decrypt(ncpd->szUserID, USR_ID_LENGTH + 1);
	m_Tea.decrypt(ncpd->szOldPassw, USR_PASS_LENGTH + 1);
	m_Tea.decrypt(ncpd->szNewPassw, USR_PASS_LENGTH + 1);
	m_Tea.decrypt(ncpd->szCaptcha1, USR_RAND_PASS_LENGTH + 1);
	m_Tea.decrypt(ncpd->szEnCrypt, ENCRYPT_KEY + 1);

	TCHAR szUserID[USR_ID_LENGTH + 1] = { 0 };
	TCHAR szOldPassw[USR_PASS_LENGTH + 1] = { 0 };
	TCHAR szNewPassw[USR_PASS_LENGTH + 1] = { 0 };
	TCHAR szCaptcha1[USR_RAND_PASS_LENGTH + 1] = { 0 };
	TCHAR szEncrypt[ENCRYPT_KEY + 1] = { 0 };

	StringCchCopy(szUserID, USR_ID_LENGTH + 1, ncpd->szUserID);
	StringCchCopy(szOldPassw, USR_PASS_LENGTH + 1, ncpd->szOldPassw);
	StringCchCopy(szNewPassw, USR_PASS_LENGTH + 1, ncpd->szNewPassw);
	StringCchCopy(szCaptcha1, USR_RAND_PASS_LENGTH + 1, ncpd->szCaptcha1);
	StringCchCopy(szEncrypt, ENCRYPT_KEY + 1, ncpd->szEnCrypt);

	if (_tcscmp(szEncrypt, m_szEncrypt))
	{

		NET_CHANGEPASSWORD_FB_DATA	ncpfbd;
		ncpfbd.nmg.nType	= NET_MSG_CHANGEPASSWORD_FB;
		ncpfbd.nResult		= EM_CHANGEPASSWORD_FB_SUB_FAIL;
		SendClient(dwClient, &ncpfbd);

		CConsoleMessage::GetInstance()->Write(
			_T("ERROR:MsgChangePassword Not Correct Encrypt UserID : %s"), ncpd->szUserID);

		return;
	}

	CAgentChangePassword* pAction = new CAgentChangePassword(
		szUserID,
		szOldPassw,
		szNewPassw,
		szCaptcha1,
		m_pClientManager->GetRandomPassNumber(dwClient),
		dwClient,
		m_pClientManager->GetClientIP(dwClient),
		m_pClientManager->GetClientPort(dwClient)
	);

	COdbcManager::GetInstance()->AddJob((CDbAction*)pAction);
}

void CAgentServer::MsgSndChangePasswordBack(
	int nResult,
	DWORD dwClient,
	const TCHAR* szUserIP,
	USHORT uPort
)
{
	switch (nResult)
	{
	case DB_ERROR:
	{
		NET_CHANGEPASSWORD_FB_DATA ncpfbd;
		ncpfbd.nmg.nType	= NET_MSG_CHANGEPASSWORD_FB;
		ncpfbd.nResult		= EM_CHANGEPASSWORD_FB_SUB_FAIL;

		CConsoleMessage::GetInstance()->Write("CAgentChangePassword result %d", nResult);

		SendClient(dwClient, &ncpfbd);
	}
	break;

	case 0:
	{
		NET_CHANGEPASSWORD_FB_DATA ncpfbd;
		ncpfbd.nmg.nType = NET_MSG_CHANGEPASSWORD_FB;
		ncpfbd.nResult = EM_CHANGEPASSWORD_FB_SUB_FAIL;

		CConsoleMessage::GetInstance()->Write("CAgentChangePassword result %d", nResult);

		SendClient(dwClient, &ncpfbd);
	}
	break;

	case 1:
	{
		NET_CHANGEPASSWORD_FB_DATA ncpfbd;
		ncpfbd.nmg.nType	= NET_MSG_CHANGEPASSWORD_FB;
		ncpfbd.nResult		= EM_CHANGEPASSWORD_FB_SUB_INCORRECT;

		CConsoleMessage::GetInstance()->Write("CAgentChangePassword result %d", nResult);

		SendClient(dwClient, &ncpfbd);
	}
	break;

	case 2:
	{
		NET_CHANGEPASSWORD_FB_DATA ncpfbd;
		ncpfbd.nmg.nType	= NET_MSG_CHANGEPASSWORD_FB;
		ncpfbd.nResult		= EM_CHANGEPASSWORD_FB_SUB_RANDOM_PASS;

		CConsoleMessage::GetInstance()->Write("CAgentChangePassword result %d", nResult);

		SendClient(dwClient, &ncpfbd);
	}
	break;

	case 3:
	{
		NET_CHANGEPASSWORD_FB_DATA ncpfbd;
		ncpfbd.nmg.nType	= NET_MSG_CHANGEPASSWORD_FB;
		ncpfbd.nResult		= EM_CHANGEPASSWORD_FB_SUB_OK;

		CConsoleMessage::GetInstance()->Write("CAgentChangePassword result %d", nResult);

		SendClient(dwClient, &ncpfbd);
	}
	break;

	default:
	{
		NET_CHANGEPASSWORD_FB_DATA ncpfbd;
		ncpfbd.nmg.nType	= NET_MSG_CHANGEPASSWORD_FB;
		ncpfbd.nResult		= EM_CHANGEPASSWORD_FB_SUB_FAIL;

		CConsoleMessage::GetInstance()->Write("CAgentChangePassword result %d", nResult);

		SendClient(dwClient, &ncpfbd);
	}
	break;
	}
}


// ChangePinCode
void CAgentServer::MsgSndChangePinCode(MSG_LIST* pMsg)
{
	if (pMsg == NULL) return;

	NET_CHANGEPINCODE_DATA* ncpd = reinterpret_cast<NET_CHANGEPINCODE_DATA*> (pMsg->Buffer);

	if (sizeof(NET_CHANGEPINCODE_DATA) != ncpd->nmg.dwSize)
	{
		CConsoleMessage::GetInstance()->Write(
			_T("ERROR:NET_CHANGEPINCODE_DATA Wrong Message Size"));
		return;
	}

	DWORD dwClient = pMsg->dwClient;

	m_Tea.decrypt(ncpd->szUserID, USR_ID_LENGTH + 1);
	m_Tea.decrypt(ncpd->szPassw, USR_PASS_LENGTH + 1);
	m_Tea.decrypt(ncpd->szOldPinCode, USR_PASS_LENGTH + 1);
	m_Tea.decrypt(ncpd->szNewPinCode, USR_PASS_LENGTH + 1);
	m_Tea.decrypt(ncpd->szCaptcha1, USR_RAND_PASS_LENGTH + 1);
	m_Tea.decrypt(ncpd->szEnCrypt, ENCRYPT_KEY + 1);

	TCHAR szUserID[USR_ID_LENGTH + 1] = { 0 };
	TCHAR szPassw[USR_PASS_LENGTH + 1] = { 0 };
	TCHAR szOldPinCode[USR_PASS_LENGTH + 1] = { 0 };
	TCHAR szNewPinCode[USR_PASS_LENGTH + 1] = { 0 };
	TCHAR szCaptcha1[USR_RAND_PASS_LENGTH + 1] = { 0 };
	TCHAR szEncrypt[ENCRYPT_KEY + 1] = { 0 };

	StringCchCopy(szUserID, USR_ID_LENGTH + 1, ncpd->szUserID);
	StringCchCopy(szPassw, USR_PASS_LENGTH + 1, ncpd->szPassw);
	StringCchCopy(szOldPinCode, USR_PASS_LENGTH + 1, ncpd->szOldPinCode);
	StringCchCopy(szNewPinCode, USR_PASS_LENGTH + 1, ncpd->szNewPinCode);
	StringCchCopy(szCaptcha1, USR_RAND_PASS_LENGTH + 1, ncpd->szCaptcha1);
	StringCchCopy(szEncrypt, ENCRYPT_KEY + 1, ncpd->szEnCrypt);

	if (_tcscmp(szEncrypt, m_szEncrypt))
	{

		NET_CHANGEPINCODE_FB_DATA	ncpfbd;
		ncpfbd.nmg.nType = NET_MSG_CHANGEPINCODE_FB;
		ncpfbd.nResult = EM_CHANGEPINCODE_FB_SUB_FAIL;
		SendClient(dwClient, &ncpfbd);

		CConsoleMessage::GetInstance()->Write(
			_T("ERROR:MsgChangePinCode Not Correct Encrypt UserID : %s"), ncpd->szUserID);

		return;
	}

	CAgentChangePinCode* pAction = new CAgentChangePinCode(
		szUserID,
		szPassw,
		szOldPinCode,
		szNewPinCode,
		szCaptcha1,
		m_pClientManager->GetRandomPassNumber(dwClient),
		dwClient,
		m_pClientManager->GetClientIP(dwClient),
		m_pClientManager->GetClientPort(dwClient)
	);

	COdbcManager::GetInstance()->AddJob((CDbAction*)pAction);
}

void CAgentServer::MsgSndChangePinCodeBack(
	int nResult,
	DWORD dwClient,
	const TCHAR* szUserIP,
	USHORT uPort
)
{
	switch (nResult)
	{
	case DB_ERROR:
	{
		NET_CHANGEPINCODE_FB_DATA ncpfbd;
		ncpfbd.nmg.nType = NET_MSG_CHANGEPINCODE_FB;
		ncpfbd.nResult = EM_CHANGEPINCODE_FB_SUB_FAIL;

		CConsoleMessage::GetInstance()->Write("CAgentChangePinCode result %d", nResult);

		SendClient(dwClient, &ncpfbd);
	}
	break;

	case 0:
	{
		NET_CHANGEPINCODE_FB_DATA ncpfbd;
		ncpfbd.nmg.nType = NET_MSG_CHANGEPINCODE_FB;
		ncpfbd.nResult = EM_CHANGEPINCODE_FB_SUB_FAIL;

		CConsoleMessage::GetInstance()->Write("CAgentChangePinCode result %d", nResult);

		SendClient(dwClient, &ncpfbd);
	}
	break;

	case 1:
	{
		NET_CHANGEPINCODE_FB_DATA ncpfbd;
		ncpfbd.nmg.nType = NET_MSG_CHANGEPINCODE_FB;
		ncpfbd.nResult = EM_CHANGEPINCODE_FB_SUB_INCORRECT;

		CConsoleMessage::GetInstance()->Write("CAgentChangePinCode result %d", nResult);

		SendClient(dwClient, &ncpfbd);
	}
	break;

	case 2:
	{
		NET_CHANGEPINCODE_FB_DATA ncpfbd;
		ncpfbd.nmg.nType = NET_MSG_CHANGEPINCODE_FB;
		ncpfbd.nResult = EM_CHANGEPINCODE_FB_SUB_RANDOM_PIN;

		CConsoleMessage::GetInstance()->Write("CAgentChangePinCode result %d", nResult);

		SendClient(dwClient, &ncpfbd);
	}
	break;

	case 3:
	{
		NET_CHANGEPINCODE_FB_DATA ncpfbd;
		ncpfbd.nmg.nType = NET_MSG_CHANGEPINCODE_FB;
		ncpfbd.nResult = EM_CHANGEPINCODE_FB_SUB_OK;

		CConsoleMessage::GetInstance()->Write("CAgentChangePinCode result %d", nResult);

		SendClient(dwClient, &ncpfbd);
	}
	break;

	default:
	{
		NET_CHANGEPINCODE_FB_DATA ncpfbd;
		ncpfbd.nmg.nType = NET_MSG_CHANGEPINCODE_FB;
		ncpfbd.nResult = EM_CHANGEPINCODE_FB_SUB_FAIL;

		CConsoleMessage::GetInstance()->Write("CAgentChangePinCode result %d", nResult);

		SendClient(dwClient, &ncpfbd);
	}
	break;
	}
}

// FixUser
void CAgentServer::MsgSndFixUser(MSG_LIST* pMsg)
{
	if (pMsg == NULL) return;

	NET_FIXUSER_DATA* nfud = reinterpret_cast<NET_FIXUSER_DATA*> (pMsg->Buffer);

	if (sizeof(NET_FIXUSER_DATA) != nfud->nmg.dwSize)
	{
		CConsoleMessage::GetInstance()->Write(
			_T("ERROR:NET_FIXUSER_DATA Wrong Message Size"));
		return;
	}

	DWORD dwClient = pMsg->dwClient;

	m_Tea.decrypt(nfud->szUserID, USR_ID_LENGTH + 1);
	m_Tea.decrypt(nfud->szPassw, USR_PASS_LENGTH + 1);
	m_Tea.decrypt(nfud->szCaptcha1, USR_RAND_PASS_LENGTH + 1);
	m_Tea.decrypt(nfud->szEnCrypt, ENCRYPT_KEY + 1);

	TCHAR szUserID[USR_ID_LENGTH + 1] = { 0 };
	TCHAR szPassw[USR_PASS_LENGTH + 1] = { 0 };
	TCHAR szCaptcha1[USR_RAND_PASS_LENGTH + 1] = { 0 };
	TCHAR szEncrypt[ENCRYPT_KEY + 1] = { 0 };

	StringCchCopy(szUserID, USR_ID_LENGTH + 1, nfud->szUserID);
	StringCchCopy(szPassw, USR_PASS_LENGTH + 1, nfud->szPassw);
	StringCchCopy(szCaptcha1, USR_RAND_PASS_LENGTH + 1, nfud->szCaptcha1);
	StringCchCopy(szEncrypt, ENCRYPT_KEY + 1, nfud->szEnCrypt);

	if (_tcscmp(szEncrypt, m_szEncrypt))
	{

		NET_FIXUSER_FB_DATA	nfufbd;
		nfufbd.nmg.nType	= NET_MSG_FIXUSER_FB;
		nfufbd.nResult		= EM_FIXUSER_FB_SUB_FAIL;
		SendClient(dwClient, &nfufbd);

		CConsoleMessage::GetInstance()->Write(
			_T("ERROR:SndMsgFixUser Not Correct Encrypt UserID : %s"), nfud->szUserID);

		return;
	}

	CAgentFixUser* pAction = new CAgentFixUser(
		szUserID,
		szPassw,
		szCaptcha1,
		m_pClientManager->GetRandomPassNumber(dwClient),
		dwClient,
		m_pClientManager->GetClientIP(dwClient),
		m_pClientManager->GetClientPort(dwClient)
	);

	COdbcManager::GetInstance()->AddJob((CDbAction*)pAction);
}

void CAgentServer::MsgSndFixUserBack(
	int nResult,
	DWORD dwClient,
	const TCHAR* szUserIP,
	USHORT uPort
)
{
	switch (nResult)
	{
	case DB_ERROR:
	{
		NET_FIXUSER_FB_DATA nfufbd;
		nfufbd.nmg.nType	= NET_MSG_FIXUSER_FB;
		nfufbd.nResult		= EM_FIXUSER_FB_SUB_FAIL;

		CConsoleMessage::GetInstance()->Write("CAgentFixUser result %d", nResult);

		SendClient(dwClient, &nfufbd);
	}
	break;

	case 0:
	{
		NET_FIXUSER_FB_DATA nfufbd;
		nfufbd.nmg.nType	= NET_MSG_FIXUSER_FB;
		nfufbd.nResult		= EM_FIXUSER_FB_SUB_FAIL;

		CConsoleMessage::GetInstance()->Write("CAgentFixUser result %d", nResult);

		SendClient(dwClient, &nfufbd);
	}
	break;

	case 1:
	{
		NET_FIXUSER_FB_DATA nfufbd;
		nfufbd.nmg.nType	= NET_MSG_FIXUSER_FB;
		nfufbd.nResult		= EM_FIXUSER_FB_SUB_INCORRECT;

		CConsoleMessage::GetInstance()->Write("CAgentFixUser result %d", nResult);

		SendClient(dwClient, &nfufbd);
	}
	break;

	case 2:
	{
		NET_FIXUSER_FB_DATA nfufbd;
		nfufbd.nmg.nType	= NET_MSG_FIXUSER_FB;
		nfufbd.nResult		= EM_FIXUSER_FB_SUB_RANDOM_PASS;

		CConsoleMessage::GetInstance()->Write("CAgentFixUser result %d", nResult);

		SendClient(dwClient, &nfufbd);
	}
	break;

	case 3:
	{
		NET_FIXUSER_FB_DATA nfufbd;
		nfufbd.nmg.nType	= NET_MSG_FIXUSER_FB;
		nfufbd.nResult		= EM_FIXUSER_FB_SUB_OK;

		CConsoleMessage::GetInstance()->Write("CAgentFixUser result %d", nResult);

		SendClient(dwClient, &nfufbd);
	}
	break;

	default:
	{
		NET_FIXUSER_FB_DATA nfufbd;
		nfufbd.nmg.nType	= NET_MSG_FIXUSER_FB;
		nfufbd.nResult		= EM_FIXUSER_FB_SUB_FAIL;

		CConsoleMessage::GetInstance()->Write("CAgentFixUser result %d", nResult);

		SendClient(dwClient, &nfufbd);
	}
	break;
	}
}