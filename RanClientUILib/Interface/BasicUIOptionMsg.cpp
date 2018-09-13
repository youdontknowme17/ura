#include "StdAfx.h"
#include "BasicUIOption.h"
#include "../EngineUILib/GUInterface/BasicButton.h"
#include "../EngineUILib/GUInterface/BasicTextBox.h"
#include "DxParamSet.h"
#include "InnerInterface.h"
#include "ModalCallerID.h"
#include "ModalWindow.h"
#include "GameTextControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CBasicUIOption::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case HWOPTION_UI_SIMPLEHP_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				//DXPARAMSET::GetInstance().m_bSHOW_SIMPLEHP = !DXPARAMSET::GetInstance().m_bSHOW_SIMPLEHP;
				//LoadSimpleHP ();
				m_bSimpleHP = !m_bSimpleHP;
				m_pSimpleHPButton->SetFlip ( m_bSimpleHP );
			}
		}
		break;

	case HWOPTION_UI_MOVABLE_ON_CHAT_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				//RANPARAM::bMOVABLE_ON_CHAT = !RANPARAM::bMOVABLE_ON_CHAT;
				//LoadMovableOnChat ();
				m_bMovableOnChat = !m_bMovableOnChat;
				m_pMovableOnChatButton->SetFlip ( m_bMovableOnChat );
			}
		}
		break;
	case HWOPTION_UI_DEFAULTPOS_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				DoModal ( ID2GAMEINTEXT("DEFAULT_POSITION"), MODAL_QUESTION, YESNO, MODAL_DEFAULT_POS );					
			}
		}
		break;
	}
}