// Name		: APSkillTrayTab.h
// Project	: Lib-RanClientUI


#pragma once

#include "../../../../EngineUILib/GUInterface/UIGroup.h"

const INT AP_MAX_TAB_INDEX			= 5;
const INT AP_QUICK_SKILL_SLOT_MAX	= 10;

class CBasicTextButton;
class CAPSkillTray;
class CAPSkillTrayTab : public CUIGroup
{
	static INT NEXT_INDEX[AP_MAX_TAB_INDEX];

public:
	enum
	{
		AP_QUICK_SKILL_TRAY_F5 = NO_ID + 1,
		AP_QUICK_SKILL_TRAY_F6,
		

		BASIC_TEXT_BUTTON_F5,
		BASIC_TEXT_BUTTON_F6,
	};

	enum
	{
		TABBUTTON_FOLDGROUP_ID = 1
	};

public:
	CAPSkillTrayTab(void);
	virtual ~CAPSkillTrayTab(void);

public:
	void CreateSubControl();

	void SetTabIndex(INT nIndex) { GASSERT(0 <= nIndex && nIndex <= MAX_TAB_INDEX); m_nTabIndex = nIndex; }
	INT GetTabIndex() { return m_nTabIndex; }

public:
	virtual void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);
	virtual	void TranslateUIMessage(UIGUID ControlID, DWORD dwMsg);
	virtual	void SetVisibleSingle(BOOL bVisible);

protected:
	CAPSkillTray*		m_pSkillTray	[AP_MAX_TAB_INDEX];
	CBasicTextButton*	m_pTextButton	[AP_MAX_TAB_INDEX];

	INT m_nTabIndex;

public:
	void SetSkillTabIndex(INT nIndex);
	void SetAutoSkillOn();
	void SetAutoSkillOff();
};