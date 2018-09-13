// Name		: NewLoginPage.h
// Project	: Lib-RanClientUI

#pragma	once

#include "UIOuterWindow.h"

class CUIEditBoxMan;

class CNewLoginPage : public CUIOuterWindow
{
	enum
	{
		NEW_LOGIN_PAGE_SUBMIT = ET_CONTROL_NEXT,
		NEW_LOGIN_PAGE_CANCEL,
		NEW_LOGIN_PAGE_EDITMAN,
		NEW_LOGIN_PAGE_EDIT_USERID,
		NEW_LOGIN_PAGE_EDIT_PASSW,
		NEW_LOGIN_PAGE_EDIT_CAPTCHA1,
		NEW_LOGIN_PAGE_IDSAVE_BUTTON
	};

public:
	static int nLIMIT_USERID;
	static int nLIMIT_PASSW;
	static int nLIMIT_CAPTCHA;

public:
	CNewLoginPage();
	virtual	~CNewLoginPage();

public:
	void CreateSubControl();
	CBasicButton * CreateFlipButton(char* szButton, char* szButtonFlip, UIGUID ControlID);

	void ResetAll();
	void GoNextTab();
	void SetCharToEditBox(TCHAR cKey);
	void DelCharToEditBox();

public:
	virtual	void TranslateUIMessage(UIGUID ControlID, DWORD dwMsg);
	virtual void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);
	virtual	void SetVisibleSingle(BOOL bVisible);

private:
	void LoadIDSaveButton();

private:
	CUIEditBoxMan*	m_pEditBoxManLoginPage;
	CBasicButton*	m_pIDSaveButton;
	CBasicTextBox*	m_pRandTextBoxLoginPage;

	INT m_nRandPassNumberLoginPage;
	INT m_nRPUpdateCntLoginPage;
};