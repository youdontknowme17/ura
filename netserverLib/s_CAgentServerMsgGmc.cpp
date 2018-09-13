// Name		: s_CAgentServerMsgGmc.cpp
// Project	: Lib-NetServer

#include "StdAfx.h"
#include "s_CAgentServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// MsgSndGenVoc
void CAgentServer::MsgSndGenVoc(MSG_LIST* pMsg)
{
	NET_SNDGENVOC_DATA* nsgvd = reinterpret_cast<NET_SNDGENVOC_DATA*> (pMsg->Buffer);

	if (sizeof(NET_SNDGENVOC_DATA) != nsgvd->nmg.dwSize)
	{
		CConsoleMessage::GetInstance()->Write(
			_T("ERROR:NET_SNDGENVOC_DATA Wrong Message Size"));
		return;
	}

	DWORD dwClient = pMsg->dwClient;

	m_Tea.decrypt(nsgvd->szVocID, USR_ID_LENGTH + 1);
	m_Tea.decrypt(nsgvd->szVocCodes, USR_ID_LENGTH + 1);
	m_Tea.decrypt(nsgvd->szNominal, USR_RAND_PASS_LENGTH + 1);
	m_Tea.decrypt(nsgvd->szEnCrypt, ENCRYPT_KEY + 1);

	TCHAR szVocID[USR_ID_LENGTH + 1] = { 0 };
	TCHAR szVocCodes[USR_ID_LENGTH + 1] = { 0 };
	TCHAR szNominal[USR_RAND_PASS_LENGTH + 1] = { 0 };
	TCHAR szEncrypt[ENCRYPT_KEY + 1] = { 0 };

	StringCchCopy(szVocID, USR_ID_LENGTH + 1, nsgvd->szVocID);
	StringCchCopy(szVocCodes, USR_ID_LENGTH + 1, nsgvd->szVocCodes);
	StringCchCopy(szNominal, USR_RAND_PASS_LENGTH + 1, nsgvd->szNominal);
	StringCchCopy(szEncrypt, ENCRYPT_KEY + 1, nsgvd->szEnCrypt);

	if (_tcscmp(szEncrypt, m_szEncrypt))
	{
		NET_SNDGENVOC_FB_DATA	nsgvfbd;
		nsgvfbd.nmg.nType	= NET_MSG_SNDGENVOC_FB;
		nsgvfbd.nResult		= EM_SNDGENVOC_FB_SUB_FAIL;
		SendClient(dwClient, &nsgvfbd);

		CConsoleMessage::GetInstance()->Write(
			_T("ERROR:SndGenVoc Not Correct Encrypt UserID : %s"), nsgvd->szUserID);

		return;
	}

	CAgentSndGenVoc* pAction = new CAgentSndGenVoc(
		m_pClientManager->GetUserID(dwClient),
		szVocID,
		szVocCodes,
		szNominal,
		dwClient,
		m_pClientManager->GetClientIP(dwClient),
		m_pClientManager->GetClientPort(dwClient)
	);

	COdbcManager::GetInstance()->AddJob((CDbAction*)pAction);
}

// MsgSndGenVocBack
void CAgentServer::MsgSndGenVocBack(
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
		NET_SNDGENVOC_FB_DATA nsgvfbd;
		nsgvfbd.nmg.nType	= NET_MSG_SNDGENVOC_FB;
		nsgvfbd.nResult		= EM_SNDGENVOC_FB_SUB_FAIL;

		CConsoleMessage::GetInstance()->Write("CAgentSndGenVoc result %d", nResult);

		SendClient(dwClient, &nsgvfbd);
	}
	break;

	case 0:
	{
		NET_SNDGENVOC_FB_DATA nsgvfbd;
		nsgvfbd.nmg.nType = NET_MSG_SNDGENVOC_FB;
		nsgvfbd.nResult = EM_SNDGENVOC_FB_SUB_FAIL;

		CConsoleMessage::GetInstance()->Write("CAgentSndGenVoc result %d", nResult);

		SendClient(dwClient, &nsgvfbd);
	}
	break;

	case 1:
	{
		NET_SNDGENVOC_FB_DATA nsgvfbd;
		nsgvfbd.nmg.nType = NET_MSG_SNDGENVOC_FB;
		nsgvfbd.nResult = EM_SNDGENVOC_FB_SUB_OK;

		CConsoleMessage::GetInstance()->Write("CAgentSndGenVoc result %d", nResult);

		SendClient(dwClient, &nsgvfbd);
	}
	break;

	default:
	{
		NET_SNDGENVOC_FB_DATA nsgvfbd;
		nsgvfbd.nmg.nType = NET_MSG_SNDGENVOC_FB;
		nsgvfbd.nResult = EM_SNDGENVOC_FB_SUB_FAIL;

		CConsoleMessage::GetInstance()->Write("CAgentSndGenVoc result %d", nResult);

		SendClient(dwClient, &nsgvfbd);
	}
	break;
	}
}

