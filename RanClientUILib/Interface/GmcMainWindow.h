// Name		: GmcMainWindow.h
// Project	: Lib-RanClientUI

#pragma once

#include "UIWindowEx.h"

class CBasicTextButton;

class CGmcMainWindow : public CUIWindowEx
{
private:
	static const int nOUTOFRANGE;

protected:
	enum
	{
		nMAXPAGE = 3
	};

	enum
	{
		GMC_PAGE0 = ET_CONTROL_NEXT,
		GMC_PAGE1,
		GMC_PAGE2,
		GMC_PAGE_END = GMC_PAGE0 + nMAXPAGE,
		GMC_WINDOW_PAGE_BUTTON0,
		GMC_WINDOW_PAGE_BUTTON1,
		GMC_WINDOW_PAGE_BUTTON2,
		GMC_WINDOW_PAGE_BUTTON_END = GMC_WINDOW_PAGE_BUTTON0 + nMAXPAGE,
	};

public:
	CGmcMainWindow();
	virtual	~CGmcMainWindow();

public:
	void CreateSubControl();

public:
	virtual void TranslateUIMessage(UIGUID ControlID, DWORD dwMsg);

public:
	void SetVisiblePage(int nPage);

protected:
	CBasicTextButton*	CreateTextButton(const char* szButton, UIGUID ControlID, const char* szText);

private:
	CUIControl*			m_pGmcMainWindow_BG;
	CBasicTextButton*	m_pPageButton[nMAXPAGE];

	int m_nPageIndex;
};
