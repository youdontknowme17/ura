// Name		: GmcVocWnd.cpp
// Project	: Lib-RanClientUI

#include "StdAfx.h"
#include "GmcVocWnd.h"
#include "BasicLineBox.h"
#include "BasicTextButton.h"
#include "BasicComboBox.h"
#include "BasicComboBoxRollOver.h"
#include "ModalWindow.h"
#include "../EngineLib/DxCommon/DxFontMan.h"
#include "GameTextControl.h"
#include "UITextControl.h"
#include "../EngineUILib/GUInterface/BasicTextBox.h"
#include "../EngineUIlib/GUInterface/UIEditBoxMan.h"
#include "DXInputString.h"
#include "GLogicData.h"
#include "../EngineUILib/GUInterface/UIKeyCheck.h"
#include "InnerInterface.h"
#include "DxGlobalStage.h"
#include "GLGaeaClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int CGmcVocWnd::nOUTOFRANGE = -1;

int	CGmcVocWnd::nLIMIT_NOMINAL = 6;
int	CGmcVocWnd::nLIMIT_SRCH = 9;

CGmcVocWnd::CGmcVocWnd()
	: m_nBtnIndex1(nOUTOFRANGE)
	, m_nBtnIndex2(nOUTOFRANGE)
{
	memset(m_pSrchByBtn1, 0, sizeof(CBasicButton*) * nMAXBTN1);
	memset(m_pSrchByBtn2, 0, sizeof(CBasicButton*) * nMAXBTN2);
}

CGmcVocWnd::~CGmcVocWnd()
{
}

