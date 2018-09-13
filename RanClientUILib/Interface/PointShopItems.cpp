#include "StdAfx.h"
#include "PointShopItems.h"
#include "../EngineUILib/GUInterface/BasicButton.h"
#include "ItemImage.h"
#include "BasicTextBoxEx.h"
#include "BasicScrollBarEx.h"
#include "../EngineUILib/GUInterface/BasicScrollThumbFrame.h"
#include "../EngineLib/DxCommon/DxFontMan.h"
#include "BasicLineBox.h"
#include "GameTextControl.h"
#include "BasicTextButton.h"
#include "BasicLineBox.h"
#include "InnerInterface.h"
#include "UITextControl.h"
#include "BasicChat.h"
#include "../EngineUILib/GUInterface/UIKeyCheck.h"
#include "GLGaeaClient.h"
#include "GLItemMan.h"
#include "GLItemDef.h"
#include "GLItem.h"
#include "../RanClientLib/G-Logic/GLPointShopMan.h"
#include "GameTextControl.h"
#include "D3DFont.h"
#include "../EngineUIlib/GUInterface/BasicTextBox.h"
#include "../EngineUILib/GUInterface/BasicButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int CPointShopItems::nLIMIT_CHAR = 16;

CPointShopItems::CPointShopItems ()
	: m_nSelectIndex ( -1 )
	, m_bINIT( FALSE )
	, m_pListText(NULL)
	, m_pListScrollBar(NULL)
	, m_pMoneyTextBox( NULL )
	, m_pRateTextBox ( NULL )
{
	memset( m_pTicketItemImage, NULL, sizeof( CItemImage* ) * POINTSHOP_ITEMNUM );
	memset( m_pTicketItemNum, NULL, sizeof( CBasicTextBox* ) * POINTSHOP_ITEMNUM );
	memset( m_pTicketItemNumBackImage, NULL, sizeof( CUIControl* ) * POINTSHOP_ITEMNUM );
}

CPointShopItems::~CPointShopItems ()
{
}

