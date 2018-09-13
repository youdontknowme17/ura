#include "StdAfx.h"
#include "./CharacterWindowCharStat.h"

#include "../EngineUiLib/GUInterface/BasicTextBox.h"
#include "../EngineUiLib/GUInterface/BasicProgressBar.h"
#include "../EngineUiLib/GUInterface/BasicButtonText.h"
#include "../EngineUiLib/GUInterface/BasicButton.h"
#include "../EngineLib/DxCommon/d3dfont.h"

#include "../BasicTextButton.h"
#include "../UITextControl.h"
#include "../GameTextControl.h"
#include "../RanClientLib/G-Logic/GLGaeaClient.h"

#include "../InnerInterface.h"
#include "../ModalCallerID.h"
#include "../ModalWindow.h"

#include "../BasicLineBox.h"
#include "../BasicLineBoxEx.h"
#include "GLItemMan.h"
#include "GLItem.h"

#include "CharacterWindowCharStatMark.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCharacterWindowCharStat::CCharacterWindowCharStat () :
	m_pPowButton_Plus ( NULL ),
	m_pDexButton_Plus ( NULL ),
	m_pSpiButton_Plus ( NULL ),
	m_pStrButton_Plus ( NULL ),
	m_pStaButton_Plus ( NULL ),
	m_pPowButton_Minus ( NULL ),
	m_pDexButton_Minus ( NULL ),
	m_pSpiButton_Minus ( NULL ),
	m_pStrButton_Minus ( NULL ),
	m_pStaButton_Minus ( NULL ),
	m_pButtonStatReset ( NULL ),
	m_pButtonStatApply ( NULL ),
	m_pPowText ( NULL ),
	m_pDexText ( NULL ),
	m_pSpiText ( NULL ),
	m_pStrText ( NULL ),
	m_pStaText ( NULL ),
	m_pPowText_Up ( NULL ),
	m_pDexText_Up ( NULL ),
	m_pSpiText_Up ( NULL ),
	m_pStrText_Up ( NULL ),
	m_pStaText_Up ( NULL ),
	m_pPointText ( NULL ),
	m_pAttackMinText ( NULL ),
	m_pAttackMaxText ( NULL ),
	m_pDefenseText ( NULL ),
	m_pFightText ( NULL ),
	m_pShootText ( NULL ),
	m_pEnergyText ( NULL ),
	m_pHitText ( NULL ),
	m_pAvoidText ( NULL ),
	m_pHPText ( NULL ),
	m_pMPText ( NULL ),
	m_pSPText ( NULL ),
/*	m_pFireText ( NULL ),
	m_pColdText ( NULL ),
	m_pElectricText ( NULL ),
	m_pPoisonText ( NULL ),
	m_pSpiritText ( NULL ),*/
	m_pHelp ( NULL ),
	m_pAttackMinMarkStatic ( NULL ),
	m_pAttackMaxMarkStatic ( NULL ),
	m_pDefenseMarkStatic ( NULL ),
	m_pFightMarkStatic ( NULL ),
	m_pShootMarkStatic ( NULL ),
	m_pEnergyMarkStatic ( NULL ),
	m_pHitMarkStatic ( NULL ),
	m_pAvoidMarkStatic ( NULL ),
	m_pHPMarkStatic ( NULL ),
	m_pMPMarkStatic ( NULL ),
	m_pSPMarkStatic ( NULL ),
	m_pAttackMinMarkValue ( NULL ),
	m_pAttackMaxMarkValue ( NULL ),
	m_pDefenseMarkValue ( NULL ),
	m_pFightMarkValue ( NULL ),
	m_pShootMarkValue ( NULL ),
	m_pEnergyMarkValue ( NULL ),
	m_pHitMarkValue ( NULL ),
	m_pAvoidMarkValue ( NULL ),
	m_pHPMarkValue ( NULL ),
	m_pMPMarkValue ( NULL ),
	m_pSPMarkValue ( NULL ),
	m_pAttackMinMark ( NULL ),
	m_pAttackMaxMark ( NULL ),
	m_pDefenseMark ( NULL ),
	m_pFightMark ( NULL ),
	m_pShootMark ( NULL ),
	m_pEnergyMark ( NULL ),
	m_pHitMark ( NULL ),
	m_pAvoidMark ( NULL ),
	m_pHPMark ( NULL ),
	m_pMPMark ( NULL ),
	m_pSPMark ( NULL ),
	m_dwPowAdd( 0 ),
	m_dwDexAdd( 0 ),
	m_dwSpiAdd( 0 ),
	m_dwStrAdd( 0 ),
	m_dwStaAdd( 0 ),
	m_dwStatsDeduct( 0 )
{
	SecureZeroMemory( m_pMouseOver, sizeof(m_pMouseOver) );
}

CCharacterWindowCharStat::~CCharacterWindowCharStat ()
{
}

CBasicTextBox* CCharacterWindowCharStat::CreateStaticControl ( char* szControlKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID )
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub ( this, szControlKeyword, UI_FLAG_DEFAULT, cID );
	pStaticText->SetFont ( pFont );
	pStaticText->SetTextAlign ( nAlign );	
	RegisterControl ( pStaticText );

	return pStaticText;
}

CUIControl*	CCharacterWindowCharStat::CreateControl ( const char* szControl, const UIGUID& cID )
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, szControl, UI_FLAG_DEFAULT, cID );
	RegisterControl ( pControl );
	return pControl;
}

CBasicTextButton*  CCharacterWindowCharStat::CreateTextButton16 ( const char* szButton, UIGUID ControlID, const char* szText )
{
	const int nBUTTONSIZE = CBasicTextButton::SIZE16;
	CBasicTextButton* pTextButton = new CBasicTextButton;
	pTextButton->CreateSub ( this, "BASIC_TEXT_BUTTON16", UI_FLAG_XSIZE|UI_FLAG_YSIZE, ControlID );
	pTextButton->CreateBaseButton ( szButton, nBUTTONSIZE, CBasicButton::CLICK_FLIP, szText ,_DEFAULT_FONT_FLAG );
	RegisterControl ( pTextButton );
	return pTextButton;
}

CBasicTextButton*  CCharacterWindowCharStat::CreateTextButton24 ( const char* szButton, UIGUID ControlID, const char* szText )
{
	const int nBUTTONSIZE = CBasicTextButton::SIZE24;
	CBasicTextButton* pTextButton = new CBasicTextButton;
	pTextButton->CreateSub ( this, "BASIC_TEXT_BUTTON24", UI_FLAG_XSIZE|UI_FLAG_YSIZE, ControlID );
	pTextButton->CreateBaseButton ( szButton, nBUTTONSIZE, CBasicButton::CLICK_FLIP, szText ,_DEFAULT_FONT_FLAG );
	RegisterControl ( pTextButton );
	return pTextButton;
}
CBasicButtonText* CCharacterWindowCharStat::CreateButtonBlue ( char* szButton, char* szTextBox, CD3DFontPar* pFont, int nAlign, UIGUID ControlID, CString strText )
{
	CBasicButtonText* pButton = new CBasicButtonText;
	pButton->CreateRanButtonBlue ( this, szButton, ControlID );
	pButton->CreateTextBox ( szTextBox, pFont, nAlign );
	pButton->SetOneLineText( strText.GetString() );
	RegisterControl ( pButton );

	return pButton;
}

CBasicButtonText* CCharacterWindowCharStat::CreateButtonRed ( char* szButton, char* szTextBox, CD3DFontPar* pFont, int nAlign, UIGUID ControlID, CString strText )
{
	CBasicButtonText* pButton = new CBasicButtonText;
	pButton->CreateRanButtonRed ( this, szButton, ControlID );
	pButton->CreateTextBox ( szTextBox, pFont, nAlign );
	pButton->SetOneLineText( strText.GetString() );
	RegisterControl ( pButton );

	return pButton;
}

CBasicButton* CCharacterWindowCharStat::CreateMouseIn( const char* szButton, const char* szMouseIn ,UIGUID ControlID )
{
	CBasicButton* pButton = new CBasicButton;
	pButton->CreateSub ( this, szButton, UI_FLAG_DEFAULT, ControlID );
	pButton->CreateFlip ( szMouseIn, CBasicButton::MOUSEIN_FLIP );
	RegisterControl ( pButton );
	return pButton;
}

CCharacterWindowCharStatMark*	CCharacterWindowCharStat::CreateStatMark ( const char* szControl, const UIGUID& cID )
{
	CCharacterWindowCharStatMark* pControl = new CCharacterWindowCharStatMark;
	pControl->CreateSub ( this, szControl, UI_FLAG_DEFAULT, cID );
	pControl->CreateSubControl();
	RegisterControl ( pControl );
	return pControl;
}

