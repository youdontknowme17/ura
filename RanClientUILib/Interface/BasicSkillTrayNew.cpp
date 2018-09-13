#include "StdAfx.h"
#include "BasicSkillTrayNew.h"
#include "BasicSkillTray.h"

#include "BasicQuickSkillSlotEx.h"
#include "GLGaeaClient.h"
#include "../../EngineLib/DxCommon/DxFontMan.h"
#include "InnerInterface.h"
#include "SkillWindowToTray.h"
#include "BasicVarTextBox.h"
#include "UITextControl.h"
#include "GameTextControl.h"
#include "d3dfontx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const	int	CBasicSkillTrayNew::nLBUPSKIP = 1;

CBasicSkillTrayNew::CBasicSkillTrayNew ()
	: m_nLBUPSKIP(0)
	, m_pSkillInfo(NULL)
{
	memset ( m_pSlotEx, 0, sizeof ( m_pSlotEx ) );
}

CBasicSkillTrayNew::~CBasicSkillTrayNew ()
{
}

void CBasicSkillTrayNew::CreateSubControl( INT nTabIndex )
{
	GASSERT( ( 0 <= nTabIndex ) && ( nTabIndex < MAX_TAB_INDEX ) );

	CD3DFontPar * m_pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG );

	for ( int nNumber = 0; nNumber < QUICK_SKILL_SLOT_MAX; ++nNumber )
	{
		CString strKeyword;
		strKeyword.Format ( "BASIC_QUICK_SKILL_TRAY_NEW_SLOT%d", nNumber );

		CBasicQuickSkillSlotEx*	pSlot = new CBasicQuickSkillSlotEx;
		pSlot->CreateSub( this, strKeyword.GetString(), UI_FLAG_DEFAULT, QUICK_SKILL_SLOT1 + nNumber );
		pSlot->CreateSubControl( nTabIndex );
		pSlot->CreateNumberText( m_pFont8, nNumber );		
		RegisterControl( pSlot );
		m_pSlotEx[nNumber] = pSlot;
	}

	CUIControl* pSlotEnd = new CUIControl;
	pSlotEnd->CreateSub ( this, "BASIC_QUICK_SKILL_TRAY_NEW_SLOT_END" );
	pSlotEnd->SetControlNameEx ( "���� ����" );
    RegisterControl ( pSlotEnd );

	CBasicVarTextBox* pSkillInfo = new CBasicVarTextBox;
	pSkillInfo->CreateSub ( this, "BASIC_VAR_TEXT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pSkillInfo->CreateSubControl ();
	pSkillInfo->SetVisibleSingle ( FALSE );
	RegisterControl ( pSkillInfo );
	m_pSkillInfo = pSkillInfo;
}