void CGmcVocWnd::CreateSubControl()
{
	CD3DFontPar* pFont10 = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 10, _DEFAULT_FONT_SHADOW_FLAG);
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG);
	const int nAlignLeft = TEXT_ALIGN_LEFT;
	const DWORD& dwGold = NS_UITEXTCOLOR::GOLD;
	const DWORD& dwLBlue = NS_UITEXTCOLOR::LIGHTSKYBLUE;

	// UinnerCfg2 : GMC_VOC_WND_BG
	{
		m_pGmcVocWnd_BG = new CUIControl;
		m_pGmcVocWnd_BG->CreateSub(this, "GMC_VOC_WND_BG", UI_FLAG_DEFAULT);
		m_pGmcVocWnd_BG->SetVisibleSingle(TRUE);
		RegisterControl(m_pGmcVocWnd_BG);
	}

	//----------------------------------------------------------------------------------------------------
	//										G E N E R A T E   V O U C H E R  
	//----------------------------------------------------------------------------------------------------

	// UinnerCfg2 : GMC_VOC_WND_GENVOC_LBOX
	{
		CBasicLineBox* pGenVocLBox = NULL;
		pGenVocLBox = new CBasicLineBox;
		pGenVocLBox->CreateSub(this, "BASIC_LINE_BOX_OPTION", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pGenVocLBox->CreateBaseBoxOption("GMC_VOC_WND_GENVOC_LBOX");
		RegisterControl(pGenVocLBox);
	}

	// UinnerCfg2 : GMC_VOC_WND_GENVOC_TXT_TITLE			| GameWord : GMC_VOC_WND_GENVOC, 0
	{
		m_pGenVocTxtTitle = CreateStaticControl("GMC_VOC_WND_GENVOC_TXT_TITLE", pFont8, nAlignLeft);
		m_pGenVocTxtTitle->AddText(ID2GAMEWORD("GMC_VOC_WND_GENVOC", 0), dwGold);
	}

	// UinnerCfg2 : GMC_VOC_WND_GENVOC_TXT_NOMINAL			| GameWord : GMC_VOC_WND_GENVOC, 1
	{
		m_pGenVocTxtNominal = CreateStaticControl("GMC_VOC_WND_GENVOC_TXT_NOMINAL", pFont8, nAlignLeft);
		m_pGenVocTxtNominal->AddText(ID2GAMEWORD("GMC_VOC_WND_GENVOC", 1), dwLBlue);
	}

	// UinnerCfg2 : GMC_VOC_WND_GENVOC_TXT_COLON			| GameWord : GMC_VOC_WND_GENVOC, 2
	{
		m_pGenVocTxtColon = CreateStaticControl("GMC_VOC_WND_GENVOC_TXT_COLON", pFont8, nAlignLeft);
		m_pGenVocTxtColon->AddText(ID2GAMEWORD("GMC_VOC_WND_GENVOC", 2), dwLBlue);
	}

	// UinnerCfg2 : GMC_VOC_WND_GENVOC_LBOX_NOMINAL
	{
		CBasicLineBox* pGenVocLBoxNominal = NULL;
		pGenVocLBoxNominal = new CBasicLineBox;
		pGenVocLBoxNominal->CreateSub(this, "BASIC_LINE_BOX_OPTION", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pGenVocLBoxNominal->CreateBaseBoxOption("GMC_VOC_WND_GENVOC_LBOX_NOMINAL");
		RegisterControl(pGenVocLBoxNominal);
	}

	// UinnerCfg2 : GMC_VOC_WND_GENVOC_BTN_GENVOC			| GameWord : GMC_VOC_WND_GENVOC, 3
	{
		CBasicTextButton* pGenVocBtn = new CBasicTextButton;

		pGenVocBtn->CreateSub(
			this,
			"BASIC_TEXT_BUTTON24",
			UI_FLAG_XSIZE,
			GMC_VOC_WND_GENVOC_BTN_GENVOC
		);

		pGenVocBtn->CreateBaseButton(
			"GMC_VOC_WND_GENVOC_BTN_GENVOC",
			CBasicTextButton::SIZE24,
			CBasicButton::CLICK_FLIP,
			(char*)ID2GAMEWORD("GMC_VOC_WND_GENVOC", 3)
		);

		RegisterControl(pGenVocBtn);
	}

	//----------------------------------------------------------------------------------------------------
	//										S E AR C H    V O U C H E R
	//----------------------------------------------------------------------------------------------------

	// UinnerCfg2 : GMC_VOC_WND_SRCHVOC_LBOX
	{
		CBasicLineBox* pSrchVocLBox = NULL;
		pSrchVocLBox = new CBasicLineBox;
		pSrchVocLBox->CreateSub(this, "BASIC_LINE_BOX_OPTION", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pSrchVocLBox->CreateBaseBoxOption("GMC_VOC_WND_SRCHVOC_LBOX");
		RegisterControl(pSrchVocLBox);
	}

	// UinnerCfg2 : GMC_VOC_WND_SRCHVOC_TXT_TITLE			| GameWord : GMC_VOC_WND_SRCHVOC, 0
	{
		m_pSrchVocTxtTitle = CreateStaticControl("GMC_VOC_WND_SRCHVOC_TXT_TITLE", pFont8, nAlignLeft);
		m_pSrchVocTxtTitle->AddText(ID2GAMEWORD("GMC_VOC_WND_SRCHVOC", 0), dwGold);
	}

	// UinnerCfg2 : GMC_VOC_WND_SRCHVOC_TXT_SRCHBY			| GameWord : GMC_VOC_WND_SRCHVOC, 1
	{
		m_pSrchVocTxtSrchBy = CreateStaticControl("GMC_VOC_WND_SRCHVOC_TXT_SRCHBY", pFont8, nAlignLeft);
		m_pSrchVocTxtSrchBy->AddText(ID2GAMEWORD("GMC_VOC_WND_SRCHVOC", 1), dwLBlue);
	}

	// UinnerCfg2 : GMC_VOC_WND_SRCHVOC_TXT_COLON_SRCHBY	| GameWord : GMC_VOC_WND_SRCHVOC, 2
	{
		m_pSrchVocTxtColonSrchBy = CreateStaticControl("GMC_VOC_WND_SRCHVOC_TXT_COLON_SRCHBY", pFont8, nAlignLeft);
		m_pSrchVocTxtColonSrchBy->AddText(ID2GAMEWORD("GMC_VOC_WND_SRCHVOC", 2), dwLBlue);
	}

	// UinnerCfg2 : GMC_VOC_WND_SRCHVOC_1BTN0
	// UinnerCfg2 : GMC_VOC_WND_SRCHVOC_1BTN1
	// UinnerCfg2 : GMC_VOC_WND_SRCHVOC_1BTN0_F
	// UinnerCfg2 : GMC_VOC_WND_SRCHVOC_1BTN1_F
	{
		CString strButtonKeyword1[nMAXBTN1] =
		{
			"GMC_VOC_WND_SRCHVOC_1BTN0",
			"GMC_VOC_WND_SRCHVOC_1BTN1"
		};


		CString strButtonKeyword1_F[nMAXBTN1_F] =
		{
			"GMC_VOC_WND_SRCHVOC_1BTN0_F",
			"GMC_VOC_WND_SRCHVOC_1BTN1_F"
		};

		for (int i = 0; i < nMAXBTN1; i++)
		{
			m_pSrchByBtn1[i] = CreateFlipButton(strButtonKeyword1[i].GetString(), strButtonKeyword1_F[i].GetString(), GMC_VOC_WND_SRCHVOC_1BTN0 + i);
		}
	}

	// UinnerCfg2 : GMC_VOC_WND_SRCHVOC_TXT_USERID			| GameWord : GMC_VOC_WND_SRCHVOC, 3
	// UinnerCfg2 : GMC_VOC_WND_SRCHVOC_TXT_VOCID			| GameWord : GMC_VOC_WND_SRCHVOC, 4
	{
		m_pSrchVocTxtUserID = CreateStaticControl("GMC_VOC_WND_SRCHVOC_TXT_USERID", pFont8, nAlignLeft);
		m_pSrchVocTxtUserID->AddText(ID2GAMEWORD("GMC_VOC_WND_SRCHVOC", 3), dwLBlue);

		m_pSrchVocTxtVocID = CreateStaticControl("GMC_VOC_WND_SRCHVOC_TXT_VOCID", pFont8, nAlignLeft);
		m_pSrchVocTxtVocID->AddText(ID2GAMEWORD("GMC_VOC_WND_SRCHVOC", 4), dwLBlue);
	}

	// UinnerCfg2 : GMC_VOC_WND_SRCHVOC_TXT_STATUS			| GameWord : GMC_VOC_WND_SRCHVOC, 5
	{
		m_pSrchVocTxtStatus = CreateStaticControl("GMC_VOC_WND_SRCHVOC_TXT_STATUS", pFont8, nAlignLeft);
		m_pSrchVocTxtStatus->AddText(ID2GAMEWORD("GMC_VOC_WND_SRCHVOC", 5), dwLBlue);
	}

	// UinnerCfg2 : GMC_VOC_WND_SRCHVOC_TXT_COLON_STATUS	| GameWord : GMC_VOC_WND_SRCHVOC, 2
	{
		m_pSrchVocTxtColonStatus = CreateStaticControl("GMC_VOC_WND_SRCHVOC_TXT_COLON_STATUS", pFont8, nAlignLeft);
		m_pSrchVocTxtColonStatus->AddText(ID2GAMEWORD("GMC_VOC_WND_SRCHVOC", 2), dwLBlue);
	}

	// UinnerCfg2 : GMC_VOC_WND_SRCHVOC_2BTN0
	// UinnerCfg2 : GMC_VOC_WND_SRCHVOC_2BTN1
	// UinnerCfg2 : GMC_VOC_WND_SRCHVOC_2BTN0_F
	// UinnerCfg2 : GMC_VOC_WND_SRCHVOC_2BTN1_F
	{
		CString strButtonKeyword2[nMAXBTN2] =
		{
			"GMC_VOC_WND_SRCHVOC_2BTN0",
			"GMC_VOC_WND_SRCHVOC_2BTN1"
		};

		CString strButtonKeyword2_F[nMAXBTN2_F] =
		{
			"GMC_VOC_WND_SRCHVOC_2BTN0_F",
			"GMC_VOC_WND_SRCHVOC_2BTN1_F"
		};

		for (int i = 0; i < nMAXBTN2; i++)
		{
			m_pSrchByBtn2[i] = CreateFlipButton(strButtonKeyword2[i].GetString(), strButtonKeyword2_F[i].GetString(), GMC_VOC_WND_SRCHVOC_2BTN0 + i);
		}
	}

	// UinnerCfg2 : GMC_VOC_WND_SRCHVOC_TXT_UNCLAIMED		| GameWord : GMC_VOC_WND_SRCHVOC, 6
	// UinnerCfg2 : GMC_VOC_WND_SRCHVOC_TXT_CLAIMED			| GameWord : GMC_VOC_WND_SRCHVOC, 7
	{
		m_pSrchVocTxtUnClaimed = CreateStaticControl("GMC_VOC_WND_SRCHVOC_TXT_UNCLAIMED", pFont8, nAlignLeft);
		m_pSrchVocTxtUnClaimed->AddText(ID2GAMEWORD("GMC_VOC_WND_SRCHVOC", 6), dwLBlue);

		m_pSrchVocTxtClaimed = CreateStaticControl("GMC_VOC_WND_SRCHVOC_TXT_CLAIMED", pFont8, nAlignLeft);
		m_pSrchVocTxtClaimed->AddText(ID2GAMEWORD("GMC_VOC_WND_SRCHVOC", 7), dwLBlue);
	}

	// UinnerCfg2 : GMC_VOC_WND_SRCHVOC_LBOX_SRCHBY
	{
		CBasicLineBox* pSrchVocLBoxSrchBy = NULL;
		pSrchVocLBoxSrchBy = new CBasicLineBox;
		pSrchVocLBoxSrchBy->CreateSub(this, "BASIC_LINE_BOX_OPTION", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pSrchVocLBoxSrchBy->CreateBaseBoxOption("GMC_VOC_WND_SRCHVOC_LBOX_SRCHBY");
		RegisterControl(pSrchVocLBoxSrchBy);
	}

	// UinnerCfg2 : GMC_VOC_WND_SRCHVOC_BTN_SRCHVOC			| GameWord : GMC_VOC_WND_SRCHVOC, 8
	{
		CBasicTextButton* pSrchVocBtn = new CBasicTextButton;

		pSrchVocBtn->CreateSub(
			this,
			"BASIC_TEXT_BUTTON24",
			UI_FLAG_XSIZE,
			GMC_VOC_WND_SRCHVOC_BTN_SRCHVOC
		);

		pSrchVocBtn->CreateBaseButton(
			"GMC_VOC_WND_SRCHVOC_BTN_SRCHVOC",
			CBasicTextButton::SIZE24,
			CBasicButton::CLICK_FLIP,
			(char*)ID2GAMEWORD("GMC_VOC_WND_SRCHVOC", 8)
		);

		RegisterControl(pSrchVocBtn);
	}

	//----------------------------------------------------------------------------------------------------
	//										E D I T  B O X  M A N
	//----------------------------------------------------------------------------------------------------

	// UinnerCfg2 : GMC_VOC_WND_EDITMAN
	// UinnerCfg2 : GMC_VOC_WND_CARRAT
	// UinnerCfg2 : GMC_VOC_WND_GENVOC_EDITMAN_NOMINAL
	// UinnerCfg2 : GMC_VOC_WND_SRCHVOC_EDITMAN_SRCHBY
	{
		CUIEditBoxMan* pGmcVocEditBoxMan = new CUIEditBoxMan;

		pGmcVocEditBoxMan->CreateSub(
			this,
			"GMC_VOC_WND_EDITMAN",
			UI_FLAG_DEFAULT,
			GMC_VOC_WND_EDITMAN
		);

		pGmcVocEditBoxMan->CreateEditBox(
			GMC_VOC_WND_GENVOC_EDITMAN_NOMINAL,
			"GMC_VOC_WND_GENVOC_EDITMAN_NOMINAL",
			"GMC_VOC_WND_CARRAT",
			TRUE,
			UINT_MAX,
			pFont8,
			nLIMIT_NOMINAL
		);

		pGmcVocEditBoxMan->CreateEditBox(
			GMC_VOC_WND_SRCHVOC_EDITMAN_SRCHBY,
			"GMC_VOC_WND_SRCHVOC_EDITMAN_SRCHBY",
			"GMC_VOC_WND_CARRAT",
			TRUE,
			UINT_MAX,
			pFont8,
			nLIMIT_SRCH
		);

		RegisterControl(pGmcVocEditBoxMan);
		m_pGmcVocEditBoxMan = pGmcVocEditBoxMan;
	}

	SetVisibleBtn1(GMC_VOC_WND_SRCHVOC_NBTN0);
	SetVisibleBtn2(GMC_VOC_WND_SRCHVOC_2NBTN0);
}

CBasicTextBox* CGmcVocWnd::CreateStaticControl(char* szControlKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID)
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub(this, szControlKeyword, UI_FLAG_DEFAULT, cID);
	pStaticText->SetFont(pFont);
	pStaticText->SetTextAlign(nAlign);
	RegisterControl(pStaticText);

	return pStaticText;
}

CBasicButton* CGmcVocWnd::CreateFlipButton(const char* szButton, const char* szButtonFlip, UIGUID ControlID)
{
	CBasicButton* pButton = new CBasicButton;
	pButton->CreateSub(this, szButton, UI_FLAG_DEFAULT, ControlID);
	pButton->CreateFlip(szButtonFlip, CBasicButton::RADIO_FLIP);
	pButton->SetControlNameEx(szButton);
	RegisterControl(pButton);

	return pButton;
}

void CGmcVocWnd::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
	CUIWindowEx::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);

	if (m_pGmcVocEditBoxMan)
	{
		CUIControl* pParent = m_pGmcVocEditBoxMan->GetTopParent();
		if (!pParent)	pParent = this;
		BOOL bFocus = (pParent->IsFocusControl());

		if (bFocus)
		{
			if (UIKeyCheck::GetInstance()->Check(DIK_TAB, DXKEY_DOWN))
			{
				m_pGmcVocEditBoxMan->GoNextTab();
			}
			else if (UIKeyCheck::GetInstance()->Check(DIK_ESCAPE, DXKEY_DOWN))
			{
				m_pGmcVocEditBoxMan->EndEdit();
				CInnerInterface::GetInstance().SetGmcVocWndClose();
			}
		}
	}
}

