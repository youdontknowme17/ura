#include "StdAfx.h"
#include "InventoryPage.h"
#include "ItemSlot.h"
#include "BasicScrollBarEx.h"
#include "../EngineLib/DxCommon/DxFontMan.h"
#include "../EngineUILib/GUInterface/BasicTextBox.h"
#include "../EngineUILib/GUInterface/BasicScrollThumbFrame.h"
#include "../EngineUILib/GUInterface/BasicButton.h"
#include "../Interface/GameTextControl.h"
#include "GLGaeaClient.h"
#include "UITextControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int CInventoryPage::nSTARTLINE = 0;
const int CInventoryPage::nOUTOFRANGE = -1;

CInventoryPage::CInventoryPage () :
	m_nCurPos ( -1 ),
	m_nONE_VIEW_SLOT ( nONE_VIEW_SLOT_DEFAULT )
{
	memset ( m_pItemSlotArray, 0, sizeof( m_pItemSlotArray ) );
	memset ( m_pItemSlotArrayDummy, 0, sizeof( m_pItemSlotArrayDummy ) );
	memset ( m_Button, 0, sizeof ( CUIControl* ) * 5 );	
	memset ( m_ButtonSet, 0, sizeof ( CUIControl* ) * 5 );	
	memset ( m_ButtonText, 0, sizeof ( CBasicTextBox* ) * 5 );	
}

CInventoryPage::~CInventoryPage ()
{
}

CUIControl*	CInventoryPage::CreateControl ( const char* szControl )
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, szControl );
	RegisterControl ( pControl );
	return pControl;
}