void CBasicSkillTrayNew::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case QUICK_SKILL_SLOT1:
	case QUICK_SKILL_SLOT2:
	case QUICK_SKILL_SLOT3:
	case QUICK_SKILL_SLOT4:
	case QUICK_SKILL_SLOT5:
	case QUICK_SKILL_SLOT6:
	case QUICK_SKILL_SLOT7:
	case QUICK_SKILL_SLOT8:
	case QUICK_SKILL_SLOT9:
	case QUICK_SKILL_SLOT0:
		{			
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{	
				int nG_Index = m_pSlotEx[0]->GetTabIndex() * QUICK_SKILL_SLOT_MAX;
				int nIndex = ControlID - QUICK_SKILL_SLOT1;
				nG_Index += nIndex;

				SNATIVEID sOverSkill = GLGaeaClient::GetInstance().GetCharacter()->GetCharData().m_sSKILLQUICK[nG_Index];
                if ( sOverSkill != NATIVEID_NULL () )
				{
					PGLSKILL pSkill = GLSkillMan::GetInstance().GetData( sOverSkill.wMainID, sOverSkill.wSubID );
					SCHARSKILL* pCharSkill = GLGaeaClient::GetInstance().GetCharacter()->GETLEARNED_SKILL ( sOverSkill );
					const SKILL::SAPPLY& sAPPLY = pSkill->m_sAPPLY;
					WORD wLevel = pCharSkill->wLevel;
					const SKILL::CDATA_LVL& sDATA_LVL = sAPPLY.sDATA_LVL[wLevel];
					if ( CInnerInterface::GetInstance().BEGIN_COMMON_LINEINFO_MULTI () )
					{
						CString strCombine;
						strCombine.Format("%s(Lv.%d)", pSkill->GetName(), wLevel);
						CInnerInterface::GetInstance().ADD_COMMON_LINEINFO_MULTI ( strCombine.GetString(), NS_UITEXTCOLOR::PALEGREEN );

						if ( 0 != sDATA_LVL.fBASIC_VAR )
						{
							if ( 0 > sDATA_LVL.fBASIC_VAR )
							{
								CString strText;
								float fBASIC_VAR_SCALE = sDATA_LVL.fBASIC_VAR * COMMENT::SKILL_TYPES_SCALE[sAPPLY.emBASIC_TYPE];
								if ( pSkill->m_sBASIC.emIMPACT_TAR == TAR_SELF )
								{
									if ( COMMENT::IsSKILL_PER(sAPPLY.emBASIC_TYPE) )
										strText.Format("Let %s decrease %2.1f%%", COMMENT::SKILL_TYPES[sAPPLY.emBASIC_TYPE].c_str(), fBASIC_VAR_SCALE );
									else
										strText.Format("Let %s decrease %2.1f", COMMENT::SKILL_TYPES[sAPPLY.emBASIC_TYPE].c_str(), fBASIC_VAR_SCALE );
								}
								else
								{
									if ( COMMENT::IsSKILL_PER(sAPPLY.emBASIC_TYPE) )
										strText.Format("Let target's %s decrease %2.1f%%", COMMENT::SKILL_TYPES[sAPPLY.emBASIC_TYPE].c_str(), fBASIC_VAR_SCALE );
									else
										strText.Format("Let target's %s decrease %2.1f", COMMENT::SKILL_TYPES[sAPPLY.emBASIC_TYPE].c_str(), fBASIC_VAR_SCALE );
								}

								CInnerInterface::GetInstance().ADD_COMMON_LINEINFO_MULTI(strText, NS_UITEXTCOLOR::GREEN );
							}
							else
							{
								CString strText;
								float fBASIC_VAR_SCALE = sDATA_LVL.fBASIC_VAR * COMMENT::SKILL_TYPES_SCALE[sAPPLY.emBASIC_TYPE];
								if ( pSkill->m_sBASIC.emIMPACT_TAR == TAR_SELF )
								{
									if ( COMMENT::IsSKILL_PER(sAPPLY.emBASIC_TYPE) )
										strText.Format("Let %s increase %2.1f%%", COMMENT::SKILL_TYPES[sAPPLY.emBASIC_TYPE].c_str(), fBASIC_VAR_SCALE );
									else
										strText.Format("Let %s increase %2.1f", COMMENT::SKILL_TYPES[sAPPLY.emBASIC_TYPE].c_str(), fBASIC_VAR_SCALE );
								}
								else
								{
									if ( COMMENT::IsSKILL_PER(sAPPLY.emBASIC_TYPE) )
										strText.Format("Let target's %s increase %2.1f%%", COMMENT::SKILL_TYPES[sAPPLY.emBASIC_TYPE].c_str(), fBASIC_VAR_SCALE );
									else
										strText.Format("Let target's %s increase %2.1f", COMMENT::SKILL_TYPES[sAPPLY.emBASIC_TYPE].c_str(), fBASIC_VAR_SCALE );
								}

								CInnerInterface::GetInstance().ADD_COMMON_LINEINFO_MULTI(strText, NS_UITEXTCOLOR::GREEN );
							}
						}

						CInnerInterface::GetInstance().END_COMMON_LINEINFO_MULTI ();
					}
				}				

				//	����, ��ų �̹��� ���̱�
				CSkillWindowToTray* pSkillWindowToTray = CInnerInterface::GetInstance().GetSkillWindowToTray ();
				if ( !pSkillWindowToTray )
				{
					GASSERT ( 0 && "CSkillWindowToTray�� ���Դϴ�." );
					return ;
				}

				if ( pSkillWindowToTray->GetSkill () != NATIVEID_NULL () )
				{
					const UIRECT& rcSlotPos = m_pSlotEx[nIndex]->GetAbsPosSkillImage ();
					pSkillWindowToTray->SetGlobalPos ( rcSlotPos );						

					AddMessageEx ( UIMSG_MOUSEIN_SKILLSLOT );
				}		

				if ( dwMsg & UIMSG_LB_UP )
				{	
					SNATIVEID sNativeID = pSkillWindowToTray->GetSkill ();

					//	��ų ���
					if ( sNativeID != NATIVEID_NULL () )
					{						
						GLGaeaClient::GetInstance().GetCharacter()->ReqSkillQuickSet ( sNativeID, nG_Index );
						pSkillWindowToTray->ResetSkill ();
						m_nLBUPSKIP = 0;
					}
					else
					{
						//	( ����� ��ų�� ���� ��� ) ���������� ��ų�� ����� ��ų�� ����.
						GLGaeaClient::GetInstance().GetCharacter()->ReqSkillRunSet( nG_Index );
					}					
				}

				//	��ų ����
				if ( dwMsg & UIMSG_RB_UP )
				{					
					GLGaeaClient::GetInstance().GetCharacter()->ReqSkillQuickReSet ( nG_Index );
				}
			}
		}
		break;
	}
}

