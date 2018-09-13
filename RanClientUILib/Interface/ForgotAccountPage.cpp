// Name		: ForgotAccountPage.cpp
// Project	: Lib-RanClientUI

#include "StdAfx.h"
#include "ForgotAccountPage.h"
#include "../EngineLib/DxCommon/DxFontMan.h"
#include "BasicTextButton.h"
#include "BasicTextBoxEx.h"
#include "BasicLineBox.h"
#include "../EngineUIlib/GUInterface/UIEditBoxMan.h"
#include "DxGlobalStage.h"
#include "GameTextControl.h"
#include "UITextControl.h"
#include "OuterInterface.h"
#include "ModalWindow.h"
#include "../EngineUILib/GUInterface/UIKeyCheck.h"
#include "DXInputString.h"
#include "../=boost/boost/regex/v4/regex.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int	CForgotAccountPage::nLIMIT_EMAIL	= 30;
int	CForgotAccountPage::nLIMIT_PINCODE	= 6;
int	CForgotAccountPage::nLIMIT_CAPTCHA = 4;

CForgotAccountPage::CForgotAccountPage()
	: m_pRandTextBoxForgotAccountPage(NULL)
	, m_nRandPassNumberForgotAccountPage(0)
	, m_nRPUpdateCntForgotAccountPage(0)
{
}

CForgotAccountPage::~CForgotAccountPage()
{
}

