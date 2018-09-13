#pragma	once

#include "../EngineUILib/GUInterface/UIMan.h"

//	PAGE ID
enum ET_UIGUID_OUTER
{
	SELECT_SERVER_PAGE = NO_ID + 1,
	SELECT_CHINA_AREA_PAGE,
	COPYRIGHT,
	CHINA_TITLE,
	SELECT_MENU_PAGE,
	NEW_LOGIN_PAGE,
	PASS_KEYBOARD,
	SELECT_USER_PAGE,
	REGISTER_PAGE,
	CHANGEPASSWORD_PAGE,
	CHANGEPINCODE_PAGE,
	FORGOTACCOUNT_PAGE,
	FIXUSER_PAGE,
	SELECT_CHARACTER_PAGE,
	//SELECT_CHARACTER_LEFTPAGE,
	CREATE_CHARACTER_PAGE,
	CREATE_CHARACTER_WEB,
	MODAL_WINDOW_OUTER,
	OUTER_UP_IMAGE,
	//LOGIN_MARK,
	OUTER_DN_IMAGE,
	FULLSCREEN_OUTER,
	FULLSCREEN_OUTER_DUMMY,
	WAIT_DISPLAY,
	SECPASS_SETPAGE,
	SECPASS_CHECKPAGE,
	MESSAGE_WINDOW,
	MESSAGE_WINDOW_DUMMY,
	OUTER_UP_LOGO_IMAGE,
	OUTER_UP_BG_IMAGE,
	NAME_PAGE,
};

//	MODAL CALLER
enum
{
	OUTER_MODAL_CLOSEGAME = NO_ID + 1,
	OUTER_MODAL_WAITSERVERINFO,
	OUTER_MODAL_RECONNECT,
	OUTER_MODAL_WAITCONFIRM,
	OUTER_MODAL_CREATEFAILED,
	OUTER_MODAL_ALREADYCONNECT,
	OUTER_MODAL_NON_PK_CHANNEL,
	OUTER_MODAL_CONNECTCLOSED,
	OUTER_MODAL_LOGOUT,
	OUTER_MODAL_SECONDPASSWORD,
	OUTER_MODAL_CREATESUCCEED,
	OUTER_MODAL_GENERAL_LOGINFAIL,
	OUTER_MODAL_NAME_ERROR,
	OUTER_MODAL_LOGIN_WAIT_DAUM,
	OUTER_MODAL_LOGIN_WAIT_DAUM_AFTER_KEYRECEIVED,
	OUTER_MODAL_LOGIN_WAIT_DAUM_SERVERFULL,
	OUTER_MODAL_CHARACTERSTAGE_GAME_JOIN_WAIT,
	OUTER_MODAL_CHARACTERSTAGE_GAME_JOIN_FAIL,
	OUTER_MODAL_THAI_CHECK_STRING_ERROR
};

class CModalWindow;
class CSelectServerPage;
class CSelectMenuPage;
class CNewLoginPage;
class CPassKeyboard;
class CSelectUserPage;
class CRegisterPage;
class CChangePasswordPage;
class CChangePinCodePage;
class CForgotAccountPage;
class CFixUserPage;
class CSelectCharacterPage;
class CCreateCharacterNew;
class CWaitDialogue;
class DxLobyStage;
class CCreateCharacterWeb;
class CSecPassSetPage;
class CSecPassCheckPage;
class CMessageWindow;
class CBasicLineBox;
class CBasicTextBox;

class COuterInterface : public CUIMan
{
	static	const	float	fLIST_LINE_INTERVAL;

private:
	BOOL	m_bOnTheInterface;
	bool	m_bBlockProgramFound;
	float	m_fWaitTime;
	BOOL	m_bUseWaitTime;
	USHORT	m_uChaRemain;			// ���� ������ ĳ���� ��
	INT		m_nChaRemainExtremeM;	// ���� ������ �ذ��� ���� ĳ���� ��
	INT		m_nChaRemainExtremeW;	// ���� ������ �ذ��� ���� ĳ���� ��
	int		m_nServerGroup;
	int		m_nServerChannel;
	bool	m_bCHANNEL;
	BOOL	m_bLoginCancel;

	LONG	m_lResolutionBack;

