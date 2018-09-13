#pragma once
#include "afxcmn.h"


// CAllItemView ��ȭ �����Դϴ�.
class	CsheetWithTab;
class CAllItemView : public CPropertyPage
{
	DECLARE_DYNAMIC(CAllItemView)

public:
	CAllItemView();   // ǥ�� �������Դϴ�.
	virtual ~CAllItemView();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ALLITEMVIEW_DIALOG };

protected:
	CsheetWithTab	*m_pSheetTab;

protected:
	int		m_SelectMenu;

public:
	void	SetSelectMenu ( int SelectMenu );
	BOOL	UpdateTreeItem ();

public:
	void	SetSheetTab ( CsheetWithTab *pSheetTab ) { m_pSheetTab = pSheetTab; }

protected:
	BOOL	AddItem ();	
	BOOL	DelItem ();
	BOOL	ModifyItem ();

protected:
	int		GetSelectMenu ();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_ctrlItemTree;
	virtual BOOL OnInitDialog();	
	afx_msg void OnNMRclickTreeItem(NMHDR *pNMHDR, LRESULT *pResult);
};
