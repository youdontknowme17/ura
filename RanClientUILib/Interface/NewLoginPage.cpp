// Name		: NewLoginPage.cpp
// Project	: Lib-RanClientUI

#include "StdAfx.h"
#include "NewLoginPage.h"
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

int	CNewLoginPage::nLIMIT_USERID	= 12;
int	CNewLoginPage::nLIMIT_PASSW		= 12;
int	CNewLoginPage::nLIMIT_CAPTCHA	= 4;

CNewLoginPage::CNewLoginPage()
	: m_pIDSaveButton(NULL)
	, m_pRandTextBoxLoginPage(NULL)
	, m_nRandPassNumberLoginPage(0)
	, m_nRPUpdateCntLoginPage(0)
{
}

CNewLoginPage::~CNewLoginPage()
{
}

void CNewLoginPage::CreateSubControl()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG);

	CBasicTextBox* pTextBox = NULL;
	pTextBox = CreateStaticControl("NEW_LOGIN_PAGE_USERID", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT);
	pTextBox->SetOneLineText((char*)ID2GAMEWORD("NEW_LOGIN_PAGE", 1));

	pTextBox = CreateStaticControl("NEW_LOGIN_PAGE_PASSW", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT);
	pTextBox->SetOneLineText((char*)ID2GAMEWORD("NEW_LOGIN_PAGE", 2));

	pTextBox = CreateStaticControl("NEW_LOGIN_PAGE_CAPTCHA1", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT);
	pTextBox->SetOneLineText((char*)ID2GAMEWORD("NEW_LOGIN_PAGE", 3));
	
	{
		CBasicLineBox* pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub(this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pBasicLineBox->CreateBaseBoxOuter("NEW_LOGIN_PAGE_USERID_BACK");
		RegisterControl(pBasicLineBox);

		pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub(this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pBasicLineBox->CreateBaseBoxOuter("NEW_LOGIN_PAGE_PASSW_BACK");
		RegisterControl(pBasicLineBox);

		pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub(this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pBasicLineBox->CreateBaseBoxOuter("NEW_LOGIN_PAGE_CAPTCHA1_BACK");
		RegisterControl(pBasicLineBox);

		pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub(this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pBasicLineBox->CreateBaseBoxOuter("NEW_LOGIN_PAGE_CAPTCHA2_BACK");
		RegisterControl(pBasicLineBox);
	}

	{
		CUIEditBoxMan* pEditBoxManLoginPage = new CUIEditBoxMan;
		pEditBoxManLoginPage->CreateSub(this, "NEW_LOGIN_PAGE_EDITMAN", UI_FLAG_DEFAULT, NEW_LOGIN_PAGE_EDITMAN);
		pEditBoxManLoginPage->CreateEditBox(NEW_LOGIN_PAGE_EDIT_USERID,		"NEW_LOGIN_PAGE_EDIT_USERID",	"NEW_LOGIN_PAGE_CARRAT", TRUE, UINT_MAX, pFont9, nLIMIT_USERID);
		pEditBoxManLoginPage->CreateEditBox(NEW_LOGIN_PAGE_EDIT_PASSW,		"NEW_LOGIN_PAGE_EDIT_PASSW",	"NEW_LOGIN_PAGE_CARRAT", TRUE, UINT_MAX, pFont9, nLIMIT_PASSW);
		pEditBoxManLoginPage->CreateEditBox(NEW_LOGIN_PAGE_EDIT_CAPTCHA1,	"NEW_LOGIN_PAGE_EDIT_CAPTCHA1",	"NEW_LOGIN_PAGE_CARRAT", TRUE, UINT_MAX, pFont9, nLIMIT_CAPTCHA);
		pEditBoxManLoginPage->SetHide(NEW_LOGIN_PAGE_EDIT_PASSW, TRUE);
		RegisterControl(pEditBoxManLoginPage);
		m_pEditBoxManLoginPage = pEditBoxManLoginPage;
	}

	{
		m_pIDSaveButton = CreateFlipButton("NEW_LOGIN_PAGE_IDSAVE_BUTTON", "NEW_LOGIN_PAGE_IDSAVE_BUTTON_F", NEW_LOGIN_PAGE_IDSAVE_BUTTON);
		pTextBox = CreateStaticControl("NEW_LOGIN_PAGE_IDSAVE_BACK", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT);
		pTextBox->SetOneLineText((char*)ID2GAMEWORD("NEW_LOGIN_PAGE", 4));
	}

	CBasicTextButton* pCancelButton = new CBasicTextButton;
	pCancelButton->CreateSub(this, "BASIC_TEXT_BUTTON24", UI_FLAG_XSIZE, NEW_LOGIN_PAGE_CANCEL);
	pCancelButton->CreateBaseButton("NEW_LOGIN_PAGE_CANCEL", CBasicTextButton::SIZE24, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD("NEW_LOGIN_PAGE", 5));
	pCancelButton->SetShortcutKey(DIK_ESCAPE);
	RegisterControl(pCancelButton);

	CBasicTextButton* pSubmitButton = new CBasicTextButton;
	pSubmitButton->CreateSub(this, "BASIC_TEXT_BUTTON24", UI_FLAG_XSIZE, NEW_LOGIN_PAGE_SUBMIT);
	pSubmitButton->CreateBaseButton("NEW_LOGIN_PAGE_SUBMIT", CBasicTextButton::SIZE24, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD("NEW_LOGIN_PAGE", 6));
	pSubmitButton->SetShortcutKey(DIK_RETURN, DIK_NUMPADENTER);
	RegisterControl(pSubmitButton);

	m_pRandTextBoxLoginPage = CreateStaticControl("NEW_LOGIN_PAGE_CAPTCHA2", pFont9, NS_UITEXTCOLOR::WHITE, TEXT_ALIGN_LEFT);

	LoadIDSaveButton();
}

void CNewLoginPage::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
	switch (ControlID)
	{
	case NEW_LOGIN_PAGE_IDSAVE_BUTTON:
	{
		if (CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			if (m_pIDSaveButton)
			{
				RANPARAM::bSAVE_USERID = !RANPARAM::bSAVE_USERID;
				RANPARAM::SAVE();
				LoadIDSaveButton();
			}
		}
	}
	break;

	case NEW_LOGIN_PAGE_CANCEL:
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

	case NEW_LOGIN_PAGE_SUBMIT:
	{
		if (CHECK_KEYFOCUSED(dwMsg) || CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			SetMessageEx(dwMsg &= ~UIMSG_KEY_FOCUSED);

			CString strUserID = m_pEditBoxManLoginPage->GetEditString(NEW_LOGIN_PAGE_EDIT_USERID);
			CString strPassw = m_pEditBoxManLoginPage->GetEditString(NEW_LOGIN_PAGE_EDIT_PASSW);
			CString strCaptcha1 = m_pEditBoxManLoginPage->GetEditString(NEW_LOGIN_PAGE_EDIT_CAPTCHA1);

			if (!strUserID.GetLength() || !strPassw.GetLength() || !strCaptcha1.GetLength())
			{
				DoModalOuter(ID2GAMEEXTEXT("LOGINSTAGE_5"));
				return;
			}

			CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient();

			int nServerGroup, nServerChannel;
			COuterInterface::GetInstance().GetConnectServerInfo(nServerGroup, nServerChannel);

			pNetClient->ChinaSndLogin(strUserID, strPassw, strCaptcha1, nServerChannel);

			RANPARAM::SETUSERID(strUserID);
			RANPARAM::SAVE();

			DoModalOuter(ID2GAMEEXTEXT("LOGINSTAGE_6"), MODAL_INFOMATION, CANCEL, OUTER_MODAL_WAITCONFIRM);

			COuterInterface::GetInstance().SetModalCallWindowID(GetWndID());
		}
	}
	break;
	}
}

void CNewLoginPage::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
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
		if (m_nRandPassNumberLoginPage == 0)
		{
			m_nRandPassNumberLoginPage = pNetClient->GetRandomPassNumber();

			if (m_nRandPassNumberLoginPage != 0)
			{
				CString strTemp;
				strTemp.Format(ID2GAMEEXTEXT("NEW_LOGIN_PAGE_CAPTCHA2"), m_nRandPassNumberLoginPage);
				if (m_pRandTextBoxLoginPage) m_pRandTextBoxLoginPage->SetOneLineText(strTemp);
			}
			else
			{
				if (m_nRPUpdateCntLoginPage++ > 10)
				{
					pNetClient->SndRequestRandomKey();
					m_nRPUpdateCntLoginPage = 0;
				}
			}
		}
	}

	if (m_pEditBoxManLoginPage)
	{
		CUIControl* pParent = m_pEditBoxManLoginPage->GetTopParent();
		if (!pParent)	pParent = this;
		BOOL bFocus = (pParent->IsFocusControl());

		if (bFocus)
		{
			if (UIKeyCheck::GetInstance()->Check(DIK_TAB, DXKEY_DOWN))
			{
				m_pEditBoxManLoginPage->GoNextTab();
			}
		}
	}
}