void CInventoryPage::CreateSubControl ()
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, _DEFAULT_FONT_FLAG );
	
	CString strSlotKeyword1[nMAX_ONE_VIEW_SLOT5] =
	{
		"HANHTRANG_VATPHAM_SLOT0",
		"HANHTRANG_VATPHAM_SLOT1",
		"HANHTRANG_VATPHAM_SLOT2",
		"HANHTRANG_VATPHAM_SLOT3",
		"HANHTRANG_VATPHAM_SLOT4",
		"HANHTRANG_VATPHAM_SLOT5",
		"HANHTRANG_VATPHAM_SLOT6",
		"HANHTRANG_VATPHAM_SLOT7",
		"HANHTRANG_VATPHAM_SLOT8",
		"HANHTRANG_VATPHAM_SLOT9",
		"HANHTRANG_VATPHAM_SLOT10",
		"HANHTRANG_VATPHAM_SLOT11",
		"HANHTRANG_VATPHAM_SLOT12",
		"HANHTRANG_VATPHAM_SLOT13",
		"HANHTRANG_VATPHAM_SLOT14",
		"HANHTRANG_VATPHAM_SLOT15",
		"HANHTRANG_VATPHAM_SLOT16",
		"HANHTRANG_VATPHAM_SLOT17",
		"HANHTRANG_VATPHAM_SLOT18",
		"HANHTRANG_VATPHAM_SLOT19",
		"HANHTRANG_VATPHAM_SLOT20",
		"HANHTRANG_VATPHAM_SLOT21",
		"HANHTRANG_VATPHAM_SLOT22",
		"HANHTRANG_VATPHAM_SLOT23",
		"HANHTRANG_VATPHAM_SLOT24",
		"HANHTRANG_VATPHAM_SLOT25",
		"HANHTRANG_VATPHAM_SLOT26",
		"HANHTRANG_VATPHAM_SLOT27",
		"HANHTRANG_VATPHAM_SLOT28",
		"HANHTRANG_VATPHAM_SLOT29",
		"HANHTRANG_VATPHAM_SLOT30",
		"HANHTRANG_VATPHAM_SLOT31",
		"HANHTRANG_VATPHAM_SLOT32",
		"HANHTRANG_VATPHAM_SLOT33",
		"HANHTRANG_VATPHAM_SLOT34",
		"HANHTRANG_VATPHAM_SLOT35",
		"HANHTRANG_VATPHAM_SLOT36",
		"HANHTRANG_VATPHAM_SLOT37",
		"HANHTRANG_VATPHAM_SLOT38",
		"HANHTRANG_VATPHAM_SLOT39",
		"HANHTRANG_VATPHAM_SLOT40",
		"HANHTRANG_VATPHAM_SLOT41",
		"HANHTRANG_VATPHAM_SLOT42",
		"HANHTRANG_VATPHAM_SLOT43",
		"HANHTRANG_VATPHAM_SLOT44",
		"HANHTRANG_VATPHAM_SLOT45",
		"HANHTRANG_VATPHAM_SLOT46",
		"HANHTRANG_VATPHAM_SLOT47",
		"HANHTRANG_VATPHAM_SLOT48",
		"HANHTRANG_VATPHAM_SLOT49"
	};

	CString strButton[5] =
	{
		"HANHTRANG_BUTTON0",
		"HANHTRANG_BUTTON1",
		"HANHTRANG_BUTTON2",
		"HANHTRANG_BUTTON3",
		"HANHTRANG_BUTTON4"
	};

	CString strButtonSet[5] =
	{
		"HANHTRANG_BUTTON0_F",
		"HANHTRANG_BUTTON1_F",
		"HANHTRANG_BUTTON2_F",
		"HANHTRANG_BUTTON3_F",
		"HANHTRANG_BUTTON4_F"
	};

	CString strButtonText[5] =
	{
		"HANHTRANG_BUTTON_TEXT0",
		"HANHTRANG_BUTTON_TEXT1",
		"HANHTRANG_BUTTON_TEXT2",
		"HANHTRANG_BUTTON_TEXT3",
		"HANHTRANG_BUTTON_TEXT4"
	};

	for ( int i = 0; i < 5 ; i++ )
	{
		m_Button[i] = new CUIControl;
		m_Button[i]->CreateSub ( this, strButton[i].GetString (), UI_FLAG_DEFAULT, INVENTORY_BUTTON0 + i );
		m_Button[i]->SetVisibleSingle ( TRUE );
		m_Button[i]->SetTransparentOption( TRUE );
		RegisterControl ( m_Button[i] );

		m_ButtonSet[i] = new CUIControl;
		m_ButtonSet[i]->CreateSub ( this, strButtonSet[i].GetString (), UI_FLAG_DEFAULT );	
		m_ButtonSet[i]->SetVisibleSingle ( FALSE );
		m_ButtonSet[i]->SetTransparentOption( TRUE );
		RegisterControl ( m_ButtonSet[i] );

		m_ButtonText[i] = new CBasicTextBox;
		m_ButtonText[i]->CreateSub ( this, strButtonText[i].GetString (), UI_FLAG_DEFAULT );
		m_ButtonText[i]->SetFont ( pFont8 );
		m_ButtonText[i]->SetTextAlign ( TEXT_ALIGN_LEFT );	
		m_ButtonText[i]->AddText ( ID2GAMEWORD ( "NEW_INVENTORY_BUTTON_SLOT", i ), NS_UITEXTCOLOR::WHITE );
		RegisterControl ( m_ButtonText[i] );
	}

	for ( int i = 0; i < nMAX_ONE_VIEW_SLOT5; i++ )
	{
		m_pItemSlotArrayDummy[i] = CreateControl ( strSlotKeyword1[i].GetString () );
	}
	
	for ( int i = 0; i < EM_INVENSIZE_Y_FORCE; i++ )
	{
		m_pItemSlotArray[i] = CreateItemSlot ( strSlotKeyword1[0], ITEM_SLOT00 + i );
	}
	

	m_ButtonSet[0]->SetVisibleSingle ( TRUE );
	ResetAllItemSlotRender ( 10,50);
	SetItemSlotRender ( 0, 10 );
}
void CInventoryPage::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	SetItemIndex ( nOUTOFRANGE, nOUTOFRANGE );

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	UpdateBLOCK ();
}
void CInventoryPage::ResetAllItemSlotRender ( int index ,int nTotal )
{
	if ( nTotal < 0 ) return ;

	for ( int i = index; i < nTotal; i++ )
	{
		CItemSlot* pItemSlot = m_pItemSlotArray[i];
		if ( pItemSlot )
		{
			pItemSlot->SetVisibleSingle ( FALSE );
		}
	}
}
void CInventoryPage::SetItemSlotRender ( int nStartIndex, int nTotal )
{
	if ( nTotal < 0 ) return ;

	const UIRECT& rcParentPos = GetGlobalPos ();

	for ( int i = nStartIndex; i < nTotal; i++ )
	{
		CItemSlot* pItemSlot = m_pItemSlotArray[i];
		if ( pItemSlot )
		{
			int nAbsoluteIndex = i - nStartIndex;

			CUIControl* pDummyControl = m_pItemSlotArrayDummy[nAbsoluteIndex];			
			const UIRECT& rcSlotPos = pDummyControl->GetGlobalPos ();
			const UIRECT& rcSlotLocalPos = pDummyControl->GetLocalPos ();

			pItemSlot->SetLocalPos ( rcSlotLocalPos );
			pItemSlot->SetGlobalPos ( rcSlotPos );

			pItemSlot->SetVisibleSingle ( TRUE );
		}
	}
}
CItemSlot* CInventoryPage::CreateItemSlot ( CString strKeyword, UIGUID ControlID )
{
	CItemSlot* pItemSlot = new CItemSlot;
	pItemSlot->CreateSub ( this, strKeyword.GetString (), UI_FLAG_DEFAULT, ControlID );
	pItemSlot->CreateSubControl ( EM_INVENSIZE_X, TRUE );	
	RegisterControl ( pItemSlot );
	return pItemSlot;
}
void CInventoryPage::SetItemIndex ( int nPosX, int nPosY )
{
	m_nPosX = nPosX;
	m_nPosY = nPosY;
}

