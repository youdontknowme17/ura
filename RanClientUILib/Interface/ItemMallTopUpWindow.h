// Name		: ItemMallWindowTopUp.h
// Project	: Lib-RanClientUI

#pragma once

#include "UIWindowEx.h"

class CUIEditBoxMan;

class CTopUpWindow : public CUIWindowEx
{
protected:
	enum
	{
		TOPUP_BTN_SUBMIT = ET_CONTROL_NEXT,
		TOPUP_EDITBOX0,
		TOPUP_EDITBOX1,
		TOPUP_EDITBOX2,
		TOPUP_EDITBOX3,
		TOPUP_EDITBOX4,
		TOPUP_EDITBOX5,
		TOPUP_EDITMAN
	};

public:
	static int nLIMIT_VOC;

public:
	CTopUpWindow();
	virtual ~CTopUpWindow();

public:
	void CreateSubControl();
	void ResetAll();
	void SetCaptcha();
	void TopUp();

	int MakeRandomNumber(int nMax);

private:
	CBasicTextBox*		m_pTopUpTxtVocIDTitle;
	CBasicTextBox*		m_pTopUpTxtVocIDSep;

	CBasicTextBox*		m_pTopUpTxtVocCodesTitle;
	CBasicTextBox*		m_pTopUpTxtVocCodesSep1;
	CBasicTextBox*		m_pTopUpTxtVocCodesSep2;

	CBasicTextBox*		m_pTopUpTxtCaptchaTitle;
	CBasicTextBox*		m_pTopUpTxtCaptchaSep;
	CBasicTextBox*		m_pTopUpTxtCaptcha2;

	INT					m_pTopUpCaptcha2;

	CUIEditBoxMan*		m_pTopUpEditBoxMan;

private:
	CBasicTextBox * CreateStaticControl(char* szConatrolKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID = NO_ID);

public:
	virtual void TranslateUIMessage(UIGUID ControlID, DWORD dwMsg);
	virtual void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);
};