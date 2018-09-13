#include "StdAfx.h"
#include "ItemMixWindow.h"
#include "CompoundWindow/ItemImageRecipe.h"
#include "CompoundWindow/CompoundWindowMenu.h"
#include "CompoundWindow/CompoundWindowCheckBox.h"
#include "ItemImage.h"
#include "BasicTextButton.h"
#include "GLItemMan.h"
#include "GLItemMixMan.h"
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
CItemMixWindow::CItemMixWindow()	// ???
	: m_pGenerateBtn( NULL )	
	, m_pCheckBoxCheckList( NULL )
	, m_pMenu( NULL )
	, m_pTypeText( NULL )
	, m_pTitleText( NULL )
	, m_pRateText( NULL )
	, m_pCostText( NULL )
	, m_pAmountText( NULL )
	, m_pGoldText( NULL )
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
{
	SecureZeroMemory( m_pMeterialItemImageSlot, sizeof(m_pMeterialItemImageSlot) );
	SecureZeroMemory( m_pMeterialItemImage, sizeof(m_pMeterialItemImage) );

}
CCompoundWindowCheckBox* CItemMixWindow::CreateCheckBox ( CString strKeyword, UIGUID ControlID )
{
	CCompoundWindowCheckBox* pSlot = new CCompoundWindowCheckBox;
	pSlot->CreateSub ( this, strKeyword.GetString (), UI_FLAG_DEFAULT, ControlID );
	pSlot->CreateSubControl();
	pSlot->SetVisibleSingle(TRUE);
	RegisterControl ( pSlot );

	return pSlot;
}
CBasicTextBox* CItemMixWindow::CreateStaticControl ( char* szControlKeyword, CD3DFontPar* pFont, int nAlign )
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub ( this, szControlKeyword );
	pStaticText->SetFont ( pFont );
	pStaticText->SetTextAlign ( nAlign );	
	RegisterControl ( pStaticText );

	return pStaticText;
}
CItemMixWindow::~CItemMixWindow()
{
}

void CItemMixWindow::Reset()
{
	m_bOK = false;
	m_bIsOK = false;
	m_bResult = false;
	m_fTime = 0.0f;
}
CBasicTextBox* CItemMixWindow::CreateTextBox ( const char* szTextBox )
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, D3DFONT_SHADOW | D3DFONT_ASCII );

	CBasicTextBox* pTextBox = new CBasicTextBox;
	pTextBox->CreateSub ( this, szTextBox );
    pTextBox->SetFont ( pFont8 );	
	RegisterControl ( pTextBox );
	return pTextBox;
}
CBasicButtonText* CItemMixWindow::CreateButtonBlue ( char* szButton, char* szTextBox, CD3DFontPar* pFont, int nAlign, UIGUID ControlID, CString strText )
{
	CBasicButtonText* pButton = new CBasicButtonText;
	pButton->CreateRanButtonBlue ( this, szButton, ControlID );
	pButton->CreateTextBox ( szTextBox, pFont, nAlign );
	pButton->SetOneLineText( strText.GetString() );
	RegisterControl ( pButton );

	return pButton;
}

CItemImageRecipe*	CItemMixWindow::CreateItemImageRecipe( const char* szControl, UIGUID ControlID )
{
	CItemImageRecipe* pItemImage = new CItemImageRecipe;
	pItemImage->CreateSub ( this, szControl, UI_FLAG_DEFAULT, ControlID );
	pItemImage->CreateSubControl ();
	RegisterControl ( pItemImage );
	return pItemImage;
}

