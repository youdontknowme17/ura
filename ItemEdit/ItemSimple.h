#pragma once


// CItemSimple ��ȭ �����Դϴ�.
class	CsheetWithTab;
struct	SITEMSIMPLE;
class CItemSimple : public CPropertyPage
{
	DECLARE_DYNAMIC(CItemSimple)

public:
	CItemSimple();
	virtual ~CItemSimple();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ITEMSIMPLE_DIALOG };

protected:
	CsheetWithTab*	m_pSheetTab;
	int				m_CallPage;

	SITEMSIMPLE*	m_pDummyItem;
	SITEMSIMPLE*	m_pItem;

	BOOL			m_bDlgInit;

public:
	void	SetCallPage ( int CallPage ) { m_CallPage = CallPage; }
	void	SetSheetTab ( CsheetWithTab* pSheetTab ) { m_pSheetTab = pSheetTab; }
	BOOL	SetItem ( SITEMSIMPLE* pItem );

protected:
	void	UpdateItems ();
	void	InverseUpdateItems ();
	void	InitDefaultCtrls ();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnBnClickedButtonOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonPrev();
};
