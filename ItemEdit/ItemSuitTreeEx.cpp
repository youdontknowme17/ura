#include "stdafx.h"
#include <vector>
#include "ItemEdit.h"
#include "SheetWithTab.h"
#include "ItemSuitTree.h"
#include "GLStringTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BOOL CItemSuitTree::UpdateItem ( SITEM* pItem, HTREEITEM hMainItem )
{
	CString strItem;
	strItem = GetFormatName ( pItem );
	
	if ( !hMainItem )
	{
		hMainItem = m_ctrlItemTree.InsertItem ( strItem, m_TreeRoot );
		m_ctrlItemTree.SetItemData ( hMainItem, (DWORD_PTR)pItem );
	}
	else
	{
		HTREEITEM hSubItem = m_ctrlItemTree.InsertItem ( strItem, hMainItem );
		m_ctrlItemTree.SetItemData ( hSubItem, (DWORD_PTR)pItem );
	}

	return TRUE;
}

BOOL CItemSuitTree::DelItem ()
{
	if ( GetSelectItem() )
	{
		WORD WID = GetSelectItem()->m_sItem.sBasicOp.sNativeID.wMainID;
		WORD SID = GetSelectItem()->m_sItem.sBasicOp.sNativeID.wSubID;

		if ( SID == 0 )	// ��Ʈ�� �����ϴ� ���̸�...
		{
			for ( int i = 0; i < GLItemMan::MAX_SID; i++ )
			{
				GLStringTable::GetInstance().DeleteString( WID, i, GLStringTable::ITEM );
				GLItemMan::GetInstance().DeleteItem ( WID, i );
			}
		}
		else
		{
			GLStringTable::GetInstance().DeleteString( GetSelectItem()->m_sItem.sBasicOp.strName,
				GetSelectItem()->m_sItem.sBasicOp.strComment, GLStringTable::ITEM );
			GLItemMan::GetInstance().DeleteItem ( GetSelectItem()->m_sItem.sBasicOp.sNativeID );
		}		
		
		m_ctrlItemTree.DeleteItem ( m_ctrlItemTree.GetSelectedItem () );

		return TRUE;	
	}

	return FALSE;
}

void CItemSuitTree::UpdateName ()
{
	HTREEITEM hItem = m_ctrlItemTree.GetSelectedItem ();
	GASSERT ( hItem );
	PITEMNODE pItemNode = (PITEMNODE)m_ctrlItemTree.GetItemData ( hItem );
	GASSERT ( pItemNode );

	m_ctrlItemTree.SetItemText ( hItem, GetFormatName ( &pItemNode->m_sItem ) );
}

