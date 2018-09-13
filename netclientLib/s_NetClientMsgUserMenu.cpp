// Name		: s_NetClientMsgUserMenu.cpp
// Project	: Lib-NetClient

#include "StdAfx.h"
#include "s_NetClient.h"
#include "s_CClientConsoleMsg.h"
#include <stdlib.h>
#include "GLContrlMsg.h"
#include "china_md5.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <string>;

using namespace std;


// RegisterAccount
int CNetClient::SndRegisterAccount(
	const TCHAR* szUserID,
	const TCHAR* szPassw,
	const TCHAR* szPinCode,
	const TCHAR* szEmail,
	const TCHAR* szCaptcha1
)
{
	if (szUserID	== NULL) return NET_ERROR;
	if (szPassw		== NULL) return NET_ERROR;
	if (szPinCode	== NULL) return NET_ERROR;
	if (szEmail		== NULL) return NET_ERROR;
	if (szCaptcha1	== NULL) return NET_ERROR;

	NET_REGISTERACCOUNT_DATA nrad;

	nrad.nmg.nType = NET_MSG_REGISTERACCOUNT;
	CHINA_MD5::MD5 md5; // md5 해쉬용 클래스
	// 비밀번호의 문자열 길이를 구한다.
	size_t nLength = 0;
	HRESULT sRet = StringCchLength( szPassw, USR_PASS_LENGTH, &nLength );
	
	if (sRet == STRSAFE_E_INVALID_PARAMETER)
		return NET_ERROR;

	// 해쉬값을 얻기 위해서 패스워드 문자열을 넣는다.
	// 해쉬값을 얻기 위해 업데이트 해 준다.
	md5.update( reinterpret_cast<const unsigned char*> (szPassw), nLength );	
	// 얻어진 해쉬값을 strPass 에 넣는다.
	std::string strPass = md5.asString();
	::StringCchCopy(nrad.szUserID,		USR_ID_LENGTH + 1,			szUserID);
	::StringCchCopy(nrad.szPassw,		USR_PASS_LENGTH + 1,		strPass.c_str());
	::StringCchCopy(nrad.szPinCode,		USR_PASS_LENGTH + 1,		szPinCode);
	::StringCchCopy(nrad.szEmail,		CHR_ID_LENGTH + 1,			szEmail);
	::StringCchCopy(nrad.szCaptcha1,	USR_RAND_PASS_LENGTH + 1,	szCaptcha1);
	::StringCchCopy(nrad.szEnCrypt,		ENCRYPT_KEY + 1,			m_szEncryptKey);

	m_Tea.encrypt(nrad.szUserID,	USR_ID_LENGTH + 1);
	m_Tea.encrypt(nrad.szPassw,		USR_PASS_LENGTH + 1);
	m_Tea.encrypt(nrad.szPinCode,	USR_PASS_LENGTH + 1);
	m_Tea.encrypt(nrad.szEmail,		CHR_ID_LENGTH + 1);
	m_Tea.encrypt(nrad.szCaptcha1,	USR_RAND_PASS_LENGTH + 1);
	m_Tea.encrypt(nrad.szEnCrypt,	ENCRYPT_KEY + 1);

	return Send((char *)&nrad);
}

// ForgotAccount
int CNetClient::SndForgotAccount(
	const TCHAR* szEmail,
	const TCHAR* szPinCode,
	const TCHAR* szCaptcha1
)
{
	if (szEmail		== NULL) return NET_ERROR;
	if (szPinCode	== NULL) return NET_ERROR;	
	if (szCaptcha1	== NULL) return NET_ERROR;

	NET_FORGOTACCOUNT_DATA nfad;

	nfad.nmg.nType = NET_MSG_FORGOTACCOUNT;

	::StringCchCopy(nfad.szEmail,		CHR_ID_LENGTH + 1,			szEmail);
	::StringCchCopy(nfad.szPinCode,		USR_PASS_LENGTH + 1,		szPinCode);
	::StringCchCopy(nfad.szCaptcha1,	USR_RAND_PASS_LENGTH + 1,	szCaptcha1);
	::StringCchCopy(nfad.szEnCrypt,		ENCRYPT_KEY + 1,			m_szEncryptKey);

	m_Tea.encrypt(nfad.szEmail,		CHR_ID_LENGTH + 1);
	m_Tea.encrypt(nfad.szPinCode,	USR_PASS_LENGTH + 1);
	m_Tea.encrypt(nfad.szCaptcha1,	USR_RAND_PASS_LENGTH + 1);
	m_Tea.encrypt(nfad.szEnCrypt,	ENCRYPT_KEY + 1);

	return Send((char *)&nfad);
}

