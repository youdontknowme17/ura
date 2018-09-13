#pragma once
#include "edit_ime.h"

class CEditBox
{
protected:
	BOOL		m_IsInput;

	BOOL		m_bComp;
	BOOL		m_bCompSet;
	int			m_nCompCount;
	RECT		m_rectEdit;
	CEdit_IME*	m_pEditBox;

public:
	BOOL Create ( CWnd *pWnd );
	void Destroy ();

	void SetWndRect ( RECT rtMove );

	BOOL IsOn ()					{ return m_IsInput; }
	void OnInput ();				// ���ڿ� �Է� ON
	void OffInput ();				// ���ڿ� �Է� OFF

	BOOL IsCOMPOSITION ();

	CString GetString ()
	{
		CString strText;
		if ( m_pEditBox )	m_pEditBox->GetWindowText(strText);
		return strText;
	}

	char* GetCompleteString ()		{ return ""; }
	char* GetCompsitionString ()	{ return ""; }
	void SetString ( const char* pStr = NULL ); // �ʱ� ���ڿ� �����ϱ�

	HRESULT FrameMove ( float fTime, float fElapsedTime );

public:
	CEditBox(void);
	~CEditBox(void);
};
