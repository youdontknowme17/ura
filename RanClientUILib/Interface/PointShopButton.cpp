#include "StdAfx.h"
#include "../EngineUILib/GUInterface/BasicButton.h"
#include "GLGaeaClient.h"
#include "GLCharacter.h"
#include "InnerInterface.h"
#include "PointShopButton.h"
#include "GameTextControl.h"
#include "UITextControl.h"
#include "CharacterEnum.h"



CPointShop::CPointShop(void)
{
}

CPointShop::~CPointShop(void)
{
}
void CPointShop::CreateSubControl ()
{
	
	m_pPointShopButton = new CBasicButton;
	m_pPointShopButton->CreateSub ( this,"POINTSHOP_BUTTON_IMAGE", UI_FLAG_DEFAULT, POINTSHOP_BUTTON );
	m_pPointShopButton->SetUseGlobalAction ( TRUE );
	m_pPointShopButton->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pPointShopButton );
}


void CPointShop::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );	

	m_pPointShopButton->SetVisibleSingle ( TRUE );
}

void CPointShop::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case	POINTSHOP_BUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO( "Point Shop(])", NS_UITEXTCOLOR::WHITE  );				
			}

			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
			if ( CInnerInterface::GetInstance().IsVisibleGroup( POINT_SHOP_WINDOW ) )		CInnerInterface::GetInstance().HideGroup ( POINT_SHOP_WINDOW );
			else	CInnerInterface::GetInstance().ShowGroupFocus( POINT_SHOP_WINDOW );
			}
		}
		break;
	}
}
