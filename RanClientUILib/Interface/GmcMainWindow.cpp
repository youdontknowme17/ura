// Name		: GmcMainWindow.cpp
// Project	: Lib-RanClientUI

#include "StdAfx.h"
#include "GmcMainWindow.h"
#include "BasicLineBox.h"
#include "BasicTextButton.h"
#include "GameTextControl.h"
#include "InnerInterface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int CGmcMainWindow::nOUTOFRANGE = -1;

CGmcMainWindow::CGmcMainWindow()
	: m_nPageIndex(nOUTOFRANGE)
{
	memset(m_pPageButton, 0, sizeof(CBasicTextButton*) * nMAXPAGE);
}

CGmcMainWindow::~CGmcMainWindow()
{
}

void CGmcMainWindow::CreateSubControl()
{
	{
		m_pGmcMainWindow_BG = new CUIControl;
		m_pGmcMainWindow_BG->CreateSub(this, "GMC_MAIN_WINDOW_BG", UI_FLAG_DEFAULT);
		m_pGmcMainWindow_BG->SetVisibleSingle(TRUE);
		RegisterControl(m_pGmcMainWindow_BG);
	}

	{
		CBasicLineBox* pBasicLineBox = NULL;
		pBasicLineBox = new CBasicLineBox;
		pBasicLineBox->CreateSub(this, "BASIC_LINE_BOX_OPTION", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
		pBasicLineBox->CreateBaseBoxOption("GMC_MAIN_WINDOW_BACK");
		RegisterControl(pBasicLineBox);
	}

	CString strButtonKeyword[nMAXPAGE] =
	{
		"GMC_WINDOW_PAGE_BUTTON0",
		"GMC_WINDOW_PAGE_BUTTON1",
		"GMC_WINDOW_PAGE_BUTTON2"
	};

	for (int i = 0; i < nMAXPAGE; i++)
	{
		m_pPageButton[i] = CreateTextButton(strButtonKeyword[i].GetString(), GMC_WINDOW_PAGE_BUTTON0 + i, (char*)ID2GAMEWORD("GMC_WINDOW_PAGE_BUTTON", i));
	}

	SetVisiblePage(GMC_PAGE0);
}

void CGmcMainWindow::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
	CUIWindowEx::TranslateUIMessage(ControlID, dwMsg);

	switch (ControlID)
	{
	case ET_CONTROL_TITLE:
	case ET_CONTROL_TITLE_F:
	{
		if ((dwMsg & UIMSG_LB_DUP) && CHECK_MOUSE_IN(dwMsg))
		{
			CInnerInterface::GetInstance().SetDefaultPosInterface(GMC_MAIN_WINDOW);
		}
	}
	break;

	case ET_CONTROL_BUTTON:
	{
		if (CHECK_MOUSEIN_LBUPLIKE(dwMsg))
		{
			CInnerInterface::GetInstance().SetGmcMainWindowClose();
		}
	}
	break;
	}

	if (GMC_WINDOW_PAGE_BUTTON0 <= ControlID && ControlID < GMC_WINDOW_PAGE_BUTTON_END)
	{
		if (CHECK_MOUSE_IN(dwMsg))
		{
			int nIndex = ControlID - GMC_WINDOW_PAGE_BUTTON0;

			if (dwMsg & UIMSG_LB_DOWN)
			{
				SetVisiblePage(GMC_PAGE0 + nIndex);
			}
		}
	}
}

CBasicTextButton* CGmcMainWindow::CreateTextButton(const char* szButton, UIGUID ControlID, const char* szText)
{
	const int nBUTTONSIZE = CBasicTextButton::SIZE24;
	CBasicTextButton* pTextButton = new CBasicTextButton;
	pTextButton->CreateSub(this, "BASIC_TEXT_BUTTON24", UI_FLAG_XSIZE, ControlID);
	pTextButton->CreateBaseButton(szButton, nBUTTONSIZE, CBasicButton::RADIO_FLIP, szText);
	RegisterControl(pTextButton);
	return pTextButton;
}

void CGmcMainWindow::SetVisiblePage(int nPage)
{
	if (nPage < GMC_PAGE0 || GMC_PAGE2 < nPage)
	{
		GASSERT(0 && "영역을 넘어서는 페이지입니다.");
		return;
	}

	{
		for (int i = 0; i < nMAXPAGE; i++)
		{
			m_pPageButton[i]->SetFlip(FALSE);
		}

		CInnerInterface::GetInstance().SetGmcVocWndClose();
	}
	
	m_nPageIndex = nPage - GMC_PAGE0;
	m_pPageButton[m_nPageIndex]->SetFlip(TRUE);

	switch (nPage)
	{
	case GMC_PAGE0:
	{
	}
	break;

	case GMC_PAGE1:
	{
	}
	break;

	case GMC_PAGE2:
	{
		CInnerInterface::GetInstance().SetGmcVocWndOpen();
	}
	break;
	}
}