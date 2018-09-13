#include "StdAfx.h"
#include "PointShopWindow.h"
#include "PointShopTicket.h"
#include "PointShopWindowMenu.h"
#include "ItemImage.h"
#include "BasicTextButton.h"
#include "GLItemMan.h"
#include "GLPointShopMan.h"
#include "GameTextControl.h"
#include "../EngineUIlib/GUInterface/BasicTextBox.h"
#include "InnerInterface.h"
#include "GLGaeaClient.h"
#include "ItemMove.h"
#include "../RanClientUILib/Interface/ModalWindow.h"
#include "../RanClientUILib/Interface/ModalCallerID.h"
#include "UITextControl.h"
#include "../EngineUILib/GUInterface/BasicAnimationBox.h"
#include "../EngineUiLib/GUInterface/BasicButton.h"
#include "../EngineUiLib/GUInterface/BasicButtonText.h"
#include "../EngineUiLib/GUInterface/BasicTextBox.h"
#include "../EngineUILib/GUInterface/BasicScrollThumbFrame.h"
#include "BasicLineBox.h"
#include "GameTextControl.h"
#include "UITextControl.h"
#include "InnerInterface.h"
#include "InnerInterfaceGuid.h"
#include "BasicTextBoxEx.h"
#include "BasicScrollBarEx.h"
#include "InventoryWindow.h"
#include "InventoryPage.h"
#include "../EngineLib/DxCommon/d3dfont.h"
#include "../RanClientLib/G-Logic/Emoticon.h"
#include "../RanClientLib/G-Logic/GLItemMan.h"
#include "../RanClientLib/G-Logic/GLGaeaClient.h"
#include "../RanClientLib/G-Logic/GLCharData.h"
#include "../EngineUIlib/GUInterface/BasicProgressBar.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const float fRusltTime = 10.0f;

//--------------------------------------------------------------------
// CItemGarbage
//--------------------------------------------------------------------
CPointShopWindow::CPointShopWindow()	// ???
	: m_pGenerateBtn( NULL )	
	, m_pMenu( NULL )
	, m_pTypeText( NULL )
	, m_pPremiumListText ( NULL )
	, m_pTitleText( NULL )
	, m_pRateText( NULL )
	, m_pCostText( NULL )
	, m_pAmountText( NULL )
	, m_pPremiumPointstText( NULL )
	, m_pResourceText( NULL )
	, m_pListText( NULL )
	, m_pListScrollBar( NULL )
	, m_wType( 0 )
	, m_nSelectIndex( 0 )
	, m_bRefresh ( false )
	, m_bOK(false)
	, m_bIsOK(false)
	, m_dwNpcID(UINT_MAX)
	, m_bResult(false)
	, m_fTime( 0.0f )
	, m_pNotes( NULL )
{
	SecureZeroMemory( m_pTicketItemImageSlot, sizeof(m_pTicketItemImageSlot) );
	SecureZeroMemory( m_pTicketItemImage, sizeof(m_pTicketItemImage) );

}
CBasicTextBox* CPointShopWindow::CreateStaticControl ( char* szControlKeyword, CD3DFontPar* pFont, int nAlign )
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub ( this, szControlKeyword );
	pStaticText->SetFont ( pFont );
	pStaticText->SetTextAlign ( nAlign );	
	RegisterControl ( pStaticText );

	return pStaticText;
}
CPointShopWindow::~CPointShopWindow()
{
}

void CPointShopWindow::Reset()
{
	m_bOK = false;
	m_bIsOK = false;
	m_bResult = false;
	m_fTime = 0.0f;
}
CBasicTextBox* CPointShopWindow::CreateTextBox ( const char* szTextBox )
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, D3DFONT_SHADOW | D3DFONT_ASCII );

	CBasicTextBox* pTextBox = new CBasicTextBox;
	pTextBox->CreateSub ( this, szTextBox );
    pTextBox->SetFont ( pFont8 );	
	RegisterControl ( pTextBox );
	return pTextBox;
}
CBasicButtonText* CPointShopWindow::CreateButtonBlue ( char* szButton, char* szTextBox, CD3DFontPar* pFont, int nAlign, UIGUID ControlID, CString strText )
{
	CBasicButtonText* pButton = new CBasicButtonText;
	pButton->CreateRanButtonBlue ( this, szButton, ControlID );
	pButton->CreateTextBox ( szTextBox, pFont, nAlign );
	pButton->SetOneLineText( strText.GetString() );
	RegisterControl ( pButton );

	return pButton;
}

CPointShopTicket*	CPointShopWindow::CreateItemImageTicket( const char* szControl, UIGUID ControlID )
{
	CPointShopTicket* pItemImage = new CPointShopTicket;
	pItemImage->CreateSub ( this, szControl, UI_FLAG_DEFAULT, ControlID );
	pItemImage->CreateSubControl ();
	RegisterControl ( pItemImage );
	return pItemImage;
}