void CCharacterWindowCharStat::CreateSubControl ()
{
/*	char* szLineBox[9] = 
	{
		"CHARACTER_WINDOW_STAT_POINT1_REGION",
		"CHARACTER_WINDOW_STAT_POINT2_REGION",
		"CHARACTER_WINDOW_STAT_POINT3_REGION",
		"CHARACTER_WINDOW_STAT_POINT4_REGION",
		"CHARACTER_WINDOW_STAT_POINT5_REGION",
		"CHARACTER_WINDOW_STAT_POINT6_REGION",
		"CHARACTER_WINDOW_STAT_POINT7_REGION",
		"CHARACTER_WINDOW_STAT_POINT8_REGION",
		"CHARACTER_WINDOW_STAT_POINT9_REGION",
	};

	for ( int i=0; i<9; ++i )
	{
		CBasicLineBox* pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBox->CreateBaseBoxQuestList( szLineBox[i] );
		RegisterControl ( pBasicLineBox );
	}*/
	
	{	// Đen Xám Background
		CBasicLineBoxEx* pBasicLineBoxEx0 = new CBasicLineBoxEx;
		pBasicLineBoxEx0->CreateSub ( this, "BASIC_LINE_BOX_EX_BODY_LOM", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBoxEx0->CreateBaseBoxLomBox ( "CHARACTER_WINDOW_STAT_POINT1_REGION" );
		RegisterControl ( pBasicLineBoxEx0 );
	}
	
	{	// Đen Xám Background
		CBasicLineBoxEx* pBasicLineBoxEx1 = new CBasicLineBoxEx;
		pBasicLineBoxEx1->CreateSub ( this, "BASIC_LINE_BOX_EX_BODY_LOM", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBoxEx1->CreateBaseBoxLomBox ( "CHARACTER_WINDOW_STAT_POINT2_REGION" );
		RegisterControl ( pBasicLineBoxEx1 );
	}
	
	{	// Đen Xám Background
		CBasicLineBoxEx* pBasicLineBoxEx2 = new CBasicLineBoxEx;
		pBasicLineBoxEx2->CreateSub ( this, "BASIC_LINE_BOX_EX_BODY_LOM", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBoxEx2->CreateBaseBoxLomBox ( "CHARACTER_WINDOW_STAT_POINT3_REGION" );
		RegisterControl ( pBasicLineBoxEx2 );
	}
	
	{	// Đen Xám Background
		CBasicLineBoxEx* pBasicLineBoxEx3 = new CBasicLineBoxEx;
		pBasicLineBoxEx3->CreateSub ( this, "BASIC_LINE_BOX_EX_BODY_LOM", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBoxEx3->CreateBaseBoxLomBox ( "CHARACTER_WINDOW_STAT_POINT4_REGION" );
		RegisterControl ( pBasicLineBoxEx3 );
	}
	
	{	// Đen Xám Background
		CBasicLineBoxEx* pBasicLineBoxEx4 = new CBasicLineBoxEx;
		pBasicLineBoxEx4->CreateSub ( this, "BASIC_LINE_BOX_EX_BODY_LOM", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBoxEx4->CreateBaseBoxLomBox ( "CHARACTER_WINDOW_STAT_POINT5_REGION" );
		RegisterControl ( pBasicLineBoxEx4 );
	}
	
	{	// Đen Xám Background
		CBasicLineBoxEx* pBasicLineBoxEx5 = new CBasicLineBoxEx;
		pBasicLineBoxEx5->CreateSub ( this, "BASIC_LINE_BOX_EX_BODY_BANG", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBoxEx5->CreateBaseBoxBangBox ( "CHARACTER_WINDOW_STAT_POINT6_REGION" );
		RegisterControl ( pBasicLineBoxEx5 );
	}
	
	{	// Đen Xám Background
		CBasicLineBoxEx* pBasicLineBoxEx6 = new CBasicLineBoxEx;
		pBasicLineBoxEx6->CreateSub ( this, "BASIC_LINE_BOX_EX_BODY_BANG", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBoxEx6->CreateBaseBoxBangBox ( "CHARACTER_WINDOW_STAT_POINT7_REGION" );
		RegisterControl ( pBasicLineBoxEx6 );
	}
	
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, _DEFAULT_FONT_FLAG );
	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );
	CD3DFontPar* pFont2 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, D3DFONT_SHADOW | D3DFONT_ASCII );
	CD3DFontPar* pFont3 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, D3DFONT_SHADOW | D3DFONT_ASCII );

	const int nAlignLeft = TEXT_ALIGN_LEFT;
	const int nAlignCenter = TEXT_ALIGN_CENTER_X;
	const int nAlignCenterBoth = TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y;
	const int nAlignRight = TEXT_ALIGN_RIGHT;
	const DWORD& dwSilver = NS_UITEXTCOLOR::WHITE;
	const DWORD& dwSubTitle = NS_UITEXTCOLOR::PALEGOLDENROD;

	//mouse in
	m_pMouseOver[0] = CreateMouseIn( "CHARACTER_WINDOW_STAT_1_POW_MOUSEOVER_RECT", "CHARACTER_WINDOW_MOUSEIN_DUMMY1", MOUSE_OVER_STAT_1_POW );
	m_pMouseOver[1] = CreateMouseIn( "CHARACTER_WINDOW_STAT_1_DEX_MOUSEOVER_RECT", "CHARACTER_WINDOW_MOUSEIN_DUMMY1", MOUSE_OVER_STAT_1_DEX );
	m_pMouseOver[2] = CreateMouseIn( "CHARACTER_WINDOW_STAT_1_SPI_MOUSEOVER_RECT", "CHARACTER_WINDOW_MOUSEIN_DUMMY1", MOUSE_OVER_STAT_1_SPI );
	m_pMouseOver[3] = CreateMouseIn( "CHARACTER_WINDOW_STAT_1_STR_MOUSEOVER_RECT", "CHARACTER_WINDOW_MOUSEIN_DUMMY1", MOUSE_OVER_STAT_1_STR );
	m_pMouseOver[4] = CreateMouseIn( "CHARACTER_WINDOW_STAT_1_STA_MOUSEOVER_RECT", "CHARACTER_WINDOW_MOUSEIN_DUMMY1", MOUSE_OVER_STAT_1_STA );

	m_pMouseOver[5] = CreateMouseIn( "CHARACTER_WINDOW_STAT_2_ATTACK_MIN_MOUSEOVER_RECT", "CHARACTER_WINDOW_MOUSEIN_DUMMY2", MOUSE_OVER_STAT_2_ATTACK_MIN );
	m_pMouseOver[6] = CreateMouseIn( "CHARACTER_WINDOW_STAT_2_ATTACK_MAX_MOUSEOVER_RECT", "CHARACTER_WINDOW_MOUSEIN_DUMMY2", MOUSE_OVER_STAT_2_ATTACK_MAX );
	m_pMouseOver[7] = CreateMouseIn( "CHARACTER_WINDOW_STAT_2_DEFENSE_MOUSEOVER_RECT", "CHARACTER_WINDOW_MOUSEIN_DUMMY2", MOUSE_OVER_STAT_2_DEFENSE );
	m_pMouseOver[8] = CreateMouseIn( "CHARACTER_WINDOW_STAT_2_FIGHT_MOUSEOVER_RECT", "CHARACTER_WINDOW_MOUSEIN_DUMMY2", MOUSE_OVER_STAT_2_FIGHT );
	m_pMouseOver[9] = CreateMouseIn( "CHARACTER_WINDOW_STAT_2_SHOOT_MOUSEOVER_RECT", "CHARACTER_WINDOW_MOUSEIN_DUMMY2", MOUSE_OVER_STAT_2_SHOOT );
	m_pMouseOver[10] = CreateMouseIn( "CHARACTER_WINDOW_STAT_2_STAMINA_MOUSEOVER_RECT", "CHARACTER_WINDOW_MOUSEIN_DUMMY2", MOUSE_OVER_STAT_2_STAMINA );
	m_pMouseOver[11] = CreateMouseIn( "CHARACTER_WINDOW_STAT_2_HIT_MOUSEOVER_RECT", "CHARACTER_WINDOW_MOUSEIN_DUMMY2", MOUSE_OVER_STAT_2_HIT );
	m_pMouseOver[12] = CreateMouseIn( "CHARACTER_WINDOW_STAT_2_AVOID_MOUSEOVER_RECT", "CHARACTER_WINDOW_MOUSEIN_DUMMY2", MOUSE_OVER_STAT_2_AVOID );
	m_pMouseOver[13] = CreateMouseIn( "CHARACTER_WINDOW_STAT_2_HP_MOUSEOVER_RECT", "CHARACTER_WINDOW_MOUSEIN_DUMMY2", MOUSE_OVER_STAT_2_HP );
	m_pMouseOver[14] = CreateMouseIn( "CHARACTER_WINDOW_STAT_2_MP_MOUSEOVER_RECT", "CHARACTER_WINDOW_MOUSEIN_DUMMY2", MOUSE_OVER_STAT_2_MP );
	m_pMouseOver[15] = CreateMouseIn( "CHARACTER_WINDOW_STAT_2_SP_MOUSEOVER_RECT", "CHARACTER_WINDOW_MOUSEIN_DUMMY2", MOUSE_OVER_STAT_2_SP );
/*	m_pMouseOver[16] = CreateMouseIn( "CHARACTER_WINDOW_STAT_3_FIRE_RESIST_MOUSEOVER_RECT", "CHARACTER_WINDOW_MOUSEIN_DUMMY2", MOUSE_OVER_STAT_3_FIRE );
	m_pMouseOver[17] = CreateMouseIn( "CHARACTER_WINDOW_STAT_3_COLD_RESIST_MOUSEOVER_RECT", "CHARACTER_WINDOW_MOUSEIN_DUMMY2", MOUSE_OVER_STAT_3_COLD );
	m_pMouseOver[18] = CreateMouseIn( "CHARACTER_WINDOW_STAT_3_ELECTRIC_RESIST_MOUSEOVER_RECT", "CHARACTER_WINDOW_MOUSEIN_DUMMY2", MOUSE_OVER_STAT_3_ELECTRIC );
	m_pMouseOver[19] = CreateMouseIn( "CHARACTER_WINDOW_STAT_3_POISON_RESIST_MOUSEOVER_RECT", "CHARACTER_WINDOW_MOUSEIN_DUMMY3", MOUSE_OVER_STAT_3_POISON );
	m_pMouseOver[20] = CreateMouseIn( "CHARACTER_WINDOW_STAT_3_SPIRIT_RESIST_MOUSEOVER_RECT", "CHARACTER_WINDOW_MOUSEIN_DUMMY3", MOUSE_OVER_STAT_3_SPIRIT );*/

	//stats text
	m_pPowTextStatic = CreateStaticControl ( "CHARACTER_WINDOW_STAT_1_POW_TEXT", pFont2, nAlignLeft );
	m_pPowTextStatic->AddText ( ID2GAMEWORD("CHARACTER_STAT_STATIC", 0), dwSilver );
	m_pDexTextStatic = CreateStaticControl ( "CHARACTER_WINDOW_STAT_1_DEX_TEXT", pFont2, nAlignLeft );
	m_pDexTextStatic->AddText ( ID2GAMEWORD("CHARACTER_STAT_STATIC", 1), dwSilver );
	m_pSpiTextStatic = CreateStaticControl ( "CHARACTER_WINDOW_STAT_1_SPI_TEXT", pFont2, nAlignLeft );
	m_pSpiTextStatic->AddText ( ID2GAMEWORD("CHARACTER_STAT_STATIC", 2), dwSilver );
	m_pStrTextStatic = CreateStaticControl ( "CHARACTER_WINDOW_STAT_1_STR_TEXT", pFont2, nAlignLeft );
	m_pStrTextStatic->AddText ( ID2GAMEWORD("CHARACTER_STAT_STATIC", 4), dwSilver );
	m_pStaTextStatic = CreateStaticControl ( "CHARACTER_WINDOW_STAT_1_STA_TEXT", pFont2, nAlignLeft );
	m_pStaTextStatic->AddText ( ID2GAMEWORD("CHARACTER_STAT_STATIC", 5), dwSilver );

	m_pPowText = CreateStaticControl ( "CHARACTER_WINDOW_STAT_1_POW_VALUE", pFont2, nAlignCenter );
	m_pDexText = CreateStaticControl ( "CHARACTER_WINDOW_STAT_1_DEX_VALUE", pFont2, nAlignCenter );
	m_pSpiText = CreateStaticControl ( "CHARACTER_WINDOW_STAT_1_SPI_VALUE", pFont2, nAlignCenter );
	m_pStrText = CreateStaticControl ( "CHARACTER_WINDOW_STAT_1_STR_VALUE", pFont2, nAlignCenter );
	m_pStaText = CreateStaticControl ( "CHARACTER_WINDOW_STAT_1_STA_VALUE", pFont2, nAlignCenter );
	m_pPowText_Up = CreateStaticControl ( "CHARACTER_WINDOW_STAT_1_POW_UP_VALUE", pFont2, nAlignRight );
	m_pDexText_Up = CreateStaticControl ( "CHARACTER_WINDOW_STAT_1_DEX_UP_VALUE", pFont2, nAlignRight );
	m_pSpiText_Up = CreateStaticControl ( "CHARACTER_WINDOW_STAT_1_SPI_UP_VALUE", pFont2, nAlignRight );
	m_pStrText_Up = CreateStaticControl ( "CHARACTER_WINDOW_STAT_1_STR_UP_VALUE", pFont2, nAlignRight );
	m_pStaText_Up = CreateStaticControl ( "CHARACTER_WINDOW_STAT_1_STA_UP_VALUE", pFont2, nAlignRight );

	//stats buttons
	m_pPowButton_Plus = CreateTextButton16 ( "CHARACTER_WINDOW_STAT_POW_PLUS_BUTTON", CHARACTER_POW_BUTTON_PLUS, "+" );		
	m_pDexButton_Plus = CreateTextButton16 ( "CHARACTER_WINDOW_STAT_DEX_PLUS_BUTTON", CHARACTER_DEX_BUTTON_PLUS, "+" );
	m_pSpiButton_Plus = CreateTextButton16 ( "CHARACTER_WINDOW_STAT_SPI_PLUS_BUTTON", CHARACTER_SPI_BUTTON_PLUS, "+" );
	m_pStrButton_Plus = CreateTextButton16 ( "CHARACTER_WINDOW_STAT_STR_PLUS_BUTTON", CHARACTER_STR_BUTTON_PLUS, "+" );
	m_pStaButton_Plus = CreateTextButton16 ( "CHARACTER_WINDOW_STAT_STA_PLUS_BUTTON", CHARACTER_STA_BUTTON_PLUS, "+" );
	m_pPowButton_Minus = CreateTextButton16 ( "CHARACTER_WINDOW_STAT_POW_MINUS_BUTTON", CHARACTER_POW_BUTTON_MINUS, "-" );		
	m_pDexButton_Minus = CreateTextButton16 ( "CHARACTER_WINDOW_STAT_DEX_MINUS_BUTTON", CHARACTER_DEX_BUTTON_MINUS, "-" );
	m_pSpiButton_Minus = CreateTextButton16 ( "CHARACTER_WINDOW_STAT_SPI_MINUS_BUTTON", CHARACTER_SPI_BUTTON_MINUS, "-" );
	m_pStrButton_Minus = CreateTextButton16 ( "CHARACTER_WINDOW_STAT_STR_MINUS_BUTTON", CHARACTER_STR_BUTTON_MINUS, "-" );
	m_pStaButton_Minus = CreateTextButton16 ( "CHARACTER_WINDOW_STAT_STA_MINUS_BUTTON", CHARACTER_STA_BUTTON_MINUS, "-" );
	m_pButtonStatReset = CreateTextButton24 ( "CHARACTER_WINDOW_STAT_INIT_BUTTON", CHARACTER_STAT_RESET, ID2GAMEWORD("CHARACTER_STAT_INIT", 0) );
	m_pButtonStatApply = CreateTextButton24 ( "CHARACTER_WINDOW_STAT_APPLY_BUTTON", CHARACTER_STAT_APPLY, ID2GAMEWORD("CHARACTER_STAT_APPLY", 0) );

	CBasicTextBox* pTextBox = NULL;
	pTextBox = CreateStaticControl ( "CHARACTER_WINDOW_STAT_1_POINT_TEXT", pFont2, nAlignLeft );
	pTextBox->AddText ( ID2GAMEWORD("CHARACTER_STAT_POINT", 0), dwSilver );

	pTextBox = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_ATTACK_MIN_TEXT", pFont2, nAlignLeft );
	pTextBox->AddText ( ID2GAMEWORD("CHARACTER_STATUS_STATIC", 15), dwSilver );
	pTextBox = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_ATTACK_MAX_TEXT", pFont2, nAlignLeft );
	pTextBox->AddText ( ID2GAMEWORD("CHARACTER_STATUS_STATIC", 14), dwSilver );
	pTextBox = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_DEFENSE_TEXT", pFont2, nAlignLeft );
	pTextBox->AddText ( ID2GAMEWORD("CHARACTER_STATUS_STATIC", 3), dwSilver );
	pTextBox = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_FIGHT_TEXT", pFont2, nAlignLeft );
	pTextBox->AddText ( ID2GAMEWORD("CHARACTER_STATUS_STATIC", 4), dwSilver );
	pTextBox = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_SHOOT_TEXT", pFont2, nAlignLeft );
	pTextBox->AddText ( ID2GAMEWORD("CHARACTER_STATUS_STATIC", 5), dwSilver );
	pTextBox = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_STAMINA_TEXT", pFont2, nAlignLeft );
	pTextBox->AddText ( ID2GAMEWORD("CHARACTER_STATUS_STATIC", 11), dwSilver );
	pTextBox = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_HIT_TEXT", pFont2, nAlignLeft );
	pTextBox->AddText ( ID2GAMEWORD("CHARACTER_STATUS_STATIC", 9), dwSilver );
	pTextBox = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_AVOID_TEXT", pFont2, nAlignLeft );
	pTextBox->AddText ( ID2GAMEWORD("CHARACTER_STATUS_STATIC", 10), dwSilver );
	pTextBox = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_HP_TEXT", pFont2, nAlignLeft );
	pTextBox->AddText ( ID2GAMEWORD("CHARACTER_STATUS_STATIC", 6), dwSilver );
	pTextBox = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_MP_TEXT", pFont2, nAlignLeft );
	pTextBox->AddText ( ID2GAMEWORD("CHARACTER_STATUS_STATIC", 7), dwSilver );
	pTextBox = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_SP_TEXT", pFont2, nAlignLeft );
	pTextBox->AddText ( ID2GAMEWORD("CHARACTER_STATUS_STATIC", 8), dwSilver );

	m_pPointText = CreateStaticControl ( "CHARACTER_WINDOW_STAT_1_POINT_VALUE", pFont2, nAlignRight );
	m_pAttackMinText = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_ATTACK_MIN_VALUE", pFont2, nAlignRight );
	m_pAttackMaxText = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_ATTACK_MAX_VALUE",pFont2, nAlignRight );
	m_pDefenseText = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_DEFENSE_VALUE", pFont2, nAlignRight );
	m_pFightText = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_FIGHT_VALUE", pFont2, nAlignRight );
	m_pShootText = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_SHOOT_VALUE", pFont2, nAlignRight );
	m_pEnergyText = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_STAMINA_VALUE", pFont2, nAlignRight );
	m_pHitText = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_HIT_VALUE", pFont2, nAlignRight );
	m_pAvoidText = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_AVOID_VALUE", pFont2, nAlignRight );
	m_pHPText = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_HP_VALUE", pFont2, nAlignRight );
	m_pMPText = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_MP_VALUE", pFont2, nAlignRight );
	m_pSPText = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_SP_VALUE", pFont2, nAlignRight );

/*	pTextBox = CreateStaticControl ( "CHARACTER_WINDOW_STAT_3_FIRE_RESIST_TEXT", pFont2, nAlignLeft );
	pTextBox->AddText ( ID2GAMEWORD("CHARACTER_RESI_STATIC", 1), dwSilver );
	pTextBox = CreateStaticControl ( "CHARACTER_WINDOW_STAT_3_COLD_RESIST_TEXT", pFont2, nAlignLeft );
	pTextBox->AddText ( ID2GAMEWORD("CHARACTER_RESI_STATIC", 2), dwSilver );
	pTextBox = CreateStaticControl ( "CHARACTER_WINDOW_STAT_3_ELECTRIC_RESIST_TEXT", pFont2, nAlignLeft );
	pTextBox->AddText ( ID2GAMEWORD("CHARACTER_RESI_STATIC", 3), dwSilver );
	pTextBox = CreateStaticControl ( "CHARACTER_WINDOW_STAT_3_POISON_RESIST_TEXT", pFont2, nAlignLeft );
	pTextBox->AddText ( ID2GAMEWORD("CHARACTER_RESI_STATIC", 4), dwSilver );
	pTextBox = CreateStaticControl ( "CHARACTER_WINDOW_STAT_3_SPIRIT_RESIST_TEXT", pFont2, nAlignLeft );
	pTextBox->AddText ( ID2GAMEWORD("CHARACTER_RESI_STATIC", 5), dwSilver );*/

	m_pAttackMinMarkStatic = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_ATTACK_MIN_MARK", pFont2, nAlignRight );
	m_pAttackMaxMarkStatic = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_ATTACK_MAX_MARK", pFont2, nAlignRight );
	m_pDefenseMarkStatic = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_DEFENSE_MARK", pFont2, nAlignRight );
	m_pFightMarkStatic = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_FIGHT_MARK", pFont2, nAlignRight );
	m_pShootMarkStatic = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_SHOOT_MARK", pFont2, nAlignRight );
	m_pEnergyMarkStatic = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_STAMINA_MARK", pFont2, nAlignRight );
	m_pHitMarkStatic = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_HIT_MARK", pFont2, nAlignRight );
	m_pAvoidMarkStatic = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_AVOID_MARK", pFont2, nAlignRight );
	m_pHPMarkStatic = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_HP_MARK", pFont2, nAlignRight );
	m_pMPMarkStatic = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_MP_MARK", pFont2, nAlignRight );
	m_pSPMarkStatic = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_SP_MARK", pFont2, nAlignRight );

	m_pAttackMinMarkValue = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_ATTACK_MIN_UP_VALUE", pFont2, nAlignRight );
	m_pAttackMaxMarkValue = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_ATTACK_MAX_UP_VALUE", pFont2, nAlignRight );
	m_pDefenseMarkValue = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_DEFENSE_UP_VALUE", pFont2, nAlignRight );
	m_pFightMarkValue = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_FIGHT_UP_VALUE", pFont2, nAlignRight );
	m_pShootMarkValue = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_SHOOT_UP_VALUE", pFont2, nAlignRight );
	m_pEnergyMarkValue = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_STAMINA_UP_VALUE", pFont2, nAlignRight );
	m_pHitMarkValue = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_HIT_UP_VALUE", pFont2, nAlignRight );
	m_pAvoidMarkValue = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_AVOID_UP_VALUE", pFont2, nAlignRight );
	m_pHPMarkValue = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_HP_UP_VALUE", pFont2, nAlignRight );
	m_pMPMarkValue = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_MP_UP_VALUE", pFont2, nAlignRight );
	m_pSPMarkValue = CreateStaticControl ( "CHARACTER_WINDOW_STAT_2_SP_UP_VALUE", pFont2, nAlignRight );

/*	m_pFireText = CreateStaticControl ( "CHARACTER_WINDOW_STAT_3_FIRE_RESIST_VALUE", pFont2, nAlignRight );
	m_pColdText = CreateStaticControl ( "CHARACTER_WINDOW_STAT_3_COLD_RESIST_VALUE", pFont2, nAlignRight );
	m_pElectricText = CreateStaticControl ( "CHARACTER_WINDOW_STAT_3_ELECTRIC_RESIST_VALUE", pFont2, nAlignRight );
	m_pPoisonText = CreateStaticControl ( "CHARACTER_WINDOW_STAT_3_POISON_RESIST_VALUE", pFont2, nAlignRight );
	m_pSpiritText = CreateStaticControl ( "CHARACTER_WINDOW_STAT_3_SPIRIT_RESIST_VALUE", pFont2, nAlignRight );*/

	m_pAttackMinMark = CreateStatMark ( "CHARACTER_WINDOW_STAT_2_ATTACK_MIN_MARK_IMAGE" );
	m_pAttackMaxMark = CreateStatMark ( "CHARACTER_WINDOW_STAT_2_ATTACK_MAX_MARK_IMAGE" );
	m_pDefenseMark = CreateStatMark ( "CHARACTER_WINDOW_STAT_2_DEFENSE_MARK_IMAGE" );
	m_pFightMark = CreateStatMark ( "CHARACTER_WINDOW_STAT_2_FIGHT_MARK_IMAGE" );
	m_pShootMark = CreateStatMark ( "CHARACTER_WINDOW_STAT_2_SHOOT_MARK_IMAGE" );
	m_pEnergyMark = CreateStatMark ( "CHARACTER_WINDOW_STAT_2_STAMINA_MARK_IMAGE" );
	m_pHitMark = CreateStatMark ( "CHARACTER_WINDOW_STAT_2_HIT_MARK_IMAGE" );
	m_pAvoidMark = CreateStatMark ( "CHARACTER_WINDOW_STAT_2_AVOID_MARK_IMAGE" );
	m_pHPMark = CreateStatMark ( "CHARACTER_WINDOW_STAT_2_HP_MARK_IMAGE" );
	m_pMPMark = CreateStatMark ( "CHARACTER_WINDOW_STAT_2_MP_MARK_IMAGE" );
	m_pSPMark = CreateStatMark ( "CHARACTER_WINDOW_STAT_2_SP_MARK_IMAGE" );

	m_pHelp = new CBasicButton;
	m_pHelp->CreateSub ( this, "CHARACTER_WINDOW_STAT_QUESTION_BUTTON" , UI_FLAG_DEFAULT, CHARACTER_STAT_HELP );
	m_pHelp->CreateFlip ( "CHARACTER_WINDOW_STAT_QUESTION_BUTTON_OVER", CBasicButton::MOUSEIN_FLIP );
	m_pHelp->SetVisibleSingle ( TRUE );
	RegisterControl ( m_pHelp );
}

void CCharacterWindowCharStat::SetData( const GLCHARLOGIC& sCharData )
{
	m_pPowText->ClearText();
	m_pDexText->ClearText();
	m_pSpiText->ClearText();
	m_pStrText->ClearText();
	m_pStaText->ClearText();
	m_pPowText_Up->ClearText();
	m_pDexText_Up->ClearText();
	m_pSpiText_Up->ClearText();
	m_pStrText_Up->ClearText();
	m_pStaText_Up->ClearText();
	m_pAttackMinText->ClearText();
	m_pAttackMaxText->ClearText();
	m_pDefenseText->ClearText();
	m_pFightText->ClearText();
	m_pShootText->ClearText();
	m_pEnergyText->ClearText();
	m_pHitText->ClearText();
	m_pAvoidText->ClearText();
	m_pHPText->ClearText();
	m_pMPText->ClearText();
	m_pSPText->ClearText();
/*	m_pFireText->ClearText();
	m_pColdText->ClearText();
	m_pElectricText->ClearText();
	m_pPoisonText->ClearText();
	m_pSpiritText->ClearText();*/

	SCHARSTATS sADD = sCharData.GETSTATS_ADD();
	SCHARSTATS sITEM = sCharData.GETSTATS_ITEM();

	CString strCombine;
	strCombine.Format ( "%d", sCharData.m_sSUMSTATS.dwPow + m_dwPowAdd );
	m_pPowText->AddText ( strCombine, NS_UITEXTCOLOR::PALETURQUOISE );
	if ( sADD.dwPow )
	{
		strCombine.Format ( "(%d)", sADD.dwPow );
		D3DCOLOR dwColor = NS_UITEXTCOLOR::GREEN;
		if ( sITEM.dwPow ) dwColor = NS_UITEXTCOLOR::GREEN;
		m_pPowText_Up->AddText ( strCombine, dwColor );	
	}

	strCombine.Format ( "%d", sCharData.m_sSUMSTATS.dwDex + m_dwDexAdd );
	m_pDexText->AddText ( strCombine, NS_UITEXTCOLOR::PALETURQUOISE );
	if ( sADD.dwDex )
	{
		strCombine.Format ( "(%d)", sADD.dwDex );
		D3DCOLOR dwColor = NS_UITEXTCOLOR::GREEN;
		if ( sITEM.dwDex ) dwColor = NS_UITEXTCOLOR::GREEN;
		m_pDexText_Up->AddText ( strCombine, dwColor );
	}

	strCombine.Format ( "%d", sCharData.m_sSUMSTATS.dwSpi + m_dwSpiAdd );
	m_pSpiText->AddText ( strCombine, NS_UITEXTCOLOR::PALETURQUOISE );
	if ( sADD.dwSpi ) 
	{
		strCombine.Format ( "(%d)", sADD.dwSpi );
		D3DCOLOR dwColor = NS_UITEXTCOLOR::GREEN;
		if ( sITEM.dwSpi ) dwColor = NS_UITEXTCOLOR::GREEN;
		m_pSpiText_Up->AddText ( strCombine, dwColor );
	}

	strCombine.Format ( "%d", sCharData.m_sSUMSTATS.dwStr + m_dwStrAdd );
	m_pStrText->AddText ( strCombine, NS_UITEXTCOLOR::PALETURQUOISE );
	if ( sADD.dwStr )
	{
		strCombine.Format ( "(%d)", sADD.dwStr );
		D3DCOLOR dwColor = NS_UITEXTCOLOR::GREEN;
		if ( sITEM.dwStr ) dwColor = NS_UITEXTCOLOR::GREEN;
		m_pStrText_Up->AddText ( strCombine, dwColor );
	}

	strCombine.Format ( "%d", sCharData.m_sSUMSTATS.dwSta + m_dwStaAdd );
	m_pStaText->AddText ( strCombine, NS_UITEXTCOLOR::PALETURQUOISE );
	if ( sADD.dwSta )
	{
		strCombine.Format ( "(%d)", sADD.dwSta );
		D3DCOLOR dwColor = NS_UITEXTCOLOR::GREEN;
		if ( sITEM.dwSta ) dwColor = NS_UITEXTCOLOR::GREEN;
		m_pStaText_Up->AddText ( strCombine, dwColor );
	}

	strCombine.Format ( "%d", m_sCharLogic.m_wStatsPoint );
	if ( m_pPointText ) m_pPointText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::GOLD );

	strCombine.Format ( "%d", sCharData.GETFORCE_LOW () );
	if ( m_pAttackMinText ) m_pAttackMinText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::ORANGE );

	strCombine.Format ( "%d", sCharData.GETFORCE_HIGH () );
	if ( m_pAttackMaxText ) m_pAttackMaxText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::ORANGE );

	strCombine.Format ( "%d", sCharData.GETDEFENSE () );
	if ( m_pDefenseText ) m_pDefenseText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::ORANGE );

	strCombine.Format ( "%d", sCharData.m_wSUM_PA );
	if ( m_pFightText ) m_pFightText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::ORANGE );

	strCombine.Format ( "%d", sCharData.m_wSUM_SA );
	if ( m_pShootText ) m_pShootText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::ORANGE );

	strCombine.Format ( "%d", sCharData.m_wSUM_MA );
	if ( m_pEnergyText ) m_pEnergyText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::ORANGE );

	strCombine.Format ( "%d", sCharData.GETHIT () );
	if ( m_pHitText ) m_pHitText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::ORANGE );

	strCombine.Format ( "%d", sCharData.GETAVOID () );
	if ( m_pAvoidText ) m_pAvoidText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::ORANGE );

	strCombine.Format ( "%d", sCharData.GETHP () );
	if ( m_pHPText ) m_pHPText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::ORANGE );

	strCombine.Format ( "%d", sCharData.m_sMP.dwNow );
	if ( m_pMPText ) m_pMPText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::ORANGE );

	strCombine.Format ( "%d", sCharData.m_sSP.dwNow );
	if ( m_pSPText ) m_pSPText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::ORANGE );

