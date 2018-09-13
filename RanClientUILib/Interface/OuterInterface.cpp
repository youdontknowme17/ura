#include "StdAfx.h"
#include "OuterInterface.h"
#include "GameTextControl.h"
#include "../EngineLib/DxCommon/DxFontMan.h"
#include "UITextControl.h"
#include "../EngineUIlib/GUInterface/BasicTextBox.h"

#include "RANPARAM.h"
#include "SelectServerPage.h"
#include "SelectChinaAreaPage.h"
#include "NewLoginPage.h"
#include "PassKeyboard.h"
#include "SelectCharacterPage.h"
#include "DxGlobalStage.h"
#include "ModalWindow.h"
#include "WaitDialogue.h"
#include "CreateCharacterWeb.h"
#include "CommonWeb.h"
#include "SecPassSetPage.h"
#include "SecPassCheckPage.h"
#include "MessageWindow.h"
#include "CreateCharacterNew.h" //NewCreateChar by tobets
#include "../EngineLib/Common/SubPath.h"
#include "./BasicLineBox.h"
#include "SelectMenuPage.h"
#include "NewLoginPage.h"
#include "SelectUserPage.h"
#include "RegisterPage.h"
#include "ChangePasswordPage.h"
#include "ChangePinCode.h"
#include "ForgotAccountPage.h"
#include "FixUserPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

COuterInterface& COuterInterface::GetInstance()
{
	static COuterInterface Instance;
	return Instance;
}

const	float	COuterInterface::fLIST_LINE_INTERVAL = 5.0f;

CBasicScrollBarEx* g_pScrollBar = NULL;

COuterInterface::COuterInterface ()	:
	m_bUseWaitTime ( FALSE ),
	m_bBlockProgramFound ( false ),
	m_nServerGroup ( 0 ),
	m_nServerChannel ( 0 ),
	m_bCHANNEL ( false ),
	m_bLoginCancel( FALSE ),
	m_lResolutionBack( 0 ),
	m_uidMoalCallWindow(NO_ID)
{
	ResetControl();
}

COuterInterface::~COuterInterface ()
{
}

void COuterInterface::ResetControl()
{
	m_pModalWindow = NULL;
	m_pSelectServerPage = NULL;
	m_pSelectMenuPage			= NULL;
	m_pNewLoginPage				= NULL;
	m_pSecPassSetPage = NULL;
	m_pSecPassCheckPage = NULL;
	m_pMessageWindow = NULL;
	m_pMessageWindowDummy = NULL;
	m_pPassKeyboard = NULL;
	m_pCreateCharacterWeb = NULL;
	m_pRegisterPage				= NULL;
	m_pChangePasswordPage		= NULL;
	m_pChangePinCodePage		= NULL;
	m_pForgotAccountPage		= NULL;
	m_pFixUserPage				= NULL;
	m_pSelectCharacterPage = NULL;
	m_pCreateCharacterNew = NULL; //NewCreateChar by tobets
	m_pFullScreenButton = NULL;
	m_pFullScreenButtonDummy = NULL;
	m_pUpImage = NULL;
	m_pLogoRan = NULL;
	m_pDownImage = NULL;
	m_pWaitDisplay = NULL;
	m_pDxLobyStage = NULL;
	pTextBoxPage = NULL;
}

