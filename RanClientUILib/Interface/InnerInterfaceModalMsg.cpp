#include "StdAfx.h"
#include "ClubWindow.h"
#include "ConftModalWindow.h"
#include "DialogueWindow.h"
#include "DxGlobalStage.h"
#include "FriendWindow.h"
#include "GameTextControl.h"
#include "GLFriendClient.h"
#include "GLGaeaClient.h"
#include "InnerInterface.h"
#include "InventoryWindow.h"
#include "MarketWindow.h"
#include "ModalCallerID.h"
#include "ModalWindow.h"
#include "PartyModalWindow.h"
#include "PartyWindow.h"
#include "PrivateMarketSellWindow.h"
#include "PrivateMarketWindow.h"
#include "PtoPWindow.h"
#include "QuestWindow.h"
#include "RANPARAM.h"
#include "StorageChargeCard.h"
#include "StorageWindow.h"
#include "TradeWindow.h"
#include "UITextControl.h"
#include "WaitServerDialogue.h"
#include "RebirthDialogue.h"
#include "PointShopWindow.h"
#include "ItemMixWindow.h"
#include "../CharacterViewPropertiesWindow.h"//add charinfoview
#include "BasicQuickSkillWindow.h"
#include "BasicQuickSkillWindowNew.h"
#include "SkillTrayTabNew.h"
#include "GLItem.h"
#include "GLItemMan.h"
#include "GLCharacter.h"
#include "SkillTimeDisplay.h"
#include "ItemRebuild/ItemRebuild.h"
#include "NotifyRequest/NotifyRequestWindow.h"
#include "NotifyRequest/NotifyRequestIcon.h"
#include "GmcVocWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CInnerInterface::MsgProcess ( LPVOID msgBuffer )
{
	if( !msgBuffer) return ;

	NET_MSG_GENERIC *nmg = (NET_MSG_GENERIC*) msgBuffer;

	switch ( nmg->nType )
	{
	//////////////////////////////////////////////////////////////////////////
	case NET_MSG_SNDGENVOC_FB:
	{
		NET_SNDGENVOC_FB_DATA* pNetMsg = (NET_SNDGENVOC_FB_DATA*)nmg;
		const int nResult = pNetMsg->nResult;

		switch (nResult)
		{
		case EM_SNDGENVOC_FB_SUB_FAIL:
		{
			DoModal(ID2GAMEEXTEXT("GMC_VOC_WND_STAGE_2"));
			return;
		}
		break;

		case EM_SNDGENVOC_FB_SUB_OK:
		{
			DoModal(ID2GAMEEXTEXT("GMC_VOC_WND_STAGE_3"));
			return;
		}
		break;
		}
	}

	case NET_MSG_SNDSRCHVOC_FB:
	{
		NET_SNDSRCHVOC_FB_DATA* pNetMsg = (NET_SNDSRCHVOC_FB_DATA*)nmg;
		const int nResult = pNetMsg->nResult;

		switch (nResult)
		{
		case EM_SNDSRCHVOC_FB_SUB_FAIL:
		{
			DoModal(ID2GAMEEXTEXT("GMC_VOC_WND_STAGE_4"));
			return;
		}
		break;

		case EM_SNDSRCHVOC_FB_SUB_NONE:
		{
			DoModal(ID2GAMEEXTEXT("GMC_VOC_WND_STAGE_5"));
			return;
		}
		break;

		// GameExText : GMC_VOC_WND_STAGE_6
		case EM_SNDSRCHVOC_FB_SUB_USERID_UNCLAIMED:
		{
			DoModal(ID2GAMEEXTEXT("GMC_VOC_WND_STAGE_6"));
			return;
			m_pGmcVocWnd->LoadVocByUserIDUnClaimed();	// We Send SearchVoc Command Again to SQl This Time Using SQLExecDirect
		}
		break;

		// GameExText : GMC_VOC_WND_STAGE_7
		case EM_SNDSRCHVOC_FB_SUB_USERID_CLAIMED:
		{
			DoModal(ID2GAMEEXTEXT("GMC_VOC_WND_STAGE_7"));
			return;
			m_pGmcVocWnd->LoadVocByUserIDClaimed();		// We Send SearchVoc Command Again to SQl This Time Using SQLExecDirect
		}
		break;

		// GameExText : GMC_VOC_WND_STAGE_8
		case EM_SNDSRCHVOC_FB_SUB_VOCID_UNCLAIMED:
		{
			DoModal(ID2GAMEEXTEXT("GMC_VOC_WND_STAGE_8"));
			return;
			m_pGmcVocWnd->LoadVocByVocIDUnClaimed();	// We Send SearchVoc Command Again to SQl This Time Using SQLExecDirect
		}
		break;

		// GameExText : GMC_VOC_WND_STAGE_9
		case EM_SNDSRCHVOC_FB_SUB_VOCID_CLAIMED:
		{
			DoModal(ID2GAMEEXTEXT("GMC_VOC_WND_STAGE_9"));
			return;
			m_pGmcVocWnd->LoadVocByVocIDClaimed();		// We Send SearchVoc Command Again to SQl This Time Using SQLExecDirect
		}
		break;
		}
	}
	break;

	////////////////////////////////////////////////////////////////////

	//TopUp
	case NET_MSG_TOPUP_FB:
	{
		NET_TOPUP_FB* NTFB = (NET_TOPUP_FB *)nmg;

		switch (NTFB->nResult)
		{
		case EM_TOPUP_FB_SUB_FAIL:
		{
			// GameExText02 : TOPUP_WINDOW_STAGE_3	| TopUp Failed
			DoModal(ID2GAMEEXTEXT("TOPUP_WINDOW_STAGE_3"));
			return;
		}
		break;

		case EM_TOPUP_FB_SUB_INCORRECT:
		{
			// GameExText02 : TOPUP_WINDOW_STAGE_4	| Invalid VoucherID / VoucherCodes
			DoModal(ID2GAMEEXTEXT("TOPUP_WINDOW_STAGE_4"));
			return;
		}
		break;

		case EM_TOPUP_FB_SUB_USED:
		{
			// GameExText02 : TOPUP_WINDOW_STAGE_5	| Voucher Already Used
			DoModal(ID2GAMEEXTEXT("TOPUP_WINDOW_STAGE_5"));
			return;
		}
		break;

		case EM_TOPUP_FB_SUB_CAPTCHA:
		{
			// GameExText02 : TOPUP_WINDOW_STAGE_2	| Wrong Captcha	[ServerSide]
			DoModal(ID2GAMEEXTEXT("TOPUP_WINDOW_STAGE_2"));
			return;
		}
		break;

		case EM_TOPUP_FB_SUB_OK:
		{
			GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
			if (pCharacter)
			{
				LONGLONG m_lnSaldo = NTFB->m_lnSaldo;

				pCharacter->ReqTopUp(m_lnSaldo);
			}

			// GameExText02 : TOPUP_WINDOW_STAGE_6	| TopUp Succeed
			DoModal(ID2GAMEEXTEXT("TOPUP_WINDOW_STAGE_6"));
			return;
		}
		break;
		}
	}
	break;

	case NET_MSG_GCTRL_VIEWCHAROTEM_SLOT_TAR: 
		{
			GLMSG::SNET_VIEWCHARITEM_SLOT_TAR *pNetMsg = (GLMSG::SNET_VIEWCHARITEM_SLOT_TAR *) nmg;
			CInnerInterface::GetInstance().SetInventoryViewWindowItem(pNetMsg->nSlot, pNetMsg->PutOnItem);
		}
	break; 
	case NET_MSG_EVENT_LOTTERY_FB:  
		{
			//	�ζ�
			NET_EVENT_LOTTERY_FB* pNetMsg = (NET_EVENT_LOTTERY_FB*)nmg;
			const int nResult = pNetMsg->nResult;

			enum
			{
				LOTTERY_FB_NO1 = 1,
				LOTTERY_FB_NO2,
				LOTTERY_FB_NO3,
				LOTTERY_FB_NO4,
				LOTTERY_FB_NO5,
				LOTTERY_FB_NO6,
				LOTTERY_FB_ALREADY_INPUTED,
				LOTTERY_FB_NOT_PCROOM,
				LOTTERY_FB_ALREADY_USED,
				LOTTERY_FB_ERROR = 10,
			};

			switch ( nResult )
			{
			case LOTTERY_FB_NO1:
			case LOTTERY_FB_NO2:
			case LOTTERY_FB_NO3:
			case LOTTERY_FB_NO4:
			case LOTTERY_FB_NO5:
			case LOTTERY_FB_NO6:
				{
					CString strTemp = MakeString ( ID2GAMEINTEXT("LOTTERY_FB_NO1_6"), nResult );
					DoModal ( strTemp );
				}
				break;

			case LOTTERY_FB_ALREADY_INPUTED:
				{
					DoModal ( ID2GAMEINTEXT("LOTTERY_FB_ALREADY_INPUTED") );
				}
				break;
			case LOTTERY_FB_NOT_PCROOM:
				{
					DoModal ( ID2GAMEINTEXT("LOTTERY_FB_NOT_PCROOM") );
				}
				break;
			case LOTTERY_FB_ALREADY_USED:
				{
					DoModal ( ID2GAMEINTEXT("LOTTERY_FB_ALREADY_USED") );
				}
				break;
			case LOTTERY_FB_ERROR:
				{
					DoModal ( ID2GAMEINTEXT("LOTTERY_FB_ERROR") );
				}
				break;
			}
		}
		break;
	
	case NET_MSG_APEX_ANSWER: 
		{
			NET_APEX_ANSWER* pNetMsg = (NET_APEX_ANSWER*) nmg;
			switch( pNetMsg->wAction )
			{
			case 1: // ���� ����ϴ°��� �߰������� �������� ����� �ִ� �޼���
				{
					CString strTemp( ID2GAMEINTEXT( "APEX_ACTION_MESSAGE_WARNING" ) );
					PrintConsoleText( strTemp.GetString() );
					DoModal( strTemp, MODAL_INFOMATION, OK, NO_ID );
				}
				break;
			case 2: // ��ȭ ����
				{
					CString strTemp;
					strTemp.Format( ID2GAMEINTEXT( "APEX_ACTION_MESSAGE_CHATBLOCK" ), (pNetMsg->wHour*60) );
					PrintConsoleText( strTemp.GetString() );
					DoModal( strTemp, MODAL_INFOMATION, OK, NO_ID );
				}
				break;
			case 3: // ���� ű
			case 4: // ���� ����
				{
					CString strTemp( ID2GAMEINTEXT( "APEX_ACTION_MESSAGE_KICK" ) );
					PrintConsoleText( strTemp.GetString() );
					WAITSERVER_DIALOGUE_OPEN( strTemp, WAITSERVER_CLOSEGAME, 30.0f );
				}
				break;
			default:
				GASSERT( 0 && "ERROR : DxGameStage::MsgProcess, NET_MSG_APEX_ANSER" );
				break;
			}
		}
		break;
	
	case NET_MSG_CHAT_CTRL_FB:			// �����ڿ� ä�ø޽���
		{//edit for add staffchat
			NET_CHAT_CTRL_FB* pNc = (NET_CHAT_CTRL_FB*) nmg;
			DisplayChatMessage ( pNc->emType, NULL, pNc->szChatMsg, pNc->sITEMLINK );
		}
		break;
	case NET_MSG_CHAT_FB: // ä�ø޽���  
		{
			NET_CHAT_FB* pNc = (NET_CHAT_FB*) nmg;
			DisplayChatMessage ( pNc->emType, pNc->szName, pNc->szChatMsg, pNc->sITEMLINK );
		}
		break;

	case NET_MSG_CHAT_PRIVATE_FAIL: 
		{
			GLMSG::SNETPC_CHAT_PRIVATE_FAIL *pNetMsg = (GLMSG::SNETPC_CHAT_PRIVATE_FAIL *) nmg;
			PrintConsoleText ( ID2GAMEINTEXT("CHAT_PRIVATE_FAIL"), pNetMsg->szName );
		}
		break;
	
	case NET_MSG_CHAT_BLOCK: 
		{
#ifndef CH_PARAM
			GLMSG::SNETPC_CHAT_BLOCK *pNetMsg = (GLMSG::SNETPC_CHAT_BLOCK *) nmg;
			CTime cTime( pNetMsg->tChatBlock ); // �������� 0�� �Ѿ���� �ʴ´ٰ� �����Ѵ�.
			PrintConsoleText( "[%s]:%02d/%02d/%02d %02d:%02d%s", 
															ID2GAMEWORD("NOTIFY_NAME"),
															cTime.GetYear()%2000, cTime.GetMonth(), cTime.GetDay(),
															cTime.GetHour(), cTime.GetMinute(),
															ID2GAMEINTEXT("CHAT_BLOCK_MSG") );
#endif
		}
		break;

	case NET_MSG_GCTRL_DROP_OUT_FORCED:
		{
			GLMSG::SNET_DROP_OUT_FORCED *pNetMsg = (GLMSG::SNET_DROP_OUT_FORCED *) msgBuffer;

			//CDebugSet::ToLogFile ( "error : overlapping login " );

			if ( pNetMsg->emForced==EMDROPOUT_REQLOGIN )
			{
				DoModal ( ID2GAMEINTEXT ("PLAYINTERFACE_8"), MODAL_INFOMATION, OK, MODAL_CLOSEGAME );
			}
			else
			{
				DoModal ( ID2GAMEINTEXT ("PLAYINTERFACE_9"), MODAL_INFOMATION, OK, MODAL_CLOSEGAME );
			}

			DxGlobalStage::GetInstance().GetNetClient()->CloseConnect();
		}
		break;

	case NET_MSG_GCTRL_REQ_TAKE_FB:  
		{
			GLMSG::SNETPC_REQ_TAKE_FB *pNetMsg = (GLMSG::SNETPC_REQ_TAKE_FB *)nmg;

			switch ( pNetMsg->emTakeFB )
			{
			case EMTAKE_FB_OFF:
				//pNetMsg->emCrow �����. (GUI)
				switch ( pNetMsg->emCrow)
				{
				case CROW_ITEM:
					{
						//PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PLAYINTERFACE_10") );
					}
					break;
				case CROW_MONEY:
					{
						//PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PLAYINTERFACE_11") );
					}
					break;
				}
				break;
			case EMTAKE_FB_RESERVED:
				{
					PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PLAYINTERFACE_12") );
				}
				break;

			case EMTAKE_FB_INVEN_ERROR:
				{
					PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PLAYINTERFACE_13") );
				}
				break;
			case EMTAKE_FB_TOO_EVENT_ITEM:
				{
					PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PLAYINTERFACE_14") );
				}
				break;
			case EMTAKE_FB_DISTANCE:
				{
//					PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMTAKE_FB_DISTANCE") );
				}
				break;
			};
		}
		break;

	case NET_MSG_GCTRL_PARTY_LURE_TAR:  
		{
			GLMSG::SNET_PARTY_LURE_TAR *pNetMsg = (GLMSG::SNET_PARTY_LURE_TAR *) nmg;
			
			PLANDMANCLIENT pLand = GLGaeaClient::GetInstance().GetActiveMap();
			if( !pLand ) break;

			const CString& strName = pNetMsg->szName;

			if ( IsVisibleGroup ( PARTY_MODAL_WINDOW ) )
			{
				//	Note : ���� ��Ƽ��û�� ���� �亯�� ���� ���� ���
				//		   ���� ������ ������ ��� �����Ѵ�.
				GLMSG::SNET_PARTY_LURE_TAR_ANS NetMsg;
				NetMsg.dwMasterID = pNetMsg->dwMasterID;
				NetMsg.dwPartyID = pNetMsg->dwPartyID;
				NetMsg.emAnswer = EMPARTY_LURE_REFUSE;
				NETSEND ( &NetMsg );
			}

			//	Note : ��Ƽ �ڵ� �ź�.
			//
			if ( RANPARAM::bDIS_PARTY )
			{
				//	��Ƽ ���� ó��
				GLMSG::SNET_PARTY_LURE_TAR_ANS NetMsg;
				NetMsg.dwMasterID = pNetMsg->dwMasterID;
				NetMsg.dwPartyID = pNetMsg->dwPartyID;
				NetMsg.emAnswer = EMPARTY_LURE_REFUSE;
				NETSEND ( &NetMsg );

				PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("PARTY_LURE_REFUSE_AUTO"), strName );
				break;
			}

			CNotifyRequestWindow::REQUEST_LIST rList;
			rList.dwGaeaID = pNetMsg->dwMasterID;
			rList.wType = 1;
			rList.strName = strName;
			rList.dwPartyID = pNetMsg->dwPartyID;
			rList.dwMasterID = pNetMsg->dwMasterID;
			rList.sPartyOption = pNetMsg->sOption;
			
			const int nYear = (int) GLGaeaClient::GetInstance().GetCurrentTime().GetYear ();
			const int nMonth = (int) GLGaeaClient::GetInstance().GetCurrentTime().GetMonth ();
			const int nDay = (int) GLGaeaClient::GetInstance().GetCurrentTime().GetDay ();

			CNotifyRequestWindow::REQUEST_LIST_DATE_DATA rListDate;
			rListDate.SET_DATE( nYear, nMonth, nDay );
			rList.reqDate = rListDate;

			GetNotifyRequestWindow()->AddRequest( GetNotifyRequestWindow()->GetRequestSize() + 1, rList );
			GetNotifyRequest()->SET_ALARM();
		}
		break;

	case NET_MSG_GCTRL_TRADE_TAR: 
		{
			GLMSG::SNET_TRADE_TAR *pNetMsg = (GLMSG::SNET_TRADE_TAR *) nmg;
			PLANDMANCLIENT pLand = GLGaeaClient::GetInstance().GetActiveMap();
			if( !pLand ) break;

			PGLCHARCLIENT pChar = pLand->GetChar( pNetMsg->dwMasterID );
			if( !pChar ) break;
			
			CString strName( pChar->GetName() );

			//	Note : �ŷ� �ڵ� �ź�.
			//
			bool bPMARKETOPEN = GLGaeaClient::GetInstance().GetCharacter()->m_sPMarket.IsOpen();
			if ( RANPARAM::bDIS_TRADE || bPMARKETOPEN )
			{
				//	P2P �ŷ� ���� ó��
				GLMSG::SNET_TRADE_TAR_ANS NetMsg;
				NetMsg.dwMasterID = pNetMsg->dwMasterID;
				NetMsg.emAns = EMTRADE_CANCEL;
				NETSEND ( &NetMsg );

				PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("TRADE_FB_CANCEL_AUTO"), strName );
				break;
			}

			CString strTemp;
			strTemp = MakeString ( ID2GAMEINTEXT("TRADE_TAR"), strName );
			DoModal ( strTemp, MODAL_QUESTION, OKCANCEL, MODAL_TRADE_REQ );

			GetTradeWindow()->SetTradeInfo ( strName, pNetMsg->dwMasterID );
		}
		break;

	case NET_MSG_GCTRL_CONFRONT_TAR:  //envited to duel ,recieve net message for invitation , code to show modal for invitation
		{
			GLMSG::SNETPC_REQ_CONFRONT_TAR *pNetMsg = (GLMSG::SNETPC_REQ_CONFRONT_TAR *)nmg;
			
			PGLCHARCLIENT pChar = GLGaeaClient::GetInstance().GetChar ( pNetMsg->dwID );
			if( !pChar ) break;
			
			if ( pNetMsg->sOption.emBet != 0 )
			{
				if ( !RANPARAM::bAllowBet )
				{
					GLMSG::SNETPC_REQ_CONFRONT_ANS NetMsgAns;
					NetMsgAns.emTYPE = pNetMsg->emTYPE;
					NetMsgAns.dwID = pNetMsg->dwID;
					NetMsgAns.emFB = EMCONFRONT_NO_BET;
					NetMsgAns.sOption = pNetMsg->sOption;
					NETSEND ( &NetMsgAns );

					PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMCONFRONT_NO_BET_AUTO"), pChar->GetName() );
					break;
				}

				if ( pNetMsg->emTYPE != EMCONFT_ONE )
				{
					GLMSG::SNETPC_REQ_CONFRONT_ANS NetMsgAns;
					NetMsgAns.emTYPE = pNetMsg->emTYPE;
					NetMsgAns.dwID = pNetMsg->dwID;
					NetMsgAns.emFB = EMCONFRONT_CANT_BET;
					NetMsgAns.sOption = pNetMsg->sOption;
					NETSEND ( &NetMsgAns );

					PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMCONFRONT_CANT_BET_AUTO"), pChar->GetName() );
					break;
				}

				LONGLONG llMoney = GLGaeaClient::GetInstance().GetCharacter ()->m_lnMoney;
				LONGLONG llVoteP = GLGaeaClient::GetInstance().GetCharacter ()->m_lnVoteP;
				LONGLONG llPremP = GLGaeaClient::GetInstance().GetCharacter ()->m_lnPremP;

				if ( pNetMsg->sOption.emBet == 1 )
				{
					if ( llMoney < pNetMsg->sOption.llBet )
					{
						GLMSG::SNETPC_REQ_CONFRONT_ANS NetMsgAns;
						NetMsgAns.emTYPE = pNetMsg->emTYPE;
						NetMsgAns.dwID = pNetMsg->dwID;
						NetMsgAns.emFB = EMCONFRONT_CANT_BET_MONEY;
						NetMsgAns.sOption = pNetMsg->sOption;
						NETSEND ( &NetMsgAns );

						PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMCONFRONT_CANT_BET_MONEY_AUTO"), pChar->GetName() );
						break;
					}
				}
				else if ( pNetMsg->sOption.emBet == 2 )
				{
					if ( llVoteP < pNetMsg->sOption.llBet )
					{
						GLMSG::SNETPC_REQ_CONFRONT_ANS NetMsgAns;
						NetMsgAns.emTYPE = pNetMsg->emTYPE;
						NetMsgAns.dwID = pNetMsg->dwID;
						NetMsgAns.emFB = EMCONFRONT_CANT_BET_VOTEP;
						NetMsgAns.sOption = pNetMsg->sOption;
						NETSEND ( &NetMsgAns );

						PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMCONFRONT_CANT_BET_VOTEP_AUTO"), pChar->GetName() );
						break;
					}
				}
				else if ( pNetMsg->sOption.emBet == 3 )
				{
					if ( llPremP < pNetMsg->sOption.llBet )
					{
						GLMSG::SNETPC_REQ_CONFRONT_ANS NetMsgAns;
						NetMsgAns.emTYPE = pNetMsg->emTYPE;
						NetMsgAns.dwID = pNetMsg->dwID;
						NetMsgAns.emFB = EMCONFRONT_CANT_BET_PREMP;
						NetMsgAns.sOption = pNetMsg->sOption;
						NETSEND ( &NetMsgAns );

						PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMCONFRONT_CANT_BET_PREMP_AUTO"), pChar->GetName() );
						break;
					}
				}
			}

			if ( IsVisibleGroup ( CONFT_MODAL_WINDOW ) )
			{
				//	Note : ���� ��ý�û�� ���� �亯�� ���� ���� ���
				//		   ���� ������ ������ ��� �����Ѵ�.
				GLMSG::SNETPC_REQ_CONFRONT_ANS NetMsgAns;
				NetMsgAns.emTYPE = pNetMsg->emTYPE;
				NetMsgAns.dwID = pNetMsg->dwID;
				NetMsgAns.emFB = EMCONFRONT_REFUSE;
				NetMsgAns.sOption = pNetMsg->sOption;
				NETSEND ( &NetMsgAns );
			}
			
			//	Note : ��� �ڵ� �ź�.
			//
			if ( RANPARAM::bDIS_CONFT )
			{
				//	Note : ��� �����ÿ� �޽��� ����.
				GLMSG::SNETPC_REQ_CONFRONT_ANS NetMsgAns;
				NetMsgAns.emTYPE = pNetMsg->emTYPE;
				NetMsgAns.dwID = pNetMsg->dwID;
				NetMsgAns.emFB = EMCONFRONT_REFUSE;
				NetMsgAns.sOption = pNetMsg->sOption;
				NETSEND ( &NetMsgAns );

				PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMCONFRONT_REFUSE_AUTO"), pChar->GetName() );
				break;
			}

			const char *szReqName = pChar->GetName();
			m_ConflictReqID = pNetMsg->dwID;
			m_emConflictReqType = pNetMsg->emTYPE;
			m_sReqConflictOption = pNetMsg->sOption;
			UIGUID MODAL_CONFT_TYPE = NO_ID;

			CString strTemp;
			switch (pNetMsg->emTYPE)
			{
			case EMCONFT_NONE:
//				GASSERT ( 0 && "����" );
				return ;
				break;
			case EMCONFT_ONE:
				strTemp = MakeString ( ID2GAMEINTEXT("CONFLICT_ONE_QUESTION_TAR"), szReqName );
				MODAL_CONFT_TYPE = MODAL_CONFLICT_ONE_TAR;
				break;
			case EMCONFT_PARTY:
				strTemp = MakeString ( ID2GAMEINTEXT("CONFLICT_PARTY_QUESTION_TAR"), szReqName );
				MODAL_CONFT_TYPE = MODAL_CONFLICT_PARTY_TAR;
				break;
			case EMCONFT_GUILD:
				strTemp = MakeString ( ID2GAMEINTEXT("CONFLICT_CLUB_QUESTION_TAR"), szReqName );
				MODAL_CONFT_TYPE = MODAL_CONFLICT_GUILD_TAR;
				break;
			};

			CString strMyName = GLGaeaClient::GetInstance().GetCharacter ()->GetName ();
			DoConftModal ( strTemp, MODAL_CONFT_TYPE, szReqName, strMyName );
		}
		break;
	case NET_MSG_SERVER_GENERALCHAT: 
		{
			GLMSG::SNET_SERVER_GENERALCHAT* pNetMsg = (GLMSG::SNET_SERVER_GENERALCHAT*) nmg;
			DisplayChatMessage ( CHAT_TYPE_CTRL_GLOBAL, "", pNetMsg->szTEXT );
		}
		break;