void	CBasicSkillTrayNew::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	m_pSkillInfo->SetVisibleSingle ( FALSE );

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	DWORD dwMsg = GetMessageEx ();
	if ( dwMsg & UIMSG_LB_UP )
	{
		CSkillWindowToTray* pSkillWindowToTray = CInnerInterface::GetInstance().GetSkillWindowToTray ();
		if ( !pSkillWindowToTray )	return ;

		//	��ų�� �տ� ������ְ�...
		//	ù��° ����Ŭ���޽����� �����Ѵ�.
		BOOL bExist = ( pSkillWindowToTray->GetSkill () != NATIVEID_NULL () );
		if ( bExist )
		{
			if ( nLBUPSKIP <= m_nLBUPSKIP )
			{
				pSkillWindowToTray->ResetSkill ();
				m_nLBUPSKIP = 0;
			}
			else
			{
				m_nLBUPSKIP++;
			}
		}		
	}
}

void CBasicSkillTrayNew::UpdateSkillInfo ( SNATIVEID sNativeID )
{
	if ( sNativeID != NATIVEID_NULL() )
	{
		static SNATIVEID sNativeIDBack = NATIVEID_NULL();

		BOOL bUPDATE = FALSE;
		if ( sNativeID != sNativeIDBack )
		{			
			bUPDATE = TRUE;
		}

		if ( bUPDATE )
		{
			PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( sNativeID.wMainID, sNativeID.wSubID );
			if ( pSkill )	//	��ų�� ������ ���
			{
				SCHARSKILL* pCharSkill = GLGaeaClient::GetInstance().GetCharacter()->GETLEARNED_SKILL ( sNativeID );
				LoadSkillInfo ( pSkill, pCharSkill );
			}

			sNativeIDBack = sNativeID;
		}		
	}
}

void CBasicSkillTrayNew::LoadSkillInfo ( PGLSKILL const pSkill, SCHARSKILL* const pCharSkill )
{
	m_pSkillInfo->SetTextNoSplit( pSkill->GetName(), NS_UITEXTCOLOR::PALEGREEN );
}
void CBasicSkillTrayNew::SetShotcutText ( DWORD nID, CString& strTemp )
{
	m_pSlotEx[nID]->SetShotcutText(strTemp);	
}