	UIGUID	m_uidMoalCallWindow;
	CString	m_strDaumGID;

private:
	CModalWindow*				m_pModalWindow;
	CSelectServerPage*			m_pSelectServerPage;
	CSelectMenuPage*			m_pSelectMenuPage;
	CNewLoginPage*				m_pNewLoginPage;
	CSecPassSetPage*			m_pSecPassSetPage;
	CSecPassCheckPage*			m_pSecPassCheckPage;
	CMessageWindow*				m_pMessageWindow;
	CPassKeyboard*				m_pPassKeyboard;
	CCreateCharacterWeb*		m_pCreateCharacterWeb;
	CSelectUserPage*			m_pSelectUserPage;
	CRegisterPage*				m_pRegisterPage;
	CChangePasswordPage*		m_pChangePasswordPage;
	CChangePinCodePage*			m_pChangePinCodePage;
	CForgotAccountPage*			m_pForgotAccountPage;
	CFixUserPage*				m_pFixUserPage;
	CSelectCharacterPage*		m_pSelectCharacterPage;
	CCreateCharacterNew*		m_pCreateCharacterNew;
	CUIControl*					m_pFullScreenButton;
	CUIControl*					m_pFullScreenButtonDummy;
	CUIControl*					m_pUpImage;
	CUIControl*					m_pLogoRan;
	CUIControl*					m_pLogoRanBG;
	CUIControl*					m_pDownImage;
	CUIControl*					m_pMessageWindowDummy;
	CBasicLineBox*				m_pBasicLineSchoolDummy;
	CBasicLineBox*				m_pBasicLineSexDummy;
	CUIControl*					m_pSexIconDummy;
	CUIControl*					m_pSchoolIconDummy1;
	CUIControl*					m_pSchoolIconDummy2;
	CUIControl*					m_pSchoolIconDummy3;
	CBasicTextBox*				m_pSchoolNameDummy;
	CBasicTextBox*				m_pSchoolInfoDummy;
	CWaitDialogue*				m_pWaitDisplay;
	DxLobyStage*				m_pDxLobyStage;
	CBasicTextBox*				pTextBoxPage;

	void ResetControl();

public:
	COuterInterface ();
	virtual	~COuterInterface ();

public:
	UIGUID	GetRenderStage ();

	BOOL    SetSex;
	void	SelectSex();
public:
	virtual HRESULT OneTimeSceneInit();
	//virtual	HRESULT FrameMove( LPDIRECT3DDEVICEQ pd3dDevice, float fElapsedTime );
	virtual	HRESULT FrameMove( LPDIRECT3DDEVICEQ pd3dDevice, float fTime, float fElapsedTime );
	virtual	void TranslateUIMessage( UIGUID ControlID, DWORD dwMsg );
	virtual HRESULT FinalCleanup();
	virtual HRESULT DeleteDeviceObjects();

public:
	void SetOnTheInterface ()							{ m_bOnTheInterface = TRUE; }
	BOOL IsOnTheInterface ()							{ return m_bOnTheInterface; }
	void ResetOnTheInterface ()							{ m_bOnTheInterface = FALSE; }

	void	START_WAIT_TIME ( float fTime );
	float	GET_WAIT_TIME()								{ return m_fWaitTime; }

	void SetCancelToLogin()								{ m_bLoginCancel = TRUE; }
	void ResetCancelToLogin()							{ m_bLoginCancel = FALSE; }
	BOOL IsLoginCancel()								{ return m_bLoginCancel; }

	void SetModalCallWindowID( UIGUID uID )				{ m_uidMoalCallWindow = uID; }
	UIGUID GetModalCallWindowID()						{ return m_uidMoalCallWindow; }

private:
	void	UPDATE_WAIT_TIME ( float fElapsedTime );
	void	ModalMsgProcess ( UIGUID nCallerID, DWORD dwMsg );
	void	CloseAllWindow ();

public:
	void MsgProcess ( LPVOID pBuffer );

	void SetDaumGID( const CString & strDaumGID )	{ m_strDaumGID = strDaumGID; }
	const TCHAR * GetDaumGID() const				{ return m_strDaumGID.GetString(); }

	void SetCharRemain( const USHORT uChaRemain )		{ m_uChaRemain = uChaRemain; }
	const USHORT GetCharRemain() const					{ return m_uChaRemain; }

	void SetCharRemainExM( const INT nChaRemain )		{ m_nChaRemainExtremeM = nChaRemain; }
	const INT GetCharRemainExM() const					{ return m_nChaRemainExtremeM; }