const char* CItemSuitTree::GetFormatName ( SITEM* pItem )
{
	GASSERT ( pItem );

	//** !!Add New Sex!!     
	static	CString strItem;
	if ( pItem->sBasicOp.dwReqCharClass==GLCC_NONE)
	{
		strItem.Format ( "[%04d/%02d] %s            [%d][%s] [%s]   [{%s}]",
			pItem->sBasicOp.sNativeID.wMainID
			, pItem->sBasicOp.sNativeID.wSubID
			, pItem->GetName()
			,pItem->sGenerateOp.dwSpecID
			,COMMENT::ITEMSUIT[pItem->sSuitOp.emSuit].c_str()
			,COMMENT::ITEMATTACK[pItem->sSuitOp.emAttack].c_str()
			, "Unusable" );
	}
	//else if ( pItem->sBasicOp.dwReqCharClass==GLCC_ALL_NEWSEX)
	//add class
	else if ( pItem->sBasicOp.dwReqCharClass==GLCC_ALL_2015)
	{
		strItem.Format ( "[%04d/%02d] %s           [%d][%s] [%s]   [{%s}]",
			pItem->sBasicOp.sNativeID.wMainID
			, pItem->sBasicOp.sNativeID.wSubID
			, pItem->GetName()
			,pItem->sGenerateOp.dwSpecID
			,COMMENT::ITEMSUIT[pItem->sSuitOp.emSuit].c_str()
			,COMMENT::ITEMATTACK[pItem->sSuitOp.emAttack].c_str()
			, "All" );
	}
	else
	{
		strItem.Format ( "[%04d/%02d] %s           [%d][%s] [%s]   [",
			pItem->sBasicOp.sNativeID.wMainID
			, pItem->sBasicOp.sNativeID.wSubID
			, pItem->GetName()
			,pItem->sGenerateOp.dwSpecID
			,COMMENT::ITEMSUIT[pItem->sSuitOp.emSuit].c_str()
			,COMMENT::ITEMATTACK[pItem->sSuitOp.emAttack].c_str() );

		if ( pItem->sBasicOp.dwReqCharClass&GLCC_FIGHTER_M )	strItem += "Brawler[M] | ";
		if ( pItem->sBasicOp.dwReqCharClass&GLCC_FIGHTER_W )	strItem += "Brawler[W] | ";
		if ( pItem->sBasicOp.dwReqCharClass&GLCC_ARMS_M )		strItem += "Swordsman[M] | ";
		if ( pItem->sBasicOp.dwReqCharClass&GLCC_ARMS_W )		strItem += "Swordsman[W] | ";
		if ( pItem->sBasicOp.dwReqCharClass&GLCC_ARCHER_M )		strItem += "Archer[M] | ";
		if ( pItem->sBasicOp.dwReqCharClass&GLCC_ARCHER_W )		strItem += "Archer[W] | ";
		if ( pItem->sBasicOp.dwReqCharClass&GLCC_SPIRIT_M )		strItem += "Shamman[M] | ";
		if ( pItem->sBasicOp.dwReqCharClass&GLCC_SPIRIT_W )		strItem += "Shamman[W] | ";
		if ( pItem->sBasicOp.dwReqCharClass&GLCC_EXTREME_M )	strItem += "Extreme[M] | ";
		if ( pItem->sBasicOp.dwReqCharClass&GLCC_EXTREME_W )	strItem += "Extreme[W] | ";
		//if ( pItem->sBasicOp.dwReqCharClass&GLCC_EXTREME_W )	strItem += "ExtremeW";
		//add class
		if ( pItem->sBasicOp.dwReqCharClass&GLCC_SCIENCE_M )	strItem += "Gunner[M] | ";
		if ( pItem->sBasicOp.dwReqCharClass&GLCC_SCIENCE_W )	strItem += "Gunner[W] | ";
		if ( pItem->sBasicOp.dwReqCharClass&GLCC_ASSASIN_M )	strItem += "Assasin[M] | ";
		if ( pItem->sBasicOp.dwReqCharClass&GLCC_ASSASIN_W )	strItem += "Assasin[W] | ";
		if ( pItem->sBasicOp.dwReqCharClass&GLCC_TESTING_M )	strItem += "Magician[M] | ";
		if ( pItem->sBasicOp.dwReqCharClass&GLCC_TESTING_W )	strItem += "Magician[W] | ";
		//add shaper class by CNDev
		if ( pItem->sBasicOp.dwReqCharClass&GLCC_SHAPER_M )	strItem += "Shaper[M] | ";
		if ( pItem->sBasicOp.dwReqCharClass&GLCC_SHAPER_W )	strItem += "Shaper[W] | ";
		strItem += "]";
	}

	CString strNewName;
	strNewName.Format( "IN_%03d_%03d", pItem->sBasicOp.sNativeID.wMainID, pItem->sBasicOp.sNativeID.wSubID );
	pItem->sBasicOp.strName = strNewName;
	strNewName.Format( "ID_%03d_%03d", pItem->sBasicOp.sNativeID.wMainID, pItem->sBasicOp.sNativeID.wSubID );
	pItem->sBasicOp.strComment = strNewName;

	return strItem.GetString ();
}

BOOL	CItemSuitTree::AddItem ()
{
	HTREEITEM hMainItem = m_ctrlItemTree.GetSelectedItem ();
	if ( hMainItem && GetSelectItem () )	//	����� �߰���
	{		
		return AddItemMain( hMainItem );
	}
	else	//	�������� �߰���
	{
		return AddItemSub ();
	}
	
	return FALSE;
}