VOID CPointShopWindow::CreateSubControl()
{
	const int nAlignCenterBoth = TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y;
	const int nAlignLeftCenter = TEXT_ALIGN_CENTER_Y | TEXT_ALIGN_LEFT;
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );


	{
	CBasicProgressBar* pHP = new CBasicProgressBar;
	pHP->CreateSub ( this, "RESULT_PROGRESS",UI_FLAG_XSIZE | UI_FLAG_YSIZE  );
	pHP->CreateOverImage ( "RESULT_PROGRESS_OVER" );	
	pHP->SetAlignFlag ( UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	RegisterControl ( pHP );
	m_pResult = pHP;
	m_pResult->SetVisibleSingle ( FALSE );

	m_pBodyLeft		= CreateControl( "RESULT_PROGRESS_LEFT_BODY" );
	m_pBodyRight	= CreateControl( "RESULT_PROGRESS_RIGHT_BODY" );

	}

	{
	CBasicTextBox* pTextBox = new CBasicTextBox;
	pTextBox->CreateSub ( this, "REBIRTH_DIALOGUE_TEXTBOX_EX", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
    pTextBox->SetFont ( pFont9 );
	pTextBox->SetTextAlign ( TEXT_ALIGN_CENTER_X );
	pTextBox->SetPartInterval ( 10.0f );
	RegisterControl ( pTextBox );
	m_pTextBox = pTextBox;

	}

	//Type Image linebox
	{
		CBasicLineBox* pBasicLineBoxType = new CBasicLineBox;
		pBasicLineBoxType->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBoxType->CreateBaseBoxQuestList( "CNDEV_POINTSHOP_LINEBOX_02" );
		RegisterControl ( pBasicLineBoxType );
	}

	//Top-mid linebox
	{
		CBasicLineBox* pBasicLineBoxTitle = new CBasicLineBox;
		pBasicLineBoxTitle->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBoxTitle->CreateBaseBoxQuestList( "CNDEV_POINTSHOP_LINEBOX_03" );
		RegisterControl ( pBasicLineBoxTitle );
	}

	{
		CBasicLineBox* pBasicLineBoxItemList = new CBasicLineBox;
		pBasicLineBoxItemList->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBoxItemList->CreateBaseBoxQuestList( "CNDEV_POINTSHOP_LINEBOX_04" );
		RegisterControl ( pBasicLineBoxItemList );
	}

	{
		CBasicLineBox* pBasicLineBoxBody = new CBasicLineBox;
		pBasicLineBoxBody->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBoxBody->CreateBaseBoxQuestList( "CNDEV_POINTSHOP_LINEBOX_05" );
		RegisterControl ( pBasicLineBoxBody );

		//Inner-Body

		{
			CBasicLineBox* pBasicLineBoxCondition = new CBasicLineBox;
			pBasicLineBoxCondition->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
			pBasicLineBoxCondition->CreateBaseBoxQuestList( "CNDEV_POINTSHOP_LINEBOX_06" );
			RegisterControl ( pBasicLineBoxCondition );
		}

		{
			CBasicLineBox* pBasicLineBoxCost = new CBasicLineBox;
			pBasicLineBoxCost->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
			pBasicLineBoxCost->CreateBaseBoxQuestList( "CNDEV_POINTSHOP_LINEBOX_07" );
			RegisterControl ( pBasicLineBoxCost );
		}

		{
			CBasicLineBox* pBasicLineBoxResult = new CBasicLineBox;
			pBasicLineBoxResult->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
			pBasicLineBoxResult->CreateBaseBoxQuestList( "CNDEV_POINTSHOP_LINEBOX_08" );
			RegisterControl ( pBasicLineBoxResult );
		}

	}

	{
		CPointShopWindowMenu* pMenu = new CPointShopWindowMenu;
		pMenu->CreateSub ( this, "CNDEV_POINTSHOP_MENU" );
		pMenu->CreateSubControl();
		pMenu->SetVisibleSingle(TRUE);
		RegisterControl ( pMenu );
		m_pMenu = pMenu;
	}
	//notes
	{
		m_pNotes = new CBasicTextBox;
		m_pNotes->CreateSub ( this, "CNDEV_POINTSHOP_NOTES_TEXT" );
		m_pNotes->SetFont ( pFont9 );
		m_pNotes->SetTextAlign ( TEXT_ALIGN_LEFT );
		m_pNotes->AddTextNoSplit( "<NOTES>", NS_UITEXTCOLOR::WHITE );
		m_pNotes->AddTextNoSplit( "1. Do not buy the item if Ticket is gone.", NS_UITEXTCOLOR::WHITE );
		m_pNotes->AddTextNoSplit( "2. Ticket No. increases when you pile the Ticket.", NS_UITEXTCOLOR::WHITE );
		m_pNotes->AddTextNoSplit( "3. Purchased Item goes directly to your Inventory.", NS_UITEXTCOLOR::WHITE );
		m_pNotes->AddTextNoSplit( "4. Accidental purchased of items are not refundable.", NS_UITEXTCOLOR::WHITE );
		RegisterControl ( m_pNotes );
		m_pNotes->SetVisibleSingle( TRUE );
	}


	//Type Title
	{
		m_pTitleText = CreateStaticControl ( "CNDEV_POINTSHOP_TITLE_TEXT", pFont9, nAlignCenterBoth );
		m_pPremiumListText	= CreateStaticControl ( "CNDEV_POINTSHOP_PREMIUMLIST_TEXT", pFont9, nAlignCenterBoth );
		m_pCostText = CreateStaticControl ( "CNDEV_POINTSHOP_COST_TEXT", pFont9, nAlignLeftCenter );
		m_pAmountText = CreateStaticControl ( "CNDEV_POINTSHOP_AMOUNT_TEXT", pFont9, nAlignLeftCenter );
		m_pPremiumPointstText = CreateStaticControl ( "CNDEV_POINTSHOP_PREMIUMPOINTS_TEXT", pFont9, nAlignLeftCenter );

	}

	{
		CBasicButton* pButton = NULL;
		pButton = CreateFlipButton ( "CNDEV_POINTSHOP_PPOINTS_BUTTON", "CNDEV_POINTSHOP_PPOINTS_BUTTON_F", CNDEV_POINTSHOP_PPOINTS_BUTTON, CBasicButton::CLICK_FLIP );
		pButton->CreateMouseOver ( "CNDEV_POINTSHOP_PPOINTS_BUTTON_F" );
		pButton->SetAlignFlag ( UI_FLAG_DEFAULT );
		pButton->SetUseDynamic ( TRUE );
	}

	{
		CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );

		CString strName;

		strName.Format( "Premium Items" );
		if ( m_pPremiumListText ) m_pPremiumListText->SetOneLineText( strName, NS_UITEXTCOLOR::WHITE );

		CBasicTextBoxEx* pTextBox = new CBasicTextBoxEx;
		pTextBox->CreateSub ( this, "CNDEV_POINTSHOP_ITEM_LIST_TEXTBOX", UI_FLAG_DEFAULT, CNDEV_POINTSHOP_ITEM_LIST_TEXTBOX );
		pTextBox->SetFont ( pFont );		
		pTextBox->SetLineInterval ( 3.0f );
		pTextBox->SetSensitive ( true );
		pTextBox->SetLimitLine ( 10000 );
		RegisterControl ( pTextBox );
		m_pListText = pTextBox;
		
		int nTotalLine = pTextBox->GetVisibleLine ();

		CBasicScrollBarEx* pScrollBar = new CBasicScrollBarEx;
		pScrollBar->CreateSub ( this, "BASIC_SCROLLBAR", UI_FLAG_RIGHT | UI_FLAG_YSIZE, CNDEV_POINTSHOP_ITEM_LIST_SCROLLBAR );
		pScrollBar->CreateBaseScrollBar ( "CNDEV_POINTSHOP_ITEM_LIST_SCROLLBAR" );
		pScrollBar->GetThumbFrame()->SetState ( 1, nTotalLine );
		RegisterControl ( pScrollBar );
		m_pListScrollBar = pScrollBar;
	}

	//Result image
	{
		CBasicLineBox* pBasicLineBoxBack = new CBasicLineBox;
		pBasicLineBoxBack->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBoxBack->CreateBaseBoxQuestList( "CNDEV_POINTSHOP_PRODUCT_IMAGE_BACK" );
		RegisterControl ( pBasicLineBoxBack );

		CItemImage* pItemImage = new CItemImage;
		pItemImage->CreateSub ( this, "CNDEV_POINTSHOP_PRODUCT_IMAGE", UI_FLAG_DEFAULT, PRODUCT_IMAGE );
		pItemImage->CreateSubControl ();
		pItemImage->SetUseRender ( TRUE );
		pItemImage->SetVisibleSingle ( FALSE );
		RegisterControl ( pItemImage );
		m_pItemResultImage = pItemImage;
	}
	//recipe slot - 
	CString strRecipeSlot[TICKET_SLOT] = 
	{
		"CNDEV_POINTSHOP_TICKET_IMAGE00",
	};

	CString strRecipeSlotOver[TICKET_SLOT] = 
	{
		"CNDEV_POINTSHOP_TICKET_IMAGE_BACK00",
	};
	for ( int i = 0; i < TICKET_SLOT; i++ )
	{
		m_pTicketItemImage[i] = CreateItemImageTicket( strRecipeSlot[i].GetString(), TICKET_IMAGE00 + i );
		m_pTicketItemImageSlot[i] = CreateControl( strRecipeSlotOver[i].GetString() );
		m_pTicketItemImageSlot[i]->SetVisibleSingle( FALSE );
	}
	for(int i = 0; i < 1; i++ )
	{	
		CString strKeyword;
		strKeyword.Format( "CNDEV_POINTSHOP_TICKET_TEXT_BACK0%d", i );

		CBasicLineBox* pBasicLineBoxBack2 = new CBasicLineBox;
		pBasicLineBoxBack2->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBoxBack2->CreateBaseBoxQuestList( (char*)strKeyword.GetString() );
		RegisterControl ( pBasicLineBoxBack2 );

		strKeyword.Format( "CNDEV_POINTSHOP_TICKET_TEXT0%d", i );
		m_pRecipeNumText[i] = CreateStaticControl ( (char*)strKeyword.GetString(), pFont9, nAlignCenterBoth );

		strKeyword.Format( "CNDEV_POINTSHOP_TICKET_TEXT0%d", i );
		m_pRecipeNumText2[i] = CreateStaticControl ( (char*)strKeyword.GetString(), pFont9, nAlignCenterBoth );
	}

//	m_pGenerateButton = CreateButtonBlue ( "ITEM_COMPOUND_GENERATE_BUTTON", "ITEM_COMPOUND_GENERATE_BUTTON_TEXT_POS", pFont9, nAlignCenterBoth, ITEM_COMPOUND_GENERATE_BUTTON, "Generate" );		
	m_pGenerateButton = CreateTextButton ( "CNDEV_POINTSHOP_PURCHASE_BUTTON", CNDEV_POINTSHOP_PURCHASE_BUTTON, "Purchase" );
	m_pTopUpButton = CreateTextButton ( "CNDEV_POINTSHOP_TOPUP_BUTTON", CNDEV_POINTSHOP_TOPUP_BUTTON, "Top-Up" );
	
	LoadItemList( 0 );
}