	void SetCharRemainExW( const INT nChaRemain )		{ m_nChaRemainExtremeW = nChaRemain; }
	const INT GetCharRemainExW() const					{ return m_nChaRemainExtremeW; }

	BOOL IsCreateExtream()								{ return (m_nChaRemainExtremeM>0 || m_nChaRemainExtremeW>0); }

	const UIRECT	GetBackImageRect();

	// �߱� MMOSPEED
	void RunMMOSpeed();

	const char* MakeString ( const char* szFormat, ... );

	void SetBlockProgramFound ( bool bFOUND )			{ m_bBlockProgramFound = bFOUND; }
	bool IsBlockProgramFound ()							{ return m_bBlockProgramFound; }

	void WAITSERVER_DIALOGUE_OPEN ( const CString& strMessage, const int nAction, const float fTimer );
	void WAITSERVER_DIALOGUE_CLOSE();

	void ToSelectServerPage ( UIGUID cID );
	void ToLoginPage ( UIGUID cID );
	void ToSecPassPage ( UIGUID cID, int nCheckFlag );
	void ToChinaAreaPage ( UIGUID cID );
	void ToSelectMenuPage(UIGUID cID);
	void ToNewLoginPage ( UIGUID cID );
	void ToSelectUserPage(UIGUID cID);
	void ToRegisterPage(UIGUID cID);
	void ToChangePasswordPage(UIGUID cID);
	void ToChangePinCodePage(UIGUID cID);
	void ToForgotAccountPage(UIGUID cID);
	void ToFixUserPage(UIGUID cID);
	void ToSelectCharacterPage ( UIGUID cID );
	void ToCreateCharacterPage ( UIGUID cID );

	void OpenMessageWindow ( UIGUID cID, BOOL bOpen );

	void DeleteCharacter ();
	
	bool IsCHANNEL()									{ return m_bCHANNEL; }

	CModalWindow*			GetModalWindow ()			{ return m_pModalWindow; }
	CSelectServerPage*		GetSelectServerPage ()		{ return m_pSelectServerPage; }
	CSelectMenuPage*		GetSelectMenuPage()			{ return m_pSelectMenuPage; }
	CNewLoginPage*			GetNewLoginPage ()			{ return m_pNewLoginPage; }
	CSecPassSetPage*		GetSecPassSetPage ()		{ return m_pSecPassSetPage;	}
	CSecPassCheckPage*		GetSecPassCheckPage ()		{ return m_pSecPassCheckPage;	}
	CPassKeyboard*			GetPassKeyboard()			{ return m_pPassKeyboard; }
	CSelectUserPage*		GetSelectUserPage()			{ return m_pSelectUserPage; }
	CRegisterPage*			GetRegisterPage()			{ return m_pRegisterPage; }
	CChangePasswordPage*	GetChangePasswordPage()		{ return m_pChangePasswordPage; }
	CChangePinCodePage*		GetChangePinCodePage()		{ return m_pChangePinCodePage; }
	CForgotAccountPage*		GetForgotAccountPage()		{ return m_pForgotAccountPage; }
	CFixUserPage*			GetFixUserPage()			{ return m_pFixUserPage; }
	CSelectCharacterPage*	GetSelectCharacterPage ()	{ return m_pSelectCharacterPage; }
	CCreateCharacterNew*	GetCreateCharacterNew()		{ return m_pCreateCharacterNew; } // NewCreateChar by tobets
	CCreateCharacterWeb*	GetCreateCharacterWeb()		{ return m_pCreateCharacterWeb; }
	CUIControl*				GetUpImage()				{ return m_pUpImage; }
	CUIControl*				GetDownImage()				{ return m_pDownImage; }

	void SetConnectServerInfo ( const int nServerGroup, const int nServerChannel );
	void GetConnectServerInfo ( int& nServerGroup, int& nServerChannel );

	void SetLobyStage( DxLobyStage * pLobyStage )	{ GASSERT( pLobyStage ); m_pDxLobyStage = pLobyStage; }

	void SetResolutionBack( long lResolution )	{ m_lResolutionBack = lResolution; }
	long GetResolutionBack()						{ return m_lResolutionBack; }

private:
	BOOL LoadClientVersion ( int& nPatchVer, int& nGameVer );

public:
	static COuterInterface& GetInstance();
};