void CGmcVocWnd::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
	CUIWindowEx::TranslateUIMessage(ControlID, dwMsg);

	if (GMC_VOC_WND_SRCHVOC_1BTN0 <= ControlID && ControlID < GMC_VOC_WND_SRCHVOC_1BTN_END)
	{
		if (CHECK_MOUSE_IN(dwMsg))
		{
			int nIndex = ControlID - GMC_VOC_WND_SRCHVOC_1BTN0;

			if (dwMsg & UIMSG_LB_DOWN)
			{
				SetVisibleBtn1(GMC_VOC_WND_SRCHVOC_NBTN0 + nIndex);
			}
		}
	}

	if (GMC_VOC_WND_SRCHVOC_2BTN0 <= ControlID && ControlID < GMC_VOC_WND_SRCHVOC_2BTN_END)
	{
		if (CHECK_MOUSE_IN(dwMsg))
		{
			int nIndex = ControlID - GMC_VOC_WND_SRCHVOC_2BTN0;

			if (dwMsg & UIMSG_LB_DOWN)
			{
				SetVisibleBtn2(GMC_VOC_WND_SRCHVOC_2NBTN0 + nIndex);
			}
		}
	}

	switch (ControlID)
	{
	case ET_CONTROL_TITLE:
	case ET_CONTROL_TITLE_F:
	{
		if ((dwMsg & UIMSG_LB_DUP) && CHECK_MOUSE_IN(dwMsg))
		{
			CInnerInterface::GetInstance().SetDefaultPosInterface(GMC_VOC_WND);
		}
	}
	break;

	case ET_CONTROL_BUTTON:
	{
		if (CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			CInnerInterface::GetInstance().SetGmcVocWndClose();
		}
	}
	break;

	case GMC_VOC_WND_GENVOC_BTN_GENVOC:
	{
		if (CHECK_KEYFOCUSED(dwMsg) || CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			SetMessageEx(dwMsg &= ~UIMSG_KEY_FOCUSED);

			CString strNominal = m_pGmcVocEditBoxMan->GetEditString(GMC_VOC_WND_GENVOC_EDITMAN_NOMINAL);

			if (!strNominal.GetLength())
			{
				DoModal(ID2GAMEEXTEXT("GMC_VOC_WND_STAGE_1"));
				return;
			}

			SndGenVoc();
		}
	}
	break;

	case GMC_VOC_WND_SRCHVOC_BTN_SRCHVOC:
	{
		if (CHECK_KEYFOCUSED(dwMsg) || CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			SetMessageEx(dwMsg &= ~UIMSG_KEY_FOCUSED);

			CString strNominal = m_pGmcVocEditBoxMan->GetEditString(GMC_VOC_WND_SRCHVOC_EDITMAN_SRCHBY);

			if (!strNominal.GetLength())
			{
				DoModal(ID2GAMEEXTEXT("GMC_VOC_WND_STAGE_1"));
				return;
			}

			SndSrchVoc();
		}
	}
	break;
	}
}

