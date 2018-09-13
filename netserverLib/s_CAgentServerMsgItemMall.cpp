// Name		: s_CAgentServerMsgItemMall.cpp
// Project	: Lib-NetServer

#include "StdAfx.h"
#include "s_CAgentServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// TopUp
void CAgentServer::MsgSndTopUp(MSG_LIST* pMsg)
{
	if (pMsg == NULL) return;

	NET_TOPUP* NTP = reinterpret_cast<NET_TOPUP*> (pMsg->Buffer);

	if (sizeof(NET_TOPUP) != NTP->nmg.dwSize)
	{
		CConsoleMessage::GetInstance()->Write(
			_T("ERROR:NET_TOPUP Wrong Message Size"));
		return;
	}

	DWORD dwClient = pMsg->dwClient;

	m_Tea.decrypt(NTP->szVocID,		VOC_ID_LENGTH + 1);
	m_Tea.decrypt(NTP->szVocCodes,	VOC_CODES_LENGTH + 1);
	m_Tea.decrypt(NTP->szCaptcha1,	USR_RAND_PASS_LENGTH + 1);
	m_Tea.decrypt(NTP->szCaptcha2,	USR_RAND_PASS_LENGTH + 1);
	m_Tea.decrypt(NTP->szEnCrypt,	ENCRYPT_KEY + 1);

	TCHAR szVocID		[VOC_ID_LENGTH + 1]			= { 0 };
	TCHAR szVocCodes	[VOC_CODES_LENGTH + 1]		= { 0 };
	TCHAR szCaptcha1	[USR_RAND_PASS_LENGTH + 1]	= { 0 };
	TCHAR szCaptcha2	[USR_RAND_PASS_LENGTH + 1]	= { 0 };
	TCHAR szEncrypt		[ENCRYPT_KEY + 1]			= { 0 };

	StringCchCopy(szVocID,		VOC_ID_LENGTH + 1,			NTP->szVocID);
	StringCchCopy(szVocCodes,	VOC_CODES_LENGTH + 1,		NTP->szVocCodes);
	StringCchCopy(szCaptcha1,	USR_RAND_PASS_LENGTH + 1,	NTP->szCaptcha1);
	StringCchCopy(szCaptcha2,	USR_RAND_PASS_LENGTH + 1,	NTP->szCaptcha2);
	StringCchCopy(szEncrypt,	ENCRYPT_KEY + 1,			NTP->szEnCrypt);

	if (_tcscmp(szEncrypt, m_szEncrypt))
	{

		NET_TOPUP_FB NTPFB;
		NTPFB.nmg.nType = NET_MSG_TOPUP_FB;
		NTPFB.nResult = EM_TOPUP_FB_SUB_FAIL;
		SendClient(dwClient, &NTPFB);

		return;
	}

	CAgentTopUp* pAction = new CAgentTopUp(
		szVocID,
		szVocCodes,
		szCaptcha1,
		szCaptcha2,
		dwClient,
		m_pClientManager->GetUserID(dwClient),
		m_pClientManager->GetClientIP(dwClient),
		m_pClientManager->GetClientPort(dwClient)
	);

	COdbcManager::GetInstance()->AddJob((CDbAction*)pAction);
}

void CAgentServer::MsgSndTopUpBack(
	STOPUP* sTopUp,
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
		NET_TOPUP_FB NTPFB;
		NTPFB.nmg.nType = NET_MSG_TOPUP_FB;
		NTPFB.nResult = EM_TOPUP_FB_SUB_FAIL;

		CConsoleMessage::GetInstance()->Write("CAgentTopUp result %d", nResult);

		SendClient(dwClient, &NTPFB);
	}
	break;

	case 0:
	{
		NET_TOPUP_FB NTPFB;
		NTPFB.nmg.nType = NET_MSG_TOPUP_FB;
		NTPFB.nResult = EM_TOPUP_FB_SUB_FAIL;

		CConsoleMessage::GetInstance()->Write("CAgentTopUp result %d", nResult);

		SendClient(dwClient, &NTPFB);
	}
	break;

	case 1:
	{
		NET_TOPUP_FB NTPFB;
		NTPFB.nmg.nType = NET_MSG_TOPUP_FB;
		NTPFB.nResult = EM_TOPUP_FB_SUB_INCORRECT;

		CConsoleMessage::GetInstance()->Write("CAgentTopUp result %d", nResult);

		SendClient(dwClient, &NTPFB);
	}
	break;

	case 2:
	{
		NET_TOPUP_FB NTPFB;
		NTPFB.nmg.nType = NET_MSG_TOPUP_FB;
		NTPFB.nResult = EM_TOPUP_FB_SUB_USED;

		CConsoleMessage::GetInstance()->Write("CAgentTopUp result %d", nResult);

		SendClient(dwClient, &NTPFB);
	}
	break;

	case 3:
	{
		NET_TOPUP_FB NTPFB;
		NTPFB.nmg.nType = NET_MSG_TOPUP_FB;
		NTPFB.nResult = EM_TOPUP_FB_SUB_CAPTCHA;

		CConsoleMessage::GetInstance()->Write("CAgentTopUp result %d", nResult);

		SendClient(dwClient, &NTPFB);
	}
	break;

	case 4:
	{
		NET_TOPUP_FB NTPFB;
		NTPFB.nmg.nType = NET_MSG_TOPUP_FB;
		NTPFB.nResult = EM_TOPUP_FB_SUB_OK;
		NTPFB.m_lnSaldo = sTopUp->m_lnSaldo;

		CConsoleMessage::GetInstance()->Write("CAgentTopUp result %d", nResult);

		SendClient(dwClient, &NTPFB);
	}
	break;

	default:
	{
		NET_TOPUP_FB NTPFB;
		NTPFB.nmg.nType = NET_MSG_TOPUP_FB;
		NTPFB.nResult = EM_TOPUP_FB_SUB_FAIL;

		CConsoleMessage::GetInstance()->Write("CAgentTopUp result %d", nResult);

		SendClient(dwClient, &NTPFB);
	}
	break;
	}
}