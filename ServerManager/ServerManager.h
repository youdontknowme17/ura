// ServerManager.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// �� ��ȣ


// CServerManagerApp:
// �� Ŭ������ ������ ���ؼ��� ServerManager.cpp�� �����Ͻʽÿ�.
//

class CServerManagerApp : public CWinApp
{
public:
	CServerManagerApp();

// ������
	public:
	virtual BOOL InitInstance();

// ����

	DECLARE_MESSAGE_MAP()
};

extern CServerManagerApp theApp;