/*	strCombine.Format ( "%d", sCharData.m_sSUMRESIST_SKILL.nFire );
	if ( m_pFireText ) m_pFireText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::SILVER );

	strCombine.Format ( "%d", sCharData.m_sSUMRESIST_SKILL.nIce );
	if ( m_pColdText ) m_pColdText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::SILVER );

	strCombine.Format ( "%d", sCharData.m_sSUMRESIST_SKILL.nElectric );
	if ( m_pElectricText ) m_pElectricText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::SILVER );

	strCombine.Format ( "%d", sCharData.m_sSUMRESIST_SKILL.nPoison );
	if ( m_pPoisonText ) m_pPoisonText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::SILVER );

	strCombine.Format ( "%d", sCharData.m_sSUMRESIST_SKILL.nSpirit );
	if ( m_pSpiritText ) m_pSpiritText->SetOneLineText ( strCombine, NS_UITEXTCOLOR::SILVER );*/
}

void CCharacterWindowCharStat::SetUpData( const GLCHARLOGIC& sCharData )
{
	//1st version of stats comparison
	//compare stats from character data
	m_pAttackMinMarkStatic->ClearText();
	m_pAttackMaxMarkStatic->ClearText();
	m_pDefenseMarkStatic->ClearText();
	m_pFightMarkStatic->ClearText();
	m_pShootMarkStatic->ClearText();
	m_pEnergyMarkStatic->ClearText();
	m_pHitMarkStatic->ClearText();
	m_pAvoidMarkStatic->ClearText();
	m_pHPMarkStatic->ClearText();
	m_pMPMarkStatic->ClearText();
	m_pSPMarkStatic->ClearText();
	m_pAttackMinMarkValue->ClearText();
	m_pAttackMaxMarkValue->ClearText();
	m_pDefenseMarkValue->ClearText();
	m_pFightMarkValue->ClearText();
	m_pShootMarkValue->ClearText();
	m_pEnergyMarkValue->ClearText();
	m_pHitMarkValue->ClearText();
	m_pAvoidMarkValue->ClearText();
	m_pHPMarkValue->ClearText();
	m_pMPMarkValue->ClearText();
	m_pSPMarkValue->ClearText();

	m_pAttackMinMark->SetVisibleSingle ( FALSE );
	m_pAttackMaxMark->SetVisibleSingle ( FALSE );
	m_pDefenseMark->SetVisibleSingle ( FALSE );
	m_pFightMark->SetVisibleSingle ( FALSE );
	m_pShootMark->SetVisibleSingle ( FALSE );
	m_pEnergyMark->SetVisibleSingle ( FALSE );
	m_pHitMark->SetVisibleSingle ( FALSE );
	m_pAvoidMark->SetVisibleSingle ( FALSE );
	m_pHPMark->SetVisibleSingle ( FALSE );
	m_pMPMark->SetVisibleSingle ( FALSE );
	m_pSPMark->SetVisibleSingle ( FALSE );

	int nAttackMin = m_sCharLogic.GETFORCE_LOW() - sCharData.GETFORCE_LOW ();
	int nAttackMax = m_sCharLogic.GETFORCE_HIGH() - sCharData.GETFORCE_HIGH();
	int nDefense = m_sCharLogic.GETDEFENSE() - sCharData.GETDEFENSE();
	int nFight = m_sCharLogic.m_wSUM_PA - sCharData.m_wSUM_PA;
	int nShoot = m_sCharLogic.m_wSUM_SA - sCharData.m_wSUM_SA;
	int nEnergy = m_sCharLogic.m_wSUM_MA - sCharData.m_wSUM_MA;
	int nHit = m_sCharLogic.GETHIT() - sCharData.GETHIT();
	int nAvoid = m_sCharLogic.GETAVOID() - sCharData.GETAVOID();
	int nHP = m_sCharLogic.m_sHP.dwMax - sCharData.m_sHP.dwMax;
	int nMP = m_sCharLogic.m_sMP.dwMax - sCharData.m_sMP.dwMax;
	int nSP = m_sCharLogic.m_sSP.dwMax - sCharData.m_sSP.dwMax;

	CString strCombine;
	strCombine.Format ( "%s", "-" );
	if ( nAttackMin == 0 )	
	{
		m_pAttackMinMarkStatic->AddText ( strCombine, NS_UITEXTCOLOR::SILVER );
	}else{
		m_pAttackMinMark->SetVisibleSingle ( TRUE );
		m_pAttackMinMark->CheckValue( nAttackMin );
	}

	if ( nAttackMax == 0 ) 
	{
		m_pAttackMaxMarkStatic->AddText ( strCombine, NS_UITEXTCOLOR::SILVER );
	}else{
		m_pAttackMaxMark->SetVisibleSingle ( TRUE );
		m_pAttackMaxMark->CheckValue( nAttackMax );
	}

	if ( nDefense == 0 )	
	{
		m_pDefenseMarkStatic->AddText ( strCombine, NS_UITEXTCOLOR::SILVER );
	}else{
		m_pDefenseMark->SetVisibleSingle ( TRUE );
		m_pDefenseMark->CheckValue( nDefense );
	}

	if ( nFight == 0 )		
	{
		m_pFightMarkStatic->AddText ( strCombine, NS_UITEXTCOLOR::SILVER );
	}else{
		m_pFightMark->SetVisibleSingle ( TRUE );
		m_pFightMark->CheckValue( nFight );
	}

	if ( nShoot == 0 )		
	{
		m_pShootMarkStatic->AddText ( strCombine, NS_UITEXTCOLOR::SILVER );
	}else{
		m_pShootMark->SetVisibleSingle ( TRUE );
		m_pShootMark->CheckValue( nShoot );
	}

	if ( nEnergy == 0 )	
	{
		m_pEnergyMarkStatic->AddText ( strCombine, NS_UITEXTCOLOR::SILVER );
	}else{
		m_pEnergyMark->SetVisibleSingle ( TRUE );
		m_pEnergyMark->CheckValue( nEnergy );
	}

	if ( nHit == 0 )		
	{
		m_pHitMarkStatic->AddText ( strCombine, NS_UITEXTCOLOR::SILVER );
	}else{
		m_pHitMark->SetVisibleSingle ( TRUE );
		m_pHitMark->CheckValue( nHit );
	}

	if ( nAvoid == 0 )		
	{
		m_pAvoidMarkStatic->AddText ( strCombine, NS_UITEXTCOLOR::SILVER );
	}else{
		m_pAvoidMark->SetVisibleSingle ( TRUE );
		m_pAvoidMark->CheckValue( nAvoid );
	}

	if ( nHP == 0 )		
	{
		m_pHPMarkStatic->AddText ( strCombine, NS_UITEXTCOLOR::SILVER );
	}else{
		m_pHPMark->SetVisibleSingle ( TRUE );
		m_pHPMark->CheckValue( nHP );
	}

	if ( nMP == 0 )		
	{
		m_pMPMarkStatic->AddText ( strCombine, NS_UITEXTCOLOR::SILVER );
	}else{
		m_pMPMark->SetVisibleSingle ( TRUE );
		m_pMPMark->CheckValue( nMP );
	}

	if ( nSP == 0 )		
	{
		m_pSPMarkStatic->AddText ( strCombine, NS_UITEXTCOLOR::SILVER );
	}else{
		m_pSPMark->SetVisibleSingle ( TRUE );
		m_pSPMark->CheckValue( nSP );
	}

	strCombine.Format ( "%d", nAttackMin );
	if ( m_pAttackMinMarkValue ) m_pAttackMinMarkValue->SetOneLineText ( strCombine, GetTextColor( nAttackMin ) );
	strCombine.Format ( "%d", nAttackMax );
	if ( m_pAttackMaxMarkValue ) m_pAttackMaxMarkValue->SetOneLineText ( strCombine, GetTextColor( nAttackMax ) );
	strCombine.Format ( "%d", nDefense );
	if ( m_pDefenseMarkValue ) m_pDefenseMarkValue->SetOneLineText ( strCombine, GetTextColor( nDefense ) );
	strCombine.Format ( "%d", nFight );
	if ( m_pFightMarkValue ) m_pFightMarkValue->SetOneLineText ( strCombine, GetTextColor( nFight ) );
	strCombine.Format ( "%d", nShoot );
	if ( m_pShootMarkValue ) m_pShootMarkValue->SetOneLineText ( strCombine, GetTextColor( nShoot ) );
	strCombine.Format ( "%d", nEnergy );
	if ( m_pEnergyMarkValue ) m_pEnergyMarkValue->SetOneLineText ( strCombine, GetTextColor( nEnergy) );
	strCombine.Format ( "%d", nHit );
	if ( m_pHitMarkValue ) m_pHitMarkValue->SetOneLineText ( strCombine, GetTextColor( nHit ) );
	strCombine.Format ( "%d", nAvoid );
	if ( m_pAvoidMarkValue ) m_pAvoidMarkValue->SetOneLineText ( strCombine, GetTextColor( nAvoid ) );
	strCombine.Format ( "%d", nHP );
	if ( m_pHPMarkValue ) m_pHPMarkValue->SetOneLineText ( strCombine, GetTextColor( nHP ) );
	strCombine.Format ( "%d", nMP );
	if ( m_pMPMarkValue ) m_pMPMarkValue->SetOneLineText ( strCombine, GetTextColor( nMP ) );
	strCombine.Format ( "%d", nSP );
	if ( m_pSPMarkValue ) m_pSPMarkValue->SetOneLineText ( strCombine, GetTextColor( nSP ) );
}

