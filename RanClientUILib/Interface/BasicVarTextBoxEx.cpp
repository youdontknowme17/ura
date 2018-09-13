#include "StdAfx.h"

#include "./BasicVarTextBoxEx.h"
#include "../EngineUIlib/GUInterface/BasicTextBox.h"
#include "../Interface/BasicTextBoxEx.h"
#include "../Interface/BasicLineBoxEx.h"
#include "../EngineLib/DxCommon/DxFontMan.h"
#include "../Interface/UITextControl.h"
#include "../EngineUIlib/GUInterface/UIDebugSet.h"
#include "../Interface/ItemImage.h"
#include "GLItemMan.h"
#include "GLItem.h"
#include "GLGaeaClient.h"
#include "../EngineUIlib/GUInterface/BasicButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const	float	CBasicVarTextBoxEx::fMOUSEPOINT_GAP = 20.0f;

CBasicVarTextBoxEx::CBasicVarTextBoxEx () :
	m_pTextBox ( NULL ),
	m_pLineBox ( NULL ),
	m_pLineBoxGray ( NULL ),
	m_pFont ( NULL ),
	m_pSelfDummy ( NULL ),
	m_vMousePointGap (fMOUSEPOINT_GAP,fMOUSEPOINT_GAP),
	m_bBLOCK_MOUSETRACKING ( false ),
	m_pCloseButton ( NULL ),
	m_pItemImage( NULL ), //Item Texture
	m_pTextBoxName ( NULL ) //Item Name
{
	memset ( m_pBoxImg, 0, sizeof ( CUIControl* ) * ITEM::SBOX::ITEM_SIZE );	
	memset ( m_pRandBoxImg, 0, sizeof ( CUIControl* ) * 30 );	
}

CBasicVarTextBoxEx::~CBasicVarTextBoxEx ()
{
}

CItemImage*	CBasicVarTextBoxEx::CreateItemImage ( const char* szControl )
{
	CItemImage* pItemImage = new CItemImage;
	pItemImage->CreateSub ( this, szControl, UI_FLAG_DEFAULT );
	pItemImage->CreateSubControl ();
	pItemImage->SetUseRender ( TRUE );
	pItemImage->SetVisibleSingle ( FALSE );
	RegisterControl ( pItemImage );

	return pItemImage;
}

