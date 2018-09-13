// Name		: GmcVocWnd.h
// Project	: Lib-RanClientUI

#pragma once

#include "UIWindowEx.h"

class CUIEditBoxMan;

class CGmcVocWnd : public CUIWindowEx
{
private:
	static const int nOUTOFRANGE;

protected:
	enum
	{
		nMAXBTN1 = 2,
		nMAXBTN1_F = 2,
		nMAXBTN2 = 2,
		nMAXBTN2_F = 2
	};

	enum
	{
		GMC_VOC_WND_GENVOC_BTN_GENVOC = ET_CONTROL_NEXT,
		GMC_VOC_WND_SRCHVOC_BTN_SRCHVOC,
		GMC_VOC_WND_EDITMAN,
		GMC_VOC_WND_SRCHVOC_NBTN0,	// UserID
		GMC_VOC_WND_SRCHVOC_NBTN1,	// VocID
		GMC_VOC_WND_SRCHVOC_2NBTN0,	// UnClaimed
		GMC_VOC_WND_SRCHVOC_2NBTN1,	// Claimed
		GMC_VOC_WND_SRCHVOC_1BTN0,
		GMC_VOC_WND_SRCHVOC_1BTN1,
		GMC_VOC_WND_SRCHVOC_2BTN0,
		GMC_VOC_WND_SRCHVOC_2BTN1,
		GMC_VOC_WND_SRCHVOC_1BTN_END = GMC_VOC_WND_SRCHVOC_1BTN0 + nMAXBTN1,
		GMC_VOC_WND_SRCHVOC_2BTN_END = GMC_VOC_WND_SRCHVOC_2BTN0 + nMAXBTN2,
		GMC_VOC_WND_GENVOC_EDITMAN_NOMINAL,
		GMC_VOC_WND_SRCHVOC_EDITMAN_SRCHBY,
	};

public:
	static int nLIMIT_NOMINAL;
	static int nLIMIT_SRCH;

public:
	CGmcVocWnd();
	virtual ~CGmcVocWnd();

public:
	void CreateSubControl();
	void ResetAll();

	void SndGenVoc();					// Using StoredProcedure Method
	void SndSrchVoc();					// Using StoredProcedure Method

	void LoadVocByUserIDUnClaimed();	// Using SQLExecDirect Method
	void LoadVocByUserIDClaimed();		// Using SQLExecDirect Method

	void LoadVocByVocIDUnClaimed();		// Using SQLExecDirect Method
	void LoadVocByVocIDClaimed();		// Using SQLExecDirect Method

public:
	virtual	void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);
	virtual void TranslateUIMessage(UIGUID ControlID, DWORD dwMsg);

private:
	CUIControl*			m_pGmcVocWnd_BG;

	CBasicTextBox*		m_pGenVocTxtTitle;
	CBasicTextBox*		m_pGenVocTxtNominal;
	CBasicTextBox*		m_pGenVocTxtColon;

	CBasicTextBox*		m_pSrchVocTxtTitle;
	CBasicTextBox*		m_pSrchVocTxtSrchBy;
	CBasicTextBox*		m_pSrchVocTxtColonSrchBy;

	CBasicTextBox*		m_pSrchVocTxtUserID;
	CBasicTextBox*		m_pSrchVocTxtVocID;

	CBasicTextBox*		m_pSrchVocTxtStatus;
	CBasicTextBox*		m_pSrchVocTxtColonStatus;

	CBasicTextBox*		m_pSrchVocTxtUnClaimed;
	CBasicTextBox*		m_pSrchVocTxtClaimed;

	CBasicTextBox*		m_pSrchGenVocNominal;
	CUIEditBoxMan*		m_pGmcVocEditBoxMan;

private:
	CBasicTextBox*		CreateStaticControl(char* szConatrolKeyword, CD3DFontPar* pFont, int nAlign, const UIGUID& cID = NO_ID);
	CBasicButton*		CreateFlipButton(const char* szButton, const char* szButtonFlip, UIGUID ControlID);

	CBasicButton*		m_pSrchByBtn1[nMAXBTN1];
	CBasicButton*		m_pSrchByBtn2[nMAXBTN2];

public:
	void SetVisibleBtn1(int nBtn);
	void SetVisibleBtn2(int nBtn);

	int m_nBtnIndex1;
	int m_nBtnIndex2;

	int m_nUserIDShit;
	int m_nVocIDShit;

	int m_nUnClaimedShit;
	int m_nClaimedShit;
};