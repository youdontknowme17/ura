#include "StdAfx.h"
#include "BasicUIOption.h"
#include "../EngineUILib/GUInterface/BasicButton.h"
#include "../EngineUILib/GUInterface/BasicTextBox.h"
#include "../EngineUiLib/GUInterface/BasicButtonText.h"
#include "GameTextControl.h"
#include "UITextControl.h"
#include "BasicTextButton.h"
#include "../EngineLib/DxCommon/DxFontMan.h"

#include "DxParamSet.h"
#include "RANPARAM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CBasicUIOption::CBasicUIOption () :
	m_pSimpleHPButton ( NULL ),
	m_pMovableOnChatButton ( NULL ),
	m_pUserDamageButton ( NULL ),
	m_pNameDamageButton ( NULL )
{
}

CBasicUIOption::~CBasicUIOption ()
{
}

void CBasicUIOption::CreateSubControl ()
{
	m_pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );

	CBasicTextBox* pTextBox = NULL;

	{
		DWORD dwFontColor = NS_UITEXTCOLOR::DEFAULT;
		DWORD dwFontDisableColor = NS_UITEXTCOLOR::DARKGRAY;
		int nAlign = TEXT_ALIGN_LEFT;

		// HP Mini Gauge
		pTextBox = CreateStaticControl ( "HWOPTION_UI_SIMPLEHP_STATIC", m_pFont, dwFontColor, nAlign );
		pTextBox->SetOneLineText ( ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION", 3 ) );

		// Transmission
		pTextBox = CreateStaticControl ( "HWOPTION_UI_MOVABLE_ON_CHAT_STATIC", m_pFont, dwFontColor, nAlign );
		pTextBox->SetOneLineText ( ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION", 6 ) );

		// View User Damage
		pTextBox = CreateStaticControl ( "HWOPTION_UI_USER_DAMAGE_STATIC", m_pFont, dwFontColor, nAlign );
		pTextBox->SetOneLineText ( ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION", 20 ) );

		// Remove Name & Damage
		pTextBox = CreateStaticControl ( "HWOPTION_UI_VIEW_NAME_DAMAGE_STATIC", m_pFont, dwFontDisableColor, nAlign );
		pTextBox->SetOneLineText ( ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION", 21 ) );
	}

	{
		m_pSimpleHPButton = CreateFlipButton( "HWOPTION_UI_SIMPLEHP_BUTTON", "HWOPTION_UI_SIMPLEHP_BUTTON_F", HWOPTION_UI_SIMPLEHP_BUTTON );
		m_pUserDamageButton = CreateFlipButton( "HWOPTION_UI_VIEW_DAMAGE_BUTTON", "HWOPTION_UI_VIEW_DAMAGE_BUTTON_F", HWOPTION_UI_VIEW_DAMAGE_BUTTON );
		m_pNameDamageButton = CreateFlipButton( "HWOPTION_UI_VIEW_NAME_DAMAGE_BUTTON", "HWOPTION_UI_VIEW_NAME_DAMAGE_BUTTON_F", HWOPTION_UI_VIEW_NAME_DAMAGE_BUTTON );
		m_pMovableOnChatButton = CreateFlipButton( "HWOPTION_UI_MOVABLE_ON_CHAT_BUTTON", "HWOPTION_UI_MOVABLE_ON_CHAT_BUTTON_F", HWOPTION_UI_MOVABLE_ON_CHAT_BUTTON );
	}

	{
		m_pDefaultPosButton = CreateTextButton24 ( "HWOPTION_UI_DEFAULTPOS_BUTTON", HWOPTION_UI_DEFAULTPOS_BUTTON, ID2GAMEWORD("HWOPTION_GAMEPLAY_OPTION", 5) );
	}
}

void CBasicUIOption::LoadSimpleHP ()
{
	m_bSimpleHP = DXPARAMSET::GetInstance().m_bSHOW_SIMPLEHP;
	m_pSimpleHPButton->SetFlip ( m_bSimpleHP );
}

void CBasicUIOption::LoadMovableOnChat ()
{
	m_bMovableOnChat = RANPARAM::bMOVABLE_ON_CHAT;
	m_pMovableOnChatButton->SetFlip ( m_bMovableOnChat );
}


CBasicTextButton*  CBasicUIOption::CreateTextButton24 ( const char* szButton, UIGUID ControlID, const char* szText )
{
	const int nBUTTONSIZE = CBasicTextButton::SIZE24;
	CBasicTextButton* pTextButton = new CBasicTextButton;
	pTextButton->CreateSub ( this, "BASIC_TEXT_BUTTON24", UI_FLAG_XSIZE|UI_FLAG_YSIZE, ControlID );
	pTextButton->CreateBaseButton ( szButton, nBUTTONSIZE, CBasicButton::CLICK_FLIP, szText ,_DEFAULT_FONT_FLAG );
	RegisterControl ( pTextButton );
	return pTextButton;
}

CBasicButton* CBasicUIOption::CreateFlipButton ( char* szButton, char* szButtonFlip, UIGUID ControlID )
{
	CBasicButton* pButton = new CBasicButton;
	pButton->CreateSub ( this, szButton, UI_FLAG_DEFAULT, ControlID );
	pButton->CreateFlip ( szButtonFlip, CBasicButton::RADIO_FLIP );
	pButton->SetControlNameEx ( szButton );
	RegisterControl ( pButton );
	return pButton;
}

CBasicTextBox* CBasicUIOption::CreateStaticControl ( char* szControlKeyword, CD3DFontPar* pFont, D3DCOLOR dwColor, int nAlign )
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub ( this, szControlKeyword );
	pStaticText->SetFont ( pFont );
	pStaticText->SetTextAlign ( nAlign );	
	RegisterControl ( pStaticText );
	return pStaticText;
}

void CBasicUIOption::LoadCurrentOption()
{
	LoadSimpleHP();
	LoadMovableOnChat();
}

void CBasicUIOption::SetVisibleSingle( BOOL bVisible )
{
	CUIGroup::SetVisibleSingle ( bVisible );

	if( bVisible )
	{
		LoadCurrentOption();
	}
}