void CGmcVocWnd::ResetAll()
{
	// ToDo
}

void CGmcVocWnd::SetVisibleBtn1(int nBtn)
{
	if (nBtn < GMC_VOC_WND_SRCHVOC_NBTN0 || GMC_VOC_WND_SRCHVOC_NBTN1 < nBtn)
	{
		GASSERT(0 && "영역을 넘어서는 페이지입니다.");
		return;
	}

	{
		for (int i = 0; i < nMAXBTN1; i++)
		{
			m_pSrchByBtn1[i]->SetFlip(FALSE);
		}

	}

	m_nBtnIndex1 = nBtn - GMC_VOC_WND_SRCHVOC_NBTN0;
	m_pSrchByBtn1[m_nBtnIndex1]->SetFlip(TRUE);

	switch (nBtn)
	{
	case GMC_VOC_WND_SRCHVOC_NBTN0:
	{
		m_nUserIDShit = 1;
		m_nVocIDShit = 0;
	}
	break;

	case GMC_VOC_WND_SRCHVOC_NBTN1:
	{
		m_nUserIDShit = 0;
		m_nVocIDShit = 1;
	}
	break;
	}
}

void CGmcVocWnd::SetVisibleBtn2(int nBtn)
{
	if (nBtn < GMC_VOC_WND_SRCHVOC_2NBTN0 || GMC_VOC_WND_SRCHVOC_2NBTN1 < nBtn)
	{
		GASSERT(0 && "영역을 넘어서는 페이지입니다.");
		return;
	}

	{
		for (int i = 0; i < nMAXBTN2; i++)
		{
			m_pSrchByBtn2[i]->SetFlip(FALSE);
		}

	}

	m_nBtnIndex2 = nBtn - GMC_VOC_WND_SRCHVOC_2NBTN0;
	m_pSrchByBtn2[m_nBtnIndex2]->SetFlip(TRUE);

	switch (nBtn)
	{
	case GMC_VOC_WND_SRCHVOC_2NBTN0:
	{
		m_nUnClaimedShit = 1;
		m_nClaimedShit = 0;
	}
	break;

	case GMC_VOC_WND_SRCHVOC_2NBTN1:
	{
		m_nUnClaimedShit = 0;
		m_nClaimedShit = 1;
	}
	break;
	}
}

