// Name		: s_NetClientMsgItemMall.cpp
// Project	: Lib-NetClient

#include "StdAfx.h"
#include "s_NetClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// TopUp
int CNetClient::SndTopUp(
	const TCHAR* szVocID,
	const TCHAR* szVocCodes,
	const TCHAR* szCaptcha1,
	const TCHAR* szCaptcha2
)
{
	if (szVocID		== NULL) return NET_ERROR;
	if (szVocCodes	== NULL) return NET_ERROR;
	if (szCaptcha1	== NULL) return NET_ERROR;
	if (szCaptcha2	== NULL) return NET_ERROR;

	NET_TOPUP NTP;

	NTP.nmg.nType = NET_MSG_TOPUP;

	::StringCchCopy(NTP.szVocID,	VOC_ID_LENGTH + 1,			szVocID);
	::StringCchCopy(NTP.szVocCodes, VOC_CODES_LENGTH + 1,		szVocCodes);
	::StringCchCopy(NTP.szCaptcha1, USR_RAND_PASS_LENGTH + 1,	szCaptcha1);
	::StringCchCopy(NTP.szCaptcha2,	USR_RAND_PASS_LENGTH + 1,	szCaptcha2);
	::StringCchCopy(NTP.szEnCrypt,	ENCRYPT_KEY + 1,			m_szEncryptKey);

	m_Tea.encrypt(NTP.szVocID,		VOC_ID_LENGTH + 1);
	m_Tea.encrypt(NTP.szVocCodes,	VOC_CODES_LENGTH + 1);
	m_Tea.encrypt(NTP.szCaptcha1,	USR_RAND_PASS_LENGTH + 1);
	m_Tea.encrypt(NTP.szCaptcha2,	USR_RAND_PASS_LENGTH + 1);
	m_Tea.encrypt(NTP.szEnCrypt,	ENCRYPT_KEY + 1);

	return Send((char *)&NTP);
}