void CNewLoginPage::ResetAll()
{
	m_pEditBoxManLoginPage->EndEdit();

	m_pEditBoxManLoginPage->ClearEdit(NEW_LOGIN_PAGE_EDIT_USERID);
	m_pEditBoxManLoginPage->ClearEdit(NEW_LOGIN_PAGE_EDIT_PASSW);
	m_pEditBoxManLoginPage->ClearEdit(NEW_LOGIN_PAGE_EDIT_CAPTCHA1);

	m_nRandPassNumberLoginPage	= 0;
	m_nRPUpdateCntLoginPage		= 0;

	m_pEditBoxManLoginPage->Init();
	m_pEditBoxManLoginPage->BeginEdit();

	if (m_pEditBoxManLoginPage->IsMODE_NATIVE())
	{
		m_pEditBoxManLoginPage->DoMODE_TOGGLE();
	}

	std::string& strSavedUserID = RANPARAM::GETUSERID_DEC();
	if (strSavedUserID.size())
	{
		m_pEditBoxManLoginPage->SetEditString(NEW_LOGIN_PAGE_EDIT_USERID, CString(strSavedUserID.c_str()));
		m_pEditBoxManLoginPage->GoNextTab();
	}
}

void CNewLoginPage::SetVisibleSingle(BOOL bVisible)
{
	CUIGroup::SetVisibleSingle(bVisible);

	if (bVisible)
	{
		if (GLCONST_CHAR::nUI_KEYBOARD == 2)
		{
			m_pEditBoxManLoginPage->DisableKeyInput();
			DXInputString::GetInstance().DisableKeyInput();
		}

		ResetAll();

		if (m_pRandTextBoxLoginPage) m_pRandTextBoxLoginPage->ClearText();
		COuterInterface::GetInstance().ResetCancelToLogin();
	}
	else
	{
		m_pEditBoxManLoginPage->EndEdit();

		if (GLCONST_CHAR::nUI_KEYBOARD == 2)
		{
			m_pEditBoxManLoginPage->UsableKeyInput();
			DXInputString::GetInstance().UsableKeyInput();
		}

		CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient();
		pNetClient->ResetRandomPassNumber();
	}
}

