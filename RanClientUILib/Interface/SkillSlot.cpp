#include "StdAfx.h"
#include "SkillSlot.h"
#include "GameTextControl.h"
#include "UITextControl.h"
#include "SkillImage.h"
#include "GLGaeaClient.h"
#include "InnerInterface.h"
#include "SkillWindowToTray.h"
#include "ModalWindow.h"
#include "ModalCallerID.h"
#include "BasicQuickSkillWindow.h"
#include "BasicQuickSkillWindowNew.h"

#include "SkillTrayTabNew.h"
#include "../EngineLib/DxCommon/DxFontMan.h"
#include "../EngineUILib/GUInterface/BasicButton.h"
#include "../EngineUILib/GUInterface/BasicTextBox.h"
#include "BasicLineBox.h"
#include "BasicLineBoxEx.h"
#include "SkillTrayTab.h"
#include "RANPARAM.h"
#include "../EngineUILib/GUInterface/UIKeyCheck.h"
#include "../EngineUiLib/GUInterface/BasicButtonText.h"
#include "./BasicTextButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSkillSlot::CSkillSlot () :
	m_pNotLearnImage ( NULL ),
	m_pSkillImage ( NULL ),
	//m_pSkillUpButton ( NULL ),
	m_pTextBoxLine1 ( NULL ),
	m_pTextBoxLine2 ( NULL ),
	m_sNativeID ( NATIVEID_NULL () ),
	m_bLearnSkill ( FALSE ),
	m_bCanLearn ( FALSE ),
	m_bLEVELUP_ENABLE ( FALSE ),
	m_bSKILLSLOT_MOUSE_IN ( FALSE ),
	m_wLevel ( 0 ),
	m_pButton_Plus(NULL),
	m_pButton_Minus(NULL),
	m_pButton_Black( NULL ),
	m_pButton_Black2( NULL ),
	nLevel(0)
{
}

CSkillSlot::~CSkillSlot ()
{
}

CBasicButtonText* CSkillSlot::CreateButtonBlue ( char* szButton, char* szTextBox, CD3DFontPar* pFont, int nAlign, UIGUID ControlID, CString strText )
{
	CBasicButtonText* pButton = new CBasicButtonText;
	pButton->CreateRanButtonBlue ( this, szButton, ControlID );
	pButton->CreateTextBox ( szTextBox, pFont, nAlign );
	pButton->SetOneLineText( strText.GetString() );
	RegisterControl ( pButton );

	return pButton;
}

/*CBasicTextButton*  CSkillSlot::CreateButtonBlue ( const char* szButton, UIGUID ControlID, const char* szText )
{
	const int nBUTTONSIZE = CBasicTextButton::SIZE16;
	CBasicTextButton* pTextButton = new CBasicTextButton;
	pTextButton->CreateSub ( this, "BASIC_TEXT_BUTTON16", UI_FLAG_XSIZE|UI_FLAG_YSIZE, ControlID );
	pTextButton->CreateBaseButton ( szButton, nBUTTONSIZE, CBasicButton::CLICK_FLIP, szText ,_DEFAULT_FONT_FLAG );
	RegisterControl ( pTextButton );
	return pTextButton;
}*/