VOID CItemMixWindow::CreateSubControl()
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
	//Top-most linebox
	{
		CBasicLineBox* pBasicLineBox00 = new CBasicLineBox;
		pBasicLineBox00->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBox00->CreateBaseBoxQuestList( "ITEM_COMPOUND_LINEBOX_00" );
		RegisterControl ( pBasicLineBox00 );

		CBasicLineBox* pBasicLineBox01 = new CBasicLineBox;
		pBasicLineBox01->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBox01->CreateBaseBoxQuestList( "ITEM_COMPOUND_LINEBOX_01" );
		RegisterControl ( pBasicLineBox01 );
	}

	//Type Image linebox
	{
		CBasicLineBox* pBasicLineBoxType = new CBasicLineBox;
		pBasicLineBoxType->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBoxType->CreateBaseBoxQuestList( "ITEM_COMPOUND_LINEBOX_02" );
		RegisterControl ( pBasicLineBoxType );
	}

	//Top-mid linebox
	{
		CBasicLineBox* pBasicLineBoxList = new CBasicLineBox;
		pBasicLineBoxList->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBoxList->CreateBaseBoxQuestList( "ITEM_COMPOUND_LINEBOX_03" );
		RegisterControl ( pBasicLineBoxList );

		CBasicLineBox* pBasicLineBoxTitle = new CBasicLineBox;
		pBasicLineBoxTitle->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBoxTitle->CreateBaseBoxQuestList( "ITEM_COMPOUND_LINEBOX_04" );
		RegisterControl ( pBasicLineBoxTitle );
	}

	{
		CBasicLineBox* pBasicLineBoxItemList = new CBasicLineBox;
		pBasicLineBoxItemList->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBoxItemList->CreateBaseBoxQuestList( "ITEM_COMPOUND_LINEBOX_05" );
		RegisterControl ( pBasicLineBoxItemList );
	}

	{
		CBasicLineBox* pBasicLineBoxBody = new CBasicLineBox;
		pBasicLineBoxBody->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBoxBody->CreateBaseBoxQuestList( "ITEM_COMPOUND_LINEBOX_06" );
		RegisterControl ( pBasicLineBoxBody );

		//Inner-Body

		{
			CBasicLineBox* pBasicLineBoxMoney = new CBasicLineBox;
			pBasicLineBoxMoney->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
			pBasicLineBoxMoney->CreateBaseBoxQuestList( "ITEM_COMPOUND_LINEBOX_08" );
			RegisterControl ( pBasicLineBoxMoney );
		}

		{
			CBasicLineBox* pBasicLineBoxCondition = new CBasicLineBox;
			pBasicLineBoxCondition->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
			pBasicLineBoxCondition->CreateBaseBoxQuestList( "ITEM_COMPOUND_LINEBOX_09" );
			RegisterControl ( pBasicLineBoxCondition );
		}

		{
			CBasicLineBox* pBasicLineBoxCost = new CBasicLineBox;
			pBasicLineBoxCost->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
			pBasicLineBoxCost->CreateBaseBoxQuestList( "ITEM_COMPOUND_LINEBOX_10" );
			RegisterControl ( pBasicLineBoxCost );
		}

		{
			CBasicLineBox* pBasicLineBoxResult = new CBasicLineBox;
			pBasicLineBoxResult->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
			pBasicLineBoxResult->CreateBaseBoxQuestList( "ITEM_COMPOUND_LINEBOX_11" );
			RegisterControl ( pBasicLineBoxResult );
		}

		{
			CBasicLineBox* pBasicLineBoxAmount = new CBasicLineBox;
			pBasicLineBoxAmount->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
			pBasicLineBoxAmount->CreateBaseBoxQuestList( "ITEM_COMPOUND_LINEBOX_12" );
			RegisterControl ( pBasicLineBoxAmount );
		}
	}

	{
		CBasicLineBox* pBasicLineBoxCheckList = new CBasicLineBox;
		pBasicLineBoxCheckList->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBoxCheckList->CreateBaseBoxQuestList( "ITEM_COMPOUND_LINEBOX_07" );
		RegisterControl ( pBasicLineBoxCheckList );
	}

	{
		m_pCheckBoxCheckList = CreateCheckBox( "ITEM_COMPOUND_WINDOW_CHECKBOX_CHECKLIST", CHECK_CHECKLIST );
		m_pCheckBoxCheckList->SetTextTitle( ID2GAMEWORD( "ITEM_COMPOUND_CHECKBOX_TEXT_STATIC", 1 ), NS_UITEXTCOLOR::WHITE );
	}

	{
		CCompoundWindowMenu* pMenu = new CCompoundWindowMenu;
		pMenu->CreateSub ( this, "ITEM_COMPOUND_MENU" );
		pMenu->CreateSubControl();
		pMenu->SetVisibleSingle(TRUE);
		RegisterControl ( pMenu );
		m_pMenu = pMenu;
	}

	


	//Type Title
	{
		m_pTypeText = CreateStaticControl ( "ITEM_COMPOUND_TYPE_TEXT", pFont9, nAlignCenterBoth );
		m_pTitleText = CreateStaticControl ( "ITEM_COMPOUND_TITLE_TEXT", pFont9, nAlignCenterBoth );
		m_pRateText = CreateStaticControl ( "ITEM_COMPOUND_RATE_TEXT", pFont9, nAlignCenterBoth );
		m_pCostText = CreateStaticControl ( "ITEM_COMPOUND_COST_TEXT", pFont9, nAlignLeftCenter );
		m_pAmountText = CreateStaticControl ( "ITEM_COMPOUND_AMOUNT_TEXT", pFont9, nAlignLeftCenter );
		m_pGoldText = CreateStaticControl ( "ITEM_COMPOUND_GOLD_TEXT", pFont9, nAlignLeftCenter );
		m_pResourceText = CreateStaticControl ( "ITEM_COMPOUND_RESOURCE_TEXT", pFont9, nAlignCenterBoth );

		m_pResourceText->SetOneLineText( ID2GAMEWORD( "ITEM_COMPOUND_RESOURCE_TEXT_STATIC", 0 ), NS_UITEXTCOLOR::CHARTREUSE );
	}

	{
		CBasicButton* pButton = NULL;
		pButton = CreateFlipButton ( "ITEM_COMPOUND_MONEY_BUTTON", "ITEM_COMPOUND_MONEY_BUTTON_F", ITEM_COMPOUND_MONEY_BUTTON, CBasicButton::CLICK_FLIP );
		pButton->CreateMouseOver ( "ITEM_COMPOUND_MONEY_BUTTON_F" );
		pButton->SetAlignFlag ( UI_FLAG_DEFAULT );
		pButton->SetUseDynamic ( TRUE );
	}

	{
		CD3DFontPar* pFont = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 10, _DEFAULT_FONT_FLAG );

		CBasicTextBoxEx* pTextBox = new CBasicTextBoxEx;
		pTextBox->CreateSub ( this, "ITEM_LIST_TEXTBOX", UI_FLAG_DEFAULT, ITEM_LIST_TEXTBOX );
		pTextBox->SetFont ( pFont );		
		pTextBox->SetLineInterval ( 3.0f );
		pTextBox->SetSensitive ( true );
		pTextBox->SetLimitLine ( 10000 );
		RegisterControl ( pTextBox );
		m_pListText = pTextBox;
		
		int nTotalLine = pTextBox->GetVisibleLine ();

		CBasicScrollBarEx* pScrollBar = new CBasicScrollBarEx;
		pScrollBar->CreateSub ( this, "BASIC_SCROLLBAR", UI_FLAG_RIGHT | UI_FLAG_YSIZE, ITEM_LIST_SCROLLBAR );
		pScrollBar->CreateBaseScrollBar ( "ITEM_LIST_SCROLLBAR" );
		pScrollBar->GetThumbFrame()->SetState ( 1, nTotalLine );
		RegisterControl ( pScrollBar );
		m_pListScrollBar = pScrollBar;
	}

	//Result image
	{
		CBasicLineBox* pBasicLineBoxBack = new CBasicLineBox;
		pBasicLineBoxBack->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBoxBack->CreateBaseBoxQuestList( "ITEM_COMPOUND_RESULT_IMAGE_BACK" );
		RegisterControl ( pBasicLineBoxBack );

		CItemImage* pItemImage = new CItemImage;
		pItemImage->CreateSub ( this, "ITEM_COMPOUND_RESULT_IMAGE", UI_FLAG_DEFAULT, RESULT_IMAGE );
		pItemImage->CreateSubControl ();
		pItemImage->SetUseRender ( TRUE );
		pItemImage->SetVisibleSingle ( FALSE );
		RegisterControl ( pItemImage );
		m_pItemResultImage = pItemImage;
	}
	//recipe slot - 
	CString strRecipeSlot[SLOT_RECIPE_SIZE] = 
	{
		"ITEM_COMPOUND_RECIPE_IMAGE00",
		"ITEM_COMPOUND_RECIPE_IMAGE01",
		"ITEM_COMPOUND_RECIPE_IMAGE02",
		"ITEM_COMPOUND_RECIPE_IMAGE03",
		"ITEM_COMPOUND_RECIPE_IMAGE04",
	};

	CString strRecipeSlotOver[SLOT_RECIPE_SIZE] = 
	{
		"ITEM_COMPOUND_RECIPE_IMAGE_BACK00",
		"ITEM_COMPOUND_RECIPE_IMAGE_BACK01",
		"ITEM_COMPOUND_RECIPE_IMAGE_BACK02",
		"ITEM_COMPOUND_RECIPE_IMAGE_BACK03",
		"ITEM_COMPOUND_RECIPE_IMAGE_BACK04",
	};
	for ( int i = 0; i < SLOT_RECIPE_SIZE; i++ )
	{
		m_pMeterialItemImage[i] = CreateItemImageRecipe( strRecipeSlot[i].GetString(), RECIPE_IMAGE00 + i );
		m_pMeterialItemImageSlot[i] = CreateControl( strRecipeSlotOver[i].GetString() );
		m_pMeterialItemImageSlot[i]->SetVisibleSingle( FALSE );
	}
	for(int i = 0; i < 5; i++ )
	{	
		CString strKeyword;
		strKeyword.Format( "ITEM_COMPOUND_RECIPE_TEXT_BACK0%d", i );

		CBasicLineBox* pBasicLineBoxBack2 = new CBasicLineBox;
		pBasicLineBoxBack2->CreateSub ( this, "BASIC_LINE_BOX_QUEST_LIST", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBoxBack2->CreateBaseBoxQuestList( (char*)strKeyword.GetString() );
		RegisterControl ( pBasicLineBoxBack2 );

		strKeyword.Format( "ITEM_COMPOUND_RECIPE_TEXT0%d", i );
		m_pRecipeNumText[i] = CreateStaticControl ( (char*)strKeyword.GetString(), pFont9, nAlignCenterBoth );

		strKeyword.Format( "ITEM_COMPOUND_RECIPE_TEXT0%d", i );
		m_pRecipeNumText2[i] = CreateStaticControl ( (char*)strKeyword.GetString(), pFont9, nAlignCenterBoth );
	}

//	m_pGenerateButton = CreateButtonBlue ( "ITEM_COMPOUND_GENERATE_BUTTON", "ITEM_COMPOUND_GENERATE_BUTTON_TEXT_POS", pFont9, nAlignCenterBoth, ITEM_COMPOUND_GENERATE_BUTTON, "Generate" );		
	m_pGenerateButton = CreateTextButton ( "ITEM_COMPOUND_GENERATE_BUTTON", ITEM_COMPOUND_GENERATE_BUTTON, "Generate" );
	
	LoadItemList( 0 );
}