void CNewLoginPage::GoNextTab()
{
	if (m_pEditBoxManLoginPage)
		m_pEditBoxManLoginPage->GoNextTab();
}

CBasicButton* CNewLoginPage::CreateFlipButton(char* szButton, char* szButtonFlip, UIGUID ControlID)
{
	CBasicButton* pButton = new CBasicButton;
	pButton->CreateSub(this, szButton, UI_FLAG_DEFAULT, ControlID);
	pButton->CreateFlip(szButtonFlip, CBasicButton::RADIO_FLIP);
	pButton->SetControlNameEx(szButton);
	RegisterControl(pButton);

	return pButton;
}

void CNewLoginPage::LoadIDSaveButton()
{
	BOOL bIDSave = RANPARAM::bSAVE_USERID;
	m_pIDSaveButton->SetFlip(bIDSave);
}

void CNewLoginPage::SetCharToEditBox(TCHAR cKey)
{
	if (!m_pEditBoxManLoginPage) return;

	CString strTemp;

	UIGUID nID = m_pEditBoxManLoginPage->GetBeginEditBox();

	strTemp = m_pEditBoxManLoginPage->GetEditString(nID);
	strTemp += cKey;

	m_pEditBoxManLoginPage->SetEditString(nID, strTemp);
}

void CNewLoginPage::DelCharToEditBox()
{
	if (!m_pEditBoxManLoginPage) return;

	CString strTemp;

	UIGUID nID = m_pEditBoxManLoginPage->GetBeginEditBox();

	strTemp = m_pEditBoxManLoginPage->GetEditString(nID);
	INT nLenth = strTemp.GetLength();
	strTemp = strTemp.Left(nLenth - 1);

	m_pEditBoxManLoginPage->SetEditString(nID, strTemp);
}