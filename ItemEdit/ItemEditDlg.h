// ItemEditDlg.h : ��� ����
//

#pragma once

#include "afxcmn.h"
#include "afxwin.h"


class	CsheetWithTab;

// CItemEditDlg ��ȭ ����
class CItemEditDlg : public CDialog
{
// ����
public:
	CItemEditDlg(CWnd* pParent = NULL);	// ǥ�� ������

// ��ȭ ���� ������
	enum { IDD = IDD_ITEMEDIT_DIALOG };

	enum CNV_TYPE
	{
		CNV_1		= 0,
		CNV_2		= 1,
		CNV_3		= 2,
		CNV_4		= 3, // add convert to all class by CN Dev
		CNV_5		= 4, // add convert to all class by CN Dev
		CNV_SIZE	= 5,
	};

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ����

public:

	CSearchDialog	m_wndSearchDlg;
	CsheetWithTab	*m_pSheetTab;

	char			m_szAppPath[MAX_PATH];
	CString			m_strAppPath;
	CComboBox		m_cCnvType;

private:
	CToolBar    m_wndToolBar;

private:
//	void	MakeToolBar ();

public:	
	void	EnableMenu ( UINT uIDEnableItem, BOOL bEnable );
//	void	EnableToobar ( UINT uIDEnableItem, BOOL bEnable );
	void	SetReadMode();

protected:
	bool DoSave ();

// ����
protected:
	HICON m_hIcon;

	// �޽��� �� �Լ��� �����߽��ϴ�.
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
protected:
	virtual void PostNcDestroy();
public:
	afx_msg void OnMenuitemLoad();
	afx_msg void OnMenuitemSave();
	afx_msg void OnMenuitemExit();
	afx_msg void OnMenuitemNew();	
	afx_msg void OnBnClickedButtonUpdateSync();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonStrtbSync();
	afx_msg void OnBnClickedButtonReloadStrtbl();
	afx_msg void OnBnClickedBtnCsvSave();
	afx_msg void OnBnClickedBtnCsvLoad();
	afx_msg void OnBnClickedItemSearchButton();
	afx_msg void OnBnClickedSearchReportButton();
	afx_msg void OnBnClickedButtonSavecsv2();
	afx_msg void OnBnClickedButtonRun();
};