CBasicTextButton*	 CPointShopWindow::CreateTextButton ( char* szButton, UIGUID ControlID, char* szText )
{
	CBasicTextButton* pTextButton = new CBasicTextButton;
	pTextButton->CreateSub ( this, "BASIC_TEXT_BUTTON24", UI_FLAG_XSIZE, ControlID );
	pTextButton->CreateBaseButton ( szButton, 
									CBasicTextButton::SIZE24, 
									CBasicButton::CLICK_FLIP, 
									szText,
									_DEFAULT_FONT_SHADOW_EX_FLAG );
	pTextButton->SetFlip ( TRUE );
	RegisterControl ( pTextButton );

	return pTextButton;
}

void CPointShopWindow::SetVisibleSingle( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle( bVisible );

	Reset();
}

VOID CPointShopWindow::Update( INT x, INT y, BYTE LB, BYTE MB, BYTE RB, INT nScroll, FLOAT fElapsedTime, BOOL bFirstControl )
{
	m_pListText->SetUseOverColor ( FALSE );
	if ( 0 <= m_nSelectIndex && m_nSelectIndex < m_pListText->GetCount () )
		m_pListText->SetUseTextColor ( m_nSelectIndex, FALSE );

	CUIWindowEx::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if ( 0 <= m_nSelectIndex && m_nSelectIndex < m_pListText->GetCount () )
	{
		m_pListText->SetUseTextColor ( m_nSelectIndex, TRUE );
		m_pListText->SetTextColor ( m_nSelectIndex, NS_UITEXTCOLOR::PARTY );
	}

	{
		CBasicScrollThumbFrame* const pThumbFrame = m_pListScrollBar->GetThumbFrame ();

		const int nTotalLine = m_pListText->GetTotalLine ();
		const int nLinePerOneView = m_pListText->GetVisibleLine ();
		CDebugSet::ToView ( 1, 10, "InitFucker %d", nLinePerOneView );
		pThumbFrame->SetState ( nTotalLine, nLinePerOneView );
		if ( nLinePerOneView < nTotalLine )
		{
			const int nMovableLine = nTotalLine - nLinePerOneView;
			float fPercent = pThumbFrame->GetPercent ();
			int nPos = (int)floor(fPercent * nMovableLine);
			m_pListText->SetCurLine ( nPos );			
		}
	}

	WORD wType = m_pMenu->GetType();

	if ( m_pTypeText )
		m_pTypeText->SetOneLineText ( ID2GAMEWORD("ITEM_COMPOUND_TYPE_TEXT_STATIC", wType ), NS_UITEXTCOLOR::WHITE );
	
	if ( wType != m_wType )
	{
		LoadItemList( wType );
		m_wType = wType;
		m_nSelectIndex = -1;
		m_bRefresh = true;
	}

	if ( m_nSelectIndex != -1 )
	{
		if ( m_bRefresh )
		{
			SNATIVEID sItemID;
			float fRate;
			LONGLONG lnPrice;
			int nAmount;

			SNATIVEID sItemRecipeID[1];
			int nRecipeAmount[1];

			int nID = m_pListText->GetTextData ( m_nSelectIndex );
			std::vector<GLSHOP>::size_type i = 0, j = GLCONST_CHAR::vecSHOP.size();
			for( ; i < j; ++i )
			{
				GLSHOP sShop = GLCONST_CHAR::vecSHOP[i];
				if ( sShop.dwID == nID )
				{
					sItemID = sShop.sNativeID;
					fRate = sShop.fRate;
					lnPrice = sShop.lnPrice;
					nAmount = sShop.wNum;
					m_sNativeID = sItemID;

					for(int i = 0; i < 1; i++ )
					{
						GLSHOP_TICKET sRecipe = sShop.glTicket[i];
						sItemRecipeID[i] = sRecipe.sNativeID;
						nRecipeAmount[i] = sRecipe.wNum;
						m_sTicketNativeID[i] = sRecipe.sNativeID;
                    	m_pTicketItemImage[i]->ResetItem();
		            	m_pRecipeNumText[i]->ClearText();
					}

					break;
				}
			}


			CString strName;
			SITEM* pItem = GLItemMan::GetInstance().GetItem( sItemID );
			if ( !pItem )
			{
				strName = "";
				m_pItemResultImage->ResetItem();
				m_pItemResultImage->SetVisibleSingle( FALSE );
			}
			else
			{
				if ( m_pItemResultImage )
				{
					m_pItemResultImage->SetItem( pItem->sBasicOp.sICONID, pItem->GetInventoryFile() );
					m_pItemResultImage->SetVisibleSingle( TRUE );
				}
			}

			strName.Format( "Item Name: %s", pItem->GetName() );
			if ( m_pTitleText ) m_pTitleText->SetOneLineText( strName, NS_UITEXTCOLOR::WHITE );

			CString strTemp = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( lnPrice, 3, "," );
			strName.Format( "Price: %s Points", strTemp );
			if ( m_pCostText ) m_pCostText->SetOneLineText ( strName, NS_UITEXTCOLOR::ORANGE );

			strName.Format( "Quantity: %d", nAmount );
			if ( m_pAmountText ) m_pAmountText->SetOneLineText( strName, NS_UITEXTCOLOR::WHITE );



	for(int i = 0; i < 1; i++ )
	{
		CString strNum;
		const SITEMCUSTOM sPointShop = GLGaeaClient::GetInstance().GetCharacter()->GET_POINT_SHOP(i);
		SITEM* pItem = GLItemMan::GetInstance().GetItem( sPointShop.sNativeID );
		SITEM* pItem2 = GLItemMan::GetInstance().GetItem( sItemRecipeID[i] );
		if ( !pItem2 )
		{
			m_pTicketItemImage[i]->ResetItem();
			m_pRecipeNumText[i]->ClearText();
			continue;
		}
		if( sPointShop.sNativeID == NATIVEID_NULL() )
		{	
			strNum.Format( "Ticket No. %d", nRecipeAmount[i] );
			m_pRecipeNumText[i]->SetText( strNum, NS_UITEXTCOLOR::RED );
			m_pTicketItemImage[i]->SetItem ( sItemRecipeID[i] );
		}
		else
		{
			if ( sPointShop.sNativeID != NATIVEID_NULL()  )
			{
             if ( sPointShop.sNativeID == sItemRecipeID[i]  )
			 {
				if ( sPointShop.wTurnNum >= nRecipeAmount[i] )
				{
				strNum.Format( "Ticket No. %d", nRecipeAmount[i] );
				m_pRecipeNumText[i]->SetText( strNum, NS_UITEXTCOLOR::WHITE );
				m_pTicketItemImage[i]->SetItem ( sPointShop.sNativeID );
				}
			 }
			else if ( sPointShop.wTurnNum < nRecipeAmount[i] )
			 { 
				strNum.Format( "Ticket No. %d", nRecipeAmount[i] );
				m_pRecipeNumText[i]->SetText( strNum, NS_UITEXTCOLOR::RED );
				m_pTicketItemImage[i]->SetItem ( sPointShop.sNativeID );
			 }
			else if ( sPointShop.sNativeID != sItemRecipeID[i] )
			 { 
				strNum.Format( "Ticket No. %d", nRecipeAmount[i] );
				m_pRecipeNumText[i]->SetText( strNum, NS_UITEXTCOLOR::RED );
				m_pTicketItemImage[i]->SetItem ( sPointShop.sNativeID );
			}

			}
			else
			{
				m_pRecipeNumText[i]->ClearText();
			}
		  
		
		   }
	    }
      			m_bRefresh = false;
    }

}

	
	GLCHARLOGIC& ref_CharData = GLGaeaClient::GetInstance().GetCharacterLogic();
	LONGLONG lnSaldo = ref_CharData.m_lnSaldo;
	CString strTemp = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( lnSaldo, 3, "," );
	if ( m_pPremiumPointstText ) m_pPremiumPointstText->SetOneLineText( strTemp, NS_UITEXTCOLOR::ORANGE );

    if ( m_bOK || m_bIsOK ) HideButton();
    if ( !m_bOK )
	{
		ShowButton();
		m_pResult->SetPercent(FALSE);
	    m_pResult->SetVisibleSingle(FALSE);
	    m_pBodyLeft->SetVisibleSingle(FALSE);
	    m_pBodyRight->SetVisibleSingle(FALSE);
	    m_pTextBox->SetVisibleSingle(FALSE);
	}
	if ( m_bOK )
	{  
	  if ( m_bIsOK )
	  {
		/*m_fTime -=fElapsedTime;
		int nMAX = 5;	

	    float _fPercent = m_fTime/(float)nMAX;
	    m_pResult->SetVisibleSingle(TRUE);
	    m_pResult->SetPercent(_fPercent);       
		//GLGaeaClient::GetInstance().GetCharacter()->ReqGESTURE ( 48, true );
		if ( m_fTime <= 0.0f ) m_fTime = 0.0f;
		if ( m_fTime == 0.0f )*/
		{
	    //m_pBodyLeft->SetVisibleSingle(TRUE);
	    //m_pBodyRight->SetVisibleSingle(TRUE);
	    //m_pTextBox->SetVisibleSingle(TRUE);
 		GLGaeaClient::GetInstance().GetCharacter()->ReqPointShop( 0 );//m_dwNpcID );
		m_bOK = false;
		}
	  }else if ( !m_bIsOK )
	  {
		GLGaeaClient::GetInstance().GetCharacter()->ReqPointShop( 0 );//m_dwNpcID );
		m_bOK = false;
	  }
	}

	if ( m_bResult ) 
	{
	 m_bIsOK = false;
	 m_bResult = false;
	 m_fTime = 0.0f;
    }
}

