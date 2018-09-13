#include "StdAfx.h"
#include "RebirthDialogue.h"
#include "BasicLineBox.h"
#include "BasicLineBoxEx.h"
#include "../EngineUIlib/GUInterface/BasicTextBox.h"
#include "BasicTextButton.h"

#include "GameTextControl.h"
#include "GLGaeaClient.h"
#include "DxGlobalStage.h"
#include "InnerInterface.h"
#include "../Interface/UIWindowEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const float CRebirthDialogue::fDEFAULT_TIME_LEFT = 5.0f;
const float CRebirthDialogue::fDEFAULT_SELECT_TIME_LEFT = 10.0f;

CRebirthDialogue::CRebirthDialogue ()
	: m_pTextBox(NULL)
	, m_pTextBox2(NULL)
    , m_pCheckPointButton(NULL)
	, m_pCurrentPositionButton(NULL)
	, m_pRecoveryExpButton(NULL)
	, m_pDummy_1BUTTON(NULL)
	, m_pDummy_2BUTTON_L(NULL)
	, m_pDummy_2BUTTON_R(NULL)
	, m_bDO_ACTION(false)
	, m_CallerID(NO_ID)
	, m_fTIME_LEFT(fDEFAULT_TIME_LEFT)
	, m_fSelTIME_LEFT( fDEFAULT_SELECT_TIME_LEFT )
	, m_nACTION(NO_ID)
	, m_bDONE_ACTION(false)
	, m_bAutoRebirth( false )
{
}

CRebirthDialogue::~CRebirthDialogue ()
{
}

CBasicTextBox*	CRebirthDialogue::CreateTextBox ( char* szKeyword, CD3DFontPar* pFont )
{
	CBasicTextBox* pTextBox = new CBasicTextBox;
	pTextBox->CreateSub ( this, szKeyword );
	pTextBox->SetFont ( pFont );
	RegisterControl ( pTextBox );

	return pTextBox;
}

void CRebirthDialogue::CreateSubControl ()
{
	CD3DFontPar* pFont10 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 10, _DEFAULT_FONT_FLAG );
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 10, _DEFAULT_FONT_FLAG );
	CBasicTextBox* pTextBox = NULL;	
	//CD3DFontPar* pFont6 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 6, _DEFAULT_FONT_SHADOW_FLAG );
	
