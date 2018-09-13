// Name		: s_NetClientMsgGmc.cpp
// Project	: Lib-NetClient

#include "StdAfx.h"
#include "s_NetClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// SndGenVoc
int CNetClient::SndGenVoc(
	const TCHAR* szVocID,
	const TCHAR* szVocCodes,
	const TCHAR* szNominal
)
{
	if (szVocID == NULL) return NET_ERROR;
	if (szVocCodes == NULL) return NET_ERROR;
	if (szNominal == NULL) return NET_ERROR;

	NET_SNDGENVOC_DATA nsgvd;

	nsgvd.nmg.nType = NET_MSG_SNDGENVOC;

	::StringCchCopy(nsgvd.szVocID, USR_ID_LENGTH + 1, szVocID);
	::StringCchCopy(nsgvd.szVocCodes, USR_ID_LENGTH + 1, szVocCodes);
	::StringCchCopy(nsgvd.szNominal, USR_RAND_PASS_LENGTH + 1, szNominal);
	::StringCchCopy(nsgvd.szEnCrypt, ENCRYPT_KEY + 1, m_szEncryptKey);

	m_Tea.encrypt(nsgvd.szVocID, USR_ID_LENGTH + 1);
	m_Tea.encrypt(nsgvd.szVocCodes, USR_ID_LENGTH + 1);
	m_Tea.encrypt(nsgvd.szNominal, USR_RAND_PASS_LENGTH + 1);
	m_Tea.encrypt(nsgvd.szEnCrypt, ENCRYPT_KEY + 1);

	return Send((char *)&nsgvd);
}

// SndSrchVoc
int CNetClient::SndSrchVoc(
	int nSrchCode,
	const TCHAR* szInput)
{
	if (nSrchCode == 0) return NET_ERROR;
	if (szInput == NULL) return NET_ERROR;

	NET_SNDSRCHVOC_DATA nssvd;

	nssvd.nSrchCode = nSrchCode;

	nssvd.nmg.nType = NET_MSG_SNDSRCHVOC;

	::StringCchCopy(nssvd.szInput, USR_ID_LENGTH + 1, szInput);
	::StringCchCopy(nssvd.szEnCrypt, ENCRYPT_KEY + 1, m_szEncryptKey);

	m_Tea.encrypt(nssvd.szInput, USR_ID_LENGTH + 1);
	m_Tea.encrypt(nssvd.szEnCrypt, ENCRYPT_KEY + 1);

	return Send((char *)&nssvd);
}