VOID CPointShopWindow::TranslateUIMessage( UIGUID ControlID, DWORD dwMsg )
{
	switch( ControlID )
	{
	case ET_CONTROL_TITLE:
	case ET_CONTROL_TITLE_F:
		{
			if ( (dwMsg & UIMSG_LB_DUP) && CHECK_MOUSE_IN ( dwMsg ) )
				CInnerInterface::GetInstance().SetDefaultPosInterface( POINT_SHOP_WINDOW );
		}
		break;
	case CNDEV_POINTSHOP_ITEM_LIST_TEXTBOX:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				const int nIndex = m_pListText->GetSelectPos ();
				if ( nIndex < 0 || m_pListText->GetCount () <= nIndex ) return ;

				m_pListText->SetUseOverColor ( TRUE );
				m_pListText->SetOverColor ( nIndex, NS_UITEXTCOLOR::PARTY );

				if( UIMSG_LB_UP & dwMsg )
				{
					m_nSelectIndex = nIndex;
					m_bRefresh = true;
				}
			}
		}
		break;
	case PRODUCT_IMAGE:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( m_sNativeID != NATIVEID_NULL() )
				{
					SITEMCUSTOM sItemCustom(m_sNativeID);
					CInnerInterface::GetInstance().SHOW_ITEM_INFO ( sItemCustom, FALSE, FALSE, FALSE, USHRT_MAX, USHRT_MAX );
				}
			}
		}
		break;
	case TICKET_IMAGE00:
		{
			if( CHECK_MOUSE_IN( dwMsg ) )
			{
				int nIndex = ControlID - TICKET_IMAGE00;
				CItemMove* pItemMove = CInnerInterface::GetInstance().GetItemMove();
				if( !pItemMove )
				{
					GASSERT( 0 && "CPointShopWindow::TranslateUIMessage, pItemMove == NULL" );
					break;
				}         
				if ( pItemMove->GetItem () != NATIVEID_NULL () )
				{
					const UIRECT& rcSlotPos = m_pTicketItemImage[nIndex]->GetGlobalPos ();
					pItemMove->SetGlobalPos ( rcSlotPos );						
				}				

				if ( pItemMove->GetItem () == NATIVEID_NULL () && m_pTicketItemImage[nIndex]->GetItem () != NATIVEID_NULL () )
				{
					const UIRECT& rcImagePos = m_pTicketItemImage[nIndex]->GetGlobalPos ();
				}


				if ( !m_bOK && !m_bResult )
				{
					if( dwMsg & UIMSG_LB_UP )
					{
						m_bRefresh = true;
						GLGaeaClient::GetInstance().GetCharacter()->SetPointShopMoveItem( nIndex );
					}

					if( dwMsg & UIMSG_RB_UP )
					{
						m_bRefresh = true;
						GLGaeaClient::GetInstance().GetCharacter()->ReSetPointShopItem( nIndex );
					}
				}
				if( m_pTicketItemImage[nIndex]->GetItem().sNativeID != NATIVEID_NULL () )
				{
					CInnerInterface::GetInstance().SHOW_ITEM_INFO ( m_pTicketItemImage[nIndex]->GetItem(), FALSE, FALSE, FALSE, USHRT_MAX, USHRT_MAX );
				}
			}
		}
	break;
	case CNDEV_POINTSHOP_PURCHASE_BUTTON:
		if( CHECK_KEYFOCUSED( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
		{
			if ( !m_bOK )
			{
				DoPurchaseItem();
			}
			SetMessageEx( dwMsg &= ~UIMSG_KEY_FOCUSED ); // Focus ??? ??? ??
		}
		break;

	case CNDEV_POINTSHOP_TOPUP_BUTTON:
		{
			if (CHECK_MOUSEIN_LBUPLIKE(dwMsg))
			{
				CInnerInterface::GetInstance().SetItemMallTopUpWindowOpen();
			}
		}
		break;

	case ET_CONTROL_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				CInnerInterface::GetInstance().ClosePointShopWindow();
				m_bRefresh = true;
				m_pBodyLeft->SetVisibleSingle(FALSE);
			    m_pBodyRight->SetVisibleSingle(FALSE);
		        m_pTextBox->SetVisibleSingle(FALSE);

			}
		}
		break;
	}

	CUIWindowEx::TranslateUIMessage( ControlID, dwMsg );
}