/*	{
		m_bg_white = new CUIControl;
		m_bg_white->CreateSub ( this, "RNREBIRTH_WINDOW_REGION", UI_FLAG_DEFAULT);	
		m_bg_white->SetVisibleSingle ( TRUE );
		RegisterControl ( m_bg_white );
	}

	{
		m_pTargetBox = new CBasicLineBoxEx;
		m_pTargetBox->CreateSub ( this, "BASIC_LINE_BOX_EX_BODY_DIALOGUE_WHITE_BACK", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pTargetBox->CreateBaseBoxDialogueWhiteBack ( "RNREBIRTH_WHITE_LINEBOX" );
		m_pTargetBox->SetVisibleSingle ( TRUE );
		RegisterControl ( m_pTargetBox );

		m_pTargetBox = new CBasicLineBoxEx;
		m_pTargetBox->CreateSub ( this, "BASIC_LINE_BOX_EX_BODY_DIALOGUE_WHITE_BACK", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pTargetBox->CreateBaseBoxDialogueWhiteBack ( "RNREBIRTH_WHITE_LINEBOX_BOTTOM" );
		m_pTargetBox->SetVisibleSingle ( TRUE );
		RegisterControl ( m_pTargetBox );
	}

	{

		m_pTargetBox = new CBasicLineBoxEx;
		m_pTargetBox->CreateSub ( this, "BASIC_LINE_BOX_EX_DIALOGUE_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pTargetBox->CreateBaseBoxDialogueWhite ( "REBIRTH_DIALOGUE_LINE_BOX" );
		m_pTargetBox->SetVisibleSingle ( TRUE );
		RegisterControl ( m_pTargetBox );
	}
	{
		m_pTargetBox = new CBasicLineBoxEx;
		m_pTargetBox->CreateSub ( this, "BASIC_LINE_BOX_EX_DIALOGUE_WHITE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pTargetBox->CreateBaseBoxDialogueWhite ( "REBIRTH_DIALOGUE_LINE_BOX_BOTTOM" );
		m_pTargetBox->SetVisibleSingle ( TRUE );
		RegisterControl ( m_pTargetBox );

	}*/
	
	{	// Ðen Tra´ng Ðen Ðen Background
		CBasicLineBoxEx* pBasicLineBoxEx = new CBasicLineBoxEx;
		pBasicLineBoxEx->CreateSub ( this, "BLACKCATYB_DTDX_GIUA", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBoxEx->BlackCatYBDTDXBox ( "CUASO_HOISINH_BG" );
		RegisterControl ( pBasicLineBoxEx );
	}
	{	// Xa´m Ðen Background
		CBasicLineBoxEx* pBasicLineBoxEx = new CBasicLineBoxEx;
		pBasicLineBoxEx->CreateSub ( this, "BLACKCATYB_XAM_DEN_GIUA", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBoxEx->BlackCatYBXamDenBox ( "CUASO_HOISINH_NUTBAM" );
		RegisterControl ( pBasicLineBoxEx );
	}
	{	// Ðen Tra´ng Ðen Xa´m Background
		CBasicLineBoxEx* pBasicLineBoxEx = new CBasicLineBoxEx;
		pBasicLineBoxEx->CreateSub ( this, "BLACKCATYB_DTDX_GIUA", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBoxEx->BlackCatYBDTDXBox ( "CUASO_HOISINH_VATPHAM" );
		RegisterControl ( pBasicLineBoxEx );
	}
	

	{
		pButtonCheck = CreateControl ( "CUASO_HOISING_NUTBAM", CHECK_BUTTON );
		pButtonCheckOver = CreateControl ( "CUASO_HOISING_NUTBAM_KETTHUC");
		pButtonCheckOver->SetVisibleSingle ( FALSE );
	}
//		m_pRebirthLogo = CreateControl ( "RNREBIRTH_LOGO" );
	{
		CBasicTextBox* pTextBox = new CBasicTextBox;
		pTextBox->CreateSub ( this, "CUASO_HOISINH_TEXTBOX", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pTextBox->SetFont ( pFont );
		pTextBox->SetTextAlign ( TEXT_ALIGN_CENTER_X );
		pTextBox->SetPartInterval ( 5.0f );
		RegisterControl ( pTextBox );
		m_pTextBox = pTextBox;
	}
	
	{
		CBasicTextBox* pTextBox2 = new CBasicTextBox;
		pTextBox2->CreateSub ( this, "CUASO_HOISINH_TEXTBOX_BOTTOM", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pTextBox2->SetFont ( pFont );
		pTextBox2->SetTextAlign ( TEXT_ALIGN_LEFT );
		pTextBox2->SetPartInterval ( 5.0f );
		RegisterControl ( pTextBox2 );
		m_pTextBox2 = pTextBox2;
		m_pTextBox2->SetText ( ID2GAMEINTEXT("REBIRTH_WINDOW_RECOMMEND_ITEM_EXPLAIN"), NS_UITEXTCOLOR::WHITE );
	}

	{
		pTextBox = CreateTextBox ( "CUASO_HOISINH_TEXTBOX_BUTTON", pFont );
		pTextBox->AddText ( "Revive automatically?", NS_UITEXTCOLOR::WHITE );
	}
	
	m_pCheckPointButton = CreateTextButton ( "CUASO_HOISINH_CHECKPOINT", REBIRTH_CHECK_POINT, (char*)ID2GAMEWORD ( "REBIRTH_BUTTON", 0 ) );
	m_pCurrentPositionButton = CreateTextButton ( "CUASO_HOISINH_VITRI_HIENTAI", REBIRTH_CURRENT_POSITION, (char*)ID2GAMEWORD ( "REBIRTH_BUTTON", 1 ) );
	m_pRecoveryExpButton = CreateTextButton ( "CUASO_HOISINH_PHUCHOI_EXP", REBIRTH_RECOVERY_EXP, (char*)ID2GAMEWORD ( "REBIRTH_BUTTON", 2 ) );
	m_pADD = CreateTextButton ( "CUASO_HOISINH_MUA", REBIRTH_DIALOGUE_BUY, (char*)ID2GAMEWORD("REBIRTH_BUTTON",4) );

	m_pDummy_1BUTTON = CreateDummyControl ( "CUASO_HOISINH_1_BUTTON" );
	m_pDummy_2BUTTON_L = CreateDummyControl ( "CUASO_HOISINH_2_BUTTON_L" );
	m_pDummy_2BUTTON_R = CreateDummyControl ( "CUASO_HOISINH_2_BUTTON_R" );

	m_pRebirthLogo = CreateControl ( "CUASO_HOISINH_LOGO_VATPHAM" );
}

CUIControl*	CRebirthDialogue::CreateDummyControl ( char* szKeyword )
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, szKeyword );
	pControl->SetVisibleSingle ( FALSE );
	RegisterControl ( pControl );
	return pControl;
}

CBasicTextButton* CRebirthDialogue::CreateTextButton ( char* szButton, UIGUID ControlID , char* szText )
{
	const int nBUTTONSIZE = CBasicTextButton::SIZE24;
	CBasicTextButton* pButton = new CBasicTextButton;
	pButton->CreateSub ( this, "BASIC_TEXT_BUTTON24", UI_FLAG_XSIZE, ControlID );
	pButton->CreateBaseButton ( szButton, nBUTTONSIZE, CBasicButton::CLICK_FLIP, szText, _DEFAULT_FONT_SHADOW_FLAG );
	RegisterControl ( pButton );
	return pButton;
}

void CRebirthDialogue::SET_ITEMREBIRTH ( const bool& bItemRebirth, bool bAutoRebirth, bool bCantUseRebirth )
{
	m_bDO_ACTION = false;
	m_bDONE_ACTION = false;
	m_fTIME_LEFT = fDEFAULT_TIME_LEFT;
	m_fSelTIME_LEFT = fDEFAULT_SELECT_TIME_LEFT;

	m_bAutoRebirth = false;

	if( bCantUseRebirth )
	{
		m_pTextBox->SetText ( ID2GAMEINTEXT("REBIRTH_DIALOGUE_TEXT2"), NS_UITEXTCOLOR::DARKORANGE );
	}
	else
	{
		m_pTextBox->SetText ( ID2GAMEINTEXT("REBIRTH_DIALOGUE_TEXT"), NS_UITEXTCOLOR::DARKORANGE );
	}
	if ( m_bAutoRebirth )
	{
		CString strLeftTime;
		strLeftTime.Format ( "%1.0f%s", floor(m_fSelTIME_LEFT) + 1.0f, ID2GAMEWORD("WAITSERVER_TIMELEFT_UNIT"));		
		m_pTextBox->AddText ( strLeftTime );
	}

	bool bGuidBattleMap(false);
	PLANDMANCLIENT pLand = GLGaeaClient::GetInstance().GetActiveMap();
	if( pLand )
	{
		bGuidBattleMap = (pLand->m_bClubBattle || pLand->m_bClubDeathMatch ); 
	}

	ShowButton ();

	if ( bItemRebirth && !bGuidBattleMap )	// ±ÍÈ¥ÁÖ, ¼±µµºÒ°¡Áö¿ªx, ±ÍÈ¥ºÒ°¡Áö¿ªx
	{
		{
			const UIRECT& rc2ButtonL_G = m_pDummy_2BUTTON_L->GetGlobalPos();
			const UIRECT& rc2ButtonL_L = m_pDummy_2BUTTON_L->GetLocalPos();
			
			m_pCheckPointButton->SetGlobalPos( rc2ButtonL_G );
			m_pCheckPointButton->SetLocalPos( rc2ButtonL_L );
			m_pCheckPointButton->SetFlip( TRUE );
		}
		{
			const UIRECT& rc2ButtonR_G = m_pDummy_2BUTTON_R->GetGlobalPos();
			const UIRECT& rc2ButtonR_L = m_pDummy_2BUTTON_R->GetLocalPos();

			m_pCurrentPositionButton->SetGlobalPos( rc2ButtonR_G );
			m_pCurrentPositionButton->SetLocalPos( rc2ButtonR_L );
			m_pCurrentPositionButton->SetFlip( TRUE );
		}

		m_pRecoveryExpButton->SetVisibleSingle ( FALSE );

		m_bAutoRebirth = bAutoRebirth;

	}
// °æÇèÄ¡È¸º¹_Á¤ÀÇ_Normal
#if defined( _RELEASED ) || defined ( KRT_PARAM ) || defined ( KR_PARAM )
	else if ( !bCantUseRebirth && !bGuidBattleMap )	// ±ÍÈ¥ÁÖx, ±ÍÈ¥ºÒ°¡Áö¿ªx, ¼±µµÀüÁö¿ªx
	{
		{
			const UIRECT& rc2ButtonL_G = m_pDummy_2BUTTON_L->GetGlobalPos();
			const UIRECT& rc2ButtonL_L = m_pDummy_2BUTTON_L->GetLocalPos();
			
			m_pCheckPointButton->SetGlobalPos( rc2ButtonL_G );
			m_pCheckPointButton->SetLocalPos( rc2ButtonL_L );
			m_pCheckPointButton->SetFlip( TRUE );
		}
		
		{
			const UIRECT& rc2ButtonR_G = m_pDummy_2BUTTON_R->GetGlobalPos();
			const UIRECT& rc2ButtonR_L = m_pDummy_2BUTTON_R->GetLocalPos();

			m_pRecoveryExpButton->SetGlobalPos( rc2ButtonR_G );
			m_pRecoveryExpButton->SetLocalPos( rc2ButtonR_L );
			m_pRecoveryExpButton->SetFlip( TRUE );
		}

		{
			m_pCurrentPositionButton->SetVisibleSingle ( FALSE );
		}
	}
#endif
	else
	{
		{
			const UIRECT& rc1BUTTON = m_pDummy_1BUTTON->GetGlobalPos ();
			D3DXVECTOR2 vPos ( rc1BUTTON.left, rc1BUTTON.top );
			m_pCheckPointButton->SetGlobalPos ( vPos );
			m_pCheckPointButton->SetFlip ( TRUE );
		}

		{
			m_pCurrentPositionButton->SetVisibleSingle ( FALSE );
			m_pRecoveryExpButton->SetVisibleSingle ( FALSE );
		}
	}
}

void CRebirthDialogue::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{

	CUIWindowEx::TranslateUIMessage ( ControlID, dwMsg );
	switch ( ControlID )
	{
	case REBIRTH_CHECK_POINT:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
                m_nACTION = REBIRTH_CHECK_POINT;
				DoACTION ();
				HideButton ();
			}
		}
        break;

	case REBIRTH_CURRENT_POSITION:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				m_nACTION = REBIRTH_CURRENT_POSITION;
				DoACTION ();
                HideButton ();
			}
		}
		break;
	case CHECK_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( CHECK_LB_UP_LIKE( dwMsg ) )
				{
					if ( pButtonCheck->IsVisible () && !pButtonCheckOver->IsVisible () )
					{
						pButtonCheckOver->SetVisibleSingle ( TRUE );
					}
					else if ( pButtonCheck->IsVisible () && pButtonCheckOver->IsVisible () )
					{
						pButtonCheckOver->SetVisibleSingle ( FALSE );
					}
				}
			}
		}
		break;
	case REBIRTH_RECOVERY_EXP:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				
				m_nACTION = REBIRTH_RECOVERY_EXP;
				DoACTION ();
                HideButton ();				
			}
		}
		break;
	}
}