void CSkillSlot::CreateSubControl ( SNATIVEID sNativeID )
{
	D3DCOLOR dwColor = NS_UITEXTCOLOR::WHITE;

	CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_EX_FLAG );
	int	nTextAlign = TEXT_ALIGN_LEFT;

	//CreateControl ( "RNSKILL_SKILL_SLOT_IMAGE" );
   // CreateControl ( "SKILL_SLOT_BACK_SKILL_TEXT_L" );	
	//CreateControl ( "SKILL_SLOT_BACK_SKILL_TEXT_M" );
	//CreateControl ( "SKILL_SLOT_BACK_SKILL_TEXT_R" );

	if ( sNativeID == NATIVEID_NULL () )
	{
//		GASSERT ( 0 && "???? ID ???. ??????." );
	}
	else
	{
		CreateControl ( "HINH_ANH_O_KYNANG" );

		{	// �en X�m �en Background
		CBasicLineBoxEx* pBasicLineBoxEx = new CBasicLineBoxEx;
		pBasicLineBoxEx->CreateSub ( this, "BASIC_LINE_BOX_EX_BODY_LOM", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBoxEx->CreateBaseBoxLomBox ( "KYNANG_KICHTHUOC_KHE" );
		RegisterControl ( pBasicLineBoxEx );
		}
/*		{
			CBasicLineBox* pLineBox = new CBasicLineBox;
			pLineBox->CreateSub ( this, "BASIC_LINE_BOX_SKILL_BACK", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
			pLineBox->CreateBaseBoxSkillBack ( "RNSKILL_SLOT_SIZE" );
			RegisterControl ( pLineBox );
		}*/
		{
			m_pSkillSlotAvtive = CreateControl ( "KYNANG_O_HOATDONG" );
			m_pSkillSlotAvtive->SetVisibleSingle( FALSE );
		}
		{
			m_pLineBoxGreen = new CBasicLineBoxEx;
			m_pLineBoxGreen->CreateSub ( this, "BASIC_LINE_BOX_EX_SKILL_GREEN", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
			m_pLineBoxGreen->CreateBaseBoxSkillGreen ( "RNSKILL_SLOT_GREEN" );
			m_pLineBoxGreen->SetVisibleSingle ( FALSE );
			RegisterControl ( m_pLineBoxGreen );

			m_pLineBoxBlue = new CBasicLineBoxEx;
			m_pLineBoxBlue->CreateSub ( this, "BASIC_LINE_BOX_EX_SKILL_BLUE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
			m_pLineBoxBlue->CreateBaseBoxSkillBlue ( "RNSKILL_SLOT_BLUE" );
			m_pLineBoxBlue->SetVisibleSingle ( FALSE );
			RegisterControl ( m_pLineBoxBlue );

			m_pLineBoxYellow = new CBasicLineBoxEx;
			m_pLineBoxYellow->CreateSub ( this, "BASIC_LINE_BOX_EX_SKILL_YELLOW", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
			m_pLineBoxYellow->CreateBaseBoxSkillYellow ( "RNSKILL_SLOT_YELLOW" );
			m_pLineBoxYellow->SetVisibleSingle ( FALSE );
			RegisterControl ( m_pLineBoxYellow );
		}

		{
			CSkillImage* pSkillImage = CreateSkillImage ( SKILL_IMAGE );
			pSkillImage->SetSkill ( sNativeID );
			m_pSkillImage = pSkillImage;		
		}	

		{
			m_pTextBoxLine1 = CreateStaticControl ( "MOTA_KYNANG_DONGTREN", pFont, dwColor, nTextAlign );
			m_pTextBoxLine2 = CreateStaticControl ( "MOTA_KYNANG_DONGDUOI", pFont, dwColor, nTextAlign );
		}

		{
			m_pNotLearnImage = CreateControl ( "HINH_ANH_O_KYNANG_CHUAHOC" );
		}

		{
			const int nAlignCenterBoth = TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y;
			CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );
			//m_pSkillUpButton = CreateFlipButton ( "SKILL_SLOT_LEVEL_UP", "SKILL_SLOT_LEVEL_UP_F", "SKILL_SLOT_LEVEL_UP_OVER", SKILL_SLOT_LEVEL_UP );
//			m_pButton_Plus = CreateButtonBlue ( "SKILL_SLOT_LEVEL_UP_2015", SKILL_SLOT_LEVEL_UP, " +" );
//			m_pButton_Minus = CreateButtonBlue ( "SKILL_SLOT_LEVEL_DOWN_2015", SKILL_SLOT_LEVEL_DOWN, " -" );

			CreateControl ( "KYNANG_NUT_CONG" );
			CreateControl ( "KYNANG_NUT_TRU" );

			m_pButton_Black = CreateControl ( "KYNANG_NUTBAM_CONG_BG" );
			m_pButton_Black2 = CreateControl ( "KYNANG_NUTBAM_TRU_BG" );
			m_pButton_Plus = CreateButtonBlue ( "KYNANG_NUTBAM_CONG", "KYNANG_NUTBAM_CONG_CHU", pFont, nAlignCenterBoth, SKILL_SLOT_LEVEL_UP, "+" );	
			m_pButton_Minus = CreateButtonBlue ( "KYNANG_NUTBAM_TRU", "KYNANG_NUTBAM_TRU_CHU", pFont, nAlignCenterBoth, SKILL_SLOT_LEVEL_DOWN, "-" );
		}
	}

	m_sNativeID = sNativeID;
	nLevel = 0;
}

CSkillImage* CSkillSlot::CreateSkillImage ( UIGUID ControlID )
{
	CSkillImage* pSkillImage = new CSkillImage;
	pSkillImage->CreateSub ( this, "HINH_ANH_O_KYNANG", UI_FLAG_DEFAULT, ControlID );
	pSkillImage->CreateSubControl ();
	pSkillImage->SetUseRender ( TRUE );
    RegisterControl ( pSkillImage );
	return pSkillImage;
}

CUIControl*	CSkillSlot::CreateControl ( char* szControl )
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, szControl );
	RegisterControl ( pControl );
	return pControl;
}

