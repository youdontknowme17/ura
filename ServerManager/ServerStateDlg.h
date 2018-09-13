#pragma once
#include "afxwin.h"


// CServerStateDlg ��ȭ �����Դϴ�.

class CServerStateDlg : public CDialog
{
	DECLARE_DYNAMIC(CServerStateDlg)

public:
	CServerStateDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CServerStateDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SVERSTATE_DIALOG };

	void SetServerState( std::vector<G_SERVER_INFO_DLG*> vecState );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CMatrixStatic m_CurrentNum[8];
	CMatrixStatic m_MaxNum[8];
	CMatrixStatic m_SvrNum[8];
	virtual BOOL OnInitDialog();
private:
	std::vector<G_SERVER_INFO_DLG*> m_vecState;
};
