// ServerViewDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ServerManager.h"
#include "ServerViewDlg.h"
#include "ServerManagerDlg.h"

// CServerViewDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CServerViewDlg, CDialog)
CServerViewDlg::CServerViewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerViewDlg::IDD, pParent)
{
	m_pDlg = (CDialog*) pParent;
	Create();
	m_PinButton.SetWnd(this);	
}

CServerViewDlg::~CServerViewDlg()
{
}

BOOL CServerViewDlg::Create()
{
	return CDialog::Create(CServerViewDlg::IDD);
}

BOOL CServerViewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_PinButton.SetPinned(TRUE);

	// Top most window
	SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	
	m_UserHistogram.SetTextLeft();
	m_UserHistogram.SetMotionLtoR(FALSE);

	m_UserHistogram.Add(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CServerViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_PIN, m_PinButton);
	DDX_Control(pDX, IDC_STATIC_USER, m_UserHistogram);
}


BEGIN_MESSAGE_MAP(CServerViewDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_NCDESTROY()
	ON_WM_SYSCOLORCHANGE()
END_MESSAGE_MAP()

// CServerViewDlg �޽��� ó�����Դϴ�.

void CServerViewDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_pDlg != NULL) {
		// modeless case -- do not call base class OnOK
		UpdateData(TRUE);
		m_pDlg->PostMessage(WM_GOODBYE, IDOK);
		DestroyWindow();
	}
	else {
		DestroyWindow();
	}
}

void CServerViewDlg::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_pDlg != NULL) {
		// modeless case -- do not call base class OnCancel
		m_pDlg->PostMessage(WM_GOODBYE, IDCANCEL);
		DestroyWindow();
	}
	else {		
		DestroyWindow();
	}
}

// ���̾�α�����, �޸� ����
void CServerViewDlg::OnNcDestroy()
{
	CDialog::OnNcDestroy();
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	CServerManagerDlg *pDlg;
	pDlg = (CServerManagerDlg*) m_pDlg;
	pDlg->RemoveDlgList(this);
	delete this;
}

void CServerViewDlg::OnSysColorChange()
{
	CDialog::OnSysColorChange();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.

	//Inform the push pin of the system color changes
	m_PinButton.ReloadBitmaps();
}

void CServerViewDlg::TopWindow()
{
	if (m_PinButton.IsPinned())
		SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	else
		SetWindowPos(&CWnd::wndNoTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
}

void CServerViewDlg::UpdateStatus(G_SERVER_INFO gsi)
{
	int nPercent = (int) (gsi.nServerCurrentClient * 100/gsi.nServerMaxClient);
	if (nPercent < 5) 
		nPercent = 5;
	m_UserHistogram.Add(nPercent);
}