void CInventoryPage::GetItemIndex ( int* pnPosX, int* pnPosY )
{
	*pnPosX = m_nPosX;
	*pnPosY = m_nPosY;
}

void CInventoryPage::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	if ( ITEM_SLOT00 <= ControlID && ControlID < ITEM_SLOT_END )
	{
		if ( CHECK_MOUSE_IN ( dwMsg ) )
		{
			const int nPosY = ControlID - ITEM_SLOT00;
			CItemSlot* pSlot = m_pItemSlotArray[nPosY];
			const int nPosX = pSlot->GetItemIndex ();

			if ( nPosY < 0 || nPosX < 0 ) return ;

			SetItemIndex ( nPosX, nPosY );
		}
	}
	switch ( ControlID )
	{
	case INVENTORY_BUTTON0:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				INVENTORY1();
			}
		}
		break;
	case INVENTORY_BUTTON1:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				INVENTORY2();
			}
		}
		break;
	case INVENTORY_BUTTON2:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				INVENTORY3();
			}
		}
		break;

		case INVENTORY_BUTTON3:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				INVENTORY4();
			}
		}
		break;

		case INVENTORY_BUTTON4:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				INVENTORY5();
			}
		}
		break;

	
	}
}
void CInventoryPage::INVENTORY1()
{
	for ( int i = 0; i < 5; i++ )
	{
		m_Button[i]->SetVisibleSingle ( TRUE );
		m_ButtonSet[i]->SetVisibleSingle ( FALSE );
	}
	m_ButtonSet[0]->SetVisibleSingle ( TRUE );
	m_Button[0]->SetVisibleSingle ( FALSE );
	ResetAllItemSlotRender ( 10,50 );
	SetItemSlotRender ( 0, 10 );
}
void CInventoryPage::INVENTORY2()
{
	for ( int i = 0; i < 5; i++ )
	{
		m_Button[i]->SetVisibleSingle ( TRUE );
		m_ButtonSet[i]->SetVisibleSingle ( FALSE );
	}
	m_ButtonSet[1]->SetVisibleSingle ( TRUE );
	m_Button[1]->SetVisibleSingle ( FALSE );
	ResetAllItemSlotRender ( 0,10 );
	ResetAllItemSlotRender ( 20,50 );
	SetItemSlotRender ( 10, 20 );
}
void CInventoryPage::INVENTORY3()
{
	for ( int i = 0; i < 5; i++ )
	{
		m_Button[i]->SetVisibleSingle ( TRUE );
		m_ButtonSet[i]->SetVisibleSingle ( FALSE );
	}
	m_ButtonSet[2]->SetVisibleSingle ( TRUE );
	m_Button[2]->SetVisibleSingle ( FALSE );
	ResetAllItemSlotRender ( 0,20 );
	ResetAllItemSlotRender ( 30,50 );
	SetItemSlotRender ( 20, 30 );
}
void CInventoryPage::INVENTORY4()
{
	for ( int i = 0; i < 5; i++ )
	{
		m_Button[i]->SetVisibleSingle ( TRUE );
		m_ButtonSet[i]->SetVisibleSingle ( FALSE );
	}
	m_ButtonSet[3]->SetVisibleSingle ( TRUE );
	m_Button[3]->SetVisibleSingle ( FALSE );
	ResetAllItemSlotRender ( 0,30 );
	ResetAllItemSlotRender ( 40,50 );
	SetItemSlotRender ( 30, 40 );
}
void CInventoryPage::INVENTORY5()
{
	for ( int i = 0; i < 5; i++ )
	{
		m_Button[i]->SetVisibleSingle ( TRUE );
		m_ButtonSet[i]->SetVisibleSingle ( FALSE );
	}
	m_ButtonSet[4]->SetVisibleSingle ( TRUE );
	m_Button[4]->SetVisibleSingle ( FALSE );
	ResetAllItemSlotRender ( 0,40 );
	SetItemSlotRender ( 40, 50 );
}


