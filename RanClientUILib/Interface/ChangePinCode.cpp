// Name		: ChangePinCode.cpp
// Project	: Lib-RanClientUI
// Author	: CNDev

#include "StdAfx.h"
#include "ChangePinCode.h"
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

int	CChangePinCodePage::nLIMIT_USERID = 12;
int	CChangePinCodePage::nLIMIT_PASSW = 12;
int CChangePinCodePage::nLIMIT_PINCODE = 6;
int	CChangePinCodePage::nLIMIT_CAPTCHA = 4;

CChangePinCodePage::CChangePinCodePage()
	: m_pRandTextBoxChangePinCodePage(NULL)
	, m_nRandPassNumberChangePinCodePage(0)
	, m_nRPUpdateCntChangePinCodePage(0)
{
}

CChangePinCodePage::~CChangePinCodePage()
{
}

void CChangePinCodePage::CreateSubControl()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG);

	{
		CBasicTextBox* pTextBox = NULL;
		pTextBox = CreateStaticControl("CHANGEPINCODE_PAGE_USERID", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT);
		pTextBox->SetOneLineText((char*)ID2GAMEWORD("CHANGEPINCODE_PAGE", 1));
		pTextBox = CreateStaticControl("CHANGEPINCODE_PAGE_PASSWORD", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT);
		pTextBox->SetOneLineText((char*)ID2GAMEWORD("CHANGEPINCODE_PAGE", 2));
		pTextBox = CreateStaticControl("CHANGEPINCODE_PAGE_OLDPINCODE", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT);
		pTextBox->SetOneLineText((char*)ID2GAMEWORD("CHANGEPINCODE_PAGE", 3));
		pTextBox = CreateStaticControl("CHANGEPINCODE_PAGE_NEWPINCODE", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT);
		pTextBox->SetOneLineText((char*)ID2GAMEWORD("CHANGEPINCODE_PAGE", 4));
		pTextBox = CreateStaticControl("CHANGEPINCODE_PAGE_RENEWPINCODE", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT);
		pTextBox->SetOneLineText((char*)ID2GAMEWORD("CHANGEPINCODE_PAGE", 5));
		pTextBox = CreateStaticControl("CHANGEPINCODE_PAGE_CAPTCHA1", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT);
		pTextBox->SetOneLineText((char*)ID2GAMEWORD("CHANGEPINCODE_PAGE", 6));

		m_pRandTextBoxChangePinCodePage = CreateStaticControl("CHANGEPINCODE_PAGE_CAPTCHA2", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT);
	}

	{
		CBasicLineBox* pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub(this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pBasicLineBox->CreateBaseBoxOuter("CHANGEPINCODE_PAGE_USERID_BACK");
		RegisterControl(pBasicLineBox);

		pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub(this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pBasicLineBox->CreateBaseBoxOuter("CHANGEPINCODE_PAGE_PASSWORD_BACK");
		RegisterControl(pBasicLineBox);

		pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub(this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pBasicLineBox->CreateBaseBoxOuter("CHANGEPINCODE_PAGE_OLDPINCODE_BACK");
		RegisterControl(pBasicLineBox);

		pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub(this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pBasicLineBox->CreateBaseBoxOuter("CHANGEPINCODE_PAGE_NEWPINCODE_BACK");
		RegisterControl(pBasicLineBox);

		pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub(this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pBasicLineBox->CreateBaseBoxOuter("CHANGEPINCODE_PAGE_RENEWPINCODE_BACK");
		RegisterControl(pBasicLineBox);

		pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub(this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pBasicLineBox->CreateBaseBoxOuter("CHANGEPINCODE_PAGE_CAPTCHA1_BACK");
		RegisterControl(pBasicLineBox);

		pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub(this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pBasicLineBox->CreateBaseBoxOuter("CHANGEPINCODE_PAGE_CAPTCHA2_BACK");
		RegisterControl(pBasicLineBox);
	}

	{
		CUIEditBoxMan* pEditBoxManChangePinCodePage = new CUIEditBoxMan;
		pEditBoxManChangePinCodePage->CreateSub(this, "CHANGEPINCODE_PAGE_EDITMAN", UI_FLAG_DEFAULT, CHANGEPINCODE_PAGE_EDITMAN);
		pEditBoxManChangePinCodePage->CreateEditBox(CHANGEPINCODE_PAGE_EDIT_USERID, "CHANGEPINCODE_PAGE_EDIT_USERID", "CHANGEPINCODE_PAGE_CARRAT", TRUE, UINT_MAX, pFont9, nLIMIT_USERID);
		pEditBoxManChangePinCodePage->CreateEditBox(CHANGEPINCODE_PAGE_EDIT_PASSWORD, "CHANGEPINCODE_PAGE_EDIT_PASSWORD", "CHANGEPINCODE_PAGE_CARRAT", TRUE, UINT_MAX, pFont9, nLIMIT_PASSW);
		pEditBoxManChangePinCodePage->CreateEditBox(CHANGEPINCODE_PAGE_EDIT_OLDPINCODE, "CHANGEPINCODE_PAGE_EDIT_OLDPINCODE", "CHANGEPINCODE_PAGE_CARRAT", TRUE, UINT_MAX, pFont9, nLIMIT_PINCODE);
		pEditBoxManChangePinCodePage->CreateEditBox(CHANGEPINCODE_PAGE_EDIT_NEWPINCODE, "CHANGEPINCODE_PAGE_EDIT_NEWPINCODE", "CHANGEPINCODE_PAGE_CARRAT", TRUE, UINT_MAX, pFont9, nLIMIT_PINCODE);
		pEditBoxManChangePinCodePage->CreateEditBox(CHANGEPINCODE_PAGE_EDIT_RENEWPINCODE, "CHANGEPINCODE_PAGE_EDIT_RENEWPINCODE", "CHANGEPINCODE_PAGE_CARRAT", TRUE, UINT_MAX, pFont9, nLIMIT_PINCODE);
		pEditBoxManChangePinCodePage->CreateEditBox(CHANGEPINCODE_PAGE_EDIT_CAPTCHA1, "CHANGEPINCODE_PAGE_EDIT_CAPTCHA1", "CHANGEPINCODE_PAGE_CARRAT", TRUE, UINT_MAX, pFont9, nLIMIT_CAPTCHA);
		pEditBoxManChangePinCodePage->SetHide(CHANGEPINCODE_PAGE_EDIT_PASSWORD, TRUE);
		pEditBoxManChangePinCodePage->SetHide(CHANGEPINCODE_PAGE_EDIT_OLDPINCODE, TRUE);
		pEditBoxManChangePinCodePage->SetHide(CHANGEPINCODE_PAGE_EDIT_NEWPINCODE, TRUE);
		pEditBoxManChangePinCodePage->SetHide(CHANGEPINCODE_PAGE_EDIT_RENEWPINCODE, TRUE);
		RegisterControl(pEditBoxManChangePinCodePage);
		m_pEditBoxManChangePinCodePage = pEditBoxManChangePinCodePage;
	}
	m_pRandTextBoxChangePinCodePage = CreateStaticControl("CHANGEPINCODE_PAGE_CAPTCHA2", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT);
	CBasicTextButton* pCancelButton = new CBasicTextButton;
	pCancelButton->CreateSub(this, "BASIC_TEXT_BUTTON24", UI_FLAG_XSIZE, CHANGEPINCODE_PAGE_CANCEL);
	pCancelButton->CreateBaseButton("CHANGEPINCODE_PAGE_CANCEL", CBasicTextButton::SIZE24, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD("CHANGEPINCODE_PAGE", 7));
	pCancelButton->SetShortcutKey(DIK_ESCAPE);
	RegisterControl(pCancelButton);

	CBasicTextButton* pSubmitButton = new CBasicTextButton;
	pSubmitButton->CreateSub(this, "BASIC_TEXT_BUTTON24", UI_FLAG_XSIZE, CHANGEPINCODE_PAGE_SUBMIT);
	pSubmitButton->CreateBaseButton("CHANGEPINCODE_PAGE_SUBMIT", CBasicTextButton::SIZE24, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD("CHANGEPINCODE_PAGE", 8));
	pSubmitButton->SetShortcutKey(DIK_RETURN, DIK_NUMPADENTER);
	RegisterControl(pSubmitButton);
}

void CChangePinCodePage::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
	switch (ControlID)
	{
	case CHANGEPINCODE_PAGE_CANCEL:
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

	case CHANGEPINCODE_PAGE_SUBMIT:
	{
		if (CHECK_KEYFOCUSED(dwMsg) || CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			SetMessageEx(dwMsg &= ~UIMSG_KEY_FOCUSED);

			CString strUserID = m_pEditBoxManChangePinCodePage->GetEditString(CHANGEPINCODE_PAGE_EDIT_USERID);
			CString strPassword = m_pEditBoxManChangePinCodePage->GetEditString(CHANGEPINCODE_PAGE_EDIT_PASSWORD);
			CString strOldPinCode = m_pEditBoxManChangePinCodePage->GetEditString(CHANGEPINCODE_PAGE_EDIT_OLDPINCODE);
			CString strNewPinCode = m_pEditBoxManChangePinCodePage->GetEditString(CHANGEPINCODE_PAGE_EDIT_NEWPINCODE);
			CString strReNewPinCode = m_pEditBoxManChangePinCodePage->GetEditString(CHANGEPINCODE_PAGE_EDIT_RENEWPINCODE);
			CString strCaptcha1 = m_pEditBoxManChangePinCodePage->GetEditString(CHANGEPINCODE_PAGE_EDIT_CAPTCHA1);
			int strCaptcha2 = m_nRandPassNumberChangePinCodePage;

			// Field can't empty - STAGE_1
			// I think we should add more restriction here
			// if ( j > 16 )
			// {
			// DoModalOuter(IDGGAMEEXTEXT("CHANGEPINCODE_PAGE_STAGE_XXX") <--- just an example
			// return;
			// }
			if (!strUserID.GetLength() || !strPassword.GetLength() || !strOldPinCode.GetLength() || !strNewPinCode.GetLength() || !strReNewPinCode.GetLength() || !strCaptcha1.GetLength())
			{
				DoModalOuter(ID2GAMEEXTEXT("CHANGEPINCODE_PAGE_STAGE_1"));
				return;
			}

			// Pincode must be between 5-12 character's - STAGE_2
			{
				int j = m_pEditBoxManChangePinCodePage->GetEditLength(CHANGEPINCODE_PAGE_EDIT_NEWPINCODE);

				if (j < 5)
				{
					DoModalOuter(ID2GAMEEXTEXT("CHANGEPINCODE_PAGE_STAGE_2"));
					return;
				}
			}

			// Re-Type Password is not match - STAGE_3
			if (strNewPinCode != strReNewPinCode)
			{
				DoModalOuter(ID2GAMEEXTEXT("CHANGEPINCODE_PAGE_STAGE_3"));
				return;
			}
			if (atoi(strCaptcha1) != strCaptcha2)
			{
				DoModalOuter(ID2GAMEEXTEXT("RECAPCHA_NOT_MATCH"));
				return;
			}

			ChangePinCode();

			COuterInterface::GetInstance().SetModalCallWindowID(GetWndID());
		}
	}
	break;
	}
}

void CChangePinCodePage::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
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
		if (m_nRandPassNumberChangePinCodePage == 0)
		{
			m_nRandPassNumberChangePinCodePage = pNetClient->GetRandomPassNumber();

			if (m_nRandPassNumberChangePinCodePage != 0)
			{
				CString strTemp;
				strTemp.Format(ID2GAMEEXTEXT("CHANGEPINCODE_PAGE_CAPTCHA2"), m_nRandPassNumberChangePinCodePage);
				if (m_pRandTextBoxChangePinCodePage) m_pRandTextBoxChangePinCodePage->SetOneLineText(strTemp);
			}
			else
			{
				if (m_nRPUpdateCntChangePinCodePage++ > 10)
				{
					pNetClient->SndRequestRandomKey();
					m_nRPUpdateCntChangePinCodePage = 0;
				}
			}
		}
	}

	if (m_pEditBoxManChangePinCodePage)
	{
		CUIControl* pParent = m_pEditBoxManChangePinCodePage->GetTopParent();
		if (!pParent)	pParent = this;
		BOOL bFocus = (pParent->IsFocusControl());

		if (bFocus)
		{
			if (UIKeyCheck::GetInstance()->Check(DIK_TAB, DXKEY_DOWN))
			{
				m_pEditBoxManChangePinCodePage->GoNextTab();
			}
		}
	}
}

void CChangePinCodePage::ResetAll()
{
	m_pEditBoxManChangePinCodePage->EndEdit();

	m_pEditBoxManChangePinCodePage->ClearEdit(CHANGEPINCODE_PAGE_EDIT_USERID);
	m_pEditBoxManChangePinCodePage->ClearEdit(CHANGEPINCODE_PAGE_EDIT_PASSWORD);
	m_pEditBoxManChangePinCodePage->ClearEdit(CHANGEPINCODE_PAGE_EDIT_OLDPINCODE);
	m_pEditBoxManChangePinCodePage->ClearEdit(CHANGEPINCODE_PAGE_EDIT_NEWPINCODE);
	m_pEditBoxManChangePinCodePage->ClearEdit(CHANGEPINCODE_PAGE_EDIT_RENEWPINCODE);
	m_pEditBoxManChangePinCodePage->ClearEdit(CHANGEPINCODE_PAGE_EDIT_CAPTCHA1);

	m_nRandPassNumberChangePinCodePage = 0;
	m_nRPUpdateCntChangePinCodePage = 0;

	m_pEditBoxManChangePinCodePage->Init();
	m_pEditBoxManChangePinCodePage->BeginEdit();

	if (m_pEditBoxManChangePinCodePage->IsMODE_NATIVE())
	{
		m_pEditBoxManChangePinCodePage->DoMODE_TOGGLE();
	}
}

void CChangePinCodePage::SetVisibleSingle(BOOL bVisible)
{
	CUIGroup::SetVisibleSingle(bVisible);

	if (bVisible)
	{
		if (GLCONST_CHAR::nUI_KEYBOARD == 2)
		{
			m_pEditBoxManChangePinCodePage->DisableKeyInput();
			DXInputString::GetInstance().DisableKeyInput();
		}

		ResetAll();

		if (m_pRandTextBoxChangePinCodePage) m_pRandTextBoxChangePinCodePage->ClearText();
		COuterInterface::GetInstance().ResetCancelToLogin();
	}
	else
	{
		m_pEditBoxManChangePinCodePage->EndEdit();

		if (GLCONST_CHAR::nUI_KEYBOARD == 2)
		{
			m_pEditBoxManChangePinCodePage->UsableKeyInput();
			DXInputString::GetInstance().UsableKeyInput();
		}

		CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient();
		pNetClient->ResetRandomPassNumber();
	}
}

void CChangePinCodePage::GoNextTab()
{
	if (m_pEditBoxManChangePinCodePage)
		m_pEditBoxManChangePinCodePage->GoNextTab();
}

void CChangePinCodePage::ChangePinCode()
{
	CString strUserID = m_pEditBoxManChangePinCodePage->GetEditString(CHANGEPINCODE_PAGE_EDIT_USERID);
	CString strPassword = m_pEditBoxManChangePinCodePage->GetEditString(CHANGEPINCODE_PAGE_EDIT_PASSWORD);
	CString strOldPincode = m_pEditBoxManChangePinCodePage->GetEditString(CHANGEPINCODE_PAGE_EDIT_OLDPINCODE);
	CString strNewPincode = m_pEditBoxManChangePinCodePage->GetEditString(CHANGEPINCODE_PAGE_EDIT_NEWPINCODE);
	CString strCaptcha1 = m_pEditBoxManChangePinCodePage->GetEditString(CHANGEPINCODE_PAGE_EDIT_CAPTCHA1);

	CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient();

	pNetClient->SndChangePinCode(
		strUserID.GetString(),
		strPassword.GetString(),
		strOldPincode.GetString(),
		strNewPincode.GetString(),
		strCaptcha1.GetString()
	);
}