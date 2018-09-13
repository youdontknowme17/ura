// Name		: FixUserPage.h
// Project	: Lib-RanClientUI

#pragma	once

#include "UIOuterWindow.h"

class CUIEditBoxMan;
class CClientAgent;

class CFixUserPage : public CUIOuterWindow
{
	enum
	{
		FIXUSER_PAGE_SUBMIT = ET_CONTROL_NEXT,
		FIXUSER_PAGE_EDITMAN,
		FIXUSER_PAGE_EDIT_USERID,
		FIXUSER_PAGE_EDIT_PASSW,
		FIXUSER_PAGE_EDIT_CAPTCHA1,
		FIXUSER_PAGE_CANCEL
	};

public:
	static int nLIMIT_USERID;
	static int nLIMIT_PASSW;
	static int nLIMIT_CAPTCHA;

public:
	CFixUserPage();
	virtual	~CFixUserPage();

public:
	virtual	void TranslateUIMessage(UIGUID ControlID, DWORD dwMsg);
	virtual void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);
	virtual	void SetVisibleSingle(BOOL bVisible);

public:
	void CreateSubControl();
	void ResetAll();
	void GoNextTab();
	void FixUser();

private:
	CUIEditBoxMan*	m_pEditBoxManFixUserPage;
	CBasicTextBox * m_pRandTextBoxFixUserPage;

	INT m_nRandPassNumberFixUserPage;
	INT m_nRPUpdateCntFixUserPage;
};