void CRebirthDialogue::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	CUIWindow::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

    if( m_bDO_ACTION )
	{
		m_fTIME_LEFT -= fElapsedTime;

		bool bTimeEvent = false;
		if ( m_fTIME_LEFT < -1.0f ) bTimeEvent = true;

		if ( bTimeEvent )
		{
			if ( m_bDONE_ACTION ) return ;

			switch ( m_nACTION )
			{
			case REBIRTH_CHECK_POINT:
				{
					AddMessageEx ( UIMSG_MODAL_OK );
					CInnerInterface::GetInstance().HideGroup ( GetWndID (), true );

					m_bDONE_ACTION = true;
				}
				break;
			case REBIRTH_CURRENT_POSITION:
				{
					AddMessageEx ( UIMSG_MODAL_CANCEL );
					CInnerInterface::GetInstance().HideGroup ( GetWndID (), true );

					m_bDONE_ACTION = true;
				}
				break;
			case REBIRTH_RECOVERY_EXP:
				{
					AddMessageEx ( UIMSG_MODAL_CANCEL );
					m_bDONE_ACTION = true;
				}
				break;
			}
		}
		else
		{
			CString strLeftTime;
			strLeftTime.Format ( "%1.0f%s", floor(m_fTIME_LEFT) + 1.0f, ID2GAMEWORD("WAITSERVER_TIMELEFT_UNIT"));

			m_pTextBox->ClearText ();
			m_pTextBox->AddText ( ID2GAMEINTEXT("WAITSERVER_MESSAGE") );
			m_pTextBox->AddText ( strLeftTime );
		}	
	}

	else if ( m_bAutoRebirth )
	{
			m_fSelTIME_LEFT -= fElapsedTime;

			bool bTimeEvent = false;
			if ( m_fSelTIME_LEFT < -1.0f ) bTimeEvent = true;

			if ( bTimeEvent )
			{
				// Default
				m_nACTION = REBIRTH_CURRENT_POSITION;
				
				DoACTION ();
				HideButton ();
			}
			else 
			{
				CString strLeftTime;
				strLeftTime.Format ( "%1.0f%s", floor(m_fSelTIME_LEFT) + 1.0f, ID2GAMEWORD("WAITSERVER_TIMELEFT_UNIT"));
		
				m_pTextBox->SetText ( ID2GAMEINTEXT("REBIRTH_DIALOGUE_TEXT") );
				m_pTextBox->AddText ( strLeftTime );
			}
	}

}