HRESULT CPointShopItems::InitDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	GASSERT( pd3dDevice );

	HRESULT hr = S_OK;
	hr = CUIGroup::InitDeviceObjects ( pd3dDevice );
	if ( FAILED ( hr ) ) return hr;

	m_bINIT = TRUE;

	LoadPointShopList();

	return S_OK;
}
CBasicTextBox* CPointShopItems::CreateTextBox ( const char* szTextBox )
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, D3DFONT_SHADOW | D3DFONT_ASCII );

	CBasicTextBox* pTextBox = new CBasicTextBox;
	pTextBox->CreateSub ( this, szTextBox );
    pTextBox->SetFont ( pFont8 );	
	RegisterControl ( pTextBox );
	return pTextBox;
}
void CPointShopItems::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, D3DFONT_SHADOW );
	
	{
		CString strImage[POINTSHOP_CATEGORYNUM] =
		{
			"IMIX_BOT_ALL_ACTIVE",
			"IMIX_BOT_HEAD_ACTIVE",
			"IMIX_BOT_UPPER_ACTIVE",
			"IMIX_BOT_UNDER_ACTIVE",
			"IMIX_BOT_SHOES_ACTIVE",
			"IMIX_BOT_GLOVES_ACTIVE",
			"IMIX_BOT_WEAPON_ACTIVE",
			"IMIX_BOT_NECKLACE_ACTIVE",
			"IMIX_BOT_BRACELET_ACTIVE",
			"IMIX_BOT_RING_ACTIVE",
			"IMIX_BOT_ETC_ACTIVE",
			"IMIX_BOT_RANDB_ACTIVE",
			"IMIX_BOT_COS_ACTIVE",
		};

		CString strImage_f[POINTSHOP_CATEGORYNUM] =
		{
			"IMIX_BOT_ALL_OVER",
			"IMIX_BOT_HEAD_OVER",
			"IMIX_BOT_UPPER_OVER",
			"IMIX_BOT_UNDER_OVER",
			"IMIX_BOT_SHOES_OVER",
			"IMIX_BOT_GLOVES_OVER",
			"IMIX_BOT_WEAPON_OVER",
			"IMIX_BOT_NECKLACE_OVER",
			"IMIX_BOT_BRACELET_OVER",
			"IMIX_BOT_RING_OVER",
			"IMIX_BOT_ETC_OVER",
			"IMIX_BOT_RANDB_OVER",
			"IMIX_BOT_COS_OVER",
		};
		
		
		for ( int i = 0; i < POINTSHOP_CATEGORYNUM; ++i )
		{
			m_pCategoryButton[i] = new CBasicButton;
			m_pCategoryButton[i]->CreateSub ( this,strImage[i].GetString(), UI_FLAG_DEFAULT, POINTSHOP_CATEGORY_BUTTON0 + i);
			m_pCategoryButton[i]->CreateFlip ( strImage_f[i].GetString(), CBasicButton::RADIO_FLIP );
			m_pCategoryButton[i]->SetUseGlobalAction ( TRUE );
			m_pCategoryButton[i]->SetFlip ( FALSE );
			m_pCategoryButton[i]->SetVisibleSingle ( TRUE );
			RegisterControl ( m_pCategoryButton[i] );
		}

	}
	//	텍스트 박스
	{
		CBasicTextBoxEx* pTextBox = new CBasicTextBoxEx;
		pTextBox->CreateSub ( this, "ITEMMIX_LIST_TEXTBOX", UI_FLAG_DEFAULT, POINTSHOP_LIST_TEXTBOX );
		pTextBox->SetFont ( pFont9 );		
		pTextBox->SetLineInterval ( 3.0f );
		pTextBox->SetSensitive ( true );
		pTextBox->SetLimitLine ( 10000 );
		RegisterControl ( pTextBox );
		m_pListText = pTextBox;

		int nTotalLine = pTextBox->GetVisibleLine ();
	}
			CString str;
			str.Format("POINT_SHOP_ITEMS_SLOTR");

			m_pProductItemImage = new CItemImage;
			m_pProductItemImage->CreateSub( this, str, UI_FLAG_DEFAULT, POINT_SHOP_ITEMS_SLOTR );
			m_pProductItemImage->CreateSubControl();
			RegisterControl( m_pProductItemImage );

			m_pTicketItemNumBackImage[0] = new CBasicLineBox;
			m_pTicketItemNumBackImage[0]->CreateSub ( this, "BASIC_LINE_BOX_SKILL", UI_FLAG_XSIZE | UI_FLAG_YSIZE );	
			m_pTicketItemNumBackImage[0]->CreateBaseBoxSkill ( "POINT_SHOP_NUM_SLOT0_BACK");
			RegisterControl ( m_pTicketItemNumBackImage[0] );

			m_pTicketItemNumBackImage[1] = new CBasicLineBox;
			m_pTicketItemNumBackImage[1]->CreateSub ( this, "BASIC_LINE_BOX_SKILL", UI_FLAG_XSIZE | UI_FLAG_YSIZE );	
			m_pTicketItemNumBackImage[1]->CreateBaseBoxSkill ( "POINT_SHOP_NUM_SLOT1_BACK");
			RegisterControl ( m_pTicketItemNumBackImage[1] );

			m_pTicketItemNumBackImage[2] = new CBasicLineBox;
			m_pTicketItemNumBackImage[2]->CreateSub ( this, "BASIC_LINE_BOX_SKILL", UI_FLAG_XSIZE | UI_FLAG_YSIZE );	
			m_pTicketItemNumBackImage[2]->CreateBaseBoxSkill ( "POINT_SHOP_NUM_SLOT2_BACK");
			RegisterControl ( m_pTicketItemNumBackImage[2] );

			m_pTicketItemNumBackImage[3] = new CBasicLineBox;
			m_pTicketItemNumBackImage[3]->CreateSub ( this, "BASIC_LINE_BOX_SKILL", UI_FLAG_XSIZE | UI_FLAG_YSIZE );	
			m_pTicketItemNumBackImage[3]->CreateBaseBoxSkill ( "POINT_SHOP_NUM_SLOT3_BACK");
			RegisterControl ( m_pTicketItemNumBackImage[3] );
			
			m_pTicketItemNumBackImage[4] = new CBasicLineBox;
			m_pTicketItemNumBackImage[4]->CreateSub ( this, "BASIC_LINE_BOX_SKILL", UI_FLAG_XSIZE | UI_FLAG_YSIZE );	
			m_pTicketItemNumBackImage[4]->CreateBaseBoxSkill ( "POINT_SHOP_NUM_SLOT4_BACK");
			RegisterControl ( m_pTicketItemNumBackImage[4] );

		for( int i = 0; i < POINTSHOP_ITEMNUM; ++i )
		{

			CString strTemp;
			strTemp.Format("POINT_SHOP_ITEMS_SLOT%d", i );

			m_pTicketItemImage[i] = new CItemImage;
			m_pTicketItemImage[i]->CreateSub( this, strTemp, UI_FLAG_DEFAULT, POINT_SHOP_ITEMS_SLOT0+i );
			m_pTicketItemImage[i]->CreateSubControl();
			RegisterControl( m_pTicketItemImage[i] );

			strTemp.Format("POINT_SHOP_ITEMS_SLOT%d_AMOUNT", i );
			m_pTicketItemNum[i] = CreateTextBox ( strTemp );	
			m_pTicketItemNum[i]->SetTextAlign ( TEXT_ALIGN_RIGHT );		
		

		}

			

			
}


