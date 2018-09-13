// Name		: ItemMallWindowTopUp.h
// Project	: Lib-RanClientUI

#include "StdAfx.h"
#include "ItemMallTopUpWindow.h"
#include "../EngineLib/DxCommon/DxFontMan.h"
#include "BasicTextButton.h"
#include "BasicTextBoxEx.h"
#include "BasicLineBox.h"
#include "../EngineUIlib/GUInterface/UIEditBoxMan.h"
#include "DxGlobalStage.h"
#include "GameTextControl.h"
#include "UITextControl.h"
#include "InnerInterface.h"
#include "ModalWindow.h"
#include "../EngineUILib/GUInterface/UIKeyCheck.h"
#include "DXInputString.h"
#include "GLGaeaClient.h"
#include "GLCharacter.h"

#include <cstdlib>
#include <ctime>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int	CTopUpWindow::nLIMIT_VOC = 4;

CTopUpWindow::CTopUpWindow()
	: m_pTopUpTxtCaptcha2(NULL)
	, m_pTopUpCaptcha2(0)
{
}

CTopUpWindow::~CTopUpWindow()
{
}

void CTopUpWindow::CreateSubControl()
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG);
	const int nAlignLeft = TEXT_ALIGN_LEFT;
	const DWORD& dwGold = NS_UITEXTCOLOR::GOLD;

	//-------------------------------------------------------------------------------------
	//
	//	T  O  P    U  P    L  B  O  X
	//
	//-------------------------------------------------------------------------------------

	// UIInnerCfg04 : TOPUP_LBOX
	{
		CBasicLineBox* pTopUpLBox = NULL;
		pTopUpLBox = new CBasicLineBox;
		pTopUpLBox->CreateSub(this, "BASIC_LINE_BOX_OPTION", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pTopUpLBox->CreateBaseBoxOption("TOPUP_LBOX");
		RegisterControl(pTopUpLBox);
	}

	// UIInnerCfg04 : TOPUP_VOCID_LBOX1
	{
		CBasicLineBox* pVocIDLBox1 = NULL;
		pVocIDLBox1 = new CBasicLineBox;
		pVocIDLBox1->CreateSub(this, "BASIC_LINE_BOX_OPTION", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pVocIDLBox1->CreateBaseBoxOption("TOPUP_VOCID_LBOX1");
		RegisterControl(pVocIDLBox1);
	}

	// UIInnerCfg04 : TOPUP_VOCID_LBOX2
	{
		CBasicLineBox* pVocIDLBox2 = NULL;
		pVocIDLBox2 = new CBasicLineBox;
		pVocIDLBox2->CreateSub(this, "BASIC_LINE_BOX_OPTION", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pVocIDLBox2->CreateBaseBoxOption("TOPUP_VOCID_LBOX2");
		RegisterControl(pVocIDLBox2);
	}

	// UIInnerCfg04 : TOPUP_VOCCODES_LBOX1
	{
		CBasicLineBox* pVocCodesLBox1 = NULL;
		pVocCodesLBox1 = new CBasicLineBox;
		pVocCodesLBox1->CreateSub(this, "BASIC_LINE_BOX_OPTION", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pVocCodesLBox1->CreateBaseBoxOption("TOPUP_VOCCODES_LBOX1");
		RegisterControl(pVocCodesLBox1);
	}

	// UIInnerCfg04 : TOPUP_VOCCODES_LBOX2
	{
		CBasicLineBox* pVocCodesLBox2 = NULL;
		pVocCodesLBox2 = new CBasicLineBox;
		pVocCodesLBox2->CreateSub(this, "BASIC_LINE_BOX_OPTION", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pVocCodesLBox2->CreateBaseBoxOption("TOPUP_VOCCODES_LBOX2");
		RegisterControl(pVocCodesLBox2);
	}

	// UIInnerCfg04 : TOPUP_VOCCODES_LBOX3
	{
		CBasicLineBox* pVocCodesLBox3 = NULL;
		pVocCodesLBox3 = new CBasicLineBox;
		pVocCodesLBox3->CreateSub(this, "BASIC_LINE_BOX_OPTION", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pVocCodesLBox3->CreateBaseBoxOption("TOPUP_VOCCODES_LBOX3");
		RegisterControl(pVocCodesLBox3);
	}

	// UIInnerCfg04 : TOPUP_CAPTCHA_LBOX1
	{
		CBasicLineBox* pCaptchaLBox1 = NULL;
		pCaptchaLBox1 = new CBasicLineBox;
		pCaptchaLBox1->CreateSub(this, "BASIC_LINE_BOX_OPTION", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pCaptchaLBox1->CreateBaseBoxOption("TOPUP_CAPTCHA_LBOX1");
		RegisterControl(pCaptchaLBox1);
	}

	// UIInnerCfg04 : TOPUP_CAPTCHA_LBOX2
	{
		CBasicLineBox* pCaptchaLBox2 = NULL;
		pCaptchaLBox2 = new CBasicLineBox;
		pCaptchaLBox2->CreateSub(this, "BASIC_LINE_BOX_OPTION", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pCaptchaLBox2->CreateBaseBoxOption("TOPUP_CAPTCHA_LBOX2");
		RegisterControl(pCaptchaLBox2);
	}

	//-------------------------------------------------------------------------------------
	//
	//	V  O  U  C  H  E  R    I  D
	//
	//-------------------------------------------------------------------------------------

	// UIInnerCfg04 : TOPUP_TXT_VOCID_TITLE			| GameWord02 : TOPUP_WINDOW, 1
	{
		m_pTopUpTxtVocIDTitle = CreateStaticControl("TOPUP_TXT_VOCID_TITLE", pFont8, nAlignLeft);
		m_pTopUpTxtVocIDTitle->AddText(ID2GAMEWORD("TOPUP_WINDOW", 1), dwGold);
	}

	// UIInnerCfg04 : TOPUP_TXT_VOCID_SEP			| GameWord02 : TOPUP_WINDOW, 2
	{
		m_pTopUpTxtVocIDSep = CreateStaticControl("TOPUP_TXT_VOCID_SEP", pFont8, nAlignLeft);
		m_pTopUpTxtVocIDSep->AddText(ID2GAMEWORD("TOPUP_WINDOW", 2), dwGold);
	}

	//-------------------------------------------------------------------------------------
	//
	//	V  O  U  C  H  E  R    C  O  D  E  S
	//
	//-------------------------------------------------------------------------------------

	// UIInnerCfg04 : TOPUP_TXT_VOCCODES_TITLE		| GameWord02 : TOPUP_WINDOW, 3
	{
		m_pTopUpTxtVocCodesTitle = CreateStaticControl("TOPUP_TXT_VOCCODES_TITLE", pFont8, nAlignLeft);
		m_pTopUpTxtVocCodesTitle->AddText(ID2GAMEWORD("TOPUP_WINDOW", 3), dwGold);
	}

	// UIInnerCfg04 : TOPUP_TXT_VOCCODES_SEP1		| GameWord02 : TOPUP_WINDOW, 2
	{
		m_pTopUpTxtVocCodesSep1 = CreateStaticControl("TOPUP_TXT_VOCCODES_SEP1", pFont8, nAlignLeft);
		m_pTopUpTxtVocCodesSep1->AddText(ID2GAMEWORD("TOPUP_WINDOW", 2), dwGold);
	}

	// UIInnerCfg04 : TOPUP_TXT_VOCCODES_SEP2		| GameWord02 : TOPUP_WINDOW, 2
	{
		m_pTopUpTxtVocCodesSep2 = CreateStaticControl("TOPUP_TXT_VOCCODES_SEP2", pFont8, nAlignLeft);
		m_pTopUpTxtVocCodesSep2->AddText(ID2GAMEWORD("TOPUP_WINDOW", 2), dwGold);
	}

	//-------------------------------------------------------------------------------------
	//
	//	C  A  P  T  C  H  A
	//
	//-------------------------------------------------------------------------------------

	// UIInnerCfg04 : TOPUP_TXT_CAPTCHA_TITLE		| GameWord02 : TOPUP_WINDOW, 4
	{
		m_pTopUpTxtCaptchaTitle = CreateStaticControl("TOPUP_TXT_CAPTCHA_TITLE", pFont8, nAlignLeft);
		m_pTopUpTxtCaptchaTitle->AddText(ID2GAMEWORD("TOPUP_WINDOW", 4), dwGold);
	}

	// UIInnerCfg04 : TOPUP_TXT_CAPTCHA_SEP			| GameWord02 : TOPUP_WINDOW, 2
	{
		m_pTopUpTxtCaptchaSep = CreateStaticControl("TOPUP_TXT_CAPTCHA_SEP", pFont8, nAlignLeft);
		m_pTopUpTxtCaptchaSep->AddText(ID2GAMEWORD("TOPUP_WINDOW", 2), dwGold);
	}

	// UIInnerCfg04 : TOPUP_TXT_CAPTCHA_2
	{
		m_pTopUpTxtCaptcha2 = CreateStaticControl("TOPUP_TXT_CAPTCHA_2", pFont8, nAlignLeft);
	}

	//-------------------------------------------------------------------------------------
	//
	//	E  D  I  T    B  O  X
	//
	//-------------------------------------------------------------------------------------

	// UIInnerCfg04 : TOPUP_EDITMAN
	// UIInnerCfg04 : TOPUP_CARRAT
	// UIInnerCfg04 : TOPUP_EDITBOX0
	// UIInnerCfg04 : TOPUP_EDITBOX1
	// UIInnerCfg04 : TOPUP_EDITBOX2
	// UIInnerCfg04 : TOPUP_EDITBOX3
	// UIInnerCfg04 : TOPUP_EDITBOX4
	// UIInnerCfg04 : TOPUP_EDITBOX5
	{
		CUIEditBoxMan* pTopUpEditBoxMan = new CUIEditBoxMan;
		pTopUpEditBoxMan->CreateSub(this, "TOPUP_EDITMAN", UI_FLAG_DEFAULT, TOPUP_EDITMAN);
		pTopUpEditBoxMan->CreateEditBox(TOPUP_EDITBOX0, "TOPUP_EDITBOX0", "TOPUP_CARRAT", TRUE, UINT_MAX, pFont8, nLIMIT_VOC);
		pTopUpEditBoxMan->CreateEditBox(TOPUP_EDITBOX1, "TOPUP_EDITBOX1", "TOPUP_CARRAT", TRUE, UINT_MAX, pFont8, nLIMIT_VOC);
		pTopUpEditBoxMan->CreateEditBox(TOPUP_EDITBOX2, "TOPUP_EDITBOX2", "TOPUP_CARRAT", TRUE, UINT_MAX, pFont8, nLIMIT_VOC);
		pTopUpEditBoxMan->CreateEditBox(TOPUP_EDITBOX3, "TOPUP_EDITBOX3", "TOPUP_CARRAT", TRUE, UINT_MAX, pFont8, nLIMIT_VOC);
		pTopUpEditBoxMan->CreateEditBox(TOPUP_EDITBOX4, "TOPUP_EDITBOX4", "TOPUP_CARRAT", TRUE, UINT_MAX, pFont8, nLIMIT_VOC);
		pTopUpEditBoxMan->CreateEditBox(TOPUP_EDITBOX5, "TOPUP_EDITBOX5", "TOPUP_CARRAT", TRUE, UINT_MAX, pFont8, nLIMIT_VOC);
		RegisterControl(pTopUpEditBoxMan);
		m_pTopUpEditBoxMan = pTopUpEditBoxMan;
	}

	//-------------------------------------------------------------------------------------
	//
	//	T  O  P    U  P    B  U  T  T  O  N
	//
	//-------------------------------------------------------------------------------------

	// UIInnerCfg04 : TOPUP_BTN_SUBMIT				| GameWord02 : TOPUP_WINDOW, 5
	{
		CBasicTextButton* pSubmitBtn = new CBasicTextButton;
		pSubmitBtn->CreateSub(this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, TOPUP_BTN_SUBMIT);
		pSubmitBtn->CreateBaseButton("TOPUP_BTN_SUBMIT", CBasicTextButton::SIZE18, CBasicButton::CLICK_FLIP, (char*)ID2GAMEWORD("TOPUP_WINDOW", 5));
		pSubmitBtn->SetShortcutKey(DIK_RETURN, DIK_NUMPADENTER);
		RegisterControl(pSubmitBtn);
	}
}

CBasicTextBox* CTopUpWindow::CreateStaticControl(char* szControlKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID)
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub(this, szControlKeyword, UI_FLAG_DEFAULT, cID);
	pStaticText->SetFont(pFont);
	pStaticText->SetTextAlign(nAlign);
	RegisterControl(pStaticText);

	return pStaticText;
}

void CTopUpWindow::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
	CUIWindowEx::TranslateUIMessage(ControlID, dwMsg);

	switch (ControlID)
	{
	case ET_CONTROL_TITLE:
	case ET_CONTROL_TITLE_F:
	{
		if ((dwMsg & UIMSG_LB_DUP) && CHECK_MOUSE_IN(dwMsg))
		{
			CInnerInterface::GetInstance().SetDefaultPosInterface(TOPUP_WINDOW);
		}
	}
	break;

	case ET_CONTROL_BUTTON:
	{
		if (CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			CInnerInterface::GetInstance().SetItemMallTopUpWindowClose();
		}
	}
	break;

	case TOPUP_BTN_SUBMIT:
	{
		if (CHECK_KEYFOCUSED(dwMsg) || CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			SetMessageEx(dwMsg &= ~UIMSG_KEY_FOCUSED);

			CString strVocID1 = m_pTopUpEditBoxMan->GetEditString(TOPUP_EDITBOX0);
			CString strVocID2 = m_pTopUpEditBoxMan->GetEditString(TOPUP_EDITBOX1);

			CString strVocCodes1 = m_pTopUpEditBoxMan->GetEditString(TOPUP_EDITBOX2);
			CString strVocCodes2 = m_pTopUpEditBoxMan->GetEditString(TOPUP_EDITBOX3);
			CString strVocCodes3 = m_pTopUpEditBoxMan->GetEditString(TOPUP_EDITBOX4);

			CString strCaptcha1 = m_pTopUpEditBoxMan->GetEditString(TOPUP_EDITBOX5);

			// GameExText02 : TOPUP_WINDOW_STAGE_1		| Field Can't be Empty
			{
				if (!strVocID1.GetLength() ||
					!strVocID2.GetLength() ||
					!strVocCodes1.GetLength() ||
					!strVocCodes2.GetLength() ||
					!strVocCodes3.GetLength() ||
					!strCaptcha1.GetLength())
				{
					DoModal(ID2GAMEEXTEXT("TOPUP_WINDOW_STAGE_1"));
					return;
				}
			}

			// GameExText02 : TOPUP_WINDOW_STAGE_2		| Wrong Captcha		[ClientSide]
			{
				CString strCaptcha2;
				strCaptcha2.Format("%d", m_pTopUpCaptcha2);

				if (strCaptcha1 != strCaptcha2)
				{
					DoModal(ID2GAMEEXTEXT("TOPUP_WINDOW_STAGE_2"));
					return;
				}
			}

			TopUp();

			CInnerInterface::GetInstance().SetItemMallTopUpWindowClose();
		}
	}
	break;
	}
}

void CTopUpWindow::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
	CUIWindowEx::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);

	if (m_pTopUpEditBoxMan)
	{
		CUIControl* pParent = m_pTopUpEditBoxMan->GetTopParent();
		if (!pParent)	pParent = this;
		BOOL bFocus = (pParent->IsFocusControl());

		if (bFocus)
		{
			if (UIKeyCheck::GetInstance()->Check(DIK_TAB, DXKEY_DOWN))
			{
				m_pTopUpEditBoxMan->GoNextTab();
			}
			else if (UIKeyCheck::GetInstance()->Check(DIK_ESCAPE, DXKEY_DOWN))
			{
				CInnerInterface::GetInstance().SetItemMallTopUpWindowClose();
			}
		}
	}

	SetCaptcha();
}

void CTopUpWindow::ResetAll()
{
	m_pTopUpEditBoxMan->EndEdit();

	m_pTopUpEditBoxMan->ClearEdit(TOPUP_EDITBOX0);
	m_pTopUpEditBoxMan->ClearEdit(TOPUP_EDITBOX1);
	m_pTopUpEditBoxMan->ClearEdit(TOPUP_EDITBOX2);
	m_pTopUpEditBoxMan->ClearEdit(TOPUP_EDITBOX3);
	m_pTopUpEditBoxMan->ClearEdit(TOPUP_EDITBOX4);
	m_pTopUpEditBoxMan->ClearEdit(TOPUP_EDITBOX5);

	if (m_pTopUpCaptcha2 != 0)
	{
		m_pTopUpCaptcha2 = 0;
	}
}

void CTopUpWindow::SetCaptcha()
{
	if (m_pTopUpCaptcha2 == 0)
	{
		m_pTopUpCaptcha2 = MakeRandomNumber(899);

		if (m_pTopUpCaptcha2 != 0)
		{
			const DWORD& dwLBlue = NS_UITEXTCOLOR::LIGHTSKYBLUE;
			CString strTemp;
			//GameExText02 : TOPUP_WINDOW_CAPTCHA2		| %d
			strTemp.Format(ID2GAMEEXTEXT("TOPUP_WINDOW_CAPTCHA2"), m_pTopUpCaptcha2);
			if (m_pTopUpTxtCaptcha2) m_pTopUpTxtCaptcha2->SetOneLineText(strTemp, dwLBlue);
		}
	}
}

int CTopUpWindow::MakeRandomNumber(int nMax)
{
	return (rand() % nMax + 100);
}

void CTopUpWindow::TopUp()
{
	CString strSep = "-";

	CString strVocID1 = m_pTopUpEditBoxMan->GetEditString(TOPUP_EDITBOX0);
	CString strVocID2 = m_pTopUpEditBoxMan->GetEditString(TOPUP_EDITBOX1);

	CString strVocCodes1 = m_pTopUpEditBoxMan->GetEditString(TOPUP_EDITBOX2);
	CString strVocCodes2 = m_pTopUpEditBoxMan->GetEditString(TOPUP_EDITBOX3);
	CString strVocCodes3 = m_pTopUpEditBoxMan->GetEditString(TOPUP_EDITBOX4);

	CString strCaptcha1 = m_pTopUpEditBoxMan->GetEditString(TOPUP_EDITBOX5);
	CString strCaptcha2;
	strCaptcha2.Format("%d", m_pTopUpCaptcha2);

	CString strVocID = strVocID1 + strSep + strVocID2;
	CString strVocCodes = strVocCodes1 + strSep + strVocCodes2 + strSep + strVocCodes3;

	CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient();

	pNetClient->SndTopUp(
		strVocID.GetString(),
		strVocCodes.GetString(),
		strCaptcha1.GetString(),
		strCaptcha2.GetString()
	);
}