#include "StdAfx.h"
#include "BasicPotionTray.h"

#include "BasicQuickPotionSlotEx.h"
#include "../EngineLib/DxCommon/DxFontMan.h"
#include "../EngineUILib/GUInterface/BasicButton.h"
#include "InnerInterface.h"
#include "GLItemMan.h"
#include "UITextControl.h"

#include "GLGaeaClient.h"
#include "ItemMove.h"
#include "d3dfont.h"

#include "RANPARAM.h"
#include "UITextControl.h"
#include "GameTextControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const	int	CBasicPotionTray::nOUTOFRANGE = -1;

CBasicPotionTray::CBasicPotionTray () :
	 m_pFont8 ( NULL )
	, pLockButton(NULL)
	, bTrayLock(FALSE)
{
}

CBasicPotionTray::~CBasicPotionTray ()
{
}

void CBasicPotionTray::CreateSubControl ()
{
	m_pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, D3DFONT_SHADOW | D3DFONT_ASCII );

	CUIControl* pSlotBegin = new CUIControl;
	pSlotBegin->CreateSub ( this, "BASIC_QUICK_POTION_TRAY_SLOT_BEGIN" );
	pSlotBegin->SetControlNameEx ( "�� ���� ���� ��ŸƮ" );
    RegisterControl ( pSlotBegin );

	for ( int i = 0; i < EMACTIONQUICK_SIZE; i++ )
	{
		CString strKeyword;
		strKeyword.Format ( "BASIC_QUICK_POTION_TRAY_SLOT%d", i );

		CBasicQuickPotionSlotEx* pSlot = new CBasicQuickPotionSlotEx;
		pSlot->CreateSub ( this, strKeyword.GetString(), UI_FLAG_DEFAULT, QUICK_POTION_SLOT0 + i );
		pSlot->CreateSubControl ( i );
		RegisterControl ( pSlot );
		m_pSlot[i] = pSlot;
	}

// #ifndef CH_PARAM // �߱� �������̽� ����
	CBasicButton* pButton = new CBasicButton;
	pButton->CreateSub ( this, "QUICK_POTION_TRAY_CLOSE_BUTTON", UI_FLAG_DEFAULT, QUICK_POTION_TRAY_CLOSE_BUTTON );
	pButton->CreateFlip ( "QUICK_POTION_TRAY_CLOSE_BUTTON_F", CBasicButton::MOUSEIN_FLIP );
	pButton->SetUseGlobalAction ( TRUE );
	pButton->SetControlNameEx ( "QUICK_POTION_TRAY_CLOSE_BUTTON" );
	RegisterControl ( pButton );
// #endif

	pLockButton = CreateFlipButton( "QUICK_POTION_TRAY_LOCK_BUTTON", "QUICK_POTION_TRAY_LOCK_BUTTON_F", QUICK_POTION_TRAY_LOCK_BUTTON ); //add lock

	bTrayLock = RANPARAM::bLockQTray;
	pLockButton->SetFlip ( bTrayLock );
}

CBasicButton* CBasicPotionTray::CreateFlipButton ( char* szButton, char* szButtonFlip, UIGUID ControlID )
{
	CBasicButton* pButton = new CBasicButton;
	pButton->CreateSub ( this, szButton, UI_FLAG_DEFAULT, ControlID );
	pButton->CreateFlip ( szButtonFlip, CBasicButton::RADIO_FLIP );
	pButton->SetControlNameEx ( szButton );
	RegisterControl ( pButton );
	return pButton;
}