void CPointShopItems::LoadPointShopList ()
{
	if( !m_bINIT ) return; // InitDeviceObject() 호출 후에 실행 되어야 한다.

	//	친구 목록을 갱신한다.
	m_pListText->ClearText ();
	FlipBot ( 0 );
	GLPointShopMan::POINTSHOP_STRMAP& PointShopStrMap = GLPointShopMan::GetInstance().GetPointShop ();
	
	{
		GLPointShopMan::POINTSHOP_STRMAP_ITER iter = PointShopStrMap.begin ();
		GLPointShopMan::POINTSHOP_STRMAP_ITER iter_end = PointShopStrMap.end ();	
		for ( ; iter != iter_end; ++iter )
		{
			POINT_SHOP& sPointShop = (*iter).second;
			if ( sPointShop.sResultItem.sNID != NATIVEID_NULL() ) 
			LoadPointShopItems ( sPointShop );
		}
	}

	{
		GLPointShopMan::POINTSHOP_STRMAP_ITER iter = PointShopStrMap.begin ();
		GLPointShopMan::POINTSHOP_STRMAP_ITER iter_end = PointShopStrMap.end ();	
		for ( ; iter != iter_end; ++iter )
		{
			POINT_SHOP& sPointShop = (*iter).second;
			if ( sPointShop.sResultItem.sNID != NATIVEID_NULL() ) 
			LoadPointShopItems ( sPointShop );
		}
	}

	m_pListText->SetCurLine ( 0 );

	const int nTotal = m_pListText->GetCount ();
	const int nViewPerPage = m_pListText->GetVisibleLine ();	
	m_pListScrollBar->GetThumbFrame()->SetState ( nTotal, nViewPerPage );
	m_pListScrollBar->GetThumbFrame()->SetPercent ( 0.0f );
}