void CGmcVocWnd::SndGenVoc()
{
	static const string charList = "0123456789";
	static const string charList2 = "23456789abcdefghjkmnpqrstuvwxyzABCDEFGHJKLMNPQRSTUVWXYZ";

	srand((unsigned int)time(NULL));

	CString strSeparate = "-";

	CString strVocID2 = "";
	CString strVocID3 = "";

	CString strVocCodes2 = "";
	CString strVocCodes3 = "";
	CString strVocCodes4 = "";

	for (int i = 0; i < 4; i++)
	{
		strVocID2 += charList[rand() % charList.size()];
		strVocID3 += charList[rand() % charList.size()];
	}

	for (int i = 0; i < 4; i++)
	{
		strVocCodes2 += charList2[rand() % charList2.size()];
		strVocCodes3 += charList2[rand() % charList2.size()];
		strVocCodes4 += charList2[rand() % charList2.size()];
	}

	CString strVocID = strVocID2 + strSeparate + strVocID3;
	CString strVocCodes = strVocCodes2 + strSeparate + strVocCodes3 + strSeparate + strVocCodes4;
	CString strVocNominal = m_pGmcVocEditBoxMan->GetEditString(GMC_VOC_WND_GENVOC_EDITMAN_NOMINAL);

	CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient();

	pNetClient->SndGenVoc(
		strVocID.GetString(),
		strVocCodes.GetString(),
		strVocNominal.GetString()
	);

	LONGLONG lTemp = atol(strVocNominal);

	CString strTemp = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT(lTemp, 3, ",");

	CInnerInterface::GetInstance().PrintConsoleText("----------------------------------------------------------------");
	CInnerInterface::GetInstance().PrintConsoleText("Voucher ID		= %s", strVocID.GetString());
	CInnerInterface::GetInstance().PrintConsoleText("Voucher Codes	= %s", strVocCodes.GetString());
	CInnerInterface::GetInstance().PrintConsoleText("Voucher Nominal	= %s", strTemp.GetString());
	CInnerInterface::GetInstance().PrintConsoleText("----------------------------------------------------------------");
}