CBasicTextButton*	 CItemMixWindow::CreateTextButton ( char* szButton, UIGUID ControlID, char* szText )
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

void CItemMixWindow::SetVisibleSingle( BOOL bVisible )
{
	CUIWindowEx::SetVisibleSingle( bVisible );

	Reset();
}

VOID CItemMixWindow::Update( INT x, INT y, BYTE LB, BYTE MB, BYTE RB, INT nScroll, FLOAT fElapsedTime, BOOL bFirstControl )
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

			SNATIVEID sItemRecipeID[5];
			int nRecipeAmount[5];

			int nID = m_pListText->GetTextData ( m_nSelectIndex );
			std::vector<GLCOMPOUND>::size_type i = 0, j = GLCONST_CHAR::vecCOMPOUND.size();
			for( ; i < j; ++i )
			{
				GLCOMPOUND sCompound = GLCONST_CHAR::vecCOMPOUND[i];
				if ( sCompound.dwID == nID )
				{
					sItemID = sCompound.sNativeID;
					fRate = sCompound.fRate;
					lnPrice = sCompound.lnPrice;
					nAmount = sCompound.wNum;
					m_sNativeID = sItemID;

					for(int i = 0; i < 5; i++ )
					{
						GLCOMPOUND_RECIPE sRecipe = sCompound.glRecipe[i];
						sItemRecipeID[i] = sRecipe.sNativeID;
						nRecipeAmount[i] = sRecipe.wNum;
						m_sRecipeNativeID[i] = sRecipe.sNativeID;
                    	m_pMeterialItemImage[i]->ResetItem();
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

			strName.Format( "%s", pItem->GetName() );
			if ( m_pTitleText ) m_pTitleText->SetOneLineText( strName, NS_UITEXTCOLOR::WHITE );

			strName.Format( "The item can be produced with %d", int(fRate));
			strName += "% rate.";
			if ( m_pRateText ) m_pRateText->SetOneLineText ( strName, NS_UITEXTCOLOR::GREEN );

			CString strTemp = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( lnPrice, 3, "," );
			strName.Format( "Cost: %s", strTemp );
			if ( m_pCostText ) m_pCostText->SetOneLineText ( strName, NS_UITEXTCOLOR::ORANGE );

			strName.Format( "Amount: %d", nAmount );
			if ( m_pAmountText ) m_pAmountText->SetOneLineText( strName, NS_UITEXTCOLOR::WHITE );



	for(int i = 0; i < 5; i++ )
	{
		CString strNum;
		const SITEMCUSTOM sItemMix = GLGaeaClient::GetInstance().GetCharacter()->GET_ITEM_MIX(i);
		SITEM* pItem = GLItemMan::GetInstance().GetItem( sItemMix.sNativeID );
		SITEM* pItem2 = GLItemMan::GetInstance().GetItem( sItemRecipeID[i] );
		if ( !pItem2 )
		{
			m_pMeterialItemImage[i]->ResetItem();
			m_pRecipeNumText[i]->ClearText();
			continue;
		}
		if( sItemMix.sNativeID == NATIVEID_NULL() )
		{	
			strNum.Format( "0/%d", nRecipeAmount[i] );
			m_pRecipeNumText[i]->SetText( strNum, NS_UITEXTCOLOR::RED );
			m_pMeterialItemImage[i]->SetItem ( sItemRecipeID[i] );
		}
		else
		{
			if ( sItemMix.sNativeID != NATIVEID_NULL()  )
			{
             if ( sItemMix.sNativeID == sItemRecipeID[i]  )
			 {
				if ( sItemMix.wTurnNum >= nRecipeAmount[i] )
				{
				strNum.Format( "%d/%d", sItemMix.wTurnNum, nRecipeAmount[i] );
				m_pRecipeNumText[i]->SetText( strNum, NS_UITEXTCOLOR::WHITE );
				m_pMeterialItemImage[i]->SetItem ( sItemMix.sNativeID );
				}
			 }
			else if ( sItemMix.wTurnNum < nRecipeAmount[i] )
			 { 
				strNum.Format( "%d/%d", sItemMix.wTurnNum, nRecipeAmount[i] );
				m_pRecipeNumText[i]->SetText( strNum, NS_UITEXTCOLOR::RED );
				m_pMeterialItemImage[i]->SetItem ( sItemMix.sNativeID );
			 }
			else if ( sItemMix.sNativeID != sItemRecipeID[i] )
			 { 
				strNum.Format( "%d/%d", sItemMix.wTurnNum, nRecipeAmount[i] );
				m_pRecipeNumText[i]->SetText( strNum, NS_UITEXTCOLOR::RED );
				m_pMeterialItemImage[i]->SetItem ( sItemMix.sNativeID );
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

	
	GLCHARLOGIC& ref_CharData = GLGaeaClient::GetInstance().GetCharacterLogic ();
	LONGLONG lnGold = ref_CharData.m_lnMoney;
	CString strTemp = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( lnGold, 3, "," );
	if ( m_pGoldText ) m_pGoldText->SetOneLineText( strTemp, NS_UITEXTCOLOR::WHITE );

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
		m_fTime -=fElapsedTime;
		int nMAX = 5;	

	    float _fPercent = m_fTime/(float)nMAX;
	    m_pResult->SetVisibleSingle(TRUE);
	    m_pResult->SetPercent(_fPercent);       
		//GLGaeaClient::GetInstance().GetCharacter()->ReqGESTURE ( 48, true );
		if ( m_fTime <= 0.0f ) m_fTime = 0.0f;
		if ( m_fTime == 0.0f )
		{
	    m_pBodyLeft->SetVisibleSingle(TRUE);
	    m_pBodyRight->SetVisibleSingle(TRUE);
	    m_pTextBox->SetVisibleSingle(TRUE);
 		GLGaeaClient::GetInstance().GetCharacter()->ReqItemMix( 0 );//m_dwNpcID );
		m_bOK = false;
		}
	  }else if ( !m_bIsOK )
	  {
		GLGaeaClient::GetInstance().GetCharacter()->ReqItemMix( 0 );//m_dwNpcID );
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

VOID CItemMixWindow::TranslateUIMessage( UIGUID ControlID, DWORD dwMsg )
{
	switch( ControlID )
	{
	case ET_CONTROL_TITLE:
	case ET_CONTROL_TITLE_F:
		{
			if ( (dwMsg & UIMSG_LB_DUP) && CHECK_MOUSE_IN ( dwMsg ) )
				CInnerInterface::GetInstance().SetDefaultPosInterface( ITEM_MIX_WINDOW );
		}
		break;
	case CHECK_CHECKLIST:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
				m_pCheckBoxCheckList->SetCheck();
		}
		break;
	case ITEM_LIST_TEXTBOX:
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
	case RESULT_IMAGE:
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
	case RECIPE_IMAGE00:
	case RECIPE_IMAGE01:
	case RECIPE_IMAGE02:
	case RECIPE_IMAGE03:
	case RECIPE_IMAGE04:
		{
			if( CHECK_MOUSE_IN( dwMsg ) )
			{
				int nIndex = ControlID - RECIPE_IMAGE00;
				CItemMove* pItemMove = CInnerInterface::GetInstance().GetItemMove();
				if( !pItemMove )
				{
					GASSERT( 0 && "CItemMixWindow::TranslateUIMessage, pItemMove == NULL" );
					break;
				}         
				if ( pItemMove->GetItem () != NATIVEID_NULL () )
				{
					const UIRECT& rcSlotPos = m_pMeterialItemImage[nIndex]->GetGlobalPos ();
					pItemMove->SetGlobalPos ( rcSlotPos );						
				}				

				if ( pItemMove->GetItem () == NATIVEID_NULL () && m_pMeterialItemImage[nIndex]->GetItem () != NATIVEID_NULL () )
				{
					const UIRECT& rcImagePos = m_pMeterialItemImage[nIndex]->GetGlobalPos ();
				}


				if ( !m_bOK && !m_bResult )
				{
					if( dwMsg & UIMSG_LB_UP )
					{
						m_bRefresh = true;
						GLGaeaClient::GetInstance().GetCharacter()->SetItemMixMoveItem( nIndex );
					}

					if( dwMsg & UIMSG_RB_UP )
					{
						m_bRefresh = true;
						GLGaeaClient::GetInstance().GetCharacter()->ReSetItemMixItem( nIndex );
					}
				}
				if( m_pMeterialItemImage[nIndex]->GetItem().sNativeID != NATIVEID_NULL () )
				{
					CInnerInterface::GetInstance().SHOW_ITEM_INFO ( m_pMeterialItemImage[nIndex]->GetItem(), FALSE, FALSE, FALSE, USHRT_MAX, USHRT_MAX );
				}
			}
		}
	break;
	case ITEM_COMPOUND_GENERATE_BUTTON:
		if( CHECK_KEYFOCUSED( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE( dwMsg ) )
		{
			if ( !m_bOK )
			{
				DoItemMix();
			}
			SetMessageEx( dwMsg &= ~UIMSG_KEY_FOCUSED ); // Focus ??? ??? ??
		}
		break;
	case ET_CONTROL_BUTTON:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				CInnerInterface::GetInstance().CloseItemMixWindow();
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

void CItemMixWindow::SetAnimation()
{
    m_bOK = true;
	m_bResult = false;
	m_fTime = 5.0f;
}

VOID CItemMixWindow::DoItemMix()
{
	GLCHARLOGIC& ref_CharData = GLGaeaClient::GetInstance().GetCharacterLogic ();
    LONGLONG lnGold = ref_CharData.m_lnMoney;
	DWORD dwPrice = GLGaeaClient::GetInstance().GetCharacter()->GetItemMixMoney();

	if ( lnGold >= (LONGLONG)dwPrice ) m_bIsOK = true;

	if ( dwPrice == UINT_MAX ) 
	{
		CInnerInterface::GetInstance().PrintMsgText ( NS_UITEXTCOLOR::DISABLE, ID2GAMEINTEXT("EMITEM_MIX_FB_NOMIX") );
		SetItemMixResult( ID2GAMEINTEXT("EMITEM_MIX_FB_NOMIX") );
		return;
	}
	if ( dwPrice > 0 )
	{
		CString strTemp;
		strTemp.Format( ID2GAMEINTEXT("ITEM_MIX_MONEY_OKQUESTION"), dwPrice );
		DoModal( strTemp, MODAL_QUESTION, YESNO, MODAL_ITEMMIX_OKQUESTION );
	}
	else
	{
		DoModal( ID2GAMEINTEXT("ITEM_MIX_OKQUESTION"), MODAL_QUESTION, YESNO, MODAL_ITEMMIX_OKQUESTION );
	}
	
}
bool CItemMixWindow::SetItemMixResult( CString strMsg, bool bSuccess, bool bFail )
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
void CItemMixWindow::LoadItemList( WORD wType )
{
	m_pListText->ClearText ();

	static CString strKeyword[] =
	{
		"ICON_OK",
		"ICON_INC",
		"ICON_NONE"
	};

	std::vector<GLCOMPOUND>::size_type i = 0, j = GLCONST_CHAR::vecCOMPOUND.size();
	for( ; i < j; ++i )
	{
		GLCOMPOUND sCompound = GLCONST_CHAR::vecCOMPOUND[i];

		SITEM* pItem = GLItemMan::GetInstance().GetItem( sCompound.sNativeID );
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
		m_pListText->SetTextData ( nIndex, sCompound.dwID );

		m_pListText->SetCurLine ( 0 );

		const int nTotal = m_pListText->GetCount ();
		const int nViewPerPage = m_pListText->GetVisibleLine ();	
		m_pListScrollBar->GetThumbFrame()->SetState ( nTotal, nViewPerPage );
		m_pListScrollBar->GetThumbFrame()->SetPercent ( 0.0f );
	}
}
void CItemMixWindow::ShowButton()
{
	m_pGenerateButton->SetVisibleSingle( TRUE );
}
void CItemMixWindow::HideButton()
{
	m_pGenerateButton->SetVisibleSingle( FALSE );
}