// MsgSndSrchVoc
void CAgentServer::MsgSndSrchVoc(MSG_LIST* pMsg)
{
	NET_SNDSRCHVOC_DATA* nssvd = reinterpret_cast<NET_SNDSRCHVOC_DATA*> (pMsg->Buffer);

	if (sizeof(NET_SNDSRCHVOC_DATA) != nssvd->nmg.dwSize)
	{
		CConsoleMessage::GetInstance()->Write(
			_T("ERROR:NET_SNDSRCHVOC_DATA Wrong Message Size"));
		return;
	}

	DWORD dwClient = pMsg->dwClient;

	m_Tea.decrypt(nssvd->szInput, USR_ID_LENGTH + 1);
	m_Tea.decrypt(nssvd->szEnCrypt, ENCRYPT_KEY + 1);

	TCHAR szInput[USR_ID_LENGTH + 1] = { 0 };
	TCHAR szEncrypt[ENCRYPT_KEY + 1] = { 0 };

	StringCchCopy(szInput, USR_ID_LENGTH + 1, nssvd->szInput);
	StringCchCopy(szEncrypt, ENCRYPT_KEY + 1, nssvd->szEnCrypt);

	if (_tcscmp(szEncrypt, m_szEncrypt))
	{
		NET_SNDSRCHVOC_FB_DATA	nssvfbd;
		nssvfbd.nmg.nType = NET_MSG_SNDSRCHVOC_FB;
		nssvfbd.nResult = EM_SNDSRCHVOC_FB_SUB_FAIL;
		SendClient(dwClient, &nssvfbd);

		CConsoleMessage::GetInstance()->Write(
			_T("ERROR:SndSrchVoc Not Correct Encrypt UserID : %s"), nssvd->szUserID);

		return;
	}

	int nSrchCode = nssvd->nSrchCode;

	CAgentSndSrchVoc* pAction = new CAgentSndSrchVoc(
		m_pClientManager->GetUserID(dwClient),
		nSrchCode,
		szInput,
		dwClient,
		m_pClientManager->GetClientIP(dwClient),
		m_pClientManager->GetClientPort(dwClient)
	);

	COdbcManager::GetInstance()->AddJob((CDbAction*)pAction);
}

// MsgSndSrchVocBack
void CAgentServer::MsgSndSrchVocBack(
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
		NET_SNDSRCHVOC_FB_DATA nssvfbd;
		nssvfbd.nmg.nType = NET_MSG_SNDSRCHVOC_FB;
		nssvfbd.nResult = EM_SNDSRCHVOC_FB_SUB_FAIL;

		CConsoleMessage::GetInstance()->Write("CAgentSndSrchVoc result %d", nResult);

		SendClient(dwClient, &nssvfbd);
	}
	break;

	case 0:
	{
		NET_SNDSRCHVOC_FB_DATA nssvfbd;
		nssvfbd.nmg.nType = NET_MSG_SNDSRCHVOC_FB;
		nssvfbd.nResult = EM_SNDSRCHVOC_FB_SUB_FAIL;

		CConsoleMessage::GetInstance()->Write("CAgentSndSrchVoc result %d", nResult);

		SendClient(dwClient, &nssvfbd);
	}
	break;

	case 1:
	{
		NET_SNDSRCHVOC_FB_DATA nssvfbd;
		nssvfbd.nmg.nType = NET_MSG_SNDSRCHVOC_FB;
		nssvfbd.nResult = EM_SNDSRCHVOC_FB_SUB_NONE;

		CConsoleMessage::GetInstance()->Write("CAgentSndSrchVoc result %d", nResult);

		SendClient(dwClient, &nssvfbd);
	}
	break;

	case 2:
	{
		NET_SNDSRCHVOC_FB_DATA nssvfbd;
		nssvfbd.nmg.nType = NET_MSG_SNDSRCHVOC_FB;
		nssvfbd.nResult = EM_SNDSRCHVOC_FB_SUB_USERID_UNCLAIMED;

		CConsoleMessage::GetInstance()->Write("CAgentSndSrchVoc result %d", nResult);

		SendClient(dwClient, &nssvfbd);
	}
	break;

	case 3:
	{
		NET_SNDSRCHVOC_FB_DATA nssvfbd;
		nssvfbd.nmg.nType = NET_MSG_SNDSRCHVOC_FB;
		nssvfbd.nResult = EM_SNDSRCHVOC_FB_SUB_USERID_CLAIMED;

		CConsoleMessage::GetInstance()->Write("CAgentSndSrchVoc result %d", nResult);

		SendClient(dwClient, &nssvfbd);
	}
	break;

	case 4:
	{
		NET_SNDSRCHVOC_FB_DATA nssvfbd;
		nssvfbd.nmg.nType = NET_MSG_SNDSRCHVOC_FB;
		nssvfbd.nResult = EM_SNDSRCHVOC_FB_SUB_VOCID_UNCLAIMED;

		CConsoleMessage::GetInstance()->Write("CAgentSndSrchVoc result %d", nResult);

		SendClient(dwClient, &nssvfbd);
	}
	break;

	case 5:
	{
		NET_SNDSRCHVOC_FB_DATA nssvfbd;
		nssvfbd.nmg.nType = NET_MSG_SNDSRCHVOC_FB;
		nssvfbd.nResult = EM_SNDSRCHVOC_FB_SUB_VOCID_CLAIMED;

		CConsoleMessage::GetInstance()->Write("CAgentSndSrchVoc result %d", nResult);

		SendClient(dwClient, &nssvfbd);
	}
	break;

	default:
	{
		NET_SNDSRCHVOC_FB_DATA nssvfbd;
		nssvfbd.nmg.nType = NET_MSG_SNDSRCHVOC_FB;
		nssvfbd.nResult = EM_SNDSRCHVOC_FB_SUB_FAIL;

		CConsoleMessage::GetInstance()->Write("CAgentSndSrchVoc result %d", nResult);

		SendClient(dwClient, &nssvfbd);
	}
	break;
	}
}