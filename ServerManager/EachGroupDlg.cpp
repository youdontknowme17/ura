// EachGroupDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include <vector>
#include "ServerManager.h"
#include "EachGroupDlg.h"
#include ".\eachgroupdlg.h"


// CEachGroupDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CEachGroupDlg, CDialog)
CEachGroupDlg::CEachGroupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEachGroupDlg::IDD, pParent)
{
}

CEachGroupDlg::~CEachGroupDlg()
{
}

void CEachGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SVR_GROUP_CHECK1, m_SvrGroupCheck[0]);
	DDX_Control(pDX, IDC_SVR_GROUP_CHECK2, m_SvrGroupCheck[1]);
	DDX_Control(pDX, IDC_SVR_GROUP_CHECK3, m_SvrGroupCheck[2]);
	DDX_Control(pDX, IDC_SVR_GROUP_CHECK4, m_SvrGroupCheck[3]);
	DDX_Control(pDX, IDC_SVR_GROUP_CHECK5, m_SvrGroupCheck[4]);
	DDX_Control(pDX, IDC_SVR_GROUP_CHECK6, m_SvrGroupCheck[5]);
	DDX_Control(pDX, IDC_SVR_GROUP_CHECK7, m_SvrGroupCheck[6]);
	DDX_Control(pDX, IDC_SVR_GROUP_CHECK8, m_SvrGroupCheck[7]);
}


BEGIN_MESSAGE_MAP(CEachGroupDlg, CDialog)
END_MESSAGE_MAP()


// CEachGroupDlg �޽��� ó�����Դϴ�.
void CEachGroupDlg::SetEachGroupList( std::vector<int> vecEachGroup )
{ 
	m_vecEachGroup.clear(); 
	m_vecEachGroup = vecEachGroup; 

	GetDlgItem(IDC_SVR_GROUP_CHECK1)->EnableWindow(TRUE);
}

void CEachGroupDlg::GetSelectEachGroupList()
{
	BYTE i;
	m_vecCheckServ.clear();
	for( i = 0; i < 8; i++ )
	{
		CButton *pButton = (CButton *)GetDlgItem(IDC_SVR_GROUP_CHECK1+i);
		if( pButton->GetCheck() )
			m_vecCheckServ.push_back(i);
	}
}

BOOL CEachGroupDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	BYTE i, j;
	bool bFind = FALSE;
	for( i = 0; i < 8; i++ )
	{
		bFind = FALSE;
		for( j = 0; j < m_vecEachGroup.size(); j++ )
		{
			// Server Group Num �� 1���� �����ϹǷ� i�� 1�� ���Ѵ�.
			if( i+1 == m_vecEachGroup[j] ) bFind = TRUE;			
		}
		if( bFind )
		{
			GetDlgItem(IDC_SVR_GROUP_CHECK1+i)->EnableWindow(TRUE);
		}else{
			GetDlgItem(IDC_SVR_GROUP_CHECK1+i)->EnableWindow(FALSE);
		}
	}



	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

