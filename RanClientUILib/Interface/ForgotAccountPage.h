// Name		: ForgotAccountPage.h
// Project	: Lib-RanClientUI

#pragma	once

#include "UIOuterWindow.h"

class CUIEditBoxMan;
class CClientAgent;

class CForgotAccountPage : public CUIOuterWindow
{
	enum
	{
		FORGOTACCOUNT_PAGE_SUBMIT = ET_CONTROL_NEXT,
		FORGOTACCOUNT_PAGE_EDITMAN,
		FORGOTACCOUNT_PAGE_EDIT_EMAIL,
		FORGOTACCOUNT_PAGE_EDIT_PINCODE,
		FORGOTACCOUNT_PAGE_EDIT_CAPTCHA1,
		FORGOTACCOUNT_PAGE_CANCEL
	};

public:
	static int nLIMIT_EMAIL;
	static int nLIMIT_PINCODE;
	static int nLIMIT_CAPTCHA;

public:
	CForgotAccountPage();
	virtual	~CForgotAccountPage();

public:
	virtual	void TranslateUIMessage(UIGUID ControlID, DWORD dwMsg);
	virtual void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);
	virtual	void SetVisibleSingle(BOOL bVisible);

public:
	void CreateSubControl();
	void ResetAll();
	void GoNextTab();
	void ForgotAccount();

private:
	CUIEditBoxMan*	m_pEditBoxManForgotAccountPage;
	CBasicTextBox * m_pRandTextBoxForgotAccountPage;

	INT m_nRandPassNumberForgotAccountPage;
	INT m_nRPUpdateCntForgotAccountPage;
};