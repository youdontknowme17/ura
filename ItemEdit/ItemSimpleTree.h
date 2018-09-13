#pragma once
#include "afxcmn.h"
#include "GLItemMan.h"

// CItemSimpleTree ��ȭ �����Դϴ�.
class	CsheetWithTab;
class CItemSimpleTree : public CPropertyPage
{
	DECLARE_DYNAMIC(CItemSimpleTree)

public:
	CItemSimpleTree();   // ǥ�� �������Դϴ�.
	virtual ~CItemSimpleTree();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ITEMSIMPLETREE_DIALOG };

protected:
	CsheetWithTab*	m_pSheetTab;
	CWnd*			m_pComboBox;
	BOOL			m_bDlgInit;

public:
	BOOL	UpdateTreeItem ();

public:
	void	SetSheetTab ( CsheetWithTab *pSheetTab ) { m_pSheetTab = pSheetTab; }
	void	SetComboBoxHandle ( CWnd* pWnd ) { m_pComboBox = pWnd; }
	void	CleanAllItem ();

protected:
	PITEMSIMPLENODE	m_pItemNode;

	//	Ʈ������ �������� �����ϱ� ���� ���
	BOOL			m_bDummyHasItem;
	PITEMSIMPLENODE	m_pDummyItemNode;

protected:
	BOOL	AddItem ();	
	BOOL	DelItem ();
	BOOL	ModifyItem ();

protected:
	BOOL	CopyItem ();
	BOOL	PasteItemNew ();
	BOOL	PasteItemValue ();

protected:
	int		GetSelectMenu ();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_ctrlItemTree;	
	virtual BOOL OnInitDialog();	
	afx_msg void OnNMRclickTreeItem(NMHDR *pNMHDR, LRESULT *pResult);	
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
