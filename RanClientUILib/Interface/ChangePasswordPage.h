// Name		: ChangePasswordPage.h
// Project	: Lib-RanClientUI

#pragma	once

#include "UIOuterWindow.h"

class CUIEditBoxMan;
class CClientAgent;

class CChangePasswordPage : public CUIOuterWindow
{
	enum
	{
		CHANGEPASSWORD_PAGE_SUBMIT = ET_CONTROL_NEXT,
		CHANGEPASSWORD_PAGE_CANCEL,
		CHANGEPASSWORD_PAGE_EDITMAN,
		CHANGEPASSWORD_PAGE_EDIT_USERID,
		CHANGEPASSWORD_PAGE_EDIT_OLDPASSW,
		CHANGEPASSWORD_PAGE_EDIT_NEWPASSW,
		CHANGEPASSWORD_PAGE_EDIT_RENEWPASSW,
		CHANGEPASSWORD_PAGE_EDIT_CAPTCHA1
	};

public:
	static int nLIMIT_USERID;
	static int nLIMIT_PASSW;
	static int nLIMIT_CAPTCHA;

public:
	CChangePasswordPage();
	virtual	~CChangePasswordPage();

public:
	virtual	void TranslateUIMessage(UIGUID ControlID, DWORD dwMsg);
	virtual void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);
	virtual	void SetVisibleSingle(BOOL bVisible);

public:
	void CreateSubControl();
	void ResetAll();
	void GoNextTab();
	void ChangePassword();

private:
	CUIEditBoxMan*	m_pEditBoxManChangePasswordPage;
	CBasicTextBox * m_pRandTextBoxChangePasswordPage;

	INT m_nRandPassNumberChangePasswordPage;
	INT m_nRPUpdateCntChangePasswordPage;
};