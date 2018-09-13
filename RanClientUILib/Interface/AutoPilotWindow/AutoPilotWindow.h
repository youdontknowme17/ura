// Name		: AutoPilotWindow.h
// Project	: Lib-RanClientUI


#pragma once

#include "../UIWindowEx.h"

class CBasicLineBox;
class CBasicTextBox;
class CAPSkillTrayTab;
class CAutoPilotWindow : public CUIWindowEx
{
protected:
	enum
	{
		nMAXLBOX = 1,
		nMAXTEXT = 1,
	};

	enum
	{
		AP_SKILL_TRAY_TAB_WINDOW
	};

public:
	CAutoPilotWindow();
	~CAutoPilotWindow();

public:
	void CreateSubControl();

private:
	CBasicLineBox*		m_pLBox_	[nMAXLBOX];
	CBasicTextBox*		m_pText_	[nMAXTEXT];

	CAPSkillTrayTab*	m_pAPSkillTrayTab;

private:
	CBasicTextBox*	CreateStaticControl(char* szConatrolKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID = NO_ID);

public:
	void SetSkillTabIndex(INT nIndex);
	void SetAutoSkillOn();
	void SetAutoSkillOff();
};