BOOL CItemSuitTree::AddItemMain( HTREEITEM hMainItem )
{
	WORD MID = GetSelectItem()->m_sItem.sBasicOp.sNativeID.wMainID;
	WORD SID = GLItemMan::GetInstance().FindFreeSID ( MID );
	if ( SID == ITEMID_NOTFOUND )
	{
		return FALSE;
	}

	CItemNode sItemNode;
	sItemNode = *m_pDummyItemNode;
	sItemNode.m_sItem.sBasicOp.sNativeID.wMainID = MID;
	sItemNode.m_sItem.sBasicOp.sNativeID.wSubID = SID;

	char szNewName[ITEM_SZNAME];
	sprintf ( szNewName, "IN_%03d_%03d", MID, SID );
	sItemNode.m_sItem.sBasicOp.strName = szNewName;
	sprintf ( szNewName, "ID_%03d_%03d", MID, SID );
	sItemNode.m_sItem.sBasicOp.strComment = szNewName;
	GLItemMan::GetInstance().InsertItem ( MID, SID, &sItemNode );
	SITEM *pITEM = GLItemMan::GetInstance().GetItem ( MID, SID );

    HTREEITEM hParentItem = m_ctrlItemTree.GetParentItem ( hMainItem );
	if ( hParentItem != m_TreeRoot )
	{
		UpdateItem ( pITEM, hParentItem );
	}
	else
	{
		UpdateItem ( pITEM, hMainItem );
	}	

	return TRUE;
}

BOOL CItemSuitTree::AddItemSub ()
{
	WORD MID = GLItemMan::GetInstance().FindFreeMID ();
	if ( MID == ITEMID_NOTFOUND )
	{
		return FALSE;
	}
	WORD SID = GLItemMan::GetInstance().FindFreeSID ( MID );
	if ( SID == ITEMID_NOTFOUND )
	{
		return FALSE;
	}
	
	CItemNode sItemNode;
	sItemNode = *m_pDummyItemNode;
	sItemNode.m_sItem.sBasicOp.sNativeID.wMainID = MID;
	sItemNode.m_sItem.sBasicOp.sNativeID.wSubID = SID;
	
	char szNewName[ITEM_SZNAME];
	sprintf ( szNewName, "IN_%03d_%03d", MID, SID );
	sItemNode.m_sItem.sBasicOp.strName = szNewName;
	sprintf ( szNewName, "ID_%03d_%03d", MID, SID );
	sItemNode.m_sItem.sBasicOp.strComment = szNewName;
	GLItemMan::GetInstance().InsertItem ( MID, SID, &sItemNode );
	SITEM *pITEM = GLItemMan::GetInstance().GetItem ( MID, SID );

	UpdateItem ( pITEM, NULL );

	return TRUE;
}

void CItemSuitTree::ForceSelectOnRightClick ( POINT pt )
{		
	m_ctrlItemTree.ScreenToClient( &pt );

	UINT uFlag = 0;
	PITEMNODE pItemNode = NULL;
	HTREEITEM hItem = m_ctrlItemTree.HitTest ( CPoint(pt), &uFlag );


    SetSelectItem ( NULL );	
	if ( hItem )
	{
		m_ctrlItemTree.SelectItem ( hItem );
		if ( uFlag & TVHT_ONITEMLABEL )
		{			
			SetSelectItem ( (PITEMNODE)m_ctrlItemTree.GetItemData ( hItem ) );
		}
	}
}

