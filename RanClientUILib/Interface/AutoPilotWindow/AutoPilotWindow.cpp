// Name		: AutoPilotWindow.cpp
// Project	: Lib-RanClientUI

#include "../../StdAfx.h"
#include "AutoPilotWindow.h"

#include "AutoSkill\APSkillTrayTab.h"

#include "../BasicLineBox.h"

#include "../GameTextControl.h"
#include "../UITextControl.h"

#include "../../../EngineLib/DxCommon/DxFontMan.h"

#include "../../../EngineUILib/GUInterface/BasicTextBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CAutoPilotWindow::CAutoPilotWindow()
{
}

CAutoPilotWindow::~CAutoPilotWindow()
{
}

void CAutoPilotWindow::CreateSubControl()
{
	CD3DFontPar* pFont8		= DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 8, _DEFAULT_FONT_SHADOW_FLAG);
	const int nAlignLeft	= TEXT_ALIGN_LEFT;
	const DWORD& dwGold		= NS_UITEXTCOLOR::GOLD;

	// AutoPilotCfg : AP_WINDOW_LBOX_0
	{
		char strTemp[50];

		for (int i = 0; i < nMAXLBOX; ++i)
		{
			sprintf(strTemp, "AP_WINDOW_LBOX_%d", i);

			m_pLBox_[i] = NULL;
			m_pLBox_[i] = new CBasicLineBox;
			m_pLBox_[i]->CreateSub(this, "BASIC_LINE_BOX_OPTION", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
			m_pLBox_[i]->CreateBaseBoxOption(strTemp);
			RegisterControl(m_pLBox_[i]);
		}
	}

	// AutoPilotCfg : AP_WINDOW_TEXT_0				| AutoPilotWord : AP_WINDOW_TEXT, 0		| Skill Slot
	{
		char strTemp[50];

		for (int i = 0; i < nMAXTEXT; ++i)
		{
			sprintf(strTemp, "AP_WINDOW_TEXT_%d", i);

			m_pText_[i] = CreateStaticControl(strTemp, pFont8, nAlignLeft);
			m_pText_[i]->AddText(ID2GAMEWORD("AP_WINDOW_TEXT", i), dwGold);
		}
	}

	// AutoPilotCfg : AP_SKILL_TRAY_TAB_WINDOW
	{
		m_pAPSkillTrayTab = new CAPSkillTrayTab;
		m_pAPSkillTrayTab->CreateEx(AP_SKILL_TRAY_TAB_WINDOW, "AP_SKILL_TRAY_TAB_WINDOW");
		m_pAPSkillTrayTab->CreateSubControl();
		RegisterControl(m_pAPSkillTrayTab);
		SetVisibleSingle(TRUE);
	}
}

CBasicTextBox* CAutoPilotWindow::CreateStaticControl(char* szControlKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID)
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub(this, szControlKeyword, UI_FLAG_DEFAULT, cID);
	pStaticText->SetFont(pFont);
	pStaticText->SetTextAlign(nAlign);
	RegisterControl(pStaticText);

	return pStaticText;
}

void CAutoPilotWindow::SetSkillTabIndex(INT nIndex)
{
	m_pAPSkillTrayTab->SetSkillTabIndex(nIndex);
}

void CAutoPilotWindow::SetAutoSkillOn()
{
	m_pAPSkillTrayTab->SetAutoSkillOn();
}

void CAutoPilotWindow::SetAutoSkillOff()
{
	m_pAPSkillTrayTab->SetAutoSkillOff();
}