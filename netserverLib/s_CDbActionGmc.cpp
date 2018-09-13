// Name		: s_CDbActionGmc.cpp
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

// CAgentSndGenVoc
CAgentSndGenVoc::CAgentSndGenVoc(
	const TCHAR* szUserID,
	const TCHAR* szVocID,
	const TCHAR* szVocCodes,
	const TCHAR* szNominal,
	DWORD dwClient,
	const TCHAR* szUserIP,
	USHORT uPort
)
{
	m_strUserID		= szUserID;
	m_strVocID		= szVocID;
	m_strVocCodes	= szVocCodes;
	m_strNominal	= szNominal;
	m_dwClient		= dwClient;
	m_strUserIP		= szUserIP;
	m_nPort			= uPort;
}

int CAgentSndGenVoc::Execute(CServer* pServer)
{
	CAgentServer* pTemp = reinterpret_cast<CAgentServer*> (pServer);

	if (pServer == NULL) return NET_ERROR;

	int nResult = COdbcManager::GetInstance()->GMCSndGenVoc(
		m_strUserID.GetString(),
		m_strVocID.GetString(),
		m_strVocCodes.GetString(),
		m_strNominal.GetString()
	);

	pTemp->MsgSndGenVocBack(
		nResult,
		m_dwClient,
		m_strUserIP.GetString(),
		m_uPort
	);

	return NET_OK;
}

// SndSrchVoc
CAgentSndSrchVoc::CAgentSndSrchVoc(
	const TCHAR* szUserID,
	int nSrchCode,
	const TCHAR* szInput,
	DWORD dwClient,
	const TCHAR* szUserIP,
	USHORT uPort
)
{
	m_strUserID = szUserID;
	m_nSrchCode = nSrchCode;
	m_strInput = szInput;
	m_dwClient = dwClient;
	m_strUserIP = szUserIP;
	m_nPort = uPort;
}

int CAgentSndSrchVoc::Execute(CServer* pServer)
{
	CAgentServer* pTemp = reinterpret_cast<CAgentServer*> (pServer);

	if (pServer == NULL) return NET_ERROR;

	int nResult = COdbcManager::GetInstance()->GMCSndSrchVoc(
		m_strUserID.GetString(),
		m_nSrchCode,
		m_strInput.GetString()
	);

	pTemp->MsgSndSrchVocBack(
		nResult,
		m_dwClient,
		m_strUserIP.GetString(),
		m_uPort
	);

	return NET_OK;
}