void CBasicVarTextBoxEx::CreateSubControl ()
{
	m_pFont = DxFontMan::GetInstance().LoadDxFont ( _BOLD_FONT, 9, _DEFAULT_FONT_FLAG );
	m_pFont12 = DxFontMan::GetInstance().LoadDxFont ( _BOLD_FONT, 12, _DEFAULT_FONT_FLAG );

	CBasicLineBoxEx* pLineBox = new CBasicLineBoxEx;
	pLineBox->CreateSub ( this, "BASIC_VAR_LINE_BOX", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxVarTextBox ( "BASIC_VAR_LINE_BOX" );
	pLineBox->SetNoUpdate ( true );
	RegisterControl ( pLineBox );
	m_pLineBox = pLineBox;
	
	CBasicLineBoxEx* pLineBoxGray = new CBasicLineBoxEx;
	pLineBoxGray->CreateSub ( this, "BASIC_VAR_LINE_BOX", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBoxGray->CreateBaseBoxVarTextBoxGreen ( "BASIC_VAR_LINE_BOX" );
	pLineBoxGray->SetNoUpdate ( true );
	RegisterControl ( pLineBoxGray );
	m_pLineBoxGray = pLineBoxGray;

	CUIControl* pSelfDummy = new CUIControl;
	pSelfDummy->CreateSub ( this, "BASIC_VAR_LINE_BOX" );
	pSelfDummy->SetVisibleSingle ( FALSE );
	RegisterControl ( pSelfDummy );
	m_pSelfDummy = pSelfDummy;

	CBasicTextBoxEx* pTextBox = new CBasicTextBoxEx;
	pTextBox->CreateSub ( this, "VAR_TEXT_BOX", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pTextBox->SetFont ( m_pFont );
	pTextBox->SetTextAlign ( TEXT_ALIGN_LEFT );
	pTextBox->SetNoUpdate ( true );
	RegisterControl ( pTextBox );
	m_pTextBox = pTextBox;
	m_pTextBox->ClearText();

	CItemImage* pItemImage = new CItemImage;
	pItemImage->CreateSub ( this, "ITEM_IMAGE_TEXTURE" );
	pItemImage->CreateSubControl ();
	pItemImage->SetUseRender ( TRUE );
	pItemImage->SetVisibleSingle ( FALSE );
	RegisterControl ( pItemImage );
	m_pItemImage = pItemImage;

	CBasicTextBox* pTextBoxName = new CBasicTextBox;
	pTextBoxName->CreateSub ( this, "VAR_TEXT_BOX", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pTextBoxName->SetFont ( m_pFont12 );
	pTextBoxName->SetTextAlign ( TEXT_ALIGN_LEFT );
	pTextBoxName->SetNoUpdate ( true );
	RegisterControl ( pTextBoxName );
	m_pTextBoxName = pTextBoxName;

	{
		for ( int i = 0; i < ITEM::SBOX::ITEM_SIZE; i++ )
		{
			CString strBox;
			strBox.Format ( "ITEM_BOX_IMAGE%d", i );
			m_pBoxImg[i] = CreateItemImage (strBox.GetString());
		}

		for ( int i = 0; i < 30; i++ )
		{
			CString strRandomBox;
			strRandomBox.Format ( "ITEM_BOXRANDOM_IMAGE%d", i );
			m_pRandBoxImg[i] = CreateItemImage (strRandomBox.GetString());
		}
	}

	m_pCloseButton = new CBasicButton;
	m_pCloseButton->CreateSub ( this, "THONGTIN_HIENTHI_DUONGDAN_DONG" , UI_FLAG_DEFAULT, BUTTON_CLOSE );
	m_pCloseButton->CreateFlip ( "THONGTIN_HIENTHI_DUONGDAN_DONG_F", CBasicButton::CLICK_FLIP );
	m_pCloseButton->CreateMouseOver( "THONGTIN_HIENTHI_DUONGDAN_DONG_KETTHUC" );
	m_pCloseButton->SetUseGlobalAction ( TRUE );
	m_pCloseButton->SetProtectSizePoint ();
//	m_pCloseButton->SetVisibleSingle ( FALSE );
	RegisterControl ( m_pCloseButton );
}

void CBasicVarTextBoxEx::SetTextAlign ( int nALIGN )
{
	m_pTextBox->SetTextAlign ( nALIGN );
}

void CBasicVarTextBoxEx::ClearText ()
{
	if ( m_pTextBox ) m_pTextBox->ClearText ();
	if ( m_pTextBoxName ) m_pTextBoxName->ClearText ();
}

int CBasicVarTextBoxEx::AddTextNoSplit ( CString strText, D3DCOLOR dwColor )
{
	if ( m_pTextBox ) return m_pTextBox->AddTextNoSplit ( strText, dwColor );

	return -1;
}

int	CBasicVarTextBoxEx::SetTextNoSplit ( CString strText, D3DCOLOR dwColor )
{
	if ( m_pTextBox ) return m_pTextBox->SetTextNoSplit ( strText, dwColor );

	return -1;
}

void CBasicVarTextBoxEx::SetText( CString strText, D3DCOLOR dwColor )
{
	if ( m_pTextBox ) m_pTextBox->SetText( strText, dwColor );
}

void CBasicVarTextBoxEx::AddString ( int nIndex, const CString& strText, const D3DCOLOR& dwColor )
{
	if ( m_pTextBox ) m_pTextBox->AddString ( nIndex, strText, dwColor );
}

int CBasicVarTextBoxEx::AddText ( CString strText, D3DCOLOR dwColor )
{
	if ( m_pTextBox ) return m_pTextBox->AddText ( strText, dwColor );

	return -1;
}

void CBasicVarTextBoxEx::SetUseOverColor ( bool bUseColor )
{
	m_pTextBox->SetUseOverColor ( (bUseColor)?TRUE:FALSE );
}

void CBasicVarTextBoxEx::SetOverColor ( const D3DCOLOR& dwColor )
{
	int nIndex = m_pTextBox->GetCount () - 1;
	m_pTextBox->SetOverColor ( nIndex, dwColor );
}

void CBasicVarTextBoxEx::RePosControl ( int x, int y )
{
	const float fTextBoxGapX_ = (float)m_pTextBox->GetLongestLine ();
	const float fTextBoxGapXName_ = (float)m_pTextBoxName->GetLongestLine ();

	if ( fTextBoxGapXName_ > fTextBoxGapX_ )
	{
		const UIRECT& rcTextBoxLocalPos = m_pTextBoxName->GetLocalPos ();
		const UIRECT& rcLocalPosDummy = m_pSelfDummy->GetLocalPos ();
		const UIRECT& rcOriginPos = GetLocalPos ();

		AlignSubControl ( rcOriginPos, rcLocalPosDummy );

		if ( m_pTextBoxName )
		{
			int nTotalLine = m_pTextBox->GetTotalLine ();
			const float fAllLine = m_pTextBox->CalcMaxHEIGHT ( 0, nTotalLine );
			const float fTextBoxGapX = (float)m_pTextBoxName->GetLongestLine () - rcTextBoxLocalPos.sizeX;
			const float fTextBoxGapY = fAllLine - rcTextBoxLocalPos.sizeY;
			
			UIRECT rcLocalNewPos = UIRECT ( float(x) + m_vMousePointGap.x, float(y) + m_vMousePointGap.y,
				rcOriginPos.sizeX + fTextBoxGapX + 30.0f, rcOriginPos.sizeY + fTextBoxGapY );

			AlignSubControl ( rcLocalPosDummy, rcLocalNewPos );

			SetGlobalPos ( rcLocalNewPos );		
		}
	}
	else
	{
		const UIRECT& rcTextBoxLocalPos = m_pTextBox->GetLocalPos ();
		const UIRECT& rcLocalPosDummy = m_pSelfDummy->GetLocalPos ();
		const UIRECT& rcOriginPos = GetLocalPos ();

		AlignSubControl ( rcOriginPos, rcLocalPosDummy );

		if ( m_pTextBox )
		{
			int nTotalLine = m_pTextBox->GetTotalLine ();
			const float fAllLine = m_pTextBox->CalcMaxHEIGHT ( 0, nTotalLine );
			const float fTextBoxGapX = (float)m_pTextBox->GetLongestLine () - rcTextBoxLocalPos.sizeX;
			const float fTextBoxGapY = fAllLine - rcTextBoxLocalPos.sizeY;
			
			UIRECT rcLocalNewPos = UIRECT ( float(x) + m_vMousePointGap.x, float(y) + m_vMousePointGap.y,
				rcOriginPos.sizeX + fTextBoxGapX + 30.0f, rcOriginPos.sizeY + fTextBoxGapY );

			AlignSubControl ( rcLocalPosDummy, rcLocalNewPos );

			SetGlobalPos ( rcLocalNewPos );		
		}
	}
}

void CBasicVarTextBoxEx::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	if ( m_pItemImage->IsVisible())
	{
		m_pLineBoxGray->SetVisibleSingle ( TRUE );
		m_pLineBox->SetVisibleSingle ( FALSE );
	}
	else
	{
		m_pLineBoxGray->SetVisibleSingle ( FALSE );
		m_pLineBox->SetVisibleSingle ( TRUE );
	}

	SetUseOverColor ( false );
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if ( !m_bBLOCK_MOUSETRACKING ) RePosControl ( x, y );	

	if ( IsClose())
	{
		if ( m_pCloseButton )
		{
			const UIRECT& rcOriginPos1 = m_pTextBox->GetLocalPos();
			const UIRECT& rcOriginPos2 = m_pCloseButton->GetLocalPos();
			float fx = rcOriginPos1.right - rcOriginPos2.sizeX;
			float fy = rcOriginPos1.top;
			m_pCloseButton->SetLocalPos( D3DXVECTOR2 ( fx , fy ));
			SetClose(FALSE);
		}
	}
}

void CBasicVarTextBoxEx::SetMousePointGap ( D3DXVECTOR2 vGap )
{
	m_vMousePointGap = vGap;
}

float CBasicVarTextBoxEx::GetLongestLine ()
{
	if ( m_pTextBox )
	{
		return (float)m_pTextBox->GetLongestLine ();
	}
	return 0.0f;
}

void CBasicVarTextBoxEx::AddTextLongestLineSplit ( CString strText, D3DCOLOR dwColor )
{
	//float fLongestLine = GetLongestLine ();
	//float fLongLine = float(m_pTextBoxName->GetLongestLine()) + 40.0f;

//	if ( IsLong())
//	if ( fLongLine > fLongestLine )
	const float fTextBoxGapX_ = (float)m_pTextBox->GetLongestLine ();
	const float fTextBoxGapXName_ = (float)m_pTextBoxName->GetLongestLine ();

	if ( fTextBoxGapXName_ > fTextBoxGapX_ )
	{
		NS_UITEXTCONTROL::MULTILINETEXT& MLTextSet = NS_UITEXTCONTROL::GetMLTextWithoutCRLF ( strText, fTextBoxGapXName_, m_pFont12 );

		for ( int i = 0; i < (int)MLTextSet.size(); ++i )
		{
			CString strTemp = MLTextSet[i].strLine;
			if ( strTemp.GetLength() ) AddTextNoSplit ( strTemp, dwColor );		
		}	
	}
	else
	{
		NS_UITEXTCONTROL::MULTILINETEXT& MLTextSet = NS_UITEXTCONTROL::GetMLTextWithoutCRLF ( strText, fTextBoxGapX_, m_pFont );

		for ( int i = 0; i < (int)MLTextSet.size(); ++i )
		{
			CString strTemp = MLTextSet[i].strLine;
			if ( strTemp.GetLength() ) AddTextNoSplit ( strTemp, dwColor );		
		}
	}
}

int	CBasicVarTextBoxEx::GetCount ()
{
	return m_pTextBox->GetCount ();
}

void CBasicVarTextBoxEx::SetLineInterval ( const float fLineInterval )
{
	m_pTextBox->SetLineInterval ( fLineInterval );
}

void CBasicVarTextBoxEx::SetBlockMouseTracking ( bool bBlock )
{
	m_bBLOCK_MOUSETRACKING = bBlock;
}
//Item Texture
void CBasicVarTextBoxEx::SetItemRender ( SNATIVEID sICONINDEX, const char* szTexture, SITEMCUSTOM sCustom ) 
{
	SITEM* pItem = GLItemMan::GetInstance().GetItem ( sICONINDEX );
	if ( !pItem ) m_pItemImage->SetVisibleSingle ( FALSE );

	if ( m_pItemImage )
	{
		m_pItemImage->SetItem( sICONINDEX, szTexture );
		m_pItemImage->SetVisibleSingle ( TRUE );
	}
	
}
void CBasicVarTextBoxEx::ResetItemRender ()
{
	if ( m_pItemImage )
	{
		m_pItemImage->ResetItem();
		m_pItemImage->SetVisibleSingle ( FALSE );
	}
}
//Item Name
int CBasicVarTextBoxEx::AddTextNoSplitBig ( CString strText, D3DCOLOR dwColor )
{
	if ( m_pTextBoxName ) return m_pTextBoxName->AddTextNoSplitBig ( strText, dwColor );
								 m_pTextBoxName->SetFont ( m_pFont12 );
	return -1;
}

void CBasicVarTextBoxEx::SetItemInBox ( SNATIVEID sICONINDEX, const char* szTexture, int nIndex )
{		
	if ( m_pBoxImg[nIndex] )
	{
		m_pBoxImg[nIndex]->SetItem( sICONINDEX, szTexture );
		m_pBoxImg[nIndex]->SetVisibleSingle ( TRUE );
	}
}

void CBasicVarTextBoxEx::ResetItemInBox ()
{
	for ( int i = 0; i < ITEM::SBOX::ITEM_SIZE; i++ )
	{
		if ( m_pBoxImg[i] )
		{
			m_pBoxImg[i]->ResetItem();
			m_pBoxImg[i]->SetVisibleSingle ( FALSE );
		}
	}
}
void CBasicVarTextBoxEx::SetItemInRandomBox ( SNATIVEID sICONINDEX, const char* szTexture, int nIndex )
{		
	if ( m_pRandBoxImg[nIndex] )
	{
		m_pRandBoxImg[nIndex]->SetItem( sICONINDEX, szTexture );
		m_pRandBoxImg[nIndex]->SetVisibleSingle ( TRUE );
	}
}

void CBasicVarTextBoxEx::ResetItemInRandomBox ()
{
	for ( int i = 0; i < 30; i++ )
	{
		if ( m_pRandBoxImg[i] )
		{
			m_pRandBoxImg[i]->ResetItem();
			m_pRandBoxImg[i]->SetVisibleSingle ( FALSE );
		}
	}
}
void CBasicVarTextBoxEx::ShowCloseButton( bool bSHOW )
{
	if ( m_pCloseButton )
		m_pCloseButton->SetVisibleSingle( bSHOW );
}
void CBasicVarTextBoxEx::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case BUTTON_CLOSE:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				if ( UIMSG_LB_UP & dwMsg )
				{
					AddMessageEx ( UIMSG_UIVARTEXT_BUTTON_CLOSE_CLICK );
				}

				AddMessageEx ( UIMSG_UIVARTEXT_BUTTON_CLOSE_MOUSEIN );
			}
		}break;
	};

	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );
}

//Add MultiText by NaJ
int	CBasicVarTextBoxEx::AddMultiText ( CBasicTextBox::MULTITEXT mlText )
{
	if ( m_pTextBox ) return m_pTextBox->AddMultiText( mlText );

	return -1;
}

//Add MultiTextLongestLine by NaJ
int	CBasicVarTextBoxEx::AddMultiTextLongestLineSplit ( CBasicTextBox::MULTITEXT mlText )
{
	if ( m_pTextBox ) return m_pTextBox->AddMultiTextLongestLine( mlText );

	return -1;
}