HRESULT COuterInterface::OneTimeSceneInit ()
{
	switch ( RANPARAM::emSERVICE_TYPE )
	{
	case EMSERVICE_INDONESIA:
	case EMSERVICE_MALAYSIA_CN:
	case EMSERVICE_MALAYSIA_EN:
	case EMSERVICE_THAILAND:
		CNewLoginPage::nLIMIT_USERID = 20;
		CNewLoginPage::nLIMIT_PASSW = 20;
		m_bCHANNEL = true;
		break;
	case EMSERVICE_PHILIPPINES:
	case EMSERVICE_VIETNAM:
	case EMSERVICE_DEFAULT:
		CNewLoginPage::nLIMIT_USERID = 20;
		CNewLoginPage::nLIMIT_PASSW = 20;
		m_bCHANNEL = true;
		break;
	case EMSERVICE_FEYA:
		CNewLoginPage::nLIMIT_USERID = 20;
		CNewLoginPage::nLIMIT_PASSW = 20;
		m_bCHANNEL = true;
		break;
	case EMSERVICE_KOREA:
		CNewLoginPage::nLIMIT_USERID = 20;
		CNewLoginPage::nLIMIT_PASSW = 20;
		m_bCHANNEL = true;
		break;
	case EMSERVICE_CHINA:
		CNewLoginPage::nLIMIT_USERID = 20;
		CNewLoginPage::nLIMIT_PASSW = 20;
		m_bCHANNEL = true;
		break;

	case EMSERVICE_JAPAN:
		CNewLoginPage::nLIMIT_USERID = 20;
		CNewLoginPage::nLIMIT_PASSW = 20;
		m_bCHANNEL = true;
		break;

	case EMSERVICE_GLOBAL:
		CNewLoginPage::nLIMIT_USERID = 20;
		CNewLoginPage::nLIMIT_PASSW = 20;
		m_bCHANNEL = true;
		break;

	default:
		CNewLoginPage::nLIMIT_USERID = 12;
		CNewLoginPage::nLIMIT_PASSW = 12;
		m_bCHANNEL = false;
		break;
	};

	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );
	CD3DFontPar* pFont10 = DxFontMan::GetInstance().LoadDxFont( _DEFAULT_FONT, 10, _DEFAULT_FONT_FLAG );