void CGmcVocWnd::SndSrchVoc()
{
	int nSrchCode = 0;

	if (m_nUserIDShit == 1)
	{
		if (m_nUnClaimedShit == 1)
		{
			nSrchCode = 11;
		}

		if (m_nClaimedShit == 1)
		{
			nSrchCode = 12;
		}
	}

	if (m_nVocIDShit == 1)
	{
		if (m_nUnClaimedShit == 1)
		{
			nSrchCode = 21;
		}

		if (m_nClaimedShit == 1)
		{
			nSrchCode = 22;
		}
	}

	CString strInput = m_pGmcVocEditBoxMan->GetEditString(GMC_VOC_WND_SRCHVOC_EDITMAN_SRCHBY);

	CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient();

	pNetClient->SndSrchVoc(
		nSrchCode,
		strInput.GetString()
	);
}

void CGmcVocWnd::LoadVocByUserIDUnClaimed()
{
	// TestCode Will Removed Later
	CInnerInterface::GetInstance().PrintConsoleText("Sending Command to Sql! Load Voucher by UserID and UnClaimed");
}

void CGmcVocWnd::LoadVocByUserIDClaimed()
{
	// TestCode Will Removed Later
	CInnerInterface::GetInstance().PrintConsoleText("Sending Command to Sql! Load Voucher by UserID and Claimed");
}

void CGmcVocWnd::LoadVocByVocIDUnClaimed()
{
	// TestCode Will Removed Late
	CInnerInterface::GetInstance().PrintConsoleText("Sending Command to Sql! Load Voucher by VoucherID and UnClaimed");
}

void CGmcVocWnd::LoadVocByVocIDClaimed()
{
	// TestCode Will Removed Later
	CInnerInterface::GetInstance().PrintConsoleText("Sending Command to Sql! Load Voucher by VoucherID and Claimed");
}