void CItemSuitTree::SearchItemSelect( BYTE dwType, char *szItemName )
{
	const char *AllItemTypeName[] = 
	{
		"���� ���� ����",
		"ȭ��",

		"��ǰ",
		"��ų ���� ����",
		"��ȯ��",
		"������ ( ���� )",
		"������",
		"����",
		"������",
		"��ų ����Ʈ ����",
		"���� ����Ʈ ����",
		"��ų, ���� ����Ʈ ����",

		"���� ����",
		"��Ź��",
		"Ȯ����",
		"����",
		"ĳ���� �߰� ī��",
		"�κ� Ȯ�� ī��",
		"â�� Ȯ�� ī��",
		"â�� ���� ī��",
		"�����̾� ��Ʈ",
		"���λ��� ���� �Ǹ�",
		"���� ������",
		"�ڽ��� �и�",
		"��Ÿ�� ����",
		"�� ����",
		"? ������",
		"CD",
		"ģ������",
		"Ŭ��ȣ��",
		"���� �̿��",
		"�̸����� ī��",
		"��Ÿ��",
		"����÷�",
		"��ȥ��",
		"�� ī��",
		"�� ����",
		"�� �̸�����",
		"�� �÷�����",
		"�� ��Ÿ�Ϻ���",
		"�� ��ų",
		"SMS���� �߼�",
		"�� ��Ȱī��",
		"�ҹ���(�Ҹ����)",
		"���� ���",
		"��������"
	};

	m_vecSearchItemList.clear();

	bool bFind = FALSE;
	bool bFindSection = FALSE;
	int  iFindCount = 0;
	int  searchMID = -1, searchSID = -1;
	int  itemType;
	string strItemName = szItemName;

	if( dwType == 0 )
	{
		string strTemp;

		int i, iSize = (int)strlen(szItemName);
		for( i = 0; i < iSize; i++ )
		{
			if( szItemName[i] == ',' )
			{
				searchMID = atoi(strTemp.c_str());
				strTemp.clear();
			}else{
				strTemp += szItemName[i];
			}
		}
		searchSID = atoi(strTemp.c_str());

	}
	if( dwType == 4 )
	{
		itemType = atoi(szItemName);
		strItemName = AllItemTypeName[itemType];
	}

	for ( int MID = 0; MID < GLItemMan::MAX_MID; MID++ )
	{
		for ( int SID = 0; SID < GLItemMan::MAX_SID; SID++ )
		{
			bFindSection = FALSE;

			SITEM* pItem = GLItemMan::GetInstance().GetItem ( MID, SID );
			HTREEITEM hItem = m_TreeItemList[MID][SID];
			m_ctrlItemTree.SetCheck( hItem, FALSE );

			if ( pItem )
			{
				switch( dwType )
				{
				case 0: // MID/SID
					{
						if( searchMID == MID && searchSID == SID )
						{
							bFindSection = TRUE;
						}
					}
					break;
				case 1: // ������ �̸�
					{
						if( stricmp( pItem->GetName(), strItemName.c_str() ) == 0 ) { bFindSection = TRUE; }
					}
					break;
				case 2: // ��� DDS
					{
						if( stricmp( pItem->sBasicOp.strInventoryFile.c_str(), strItemName.c_str() ) == 0 ) { bFindSection = TRUE; }
					}
				    break;
				case 3: // ��� CPS
					{
						int i;
//						for( i = 0; i < GLCI_NUM_NEWSEX; i++ )
						for( i = 0; i < GLCI_NUM_EX; i++ )
						{
							if( stricmp( pItem->sBasicOp.strWearingFile[i].c_str(), strItemName.c_str() ) == 0 ) 
							{ 
								bFindSection = TRUE; 
								break;
							}
						}
					}
				    break;
				case 4: // ������ ����
					{
						if( pItem->sBasicOp.emItemType == itemType ) { bFindSection = TRUE; }
					}
					break;
				}

				if( bFindSection == TRUE )
				{
					m_ctrlItemTree.SetCheck( hItem, TRUE );
					bFind = TRUE;

					SSearchItemList inputSearchItem;
					inputSearchItem.MID = MID;
					inputSearchItem.SID = SID;
					strcpy( inputSearchItem.szItemName, pItem->GetName() );
					strcpy( inputSearchItem.szItemValue, strItemName.c_str() );
					m_vecSearchItemList.push_back( inputSearchItem );
					iFindCount++;
				}

			}
		}
	}
	if( bFind == FALSE ) 
	{
		MessageBox( "�������� ã�� ���߽��ϴ�.", "�˸�" );
	}else{
		char szTempChar[256];
		sprintf( szTempChar, "%d���� �������� ã�ҽ��ϴ�.", iFindCount );
		MessageBox( szTempChar, "�˸�" );
	}
}

void CItemSuitTree::SetSelectIndex( int iSelectIndex )
{
	if( iSelectIndex == -1 ) 
	{
		MessageBox( "�������� �����ϼ���.", "�˸�" );
		return;
	}

	int MID, SID;
	MID = m_vecSearchItemList[iSelectIndex].MID;
	SID = m_vecSearchItemList[iSelectIndex].SID;
	HTREEITEM hItem = m_TreeItemList[MID][SID];


	SetSelectItem ( NULL );	
	m_ctrlItemTree.SelectItem ( hItem );
	SetSelectItem ( (PITEMNODE)m_ctrlItemTree.GetItemData ( hItem ) );

	CWnd *pWnd = GetDlgItem(IDC_TREE_ITEM);
	pWnd->SetFocus();					
}