void CPointShopWindow::SetAnimation()
{
    m_bOK = true;
	m_bResult = false;
	m_fTime = 0.0f;
}

VOID CPointShopWindow::DoPurchaseItem()
{
	GLCHARLOGIC& ref_CharData = GLGaeaClient::GetInstance().GetCharacterLogic ();
    LONGLONG lnSaldo = ref_CharData.m_lnSaldo;
	DWORD dwPrice = GLGaeaClient::GetInstance().GetCharacter()->GetPointShopMoney();

	if ( lnSaldo >= (LONGLONG)dwPrice ) m_bIsOK = true;

	if ( dwPrice == UINT_MAX ) 
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMPOINT_SHOP_FB_NOMIX") );
		SetBuyItem( ID2GAMEINTEXT("EMPOINT_SHOP_FB_NOMIX") );
		return;
	}
	if ( dwPrice > 0 )
	{
		CString strTemp;
		strTemp.Format( ID2GAMEINTEXT("POINT_SHOP_MONEY_OKQUESTION"), dwPrice );
		DoModal( strTemp, MODAL_QUESTION, YESNO, MODAL_POINTSHOP_OKQUESTION );
	}
	else
	{
		DoModal( ID2GAMEINTEXT("POINT_SHOP_OKQUESTION"), MODAL_QUESTION, YESNO, MODAL_POINTSHOP_OKQUESTION );
	}
	
}
bool CPointShopWindow::SetBuyItem( CString strMsg, bool bSuccess, bool bFail )
{
	DWORD dwColor = NS_UITEXTCOLOR::SILVER;
		
	if ( bSuccess ) 
	{
		///bool bACTION = GLGaeaClient::GetInstance().GetCharacter()->ReqGESTURE ( 9, true );
		////if ( bACTION )
		//{
		m_bResult = true;
		m_bRefresh = true;
		return false;
		//}
	}

	if ( bFail ) 
	{
		//bool bACTION = GLGaeaClient::GetInstance().GetCharacter()->ReqGESTURE ( 14, true );
		//if ( bACTION )
		//{
		m_bResult = true;
		m_bRefresh = true;
		//return false;
		//}
	}
	return true;
}
void CPointShopWindow::LoadItemList( WORD wType )
{
	m_pListText->ClearText ();

	static CString strKeyword[] =
	{
		"CNDEV_POINTSHOP_NEW_ITEM",
		"CNDEV_POINTSHOP_BESTSELLER_ITEM",
		"CNDEV_POINTSHOP_OLD_ITEM"
	};

	std::vector<GLSHOP>::size_type i = 0, j = GLCONST_CHAR::vecSHOP.size();
	for( ; i < j; ++i )
	{
		GLSHOP sShop = GLCONST_CHAR::vecSHOP[i];

		SITEM* pItem = GLItemMan::GetInstance().GetItem( sShop.sNativeID );
		if ( !pItem )
			continue;

		if ( wType != 0 )
		{
			if ( wType >= 1 && wType <= 9 ) 
			{
				if ( pItem->sBasicOp.emItemType != ITEM_SUIT )
					continue;

				if ( wType == 1 )
				{
					if ( pItem->sSuitOp.emSuit != SUIT_HEADGEAR )
						continue;
				}
				else if ( wType == 2 )
				{
					if ( pItem->sSuitOp.emSuit != SUIT_UPPER )
						continue;
				}
				else if ( wType == 3 )
				{
					if ( pItem->sSuitOp.emSuit != SUIT_LOWER )
						continue;
				}
				else if ( wType == 4 )
				{
					if ( pItem->sSuitOp.emSuit != SUIT_HAND )
						continue;
				}
				else if ( wType == 5 )
				{
					if ( pItem->sSuitOp.emSuit != SUIT_FOOT )
						continue;
				}
				else if ( wType == 6 )
				{
					if ( pItem->sSuitOp.emSuit != SUIT_HANDHELD )
						continue;
				}
				else if ( wType == 7 )
				{
					if ( pItem->sSuitOp.emSuit != SUIT_NECK )
						continue;
				}
				else if ( wType == 8 )
				{
					if ( pItem->sSuitOp.emSuit != SUIT_WRIST )
						continue;
				}
				else if ( wType == 9 )
				{
					if ( pItem->sSuitOp.emSuit != SUIT_FINGER )
						continue;
				}
			}
			else if ( wType == 10 )
			{
				if ( pItem->sBasicOp.emItemType == ITEM_SUIT )
					continue;

				if ( pItem->sBasicOp.emItemType == ITEM_BOX )
					continue;

				if ( pItem->sBasicOp.emItemType == ITEM_RANDOMITEM )
					continue;

				if ( pItem->sBasicOp.IsDISGUISE() )
					continue;
			}
			else if ( wType == 11 )
			{
				if ( pItem->sBasicOp.emItemType != ITEM_BOX || pItem->sBasicOp.emItemType != ITEM_RANDOMITEM )
					continue;
			}
			else if ( wType == 12 )
			{
				if ( !pItem->sBasicOp.IsDISGUISE() )
					continue;
			}
		}

		CString strTemp;
		D3DCOLOR dwColor = NS_UITEXTCOLOR::WHITE;
		strTemp.Format( "%s", pItem->GetName() );
		switch ( pItem->sBasicOp.emLevel )
		{
			case LEVEL_NORMAL:			
				dwColor = NS_UITEXTCOLOR::WHITE;					
				break;
			case LEVEL_RARE:
				dwColor = NS_UITEXTCOLOR::RARE;
				break;
			case LEVEL_HEROIC:
				dwColor = NS_UITEXTCOLOR::HEROIC;		
				break;
			case LEVEL_ANCIENT:
				dwColor = NS_UITEXTCOLOR::ANCIENT;
				break;
			case LEVEL_LEGENDARY:
				dwColor = NS_UITEXTCOLOR::LEGENDARY;		
				break;
			case LEVEL_VOTE_ITEM:		
				dwColor = NS_UITEXTCOLOR::VOTEITEM;			
				break;
			case LEVEL_PREMIUM_ITEM:	
				dwColor = NS_UITEXTCOLOR::PREMIUMITEM;			
				break;
			case LEVEL_CONTRIBUTION_ITEM:
				dwColor = NS_UITEXTCOLOR::PALEGREEN;
				break;
			case LEVEL_HIGH_CLASS:
				dwColor = NS_UITEXTCOLOR::HIGHCLASS;
				break;
		}

		int nIndex = m_pListText->AddText ( strTemp, dwColor );	
		m_pListText->SetTextImage ( nIndex, strKeyword[0] );	
		m_pListText->SetTextData ( nIndex, sShop.dwID );

		m_pListText->SetCurLine ( 0 );

		const int nTotal = m_pListText->GetCount ();
		const int nViewPerPage = m_pListText->GetVisibleLine ();	
		m_pListScrollBar->GetThumbFrame()->SetState ( nTotal, nViewPerPage );
		m_pListScrollBar->GetThumbFrame()->SetPercent ( 0.0f );
	}
}
void CPointShopWindow::ShowButton()
{
	m_pGenerateButton->SetVisibleSingle( TRUE );
}
void CPointShopWindow::HideButton()
{
	m_pGenerateButton->SetVisibleSingle( FALSE );
}