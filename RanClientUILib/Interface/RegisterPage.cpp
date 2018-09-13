// Name		: RegisterPage.cpp
// Project	: Lib-RanClientUI


#include "StdAfx.h"
#include "RegisterPage.h"
#include "../=boost/boost/regex/v4/regex.hpp"
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
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int	CRegisterPage::nLIMIT_USERID = 12;
int	CRegisterPage::nLIMIT_PASSW = 12;
int	CRegisterPage::nLIMIT_PINCODE = 6;
int	CRegisterPage::nLIMIT_EMAIL = 30;
int	CRegisterPage::nLIMIT_CAPTCHA = 4;

CRegisterPage::CRegisterPage()
	: m_pRandTextBoxRegisterPage(NULL)
	, m_nRandPassNumberRegisterPage(0)
	, m_nRPUpdateCntRegisterPage(0)
{
}

CRegisterPage::~CRegisterPage()
{
}

void CRegisterPage::CreateSubControl()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG);

	{
		CBasicTextBox* pTextBox = NULL;
		pTextBox = CreateStaticControl("REGISTER_PAGE_USERID", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT);
		pTextBox->SetOneLineText((char*)ID2GAMEWORD("REGISTER_PAGE", 1));

		pTextBox = CreateStaticControl("REGISTER_PAGE_PASSW", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT);
		pTextBox->SetOneLineText((char*)ID2GAMEWORD("REGISTER_PAGE", 2));

		pTextBox = CreateStaticControl("REGISTER_PAGE_REPASSW", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT);
		pTextBox->SetOneLineText((char*)ID2GAMEWORD("REGISTER_PAGE", 3));

		pTextBox = CreateStaticControl("REGISTER_PAGE_PINCODE", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT);
		pTextBox->SetOneLineText((char*)ID2GAMEWORD("REGISTER_PAGE", 4));

		pTextBox = CreateStaticControl("REGISTER_PAGE_EMAIL", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT);
		pTextBox->SetOneLineText((char*)ID2GAMEWORD("REGISTER_PAGE", 5));

		pTextBox = CreateStaticControl("REGISTER_PAGE_CAPTCHA1", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT);
		pTextBox->SetOneLineText((char*)ID2GAMEWORD("REGISTER_PAGE", 6));
	}

	{
		CBasicLineBox* pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub(this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pBasicLineBox->CreateBaseBoxOuter("REGISTER_PAGE_USERID_BACK");
		RegisterControl(pBasicLineBox);

		pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub(this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pBasicLineBox->CreateBaseBoxOuter("REGISTER_PAGE_PASSW_BACK");
		RegisterControl(pBasicLineBox);

		pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub(this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pBasicLineBox->CreateBaseBoxOuter("REGISTER_PAGE_REPASSW_BACK");
		RegisterControl(pBasicLineBox);

		pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub(this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pBasicLineBox->CreateBaseBoxOuter("REGISTER_PAGE_PINCODE_BACK");
		RegisterControl(pBasicLineBox);

		pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub(this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pBasicLineBox->CreateBaseBoxOuter("REGISTER_PAGE_EMAIL_BACK");
		RegisterControl(pBasicLineBox);

		pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub(this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pBasicLineBox->CreateBaseBoxOuter("REGISTER_PAGE_CAPTCHA1_BACK");
		RegisterControl(pBasicLineBox);

		pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub(this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pBasicLineBox->CreateBaseBoxOuter("REGISTER_PAGE_CAPTCHA2_BACK");
		RegisterControl(pBasicLineBox);
	}

	{
		CUIEditBoxMan* pEditBoxManRegisterPage = new CUIEditBoxMan;
		pEditBoxManRegisterPage->CreateSub(this, "REGISTER_PAGE_EDITMAN", UI_FLAG_DEFAULT, REGISTER_PAGE_EDITMAN);
		pEditBoxManRegisterPage->CreateEditBox(REGISTER_PAGE_EDIT_USERID, "REGISTER_PAGE_EDIT_USERID", "REGISTER_PAGE_CARRAT", TRUE, UINT_MAX, pFont9, nLIMIT_USERID);
		pEditBoxManRegisterPage->CreateEditBox(REGISTER_PAGE_EDIT_PASSW, "REGISTER_PAGE_EDIT_PASSW", "REGISTER_PAGE_CARRAT", TRUE, UINT_MAX, pFont9, nLIMIT_PASSW);
		pEditBoxManRegisterPage->CreateEditBox(REGISTER_PAGE_EDIT_REPASSW, "REGISTER_PAGE_EDIT_REPASSW", "REGISTER_PAGE_CARRAT", TRUE, UINT_MAX, pFont9, nLIMIT_PASSW);
		pEditBoxManRegisterPage->CreateEditBox(REGISTER_PAGE_EDIT_PINCODE, "REGISTER_PAGE_EDIT_PINCODE", "REGISTER_PAGE_CARRAT", TRUE, UINT_MAX, pFont9, nLIMIT_PINCODE);
		pEditBoxManRegisterPage->CreateEditBox(REGISTER_PAGE_EDIT_EMAIL, "REGISTER_PAGE_EDIT_EMAIL", "REGISTER_PAGE_CARRAT", TRUE, UINT_MAX, pFont9, nLIMIT_EMAIL);
		pEditBoxManRegisterPage->CreateEditBox(REGISTER_PAGE_EDIT_CAPTCHA1, "REGISTER_PAGE_EDIT_CAPTCHA1", "REGISTER_PAGE_CARRAT", TRUE, UINT_MAX, pFont9, nLIMIT_CAPTCHA);
		pEditBoxManRegisterPage->SetHide(REGISTER_PAGE_EDIT_PASSW, TRUE);
		pEditBoxManRegisterPage->SetHide(REGISTER_PAGE_EDIT_REPASSW, TRUE);
		pEditBoxManRegisterPage->SetHide(REGISTER_PAGE_EDIT_PINCODE, TRUE);
		RegisterControl(pEditBoxManRegisterPage);
		m_pEditBoxManRegisterPage = pEditBoxManRegisterPage;
	}

	m_pRandTextBoxRegisterPage = CreateStaticControl("REGISTER_PAGE_CAPTCHA2", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT);
	CBasicTextButton* pCancelButton = new CBasicTextButton;
	pCancelButton->CreateSub(this, "BASIC_TEXT_BUTTON24", UI_FLAG_XSIZE, REGISTER_PAGE_CANCEL);
	pCancelButton->CreateBaseButton("REGISTER_PAGE_CANCEL", CBasicTextButton::SIZE24, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD("REGISTER_PAGE", 7));
	pCancelButton->SetShortcutKey(DIK_ESCAPE);
	RegisterControl(pCancelButton);

	CBasicTextButton* pSubmitButton = new CBasicTextButton;
	pSubmitButton->CreateSub(this, "BASIC_TEXT_BUTTON24", UI_FLAG_XSIZE, REGISTER_PAGE_SUBMIT);
	pSubmitButton->CreateBaseButton("REGISTER_PAGE_SUBMIT", CBasicTextButton::SIZE24, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD("REGISTER_PAGE", 8));
	pSubmitButton->SetShortcutKey(DIK_RETURN, DIK_NUMPADENTER);
	RegisterControl(pSubmitButton);
}

void CRegisterPage::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
	switch (ControlID)
	{
	case REGISTER_PAGE_CANCEL:
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

	case REGISTER_PAGE_SUBMIT:
	{
		if (CHECK_KEYFOCUSED(dwMsg) || CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			SetMessageEx(dwMsg &= ~UIMSG_KEY_FOCUSED);

			CString strUserID	= m_pEditBoxManRegisterPage->GetEditString(REGISTER_PAGE_EDIT_USERID);
			CString strPassw	= m_pEditBoxManRegisterPage->GetEditString(REGISTER_PAGE_EDIT_PASSW);
			CString strRePassw	= m_pEditBoxManRegisterPage->GetEditString(REGISTER_PAGE_EDIT_REPASSW);
			CString strPinCode	= m_pEditBoxManRegisterPage->GetEditString(REGISTER_PAGE_EDIT_PINCODE);
			CString strEmail	= m_pEditBoxManRegisterPage->GetEditString(REGISTER_PAGE_EDIT_EMAIL);
			CString strCaptcha1	= m_pEditBoxManRegisterPage->GetEditString(REGISTER_PAGE_EDIT_CAPTCHA1);
			int strCaptcha2	= m_nRandPassNumberRegisterPage;
			// Field can't empty - STAGE_1
			if (!strUserID.GetLength() || !strPassw.GetLength() || !strPinCode.GetLength() || !strEmail.GetLength())
			{
				DoModalOuter(ID2GAMEEXTEXT("REGISTER_PAGE_STAGE_1"));
				return;
			}

			// UserID must be between 5-12 character's - STAGE_2
			{
				int j = m_pEditBoxManRegisterPage->GetEditLength(REGISTER_PAGE_EDIT_USERID);

				if (j < 5)
				{
					DoModalOuter(ID2GAMEEXTEXT("REGISTER_PAGE_STAGE_2"));
					return;
				}
			}

			// Password must be between 5-12 character's - STAGE_3
			{
				int k = m_pEditBoxManRegisterPage->GetEditLength(REGISTER_PAGE_EDIT_PASSW);

				if (k < 5)
				{
					DoModalOuter(ID2GAMEEXTEXT("REGISTER_PAGE_STAGE_3"));
					return;
				}
			}

			// Re-Type Password is not match - STAGE_4
			if (strPassw != strRePassw)
			{
				DoModalOuter(ID2GAMEEXTEXT("REGISTER_PAGE_STAGE_4"));
				return;
			}

			// PinCode Must be 6 character's - STAGE_5
			{
				int l = m_pEditBoxManRegisterPage->GetEditLength(REGISTER_PAGE_EDIT_PINCODE);

				if (l <= 5)
				{
					DoModalOuter(ID2GAMEEXTEXT("REGISTER_PAGE_STAGE_5"));
					return;
				}
			}

			// invalid character used : UserID - STAGE_6
			if (!CheckString(strUserID))
			{
				DoModalOuter(ID2GAMEEXTEXT("REGISTER_PAGE_STAGE_6"));
				return;
			}

			// invalid character used : PinCode - STAGE_7
			if (!CheckString(strPinCode))
			{
				DoModalOuter(ID2GAMEEXTEXT("REGISTER_PAGE_STAGE_7"));
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

			RegisterAccount();

			COuterInterface::GetInstance().SetModalCallWindowID(GetWndID());
		}
	}
	break;
	}
}

void CRegisterPage::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
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
		if (m_nRandPassNumberRegisterPage == 0)
		{
			m_nRandPassNumberRegisterPage = pNetClient->GetRandomPassNumber();

			if (m_nRandPassNumberRegisterPage != 0)
			{
				CString strTemp;
				strTemp.Format(ID2GAMEEXTEXT("REGISTER_PAGE_CAPTCHA2"), m_nRandPassNumberRegisterPage);
				if (m_pRandTextBoxRegisterPage) m_pRandTextBoxRegisterPage->SetOneLineText(strTemp);
			}
			else
			{
				if (m_nRPUpdateCntRegisterPage++ > 10)
				{
					pNetClient->SndRequestRandomKey();
					m_nRPUpdateCntRegisterPage = 0;
				}
			}
		}
	}

	if (m_pEditBoxManRegisterPage)
	{
		CUIControl* pParent = m_pEditBoxManRegisterPage->GetTopParent();
		if (!pParent)	pParent = this;
		BOOL bFocus = (pParent->IsFocusControl());

		if (bFocus)
		{
			if (UIKeyCheck::GetInstance()->Check(DIK_TAB, DXKEY_DOWN))
			{
				m_pEditBoxManRegisterPage->GoNextTab();
			}
		}
	}
}

void CRegisterPage::ResetAll()
{
	m_pEditBoxManRegisterPage->EndEdit();

	m_pEditBoxManRegisterPage->ClearEdit(REGISTER_PAGE_EDIT_USERID);
	m_pEditBoxManRegisterPage->ClearEdit(REGISTER_PAGE_EDIT_PASSW);
	m_pEditBoxManRegisterPage->ClearEdit(REGISTER_PAGE_EDIT_REPASSW);
	m_pEditBoxManRegisterPage->ClearEdit(REGISTER_PAGE_EDIT_PINCODE);
	m_pEditBoxManRegisterPage->ClearEdit(REGISTER_PAGE_EDIT_EMAIL);
	m_pEditBoxManRegisterPage->ClearEdit(REGISTER_PAGE_EDIT_CAPTCHA1);

	m_nRandPassNumberRegisterPage = 0;
	m_nRPUpdateCntRegisterPage = 0;

	m_pEditBoxManRegisterPage->Init();
	m_pEditBoxManRegisterPage->BeginEdit();

	if (m_pEditBoxManRegisterPage->IsMODE_NATIVE())
	{
		m_pEditBoxManRegisterPage->DoMODE_TOGGLE();
	}
}

void CRegisterPage::SetVisibleSingle(BOOL bVisible)
{
	CUIGroup::SetVisibleSingle(bVisible);

	if (bVisible)
	{
		if (GLCONST_CHAR::nUI_KEYBOARD == 2)
		{
			m_pEditBoxManRegisterPage->DisableKeyInput();
			DXInputString::GetInstance().DisableKeyInput();
		}

		ResetAll();

		if (m_pRandTextBoxRegisterPage) m_pRandTextBoxRegisterPage->ClearText();
		COuterInterface::GetInstance().ResetCancelToLogin();
	}
	else
	{
		m_pEditBoxManRegisterPage->EndEdit();

		if (GLCONST_CHAR::nUI_KEYBOARD == 2)
		{
			m_pEditBoxManRegisterPage->UsableKeyInput();
			DXInputString::GetInstance().UsableKeyInput();
		}

		CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient();
		pNetClient->ResetRandomPassNumber();
	}
}

void CRegisterPage::GoNextTab()
{
	if (m_pEditBoxManRegisterPage)
		m_pEditBoxManRegisterPage->GoNextTab();
}

BOOL CRegisterPage::CheckString(CString strTemp)
{
	strTemp = strTemp.Trim();

	if (STRUTIL::CheckString(strTemp))
	{
		DoModalOuter(ID2GAMEEXTEXT("REGISTER_PAGE_STAGE_6"), MODAL_INFOMATION, OK);
		return FALSE;
	}

	return TRUE;
}

void CRegisterPage::RegisterAccount()
{
	CString strUserID	= m_pEditBoxManRegisterPage->GetEditString(REGISTER_PAGE_EDIT_USERID);
	CString strPassw	= m_pEditBoxManRegisterPage->GetEditString(REGISTER_PAGE_EDIT_PASSW);
	CString strPinCode	= m_pEditBoxManRegisterPage->GetEditString(REGISTER_PAGE_EDIT_PINCODE);
	CString strEmail	= m_pEditBoxManRegisterPage->GetEditString(REGISTER_PAGE_EDIT_EMAIL);
	CString strCaptcha1 = m_pEditBoxManRegisterPage->GetEditString(REGISTER_PAGE_EDIT_CAPTCHA1);

	CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient();

	pNetClient->SndRegisterAccount(
		strUserID.GetString(),
		strPassw.GetString(),
		strPinCode.GetString(),
		strEmail.GetString(),
		strCaptcha1.GetString()
	);
}