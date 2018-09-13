// ItemProperty2.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ItemEdit.h"
#include "ItemSimple.h"

#include "SheetWithTab.h"
#include "GLItemDef.h"

// CItemSimple ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CItemSimple, CPropertyPage)
CItemSimple::CItemSimple()
	: CPropertyPage(CItemSimple::IDD)
{
	m_bDlgInit = FALSE;
	m_pItem = NULL;
	m_pDummyItem = new SITEMSIMPLE;
}

CItemSimple::~CItemSimple()
{
	SAFE_DELETE ( m_pDummyItem );
}

void CItemSimple::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CItemSimple, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnBnClickedButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnBnClickedButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_PREV, OnBnClickedButtonPrev)
END_MESSAGE_MAP()


// CItemSimple �޽��� ó�����Դϴ�.

void CItemSimple::OnBnClickedButtonCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_pItem = NULL;
	m_pSheetTab->SetActivePage ( m_CallPage );
}

void CItemSimple::OnBnClickedButtonOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.	
	InverseUpdateItems();

	*m_pItem = *m_pDummyItem;

	m_pItem = NULL;
	m_pSheetTab->SetActivePage ( m_CallPage );
}

BOOL CItemSimple::SetItem ( SITEMSIMPLE* pItem )
{
	if ( m_pItem )
	{
		return FALSE;
	}

	m_pItem = pItem;

	//	���̷� ��������
	*m_pDummyItem = *m_pItem;

	if ( m_bDlgInit ) 
	{
		InitDefaultCtrls ();
		UpdateItems ();		
	}

	return TRUE;
}

void CItemSimple::UpdateItems ()
{
	CString	str;
	CComboBox* pComboBox = NULL;
	int nIndex = -1;

	//	���� �������� ����
	pComboBox = (CComboBox*)GetDlgItem ( IDC_COMBO_TYPE );	
	pComboBox->SetCurSel ( m_pDummyItem->sSimpleType );	

	//	����
	str.Format ( "%d", m_pDummyItem->dwCount );
	GetDlgItem ( IDC_EDIT_COUNT )->SetWindowText ( str.GetBuffer () );

	//	��
	str.Format ( "%d", m_pDummyItem->dwVolume );
	GetDlgItem ( IDC_EDIT_VOLUME )->SetWindowText ( str.GetBuffer () );		

	//	����
	str.Format ( "%d", m_pDummyItem->dwRef03 );
	GetDlgItem ( IDC_EDIT_NEEDLEVEL )->SetWindowText ( str.GetBuffer () );

	//	����
	str.Format ( "%d", m_pDummyItem->dwRef04 );
	GetDlgItem ( IDC_EDIT_INVENSIZEX )->SetWindowText ( str.GetBuffer () );
}

BOOL CItemSimple::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_bDlgInit = TRUE;

	InitDefaultCtrls ();
	UpdateItems ();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CItemSimple::InverseUpdateItems ()
{
	CString	str;
	CComboBox* pComboBox = NULL;
	int nIndex = -1;

	//	���� �������� ����
	pComboBox = (CComboBox*)GetDlgItem ( IDC_COMBO_TYPE );	
	nIndex = pComboBox->GetCurSel ();
	m_pDummyItem->sSimpleType = (EMITEMSIMPLE)nIndex;

	//	����
	GetDlgItem ( IDC_EDIT_COUNT )->GetWindowText ( str );
	m_pDummyItem->dwCount = (DWORD) atoi ( str.GetBuffer () );

	//	��	
	GetDlgItem ( IDC_EDIT_VOLUME )->GetWindowText ( str );		
	m_pDummyItem->dwVolume = (DWORD) atoi ( str.GetBuffer () );

	//	����	
	GetDlgItem ( IDC_EDIT_NEEDLEVEL )->GetWindowText ( str );
	m_pDummyItem->dwRef03 = (DWORD) atoi ( str.GetBuffer () );

	//	����	
	GetDlgItem ( IDC_EDIT_INVENSIZEX )->GetWindowText ( str );
	m_pDummyItem->dwRef04 = (DWORD) atoi ( str.GetBuffer () );
}

void CItemSimple::OnBnClickedButtonPrev()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	InverseUpdateItems();

	*m_pItem = *m_pDummyItem;

	m_pSheetTab->m_ItemParentPage.SetCallPage ( m_CallPage );
	m_pSheetTab->m_ItemParentPage.SetItem ( m_pItem );
	m_pSheetTab->SetActivePage ( ITEMPARENTPAGE );

	m_pItem = NULL;
}

void CItemSimple::InitDefaultCtrls ()
{
	CComboBox* pComboBox = NULL;

	pComboBox = (CComboBox*) GetDlgItem ( IDC_COMBO_TYPE );	
	while ( pComboBox->GetCount() ) pComboBox->DeleteString ( 0 );	
	for ( int i = 0; i < SIMPLE_NSIZE; i++ )
	{
		pComboBox->AddString ( COMMENT::ITEMSIMPLE[i] );
	}
	pComboBox->SetCurSel ( 0 );

	GetDlgItem ( IDC_BUTTON_NEXT )->EnableWindow ( FALSE );
}