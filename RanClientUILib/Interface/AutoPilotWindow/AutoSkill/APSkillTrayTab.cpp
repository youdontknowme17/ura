// Name		: APSkillTrayTab.cpp
// Project	: Lib-RanClientUI


#include "../../../StdAfx.h"
#include "APSkillTrayTab.h"
#include "APSkillTray.h"

#include "../../InnerInterface.h"

#include "../../GameTextControl.h"
#include "../../UITextControl.h"

#include "../../BasicTextButton.h"

#include "../../../../EngineUILib/GUInterface/UIKeyCheck.h"

#include "../../../../RanClientLib/G-Logic/GLGaeaClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

INT CAPSkillTrayTab::NEXT_INDEX[] = { 3, 4, 3, 4, 3 };

CAPSkillTrayTab::CAPSkillTrayTab(void)
	: m_nTabIndex(0)
{
	memset(m_pSkillTray, 0, sizeof(CAPSkillTray*) * AP_MAX_TAB_INDEX);
	memset(m_pTextButton, 0, sizeof(CBasicTextButton*) * AP_MAX_TAB_INDEX);
}

CAPSkillTrayTab::~CAPSkillTrayTab()
{
}

void CAPSkillTrayTab::CreateSubControl()
{
	// AutoPilotCfg : AP_QUICK_SKILL_TRAY
	{
		CAPSkillTray * pSkillTray = new CAPSkillTray;
		pSkillTray->CreateSub(this, "AP_QUICK_SKILL_TRAY", UI_FLAG_DEFAULT, AP_QUICK_SKILL_TRAY_F5);
		pSkillTray->CreateSubControl(3);
		RegisterControl(pSkillTray);
		m_pSkillTray[3] = pSkillTray;

		pSkillTray = new CAPSkillTray;
		pSkillTray->CreateSub(this, "AP_QUICK_SKILL_TRAY", UI_FLAG_DEFAULT, AP_QUICK_SKILL_TRAY_F6);
		pSkillTray->CreateSubControl(4);
		pSkillTray->SetVisibleSingle(FALSE);
		RegisterControl(pSkillTray);
		m_pSkillTray[4] = pSkillTray;

		pSkillTray = new CAPSkillTray;
		pSkillTray->SetVisibleSingle(FALSE);
		RegisterControl(pSkillTray);
		m_pSkillTray[0] = pSkillTray;

		pSkillTray = new CAPSkillTray;
		pSkillTray->SetVisibleSingle(FALSE);
		RegisterControl(pSkillTray);
		m_pSkillTray[1] = pSkillTray;

		pSkillTray = new CAPSkillTray;
		pSkillTray->SetVisibleSingle(FALSE);
		RegisterControl(pSkillTray);
		m_pSkillTray[2] = pSkillTray;
	}

	// AutoPilotCfg : AP_SKILL_TRAY_TAB_BUTTON		| AutoPilotWord : AP_SKILL_TAB_INFO, 0		| F5
	// AutoPilotCfg : AP_SKILL_TRAY_TAB_BUTTON		| AutoPilotWord : AP_SKILL_TAB_INFO, 1		| F6
	{
		CBasicTextButton * pTextButton = new CBasicTextButton;
		pTextButton->CreateSub(this, "BASIC_TEXT_BUTTON14", UI_FLAG_XSIZE, BASIC_TEXT_BUTTON_F5);
		pTextButton->CreateBaseButton("AP_SKILL_TRAY_TAB_BUTTON", CBasicTextButton::SIZE14, CBasicButton::CLICK_FLIP, ID2GAMEWORD("AP_SKILL_TAB_INFO", 0));
		RegisterControl(pTextButton);
		m_pTextButton[3] = pTextButton;
		SetFoldGroup(pTextButton, TABBUTTON_FOLDGROUP_ID);

		pTextButton = new CBasicTextButton;
		pTextButton->CreateSub(this, "BASIC_TEXT_BUTTON14", UI_FLAG_XSIZE, BASIC_TEXT_BUTTON_F6);
		pTextButton->CreateBaseButton("AP_SKILL_TRAY_TAB_BUTTON", CBasicTextButton::SIZE14, CBasicButton::CLICK_FLIP, ID2GAMEWORD("AP_SKILL_TAB_INFO", 1));
		pTextButton->SetVisibleSingle(FALSE);
		RegisterControl(pTextButton);
		m_pTextButton[4] = pTextButton;
		SetFoldGroup(pTextButton, TABBUTTON_FOLDGROUP_ID);

		pTextButton = new CBasicTextButton;
		pTextButton->SetVisibleSingle(FALSE);
		RegisterControl(pTextButton);
		m_pTextButton[0] = pTextButton;

		pTextButton = new CBasicTextButton;
		pTextButton->SetVisibleSingle(FALSE);
		RegisterControl(pTextButton);
		m_pTextButton[1] = pTextButton;

		pTextButton = new CBasicTextButton;
		pTextButton->SetVisibleSingle(FALSE);
		RegisterControl(pTextButton);
		m_pTextButton[2] = pTextButton;
	}
}

