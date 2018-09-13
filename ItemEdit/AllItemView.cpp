// AllItemView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ItemEdit.h"
#include "SheetWithTab.h"



// CAllItemView ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CAllItemView, CPropertyPage)
CAllItemView::CAllItemView()
	: CPropertyPage(CAllItemView::IDD)
{
	m_SelectMenu = -1;
}

CAllItemView::~CAllItemView()
{
}

void CAllItemView::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_ITEM, m_ctrlItemTree);
}


BEGIN_MESSAGE_MAP(CAllItemView, CPropertyPage)
	ON_NOTIFY(NM_RCLICK, IDC_TREE_ITEM, OnNMRclickTreeItem)
END_MESSAGE_MAP()


// CAllItemView �޽��� ó�����Դϴ�.

BOOL CAllItemView::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	UpdateTreeItem ();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
void CAllItemView::OnNMRclickTreeItem(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;	

	switch ( GetSelectMenu() )
	{
	case ID_MENUITEM_PARENTMODIFY:
		{
			m_pSheetTab->ActiveItemParentPage();			
		}
		break;
	case ID_MENUITEM_SIMPLEMODIFY:
		{
			m_pSheetTab->ActiveItemSimplePage();
		}
		break;
	case ID_MENUITEM_SUITMODIFY:
		{
			m_pSheetTab->ActiveItemSuitPage ();
		}
		break;
	case ID_MENUITEM_ADD:
		{
			m_pSheetTab->ActiveItemParentPage();
		}
		break;
	case ID_MENUITEM_DEL:
		{
			if ( MessageBox ( "���� �����Ͻðڽ��ϱ�?", "����", MB_YESNO ) == IDYES )
			{
				MessageBox ( "�����Ͽ����ϴ�." );
			}
			else
			{				
			}
		}
		break;
	};
}

BOOL	CAllItemView::AddItem ()
{
	return TRUE;
}

void	CAllItemView::SetSelectMenu ( int SelectMenu )
{
	m_SelectMenu = SelectMenu;
}

int		CAllItemView::GetSelectMenu ()
{
	POINT OriginPoint, ClientPoint;

	//	<--	��Ŭ������ ��Ŀ�� �ű��
	GetCursorPos ( &OriginPoint );
	ClientPoint = OriginPoint;

	m_ctrlItemTree.ScreenToClient(&ClientPoint);

	UINT uFlag = 0;
	HTREEITEM hItem = m_ctrlItemTree.HitTest ( ClientPoint, &uFlag );
	m_ctrlItemTree.SelectItem ( hItem );
	//	-->


	//	<--	���� �۵� �޴� ����
	int pMenuID = 0;
	CMenu Menu;

	DWORD SelectionMade = -1;
	switch ( m_SelectMenu )
	{
	case 0:
		{
		VERIFY ( Menu.LoadMenu ( IDR_MENU_POPUP ) );

		CMenu* pPopup = Menu.GetSubMenu ( pMenuID );
		ASSERT ( pPopup != NULL );	

		SelectionMade = pPopup->TrackPopupMenu (
			TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD,
			OriginPoint.x, OriginPoint.y, this );

		pPopup->DestroyMenu ();
		}
		break;
	}
	//	-->	

	return SelectionMade;
}

BOOL	CAllItemView::UpdateTreeItem ()
{
	m_ctrlItemTree.DeleteAllItems ();

	for ( int i = 0; i < 4000; i++ )
        m_ctrlItemTree.InsertItem("��ü ������");	

	return TRUE;
}