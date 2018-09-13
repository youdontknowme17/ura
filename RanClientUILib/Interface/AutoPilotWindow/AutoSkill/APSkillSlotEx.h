// Name		: APSkillSlotEx.h
// Project	: Lib-RanClientUI


#pragma once

#include "APSkillSlot.h"
#include "APSkillTrayTab.h"

class CBasicTextBox;
class CAPSkillSlotEx : public CAPSkillSlot
{
private:
	static const int nNOT_INIT;

public:
	CAPSkillSlotEx();
	virtual	~CAPSkillSlotEx();

public:
	virtual	void CreateSubControl(INT nTabIndex);
	void CreateNumberText(CD3DFontPar* pFont8, int nNumber);

	INT GetTabIndex() { return m_nTabIndex; }

public:
	virtual void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);

private:
	int				m_nNumber;
	INT				m_nTabIndex;

	CBasicTextBox*	m_pNumberText;
};