#pragma once


// CChatMsgDlg ��ȭ �����Դϴ�.

class CChatMsgDlg : public CDialog
{
	DECLARE_DYNAMIC(CChatMsgDlg)

public:
	CChatMsgDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CChatMsgDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_MESSAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strMessage;
};
