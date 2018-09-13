// NpcTalkDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "NpcAction.h"
#include "NpcTalkDialog.h"
#include "NpcTalk.h"
#include "NpcTalkControl.h"
#include "NpcDialogueSet.h"
#include "NpcDialogue.h"
#include "NpcDialogueCase.h"

// CNpcTalkDialog ��ȭ �����Դϴ�.

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW


IMPLEMENT_DYNAMIC(CNpcTalkDialog, CDialog)
CNpcTalkDialog::CNpcTalkDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CNpcTalkDialog::IDD, pParent)
{
	m_pTalkControl = NULL;
	m_pNpcDialogueSet = NULL;
	m_bModify = FALSE;
}

CNpcTalkDialog::~CNpcTalkDialog()
{
}

void CNpcTalkDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNpcTalkDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnBnClickedButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnBnClickedButtonOk)
	ON_CBN_SELCHANGE(IDC_COMBO_ACTION_TYPE, OnCbnSelchangeComboActionType)
	ON_CBN_SELCHANGE(IDC_COMBO_ACTION_DATA, OnCbnSelchangeComboActionData)
END_MESSAGE_MAP()


// CNpcTalkDialog �޽��� ó�����Դϴ�.

void CNpcTalkDialog::OnBnClickedButtonCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	OnCancel ();
}

void CNpcTalkDialog::OnBnClickedButtonOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	assert ( m_pTalkControl && "����Ÿ ������ �߸��Ǿ����ϴ�." );

	CString strTemp;

	GetDlgItem ( IDC_EDIT_TALKID )->GetWindowText ( strTemp );
	m_Talk.m_NativeID = (DWORD) atoi ( strTemp.GetBuffer () );

	GetDlgItem ( IDC_EDIT_TALK )->GetWindowText ( strTemp );
	m_Talk.m_szTalk = strTemp;

	if ( !strTemp.GetLength () )
	{
		MessageBox ( "����κ��� ����ֽ��ϴ�.", "���", MB_OK | MB_ICONASTERISK );
		return ;
	}

	if ( !m_bModify )
	{
		SNpcTalk* pNewTalk = new SNpcTalk;
		*pNewTalk = m_Talk;

		if ( m_pTalkControl->AddTalk ( pNewTalk ) )
		{
			OnOK ();
		}
		else
		{
			SAFE_DELETE ( pNewTalk );
			MessageBox ( "�̹� ��ϵ� ID�� �����մϴ�." );
		}
	}
	else
	{
		SNpcTalk* pTalk = m_pTalkControl->GetTalk ( m_Talk.m_NativeID );
		*pTalk = m_Talk;
		OnOK ();
	}
}

void CNpcTalkDialog::SetTalkData ( CNpcDialogueSet* pNpcDialogueSet, CNpcTalkControl* pTalkControl )
{
	assert ( pTalkControl && "���� �����ǰ� �ֽ��ϴ�." );
	assert ( pNpcDialogueSet && "���� �����ǰ� �ֽ��ϴ�." );

	m_pNpcDialogueSet = pNpcDialogueSet;
	m_pTalkControl = pTalkControl;
}

