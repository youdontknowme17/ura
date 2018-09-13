// Name		: SelectUserPage.h
// Project	: Lib-RanClientUI

#pragma	once

#include "UIOuterWindow.h"

class CSelectUserPage : public CUIOuterWindow
{
	enum
	{
		SELECT_USER_PAGE_REGISTER = ET_CONTROL_NEXT,
		SELECT_USER_PAGE_FORGOTPASSWORD,
		SELECT_USER_PAGE_CHANGEPASSWORD,
		SELECT_USER_PAGE_CHANGEPINCODE,
		SELECT_USER_PAGE_FIX,
		SELECT_USER_PAGE_BACK
	};

public:
	void CreateSubControl();

public:
	virtual	void TranslateUIMessage(UIGUID ControlID, DWORD dwMsg);
	virtual void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);
};
