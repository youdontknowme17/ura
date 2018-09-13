#ifndef S_SERVERVIEWDLG_H_
#define S_SERVERVIEWDLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "afxcmn.h"
#include "afxwin.h"
#include "Resource.h"
#include "afxtempl.h"
#include "s_CNetCtrl.h"

#include "PushPin.h" 
#include "AlexfStaticHistogram.h"

#define WM_GOODBYE     WM_USER + 5

// CServerViewDlg ��ȭ �����Դϴ�.

class CServerViewDlg : public CDialog
{
	DECLARE_DYNAMIC(CServerViewDlg)
public:
	CServerViewDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CServerViewDlg();
	BOOL Create();
	void TopWindow();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SERVERVIEW_DIALOG };

private:
	CDialog* m_pDlg;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CPushPinButton m_PinButton;
	CAlexfStaticHistogram m_UserHistogram;

public:
	void UpdateStatus(G_SERVER_INFO gsi);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnNcDestroy();	
	afx_msg void OnSysColorChange();;
	
	virtual BOOL OnInitDialog();
};

#endif // S_SERVERVIEWDLG_H_