#ifdef CH_PARAM
	{
		CSelectChinaAreaPage* pSelectChinaAreaPage = new CSelectChinaAreaPage;
		pSelectChinaAreaPage->CreateEx( SELECT_CHINA_AREA_PAGE, "CUASO_NGOAI" );
		pSelectChinaAreaPage->CreateBaseWidnow( "TRANG_CHON_KHUVUC", (char*)ID2GAMEWORD ( "SELECT_CHINA_AREA" ) );
		pSelectChinaAreaPage->CreateSubControl();
		pSelectChinaAreaPage->SetAlignFlag( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( pSelectChinaAreaPage );	
		ShowGroupFocus ( SELECT_CHINA_AREA_PAGE );
	}
#endif

	{
		m_pSelectServerPage = new CSelectServerPage;
		m_pSelectServerPage->CreateEx ( SELECT_SERVER_PAGE, "CUASO_NGOAI" );
		m_pSelectServerPage->CreateBaseWidnow ( "TRANG_CHON_MAYCHU", (char*) ID2GAMEWORD ( "SELECT_SERVER" ) );
		m_pSelectServerPage->CreateSubControl ();
		m_pSelectServerPage->SetAlignFlag ( UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		RegisterControl ( m_pSelectServerPage );	
		ShowGroupFocus ( SELECT_SERVER_PAGE );
	}

	// SelectMenuPage
	{
		m_pSelectMenuPage = new CSelectMenuPage;
		m_pSelectMenuPage->CreateEx(SELECT_MENU_PAGE, "OUTER_WINDOW");
		m_pSelectMenuPage->CreateBaseWidnow("SELECT_MENU_PAGE", (char*)ID2GAMEWORD("SELECT_MENU_PAGE"));
		m_pSelectMenuPage->CreateSubControl();
		m_pSelectMenuPage->SetAlignFlag(UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y);
		RegisterControl(m_pSelectMenuPage);
		ShowGroupFocus(SELECT_MENU_PAGE);
	}

	// NewLoginPage
	{
		m_pNewLoginPage = new CNewLoginPage;
		m_pNewLoginPage->CreateEx(NEW_LOGIN_PAGE, "OUTER_WINDOW");
		m_pNewLoginPage->CreateBaseWidnow("NEW_LOGIN_PAGE", (char*)ID2GAMEWORD("NEW_LOGIN_PAGE"));
		m_pNewLoginPage->CreateSubControl();
		m_pNewLoginPage->SetAlignFlag(UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y);
		RegisterControl(m_pNewLoginPage);
		ShowGroupFocus(NEW_LOGIN_PAGE);
	}

	{
		m_pPassKeyboard = new CPassKeyboard;
		m_pPassKeyboard->Create( PASS_KEYBOARD, "PASS_KEYBOARD", UI_FLAG_CENTER_X );
		m_pPassKeyboard->CreateSubControl();
		RegisterControl( m_pPassKeyboard );
		ShowGroupFocus( PASS_KEYBOARD );
	}

	// SelectUserPage
	{
		m_pSelectUserPage = new CSelectUserPage;
		m_pSelectUserPage->CreateEx(SELECT_USER_PAGE, "OUTER_WINDOW");
		m_pSelectUserPage->CreateBaseWidnow("SELECT_USER_PAGE", (char*)ID2GAMEWORD("SELECT_USER_PAGE"));
		m_pSelectUserPage->CreateSubControl();
		m_pSelectUserPage->SetAlignFlag(UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y);
		RegisterControl(m_pSelectUserPage);
		ShowGroupFocus(SELECT_USER_PAGE);
	}

	// RegisterPage
	{
		m_pRegisterPage = new CRegisterPage;
		m_pRegisterPage->CreateEx(REGISTER_PAGE, "OUTER_WINDOW");
		m_pRegisterPage->CreateBaseWidnow("REGISTER_PAGE", (char*)ID2GAMEWORD("REGISTER_PAGE"));
		m_pRegisterPage->CreateSubControl();
		m_pRegisterPage->SetAlignFlag(UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y);
		RegisterControl(m_pRegisterPage);
		ShowGroupFocus(REGISTER_PAGE);
	}

	// ChangePasswordPage
	{
		m_pChangePasswordPage = new CChangePasswordPage;
		m_pChangePasswordPage->CreateEx(CHANGEPASSWORD_PAGE, "OUTER_WINDOW");
		m_pChangePasswordPage->CreateBaseWidnow("CHANGEPASSWORD_PAGE", (char*)ID2GAMEWORD("CHANGEPASSWORD_PAGE"));
		m_pChangePasswordPage->CreateSubControl();
		m_pChangePasswordPage->SetAlignFlag(UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y);
		RegisterControl(m_pChangePasswordPage);
		ShowGroupFocus(CHANGEPASSWORD_PAGE);
	}

	// ChangePinCoePage
	{
		m_pChangePinCodePage = new CChangePinCodePage;
		m_pChangePinCodePage->CreateEx(CHANGEPINCODE_PAGE, "OUTER_WINDOW");
		m_pChangePinCodePage->CreateBaseWidnow("CHANGEPINCODE_PAGE", (char*)ID2GAMEWORD("CHANGEPINCODE_PAGE"));
		m_pChangePinCodePage->CreateSubControl();
		m_pChangePinCodePage->SetAlignFlag(UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y);
		RegisterControl(m_pChangePinCodePage);
		ShowGroupFocus(CHANGEPINCODE_PAGE);
	}

	// ForgotAccountPage
	{
		m_pForgotAccountPage = new CForgotAccountPage;
		m_pForgotAccountPage->CreateEx(FORGOTACCOUNT_PAGE, "OUTER_WINDOW");
		m_pForgotAccountPage->CreateBaseWidnow("FORGOTACCOUNT_PAGE", (char*)ID2GAMEWORD("FORGOTACCOUNT_PAGE"));
		m_pForgotAccountPage->CreateSubControl();
		m_pForgotAccountPage->SetAlignFlag(UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y);
		RegisterControl(m_pForgotAccountPage);
		ShowGroupFocus(FORGOTACCOUNT_PAGE);
	}

	// FixUserPage
	{
		m_pFixUserPage = new CFixUserPage;
		m_pFixUserPage->CreateEx(FIXUSER_PAGE, "OUTER_WINDOW");
		m_pFixUserPage->CreateBaseWidnow("FIXUSER_PAGE", (char*)ID2GAMEWORD("FIXUSER_PAGE"));
		m_pFixUserPage->CreateSubControl();
		m_pFixUserPage->SetAlignFlag(UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y);
		RegisterControl(m_pFixUserPage);
		ShowGroupFocus(FIXUSER_PAGE);
	}

	// SelectCharacterPage
	{
		m_pSelectCharacterPage = new CSelectCharacterPage;
		m_pSelectCharacterPage->Create ( SELECT_CHARACTER_PAGE, "SELECT_CHARACTER_PAGE" );
		m_pSelectCharacterPage->CreateSubControl ();
		m_pSelectCharacterPage->SetAlignFlag ( UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		RegisterControl ( m_pSelectCharacterPage );
		ShowGroupFocus ( SELECT_CHARACTER_PAGE );

	}

	{
		m_pCreateCharacterNew = new CCreateCharacterNew; // NewCreateChar by tobets
		m_pCreateCharacterNew->Create ( CREATE_CHARACTER_PAGE, "NEW_CREATE_CHAR_PAGE", UI_FLAG_DEFAULT );
		m_pCreateCharacterNew->CreateSubControl ();
		RegisterControl ( m_pCreateCharacterNew , true );
		ShowGroupFocus ( CREATE_CHARACTER_PAGE );
		HideGroup ( CREATE_CHARACTER_PAGE );
	
	}

	{	
		
		m_pModalWindow = new CModalWindow;
		m_pModalWindow->Create ( MODAL_WINDOW_OUTER, "BASIC_MODAL_WINDOW", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pModalWindow->CreateSubControl ();
		RegisterControl ( m_pModalWindow );
		ShowGroupFocus ( MODAL_WINDOW_OUTER );
		
	}

	{
		m_pUpImage = new CUIControl;
		m_pUpImage->Create ( OUTER_UP_IMAGE, "OUTER_UP_IMAGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );		
		RegisterControl ( m_pUpImage );
		ShowGroupBottom ( OUTER_UP_IMAGE );

#ifdef CH_PARAM
		{
			CBasicTextBox* pTextBox = new CBasicTextBox;
			pTextBox->Create ( CHINA_TITLE, "CHINA_TITLE", UI_FLAG_XSIZE );
			pTextBox->SetFont ( pFont9 );
			pTextBox->SetTextAlign ( TEXT_ALIGN_CENTER_X );
			pTextBox->SetLineInterval ( fLIST_LINE_INTERVAL );
			RegisterControl ( pTextBox );		
			pTextBox->AddText ( ID2GAMEEXTEXT ( "CHINA_OUTER_UP_TITLE" ) );
			ShowGroupBottom ( CHINA_TITLE );
		}
#endif

		m_pDownImage = new CUIControl;
		m_pDownImage ->Create ( OUTER_DN_IMAGE, "OUTER_DN_IMAGE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		RegisterControl ( m_pDownImage  );
		ShowGroupBottom ( OUTER_DN_IMAGE );

		{
			CBasicTextBox* pTextBox = new CBasicTextBox;
			pTextBox->Create ( COPYRIGHT, "COPYRIGHT", UI_FLAG_XSIZE | UI_FLAG_BOTTOM );
			pTextBox->SetFont ( pFont9 );
			pTextBox->SetTextAlign ( TEXT_ALIGN_CENTER_X );
			pTextBox->SetLineInterval ( fLIST_LINE_INTERVAL );
			RegisterControl ( pTextBox );		
			pTextBox->AddText ( ID2GAMEWORD ( "COPYRIGHT_TITLE" ) );
			pTextBox->AddText ( ID2GAMEWORD ( "COPYRIGHT_COMPANY" ) );
			ShowGroupBottom ( COPYRIGHT );
		}
	}

	{
		m_pFullScreenButton = new CUIControl;
		m_pFullScreenButton->Create ( FULLSCREEN_OUTER, "FULLSCREEN_OUTER", UI_FLAG_RIGHT );
		RegisterControl ( m_pFullScreenButton );
		ShowGroupTop ( FULLSCREEN_OUTER );

		m_pFullScreenButtonDummy = new CUIControl;
		m_pFullScreenButtonDummy->Create ( FULLSCREEN_OUTER_DUMMY, "FULLSCREEN_OUTER", UI_FLAG_RIGHT );
		m_pFullScreenButtonDummy->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pFullScreenButtonDummy );
	}

	{
	/*	m_pLogoRan = new CUIControl;
		m_pLogoRan->Create ( OUTER_UP_LOGO_IMAGE, "OUTER_UP_LOGO_IMAGE", UI_FLAG_CENTER_X );		
		RegisterControl ( m_pLogoRan );
		ShowGroupBottom ( OUTER_UP_LOGO_IMAGE ); */
	}

	{		
		m_pWaitDisplay = new CWaitDialogue;
		m_pWaitDisplay->Create ( WAIT_DISPLAY, "WAITSERVER_DISPLAY", UI_FLAG_CENTER_X | UI_FLAG_CENTER_Y );
		m_pWaitDisplay->CreateSubControl ();
		RegisterControl ( m_pWaitDisplay );
		ShowGroupFocus ( WAIT_DISPLAY );
	}

	//¾×é¹Ë¹Ñ§¢éÍÁÙÅ âÃ§àÃÕÂ¹
	{
		m_pBasicLineSchoolDummy = new CBasicLineBox;
		m_pBasicLineSchoolDummy->CreateSub ( NO_ID, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pBasicLineSchoolDummy->CreateBaseBoxOuter("NEW_CHAR_BACK_SCHOOL_IMG");
		m_pBasicLineSchoolDummy->SetVisibleSingle(FALSE);
		RegisterControl ( m_pBasicLineSchoolDummy );
	}

	//¾×é¹Ë¹Ñ§¢éÍÁÙÅ à¾?
	{
		m_pBasicLineSexDummy = new CBasicLineBox;
		m_pBasicLineSexDummy->CreateSub ( NO_ID, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pBasicLineSexDummy->CreateBaseBoxOuter("NEW_CHAR_BACK_SEX_IMG");
		m_pBasicLineSexDummy->SetVisibleSingle(FALSE);
		RegisterControl ( m_pBasicLineSexDummy );
	}

	{
		m_pSexIconDummy = new CUIControl;
		m_pSexIconDummy->CreateSub ( NO_ID, "NEW_CHAR_ICON_SEX_IMAGE_MEN", UI_FLAG_DEFAULT );	
		m_pSexIconDummy->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pSexIconDummy );
	}

	//äÍ¤Í¹âÃ§àÃÕÂ?SG
	{
		m_pSchoolIconDummy1 = new CUIControl;
		m_pSchoolIconDummy1->CreateSub ( NO_ID, "NEW_CHAR_ICON_SCHOOL_IMAGE_SG", UI_FLAG_DEFAULT );	
		m_pSchoolIconDummy1->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pSchoolIconDummy1 );
	}
	//äÍ¤Í¹âÃ§àÃÕÂ?MP
	{
		m_pSchoolIconDummy2 = new CUIControl;
		m_pSchoolIconDummy2->CreateSub ( NO_ID, "NEW_CHAR_ICON_SCHOOL_IMAGE_MP", UI_FLAG_DEFAULT );	
		m_pSchoolIconDummy2->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pSchoolIconDummy2 );
	}
	//äÍ¤Í¹âÃ§àÃÕÂ?PH
	{
		m_pSchoolIconDummy3 = new CUIControl;
		m_pSchoolIconDummy3->CreateSub ( NO_ID, "NEW_CHAR_ICON_SCHOOL_IMAGE_PH", UI_FLAG_DEFAULT );	
		m_pSchoolIconDummy3->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pSchoolIconDummy3 );
	}

	//ª×èÍâÃ§àÃÕÂ¹
	{
		m_pSchoolNameDummy = new CBasicTextBox;
		m_pSchoolNameDummy->CreateSub ( NO_ID, "2012_NEW_CHAR_SELECT_SCHOOL_NAME", UI_FLAG_DEFAULT );
		m_pSchoolNameDummy->SetFont ( pFont10 );
		m_pSchoolNameDummy->SetTextAlign ( TEXT_ALIGN_LEFT );
		m_pSchoolNameDummy->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pSchoolNameDummy );
	}
	//¢éÍÁÙÅâÃ§àÃÕÂ¹
	{
		m_pSchoolInfoDummy = new CBasicTextBox;
		m_pSchoolInfoDummy->CreateSub ( NO_ID, "2012_NEW_CHAR_SELECT_SCHOOL_DEC", UI_FLAG_DEFAULT );
		m_pSchoolInfoDummy->SetFont ( pFont9 );
		m_pSchoolInfoDummy->SetTextAlign ( TEXT_ALIGN_LEFT );	
		m_pSchoolInfoDummy->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pSchoolInfoDummy );
	}

	//ª×èÍáÊá´§Ë¹éÒµèÒ?ÊÃéÒ§µÑÇÅÐ¤Ã
	{
		pTextBoxPage = new CBasicTextBox;
		pTextBoxPage->Create ( NAME_PAGE, "2012_NEW_CHAR_TEXT_PAGE", UI_FLAG_CENTER_X );
		pTextBoxPage->SetFont ( pFont10 );
		pTextBoxPage->SetTextAlign ( TEXT_ALIGN_CENTER_X );
		pTextBoxPage->SetText ( ID2GAMEEXTEXT("2012_NEW_CHAR_TEXT_PAGE" , 0 ), NS_UITEXTCOLOR::SILVER );
		RegisterControl ( pTextBoxPage );
		ShowGroupBottom ( NAME_PAGE );
	}

	CloseAllWindow ();

#ifdef CH_PARAM
	ShowGroupFocus( SELECT_CHINA_AREA_PAGE );
#else
	ShowGroupFocus( SELECT_SERVER_PAGE );	
#endif

	return S_OK;
}

