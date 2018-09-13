// Name		: SelectMenuPage.h
// Project	: Lib-RanClientUI

#pragma	once

#include "UIOuterWindow.h"

class CSelectMenuPage : public CUIOuterWindow
{
	enum
	{
		SELECT_MENU_PAGE_LOGIN = ET_CONTROL_NEXT,
		SELECT_MENU_PAGE_USER,
		SELECT_MENU_PAGE_BACK
	};

public:
	void CreateSubControl();

public:
	virtual	void TranslateUIMessage(UIGUID ControlID, DWORD dwMsg);
	virtual void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);
};