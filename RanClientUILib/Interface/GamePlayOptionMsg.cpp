#include "StdAfx.h"
#include "GamePlayOption.h"
#include "../EngineUILib/GUInterface/BasicButton.h"
#include "../EngineUILib/GUInterface/BasicTextBox.h"
#include "DxParamSet.h"
#include "InnerInterface.h"
#include "ModalCallerID.h"
#include "ModalWindow.h"
#include "GameTextControl.h"
#include "GLGaeaClient.h"
#include "UITextControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CBasicGamePlayOption::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case HWOPTION_GAMEPLAY_CONFT_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				//DXPARAMSET::GetInstance().m_bDIS_CONFT = !DXPARAMSET::GetInstance().m_bDIS_CONFT;
				//LoadConft ();
				m_bConft = !m_bConft;
				m_pConftButton->SetFlip( m_bConft );
			}
			else if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO (  ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION_DESC", 0 ), NS_UITEXTCOLOR::DEFAULT );
			}
		}
		break;

	case HWOPTION_GAMEPLAY_REVIVAL_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				//DXPARAMSET::GetInstance().m_bDIS_Revival = !DXPARAMSET::GetInstance().m_bDIS_Revival;
				//LoadRevival ();
				m_bNon_Rebirth = !m_bNon_Rebirth;
				m_pRevivalButton->SetFlip( m_bNon_Rebirth );
			}
			else if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO (  ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION_DESC", 1 ), NS_UITEXTCOLOR::DEFAULT );
			}
		}
		break;

	case HWOPTION_GAMEPLAY_TRADE_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				//DXPARAMSET::GetInstance().m_bDIS_TRADE = !DXPARAMSET::GetInstance().m_bDIS_TRADE;
				//LoadTrade ();
				m_bTrade = !m_bTrade;
				m_pTradeButton->SetFlip ( m_bTrade );
			}
			else if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO (  ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION_DESC", 2 ), NS_UITEXTCOLOR::DEFAULT );
			}
		}
		break;

	case HWOPTION_GAMEPLAY_PARTY_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				//DXPARAMSET::GetInstance().m_bDIS_PARTY = !DXPARAMSET::GetInstance().m_bDIS_PARTY;
				//LoadParty ();
				m_bParty = !m_bParty;
				m_pPartyButton->SetFlip ( m_bParty );
			}
			else if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO (  ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION_DESC", 3 ), NS_UITEXTCOLOR::DEFAULT );
			}
		}
		break;

	case HWOPTION_GAMEPLAY_SIMPLEHP_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				//DXPARAMSET::GetInstance().m_bSHOW_SIMPLEHP = !DXPARAMSET::GetInstance().m_bSHOW_SIMPLEHP;
				//LoadSimpleHP ();
				m_bSimpleHP = !m_bSimpleHP;
				m_pSimpleHPButton->SetFlip ( m_bSimpleHP );
			}
			else if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO (  ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION_DESC", 4 ), NS_UITEXTCOLOR::DEFAULT );
			}
		}
		break;

	case HWOPTION_GAMEPLAY_FRIEND_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				//RANPARAM::bDIS_FRIEND = !RANPARAM::bDIS_FRIEND;
				//LoadFriend ();
				m_bFriend = !m_bFriend;
				m_pFriendButton->SetFlip ( m_bFriend );
			}
			else if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO (  ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION_DESC", 5 ), NS_UITEXTCOLOR::DEFAULT );
			}
		}
		break;

	case HWOPTION_GAMEPLAY_DEFAULTPOS_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				DoModal ( ID2GAMEINTEXT("DEFAULT_POSITION"), MODAL_QUESTION, YESNO, MODAL_DEFAULT_POS );					
			}
			else if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO (  ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION_DESC", 6 ), NS_UITEXTCOLOR::DEFAULT );
			}
		}
		break;

	case HWOPTION_GAMEPLAY_MOVABLE_ON_CHAT_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				//RANPARAM::bMOVABLE_ON_CHAT = !RANPARAM::bMOVABLE_ON_CHAT;
				//LoadMovableOnChat ();
				m_bMovableOnChat = !m_bMovableOnChat;
				m_pMovableOnChatButton->SetFlip ( m_bMovableOnChat );
			}
			else if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO (  ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION_DESC", 7 ), NS_UITEXTCOLOR::DEFAULT );
			}
		}
		break;

	case HWOPTION_GAMEPLAY_SHOW_TIP_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				//RANPARAM::bSHOW_TIP = !RANPARAM::bSHOW_TIP;					
				//LoadShowTip ();
				m_bShowTip = !m_bShowTip;
				m_pShowTipButton->SetFlip ( m_bShowTip );
			}
			else if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO (  ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION_DESC", 8 ), NS_UITEXTCOLOR::DEFAULT );
			}
		}
		break;

	case HWOPTION_GAMEPLAY_FORCED_ATTACK_BUTTON:			// ���� ����
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				//RANPARAM::bFORCED_ATTACK = !RANPARAM::bFORCED_ATTACK;
				//LoadForcedAttack();
				m_bForcedAttack = !m_bForcedAttack;
				m_pForcedAttackButton->SetFlip ( m_bForcedAttack );
			}
			else if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO (  ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION_DESC", 9 ), NS_UITEXTCOLOR::DEFAULT );
			}
		}
		break;

	case HWOPTION_GAMEPLAY_NAME_DISPLAY_BUTTON:			// �̸� �׻� ǥ��
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				//RANPARAM::bNAME_DISPLAY = !RANPARAM::bNAME_DISPLAY;
				//LoadNameDisplay();
				m_bNameDisplay = !m_bNameDisplay;
				m_pNameDisplayButton->SetFlip ( m_bNameDisplay );
			}
			else if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO (  ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION_DESC", 10 ), NS_UITEXTCOLOR::DEFAULT );
			}
		}
		break;
	case HWOPTION_GAMEPLAY_SET_DISPLAY_BUTTON:			// �̸� �׻� ǥ��
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				//RANPARAM::bNAME_DISPLAY = !RANPARAM::bNAME_DISPLAY;
				//LoadNameDisplay();
				m_bSetDisplay = !m_bSetDisplay;
				m_pSetDisplayButton->SetFlip ( m_bSetDisplay );
			}
			else if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO (  ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION_DESC", 17 ), NS_UITEXTCOLOR::DEFAULT );
			}
		}
		break;

		//jdev help
	case HWOPTION_GAMEPLAY_HELP_POPUP_BUTTON:			// �̸� �׻� ǥ��
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				m_bHelpPopUp = !m_bHelpPopUp;
				m_pHelpPopUpButton->SetFlip ( m_bHelpPopUp );
			}
			else if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO (  ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION_DESC", 11 ), NS_UITEXTCOLOR::DEFAULT );
			}
		}
		break;

	case HWOPTION_GAMEPLAY_IDLE_ANIM_BUTTON:			// �̸� �׻� ǥ��
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				m_bIdleAnim = !m_bIdleAnim;
				m_pIdleAnimButton->SetFlip ( m_bIdleAnim );
			}
			else if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO (  ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION_DESC", 12 ), NS_UITEXTCOLOR::DEFAULT );
			}
		}
		break;

	case HWOPTION_GAMEPLAY_RANK_NAME_BUTTON:			// add pkrank
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				m_bRankName = !m_bRankName;
				m_pRankNameButton->SetFlip ( m_bRankName );
			}
			else if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO (  ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION_DESC", 13 ), NS_UITEXTCOLOR::DEFAULT );
			}
		}
		break;

	case HWOPTION_GAMEPLAY_RANK_MARK_BUTTON:			// add pkrank
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				m_bRankMark = !m_bRankMark;
				m_pRankMarkButton->SetFlip ( m_bRankMark );
			}
			else if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO (  ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION_DESC", 14 ), NS_UITEXTCOLOR::DEFAULT );
			}
		}
		break;

	case HWOPTION_GAMEPLAY_ALLOW_BET_BUTTON:			// add duel bet
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				m_bAllowBet = !m_bAllowBet;
				m_pAllowBetButton->SetFlip ( m_bAllowBet );
			}
			else if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO (  ID2GAMEWORD ( "HWOPTION_GAMEPLAY_OPTION_DESC", 15 ), NS_UITEXTCOLOR::DEFAULT );
			}
		}
		break;
	}
}