void CInventoryPage::LoadItem ( SINVENITEM& ref_InvenItem )
{	
	int nPosX = ref_InvenItem.wPosX;
	int nPosY = ref_InvenItem.wPosY;
	m_pItemSlotArray[nPosY]->SetItemImage ( nPosX, ref_InvenItem );
}

void CInventoryPage::UnLoadItemPage ()
{
	for ( int y = 0; y < EM_INVENSIZE_Y_FORCE; y++ )
	{
		for ( int x = 0; x < EM_INVENSIZE_X; x++ )
		{
			UnLoadItem ( x, y );
		}
	}
}

SINVENITEM& CInventoryPage::GetItem ( int nPosX, int nPosY )
{
	return m_pItemSlotArray[nPosY]->GetItemImage ( nPosX );
}

void CInventoryPage::UnLoadItem ( int nPosX, int nPosY )
{
	m_pItemSlotArray[nPosY]->ResetItemImage ( nPosX );
}

void CInventoryPage::LoadItemPage ( GLInventory &ItemData )
{
	GLInventory::CELL_MAP *pItemList = ItemData.GetItemList();
	
	if ( pItemList->size () > (EM_INVENSIZE_Y_FORCE * EM_INVENSIZE_X) )
	{
		GASSERT ( 0 && "" );
		return ;
	}


	{

		GLInventory::CELL_MAP_ITER iter = pItemList->begin();
		GLInventory::CELL_MAP_ITER iter_end = pItemList->end();

		for ( WORD y = 0; y < EM_INVENSIZE_Y_FORCE; y++ )
		{
			for ( WORD x = 0; x < EM_INVENSIZE_X; x++ )
			{
				SINVENITEM& ref_InvenItem = GetItem ( x, y );
				if ( ref_InvenItem.sItemCustom.sNativeID != NATIVEID_NULL() )
				{
					GLInventory::CELL_KEY foundkey ( x, y );
					GLInventory::CELL_MAP_ITER found = pItemList->find ( foundkey );
					if ( found == iter_end )
					{
						UnLoadItem ( x, y );
					}
				}
			}
		}
	}

	{
		GLInventory::CELL_MAP_ITER iter = pItemList->begin();
		GLInventory::CELL_MAP_ITER iter_end = pItemList->end();

		for ( ; iter!=iter_end; ++iter )
		{
			SINVENITEM* pInvenItem = (*iter).second;
			SINVENITEM& ref_InvenItemOld = GetItem ( pInvenItem->wPosX, pInvenItem->wPosY );
			if ( pInvenItem->sItemCustom != ref_InvenItemOld.sItemCustom )
			{
				LoadItem ( *pInvenItem );
			}	
		}
	}
}

void CInventoryPage::UpdateBLOCK ()
{
	const WORD& wAddINVENLINE = GLGaeaClient::GetInstance().GetCharacter()->GetOnINVENLINE();
	int nUSABLE_INVENLINE = EM_INVEN_DEF_SIZE_Y + wAddINVENLINE;

	if ( EM_INVENSIZE_Y_FORCE < nUSABLE_INVENLINE )
	{		
		GASSERT ( 0 && "CInventoryPage::UpdateBLOCK()" );

		nUSABLE_INVENLINE = EM_INVENSIZE_Y_FORCE;
	}
	
	//	?? ??
	for ( int i = 0; i < nUSABLE_INVENLINE; i++ )
		m_pItemSlotArray[i]->ResetBLOCK ();

	//	?? ???
	for ( int i = nUSABLE_INVENLINE; i < EM_INVENSIZE_Y; i++ )
		m_pItemSlotArray[i]->SetBLOCK ();
}

void CInventoryPage::SetOneViewSlot ( const int& nMAX_ONE_VIEW )
{
	m_nONE_VIEW_SLOT = nMAX_ONE_VIEW;
	m_nCurPos = -1;
	//m_pScrollBar->GetThumbFrame()->SetState ( EM_INVENSIZE_Y_FORCE, nMAX_ONE_VIEW );
}