void CPointShopItems::LoadPointShopItems ( const POINT_SHOP& sPointShop )
{
	SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sPointShop.sResultItem.sNID );
	const CString& strName = pItemData->GetName();

	static CString strKeyword[] =
	{
		"POINT_SHOP_ICON",
		"POINT_SHOP_ICON",
		"POINT_SHOP_ICON_LOCK"
	};

	D3DCOLOR dwTextColor;
	int nIcon;

	switch ( pItemData->sBasicOp.emLevel )
	{
		case LEVEL_NORMAL:			{	dwTextColor = NS_UITEXTCOLOR::NORMAL;		}	break;
		case LEVEL_RARE:			{	dwTextColor = NS_UITEXTCOLOR::RARE;		}	break;
		case LEVEL_HEROIC:			{	dwTextColor = NS_UITEXTCOLOR::HEROIC;	}	break;
		case LEVEL_ANCIENT:			{	dwTextColor = NS_UITEXTCOLOR::ANCIENT;		}	break;
		case LEVEL_LEGENDARY:		{	dwTextColor = NS_UITEXTCOLOR::LEGENDARY;		}	break;
		case LEVEL_VOTE_ITEM:		{	dwTextColor = NS_UITEXTCOLOR::VOTEITEM;	}	break;
		case LEVEL_PREMIUM_ITEM:	{	dwTextColor = NS_UITEXTCOLOR::PREMIUMITEM;	}	break;
		case LEVEL_HIGH_CLASS:		{	dwTextColor = NS_UITEXTCOLOR::HIGHCLASS; } break;
	}
	nIcon = ITEM;
	
	CString strTemp;
	CString strTemp2;
	
	strTemp.Format( "%s", strName );

	int nIndex = m_pListText->AddText ( strTemp, dwTextColor );	
	m_pListText->SetTextImage ( nIndex, strKeyword[nIcon] );	
	m_pListText->SetTextData ( nIndex, sPointShop.dwKey );
}

BOOL CPointShopItems::GetPointShop ( const DWORD nItemID )
{
	GLPointShopMan::POINTSHOP_STRMAP& PointShopKeyMap = GLPointShopMan::GetInstance().GetPointShop ();
	
	GLPointShopMan::POINTSHOP_STRMAP_ITER iter = PointShopKeyMap.begin();
	GLPointShopMan::POINTSHOP_STRMAP_ITER iter_end = PointShopKeyMap.end ();	
	for ( ; iter != iter_end; ++iter )
	{
		POINT_SHOP& sPointShop = (*iter).second;
			
		if ( sPointShop.dwKey == nItemID ) 
		{
			m_nItemID = nItemID;
			
					
			return TRUE;
		}
		
	}

	return FALSE;
}