CBasicButton* CSkillSlot::CreateFlipButton ( char* szButton, char* szButtonFlip, char* szMouseOver, UIGUID ControlID )
{
	CBasicButton* pButton = new CBasicButton;
	pButton->CreateSub ( this, szButton, UI_FLAG_DEFAULT, ControlID );
	pButton->CreateFlip ( szButtonFlip, CBasicButton::RADIO_FLIP );
	pButton->CreateMouseOver ( szMouseOver );
	RegisterControl ( pButton );
	return pButton;
}

CBasicTextBox* CSkillSlot::CreateStaticControl ( char* szControlKeyword, CD3DFontPar* pFont, D3DCOLOR dwColor, int nAlign )
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub ( this, szControlKeyword );
	pStaticText->SetFont ( pFont );
	pStaticText->SetTextAlign ( nAlign );
	RegisterControl ( pStaticText );
	return pStaticText;
}
void CSkillSlot::SKILLSLOT_ACTIVE()
{
	if ( m_sNativeID == NATIVEID_NULL () )
	{
		return ;
	}

	if ( m_bSKILLSLOT_ACTIVE && m_bLearnSkill )
	{
		m_pSkillSlotAvtive->SetVisibleSingle( TRUE );
		m_pLineBoxBlue->SetVisibleSingle ( TRUE );
	}
	else
	{
		if ( m_bCanLearn )
			m_pLineBoxGreen->SetVisibleSingle ( TRUE );
		else
			m_pLineBoxYellow->SetVisibleSingle ( TRUE );
	}
}
void CSkillSlot::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{	
	if ( !IsVisible () ) return ;

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if ( m_sNativeID == NATIVEID_NULL () )
	{
		return ;
	}

	{
		m_pSkillSlotAvtive->SetVisibleSingle( FALSE );
		m_pLineBoxYellow->SetVisibleSingle ( FALSE );
		m_pLineBoxGreen->SetVisibleSingle ( FALSE );
		m_pLineBoxBlue->SetVisibleSingle ( FALSE );
	}


	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( m_sNativeID.wMainID, m_sNativeID.wSubID );
	if ( !pSkill )
	{
		m_pButton_Plus->SetVisibleSingle( FALSE );
		m_pButton_Minus->SetVisibleSingle(FALSE);
		GASSERT ( 0 && "?? ? ?? ?????." );
		return ;
	}

	m_bLearnSkill = FALSE;
	BOOL bMASTER = GLGaeaClient::GetInstance().GetCharacter()->ISMASTER_SKILL ( m_sNativeID );
	if ( m_pTextBoxLine1 && m_pTextBoxLine2 ) 
	{
		SCHARSKILL* pCharSkill = GLGaeaClient::GetInstance().GetCharacter()->GETLEARNED_SKILL ( m_sNativeID );
		WORD pCharLevel = GLGaeaClient::GetInstance().GetCharacter()->m_wLevel;
		if ( pCharSkill )
		{
			m_bLearnSkill = TRUE;
			m_bSKILLSLOT_ACTIVE = TRUE;

			WORD wLevel = pCharSkill->wLevel + 1;
			if ( m_wLevel != wLevel )
			{
				m_wLevel = wLevel;
				SKILL::SLEARN_LVL sLVL = pSkill->m_sLEARN.sLVL_STEP[wLevel];

				m_strLine1.Format ( "%s", pSkill->GetName() );

				if ( !bMASTER )
				{
					m_strLine2.Format ( "%d %s %s. %d", pSkill->m_sBASIC.dwGRADE, (char*)ID2GAMEWORD ( "RNSKILL_SLOT", 0 ), (char*)ID2GAMEWORD ( "RNSKILL_SLOT", 3 ), wLevel + nLevel );
				}
				else
					m_strLine2.Format ( "%s", (char*)ID2GAMEWORD ( "RNSKILL_SLOT", 4 ) );			

				m_pTextBoxLine1->ClearText ();
				m_pTextBoxLine2->ClearText ();
				m_pTextBoxLine1->SetText ( m_strLine1, NS_UITEXTCOLOR::WHITE );
				m_pTextBoxLine2->SetText ( m_strLine2, NS_UITEXTCOLOR::GOLD );

				AddMessageEx ( UIMSG_LEVEL_UP | UIMSG_TOTOPPARENT );
			}
		}		
		else	
		{
			m_bSKILLSLOT_ACTIVE = FALSE;

			m_strLine1.Format ( "%s", pSkill->GetName() );
			m_pTextBoxLine1->ClearText ();
			m_pTextBoxLine1->SetText ( m_strLine1, NS_UITEXTCOLOR::SILVER );

			SKILL::SLEARN_LVL sLVL = pSkill->m_sLEARN.sLVL_STEP[0];
			SNATIVEID NeedSkillID = pSkill->m_sLEARN.sSKILL;
	
			BOOL NEEDSKILL = FALSE;
			BOOL NEEDSTATUS = FALSE;
			BOOL NEEDLEVEL = FALSE;
			BOOL FONT_VALID = FALSE;

			BOOL bNEEDSKP = FALSE;
			DWORD dwSkillPoint = GLGaeaClient::GetInstance().GetCharacter ()->m_dwSkillPoint;
			if ( sLVL.dwSKP <= dwSkillPoint )
			{
				bNEEDSKP = TRUE;
			}
			if ( pCharLevel < sLVL.dwLEVEL )
			{
				NEEDLEVEL = TRUE;
			}			
			if ( NeedSkillID != NATIVEID_NULL() )
			{
				NEEDSKILL = TRUE;
			}
			
			const GLCHARLOGIC& sCharData = GLGaeaClient::GetInstance().GetCharacterLogic ();	
			

			if ( sCharData.m_sSUMSTATS.dwPow < sLVL.sSTATS.dwPow || 
				sCharData.m_sSUMSTATS.dwDex < sLVL.sSTATS.dwDex ||
				sCharData.m_sSUMSTATS.dwSpi < sLVL.sSTATS.dwSpi ||
				sCharData.m_sSUMSTATS.dwSta < sLVL.sSTATS.dwSta ||
				sCharData.m_sSUMSTATS.dwStr < sLVL.sSTATS.dwStr ||
				sCharData.m_sSUMSTATS.dwInt < sLVL.sSTATS.dwInt )
			{
				NEEDSTATUS = TRUE;
			}

			if ( NEEDLEVEL && !NEEDSKILL && !NEEDSTATUS ) 
			{
				m_strLine2.Format ( "%s %d", (char*)ID2GAMEWORD ( "RNSKILL_SLOT", 3 ), sLVL.dwLEVEL );
			}
			else if ( NEEDLEVEL && NEEDSKILL  ) 
			{
				m_strLine2.Format ( "%s %d / %s", (char*)ID2GAMEWORD ( "RNSKILL_SLOT", 3 ), sLVL.dwLEVEL ,(char*)ID2GAMEWORD ( "RNSKILL_SLOT", 5 ) );
			}
			else if ( NEEDLEVEL && !NEEDSKILL  && NEEDSTATUS )
			{
				if ( sCharData.m_sSUMSTATS.dwPow < sLVL.sSTATS.dwPow )
				{
					m_strLine2.Format("%s. %d / %s %d",(char*)ID2GAMEWORD ( "RNSKILL_SLOT", 3 ), sLVL.dwLEVEL ,ID2GAMEWORD( "RNSKILL_SLOT", 6 ), sLVL.sSTATS.dwPow );
				}
				else if ( sCharData.m_sSUMSTATS.dwStr < sLVL.sSTATS.dwStr )
				{
					m_strLine2.Format("%s. %d / %s %d",(char*)ID2GAMEWORD ( "RNSKILL_SLOT", 3 ), sLVL.dwLEVEL ,ID2GAMEWORD( "RNSKILL_SLOT", 9 ), sLVL.sSTATS.dwStr);
				}
				else if ( sCharData.m_sSUMSTATS.dwSpi < sLVL.sSTATS.dwSpi )
				{
					m_strLine2.Format("%s. %d / %s %d",(char*)ID2GAMEWORD ( "RNSKILL_SLOT", 3 ), sLVL.dwLEVEL ,ID2GAMEWORD( "RNSKILL_SLOT", 8 ), sLVL.sSTATS.dwSpi);
				}
				else if ( sCharData.m_sSUMSTATS.dwDex < sLVL.sSTATS.dwDex )
				{
					m_strLine2.Format("%s. %d / %s %d",(char*)ID2GAMEWORD ( "RNSKILL_SLOT", 3 ), sLVL.dwLEVEL ,ID2GAMEWORD( "RNSKILL_SLOT", 7 ), sLVL.sSTATS.dwDex);
				}
				else if ( sCharData.m_sSUMSTATS.dwInt < sLVL.sSTATS.dwInt )
				{
					m_strLine2.Format("%s. %d / %s %d",(char*)ID2GAMEWORD ( "RNSKILL_SLOT", 3 ), sLVL.dwLEVEL ,ID2GAMEWORD( "RNSKILL_SLOT", 11 ), sLVL.sSTATS.dwInt);
				}
				else if ( sCharData.m_sSUMSTATS.dwSta < sLVL.sSTATS.dwSta )
				{
					m_strLine2.Format("%s. %d / %s %d",(char*)ID2GAMEWORD ( "RNSKILL_SLOT", 3 ), sLVL.dwLEVEL ,ID2GAMEWORD( "RNSKILL_SLOT", 10 ), sLVL.sSTATS.dwSta);
				}
			}
			else if ( !NEEDLEVEL && !NEEDSKILL  && NEEDSTATUS )
			{
				if ( sCharData.m_sSUMSTATS.dwPow < sLVL.sSTATS.dwPow )
				{
					m_strLine2.Format("%s %d",ID2GAMEWORD( "RNSKILL_SLOT", 6 ), sLVL.sSTATS.dwPow );
				}
				else if ( sCharData.m_sSUMSTATS.dwStr < sLVL.sSTATS.dwStr )
				{
					m_strLine2.Format("%s %d",ID2GAMEWORD( "RNSKILL_SLOT", 9 ), sLVL.sSTATS.dwStr);
				}
				else if ( sCharData.m_sSUMSTATS.dwSpi < sLVL.sSTATS.dwSpi )
				{
					m_strLine2.Format("%s %d",ID2GAMEWORD( "RNSKILL_SLOT", 8 ), sLVL.sSTATS.dwSpi);
				}
				else if ( sCharData.m_sSUMSTATS.dwDex < sLVL.sSTATS.dwDex )
				{
					m_strLine2.Format("%s %d",ID2GAMEWORD( "RNSKILL_SLOT", 7 ), sLVL.sSTATS.dwDex);
				}
				else if ( sCharData.m_sSUMSTATS.dwInt < sLVL.sSTATS.dwInt )
				{
					m_strLine2.Format("%s %d",ID2GAMEWORD( "RNSKILL_SLOT", 11 ), sLVL.sSTATS.dwInt);
				}
				else if ( sCharData.m_sSUMSTATS.dwSta < sLVL.sSTATS.dwSta )
				{
					m_strLine2.Format("%s %d",ID2GAMEWORD( "RNSKILL_SLOT", 10 ), sLVL.sSTATS.dwSta);
				}
			}
			else
			{
				if ( bNEEDSKP )
				{
					m_bCanLearn = TRUE;
					FONT_VALID = TRUE;
					m_bSKILLSLOT_ACTIVE = FALSE;
					m_strLine2.Format ( "%s %d %s. %d", (char*)ID2GAMEWORD ( "RNSKILL_SLOT", 0 ), pSkill->m_sBASIC.dwGRADE, (char*)ID2GAMEWORD ( "RNSKILL_SLOT", 3 ), nLevel );
				}
				else
				{	
					m_bCanLearn = FALSE;
					FONT_VALID = FALSE;
					m_bSKILLSLOT_ACTIVE = FALSE;
					m_strLine2.Format ( "Skill Point %d", sLVL.dwSKP);
				}
			}

			GLCharacter* const pCharacter = GLGaeaClient::GetInstance().GetCharacter();
			if ( pCharacter->m_lnMoney <= int(GLCONST_CHAR::lnSKILL_PREREQ[pSkill->m_sBASIC.sNATIVEID.wMainID][pSkill->m_sBASIC.sNATIVEID.wSubID])) m_bCanLearn = FALSE;
			
			m_pTextBoxLine2->ClearText ();
			if ( !FONT_VALID )
				m_pTextBoxLine2->SetText ( m_strLine2, NS_UITEXTCOLOR::RED );
			else
				m_pTextBoxLine2->SetText ( m_strLine2, NS_UITEXTCOLOR::SILVER );
		}
	}

	if ( m_pNotLearnImage ) m_pNotLearnImage->SetVisibleSingle ( !m_bLearnSkill );

	if ( m_bCanLearn ) m_pButton_Black->SetVisibleSingle(FALSE);
	else m_pButton_Black->SetVisibleSingle(TRUE);

	if ( m_bLearnSkill )
	{
		if ( bMASTER )
		{
			m_pButton_Black->SetVisibleSingle ( FALSE );
			m_pButton_Plus->SetVisibleSingle ( FALSE );
			m_pButton_Minus->SetVisibleSingle( FALSE );
			m_pButton_Black2->SetVisibleSingle( FALSE );
		}
		else
		{
			m_bLEVELUP_ENABLE = GLGaeaClient::GetInstance().GetCharacter()->CHECKLEARNABLE_SKILL ( m_sNativeID ) == EMSKILL_LEARN_OK;
			m_pButton_Black->SetVisibleSingle ( !m_bLEVELUP_ENABLE );
			m_pButton_Plus->SetVisibleSingle ( TRUE );
			m_pButton_Minus->SetVisibleSingle ( TRUE );
			m_pButton_Black2->SetVisibleSingle( nLevel == 0 );
		}
	}
	else
	{
		m_pButton_Plus->SetVisibleSingle ( TRUE );
		m_pButton_Minus->SetVisibleSingle( FALSE );
		m_pButton_Black->SetVisibleSingle( FALSE );
		m_pButton_Black2->SetVisibleSingle( FALSE );
		if ( m_bCanLearn )
		{
			m_pButton_Black->SetVisibleSingle ( FALSE );
		}
		else
		{
			m_pButton_Black->SetVisibleSingle ( TRUE );
		}
		m_bLEVELUP_ENABLE = FALSE;
	}

	if ( m_bLearnSkill )
	{
		if ( m_pSkillImage ) m_pSkillImage->SetSkillProcess ( m_sNativeID );
	}
}