BOOL CNpcTalkDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
    CComboBox* pComboBox = ( CComboBox* ) GetDlgItem ( IDC_COMBO_ACTION_TYPE );
	for ( int i = 0; i < SNpcTalk::EM_TALK_SIZE; i++ )
	{
		int nIndex = pComboBox->InsertString ( i, SNpcTalk::szActionType[i] );
		pComboBox->SetItemData ( nIndex, i );
	}

	if ( !m_bModify )
	{
		m_Talk.Init ();
		m_Talk.m_NativeID = m_pTalkControl->MakeNativeID ();

		CString strTemp;

		strTemp.Format ( "%d", m_Talk.m_NativeID );		 
		GetDlgItem ( IDC_EDIT_TALKID )->SetWindowText ( strTemp );

		pComboBox->SetCurSel ( 0 );
	}
	else
	{
		CString strTemp;

		strTemp.Format ( "%d", m_Talk.m_NativeID );		 
		GetDlgItem ( IDC_EDIT_TALKID )->SetWindowText ( strTemp );

		GetDlgItem ( IDC_EDIT_TALK )->SetWindowText ( m_Talk.m_szTalk );		

		pComboBox->SetCurSel ( m_Talk.m_emActionType );

		//	�޺��ڽ��� �ڵ����� ���õǾ��ֵ��� �ϱ� ������
		DWORD BackupActionID = m_Talk.m_ActionID;	//	OnCbnSelchangeComboActionType�� ���鼭, m_Talk.m_ActionID�� ���� �ٲٱ� ������..
		OnCbnSelchangeComboActionType ();
		CComboBox* pDataComboBox = ( CComboBox* ) GetDlgItem ( IDC_COMBO_ACTION_DATA );
		m_Talk.m_ActionID = BackupActionID;
		pDataComboBox->SetCurSel ( m_Talk.m_ActionID );
	}
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CNpcTalkDialog::OnCbnSelchangeComboActionType()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CComboBox* pDataComboBox = ( CComboBox* ) GetDlgItem ( IDC_COMBO_ACTION_DATA );
	pDataComboBox->ResetContent ();

	CComboBox* pTypeComboBox = ( CComboBox* ) GetDlgItem ( IDC_COMBO_ACTION_TYPE );
	int nIndex = pTypeComboBox->GetCurSel ();
	m_Talk.m_emActionType = (int)pTypeComboBox->GetItemData ( nIndex );
    
	switch ( nIndex )
	{
	case SNpcTalk::EM_DO_NOTHING:
		{
		}
		break;
	case SNpcTalk::EM_PAGE_MOVE:
		{
			CNpcDialogueSet::DIALOGMAP* pMap = m_pNpcDialogueSet->GetDialogueMap ();
			if ( pMap->size() )
			{
				CNpcDialogueSet::DIALOGMAP_IT iter = pMap->begin();
				CNpcDialogueSet::DIALOGMAP_IT iter_end = pMap->end();	
				for ( ; iter!=iter_end; ++iter )
				{
					CNpcDialogue* pDialogue = (*iter).second;
					CNpcDialogueCase* pPostiveCase = pDialogue->GetDialogueCase ( TRUE );

					CString strTemp;
					strTemp.Format ( "[%03d] %s", pDialogue->GetNativeID(), pPostiveCase->GetBasicTalk () );
					int nItem = pDataComboBox->AddString ( strTemp );
					pDataComboBox->SetItemData ( nItem, pDialogue->GetNativeID() );					
				}
				pDataComboBox->SetCurSel ( 0 );

				OnCbnSelchangeComboActionData ();
			}
		}
		break;
	case SNpcTalk::EM_BASIC:
		{
			CComboBox* pComboBox = ( CComboBox* ) GetDlgItem ( IDC_COMBO_ACTION_DATA );
			for ( int i = 0; i < SNpcTalk::EM_BASIC_SIZE; i++ )
			{
				int nIndex = pComboBox->InsertString ( i, SNpcTalk::szBasicDesc[i] );
				pComboBox->SetItemData ( nIndex, i );
			}
			pDataComboBox->SetCurSel ( 0 );

			OnCbnSelchangeComboActionData ();
		}
		break;
	case SNpcTalk::EM_QUEST:
		{
		}
		break;
	}
}

void CNpcTalkDialog::OnCbnSelchangeComboActionData()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CComboBox* pDataComboBox = ( CComboBox* ) GetDlgItem ( IDC_COMBO_ACTION_DATA );
	int nIndex = pDataComboBox->GetCurSel ();
	m_Talk.m_ActionID = (DWORD)pDataComboBox->GetItemData ( nIndex );
}

void CNpcTalkDialog::SetModifyData ( BOOL bModify, SNpcTalk* pTalk )
{
	m_bModify = bModify;
	m_Talk = *pTalk;
}