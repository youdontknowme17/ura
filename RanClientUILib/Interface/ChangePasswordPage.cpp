// Name		: ChangePasswordPage.cpp
// Project	: Lib-RanClientUI

#include "StdAfx.h"
#include "ChangePasswordPage.h"
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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int	CChangePasswordPage::nLIMIT_USERID = 12;
int	CChangePasswordPage::nLIMIT_PASSW = 12;
int	CChangePasswordPage::nLIMIT_CAPTCHA = 4;

CChangePasswordPage::CChangePasswordPage()
	: m_pRandTextBoxChangePasswordPage(NULL)
	, m_nRandPassNumberChangePasswordPage(0)
	, m_nRPUpdateCntChangePasswordPage(0)
{
}

CChangePasswordPage::~CChangePasswordPage()
{
}

void CChangePasswordPage::CreateSubControl()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG);

	{
		CBasicTextBox* pTextBox = NULL;
		pTextBox = CreateStaticControl("CHANGEPASSWORD_PAGE_USERID", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT);
		pTextBox->SetOneLineText((char*)ID2GAMEWORD("CHANGEPASSWORD_PAGE", 1));
		pTextBox = CreateStaticControl("CHANGEPASSWORD_PAGE_OLDPASSW", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT);
		pTextBox->SetOneLineText((char*)ID2GAMEWORD("CHANGEPASSWORD_PAGE", 2));
		pTextBox = CreateStaticControl("CHANGEPASSWORD_PAGE_NEWPASSW", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT);
		pTextBox->SetOneLineText((char*)ID2GAMEWORD("CHANGEPASSWORD_PAGE", 3));
		pTextBox = CreateStaticControl("CHANGEPASSWORD_PAGE_RENEWPASSW", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT);
		pTextBox->SetOneLineText((char*)ID2GAMEWORD("CHANGEPASSWORD_PAGE", 4));
		pTextBox = CreateStaticControl("CHANGEPASSWORD_PAGE_CAPTCHA1", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT);
		pTextBox->SetOneLineText((char*)ID2GAMEWORD("CHANGEPASSWORD_PAGE", 5));
	}

	{
		CBasicLineBox* pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub(this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pBasicLineBox->CreateBaseBoxOuter("CHANGEPASSWORD_PAGE_USERID_BACK");
		RegisterControl(pBasicLineBox);

		pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub(this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pBasicLineBox->CreateBaseBoxOuter("CHANGEPASSWORD_PAGE_OLDPASSW_BACK");
		RegisterControl(pBasicLineBox);

		pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub(this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pBasicLineBox->CreateBaseBoxOuter("CHANGEPASSWORD_PAGE_NEWPASSW_BACK");
		RegisterControl(pBasicLineBox);

		pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub(this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pBasicLineBox->CreateBaseBoxOuter("CHANGEPASSWORD_PAGE_RENEWPASSW_BACK");
		RegisterControl(pBasicLineBox);

		pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub(this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pBasicLineBox->CreateBaseBoxOuter("CHANGEPASSWORD_PAGE_CAPTCHA1_BACK");
		RegisterControl(pBasicLineBox);

		pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub(this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pBasicLineBox->CreateBaseBoxOuter("CHANGEPASSWORD_PAGE_CAPTCHA2_BACK");
		RegisterControl(pBasicLineBox);
	}

	{
		CUIEditBoxMan* pEditBoxManChangePasswordPage = new CUIEditBoxMan;
		pEditBoxManChangePasswordPage->CreateSub(this, "CHANGEPASSWORD_PAGE_EDITMAN", UI_FLAG_DEFAULT, CHANGEPASSWORD_PAGE_EDITMAN);
		pEditBoxManChangePasswordPage->CreateEditBox(CHANGEPASSWORD_PAGE_EDIT_USERID, "CHANGEPASSWORD_PAGE_EDIT_USERID", "CHANGEPASSWORD_PAGE_CARRAT", TRUE, UINT_MAX, pFont9, nLIMIT_USERID);
		pEditBoxManChangePasswordPage->CreateEditBox(CHANGEPASSWORD_PAGE_EDIT_OLDPASSW, "CHANGEPASSWORD_PAGE_EDIT_OLDPASSW", "CHANGEPASSWORD_PAGE_CARRAT", TRUE, UINT_MAX, pFont9, nLIMIT_PASSW);
		pEditBoxManChangePasswordPage->CreateEditBox(CHANGEPASSWORD_PAGE_EDIT_NEWPASSW, "CHANGEPASSWORD_PAGE_EDIT_NEWPASSW", "CHANGEPASSWORD_PAGE_CARRAT", TRUE, UINT_MAX, pFont9, nLIMIT_PASSW);
		pEditBoxManChangePasswordPage->CreateEditBox(CHANGEPASSWORD_PAGE_EDIT_RENEWPASSW, "CHANGEPASSWORD_PAGE_EDIT_RENEWPASSW", "CHANGEPASSWORD_PAGE_CARRAT", TRUE, UINT_MAX, pFont9, nLIMIT_PASSW);
		pEditBoxManChangePasswordPage->CreateEditBox(CHANGEPASSWORD_PAGE_EDIT_CAPTCHA1, "CHANGEPASSWORD_PAGE_EDIT_CAPTCHA1", "CHANGEPASSWORD_PAGE_CARRAT", TRUE, UINT_MAX, pFont9, nLIMIT_CAPTCHA);
		pEditBoxManChangePasswordPage->SetHide(CHANGEPASSWORD_PAGE_EDIT_OLDPASSW, TRUE);
		pEditBoxManChangePasswordPage->SetHide(CHANGEPASSWORD_PAGE_EDIT_NEWPASSW, TRUE);
		pEditBoxManChangePasswordPage->SetHide(CHANGEPASSWORD_PAGE_EDIT_RENEWPASSW, TRUE);
		RegisterControl(pEditBoxManChangePasswordPage);
		m_pEditBoxManChangePasswordPage = pEditBoxManChangePasswordPage;
	}

	m_pRandTextBoxChangePasswordPage = CreateStaticControl("CHANGEPASSWORD_PAGE_CAPTCHA2", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT);

	CBasicTextButton* pCancelButton = new CBasicTextButton;
	pCancelButton->CreateSub(this, "BASIC_TEXT_BUTTON24", UI_FLAG_XSIZE, CHANGEPASSWORD_PAGE_CANCEL);
	pCancelButton->CreateBaseButton("CHANGEPASSWORD_PAGE_CANCEL", CBasicTextButton::SIZE24, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD("CHANGEPASSWORD_PAGE", 6));
	pCancelButton->SetShortcutKey(DIK_ESCAPE);
	RegisterControl(pCancelButton);

	CBasicTextButton* pSubmitButton = new CBasicTextButton;
	pSubmitButton->CreateSub(this, "BASIC_TEXT_BUTTON24", UI_FLAG_XSIZE, CHANGEPASSWORD_PAGE_SUBMIT);
	pSubmitButton->CreateBaseButton("CHANGEPASSWORD_PAGE_SUBMIT", CBasicTextButton::SIZE24, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD("CHANGEPASSWORD_PAGE", 7));
	pSubmitButton->SetShortcutKey(DIK_RETURN, DIK_NUMPADENTER);
	RegisterControl(pSubmitButton);
}

void CChangePasswordPage::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
	switch (ControlID)
	{
	case CHANGEPASSWORD_PAGE_CANCEL:
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

	case CHANGEPASSWORD_PAGE_SUBMIT:
	{
		if (CHECK_KEYFOCUSED(dwMsg) || CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			SetMessageEx(dwMsg &= ~UIMSG_KEY_FOCUSED);

			CString strUserID = m_pEditBoxManChangePasswordPage->GetEditString(CHANGEPASSWORD_PAGE_EDIT_USERID);
			CString strOldPassw = m_pEditBoxManChangePasswordPage->GetEditString(CHANGEPASSWORD_PAGE_EDIT_OLDPASSW);
			CString strNewPassw = m_pEditBoxManChangePasswordPage->GetEditString(CHANGEPASSWORD_PAGE_EDIT_NEWPASSW);
			CString strReNewPassw = m_pEditBoxManChangePasswordPage->GetEditString(CHANGEPASSWORD_PAGE_EDIT_RENEWPASSW);
			CString strCaptcha1 = m_pEditBoxManChangePasswordPage->GetEditString(CHANGEPASSWORD_PAGE_EDIT_CAPTCHA1);
			int strCaptcha2 = m_nRandPassNumberChangePasswordPage;
			// Field can't empty - STAGE_1
			if (!strUserID.GetLength() || !strOldPassw.GetLength() || !strNewPassw.GetLength() || !strReNewPassw.GetLength() || !strCaptcha1.GetLength())
			{
				DoModalOuter(ID2GAMEEXTEXT("CHANGEPASSWORD_PAGE_STAGE_1"));
				return;
			}

			// Password must be between 5-12 character's - STAGE_2
			{
				int j = m_pEditBoxManChangePasswordPage->GetEditLength(CHANGEPASSWORD_PAGE_EDIT_NEWPASSW);

				if (j < 5)
				{
					DoModalOuter(ID2GAMEEXTEXT("CHANGEPASSWORD_PAGE_STAGE_2"));
					return;
				}
			}

			// Re-Type Password is not match - STAGE_3
			if (strNewPassw != strReNewPassw)
			{
				DoModalOuter(ID2GAMEEXTEXT("CHANGEPASSWORD_PAGE_STAGE_3"));
				return;
			}
			if (atoi(strCaptcha1) != strCaptcha2)
			{
				DoModalOuter(ID2GAMEEXTEXT("RECAPCHA_NOT_MATCH"));
				return;
			}

			ChangePassword();

			COuterInterface::GetInstance().SetModalCallWindowID(GetWndID());
		}
	}
	break;
	}
}

void CChangePasswordPage::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
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
		if (m_nRandPassNumberChangePasswordPage == 0)
		{
			m_nRandPassNumberChangePasswordPage = pNetClient->GetRandomPassNumber();

			if (m_nRandPassNumberChangePasswordPage != 0)
			{
				CString strTemp;
				strTemp.Format(ID2GAMEEXTEXT("CHANGEPASSWORD_PAGE_CAPTCHA2"), m_nRandPassNumberChangePasswordPage);
				if (m_pRandTextBoxChangePasswordPage) m_pRandTextBoxChangePasswordPage->SetOneLineText(strTemp);
			}
			else
			{
				if (m_nRPUpdateCntChangePasswordPage++ > 10)
				{
					pNetClient->SndRequestRandomKey();
					m_nRPUpdateCntChangePasswordPage = 0;
				}
			}
		}
	}

	if (m_pEditBoxManChangePasswordPage)
	{
		CUIControl* pParent = m_pEditBoxManChangePasswordPage->GetTopParent();
		if (!pParent)	pParent = this;
		BOOL bFocus = (pParent->IsFocusControl());

		if (bFocus)
		{
			if (UIKeyCheck::GetInstance()->Check(DIK_TAB, DXKEY_DOWN))
			{
				m_pEditBoxManChangePasswordPage->GoNextTab();
			}
		}
	}
}

void CChangePasswordPage::ResetAll()
{
	m_pEditBoxManChangePasswordPage->EndEdit();

	m_pEditBoxManChangePasswordPage->ClearEdit(CHANGEPASSWORD_PAGE_EDIT_USERID);
	m_pEditBoxManChangePasswordPage->ClearEdit(CHANGEPASSWORD_PAGE_EDIT_OLDPASSW);
	m_pEditBoxManChangePasswordPage->ClearEdit(CHANGEPASSWORD_PAGE_EDIT_NEWPASSW);
	m_pEditBoxManChangePasswordPage->ClearEdit(CHANGEPASSWORD_PAGE_EDIT_RENEWPASSW);
	m_pEditBoxManChangePasswordPage->ClearEdit(CHANGEPASSWORD_PAGE_EDIT_CAPTCHA1);

	m_nRandPassNumberChangePasswordPage = 0;
	m_nRPUpdateCntChangePasswordPage = 0;

	m_pEditBoxManChangePasswordPage->Init();
	m_pEditBoxManChangePasswordPage->BeginEdit();

	if (m_pEditBoxManChangePasswordPage->IsMODE_NATIVE())
	{
		m_pEditBoxManChangePasswordPage->DoMODE_TOGGLE();
	}
}

void CChangePasswordPage::SetVisibleSingle(BOOL bVisible)
{
	CUIGroup::SetVisibleSingle(bVisible);

	if (bVisible)
	{
		if (GLCONST_CHAR::nUI_KEYBOARD == 2)
		{
			m_pEditBoxManChangePasswordPage->DisableKeyInput();
			DXInputString::GetInstance().DisableKeyInput();
		}

		ResetAll();

		if (m_pRandTextBoxChangePasswordPage) m_pRandTextBoxChangePasswordPage->ClearText();
		COuterInterface::GetInstance().ResetCancelToLogin();
	}
	else
	{
		m_pEditBoxManChangePasswordPage->EndEdit();

		if (GLCONST_CHAR::nUI_KEYBOARD == 2)
		{
			m_pEditBoxManChangePasswordPage->UsableKeyInput();
			DXInputString::GetInstance().UsableKeyInput();
		}

		CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient();
		pNetClient->ResetRandomPassNumber();
	}
}

void CChangePasswordPage::GoNextTab()
{
	if (m_pEditBoxManChangePasswordPage)
		m_pEditBoxManChangePasswordPage->GoNextTab();
}

void CChangePasswordPage::ChangePassword()
{
	CString strUserID	= m_pEditBoxManChangePasswordPage->GetEditString(CHANGEPASSWORD_PAGE_EDIT_USERID);
	CString strOldPassw = m_pEditBoxManChangePasswordPage->GetEditString(CHANGEPASSWORD_PAGE_EDIT_OLDPASSW);
	CString strNewPassw = m_pEditBoxManChangePasswordPage->GetEditString(CHANGEPASSWORD_PAGE_EDIT_NEWPASSW);
	CString strCaptcha1 = m_pEditBoxManChangePasswordPage->GetEditString(CHANGEPASSWORD_PAGE_EDIT_CAPTCHA1);

	CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient();

	pNetClient->SndChangePassword(
		strUserID.GetString(),
		strOldPassw.GetString(),
		strNewPassw.GetString(),
		strCaptcha1.GetString()
	);
}