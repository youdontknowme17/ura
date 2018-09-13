// ServerManager.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "ServerManager.h"
#include "ServerManagerDlg.h"
#include "../MfcExLib/MinBugTrap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CServerManagerApp

BEGIN_MESSAGE_MAP(CServerManagerApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CServerManagerApp ����

CServerManagerApp::CServerManagerApp()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
	BUG_TRAP::BugTrapInstall( std::string(_T("ServerManager")));
}


// ������ CServerManagerApp ��ü�Դϴ�.

CServerManagerApp theApp;


// CServerManagerApp �ʱ�ȭ

BOOL CServerManagerApp::InitInstance()
{
	// ���� ���α׷� �Ŵ��佺Ʈ�� ComCtl32.dll ���� 6 �̻��� ����Ͽ� ���־� ��Ÿ����
	// ����ϵ��� �����ϴ� ���, Windows XP �󿡼� �ݵ�� InitCommonControls()�� �ʿ��մϴ�. 
	// InitCommonControls()�� ������� ������ â�� ���� �� �����ϴ�.
    /*
    if (!AfxOleInit())
	{
		AfxMessageBox("afxoleinit()");
		return FALSE;
	}
    */
	AfxEnableControlContainer();

	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();


	CServerManagerDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: ���⿡ ��ȭ ���ڰ� Ȯ���� ���� �������� ��� ó����
		// �ڵ带 ��ġ�մϴ�.
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: ���⿡ ��ȭ ���ڰ� ��Ҹ� ���� �������� ��� ó����
		// �ڵ带 ��ġ�մϴ�.
	}

	// ��ȭ ���ڰ� �������Ƿ� ���� ���α׷��� �޽��� ������ �������� �ʰ�
	// ���� ���α׷��� ���� �� �ֵ��� FALSE�� ��ȯ�մϴ�.
	return FALSE;
}