void CCharacterWindowCharStat::SetPointControl ()
{
	BOOL bPointUsable = m_sCharLogic.m_wStatsPoint > 0;
	BOOL bResetOrApply = !( m_dwPowAdd == 0 && m_dwDexAdd == 0 && m_dwSpiAdd == 0 && m_dwStrAdd == 0 && m_dwStaAdd == 0 );

	DWORD dwPowCheck = m_sCharLogic.GETCHARDATA().m_sStats.dwPow + m_dwPowAdd;
	DWORD dwStrCheck = m_sCharLogic.GETCHARDATA().m_sStats.dwStr + m_dwStrAdd;
	DWORD dwSpiCheck = m_sCharLogic.GETCHARDATA().m_sStats.dwSpi + m_dwSpiAdd;
	DWORD dwDexCheck = m_sCharLogic.GETCHARDATA().m_sStats.dwDex + m_dwDexAdd;
	DWORD dwStaCheck = m_sCharLogic.GETCHARDATA().m_sStats.dwSta + m_dwStaAdd;

	BOOL bMaxPow = dwPowCheck >= GLCONST_CHAR::dwPowMax;
	BOOL bMaxStr = dwStrCheck >= GLCONST_CHAR::dwStrMax;
	BOOL bMaxSpi = dwSpiCheck >= GLCONST_CHAR::dwSpiMax;
	BOOL bMaxDex = dwDexCheck >= GLCONST_CHAR::dwDexMax;
	BOOL bMaxSta = dwStaCheck >= GLCONST_CHAR::dwStaMax;
	
	if ( m_pPowButton_Plus ) m_pPowButton_Plus->SetVisibleSingle ( bPointUsable && !bMaxPow );
	if ( m_pDexButton_Plus ) m_pDexButton_Plus->SetVisibleSingle ( bPointUsable && !bMaxDex );
	if ( m_pSpiButton_Plus ) m_pSpiButton_Plus->SetVisibleSingle ( bPointUsable && !bMaxSpi );
	if ( m_pStrButton_Plus ) m_pStrButton_Plus->SetVisibleSingle ( bPointUsable && !bMaxStr );
	if ( m_pStaButton_Plus ) m_pStaButton_Plus->SetVisibleSingle ( bPointUsable && !bMaxSta );

	if ( m_pPowButton_Minus ) m_pPowButton_Minus->SetVisibleSingle ( m_dwPowAdd !=0 );
	if ( m_pDexButton_Minus ) m_pDexButton_Minus->SetVisibleSingle ( m_dwDexAdd !=0 );
	if ( m_pSpiButton_Minus ) m_pSpiButton_Minus->SetVisibleSingle ( m_dwSpiAdd !=0 );
	if ( m_pStrButton_Minus ) m_pStrButton_Minus->SetVisibleSingle ( m_dwStrAdd !=0 );
	if ( m_pStaButton_Minus ) m_pStaButton_Minus->SetVisibleSingle ( m_dwStaAdd !=0 );

	if ( m_pButtonStatReset ) m_pButtonStatReset->SetVisibleSingle ( bResetOrApply );
	if ( m_pButtonStatApply ) m_pButtonStatApply->SetVisibleSingle ( bResetOrApply );
}

