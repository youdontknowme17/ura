// Name		: FixUserPage.cpp
// Project	: Lib-RanClientUI

#include "StdAfx.h"
#include "FixUserPage.h"
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

int	CFixUserPage::nLIMIT_USERID		= 12;
int	CFixUserPage::nLIMIT_PASSW		= 12;
int	CFixUserPage::nLIMIT_CAPTCHA	= 4;

CFixUserPage::CFixUserPage()
	: m_pRandTextBoxFixUserPage(NULL)
	, m_nRandPassNumberFixUserPage(0)
	, m_nRPUpdateCntFixUserPage(0)
{
}

CFixUserPage::~CFixUserPage()
{
}

void CFixUserPage::CreateSubControl()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG);

	{
		CBasicTextBox* pTextBox = NULL;
		pTextBox = CreateStaticControl("FIXUSER_PAGE_USERID", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT);
		pTextBox->SetOneLineText((char*)ID2GAMEWORD("FIXUSER_PAGE", 1));

		pTextBox = CreateStaticControl("FIXUSER_PAGE_PASSW", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT);
		pTextBox->SetOneLineText((char*)ID2GAMEWORD("FIXUSER_PAGE", 2));

		pTextBox = CreateStaticControl("FIXUSER_PAGE_CAPTCHA1", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT);
		pTextBox->SetOneLineText((char*)ID2GAMEWORD("FIXUSER_PAGE", 3));

		m_pRandTextBoxFixUserPage = CreateStaticControl("FIXUSER_PAGE_CAPTCHA2", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT);
	}

	{
		CBasicLineBox* pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub(this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pBasicLineBox->CreateBaseBoxOuter("FIXUSER_PAGE_USERID_BACK");
		RegisterControl(pBasicLineBox);

		pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub(this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pBasicLineBox->CreateBaseBoxOuter("FIXUSER_PAGE_PASSW_BACK");
		RegisterControl(pBasicLineBox);

		pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub(this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pBasicLineBox->CreateBaseBoxOuter("FIXUSER_PAGE_CAPTCHA1_BACK");
		RegisterControl(pBasicLineBox);

		pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub(this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pBasicLineBox->CreateBaseBoxOuter("FIXUSER_PAGE_CAPTCHA2_BACK");
		RegisterControl(pBasicLineBox);
	}

	{
		CUIEditBoxMan* pEditBoxManFixUserPage = new CUIEditBoxMan;
		pEditBoxManFixUserPage->CreateSub(this, "FIXUSER_PAGE_EDITMAN", UI_FLAG_DEFAULT, FIXUSER_PAGE_EDITMAN);
		pEditBoxManFixUserPage->CreateEditBox(FIXUSER_PAGE_EDIT_USERID, "FIXUSER_PAGE_EDIT_USERID", "FIXUSER_PAGE_CARRAT", TRUE, UINT_MAX, pFont9, nLIMIT_USERID);
		pEditBoxManFixUserPage->CreateEditBox(FIXUSER_PAGE_EDIT_PASSW, "FIXUSER_PAGE_EDIT_PASSW", "FIXUSER_PAGE_CARRAT", TRUE, UINT_MAX, pFont9, nLIMIT_PASSW);
		pEditBoxManFixUserPage->CreateEditBox(FIXUSER_PAGE_EDIT_CAPTCHA1, "FIXUSER_PAGE_EDIT_CAPTCHA1", "FIXUSER_PAGE_CARRAT", TRUE, UINT_MAX, pFont9, nLIMIT_CAPTCHA);
		pEditBoxManFixUserPage->SetHide(FIXUSER_PAGE_EDIT_PASSW, TRUE);
		RegisterControl(pEditBoxManFixUserPage);
		m_pEditBoxManFixUserPage = pEditBoxManFixUserPage;
	}
	m_pRandTextBoxFixUserPage = CreateStaticControl("REGISTER_PAGE_CAPTCHA2", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT);
	CBasicTextButton* pCancelButton = new CBasicTextButton;
	pCancelButton->CreateSub(this, "BASIC_TEXT_BUTTON24", UI_FLAG_XSIZE, FIXUSER_PAGE_CANCEL);
	pCancelButton->CreateBaseButton("FIXUSER_PAGE_CANCEL", CBasicTextButton::SIZE24, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD("FIXUSER_PAGE", 4));
	pCancelButton->SetShortcutKey(DIK_ESCAPE);
	RegisterControl(pCancelButton);

	CBasicTextButton* pSubmitButton = new CBasicTextButton;
	pSubmitButton->CreateSub(this, "BASIC_TEXT_BUTTON24", UI_FLAG_XSIZE, FIXUSER_PAGE_SUBMIT);
	pSubmitButton->CreateBaseButton("FIXUSER_PAGE_SUBMIT", CBasicTextButton::SIZE24, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD("FIXUSER_PAGE", 5));
	pSubmitButton->SetShortcutKey(DIK_RETURN, DIK_NUMPADENTER);
	RegisterControl(pSubmitButton);
}

void CFixUserPage::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
	switch (ControlID)
	{
	case FIXUSER_PAGE_CANCEL:
	{
		if (CHECK_KEYFOCUSED(dwMsg) || CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient();
			if (pNetClient->IsOnline() == true)
			{
				pNetClient->CloseConnect();
				COuterInterface::GetInstance().SetCancelToLogin();
			}
			else
			{
				if (m_nRandPassNumberFixUserPage == 0)
				{
					m_nRandPassNumberFixUserPage = pNetClient->GetRandomPassNumber();

					if (m_nRandPassNumberFixUserPage != 0)
					{
						CString strTemp;
						strTemp.Format(ID2GAMEEXTEXT("REGISTER_PAGE_CAPTCHA2"), m_nRandPassNumberFixUserPage);
						if (m_pRandTextBoxFixUserPage) m_pRandTextBoxFixUserPage->SetOneLineText(strTemp);
					}
					else
					{
						if (m_nRPUpdateCntFixUserPage++ > 10)
						{
							pNetClient->SndRequestRandomKey();
							m_nRPUpdateCntFixUserPage = 0;
						}
					}
				}
			}

			COuterInterface::GetInstance().ToSelectServerPage(GetWndID());
		}
	}
	break;

	case FIXUSER_PAGE_SUBMIT:
	{
		if (CHECK_KEYFOCUSED(dwMsg) || CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			SetMessageEx(dwMsg &= ~UIMSG_KEY_FOCUSED);

			CString strUserID	= m_pEditBoxManFixUserPage->GetEditString(FIXUSER_PAGE_EDIT_USERID);
			CString strPassw	= m_pEditBoxManFixUserPage->GetEditString(FIXUSER_PAGE_EDIT_PASSW);
			CString strCaptcha1	= m_pEditBoxManFixUserPage->GetEditString(FIXUSER_PAGE_EDIT_CAPTCHA1);
			int strCaptcha2	= m_nRandPassNumberFixUserPage;
			// Field can't empty - STAGE_1
			if (!strUserID.GetLength() || !strPassw.GetLength() || !strCaptcha1.GetLength())
			{
			DoModalOuter(ID2GAMEEXTEXT("CHANGEPASSWORD_PAGE_STAGE_1"));
				return;
			}
			//Add by mcalimosa 09/10/2018
			if (atoi(strCaptcha1) != strCaptcha2)
			{
				DoModalOuter(ID2GAMEEXTEXT("RECAPCHA_NOT_MATCH"));
				return;
			}
			FixUser();

			COuterInterface::GetInstance().SetModalCallWindowID(GetWndID());
		}
	}
	break;
	}
}

void CFixUserPage::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
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
		if (m_nRandPassNumberFixUserPage == 0)
		{
			m_nRandPassNumberFixUserPage = pNetClient->GetRandomPassNumber();

			if (m_nRandPassNumberFixUserPage != 0)
			{
				CString strTemp;
				strTemp.Format(ID2GAMEEXTEXT("FIXUSER_PAGE_CAPTCHA2"), m_nRandPassNumberFixUserPage);
				if (m_pRandTextBoxFixUserPage) m_pRandTextBoxFixUserPage->SetOneLineText(strTemp);
			}
			else
			{
				if (m_nRPUpdateCntFixUserPage++ > 10)
				{
					pNetClient->SndRequestRandomKey();
					m_nRPUpdateCntFixUserPage = 0;
				}
			}
		}
	}

	if (m_pEditBoxManFixUserPage)
	{
		CUIControl* pParent = m_pEditBoxManFixUserPage->GetTopParent();
		if (!pParent)	pParent = this;
		BOOL bFocus = (pParent->IsFocusControl());

		if (bFocus)
		{
			if (UIKeyCheck::GetInstance()->Check(DIK_TAB, DXKEY_DOWN))
			{
				m_pEditBoxManFixUserPage->GoNextTab();
			}
		}
	}
}