void CPointShopItems::TranslateUIMessage ( UIGUID cID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( cID, dwMsg );

	switch ( cID )
	{
	case POINTSHOP_CATEGORY_BUTTON0:
		{
			if( CHECK_MOUSE_IN( dwMsg ) )
			{
				int nIndex = cID - POINTSHOP_CATEGORY_BUTTON0;
				if ( nIndex < 0 || nIndex >=  POINTSHOP_CATEGORY_BUTTON0 )	return;
				
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO( ID2GAMEWORD( "PRODUCT_TYPE_STRING" , 0 ), NS_UITEXTCOLOR::WHITE  );				
				if ( UIMSG_LB_UP & dwMsg )
				{
					m_pListText->ClearText ();
					CString strNum;
					FlipBot ( nIndex );
					GLPointShopMan::POINTSHOP_STRMAP& PointShopKeyMap = GLPointShopMan::GetInstance().GetPointShop();
					GLPointShopMan::POINTSHOP_STRMAP_ITER iter = PointShopKeyMap.begin();
					GLPointShopMan::POINTSHOP_STRMAP_ITER iter_end = PointShopKeyMap.end ();
					
					for ( ; iter != iter_end; ++iter )
					{
						POINT_SHOP& sPointShop = (*iter).second;
						
							SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sPointShop.sResultItem.sNID );
							if ( pItemData )
							{
								LoadPointShopItems ( sPointShop );
							}
					}
					m_pListText->SetCurLine ( 0 );
					const int nTotal = m_pListText->GetCount ();
					const int nViewPerPage = m_pListText->GetVisibleLine ();	
					m_pListScrollBar->GetThumbFrame()->SetState ( nTotal, nViewPerPage );
					m_pListScrollBar->GetThumbFrame()->SetPercent ( 0.0f );
				}
			}
		}
	case POINTSHOP_CATEGORY_BUTTON1:
	case POINTSHOP_CATEGORY_BUTTON2:
	case POINTSHOP_CATEGORY_BUTTON3:
	case POINTSHOP_CATEGORY_BUTTON4:
	case POINTSHOP_CATEGORY_BUTTON5:
	case POINTSHOP_CATEGORY_BUTTON6:
	case POINTSHOP_CATEGORY_BUTTON7:
	case POINTSHOP_CATEGORY_BUTTON8:
	case POINTSHOP_CATEGORY_BUTTON9:
	case POINTSHOP_CATEGORY_BUTTON10:
	case POINTSHOP_CATEGORY_BUTTON11:
	case POINTSHOP_CATEGORY_BUTTON12:
		{
			
			if( CHECK_MOUSE_IN( dwMsg ) )
			{
				int nIndex = cID - POINTSHOP_CATEGORY_BUTTON1 + 1;
				if ( nIndex < 1 || nIndex >=  POINTSHOP_CATEGORY_BUTTON12 )	return;
				CInnerInterface::GetInstance().SHOW_COMMON_LINEINFO( ID2GAMEWORD( "PRODUCT_TYPE_STRING" , nIndex ), NS_UITEXTCOLOR::WHITE  );				
				
				if ( UIMSG_LB_UP & dwMsg )
				{
					FlipBot ( nIndex );
					m_pListText->ClearText ();
					m_nSelectIndex = nIndex;
					CString strNum;
					GLPointShopMan::POINTSHOP_STRMAP& PointShopKeyMap = GLPointShopMan::GetInstance().GetPointShop();
					GLPointShopMan::POINTSHOP_STRMAP_ITER iter = PointShopKeyMap.begin();
					GLPointShopMan::POINTSHOP_STRMAP_ITER iter_end = PointShopKeyMap.end ();
					
					for ( ; iter != iter_end; ++iter )
					{
						POINT_SHOP& sPointShop = (*iter).second;
						if ( sPointShop.dwCategory == nIndex ) 
						{
							m_nCategory = nIndex;
							SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sPointShop.sResultItem.sNID );
							if ( pItemData )
							{
								LoadPointShopItems ( sPointShop );
							}
						}
					}
					m_pListText->SetCurLine ( 0 );
					const int nTotal = m_pListText->GetCount ();
					const int nViewPerPage = m_pListText->GetVisibleLine ();	
					m_pListScrollBar->GetThumbFrame()->SetState ( nTotal, nViewPerPage );
					m_pListScrollBar->GetThumbFrame()->SetPercent ( 0.0f );
				}
			}
		}
		break;
	case POINT_SHOP_ITEMS_SLOTR:
		{
			if( CHECK_MOUSE_IN( dwMsg ) )
			{
			
				SITEMCUSTOM sItem1 = sShopData.sResult;
				if( sItem1.sNativeID != NATIVEID_NULL() ) 
				{
					CInnerInterface::GetInstance().SHOW_ITEM_INFO ( sItem1 , FALSE, FALSE, FALSE, USHRT_MAX, USHRT_MAX );
				}
			}
		}
		break;
	case POINT_SHOP_ITEMS_SLOT0:
	case POINT_SHOP_ITEMS_SLOT1:
	case POINT_SHOP_ITEMS_SLOT2:
	case POINT_SHOP_ITEMS_SLOT3:
	case POINT_SHOP_ITEMS_SLOT4:
		{
			if( CHECK_MOUSE_IN( dwMsg ) )
			{
				int nIndex = cID - POINT_SHOP_ITEMS_SLOT0;
				
				if ( nIndex < 0 || nIndex >= SSHOPDATA::STICKET_SLOTS )	return;

					SITEMCUSTOM sItem1 = sShopData.sItems[nIndex];
					if( sItem1.sNativeID != NATIVEID_NULL() ) 
					{
						CInnerInterface::GetInstance().SHOW_ITEM_INFO ( sItem1 , FALSE, FALSE, FALSE, USHRT_MAX, USHRT_MAX );
					}
			}
		}
		break;
	case POINTSHOP_LIST_TEXTBOX:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				const int nIndex = m_pListText->GetSelectPos ();
				if ( nIndex < 0 || m_pListText->GetCount () <= nIndex ) return ;

				m_pListText->SetUseOverColor ( TRUE );
				m_pListText->SetOverColor ( nIndex, NS_UITEXTCOLOR::DARKGRAY );

				if( UIMSG_LB_UP & dwMsg )
				{
					m_nSelectIndex = nIndex;
					DWORD nItemID = m_pListText->GetTextData ( m_nSelectIndex );
					if( !GetPointShop( nItemID ) ) m_strSelectName.Empty();
					
				}
				
				if ( UIMSG_LB_UP & dwMsg )
				{
					m_nSelectIndex = nIndex;
					CString strNum;
					DWORD nItemID = m_pListText->GetTextData ( m_nSelectIndex );
					GLPointShopMan::POINTSHOP_STRMAP& PointShopKeyMap = GLPointShopMan::GetInstance().GetPointShop();
					GLPointShopMan::POINTSHOP_STRMAP_ITER iter = PointShopKeyMap.begin();
					GLPointShopMan::POINTSHOP_STRMAP_ITER iter_end = PointShopKeyMap.end ();
					for ( ; iter != iter_end; ++iter )
					{
						POINT_SHOP& sPointShop = (*iter).second;
						if ( sPointShop.dwKey == nItemID ) 
						{
							m_nItemID = nItemID;
							SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sPointShop.sResultItem.sNID );
							if ( pItemData )
							{
								m_pProductItemImage->SetItem( pItemData->sBasicOp.sICONID, pItemData->GetInventoryFile() );
								sShopData.sResult = sPointShop.sResultItem.sNID;
								SetMoney ( sPointShop.dwPrice );
								SetRate ( sPointShop.dwRate );
							}
							else
							{
								m_pProductItemImage->ResetItem();
								sShopData.sResult = NATIVEID_NULL();
							}
							for ( int i = 0; i < POINTSHOP_ITEMNUM; ++i )
							{
								SITEM* pItem	 = GLItemMan::GetInstance().GetItem( sPointShop.sTicketItem[i].sNID );
								if( pItem )
								{
									strNum.Format( "%d", sPointShop.sTicketItem[i].nNum );
									m_pTicketItemImage[i]->SetItem( pItem->sBasicOp.sICONID, pItem->GetInventoryFile() );
									m_pTicketItemNum[i]->SetText( strNum, NS_UITEXTCOLOR::WHITE );
									sShopData.sItems[i] = sPointShop.sTicketItem[i].sNID;
									
								}
								else
								{
									m_pTicketItemImage[i]->ResetItem();
									m_pTicketItemNum[i]->ClearText();
									sShopData.sItems[i] = NATIVEID_NULL();
								}
							}
						}
					}
				}	
			}
		}
		break;



	}
}


