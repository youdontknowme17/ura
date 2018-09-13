#pragma once
#include "afxcmn.h"


// CJobManagerDlg ��ȭ �����Դϴ�.

class CJobManagerDlg : public CDialog
{
	DECLARE_DYNAMIC(CJobManagerDlg)

public:
	CJobManagerDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CJobManagerDlg();
	BOOL Create();
	void TopWindow();
	void Refresh();
	void InitControl();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_JOB_SCHEDULE };
private:
	CDialog* m_pDlg;
	std::vector<int> m_vecGroupList;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:	
	virtual BOOL OnInitDialog();
	afx_msg void OnNcDestroy();	
	CListCtrl m_ListJob;

	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedBtnBroadcastMessageAdd();
	afx_msg void OnNMRclickListJob(NMHDR *pNMHDR, LRESULT *pResult);

	void OnJobAdd();
	void OnJobEdit();
	void OnJobDelete();
	afx_msg void OnBnClickedSchExport();
	afx_msg void OnBnClickedSchImport();

	void SetGroupList( std::vector<int> vecGroupList ) { m_vecGroupList = vecGroupList; }
};
