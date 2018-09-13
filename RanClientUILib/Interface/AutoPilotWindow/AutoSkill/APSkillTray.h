// Name		: APSkillTray.h
// Project	: Lib-RanClientUI


#pragma once

#include "APSkillTrayTab.h"
#include "../../../../EngineUILib/GUInterface/UIGroup.h"
#include "../../../../RanClientLib/G-Logic/GLSkill.h"

const DWORD UIMSG_MOUSEIN_SKILLSLOT = UIMSG_USER1;

class CAPSkillSlotEx;
class CBasicVarTextBox;
struct SCHARSKILL;
class CAPSkillTray : public CUIGroup
{
private:
	static const int nLBUPSKIP;

protected:
	enum
	{
		AP_QUICK_SKILL_SLOT_1 = NO_ID + 1,
		AP_QUICK_SKILL_SLOT_2,
		AP_QUICK_SKILL_SLOT_3,
		AP_QUICK_SKILL_SLOT_4,
		AP_QUICK_SKILL_SLOT_5,
		AP_QUICK_SKILL_SLOT_6,
		AP_QUICK_SKILL_SLOT_7,
		AP_QUICK_SKILL_SLOT_8,
		AP_QUICK_SKILL_SLOT_9,
		AP_QUICK_SKILL_SLOT_0
	};

public:
	CAPSkillTray();
	~CAPSkillTray();

public:
	void CreateSubControl(INT nTabIndex);

public:
	CAPSkillSlotEx*		m_pSlotEx	[AP_QUICK_SKILL_SLOT_MAX];

public:
	virtual void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);
	virtual	void TranslateUIMessage(UIGUID ControlID, DWORD dwMsg);

private:
	void UpdateSkillInfo(SNATIVEID sNativeID);
	void LoadSkillInfo(PGLSKILL const pSkill, SCHARSKILL* const pCharSkill);

private:
	CBasicVarTextBox*	m_pSkillInfo;

private:
	int	m_nLBUPSKIP;
};