void CPointShopItems::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	m_pListText->SetUseOverColor ( FALSE );
	if ( 0 <= m_nSelectIndex && m_nSelectIndex < m_pListText->GetCount () )
		m_pListText->SetUseTextColor ( m_nSelectIndex, FALSE );

	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	DWORD nItemID = m_pListText->GetTextData ( m_nSelectIndex );
	GLPointShopMan::POINTSHOP_STRMAP& PointShopKeyMap = GLPointShopMan::GetInstance().GetPointShop();
	GLPointShopMan::POINTSHOP_STRMAP_ITER iter = PointShopKeyMap.begin();
	GLPointShopMan::POINTSHOP_STRMAP_ITER iter_end = PointShopKeyMap.end ();
	CString strNum;
	for ( ; iter != iter_end; ++iter )
	{
		POINT_SHOP& sPointShop = (*iter).second;
		if ( sPointShop.dwKey == nItemID ) 
		{
			m_nItemID = nItemID;
			SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sPointShop.sResultItem.sNID );
			if ( pItemData )
			{
				m_pProductItemImage->SetItem( pItemData->sBasicOp.sICONID, pItemData->GetInventoryFile() );
				sShopData.sResult = sPointShop.sResultItem.sNID;
			}
			else
			{
				m_pProductItemImage->ResetItem();
				sShopData.sResult = NATIVEID_NULL();
			}
			for ( int i = 0; i < POINTSHOP_ITEMNUM; ++i )
			{
				SITEM* pItem = GLItemMan::GetInstance().GetItem( sPointShop.sTicketItem[i].sNID );
				if( pItem )
				{
					strNum.Format( "%d", sPointShop.sTicketItem[i].nNum );
					m_pTicketItemImage[i]->SetItem( pItem->sBasicOp.sICONID, pItem->GetInventoryFile() );
					m_pTicketItemNum[i]->SetText( strNum, NS_UITEXTCOLOR::WHITE );
					sShopData.sItems[i] = sPointShop.sTicketItem[i].sNID;
					
				}
				else
				{
					m_pTicketItemImage[i]->ResetItem();
					m_pTicketItemNum[i]->ClearText();
					sShopData.sItems[i] = NATIVEID_NULL();
				}
			}
		}
	}

	if ( 0 <= m_nSelectIndex && m_nSelectIndex < m_pListText->GetCount () )
	{
		m_pListText->SetUseTextColor ( m_nSelectIndex, TRUE );
		m_pListText->SetTextColor ( m_nSelectIndex, NS_UITEXTCOLOR::DARKGRAY );
	}

	{
		CBasicScrollThumbFrame* const pThumbFrame = m_pListScrollBar->GetThumbFrame ();

		const int nTotalLine = m_pListText->GetTotalLine ();
		const int nLinePerOneView = m_pListText->GetVisibleLine ();
		CDebugSet::ToView ( 1, 10, "보이는 라인 %d", nLinePerOneView );
		pThumbFrame->SetState ( nTotalLine, nLinePerOneView );
		if ( nLinePerOneView < nTotalLine )
		{
			const int nMovableLine = nTotalLine - nLinePerOneView;
			float fPercent = pThumbFrame->GetPercent ();
			int nPos = (int)floor(fPercent * nMovableLine);
			m_pListText->SetCurLine ( nPos );			
		}
	}
}

