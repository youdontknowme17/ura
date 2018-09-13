// Name		: SelectMenuPage.cpp
// Project	: Lib-RanClientUI

#include "StdAfx.h"
#include "SelectMenuPage.h"
#include "../EngineLib/DxCommon/DxFontMan.h"
#include "BasicTextButton.h"
#include "GameTextControl.h"
#include "DxGlobalStage.h"
#include "OuterInterface.h"
#include "ModalWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CSelectMenuPage::CreateSubControl()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG);

	CBasicTextButton* pLoginButton = new CBasicTextButton;
	pLoginButton->CreateSub(this, "BASIC_TEXT_BUTTON24", UI_FLAG_XSIZE, SELECT_MENU_PAGE_LOGIN);
	pLoginButton->CreateBaseButton("SELECT_MENU_PAGE_LOGIN", CBasicTextButton::SIZE24, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD("SELECT_MENU_PAGE", 1));
	RegisterControl(pLoginButton);

	CBasicTextButton* pUserButton = new CBasicTextButton;
	pUserButton->CreateSub(this, "BASIC_TEXT_BUTTON24", UI_FLAG_XSIZE, SELECT_MENU_PAGE_USER);
	pUserButton->CreateBaseButton("SELECT_MENU_PAGE_USER", CBasicTextButton::SIZE24, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD("SELECT_MENU_PAGE", 2));
	RegisterControl(pUserButton);

	CBasicTextButton* pBackButton = new CBasicTextButton;
	pBackButton->CreateSub(this, "BASIC_TEXT_BUTTON24", UI_FLAG_XSIZE, SELECT_MENU_PAGE_BACK);
	pBackButton->CreateBaseButton("SELECT_MENU_PAGE_BACK", CBasicTextButton::SIZE24, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD("SELECT_MENU_PAGE", 3));
	pBackButton->SetShortcutKey(DIK_ESCAPE);
	RegisterControl(pBackButton);
}

void CSelectMenuPage::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
	switch (ControlID)
	{
	case SELECT_MENU_PAGE_LOGIN:
	{
		if (CHECK_KEYFOCUSED(dwMsg) || CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			COuterInterface::GetInstance().ToNewLoginPage(GetWndID());
		}
	}
	break;

	case SELECT_MENU_PAGE_USER:
	{
		if (CHECK_KEYFOCUSED(dwMsg) || CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			COuterInterface::GetInstance().ToSelectUserPage(GetWndID());
		}
	}
	break;

	case SELECT_MENU_PAGE_BACK:
	{
		if (CHECK_KEYFOCUSED(dwMsg) || CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient();
			if (pNetClient->IsOnline() == true)
			{
				pNetClient->CloseConnect();
				COuterInterface::GetInstance().SetCancelToLogin();
			}

			COuterInterface::GetInstance().ToSelectServerPage(GetWndID());
		}
	}
	break;
	}
}

void CSelectMenuPage::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
	CUIWindow::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);

	CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient();
	if (!pNetClient->IsOnline())
	{
		if (!COuterInterface::GetInstance().IsVisibleGroup(MODAL_WINDOW_OUTER))
		{
			if (!COuterInterface::GetInstance().IsLoginCancel())
			{
				DoModalOuter(ID2GAMEEXTEXT("LOGINSTAGE_1"), MODAL_INFOMATION, OK, OUTER_MODAL_RECONNECT);
			}
		}
	}
}