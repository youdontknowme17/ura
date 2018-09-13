#pragma once
#include "afxwin.h"


// CEachGroupDlg ��ȭ �����Դϴ�.

class CEachGroupDlg : public CDialog
{
	DECLARE_DYNAMIC(CEachGroupDlg)

public:
	CEachGroupDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CEachGroupDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_EACHGROUP };

	void SetEachGroupList( std::vector<int> vecEachGroup );
	void GetSelectEachGroupList();
	std::vector<int> GetSelectEachGroupList() { return m_vecCheckServ; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

private:
	std::vector<int> m_vecEachGroup;
	std::vector<int> m_vecCheckServ;
public:
	virtual BOOL OnInitDialog();
	CButton m_SvrGroupCheck[8];
};