void CSkillSlot::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
		case SKILL_IMAGE:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				AddMessageEx ( UIMSG_MOUSEIN_SKILLIMAGE | UIMSG_TOTOPPARENT );
			}

			if ( CHECK_MOUSE_IN ( dwMsg ) && m_bLearnSkill )
			{
				if ( dwMsg & UIMSG_LB_DOWN )
				{
					PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( m_sNativeID.wMainID, m_sNativeID.wSubID );
					if ( !pSkill )
					{
						GASSERT ( 0 && "?? ? ?? ?????." );
						return ;
					}
					//	??? ??? ? ? ??.
					if ( pSkill->m_sBASIC.emROLE == SKILL::EMROLE_PASSIVE ) return ;

					CSkillWindowToTray* pSkillWindowToTray = CInnerInterface::GetInstance().GetSkillWindowToTray ();
					if ( !pSkillWindowToTray ) return ;

					pSkillWindowToTray->SetSkill ( m_sNativeID );
					if ( CInnerInterface::GetInstance().GetBasicQuickSkillWindow()->IsVisible() )
					{
						CInnerInterface::GetInstance().HideGroup ( BASIC_QUICK_SKILL_WINDOW );
						CInnerInterface::GetInstance().ShowGroupFocus ( QUICK_SKILL_TRAY_TAB_WINDOW , true);
					}
					else if ( CInnerInterface::GetInstance().GetBasicQuickSkillWindowNew()->IsVisible() )
					{
						CInnerInterface::GetInstance().HideGroup ( BASIC_QUICK_SKILL_NEW_WINDOW );
						CInnerInterface::GetInstance().ShowGroupFocus ( QUICK_SKILL_TRAY_TAB_NEW_WINDOW , true);
					}
				}

				{
					for( int i = 0; i < QUICK_SKILL_SLOT_MAX; ++i)
					{
						if ( UIKeyCheck::GetInstance()->Check ( RANPARAM::SkillSlot[i], DXKEY_DOWN ) )
						{
							PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( m_sNativeID.wMainID, m_sNativeID.wSubID );
							if ( !pSkill )
							{
								GASSERT ( 0 && "?? ? ?? ?????." );
								return ;
							}
							if ( pSkill->m_sBASIC.emROLE == SKILL::EMROLE_PASSIVE ) return ;

							INT nIndex = CInnerInterface::GetInstance().GetSkillTrayTabNew()->GetTabIndex() * QUICK_SKILL_SLOT_MAX + i;
							GLGaeaClient::GetInstance().GetCharacter()->ReqSkillQuickSet (m_sNativeID, nIndex );
						}
					}
				}
			}
		}
		break;

	case SKILL_SLOT_LEVEL_UP:
		{
			//	?? ?? ?
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( m_bLearnSkill )
					AddMessageEx ( UIMSG_MOUSEIN_BUTTON | UIMSG_TOTOPPARENT );
				else
					AddMessageEx ( UIMSG_MOUSEIN_BUTTON2 | UIMSG_TOTOPPARENT );

				if ( m_bCanLearn && dwMsg & UIMSG_LB_UP )
					GLGaeaClient::GetInstance().GetCharacter()->ReqNonInvenSkill ( m_sNativeID );

				if ( m_bLEVELUP_ENABLE && dwMsg & UIMSG_LB_UP )
				{
					SCHARSKILL* pCharSkill = GLGaeaClient::GetInstance().GetCharacter()->GETLEARNED_SKILL ( m_sNativeID );					
					BOOL bMASTER = GLGaeaClient::GetInstance().GetCharacter()->ISMASTER_SKILL ( m_sNativeID );
					if ( pCharSkill )
					{
						if ( !bMASTER )
						{
							PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( m_sNativeID.wMainID, m_sNativeID.wSubID );
							if ( !pSkill )
							{
								GASSERT ( 0 && "" );
								return ;
							}

							bool bLVLUP = GLGaeaClient::GetInstance().GetCharacter()->CHECKLEARNABLE_SKILL ( m_sNativeID ) == EMSKILL_LEARN_OK;
							CInnerInterface::GetInstance().SetSkillUpID ( m_sNativeID.dwID );

							CString strLevelUp = CInnerInterface::GetInstance().MakeString ( ID2GAMEINTEXT("SKILL_LEVEL_UP_CONFIRM"), pSkill->GetName() );                            
							DoModal ( strLevelUp, MODAL_QUESTION, YESNO, MODAL_SKILL_LEVEL_UP_CONFIRM );//							
						}
					}
				}
			}
		}
		break;
	}

	CUIGroup::TranslateUIMessage( ControlID, dwMsg );
}