//HRESULT COuterInterface::FrameMove ( LPDIRECT3DDEVICEQ pd3dDevice, float fElapsedTime )
HRESULT COuterInterface::FrameMove ( LPDIRECT3DDEVICEQ pd3dDevice, float fTime, float fElapsedTime )
{
	ResetOnTheInterface ();
	bool bFULLSCREEN = RANPARAM::bScrWindowed && RANPARAM::bScrWndHalfSize;
	if ( bFULLSCREEN )	ShowGroupTop ( FULLSCREEN_OUTER );
	else				HideGroup ( FULLSCREEN_OUTER );

	if ( IsVisibleGroup ( FULLSCREEN_OUTER ) )
	{
		const UIRECT& rcGlobalPos = m_pFullScreenButtonDummy->GetGlobalPos ();
		m_pFullScreenButton->SetGlobalPos ( D3DXVECTOR2(rcGlobalPos.left,rcGlobalPos.top) );
	}

	HRESULT hr;
	//hr = CUIMan::FrameMove ( pd3dDevice, fElapsedTime );
	hr = CUIMan::FrameMove ( pd3dDevice, fTime, fElapsedTime );
	if ( FAILED ( hr ) ) return hr;

	//	TIME UPDATE
	UPDATE_WAIT_TIME ( fElapsedTime );

	CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient ();
	if ( !pNetClient->IsOnline() )		//³×Æ®¿öÅ© ¿¬°áÀÌ ²÷¾îÁø °æ¿ì
	{
		if ( !COuterInterface::GetInstance().IsVisibleGroup ( MODAL_WINDOW_OUTER ) )
		{
			if( !IsLoginCancel() )
			{
				DoModalOuter ( ID2GAMEEXTEXT ("CHARACTERSTAGE_2"), MODAL_INFOMATION, OK, OUTER_MODAL_CONNECTCLOSED );
			}
			else
			{
				if ( DxGlobalStage::GetInstance().GetNetClient()->ConnectLoginServer(RANPARAM::LoginAddress) > NET_ERROR )
				{
					DxGlobalStage::GetInstance().GetNetClient()->SndReqServerInfo();				
				}
				else
				{
					DoModalOuter( ID2GAMEEXTEXT ( "SERVERSTAGE_3" ), MODAL_INFOMATION, OKCANCEL, OUTER_MODAL_CLOSEGAME );
				}
			}
		}
	}

	if ( IsBlockProgramFound() )
	{
		DoModalOuter ( ID2GAMEINTEXT("BLOCK_PROGRAM_FOUND"), MODAL_INFOMATION, OK, OUTER_MODAL_CLOSEGAME );
	}

	if ( DxInputDevice::GetInstance().GetKeyState ( DIK_LMENU ) & DXKEY_PRESSED )
	{
		if ( DxInputDevice::GetInstance().GetKeyState ( DIK_F4 ) & DXKEY_DOWN )
		{
			DoModalOuter ( ID2GAMEINTEXT("CLOSE_GAME"), MODAL_QUESTION, OKCANCEL, OUTER_MODAL_CLOSEGAME );
		}
	}

	if ( IsVisibleGroup ( SELECT_CHARACTER_PAGE ) )
	{
		CUIFocusContainer::UICONTROL_FOCUSLIST focus_list = GetFocusList()->GetFocusList();
		CUIFocusContainer::UICONTROL_FOCUSLIST_RITER riter = focus_list.rbegin ();
		CUIFocusContainer::UICONTROL_FOCUSLIST_RITER riter_end = focus_list.rend ();
	}

	if ( IsVisibleGroup ( CREATE_CHARACTER_PAGE ) )
	{
		if ( SetSex )
			SelectSex();
	}

	{
		// Note : À§¾Æ·¡ ¹ÙÀÇ Å©±â¸¦ Á¶ÀýÇÑ´Ù. 
		LONG lResolution = CUIMan::GetResolution ();

		//if( m_lResolutionBack != lResolution )
		{
			WORD X_RES = HIWORD( lResolution );
			WORD Y_RES = LOWORD( lResolution );

			INT nHeight = static_cast<INT>(Y_RES * 0.1f);
			INT nTop = Y_RES - nHeight;

			if ( m_pUpImage && m_pDownImage )
			{
				m_pUpImage->SetGlobalPos( UIRECT( 0, 0, (float)X_RES, (float)nHeight ) );
				m_pDownImage->SetGlobalPos( UIRECT( 0, (float)nTop, (float)X_RES, (float)nHeight ) );
			}

			// Note : Ä³¸¯ÅÍ ¼±ÅÃ, »ý¼º À©µµ¿ìÀÇ À§Ä¡¸¦ º¯°æÇÑ´Ù.
			UIRECT rcLocalPos;

			if( m_pSelectCharacterPage && m_pSelectCharacterPage->IsVisible() )
			{
				rcLocalPos = m_pSelectCharacterPage->GetLocalPos();
				rcLocalPos.left = X_RES - rcLocalPos.sizeX - 6.0f;
				rcLocalPos.top = nTop - rcLocalPos.sizeY - 6.0f;
				m_pSelectCharacterPage->SetGlobalPos( rcLocalPos );

			}

			if( m_pCreateCharacterNew && m_pCreateCharacterNew->IsVisible() )
			{
				UIRECT rcGlobalPosNew, rcGlobalPosOld;
				rcLocalPos = m_pCreateCharacterNew->GetLocalPos();


				LONG lResolutionBack = COuterInterface::GetInstance().GetResolutionBack ();
				WORD X_RES_BACK = HIWORD( lResolutionBack );
				WORD Y_RES_BACK = LOWORD( lResolutionBack );

				if ( X_RES_BACK != X_RES || Y_RES_BACK != Y_RES )
				{		

					rcGlobalPosOld.left = 0;								
					rcGlobalPosOld.top = 0;
					rcGlobalPosOld.right = X_RES_BACK;
					rcGlobalPosOld.bottom = Y_RES_BACK;
					rcGlobalPosOld.sizeX = X_RES_BACK;
					rcGlobalPosOld.sizeY = Y_RES_BACK;

					rcGlobalPosNew.left = 0;			
					rcGlobalPosNew.right = X_RES;
					rcGlobalPosNew.sizeX = X_RES;
					rcGlobalPosNew.top = 0;
					rcGlobalPosNew.bottom = Y_RES;
					rcGlobalPosNew.sizeY = Y_RES;

					m_pCreateCharacterNew->ReSizeControl( rcGlobalPosOld, rcGlobalPosNew );

					COuterInterface::GetInstance().SetResolutionBack ( MAKELONG(Y_RES, X_RES) );
				}		
			}

			if( m_pCreateCharacterWeb && m_pCreateCharacterWeb->IsVisible() )
			{
				//rcLocalPos = m_pCreateCharacterWeb->GetLocalPos();
				//rcLocalPos.left = X_RES - rcLocalPos.sizeX - 6.0f;
				//rcLocalPos.top = nTop - rcLocalPos.sizeY - 167.0f;
				//m_pCreateCharacterWeb->SetGlobalPos( rcLocalPos );
				//m_pCreateCharacterWeb->AlignWeb();
			}

			// Note : ÆÐ½º¿öµå Å°º¸µåÀÇ À§Ä¡¸¦ Á¶Á¤ÇÑ´Ù.
			if( m_pPassKeyboard->IsVisible() )
			{
				rcLocalPos = m_pPassKeyboard->GetGlobalPos();
				if ( m_pNewLoginPage->IsVisible() )
					rcLocalPos.top = m_pNewLoginPage->GetGlobalPos().bottom + 13.0f;
				m_pPassKeyboard->SetGlobalPos( rcLocalPos );
			}
		}
	}

	return S_OK;
}