void CBasicPotionTray::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case QUICK_POTION_SLOT0:
	case QUICK_POTION_SLOT1:
	case QUICK_POTION_SLOT2:
	case QUICK_POTION_SLOT3:
	case QUICK_POTION_SLOT4:
	case QUICK_POTION_SLOT5:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{	
				int nIndex = ControlID - QUICK_POTION_SLOT0;
				SetSlotIndex ( nIndex );

				if ( !CInnerInterface::GetInstance().IsFirstItemSlot () ) return ;				

				//	����, ��ų �̹��� ���̱�
				CItemMove* pItemMove = CInnerInterface::GetInstance().GetItemMove ();
				if ( !pItemMove )
				{
					GASSERT ( 0 && "CItemMove�� ���Դϴ�." );
					return ;
				}

				if ( pItemMove->GetItem () != NATIVEID_NULL () )
				{
					const UIRECT& rcSlotPotionImagePos = m_pSlot[nIndex]->GetAbsPosPotionImage ();
					pItemMove->SetGlobalPos ( rcSlotPotionImagePos );						

					AddMessageEx ( UIMSG_MOUSEIN_POTIONSLOT );
				}

				if ( dwMsg & UIMSG_LB_UP )
				{
					const SITEMCUSTOM& sItemCustom = GLGaeaClient::GetInstance().GetCharacter()->GET_HOLD_ITEM ();
					if ( sItemCustom.sNativeID != NATIVEID_NULL () )	//	����
					{
						if ( RANPARAM::bLockQTray )
						{
						CInnerInterface::GetInstance().PrintConsoleText ( ID2GAMEWORD ( "POTION_TRAY_DESC", 4 ) );
						}
						else
						{
							GLGaeaClient::GetInstance().GetCharacter()->ReqItemQuickSet ( (WORD) nIndex );
						}
					}
					else	//	����
					{
						GLGaeaClient::GetInstance().GetCharacter()->ReqActionQ ( (WORD) nIndex );
					}
				}

				//	�� ������ ����
				if ( dwMsg & UIMSG_RB_UP )
				{
					if ( RANPARAM::bLockQTray )
					{
						CInnerInterface::GetInstance().PrintConsoleText ( ID2GAMEWORD ( "POTION_TRAY_DESC", 3 ) );
					}
					else
					{
						GLGaeaClient::GetInstance().GetCharacter()->ReqItemQuickReSet ( (WORD) nIndex );
					}
				}

                //	���� �Ѹ���
				SACTION_SLOT sSlot = GLGaeaClient::GetInstance().GetCharacter()->m_sACTIONQUICK[nIndex];
				if ( sSlot.sNID != NATIVEID_NULL () )
				{
					SITEM* pItem = GLItemMan::GetInstance().GetItem ( sSlot.sNID );
					if ( pItem )
					{
						DWORD dwLevel = pItem->sBasicOp.emLevel;
						CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO ( pItem->GetName(), COMMENT::ITEMCOLOR[dwLevel] );
					}
				}
			}
		}
		break;

	case QUICK_POTION_TRAY_CLOSE_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( CHECK_LB_UP_LIKE ( dwMsg ) )
				{
					AddMessageEx ( UIMSG_TRAY_CLOSE_LBUP );
				}
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO (  ID2GAMEWORD ( "POTION_TRAY_DESC", 2 ), NS_UITEXTCOLOR::DEFAULT );
			}
		}
		break;

	case QUICK_POTION_TRAY_LOCK_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( CHECK_LB_UP_LIKE ( dwMsg ) )
				{
					bTrayLock = !bTrayLock;
					pLockButton->SetFlip ( bTrayLock );

					SaveSetting();
				}

				if ( bTrayLock ) CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO (  ID2GAMEWORD ( "POTION_TRAY_DESC", 0 ), NS_UITEXTCOLOR::DEFAULT );
				else	CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO (  ID2GAMEWORD ( "POTION_TRAY_DESC", 1 ), NS_UITEXTCOLOR::DEFAULT );
			}
		}
		break;
	}
}

void CBasicPotionTray::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{	
	if ( !IsVisible () ) return ;
	SetSlotIndex ( nOUTOFRANGE );
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );
}

void 	CBasicPotionTray::SetShotcutText ( DWORD nID, CString& strTemp )
{
	m_pSlot[nID]->SetShotcutText(strTemp );	
}

void CBasicPotionTray::SaveSetting ()
{
	RANPARAM::bLockQTray = bTrayLock;
}