// ChangePassword
int CNetClient::SndChangePassword(
	const TCHAR* szUserID,
	const TCHAR* szOldPassw,
	const TCHAR* szNewPassw,
	const TCHAR* szCaptcha1
)
{
	if (szUserID	== NULL) return NET_ERROR;
	if (szOldPassw	== NULL) return NET_ERROR;
	if (szNewPassw	== NULL) return NET_ERROR;
	if (szCaptcha1	== NULL) return NET_ERROR;

	NET_CHANGEPASSWORD_DATA ncpd;
	
	ncpd.nmg.nType = NET_MSG_CHANGEPASSWORD;
	size_t nLength = 0;
	HRESULT sRet = StringCchLength( szOldPassw, USR_PASS_LENGTH, &nLength );
	
	if (sRet == STRSAFE_E_INVALID_PARAMETER)
		return NET_ERROR;

	
	CHINA_MD5::MD5 md5;
	md5.update( reinterpret_cast<const unsigned char*> (szOldPassw), nLength );
	std::string strOldPass = md5.asString();

	::StringCchCopy(ncpd.szUserID,		USR_ID_LENGTH + 1,			szUserID);
	::StringCchCopy(ncpd.szOldPassw,	USR_PASS_LENGTH + 1,		strOldPass.c_str());
	::StringCchCopy(ncpd.szNewPassw,	USR_PASS_LENGTH + 1,		szNewPassw);
	::StringCchCopy(ncpd.szCaptcha1,	USR_RAND_PASS_LENGTH + 1,	szCaptcha1);
	::StringCchCopy(ncpd.szEnCrypt,		ENCRYPT_KEY + 1,			m_szEncryptKey);

	m_Tea.encrypt(ncpd.szUserID,	USR_ID_LENGTH + 1);
	m_Tea.encrypt(ncpd.szOldPassw,	USR_PASS_LENGTH + 1);
	m_Tea.encrypt(ncpd.szNewPassw,	USR_PASS_LENGTH + 1);
	m_Tea.encrypt(ncpd.szCaptcha1,	USR_RAND_PASS_LENGTH + 1);
	m_Tea.encrypt(ncpd.szEnCrypt,	ENCRYPT_KEY + 1);

	return Send((char *)&ncpd);
}


// ChangePinCode
int CNetClient::SndChangePinCode(
	const TCHAR* szUserID,
	const TCHAR* szPassw,
	const TCHAR* szOldPinCode,
	const TCHAR* szNewPinCode,
	const TCHAR* szCaptcha1
)
{
	if (szUserID == NULL) return NET_ERROR;
	if (szPassw == NULL) return NET_ERROR;
	if (szOldPinCode == NULL) return NET_ERROR;
	if (szNewPinCode == NULL) return NET_ERROR;
	if (szCaptcha1 == NULL) return NET_ERROR;

	NET_CHANGEPINCODE_DATA ncpd;
	
	ncpd.nmg.nType = NET_MSG_CHANGEPINCODE;

	size_t nLength = 0;

	HRESULT sRet = StringCchLength( szPassw, USR_PASS_LENGTH, &nLength );
	
	if (sRet == STRSAFE_E_INVALID_PARAMETER)
		return NET_ERROR;

	CHINA_MD5::MD5 md5;
	md5.update( reinterpret_cast<const unsigned char*> (szPassw), nLength );
	std::string strPass = md5.asString();
	::StringCchCopy(ncpd.szUserID, USR_ID_LENGTH + 1, szUserID);
	::StringCchCopy(ncpd.szPassw, USR_PASS_LENGTH + 1, strPass.c_str());
	::StringCchCopy(ncpd.szOldPinCode, USR_PASS_LENGTH + 1, szOldPinCode);
	::StringCchCopy(ncpd.szNewPinCode, USR_PASS_LENGTH + 1, szNewPinCode);
	::StringCchCopy(ncpd.szCaptcha1, USR_RAND_PASS_LENGTH + 1, szCaptcha1);
	::StringCchCopy(ncpd.szEnCrypt, ENCRYPT_KEY + 1, m_szEncryptKey);

	m_Tea.encrypt(ncpd.szUserID, USR_ID_LENGTH + 1);
	m_Tea.encrypt(ncpd.szPassw, USR_PASS_LENGTH + 1);
	m_Tea.encrypt(ncpd.szOldPinCode, USR_PASS_LENGTH + 1);
	m_Tea.encrypt(ncpd.szNewPinCode, USR_PASS_LENGTH + 1);
	m_Tea.encrypt(ncpd.szCaptcha1, USR_RAND_PASS_LENGTH + 1);
	m_Tea.encrypt(ncpd.szEnCrypt, ENCRYPT_KEY + 1);

	return Send((char *)&ncpd);
}

// FixUser
int CNetClient::SndFixUser(
	const TCHAR* szUserID,
	const TCHAR* szPassw,
	const TCHAR* szCaptcha1
)
{
	if (szUserID	== NULL) return NET_ERROR;
	if (szPassw		== NULL) return NET_ERROR;
	if (szCaptcha1	== NULL) return NET_ERROR;

	NET_FIXUSER_DATA nfud;
	nfud.nmg.nType = NET_MSG_FIXUSER;

	size_t nLength = 0;
	HRESULT sRet = StringCchLength( szPassw, USR_PASS_LENGTH, &nLength );
	
	if (sRet == STRSAFE_E_INVALID_PARAMETER)
		return NET_ERROR;

	CHINA_MD5::MD5 md5;
	md5.update( reinterpret_cast<const unsigned char*> (szPassw), nLength );
	std::string strPass = md5.asString();
	::StringCchCopy(nfud.szUserID,		USR_ID_LENGTH + 1,			szUserID);
	::StringCchCopy(nfud.szPassw,		USR_PASS_LENGTH + 1,		strPass.c_str());
	::StringCchCopy(nfud.szCaptcha1,	USR_RAND_PASS_LENGTH + 1,	szCaptcha1);
	::StringCchCopy(nfud.szEnCrypt,		ENCRYPT_KEY + 1,			m_szEncryptKey);

	m_Tea.encrypt(nfud.szUserID,	USR_ID_LENGTH + 1);
	m_Tea.encrypt(nfud.szPassw,		USR_PASS_LENGTH + 1);
	m_Tea.encrypt(nfud.szCaptcha1,	USR_RAND_PASS_LENGTH + 1);
	m_Tea.encrypt(nfud.szEnCrypt,	ENCRYPT_KEY + 1);

	return Send((char *)&nfud);
}


