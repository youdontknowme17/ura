// ChatMsgDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ServerManager.h"
#include "ChatMsgDlg.h"


// CChatMsgDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CChatMsgDlg, CDialog)
CChatMsgDlg::CChatMsgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChatMsgDlg::IDD, pParent)
	, m_strMessage(_T(""))
{
}

CChatMsgDlg::~CChatMsgDlg()
{
}

void CChatMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strMessage);
}


BEGIN_MESSAGE_MAP(CChatMsgDlg, CDialog)
END_MESSAGE_MAP()


// CChatMsgDlg �޽��� ó�����Դϴ�.
