// Name		: s_CDbActionItemMall.cpp
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

// TopUp
CAgentTopUp::CAgentTopUp(
	const TCHAR* szVocID,
	const TCHAR* szVocCodes,
	const TCHAR* szCaptcha1,
	const TCHAR* szCaptcha2,
	DWORD dwClient,
	const TCHAR* szUserID,
	const TCHAR* szUserIP,
	USHORT uPort
)
{
	m_strUserID = szUserID;
	m_strVocID = szVocID;
	m_strVocCodes = szVocCodes;
	m_strCaptcha1 = szCaptcha1;
	m_strCaptcha2 = szCaptcha2;
	m_dwClient = dwClient;
	m_strUserID = szUserID;
	m_strUserIP = szUserIP;
	m_nPort = uPort;
}

int CAgentTopUp::Execute(CServer* pServer)
{
	CAgentServer* pTemp = reinterpret_cast<CAgentServer*> (pServer);

	if (pServer == NULL) return NET_ERROR;

	int nResult = COdbcManager::GetInstance()->OdbcTopUp01(
		m_strVocID.GetString(),
		m_strVocCodes.GetString(),
		m_strCaptcha1.GetString(),
		m_strCaptcha2.GetString(),
		m_strUserID.GetString()
	);

	if (nResult == 4)
	{
		::StringCchCopy(sTopUp.m_szVocID, VOC_ID_LENGTH + 1, m_strVocID.GetString());
		::StringCchCopy(sTopUp.m_szVocCodes, VOC_CODES_LENGTH + 1, m_strVocCodes.GetString());

		int nRetData = COdbcManager::GetInstance()->OdbcTopUp02(
			&sTopUp
		);
	}

	pTemp->MsgSndTopUpBack(
		&sTopUp,
		nResult,
		m_dwClient,
		m_strUserIP.GetString(),
		m_uPort
	);

	return NET_OK;
}