HRESULT COuterInterface::DeleteDeviceObjects()
{
	ResetControl();
	return CUIMan::DeleteDeviceObjects ();
}

void COuterInterface::SelectSex()
{
	CBasicLineBox*	m_pBasicLineSchool(NULL);
	CBasicLineBox*	m_pBasicLineSex(NULL);
	CUIControl*		m_pSexIcon(NULL);
	CUIControl*		m_pSchoolIcon1(NULL);
	CUIControl*		m_pSchoolIcon2(NULL);
	CUIControl*		m_pSchoolIcon3(NULL);
	CBasicTextBox*	m_pSchoolName(NULL);
	CBasicTextBox*	m_pSchoolInfo(NULL);

	m_pBasicLineSchool = m_pCreateCharacterNew->GetBasicLineSchool();
	m_pSchoolIcon1 = m_pCreateCharacterNew->GetIconSchool( 0 );
	m_pSchoolIcon2 = m_pCreateCharacterNew->GetIconSchool( 1 );
	m_pSchoolIcon3 = m_pCreateCharacterNew->GetIconSchool( 2 );
	m_pSchoolName = m_pCreateCharacterNew->GetSchoolName();
	m_pSchoolInfo = m_pCreateCharacterNew->GetSchoolInfo();
	m_pBasicLineSex = m_pCreateCharacterNew->GetBasicLineSex();
	m_pSexIcon = m_pCreateCharacterNew->GetIconSex( 0 );

	if ( !m_pBasicLineSchool ) return;
	if ( !m_pSchoolIcon1 ) return;
	if ( !m_pSchoolIcon2 ) return;
	if ( !m_pSchoolIcon3 ) return;
	if ( !m_pSchoolName ) return;
	if ( !m_pSchoolInfo ) return;
	if ( !m_pBasicLineSex ) return;
	if ( !m_pSexIcon ) return;

	const UIRECT& rcSchoolBasicLineDummy = m_pBasicLineSchoolDummy->GetGlobalPos (); //µÓáË¹è§à´ÔÁ¢Í§ ¾×é¹ËÅÑ§ ??
	const UIRECT& rcSchoolNameDummy = m_pSchoolNameDummy->GetGlobalPos ();
	const UIRECT& rcSchoolInfoDummy = m_pSchoolInfoDummy->GetGlobalPos ();
	const UIRECT& rcSchoolIcon1Dummy = m_pSchoolIconDummy1->GetGlobalPos ();
	const UIRECT& rcSchoolIcon2Dummy = m_pSchoolIconDummy2->GetGlobalPos ();
	const UIRECT& rcSchoolIcon3Dummy = m_pSchoolIconDummy3->GetGlobalPos ();

	const UIRECT& rcSexBasicLineDummy = m_pBasicLineSex->GetGlobalPos ();
	const UIRECT& rcSexIconDummy = m_pSexIcon->GetGlobalPos ();

	const D3DXVECTOR2 NewSchoolBasicLine ( rcSexBasicLineDummy.left - rcSexBasicLineDummy.sizeX, rcSexBasicLineDummy.top );
	const D3DXVECTOR2 NewSchoolName ( rcSexBasicLineDummy.left - rcSexBasicLineDummy.sizeX + 5.0f, rcSexIconDummy.top );
	const D3DXVECTOR2 NewSchoolInfo ( rcSexBasicLineDummy.left - rcSexBasicLineDummy.sizeX + 5.0f, rcSexIconDummy.bottom );
	const D3DXVECTOR2 NewSchoolIcon1 ( rcSexBasicLineDummy.left - rcSexIconDummy.sizeX - 5.0f, rcSexIconDummy.top );
	const D3DXVECTOR2 NewSchoolIcon2 ( rcSexBasicLineDummy.left - rcSexIconDummy.sizeX - 5.0f, rcSexIconDummy.top );
	const D3DXVECTOR2 NewSchoolIcon3 ( rcSexBasicLineDummy.left - rcSexIconDummy.sizeX - 5.0f, rcSexIconDummy.top );

	m_pBasicLineSchool->SetGlobalPos( NewSchoolBasicLine );
	m_pSchoolIcon1->SetGlobalPos( NewSchoolIcon1 );
	m_pSchoolIcon2->SetGlobalPos( NewSchoolIcon2 );
	m_pSchoolIcon3->SetGlobalPos( NewSchoolIcon3 );
	m_pSchoolName->SetGlobalPos( NewSchoolName );
	m_pSchoolInfo->SetGlobalPos( NewSchoolInfo );

}
// Áß±¹ MMOSPEED
// Áß±¹ ³×Æ®¿öÅ© Ã¼Å© ÇÁ·Î±×·¥ ½ÇÇà
void COuterInterface::RunMMOSpeed()
{
	CString str;
	str.Format ( "%s\\%s", SUBPATH::APP_ROOT, "mmospeed.exe");
	
	CString strCmd;
	std::string strUserID = RANPARAM::GETUSERID_DEC();

	// UserID, °ÔÀÓ ½Äº°¹øÈ£, Áö¿ª¹øÈ£
	strCmd.Format( "%s %d %d", strUserID.c_str(), 1, RANPARAM::nChinaRegion+1 );

	int result = (int)ShellExecute( NULL , "open", str.GetString() ,strCmd.GetString(), NULL, SW_SHOW );

	if( result < 32  )
	{
		// ¿¡·¯Ã³¸®
		return;
	}
	
	return;
}