void CCharacterWindowCharStat::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case CHARACTER_POW_BUTTON_PLUS:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( m_pPowText && !m_pPowText->empty() )
				{
					m_pPowText->SetUseTextColor ( 0, TRUE );
					m_pPowText->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD );
				}
				
				if ( m_pPowText_Up && !m_pPowText_Up->empty() )
				{
					m_pPowText_Up->SetUseTextColor ( 0, TRUE );
					m_pPowText_Up->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD );
				}

				if ( m_pPowTextStatic && !m_pPowTextStatic->empty() )
				{
					m_pPowTextStatic->SetUseTextColor ( 0, TRUE );
					m_pPowTextStatic->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD );
				}

				if ( CHECK_LB_UP_LIKE ( dwMsg ) )
				{
					m_dwPowAdd++;
					m_dwStatsDeduct++;
				}
			}
		}break;

	case CHARACTER_DEX_BUTTON_PLUS:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( m_pDexText && !m_pDexText->empty() )
				{
					m_pDexText->SetUseTextColor ( 0, TRUE );
					m_pDexText->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD  );
				}

				if ( m_pDexText_Up && !m_pDexText_Up->empty() )
				{
					m_pDexText_Up->SetUseTextColor ( 0, TRUE );
					m_pDexText_Up->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD  );
				}

				if ( m_pDexTextStatic && !m_pDexTextStatic->empty() )
				{
					m_pDexTextStatic->SetUseTextColor ( 0, TRUE );
					m_pDexTextStatic->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD  );
				}

				if ( CHECK_LB_UP_LIKE ( dwMsg ) )	
				{
					m_dwDexAdd++;
					m_dwStatsDeduct++;
				}
			}
		}break;

	case CHARACTER_SPI_BUTTON_PLUS:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( m_pSpiText && !m_pSpiText->empty() )
				{
					m_pSpiText->SetUseTextColor ( 0, TRUE );
					m_pSpiText->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD  );
				}

				if ( m_pSpiText_Up && !m_pSpiText_Up->empty() )
				{
					m_pSpiText_Up->SetUseTextColor ( 0, TRUE );
					m_pSpiText_Up->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD  );
				}

				if ( m_pSpiTextStatic && !m_pSpiTextStatic->empty() )
				{
					m_pSpiTextStatic->SetUseTextColor ( 0, TRUE );
					m_pSpiTextStatic->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD  );
				}

				if ( CHECK_LB_UP_LIKE ( dwMsg ) )	
				{
					m_dwSpiAdd++;
					m_dwStatsDeduct++;
				}
			}
		}break;

	case CHARACTER_STR_BUTTON_PLUS:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( m_pStrText && !m_pStrText->empty() )
				{
					m_pStrText->SetUseTextColor ( 0, TRUE );
					m_pStrText->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD  );
				}

				if ( m_pStrText_Up && !m_pStrText_Up->empty() )
				{
					m_pStrText_Up->SetUseTextColor ( 0, TRUE );
					m_pStrText_Up->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD  );
				}

				if ( m_pStrTextStatic && !m_pStrTextStatic->empty() )
				{
					m_pStrTextStatic->SetUseTextColor ( 0, TRUE );
					m_pStrTextStatic->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD  );
				}

				if ( CHECK_LB_UP_LIKE ( dwMsg ) )
				{
					m_dwStrAdd++;
					m_dwStatsDeduct++;
				}
			}
		}break;

	case CHARACTER_STA_BUTTON_PLUS:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( m_pStaText && !m_pStaText->empty() )
				{
					m_pStaText->SetUseTextColor ( 0, TRUE );
					m_pStaText->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD  );
				}

				if ( m_pStaText_Up && !m_pStaText_Up->empty() )
				{
					m_pStaText_Up->SetUseTextColor ( 0, TRUE );
					m_pStaText_Up->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD  );
				}

				if ( m_pStaTextStatic && !m_pStaTextStatic->empty() )
				{
					m_pStaTextStatic->SetUseTextColor ( 0, TRUE );
					m_pStaTextStatic->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD  );
				}

				if ( CHECK_LB_UP_LIKE ( dwMsg ) )	
				{
					m_dwStaAdd++;
					m_dwStatsDeduct++;
				}
			}
		}break;

	case CHARACTER_POW_BUTTON_MINUS:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( m_pPowText && !m_pPowText->empty() )
				{
					m_pPowText->SetUseTextColor ( 0, TRUE );
					m_pPowText->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD );
				}

				if ( m_pPowText_Up && !m_pPowText_Up->empty() )
				{
					m_pPowText_Up->SetUseTextColor ( 0, TRUE );
					m_pPowText_Up->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD );
				}

				if ( m_pPowTextStatic && !m_pPowTextStatic->empty() )
				{
					m_pPowTextStatic->SetUseTextColor ( 0, TRUE );
					m_pPowTextStatic->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD );
				}

				if ( CHECK_LB_UP_LIKE ( dwMsg ) )
				{
					m_dwPowAdd--;
					m_dwStatsDeduct--;
				}
			}
		}break;

	case CHARACTER_DEX_BUTTON_MINUS:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( m_pDexText && !m_pDexText->empty() )
				{
					m_pDexText->SetUseTextColor ( 0, TRUE );
					m_pDexText->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD  );
				}

				if ( m_pDexText_Up && !m_pDexText_Up->empty() )
				{
					m_pDexText_Up->SetUseTextColor ( 0, TRUE );
					m_pDexText_Up->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD  );
				}

				if ( m_pDexTextStatic && !m_pDexTextStatic->empty() )
				{
					m_pDexTextStatic->SetUseTextColor ( 0, TRUE );
					m_pDexTextStatic->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD  );
				}

				if ( CHECK_LB_UP_LIKE ( dwMsg ) )	
				{
					m_dwDexAdd--;
					m_dwStatsDeduct--;
				}
			}
		}break;

	case CHARACTER_SPI_BUTTON_MINUS:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( m_pSpiText && !m_pSpiText->empty() )
				{
					m_pSpiText->SetUseTextColor ( 0, TRUE );
					m_pSpiText->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD  );
				}

				if ( m_pSpiText_Up && !m_pSpiText_Up->empty() )
				{
					m_pSpiText_Up->SetUseTextColor ( 0, TRUE );
					m_pSpiText_Up->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD  );
				}

				if ( m_pSpiTextStatic && !m_pSpiTextStatic->empty() )
				{
					m_pSpiTextStatic->SetUseTextColor ( 0, TRUE );
					m_pSpiTextStatic->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD  );
				}

				if ( CHECK_LB_UP_LIKE ( dwMsg ) )	
				{
					m_dwSpiAdd--;
					m_dwStatsDeduct--;
				}
			}
		}break;

	case CHARACTER_STR_BUTTON_MINUS:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( m_pStrText && !m_pStrText->empty() )
				{
					m_pStrText->SetUseTextColor ( 0, TRUE );
					m_pStrText->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD  );
				}

				if ( m_pStrText_Up && !m_pStrText_Up->empty() )
				{
					m_pStrText_Up->SetUseTextColor ( 0, TRUE );
					m_pStrText_Up->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD  );
				}

				if ( m_pStrTextStatic && !m_pStrTextStatic->empty() )
				{
					m_pStrTextStatic->SetUseTextColor ( 0, TRUE );
					m_pStrTextStatic->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD  );
				}

				if ( CHECK_LB_UP_LIKE ( dwMsg ) )
				{
					m_dwStrAdd--;
					m_dwStatsDeduct--;
				}
			}
		}break;

	case CHARACTER_STA_BUTTON_MINUS:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( m_pStaText && !m_pStaText->empty() )
				{
					m_pStaText->SetUseTextColor ( 0, TRUE );
					m_pStaText->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD  );
				}

				if ( m_pStaText_Up && !m_pStaText_Up->empty() )
				{
					m_pStaText_Up->SetUseTextColor ( 0, TRUE );
					m_pStaText_Up->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD  );
				}

				if ( m_pStaTextStatic && !m_pStaTextStatic->empty() )
				{
					m_pStaTextStatic->SetUseTextColor ( 0, TRUE );
					m_pStaTextStatic->SetTextColor ( 0, NS_UITEXTCOLOR::GOLD  );
				}

				if ( CHECK_LB_UP_LIKE ( dwMsg ) )	
				{
					m_dwStaAdd--;
					m_dwStatsDeduct--;
				}
			}
		}break;

	case CHARACTER_STAT_RESET:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( CHECK_LB_UP_LIKE ( dwMsg ) )				
				{
					m_dwPowAdd = 0;
					m_dwDexAdd = 0;
					m_dwSpiAdd = 0;
					m_dwStrAdd = 0;
					m_dwStaAdd = 0;
					m_dwStatsDeduct = 0;
				}
			}
		}break;

	case CHARACTER_STAT_APPLY:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( CHECK_LB_UP_LIKE ( dwMsg ) )				
				{
					SCHARSTATS sStats;
					sStats.dwPow = m_dwPowAdd;
					sStats.dwDex = m_dwDexAdd;
					sStats.dwSpi = m_dwSpiAdd;
					sStats.dwStr = m_dwStrAdd;
					sStats.dwSta = m_dwStaAdd;
					GLGaeaClient::GetInstance().GetCharacter()->ReqStatsUp( sStats );

					m_dwPowAdd = 0;
					m_dwDexAdd = 0;
					m_dwSpiAdd = 0;
					m_dwStrAdd = 0;
					m_dwStaAdd = 0;
					m_dwStatsDeduct = 0;
				}
			}
		}break;


	case CHARACTER_STAT_HELP:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO_SPLIT(  ID2GAMEINTEXT( "CHARACTER_WINDOW_POINT_QUESTION_BUTTON" ) , NS_UITEXTCOLOR::DARKLBUE );
			}
		}break;

	case MOUSE_OVER_STAT_1_POW:
	case MOUSE_OVER_STAT_1_DEX:
	case MOUSE_OVER_STAT_1_SPI:
	case MOUSE_OVER_STAT_1_STR:
	case MOUSE_OVER_STAT_1_STA:
	case MOUSE_OVER_STAT_2_ATTACK_MIN:
	case MOUSE_OVER_STAT_2_ATTACK_MAX:
	case MOUSE_OVER_STAT_2_DEFENSE:
	case MOUSE_OVER_STAT_2_FIGHT:
	case MOUSE_OVER_STAT_2_SHOOT:
	case MOUSE_OVER_STAT_2_STAMINA:
	case MOUSE_OVER_STAT_2_HIT:
	case MOUSE_OVER_STAT_2_AVOID:
	case MOUSE_OVER_STAT_2_HP:
	case MOUSE_OVER_STAT_2_MP:
	case MOUSE_OVER_STAT_2_SP:
