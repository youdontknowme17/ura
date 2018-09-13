// Name		: SelectMenuPage.cpp
// Project	: Lib-RanClientUI

#include "StdAfx.h"
#include "SelectUserPage.h"
#include "../EngineLib/DxCommon/DxFontMan.h"
#include "BasicTextButton.h"
#include "DxGlobalStage.h"
#include "GameTextControl.h"
#include "OuterInterface.h"
#include "ModalWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CSelectUserPage::CreateSubControl()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG);

	CBasicTextButton* pRegisterButton = new CBasicTextButton;
	pRegisterButton->CreateSub(this, "BASIC_TEXT_BUTTON24", UI_FLAG_XSIZE, SELECT_USER_PAGE_REGISTER);
	pRegisterButton->CreateBaseButton("SELECT_USER_PAGE_REGISTER", CBasicTextButton::SIZE24, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD("SELECT_USER_PAGE", 1));
	RegisterControl(pRegisterButton);

	CBasicTextButton* pForgotButton = new CBasicTextButton;
	pForgotButton->CreateSub(this, "BASIC_TEXT_BUTTON24", UI_FLAG_XSIZE, SELECT_USER_PAGE_FORGOTPASSWORD);
	pForgotButton->CreateBaseButton("SELECT_USER_PAGE_FORGOTPASSWORD", CBasicTextButton::SIZE24, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD("SELECT_USER_PAGE", 2));
	RegisterControl(pForgotButton);

	CBasicTextButton* pChangePasswButton = new CBasicTextButton;
	pChangePasswButton->CreateSub(this, "BASIC_TEXT_BUTTON24", UI_FLAG_XSIZE, SELECT_USER_PAGE_CHANGEPASSWORD);
	pChangePasswButton->CreateBaseButton("SELECT_USER_PAGE_CHANGEPASSWORD", CBasicTextButton::SIZE24, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD("SELECT_USER_PAGE", 3));
	RegisterControl(pChangePasswButton);

	CBasicTextButton* pChangePinCodeButton = new CBasicTextButton;
	pChangePinCodeButton->CreateSub(this, "BASIC_TEXT_BUTTON24", UI_FLAG_XSIZE, SELECT_USER_PAGE_CHANGEPINCODE);
	pChangePinCodeButton->CreateBaseButton("SELECT_USER_PAGE_CHANGEPINCODE", CBasicTextButton::SIZE24, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD("SELECT_USER_PAGE", 4));
	RegisterControl(pChangePinCodeButton);

	CBasicTextButton* pFixButton = new CBasicTextButton;
	pFixButton->CreateSub(this, "BASIC_TEXT_BUTTON24", UI_FLAG_XSIZE, SELECT_USER_PAGE_FIX);
	pFixButton->CreateBaseButton("SELECT_USER_PAGE_FIX", CBasicTextButton::SIZE24, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD("SELECT_USER_PAGE", 5));
	RegisterControl(pFixButton);

	CBasicTextButton* pBackButton = new CBasicTextButton;
	pBackButton->CreateSub(this, "BASIC_TEXT_BUTTON24", UI_FLAG_XSIZE, SELECT_USER_PAGE_BACK);
	pBackButton->CreateBaseButton("SELECT_USER_PAGE_BACK", CBasicTextButton::SIZE24, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD("SELECT_USER_PAGE", 6));
	pBackButton->SetShortcutKey(DIK_ESCAPE);
	RegisterControl(pBackButton);
}

void CSelectUserPage::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
	switch (ControlID)
	{
	case SELECT_USER_PAGE_REGISTER:
	{
		if (CHECK_KEYFOCUSED(dwMsg) || CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			COuterInterface::GetInstance().ToRegisterPage(GetWndID());
		}
	}
	break;

	case SELECT_USER_PAGE_FORGOTPASSWORD:
	{
		if (CHECK_KEYFOCUSED(dwMsg) || CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			COuterInterface::GetInstance().ToForgotAccountPage(GetWndID());
		}
	}
	break;

	case SELECT_USER_PAGE_CHANGEPASSWORD:
	{
		if (CHECK_KEYFOCUSED(dwMsg) || CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			COuterInterface::GetInstance().ToChangePasswordPage(GetWndID());
		}
	}
	break;

	case SELECT_USER_PAGE_CHANGEPINCODE:
	{
		if (CHECK_KEYFOCUSED(dwMsg) || CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			COuterInterface::GetInstance().ToChangePinCodePage(GetWndID());
		}
	}
	break;

	case SELECT_USER_PAGE_FIX:
	{
		if (CHECK_KEYFOCUSED(dwMsg) || CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			COuterInterface::GetInstance().ToFixUserPage(GetWndID());
		}
	}
	break;

	case SELECT_USER_PAGE_BACK:
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

void CSelectUserPage::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
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