void CAPSkillTrayTab::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
	CUIGroup::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);
}

void CAPSkillTrayTab::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
	switch (ControlID)
	{
	case BASIC_TEXT_BUTTON_F5:
	case BASIC_TEXT_BUTTON_F6:
	{
		if (CHECK_MOUSE_IN(dwMsg))
		{
			if (CHECK_LB_UP_LIKE(dwMsg))
			{
				INT nIndex = GetTabIndex();
				INT nNextIndex = NEXT_INDEX[nIndex];

				m_pSkillTray[nIndex]->SetVisibleSingle(FALSE);
				m_pSkillTray[nNextIndex]->SetVisibleSingle(TRUE);

				m_pTextButton[nIndex]->SetVisibleSingle(FALSE);
				m_pTextButton[nNextIndex]->SetVisibleSingle(TRUE);

				SetTabIndex(nNextIndex);
			}
		}
	}
	break;

	case AP_QUICK_SKILL_TRAY_F5:
	case AP_QUICK_SKILL_TRAY_F6:
	{
		if (dwMsg & UIMSG_MOUSEIN_SKILLSLOT)
		{
			AddMessageEx(UIMSG_MOUSEIN_SKILLSLOT);
		}
	}
	break;
	}
}

void CAPSkillTrayTab::SetVisibleSingle(BOOL bVisible)
{
	CUIGroup::SetVisibleSingle(bVisible);

	if (bVisible)
	{
		m_pSkillTray[0]->SetVisibleSingle(FALSE);
		m_pSkillTray[1]->SetVisibleSingle(FALSE);
		m_pSkillTray[2]->SetVisibleSingle(FALSE);
		m_pSkillTray[3]->SetVisibleSingle(FALSE);
		m_pSkillTray[4]->SetVisibleSingle(FALSE);

		m_pTextButton[0]->SetVisibleSingle(FALSE);
		m_pTextButton[1]->SetVisibleSingle(FALSE);
		m_pTextButton[2]->SetVisibleSingle(FALSE);
		m_pTextButton[3]->SetVisibleSingle(FALSE);
		m_pTextButton[4]->SetVisibleSingle(FALSE);

		INT nIndex = GetTabIndex();
		m_pSkillTray[nIndex]->SetVisibleSingle(TRUE);
		m_pTextButton[nIndex]->SetVisibleSingle(TRUE);
	}
}

void CAPSkillTrayTab::SetSkillTabIndex(INT nIndex)
{
	if (CInnerInterface::GetInstance().IsVisibleGroup(MODAL_WINDOW)) return;

	INT nOldIndex = GetTabIndex();
	INT nNewIndex(nOldIndex);

	nNewIndex = nIndex;

	if (nNewIndex == nOldIndex) return;
	else
	{
		m_pSkillTray[nOldIndex]->SetVisibleSingle(FALSE);
		m_pSkillTray[nNewIndex]->SetVisibleSingle(TRUE);
		m_pTextButton[nOldIndex]->SetVisibleSingle(FALSE);
		m_pTextButton[nNewIndex]->SetVisibleSingle(TRUE);
		SetTabIndex(nNewIndex);
	}
}

void CAPSkillTrayTab::SetAutoSkillOn()
{
	GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
	if (pCharacter)
	{
		pCharacter->ReqUseAutoSkill(TRUE);
	}
}

void CAPSkillTrayTab::SetAutoSkillOff()
{
	GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
	if (pCharacter)
	{
		pCharacter->ReqUseAutoSkill(FALSE);
	}
}