//		NOTE
//			CLOSING MESSAGE ó��
//	case BLAH_BLAH:
//		{
//			SetWaitServerDialogueClose ();
//		}
//		break;
	};
}

void CInnerInterface::ModalMsgProcess ( UIGUID nCallerID, DWORD dwMsg )
{
	switch ( nCallerID )
	{
	case MODAL_CLOSEGAME:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				WAITSERVER_DIALOGUE_OPEN ( ID2GAMEINTEXT("WAITSERVER_MESSAGE"), WAITSERVER_CLOSEGAME );
			}
		}
		break;
	
	case MODAL_TEST_MODAL:
		break;	

	case MODAL_INVENTORY_MONEY_TO_FIELD:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
				if ( pCharacter )
				{
					CString strTemp = GetModalWindow()->GetEditString ();
					LONGLONG lnDropMoney = _ttoi64 ( strTemp.GetString () );
					LONGLONG lnMoney = pCharacter->m_lnMoney;
					if ( lnMoney < lnDropMoney ) lnDropMoney = lnMoney;

					pCharacter->ReqMoneyToField ( lnDropMoney );
				}
			}
		}
		break;

	case MODAL_SKILLFACT_ASK_RESET:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				SNATIVEID sSKILLID = GetSKEFFID();
				for ( int i = 0; i < SKILLFACT_SIZE; i++ )
				{
					const SSKILLFACT& sSKILLFACT = GLGaeaClient::GetInstance().GetCharacter()->m_sSKILLFACT[i];
					if ( sSKILLID == sSKILLFACT.sNATIVEID ) GLGaeaClient::GetInstance().GetCharacter()->ReqDisableSkill( i, sSKILLID );
				}

			}
		}
		break;

	case MODAL_INVENTORY_ITEM_TO_FIELD:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
				if ( pCharacter )
				{
					D3DXVECTOR3 vTargetPt, vFromPt;
					vFromPt = DxViewPort::GetInstance().GetFromPt ();
					BOOL bOk = DxViewPort::GetInstance().GetMouseTargetPosWnd ( vTargetPt );
					if ( !bOk )	return;

					D3DXVECTOR3 vCollisionPos;
					DWORD dwCollisionID;
					BOOL bCollision;
					GLGaeaClient::GetInstance().GetActiveMap()->GetNaviMesh()->IsCollision
					(
						vFromPt,
						vTargetPt,
						vCollisionPos,
						dwCollisionID,
						bCollision
					);

					if ( bCollision )
					{
						D3DXVECTOR3 vDir = vCollisionPos - pCharacter->GetPosition();
						vDir.y = 0.0f;

						D3DXVec3Normalize ( &vDir, &vDir );
						D3DXVECTOR3 vDropPos = pCharacter->GetPosition() + vDir*float(GLCONST_CHAR::wBODYRADIUS+12);

						GLGaeaClient::GetInstance().GetActiveMap()->GetNaviMesh()->IsCollision
						(
							vDropPos + D3DXVECTOR3(0,+5,0),
							vDropPos + D3DXVECTOR3(0,-5,0),
							vCollisionPos,
							dwCollisionID,
							bCollision
						);

						vDropPos = pCharacter->GetPosition();
						if ( bCollision )		vDropPos = vCollisionPos;
						pCharacter->ReqHoldToField ( vDropPos );
					}
				}
			}
		}
		break;
	case MODAL_PARTY_TAR:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				//	��Ƽ ���� ó��
				GLMSG::SNET_PARTY_LURE_TAR_ANS NetMsg;
				NetMsg.dwMasterID = GetPartyWindow()->GetMasterID ();
				NetMsg.dwPartyID = GetPartyWindow()->GetPartyID ();
				NetMsg.emAnswer = EMPARTY_LURE_OK;
				NetMsg.sOption = m_sReqPartyOption;
				NETSEND ( &NetMsg );
			}
			else if ( dwMsg & UIMSG_MODAL_CANCEL )
			{
				//	��Ƽ ���� ó��
				GLMSG::SNET_PARTY_LURE_TAR_ANS NetMsg;
				NetMsg.dwMasterID = GetPartyWindow()->GetMasterID ();
				NetMsg.dwPartyID = GetPartyWindow()->GetPartyID ();
				NetMsg.emAnswer = EMPARTY_LURE_REFUSE;
				NETSEND ( &NetMsg );
			}
		}
		break;

	case MODAL_PARTY_REQ:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
			
				if ( m_pPtoPWindow )
				{
					DWORD dwGaeaID = m_pPtoPWindow->GetPlayerID ();

					SPARTY_OPT sOption;
					sOption.emGET_ITEM = (EMPARTY_ITEM_OPT)RANPARAM::dwPARTY_GET_ITEM;
					sOption.emGET_MONEY = (EMPARTY_MONEY_OPT)RANPARAM::dwPARTY_GET_MONEY;

					//	Note : ������ ��������� ��Ƽ ��û.
					//
					GLMSG::SNET_PARTY_LURE NetMsg;
					NetMsg.dwGaeaID = dwGaeaID;
					NetMsg.sOption = sOption;
					NETSEND ( (NET_MSG_GENERIC*) &NetMsg );
				}
				else if ( m_pClubWindow )
				{
					const DWORD dwGaeaID = m_pClubWindow->GetClubMemberID ();

					SPARTY_OPT sOption;
					sOption.emGET_ITEM = (EMPARTY_ITEM_OPT)RANPARAM::dwPARTY_GET_ITEM;
					sOption.emGET_MONEY = (EMPARTY_MONEY_OPT)RANPARAM::dwPARTY_GET_MONEY;

					//	Note : ������ ��������� ��Ƽ ��û.
					//
					GLMSG::SNET_PARTY_LURE NetMsg;
					NetMsg.dwGaeaID = dwGaeaID;
					NetMsg.sOption = sOption;
					NETSEND ( (NET_MSG_GENERIC*) &NetMsg );
				}
			}
		}
		break;
	
	case MODAL_TRADE_REQ:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				//	P2P �ŷ� ���� ó��
				GLMSG::SNET_TRADE_TAR_ANS NetMsg;
				NetMsg.dwMasterID = GetTradeWindow()->GetMasterID ();
				NetMsg.emAns = EMTRADE_OK;
				NETSEND ( &NetMsg );
			}
			else if ( dwMsg & UIMSG_MODAL_CANCEL )
			{
				//	P2P �ŷ� ���� ó��
				GLMSG::SNET_TRADE_TAR_ANS NetMsg;
				NetMsg.dwMasterID = GetTradeWindow()->GetMasterID ();
				NetMsg.emAns = EMTRADE_CANCEL;
				NETSEND ( &NetMsg );
			}
		}
		break;
	
	case MODAL_REBIRTH:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				if ( GLGaeaClient::GetInstance().GetCharacter()->IsDie () )
				{
					// ��Ȱ�ϱ�
					GLGaeaClient::GetInstance().GetCharacter()->ReqReBirth();
					
					// ������ â�� �ݱ�
					CloseAllWindow ();				
				}
			}

			if ( dwMsg & UIMSG_MODAL_CANCEL )
			{
				if ( GLGaeaClient::GetInstance().GetCharacter()->IsDie () )
				{
					if ( m_pRebirthDialogue->IsRecoveryAction() )
					{	
						GLGaeaClient::GetInstance().GetCharacter()->ReqGetReExp ();
					}
					else
					{
						// ��ȥ�� ���
						GLGaeaClient::GetInstance().GetCharacter()->ReqReGenRevive ();
						// ������ â�� �ݱ�
						CloseAllWindow ();						
					}
				}
			}
		}
		break;
	case MODAL_RECOVERY_EXP:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				if ( GLGaeaClient::GetInstance().GetCharacter()->IsDie () )
				{
					GLGaeaClient::GetInstance().GetCharacter()->ReqRecoveryExp();				
				}
			}
			if ( dwMsg & UIMSG_MODAL_CANCEL )
			{
				if ( GLGaeaClient::GetInstance().GetCharacter()->IsDie () )
				{
					// ��Ȱ�ϱ�
					GLGaeaClient::GetInstance().GetCharacter()->ReqReBirth();
					
					// ������ â�� �ݱ�
					CloseAllWindow ();				
				}
			}
		}
		break;
	case MODAL_RECOVERY_NPC_EXP:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				int NpcID = 0;
				int DummyData = 0;
				GetModalWindow()->GetModalData( &NpcID, &DummyData );

				GLGaeaClient::GetInstance().GetCharacter()->ReqRecoveryExpNpc( NpcID );				
			}			
		}
		break;


	//case MODAL_DROPMONEY:
	//	{
	//		if ( dwMsg & UIMSG_MODAL_OK )
	//		{
	//			GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
	//			if ( pCharacter )
	//			{
	//				CString strTemp = GetModalWindow()->GetEditString ();
	//				LONGLONG lnDropMoney = _ttoi64 ( strTemp.GetString () );
	//				LONGLONG lnMoney = pCharacter->m_lnMoney;
	//				if ( lnMoney < lnDropMoney ) lnDropMoney = lnMoney;

	//				pCharacter->ReqMoneyToField ( lnDropMoney );
	//			}
	//		}
	//	}
	//	break;

	case MODAL_TRADEMONEY:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				CString strTemp = GetModalWindow()->GetEditString ();
				LONGLONG lnMoney = _ttoi64 ( strTemp.GetString () );
				GLTradeClient::GetInstance().ReqTradeMoney ( lnMoney );
			}
		}
		break;

	case MODAL_SAVEMONEY:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
				if ( pCharacter )
				{
					CString strTemp = GetModalWindow()->GetEditString ();
					LONGLONG lnSaveMoney = _ttoi64 ( strTemp.GetString () );
					LONGLONG lnMoney = pCharacter->m_lnMoney;
					if ( lnMoney < lnSaveMoney ) lnSaveMoney = lnMoney;

					pCharacter->ReqStorageSaveMoney ( lnSaveMoney );
				}
			}
		}
		break;

	case MODAL_CLUB_SAVEMONEY:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
				if ( pCharacter )
				{
					CString strTemp = GetModalWindow()->GetEditString ();
					LONGLONG lnSaveMoney = _ttoi64 ( strTemp.GetString () );
					LONGLONG lnMoney = pCharacter->m_lnMoney;
					if ( lnMoney < lnSaveMoney ) lnSaveMoney = lnMoney;

					pCharacter->ReqClubStorageSaveMoney ( lnSaveMoney );
				}
			}
		}
		break;

	case MODAL_LOADMONEY:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
				if ( pCharacter )
				{
					CString strTemp = GetModalWindow()->GetEditString ();
					LONGLONG lnLoadMoney = _ttoi64 ( strTemp.GetString () );
					LONGLONG lnMoney = pCharacter->m_lnStorageMoney;
					if ( lnMoney < lnLoadMoney ) lnLoadMoney = lnMoney;

					pCharacter->ReqStorageDrawMoney ( lnLoadMoney );
				}
			}
		}
		break;

	case MODAL_CLUB_LOADMONEY:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
				if ( pCharacter )
				{
					CString strTemp = GetModalWindow()->GetEditString ();
					LONGLONG lnLoadMoney = _ttoi64 ( strTemp.GetString () );
					LONGLONG lnMoney = pCharacter->m_sCLUB.m_lnStorageMoney;
					if ( lnMoney < lnLoadMoney ) lnLoadMoney = lnMoney;

					pCharacter->ReqClubStorageDrawMoney ( lnLoadMoney );
				}
			}
		}
		break;

	case MODAL_SPLIT_ITEM:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
				if ( pCharacter )
				{
					CString strTemp = GetModalWindow()->GetEditString ();
					WORD wSplitNum = (WORD) atoi ( strTemp.GetString () );

					WORD wPosX, wPosY;
					GetInventoryWindow ()->GetSplitPos ( &wPosX, &wPosY );
					pCharacter->ReqInvenSplit ( wPosX, wPosY, wSplitNum );
				}
			}
		}
		break;

	case MODAL_STORAGE_SPLIT_ITEM:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
				if ( pCharacter )
				{
					CString strTemp = GetModalWindow()->GetEditString ();
					WORD wSplitNum = (WORD) atoi ( strTemp.GetString () );

					WORD wPosX, wPosY;
					int nChannel = m_pStorageWindow->GetChannel ();
					m_pStorageWindow->GetSplitPos ( &wPosX, &wPosY );					
					pCharacter->ReqStorageSplit ( nChannel, wPosX, wPosY, wSplitNum );
				}
			}
		}
		break;

	case MODAL_CLUB_STORAGE_SPLIT_ITEM:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
				if ( pCharacter )
				{
					CString strTemp = GetModalWindow()->GetEditString ();
					WORD wSplitNum = (WORD) atoi ( strTemp.GetString () );

					WORD wPosX, wPosY;
					int nChannel = m_pClubStorageWindow->GetChannel ();
					m_pClubStorageWindow->GetSplitPos ( &wPosX, &wPosY );
					pCharacter->ReqClubStorageSplit ( nChannel, wPosX, wPosY, wSplitNum );
				}
			}
		}
		break;

	case MODAL_CONFLICT_ONE_TAR:
	case MODAL_CONFLICT_PARTY_TAR:
	case MODAL_CONFLICT_GUILD_TAR: //answer to duel invitation
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				//	Note : ��� �����ÿ� �޽��� ����.
				GLMSG::SNETPC_REQ_CONFRONT_ANS NetMsgAns;
				NetMsgAns.emTYPE = m_emConflictReqType;
				NetMsgAns.dwID = m_ConflictReqID;
				NetMsgAns.emFB = EMCONFRONT_AGREE;
				NetMsgAns.sOption = m_sReqConflictOption;
				NETSEND ( &NetMsgAns );
			}
			else if ( dwMsg & UIMSG_MODAL_CANCEL )
			{
				//	Note : ��� �����ÿ� �޽��� ����.
				GLMSG::SNETPC_REQ_CONFRONT_ANS NetMsgAns;
				NetMsgAns.emTYPE = m_emConflictReqType;
				NetMsgAns.dwID = m_ConflictReqID;
				NetMsgAns.emFB = EMCONFRONT_REFUSE;
				NetMsgAns.sOption = m_sReqConflictOption;
				NETSEND ( &NetMsgAns );
			}
		}
		break;

	case MODAL_CONFLICT_ONE_REQ:
	case MODAL_CONFLICT_PARTY_REQ:
	case MODAL_CONFLICT_GUILD_REQ:
		{
			if ( dwMsg & UIMSG_MODAL_OK ) // player going to envite another player for duel
			{
				DWORD dwGaeaID = m_pPtoPWindow->GetPlayerID ();

				SCONFT_OPTION sOption;
				sOption.bBOUND = RANPARAM::bCONFT_BOUND!=FALSE;
				sOption.wRECOVER = RANPARAM::wCONFT_RECOVER;
				sOption.fHP_RATE = RANPARAM::fCONFT_HP_RATE;
				sOption.fTAR_HP_RATE = RANPARAM::fCONFT_TAR_HP_RATE;

				sOption.llBet = RANPARAM::llConft_Value;//add duel bet
				sOption.emBet = ( EMCONFT_BET ) RANPARAM::wBetType;//add duel bet

				GLGaeaClient::GetInstance().GetCharacter()->ReqConflict ( dwGaeaID, sOption );
			}
		}
		break;

	case MODAL_PILE_ITEM:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				CString strTemp = GetModalWindow()->GetEditString ();
				DWORD dwCount = (DWORD) atoi ( strTemp.GetString () );
				if ( dwCount )
				{
					CDialogueWindow* m_pDialogueWindow = GetDialogueWindow ();
					SNATIVEID sMarketID = m_pDialogueWindow->GetMarketID ();
					int	nPageIndex = m_pDialogueWindow->GetPageIndex ();

					int nPosX = nOUTOFRANGE;
					int nPosY = nOUTOFRANGE;
					m_pDialogueWindow->GetItemIndex ( &nPosX, &nPosY );
					if ( nPosX < 0 || nPosY < 0 ) return ;

					LONGLONG lnMoney = GLGaeaClient::GetInstance().GetCharacter ()->m_lnMoney;
					//add money 2
					LONGLONG lnMoney2 = GLGaeaClient::GetInstance().GetCharacter ()->m_lnVoteP;
					//add money 3
					LONGLONG lnMoney3 = GLGaeaClient::GetInstance().GetCharacter ()->m_lnPremP;
					//add contributionpoint by CNDev
					LONGLONG lnContribP = GLGaeaClient::GetInstance().GetCharacter ()->m_lnContribP;
					LONGLONG lnSaldo = GLGaeaClient::GetInstance().GetCharacter ()->m_lnSaldo;

					SINVENITEM& sInvenItem = m_pDialogueWindow->GetItem ( nPosX, nPosY );
					SITEM* pItem = GLItemMan::GetInstance().GetItem ( sInvenItem.sItemCustom.sNativeID );
					if ( !pItem ) return ;

					float fSHOP_RATE = GLGaeaClient::GetInstance().GetCharacter()->GetBuyRate();
					float fSHOP_RATE_C = fSHOP_RATE * 0.01f;
					DWORD dwPrice = static_cast<DWORD>( pItem->sBasicOp.dwBuyPrice * fSHOP_RATE_C );

					// add money 2
					//compare price
					DWORD dwMaxCount = (DWORD)(lnMoney / (LONGLONG)dwPrice);

					if ( pItem->sBasicOp.emLevel == LEVEL_VOTE_ITEM )
					{
						dwMaxCount = (DWORD)(lnMoney2 / (LONGLONG)dwPrice);
						if ( dwMaxCount < 1 )
						{					
							PrintMsgTextDlg ( NS_UITEXTCOLOR::ORANGE, ID2GAMEINTEXT("BUYITEM_NOMONEY_2") );
							return ;
						}
					}
					//add money 3
					else if ( pItem->sBasicOp.emLevel == LEVEL_PREMIUM_ITEM )
					{
						dwMaxCount = (DWORD)(lnMoney3 / (LONGLONG)dwPrice);
						if ( dwMaxCount < 1 )
						{					
							PrintMsgTextDlg ( NS_UITEXTCOLOR::ORANGE, ID2GAMEINTEXT("BUYITEM_NOMONEY_3") );
							return ;
						}
					}
					//add contributionpoint by CNDev
					else if ( pItem->sBasicOp.emLevel == LEVEL_CONTRIBUTION_ITEM )
					{
						dwMaxCount = (DWORD)(lnContribP / (LONGLONG)dwPrice);
						if ( dwMaxCount <1 )
						{
							PrintMsgTextDlg ( NS_UITEXTCOLOR::ORANGE, ID2GAMEINTEXT("BUYITEM_NOMONEY_4") );
							return;
						}
					}
					else
					{
						dwMaxCount = (DWORD)(lnMoney / (LONGLONG)dwPrice);
						if ( dwMaxCount < 1 )
						{					
							PrintMsgTextDlg ( NS_UITEXTCOLOR::ORANGE, ID2GAMEINTEXT("BUYITEM_NOMONEY") );
							return ;
						}
					}

					//DWORD dwMaxCount = (DWORD)(lnMoney / (LONGLONG)dwPrice);

					//if ( dwMaxCount < 1 )
					//{					
					//	PrintMsgTextDlg ( NS_UITEXTCOLOR::ORANGE, ID2GAMEINTEXT("BUYITEM_NOMONEY") );
					//	return ;
					//}
					//	��ư ��������´�� �ֱ�
					//	�ִ� ������ ���
					if ( dwMaxCount < dwCount )
					{												
						PrintMsgText ( NS_UITEXTCOLOR::ORANGE,
							MakeString ( ID2GAMEINTEXT("BUYITEM_MAXIMUM"),
							pItem->GetName(), dwMaxCount )
							);
						dwCount = dwMaxCount;
					}
					GLGaeaClient::GetInstance().GetCharacter()->ReqNpcTo ( sMarketID, nPageIndex, nPosX, nPosY, (WORD)dwCount );
				}
			}
		}
		break;

	case MODAL_SERVER_STAGE:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				WAITSERVER_DIALOGUE_OPEN ( ID2GAMEINTEXT("WAITSERVER_MESSAGE"), WAITSERVER_TOLOBY );
			}
		}
		break;

	case MODAL_BUY_ITEM:
	case MODAL_SELL_ITEM:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				CDialogueWindow* m_pDialogueWindow = GetDialogueWindow ();
				SNATIVEID sMarketID = m_pDialogueWindow->GetMarketID ();
				int	nPageIndex = m_pDialogueWindow->GetPageIndex ();

				int nPosX = nOUTOFRANGE;
				int nPosY = nOUTOFRANGE;
				if ( nCallerID == MODAL_BUY_ITEM )
				{
					m_pDialogueWindow->GetItemIndex ( &nPosX, &nPosY );
					if ( nPosX < 0 || nPosY < 0 ) return ;
				}
				CDebugSet::ToView ( 1, 3, "��� ������ : %d %d", nPosX, nPosY );

				GLGaeaClient::GetInstance().GetCharacter()->ReqNpcTo ( sMarketID, nPageIndex, nPosX, nPosY );
			}
		}
		break;

	case MODAL_QUEST_GIVEUP:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				//	����				
				DWORD dwQuestID = GetQuestWindow()->GetQuestID ();
				GLGaeaClient::GetInstance().GetCharacter ()->ReqQuestGiveUp ( dwQuestID );
			}
		}
		break;

	case MODAL_FRIEND_ADD:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				CString strName = m_pFriendWindow->GET_FRIEND_NAME ();
				GLFriendClient::GetInstance().ReqFriendAdd ( strName.GetString () );
			}
		}
		break;

	case MODAL_FRIEND_DEL:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				CString strName = m_pFriendWindow->GET_FRIEND_NAME ();
				GLFriendClient::GetInstance().ReqFriendDel ( strName.GetString () );
			}
		}
		break;

	case MODAL_FRIEND_MOVE:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{				
				SetFriendName ( m_pFriendWindow->GET_FRIEND_NAME () );
				WAITSERVER_DIALOGUE_OPEN ( ID2GAMEINTEXT("WAITSERVER_MESSAGE"), WAITSERVER_TOFRIEND, 10.0f );
			}
		}
		break;

	case MODAL_ADD_FRIEND_LURE_ANS:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				if ( !m_strFriendName.GetLength () )
				{
					GASSERT ( 0 && "�̸��� ��ϵ��� �ʾҽ��ϴ�." );
					return ;
				}

				GLFriendClient::GetInstance ().ReqFriendLureAns ( m_strFriendName.GetString(), true );
			}
			else if ( UIMSG_MODAL_CANCEL & dwMsg )
			{
				if ( !m_strFriendName.GetLength () )
				{
					GASSERT ( 0 && "�̸��� ��ϵ��� �ʾҽ��ϴ�." );
					return ;
				}

				GLFriendClient::GetInstance ().ReqFriendLureAns ( m_strFriendName.GetString(), false );
			}
		}
		break;

	case MODAL_DEFAULT_POS:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				MOVE_DEFAULT_POSITION ();
			}
		}
		break;

	case MODAL_LOTTO:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				CString strLotto = GetModalWindow()->GetEditString ();
				int nLength = strLotto.GetLength ();
				int nGAP = 10 - nLength;
				if ( nGAP )
				{
					strLotto += "0";
				}
				DxGlobalStage::GetInstance().NetSendEventLottery ( strLotto );
			}
		}
		break;

	case MODAL_STORAGE_CHARGE:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				WORD wPosX, wPosY, wSTORAGE;
				m_pStorageChargeCard->GetSelectedChargeCard ( wPosX, wPosY, wSTORAGE );

				GLGaeaClient::GetInstance().GetCharacter()->ReqStorageCard ( wPosX, wPosY, wSTORAGE );
			}
		}
		break;

	case MODAL_PRIVATE_MARKET_SELL:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				WORD wPosX, wPosY;
				m_pPrivateMarketSellWindow->GetItemIndex ( wPosX, wPosY );

				DWORD dwNum = m_pPrivateMarketSellWindow->GetItemCount ();
				ULONGLONG llMoney = m_pPrivateMarketSellWindow->GetItemCost ();

				if ( llMoney > GLCONST_CHAR::llVEND_PRICE_MAX ) llMoney = GLCONST_CHAR::llVEND_PRICE_MAX;
				
				LONGLONG dwMoney = (LONGLONG)llMoney;

				GLGaeaClient::GetInstance().GetCharacter()->ReqPMarketRegItem ( wPosX, wPosY, dwMoney, dwNum );
			}
		}
		break;

	case MODAL_PRIVATE_MARKET_BUY:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{	
				DWORD dwMarketID = m_pPrivateMarketWindow->GetPrivateMarketID ();
//				DWORD dwNum = static_cast<DWORD>(atoi(GetModalWindow()->GetEditString ()));

				int nPosX = nOUTOFRANGE;
				int nPosY = nOUTOFRANGE;

				GetModalWindow()->GetModalData ( &nPosX, &nPosY );
				if ( nPosX < 0 || nPosY < 0 ) return ;

				//CDebugSet::ToView ( 1, 3, "��� ������ : %d %d", nPosX, nPosY );

				GLGaeaClient::GetInstance().GetCharacter()->ReqPMarketBuy ( dwMarketID, nPosX, nPosY, 1 );
			}
		}
		break;

	case MODAL_PRIVATE_MARKET_PILE:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				CString strTemp = GetModalWindow()->GetEditString ();
				DWORD dwCount = (DWORD) atoi ( strTemp.GetString () );
				if ( dwCount )
				{
//					CMarketWindow* pMarketWindow = GetMarketWindow ();
					DWORD dwMarketID = m_pPrivateMarketWindow->GetPrivateMarketID ();
//					int	nPageIndex = pMarketWindow->GetPageIndex ();

					int nPosX = nOUTOFRANGE;
					int nPosY = nOUTOFRANGE;
					GetModalWindow()->GetModalData ( &nPosX, &nPosY );
					if ( nPosX < 0 || nPosY < 0 ) return ;

					LONGLONG lnMoney = GLGaeaClient::GetInstance().GetCharacter ()->m_lnMoney;
					SINVENITEM& sInvenItem = m_pPrivateMarketWindow->GetItem ( nPosX, nPosY );
					SITEM* pItem = GLItemMan::GetInstance().GetItem ( sInvenItem.sItemCustom.sNativeID );
					if ( !pItem ) return ;

					DWORD dwPrice = pItem->sBasicOp.dwBuyPrice;
//					float fSHOP_RATE = GLGaeaClient::GetInstance().GetCharacter()->GetBuyRate();					
//					DWORD dwPrice = static_cast<DWORD>(pItem->sBasicOp.dwBuyPrice*fSHOP_RATE/100.0f+0.1f);

					DWORD dwMaxCount = (DWORD)(lnMoney / (LONGLONG)dwPrice);
					if ( dwMaxCount < 1 )
					{					
						PrintMsgTextDlg ( NS_UITEXTCOLOR::ORANGE, ID2GAMEINTEXT("BUYITEM_NOMONEY") );
						return ;
					}
					//	��ư ��������´�� �ֱ�
					//	�ִ� ������ ���
					if ( dwMaxCount < dwCount )
					{												
						PrintMsgText ( NS_UITEXTCOLOR::ORANGE,
							MakeString ( ID2GAMEINTEXT("BUYITEM_MAXIMUM"),
								pItem->GetName(), dwMaxCount )
							);
						dwCount = dwMaxCount;
					}
					GLGaeaClient::GetInstance().GetCharacter()->ReqPMarketBuy ( dwMarketID, nPosX, nPosY, (WORD)dwCount );
				}
			}
		}
		break;

	case MODAL_PRIVATE_MARKET_QUIT:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				SetPrivateMarketClose ();
			}
		}
		break;

	case MODAL_CLUB_KICK:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				const DWORD dwGaeaID = m_pClubWindow->GetClubMemberID ();				
				GLGaeaClient::GetInstance().GetCharacter ()->ReqClubMemberDel ( dwGaeaID );
			}
		}
		break;

	case MODAL_CLUB_AUTHORITY:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				const DWORD dwGaeaID = m_pClubWindow->GetClubMemberID ();				
				GLGaeaClient::GetInstance().GetCharacter ()->ReqClubAuthority ( dwGaeaID );
			}
		}
		break;
	case MODAL_CLUB_AUTHORITY_ASK:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{						
				GLGaeaClient::GetInstance().GetCharacter()->ReqClubAuthorityAns( true );
			}
			else if ( UIMSG_MODAL_CANCEL & dwMsg )
			{
				GLGaeaClient::GetInstance().GetCharacter()->ReqClubAuthorityAns( false );
			}
		}
		break;
	case MODAL_CLUB_BREAKUP:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				GLGaeaClient::GetInstance().GetCharacter ()->ReqClubDissolution ();
			}
		}
		break;

	case MODAL_CLUB_CHANGE_MARK:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				CString strFileName = m_pModalWindow->GetEditString ();
				GLGaeaClient::GetInstance().GetCharacter ()->ReqClubMarkChange ( strFileName.GetString() );
			}
		}
		break;

	case MODAL_CLUB_NICK:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				CString strNickName = m_pModalWindow->GetEditString ();				
				GLGaeaClient::GetInstance().GetCharacter ()->ReqClubNick ( strNickName.GetString() );
			}
		}
		break;

	case MODAL_CLUB_JOIN_ASK:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{						
				GLGaeaClient::GetInstance().GetCharacter ()->ReqClubJoinAns ( m_dwClubMasterID, true );
			}
			else if ( UIMSG_MODAL_CANCEL & dwMsg )
			{
				GLGaeaClient::GetInstance().GetCharacter ()->ReqClubJoinAns ( m_dwClubMasterID, false );
			}
		}
		break;

	case MODAL_CLUB_JOIN:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				DWORD dwGaeaID = m_pPtoPWindow->GetPlayerID ();
				GLGaeaClient::GetInstance().GetCharacter ()->ReqClubJoin ( dwGaeaID );
			}
		}
		break;

	case MODAL_CLUB_BREAKUP_CANCEL:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				GLGaeaClient::GetInstance().GetCharacter ()->ReqClubDissolutionCancel ();
			}
		}
		break;

	case MODAL_CLUB_SELFQUIT:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				//	Ż�� Req
				GLGaeaClient::GetInstance().GetCharacter ()->ReqClubSecede ();
			}
		}
		break;

	case MODAL_SKILL_LEVEL_UP_CONFIRM:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				SNATIVEID sNativeID ( GetSkillUpID () );
				GLGaeaClient::GetInstance().GetCharacter()->ReqSkillUp ( sNativeID );
			}
		}
		break;

	case MODAL_COMMISSION:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				CString strCommission = m_pModalWindow->GetEditString ();
				float fCommission = (float) atof ( strCommission.GetString () );
				STARGETID sCrowID = m_pDialogueWindow->GetCrowID ();

				GLGaeaClient::GetInstance().GetCharacter()->ReqGuidCommission ( sCrowID.dwID, fCommission );
			}
		}
		break;

	case MODAL_FRIEND_BLOCK_ADD_N:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				CString strName = m_pFriendWindow->GET_FRIEND_NAME();
				GLFriendClient::GetInstance().ReqFriendBlock ( strName.GetString (), true );
			}
		}
		break;
        
	case MODAL_FRIEND_BLOCK_ADD_B:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				CString strName = m_pFriendWindow->GET_BLOCK_NAME();
				GLFriendClient::GetInstance().ReqFriendBlock ( strName.GetString (), true );
			}
		}
		break;

	case MODAL_FRIEND_BLOCK_DEL:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				CString strName = m_pFriendWindow->GET_BLOCK_NAME ();
				GLFriendClient::GetInstance().ReqFriendBlock ( strName.GetString (), false );
			}
		}
		break;

	case MODAL_UNION_JOIN_ASK:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{						
				GLGaeaClient::GetInstance().GetCharacter()->ReqClubAllianceAns( m_dwClubMasterID, true );
			}
			else if ( UIMSG_MODAL_CANCEL & dwMsg )
			{
				GLGaeaClient::GetInstance().GetCharacter()->ReqClubAllianceAns( m_dwClubMasterID, false );
			}
		}
		break;

	case MODAL_UNION_JOIN:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				DWORD dwGaeaID = m_pPtoPWindow->GetPlayerID ();
				GLGaeaClient::GetInstance().GetCharacter()->ReqClubAlliance( dwGaeaID );
			}
		}
		break;

	case MODAL_CLUB_BATTLE_ASK:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{						
				GLGaeaClient::GetInstance().GetCharacter()->ReqClubBattleAns( m_dwClubMasterID, true );
			}
			else if ( UIMSG_MODAL_CANCEL & dwMsg )
			{
				GLGaeaClient::GetInstance().GetCharacter()->ReqClubBattleAns( m_dwClubMasterID, false );
			}
		}
		break;

	case MODAL_ALLIANCE_BATTLE_ASK:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{						
				GLGaeaClient::GetInstance().GetCharacter()->ReqAllianceBattleAns( m_dwClubMasterID, true );
			}
			else if ( UIMSG_MODAL_CANCEL & dwMsg )
			{
				GLGaeaClient::GetInstance().GetCharacter()->ReqAllianceBattleAns( m_dwClubMasterID, false );
			}
		}
		break;

	case MODAL_CLUB_BATTLE_ARMISTICE:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				DWORD dwClubID = m_pClubWindow->GetBattleClubID ();
				GLGaeaClient::GetInstance().GetCharacter()->ReqClubBattleArmistice( dwClubID );
			}
		}
		break;

	case MODAL_ALLIANCE_BATTLE_ARMISTICE:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				DWORD dwClubID = m_pClubWindow->GetBattleClubID ();
				GLGaeaClient::GetInstance().GetCharacter()->ReqAllianceBattleArmistice( dwClubID );
			}
		}
		break;

	case MODAL_CLUB_BATTLE_ARMISTICE_ASK:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{						
				GLGaeaClient::GetInstance().GetCharacter()->ReqClubBattleArmisticeAns( m_dwClubMasterID, true );
			}
			else if ( UIMSG_MODAL_CANCEL & dwMsg )
			{
				GLGaeaClient::GetInstance().GetCharacter()->ReqClubBattleArmisticeAns( m_dwClubMasterID, false );
			}
		}
		break;

	case MODAL_ALLIANCE_BATTLE_ARMISTICE_ASK:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{						
				GLGaeaClient::GetInstance().GetCharacter()->ReqAllianceBattleArmisticeAns( m_dwClubMasterID, true );
			}
			else if ( UIMSG_MODAL_CANCEL & dwMsg )
			{
				GLGaeaClient::GetInstance().GetCharacter()->ReqAllianceBattleArmisticeAns( m_dwClubMasterID, false );
			}
		}
		break;

	case MODAL_CLUB_BATTLE_SUBMISSION:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				DWORD dwClubID = m_pClubWindow->GetBattleClubID ();
				GLGaeaClient::GetInstance().GetCharacter()->ReqClubBattleSubmission( dwClubID );
			}
		}
		break;

	case MODAL_ALLIANCE_BATTLE_SUBMISSION:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				DWORD dwClubID = m_pClubWindow->GetBattleClubID ();
				GLGaeaClient::GetInstance().GetCharacter()->ReqAllianceBattleSubmission( dwClubID );
			}
		}
		break;

	case MODAL_UNION_KICK:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				const DWORD dwClubID = m_pClubWindow->GetUnionMemberID();
				GLGaeaClient::GetInstance().GetCharacter()->ReqClubAllianceDel( dwClubID );
			}
		}
		break;

	case MODAL_UNION_LEAVE:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				//	Ż�� Req
				GLGaeaClient::GetInstance().GetCharacter ()->ReqClubAllianceSec();
			}
		}
		break;

	case MODAL_UNION_BREAKUP:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				GLGaeaClient::GetInstance().GetCharacter ()->ReqClubAllianceDis();
			}
		}
		break;

	case MODAL_CHAR_RENAME:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				CString strName = m_pModalWindow->GetEditString ();
				GLGaeaClient::GetInstance().GetCharacter()->ReqInvenRename( strName.GetString() );
			}
		}
		break;

	case MODAL_PET_RENAME:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				CString strName = m_pModalWindow->GetEditString ();
				GLGaeaClient::GetInstance().GetPetClient()->ReqRename( strName.GetString() );
			}
		}
		break;
	case MODAL_RESET_SKILLSTATS:
		{
			if ( UIMSG_MODAL_OK & dwMsg )
			{
				GLGaeaClient::GetInstance().GetCharacter()->ReqResetSkillStats( );
			}

		}
		break;
	case MODAL_ITEMREBUILD_MONEY:	// ITEMREBUILD_MARK
		{
			if( UIMSG_MODAL_OK & dwMsg )
			{
				CString strInputMoney = GetModalWindow()->GetEditString();
				LONGLONG i64InputMoney = _ttoi64( strInputMoney.GetString() );
				GLGaeaClient::GetInstance().GetCharacter()->ReqRebuildInputMoney( i64InputMoney );
			}
		}
		break;

	case MODAL_ITEMREBUILD_OKQUESTION: //sealed card 
		{
			if( UIMSG_MODAL_OK & dwMsg )
			{
				SITEMSEALDATA sData1 = GetItemRebuild()->m_SealData01;
				SITEMSEALDATA sData2 = GetItemRebuild()->m_SealData02;
				SITEMSEALDATA sData3 = GetItemRebuild()->m_SealData03;

				GLGaeaClient::GetInstance().GetCharacter()->ReqRebuildResult( sData1, sData2, sData3 );
//				CInnerInterface::GetInstance().CloseItemRebuildWindow();
			}
		}
		break;

	case MODAL_ITEMGARBAGE_OKQUESTION:
		{
			if( UIMSG_MODAL_OK & dwMsg )
			{
				GLGaeaClient::GetInstance().GetCharacter()->ReqGarbageResult();
			}
		}
		break;

	case MODAL_HAIRCOLOR_INFO:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				ShowGroupFocus( HAIRCOLORCARD_WINDOW );
			}
		}
		break;

	case MODAL_PETCOLOR_INFO:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				ShowGroupFocus( PETCOLORCARD_WINDOW );
			}
		}
		break;

	case MODAL_BIKECOLOR_INFO: //add bike color
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				ShowGroupFocus( BIKECOLORCARD_WINDOW );
			}
		}
		break;

	case MODAL_ITEMCOLOR_INFO: //add itemcolor
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				ShowGroupFocus( ITEMCOLORCARD_WINDOW );
			}
		}
		break;

	case MODAL_MOBILE_NUMBER:
		{
			if( UIMSG_MODAL_OK & dwMsg )
			{
				CString strInputNumber = GetModalWindow()->GetEditString();
				GLGaeaClient::GetInstance().GetCharacter()->ReqSetPhoneNumber( strInputNumber );
			}
		}
		break;

	case MODAL_GENDER_CHANGE:
		{
			if( UIMSG_MODAL_OK & dwMsg )
			{
				ShowGroupFocus( GENDER_CHANGE_WINDOW );
			}
		}
		break;
	case MODAL_GENDER_CHANGE_END:
		{
			if( UIMSG_MODAL_OK & dwMsg )
			{
				WAITSERVER_DIALOGUE_OPEN ( ID2GAMEINTEXT("WAITSERVER_MESSAGE"), WAITSERVER_TOLOBY );
			}
		}
		break;

	case MODAL_ITEMMIX_OKQUESTION:
		{
			if( UIMSG_MODAL_OK & dwMsg )
			{
				m_pItemMixWindow->SetAnimation();
			}
		}
		break;

	case MODAL_POINTSHOP_OKQUESTION:
		{
			if( UIMSG_MODAL_OK & dwMsg )
			{
				m_pPointShopWindow->SetAnimation();
			}
		}
		break;
	case MODAL_REBUILDITEM:
		{
			if ( dwMsg & UIMSG_MODAL_OK )
			{
				//GLGaeaClient::GetInstance().GetCharacter()->ReqRebuildMoveItem();
			}
		}
		break;
	}
}