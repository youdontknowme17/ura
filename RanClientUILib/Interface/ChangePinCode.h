// Name		: ChangePinCode.h
// Project	: Lib-RanClientUI
// Author	: CNDev

#pragma	once

#include "UIOuterWindow.h"

class CUIEditBoxMan;
class CClientAgent;

class CChangePinCodePage : public CUIOuterWindow
{
	enum
	{
		CHANGEPINCODE_PAGE_SUBMIT = ET_CONTROL_NEXT,
		CHANGEPINCODE_PAGE_CANCEL,
		CHANGEPINCODE_PAGE_EDITMAN,
		CHANGEPINCODE_PAGE_EDIT_USERID,
		CHANGEPINCODE_PAGE_EDIT_PASSWORD,
		CHANGEPINCODE_PAGE_EDIT_OLDPINCODE,
		CHANGEPINCODE_PAGE_EDIT_NEWPINCODE,
		CHANGEPINCODE_PAGE_EDIT_RENEWPINCODE,
		CHANGEPINCODE_PAGE_EDIT_CAPTCHA1
	};

public:
	static int nLIMIT_USERID;
	static int nLIMIT_PASSW;
	static int nLIMIT_PINCODE;
	static int nLIMIT_CAPTCHA;

public:
	CChangePinCodePage();
	virtual	~CChangePinCodePage();

public:
	virtual	void TranslateUIMessage(UIGUID ControlID, DWORD dwMsg);
	virtual void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);
	virtual	void SetVisibleSingle(BOOL bVisible);

public:
	void CreateSubControl();
	void ResetAll();
	void GoNextTab();
	void ChangePinCode();

private:
	CUIEditBoxMan*	m_pEditBoxManChangePinCodePage;
	CBasicTextBox * m_pRandTextBoxChangePinCodePage;

	INT m_nRandPassNumberChangePinCodePage;
	INT m_nRPUpdateCntChangePinCodePage;
};