/*	case MOUSE_OVER_STAT_3_FIRE:
	case MOUSE_OVER_STAT_3_COLD:
	case MOUSE_OVER_STAT_3_ELECTRIC:
	case MOUSE_OVER_STAT_3_POISON:
	case MOUSE_OVER_STAT_3_SPIRIT:*/
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				int nSelect = ControlID - MOUSE_OVER_STAT_1_POW;
				ShowTip( nSelect );
			}
		}break;
	}
}

void CCharacterWindowCharStat::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{	
	const GLCHARLOGIC& sCharData = GLGaeaClient::GetInstance().GetCharacterLogic ();	

	InitData();
	SetData ( sCharData );
	SetUpData ( sCharData );

	SetPointControl ();

	if( m_pPowText && !m_pPowText->empty() )	m_pPowText->SetUseTextColor ( 0, FALSE );
	if( m_pPowText_Up && !m_pPowText_Up->empty() )	m_pPowText_Up->SetUseTextColor ( 0, FALSE );
	if( m_pPowTextStatic && !m_pPowTextStatic->empty() )	m_pPowTextStatic->SetUseTextColor ( 0, FALSE );
	if( m_pDexText && !m_pDexText->empty() )	m_pDexText->SetUseTextColor ( 0, FALSE );
	if( m_pDexText_Up && !m_pDexText_Up->empty() )	m_pDexText_Up->SetUseTextColor ( 0, FALSE );
	if( m_pDexTextStatic && !m_pDexTextStatic->empty() )	m_pDexTextStatic->SetUseTextColor ( 0, FALSE );
	if( m_pSpiText && !m_pSpiText->empty() )	m_pSpiText->SetUseTextColor ( 0, FALSE );
	if( m_pSpiText_Up && !m_pSpiText_Up->empty() )	m_pSpiText_Up->SetUseTextColor ( 0, FALSE );
	if( m_pSpiTextStatic && !m_pSpiTextStatic->empty() )	m_pSpiTextStatic->SetUseTextColor ( 0, FALSE );
	if( m_pStrText && !m_pStrText->empty() )	m_pStrText->SetUseTextColor ( 0, FALSE );
	if( m_pStrText_Up && !m_pStrText_Up->empty() )	m_pStrText_Up->SetUseTextColor ( 0, FALSE );
	if( m_pStrTextStatic && !m_pStrTextStatic->empty() )	m_pStrTextStatic->SetUseTextColor ( 0, FALSE );
	if( m_pStaText && !m_pStaText->empty() )	m_pStaText->SetUseTextColor ( 0, FALSE );
	if( m_pStaText_Up && !m_pStaText_Up->empty() )	m_pStaText_Up->SetUseTextColor ( 0, FALSE );
	if( m_pStaTextStatic && !m_pStaTextStatic->empty() )	m_pStaTextStatic->SetUseTextColor ( 0, FALSE );

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void CCharacterWindowCharStat::SetVisibleSingle ( BOOL bVisible )
{
	CUIGroup::SetVisibleSingle( bVisible );

	if( bVisible )
	{
	}
}

void CCharacterWindowCharStat::ShowTip( int nSelect )
{
	if ( nSelect < 0 || nSelect >= MOUSE_OVER_BUTTONS )
		return;

	std::string strTip[MOUSE_OVER_BUTTONS] = 
	{
		"CHARACTER_WINDOW_POW_TOOLTIP",
		"CHARACTER_WINDOW_DEX_TOOLTIP",
		"CHARACTER_WINDOW_SPI_TOOLTIP",
		"CHARACTER_WINDOW_STR_TOOLTIP",
		"CHARACTER_WINDOW_STA_TOOLTIP",
		"CHARACTER_WINDOW_ATTACK_MIN_TOOLTIP",
		"CHARACTER_WINDOW_ATTACK_MAX_TOOLTIP",
		"CHARACTER_WINDOW_DEFENSE_TOOLTIP",
		"CHARACTER_WINDOW_FIGHT_TOOLTIP",
		"CHARACTER_WINDOW_SHOOT_TOOLTIP",
		"CHARACTER_WINDOW_STAMINA_TOOLTIP",
		"CHARACTER_WINDOW_HIT_TOOLTIP",
		"CHARACTER_WINDOW_AVOID_TOOLTIP",
		"CHARACTER_WINDOW_HP_TOOLTIP",
		"CHARACTER_WINDOW_MP_TOOLTIP",
		"CHARACTER_WINDOW_SP_TOOLTIP",
/*		"CHARACTER_WINDOW_FIRE_RESIST_TOOLTIP",
		"CHARACTER_WINDOW_COLD_RESIST_TOOLTIP",
		"CHARACTER_WINDOW_ELECTRIC_RESIST_TOOLTIP",
		"CHARACTER_WINDOW_POISON_RESIST_TOOLTIP",
		"CHARACTER_WINDOW_SPIRIT_RESIST_TOOLTIP",*/
	};

	CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO(  ID2GAMEINTEXT( strTip[ nSelect ].c_str() ) , NS_UITEXTCOLOR::GOLD );
}

void CCharacterWindowCharStat::InitData()
{
	GLCharacter* pChar = GLGaeaClient::GetInstance().GetCharacter();
	if ( pChar )
	{
		m_sCharLogic.INIT_NEW_CHAR( 
			CharClassToIndex( pChar->m_emClass ),
			pChar->GetUserID(),
			pChar->m_dwServerID,
			pChar->m_szName,
			pChar->m_wSchool,
			pChar->m_wHair,
			pChar->m_wFace,
			pChar->m_wHairColor,
			pChar->m_wSex );

		m_sCharLogic.Assign( pChar->GetCharData() );

		m_sCharLogic.GETCHARDATA().m_sStats.dwPow += m_dwPowAdd;
		m_sCharLogic.GETCHARDATA().m_sStats.dwDex += m_dwDexAdd;
		m_sCharLogic.GETCHARDATA().m_sStats.dwSpi += m_dwSpiAdd;
		m_sCharLogic.GETCHARDATA().m_sStats.dwSta += m_dwStaAdd;
		m_sCharLogic.GETCHARDATA().m_sStats.dwStr += m_dwStrAdd;
		m_sCharLogic.GETCHARDATA().m_wStatsPoint -=m_dwStatsDeduct;

		m_sCharLogic.SetUseArmSub( pChar->IsUseArmSub() );

		m_sCharLogic.m_bVehicle = pChar->m_bVehicle;

		SITEMCUSTOM sItemCheck = CInnerInterface::GetInstance().GetItemInfoTemp();
		
		SITEM* pItem = GLItemMan::GetInstance().GetItem( sItemCheck.sNativeID );
		if ( pItem && pItem->sBasicOp.emItemType == ITEM_SUIT )
		{
			EMSLOT emSlot;
			switch ( pItem->sSuitOp.emSuit )
			{
			case SUIT_HEADGEAR:		
				emSlot = SLOT_HEADGEAR;	
				break;
			case SUIT_UPPER:		
				emSlot = SLOT_UPPER;	
				break;
			case SUIT_LOWER:		
				emSlot = SLOT_LOWER;	
				break;
			case SUIT_HAND:			
				emSlot = SLOT_HAND;	
				break;
			case SUIT_FOOT:			
				emSlot = SLOT_FOOT;	
				break;
			case SUIT_HANDHELD:		
				{
					if( pChar->IsUseArmSub() )	emSlot = SLOT_RHAND_S;
					else	emSlot = SLOT_RHAND;
				}break;
			case SUIT_NECK:				
				emSlot = SLOT_NECK;	
				break;
			case SUIT_WRIST:			
				emSlot = SLOT_WRIST;	
				break;
			case SUIT_FINGER:		
				{
					if ( pChar->GetCharLogic().VALID_SLOT_ITEM( SLOT_RFINGER ) )	emSlot = SLOT_LFINGER;
					else	emSlot = SLOT_RFINGER;
				}break;
			case SUIT_VEHICLE:			
				emSlot = SLOT_VEHICLE;	
				break;

			case SUIT_EARRING:		
                emSlot = SLOT_LEARRING;	
				break;
			case SUIT_ORNAMENT:		
                emSlot = SLOT_REARRING;	
				break;
			case SUIT_ACCESSORIES:		
				{
					if ( pChar->GetCharLogic().VALID_SLOT_ITEM( SLOT_RACCESSORY ) )	emSlot = SLOT_LACCESSORY;
					else	emSlot = SLOT_RACCESSORY;
				}break;

			case SUIT_WAISTBAND:			
				emSlot = SLOT_WAIST;	
				break;

			default:				
				emSlot = SLOT_NSIZE_S_2;	
				break;
			};

			if ( pItem->sBasicOp.emItemType == ITEM_ARROW ||
				pItem->sBasicOp.emItemType == ITEM_CHARM ||
				pItem->sBasicOp.emItemType == ITEM_BULLET )	
			{
				if( pChar->IsUseArmSub() )	emSlot = SLOT_LHAND_S;
				else	emSlot = SLOT_LHAND;
			}

			if ( emSlot != SLOT_NSIZE_S_2 )
			{
				m_sCharLogic.m_PutOnItems[emSlot] = sItemCheck;
			}
		}

		for ( int i=0; i<SKILLFACT_SIZE; ++i )
		{
			m_sCharLogic.m_sSKILLFACT[i] = pChar->m_sSKILLFACT[i];
		}

		m_sCharLogic.m_sPETSKILLFACT = pChar->m_sPETSKILLFACT;

		m_sCharLogic.m_sQITEMFACT = pChar->m_sQITEMFACT;
		m_sCharLogic.m_sEVENTFACT = pChar->m_sEVENTFACT;

		m_sCharLogic.INIT_DATA( TRUE, FALSE );
		m_sCharLogic.UPDATE_DATA( 0.0f , 0.0f );
	}
}

DWORD CCharacterWindowCharStat::GetTextColor( int nVal )
{
	if ( nVal == 0 )	return NS_UITEXTCOLOR::WHITE;
	if ( nVal > 0 )		return NS_UITEXTCOLOR::BRIGHTGREEN;
	if ( nVal < 0 )		return NS_UITEXTCOLOR::RED;

	return NS_UITEXTCOLOR::WHITE;
};