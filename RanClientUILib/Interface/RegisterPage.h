// Name		: RegisterPage.h
// Project	: Lib-RanClientUI

#pragma	once

#include "UIOuterWindow.h"

class CUIEditBoxMan;

class CRegisterPage : public CUIOuterWindow
{
	enum
	{
		REGISTER_PAGE_SUBMIT = ET_CONTROL_NEXT,
		REGISTER_PAGE_CANCEL,
		REGISTER_PAGE_EDITMAN,
		REGISTER_PAGE_EDIT_USERID,
		REGISTER_PAGE_EDIT_PASSW,
		REGISTER_PAGE_EDIT_REPASSW,
		REGISTER_PAGE_EDIT_PINCODE,
		REGISTER_PAGE_EDIT_EMAIL,
		REGISTER_PAGE_EDIT_CAPTCHA1
	};

public:
	static int nLIMIT_USERID;
	static int nLIMIT_PASSW;
	static int nLIMIT_PINCODE;
	static int nLIMIT_EMAIL;
	static int nLIMIT_CAPTCHA;

public:
	CRegisterPage();
	virtual	~CRegisterPage();

public:
	void CreateSubControl();
	void ResetAll();
	void GoNextTab();
	void RegisterAccount();

public:
	BOOL CheckString(CString strTemp);

public:
	virtual	void TranslateUIMessage(UIGUID ControlID, DWORD dwMsg);
	virtual void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);
	virtual	void SetVisibleSingle(BOOL bVisible);

private:
	CUIEditBoxMan*	m_pEditBoxManRegisterPage;
	CBasicTextBox * m_pRandTextBoxRegisterPage;

	INT m_nRandPassNumberRegisterPage;
	INT m_nRPUpdateCntRegisterPage;
};