void CFixUserPage::SetVisibleSingle(BOOL bVisible)
{
	CUIGroup::SetVisibleSingle(bVisible);

	if (bVisible)
	{
		if (GLCONST_CHAR::nUI_KEYBOARD == 2)
		{
			m_pEditBoxManFixUserPage->DisableKeyInput();
			DXInputString::GetInstance().DisableKeyInput();
		}

		ResetAll();

		if (m_pRandTextBoxFixUserPage) m_pRandTextBoxFixUserPage->ClearText();
		COuterInterface::GetInstance().ResetCancelToLogin();
	}
	else
	{
		m_pEditBoxManFixUserPage->EndEdit();

		if (GLCONST_CHAR::nUI_KEYBOARD == 2)
		{
			m_pEditBoxManFixUserPage->UsableKeyInput();
			DXInputString::GetInstance().UsableKeyInput();
		}

		CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient();
		pNetClient->ResetRandomPassNumber();
	}
}

void CFixUserPage::GoNextTab()
{
	if (m_pEditBoxManFixUserPage)
		m_pEditBoxManFixUserPage->GoNextTab();
}

void CFixUserPage::ResetAll()
{
	m_pEditBoxManFixUserPage->EndEdit();

	m_pEditBoxManFixUserPage->ClearEdit(FIXUSER_PAGE_EDIT_USERID);
	m_pEditBoxManFixUserPage->ClearEdit(FIXUSER_PAGE_EDIT_PASSW);
	m_pEditBoxManFixUserPage->ClearEdit(FIXUSER_PAGE_EDIT_CAPTCHA1);

	m_nRandPassNumberFixUserPage = 0;
	m_nRPUpdateCntFixUserPage = 0;

	m_pEditBoxManFixUserPage->Init();
	m_pEditBoxManFixUserPage->BeginEdit();

	if (m_pEditBoxManFixUserPage->IsMODE_NATIVE())
	{
		m_pEditBoxManFixUserPage->DoMODE_TOGGLE();
	}
}

void CFixUserPage::FixUser()
{
	CString strUserID	= m_pEditBoxManFixUserPage->GetEditString(FIXUSER_PAGE_EDIT_USERID);
	CString strPassw	= m_pEditBoxManFixUserPage->GetEditString(FIXUSER_PAGE_EDIT_PASSW);
	CString strCaptcha1	= m_pEditBoxManFixUserPage->GetEditString(FIXUSER_PAGE_EDIT_CAPTCHA1);

	CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient();

	pNetClient->SndFixUser(
		strUserID.GetString(),
		strPassw.GetString(),
		strCaptcha1.GetString()
	);
}