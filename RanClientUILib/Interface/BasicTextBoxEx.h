//	BasicScrollBarEx Ŭ����
//
//	���� �ۼ��� : ���⿱
//	���� ������ :
//	�α�
//		[2003.12.1]
//			@ ��������

#pragma	once

#include "../EngineUIlib/GUInterface/BasicTextBox.h"

class	CBasicTextBoxEx : public CBasicTextBox
{
public:
static	const int nNOT_VISIBLE;

public:
	CBasicTextBoxEx ();
	virtual	~CBasicTextBoxEx ();

public:
	void	MouseClick ( char* KeyValue );
	void	CreateMouseOver ( D3DCOLOR dwMouseOverLineColor );
	void	CreateMouseClick ( D3DCOLOR dwMouseOverLineColor );

public:
	virtual	void	ClearText ();

protected:
	void	UpdateMousePos ( int x, int y );
//	WORD	m_wFlipMode;

public:
	void	UpdateMouseOverImage ();
	void	UpdateMouseClickImage ();

	int		GetSelectPos ();
	void	SetSelectPos ( int nIndex ) { m_nMouseOverLine = nIndex; }

public:
	void	SetSensitive ( const bool& bSensitive );	
	void	SetSensitiveView ( const bool& bSensitive );
	void	SetSelectInit( const bool& bSelectInit );

public:
	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

private:
	CUIControl*	m_pMouseOver;
	CUIControl*	m_pMouseOverImage;
	CUIControl*	m_pMouseOverImage0;
	D3DCOLOR	m_dwMouseOverImageColor;

	CUIControl* m_pMouseClickImage;
	CUIControl* m_pMouseClickImage0;

	bool	m_bSensitive;
	bool	m_bSensitiveView;

private:
	int		m_nMouseOverLine;

	int		m_nSELECTED_INDEX;

	bool	m_bSelectInit;
};