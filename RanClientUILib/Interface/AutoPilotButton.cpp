#include "StdAfx.h"
#include "../EngineUILib/GUInterface/BasicButton.h"
#include "GLGaeaClient.h"
#include "GLCharacter.h"
#include "InnerInterface.h"
#include "AutoPilotButton.h"
#include "GameTextControl.h"
#include "UITextControl.h"
#include "CharacterEnum.h"



CAutoPilotButton::CAutoPilotButton(void)
{
}

CAutoPilotButton::~CAutoPilotButton(void)
{
}
void CAutoPilotButton::CreateSubControl ()
{
	
	m_pAutoPilotButton = new CBasicButton;
	m_pAutoPilotButton->CreateSub ( this,"AUTOPILOT_BUTTON_IMAGE", UI_FLAG_DEFAULT, AUTOPILOT_BUTTON );
	m_pAutoPilotButton->SetUseGlobalAction ( TRUE );
	m_pAutoPilotButton->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pAutoPilotButton );
}


void CAutoPilotButton::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );	

	m_pAutoPilotButton->SetVisibleSingle ( TRUE );
}

void CAutoPilotButton::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case	AUTOPILOT_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO( "AutoPilot", NS_UITEXTCOLOR::WHITE  );				
			}

			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
			if ( CInnerInterface::GetInstance().IsVisibleGroup( AUTO_PILOT_WINDOW ) )		CInnerInterface::GetInstance().HideGroup ( AUTO_PILOT_WINDOW );
			else	CInnerInterface::GetInstance().ShowGroupFocus( AUTO_PILOT_WINDOW );
			}
		}
		break;
	}
}