void CPointShopItems::SetVisibleSingle ( BOOL bVisible )
{
	CUIGroup::SetVisibleSingle ( bVisible );

	if( bVisible )
	{
		LoadPointShopList ();

		m_nSelectIndex = -1;

		
	}
}

void CPointShopItems::FlipBot ( int nCategory )
{
	for ( int i = 0; i < POINTSHOP_CATEGORYNUM; ++i )
	{
		m_pCategoryButton[i]->SetFlip( FALSE );
	}
	m_pCategoryButton[nCategory]->SetFlip( TRUE );					
}
void CPointShopItems::SetMoney ( LONGLONG Money )
{
	//	돈자리수
	CString strTemp = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( Money, 3, "," );
	m_pMoneyTextBox->SetOneLineText ( strTemp );
}
void CPointShopItems::SetRate ( DWORD Rate )
{
	CString strTemp;
	strTemp.Format ("Success Rate : %d", Rate);
	strTemp += _T('%');
	m_pRateTextBox->SetOneLineText ( strTemp );
}
BOOL CPointShopItems::IsSelectedIndex()
{
	return ( 0 <= m_nSelectIndex && m_nSelectIndex < m_pListText->GetCount() );
}

/*const DWORD CPointShopItems::GetFriendID()
{
	if ( !IsSelectedIndex() )
		return NATIVEID_NULL().dwID;

	return m_pListText->GetTextData ( m_nSelectIndex );
}*/