void CForgotAccountPage::CreateSubControl()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG);

	{
		CBasicTextBox* pTextBox = NULL;
		pTextBox = CreateStaticControl("FORGOTACCOUNT_PAGE_EMAIL", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT);
		pTextBox->SetOneLineText((char*)ID2GAMEWORD("FORGOTACCOUNT_PAGE", 1));

		pTextBox = CreateStaticControl("FORGOTACCOUNT_PAGE_PINCODE", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT);
		pTextBox->SetOneLineText((char*)ID2GAMEWORD("FORGOTACCOUNT_PAGE", 2));

		pTextBox = CreateStaticControl("FORGOTACCOUNT_PAGE_CAPTCHA1", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT);
		pTextBox->SetOneLineText((char*)ID2GAMEWORD("FORGOTACCOUNT_PAGE", 3));

		m_pRandTextBoxForgotAccountPage = CreateStaticControl("FORGOTACCOUNT_PAGE_CAPTCHA2", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT);
	}

	{
		CBasicLineBox* pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub(this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pBasicLineBox->CreateBaseBoxOuter("FORGOTACCOUNT_PAGE_EMAIL_BACK");
		RegisterControl(pBasicLineBox);

		pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub(this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pBasicLineBox->CreateBaseBoxOuter("FORGOTACCOUNT_PAGE_PINCODE_BACK");
		RegisterControl(pBasicLineBox);

		pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub(this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pBasicLineBox->CreateBaseBoxOuter("FORGOTACCOUNT_PAGE_CAPTCHA1_BACK");
		RegisterControl(pBasicLineBox);

		pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub(this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pBasicLineBox->CreateBaseBoxOuter("FORGOTACCOUNT_PAGE_CAPTCHA2_BACK");
		RegisterControl(pBasicLineBox);
	}

	{
		CUIEditBoxMan* pEditBoxManForgotAccountPage = new CUIEditBoxMan;
		pEditBoxManForgotAccountPage->CreateSub(this, "FORGOTACCOUNT_PAGE_EDITMAN", UI_FLAG_DEFAULT, FORGOTACCOUNT_PAGE_EDITMAN);
		pEditBoxManForgotAccountPage->CreateEditBox(FORGOTACCOUNT_PAGE_EDIT_EMAIL, "FORGOTACCOUNT_PAGE_EDIT_EMAIL", "FORGOTACCOUNT_PAGE_CARRAT", TRUE, UINT_MAX, pFont9, nLIMIT_EMAIL);
		pEditBoxManForgotAccountPage->CreateEditBox(FORGOTACCOUNT_PAGE_EDIT_PINCODE, "FORGOTACCOUNT_PAGE_EDIT_PINCODE", "FORGOTACCOUNT_PAGE_CARRAT", TRUE, UINT_MAX, pFont9, nLIMIT_PINCODE);
		pEditBoxManForgotAccountPage->CreateEditBox(FORGOTACCOUNT_PAGE_EDIT_CAPTCHA1, "FORGOTACCOUNT_PAGE_EDIT_CAPTCHA1", "FORGOTACCOUNT_PAGE_CARRAT", TRUE, UINT_MAX, pFont9, nLIMIT_CAPTCHA);
		pEditBoxManForgotAccountPage->SetHide(FORGOTACCOUNT_PAGE_EDIT_PINCODE, TRUE);
		RegisterControl(pEditBoxManForgotAccountPage);
		m_pEditBoxManForgotAccountPage = pEditBoxManForgotAccountPage;
	}
	m_pRandTextBoxForgotAccountPage = CreateStaticControl("FORGOTACCOUNT_PAGE_CAPTCHA2", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT);
	CBasicTextButton* pCancelButton = new CBasicTextButton;
	pCancelButton->CreateSub(this, "BASIC_TEXT_BUTTON24", UI_FLAG_XSIZE, FORGOTACCOUNT_PAGE_CANCEL);
	pCancelButton->CreateBaseButton("FORGOTACCOUNT_PAGE_CANCEL", CBasicTextButton::SIZE24, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD("FORGOTACCOUNT_PAGE", 4));
	pCancelButton->SetShortcutKey(DIK_ESCAPE);
	RegisterControl(pCancelButton);

	CBasicTextButton* pSubmitButton = new CBasicTextButton;
	pSubmitButton->CreateSub(this, "BASIC_TEXT_BUTTON24", UI_FLAG_XSIZE, FORGOTACCOUNT_PAGE_SUBMIT);
	pSubmitButton->CreateBaseButton("FORGOTACCOUNT_PAGE_SUBMIT", CBasicTextButton::SIZE24, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD("FORGOTACCOUNT_PAGE", 5));
	pSubmitButton->SetShortcutKey(DIK_RETURN, DIK_NUMPADENTER);
	RegisterControl(pSubmitButton);
}

void CForgotAccountPage::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
	switch (ControlID)
	{
	case FORGOTACCOUNT_PAGE_CANCEL:
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

	case FORGOTACCOUNT_PAGE_SUBMIT:
	{
		if (CHECK_KEYFOCUSED(dwMsg) || CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			SetMessageEx(dwMsg &= ~UIMSG_KEY_FOCUSED);

			CString strEmail	= m_pEditBoxManForgotAccountPage->GetEditString(FORGOTACCOUNT_PAGE_EDIT_EMAIL);
			CString strPinCode	= m_pEditBoxManForgotAccountPage->GetEditString(FORGOTACCOUNT_PAGE_EDIT_PINCODE);
			CString strCaptcha1	= m_pEditBoxManForgotAccountPage->GetEditString(FORGOTACCOUNT_PAGE_EDIT_CAPTCHA1);
			int strCaptcha2	= m_nRandPassNumberForgotAccountPage;
			// Field can't empty - STAGE_1
			if (!strEmail.GetLength() || !strPinCode.GetLength() || !strCaptcha1.GetLength())
			{
				DoModalOuter(ID2GAMEEXTEXT("FORGOTACCOUNT_PAGE_STAGE_1"));
				return;
			}
			// Validate email
			//Add by mcalimosa 09/08/2018
			std::string subject(strEmail);
			boost::regex re("([0-9a-zA-Z]([-.\w]*[0-9a-zA-Z])*@(([0-9a-zA-Z])+([-\w]*[0-9a-zA-Z])*\.)+[a-zA-Z]{2,9})",boost::regex_constants::ECMAScript);
			boost::smatch match;
			if (!(boost::regex_search(subject, match, re)&& match.size() > 1)) {
				DoModalOuter(ID2GAMEEXTEXT("REGISTER_PAGE_STAGE_13"));
				return;
			}

			if (atoi(strCaptcha1) != strCaptcha2)
			{
				DoModalOuter(ID2GAMEEXTEXT("RECAPCHA_NOT_MATCH"));
				return;
			}

			ForgotAccount();

			COuterInterface::GetInstance().SetModalCallWindowID(GetWndID());
		}
	}
	break;
	}
}

void CForgotAccountPage::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
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
	else
	{
		if (m_nRandPassNumberForgotAccountPage == 0)
		{
			m_nRandPassNumberForgotAccountPage = pNetClient->GetRandomPassNumber();

			if (m_nRandPassNumberForgotAccountPage != 0)
			{
				CString strTemp;
				strTemp.Format(ID2GAMEEXTEXT("FORGOTACCOUNT_PAGE_CAPTCHA2"), m_nRandPassNumberForgotAccountPage);
				if (m_pRandTextBoxForgotAccountPage) m_pRandTextBoxForgotAccountPage->SetOneLineText(strTemp);
			}
			else
			{
				if (m_nRPUpdateCntForgotAccountPage++ > 10)
				{
					pNetClient->SndRequestRandomKey();
					m_nRPUpdateCntForgotAccountPage = 0;
				}
			}
		}
	}

	if (m_pEditBoxManForgotAccountPage)
	{
		CUIControl* pParent = m_pEditBoxManForgotAccountPage->GetTopParent();
		if (!pParent)	pParent = this;
		BOOL bFocus = (pParent->IsFocusControl());

		if (bFocus)
		{
			if (UIKeyCheck::GetInstance()->Check(DIK_TAB, DXKEY_DOWN))
			{
				m_pEditBoxManForgotAccountPage->GoNextTab();
			}
		}
	}
}

void CForgotAccountPage::SetVisibleSingle(BOOL bVisible)
{
	CUIGroup::SetVisibleSingle(bVisible);

	if (bVisible)
	{
		if (GLCONST_CHAR::nUI_KEYBOARD == 2)
		{
			m_pEditBoxManForgotAccountPage->DisableKeyInput();
			DXInputString::GetInstance().DisableKeyInput();
		}

		ResetAll();

		if (m_pRandTextBoxForgotAccountPage) m_pRandTextBoxForgotAccountPage->ClearText();
		COuterInterface::GetInstance().ResetCancelToLogin();
	}
	else
	{
		m_pEditBoxManForgotAccountPage->EndEdit();

		if (GLCONST_CHAR::nUI_KEYBOARD == 2)
		{
			m_pEditBoxManForgotAccountPage->UsableKeyInput();
			DXInputString::GetInstance().UsableKeyInput();
		}

		CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient();
		pNetClient->ResetRandomPassNumber();
	}
}

void CForgotAccountPage::GoNextTab()
{
	if (m_pEditBoxManForgotAccountPage)
		m_pEditBoxManForgotAccountPage->GoNextTab();
}

void CForgotAccountPage::ResetAll()
{
	m_pEditBoxManForgotAccountPage->EndEdit();

	m_pEditBoxManForgotAccountPage->ClearEdit(FORGOTACCOUNT_PAGE_EDIT_EMAIL);
	m_pEditBoxManForgotAccountPage->ClearEdit(FORGOTACCOUNT_PAGE_EDIT_PINCODE);
	m_pEditBoxManForgotAccountPage->ClearEdit(FORGOTACCOUNT_PAGE_EDIT_CAPTCHA1);

	m_nRandPassNumberForgotAccountPage = 0;
	m_nRPUpdateCntForgotAccountPage = 0;

	m_pEditBoxManForgotAccountPage->Init();
	m_pEditBoxManForgotAccountPage->BeginEdit();

	if (m_pEditBoxManForgotAccountPage->IsMODE_NATIVE())
	{
		m_pEditBoxManForgotAccountPage->DoMODE_TOGGLE();
	}
}

void CForgotAccountPage::ForgotAccount()
{
	CString strEmail	= m_pEditBoxManForgotAccountPage->GetEditString(FORGOTACCOUNT_PAGE_EDIT_EMAIL);
	CString strPinCode	= m_pEditBoxManForgotAccountPage->GetEditString(FORGOTACCOUNT_PAGE_EDIT_PINCODE);
	CString strCaptcha1	= m_pEditBoxManForgotAccountPage->GetEditString(FORGOTACCOUNT_PAGE_EDIT_CAPTCHA1);
	CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient();

	pNetClient->SndForgotAccount(
		strEmail.GetString(),
		strPinCode.GetString(),
		strCaptcha1.GetString()
	);
}