void CRebirthDialogue::HideButton ()
{
	m_pCheckPointButton->SetVisibleSingle ( FALSE );
	m_pCurrentPositionButton->SetVisibleSingle ( FALSE );
	m_pRecoveryExpButton->SetVisibleSingle( FALSE );
}

void CRebirthDialogue::ShowButton ()
{
	m_pCheckPointButton->SetVisibleSingle ( TRUE );
	m_pCurrentPositionButton->SetVisibleSingle ( TRUE );
	m_pRecoveryExpButton->SetVisibleSingle( TRUE );
}
CBasicTextButton* CRebirthDialogue::CreateTextButton23 ( char* szButton, UIGUID ControlID , char* szText )
{
	CBasicTextButton* pTextButton = new CBasicTextButton;
	pTextButton->CreateSub ( this, "BASIC_TEXT_BUTTON23", UI_FLAG_XSIZE, ControlID );
	pTextButton->CreateBaseButton ( szButton, CBasicTextButton::SIZE23, CBasicButton::RADIO_FLIP, szText );
	pTextButton->SetAlignFlag ( UI_FLAG_BOTTOM );
	pTextButton->SetFlip ( TRUE );
	RegisterControl ( pTextButton );

	return pTextButton;
}
CBasicTextButton* CRebirthDialogue::CreateTextButton24 ( char* szButton, UIGUID ControlID , char* szText )
{
	const int nBUTTONSIZE = CBasicTextButton::SIZE24;
	CBasicTextButton* pButton = new CBasicTextButton;
	pButton->CreateSub ( this, "BASIC_TEXT_BUTTON24", UI_FLAG_XSIZE, ControlID );
	pButton->CreateBaseButton ( szButton, nBUTTONSIZE, CBasicButton::CLICK_FLIP, szText );
	RegisterControl ( pButton );
	return pButton;
}
CBasicButton* CRebirthDialogue::CreateFlipButton ( char* szButton, char* szButtonFlip, UIGUID ControlID )
{
	CBasicButton* pButton = new CBasicButton;
	pButton->CreateSub ( this, szButton, UI_FLAG_DEFAULT, ControlID );
	pButton->CreateFlip ( szButtonFlip, CBasicButton::RADIO_FLIP );
	pButton->SetControlNameEx ( szButton );
	RegisterControl ( pButton );
	return pButton;
}
CBasicTextBox* CRebirthDialogue::CreateStaticControl ( char* szControlKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID )
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub ( this, szControlKeyword, UI_FLAG_DEFAULT, cID );
	pStaticText->SetFont ( pFont );
	pStaticText->SetTextAlign ( nAlign );	
	RegisterControl ( pStaticText );

	return pStaticText;
}