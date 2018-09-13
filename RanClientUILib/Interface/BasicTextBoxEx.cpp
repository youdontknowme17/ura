#include "StdAfx.h"
#include "BasicTextBoxEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int CBasicTextBoxEx::nNOT_VISIBLE = -1;

CBasicTextBoxEx::CBasicTextBoxEx () :
	m_pMouseOver ( NULL ),
//	m_wFlipMode(NOT_READY),
	m_pMouseOverImage ( NULL ),
	m_pMouseOverImage0 ( NULL ),
	m_pMouseClickImage ( NULL ),
	m_pMouseClickImage0 ( NULL ),
	m_bSensitive ( false ),
	m_bSensitiveView ( false ),
	m_nSELECTED_INDEX ( nNOT_VISIBLE ),
	m_bSelectInit( true )
{
}

CBasicTextBoxEx::~CBasicTextBoxEx ()
{
}

void CBasicTextBoxEx::CreateMouseClick ( D3DCOLOR dwMouseOverLineColor )
{
	CUIControl* pMouseOverLine = new CUIControl;
	pMouseOverLine->CreateSub ( this, "BASIC_TEXTBOX_MOUSEOVER_LINE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pMouseOverLine->SetVisibleSingle ( FALSE );
	pMouseOverLine->SetUseRender ( TRUE );	
	RegisterControl ( pMouseOverLine );
	m_pMouseClickImage = pMouseOverLine;

	m_dwMouseOverImageColor = dwMouseOverLineColor;
	
	CUIControl* pMouseOverLine0 = new CUIControl;
	pMouseOverLine0->CreateSub ( this, "BASIC_TEXTBOX_MOUSE_CLICK_BG", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pMouseOverLine0->SetVisibleSingle ( FALSE );
	pMouseOverLine0->SetUseRender ( TRUE );	
	RegisterControl ( pMouseOverLine0 );
	m_pMouseClickImage0 = pMouseOverLine0;
}

void CBasicTextBoxEx::CreateMouseOver ( D3DCOLOR dwMouseOverLineColor )
{
	CUIControl* pMouseOverLine = new CUIControl;
	pMouseOverLine->CreateSub ( this, "BASIC_TEXTBOX_MOUSEOVER_LINE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pMouseOverLine->SetVisibleSingle ( FALSE );
	pMouseOverLine->SetUseRender ( TRUE );	
	RegisterControl ( pMouseOverLine );
	m_pMouseOverImage = pMouseOverLine;

	m_dwMouseOverImageColor = dwMouseOverLineColor;
	
	CUIControl* pMouseOverLine0 = new CUIControl;
	pMouseOverLine0->CreateSub ( this, "BASIC_TEXTBOX_MOUSEOVER_BG", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pMouseOverLine0->SetVisibleSingle ( FALSE );
	pMouseOverLine0->SetUseRender ( TRUE );	
	RegisterControl ( pMouseOverLine0 );
	m_pMouseOverImage0 = pMouseOverLine0;
}

void CBasicTextBoxEx::MouseClick ( char* KeyValue )
{
/*	if ( m_wFlipMode == NOT_READY )
	{
		GASSERT ( 0 && "CreateFlip() ?? ??? ???? ???." );
		return ;
	}
	if ( m_wFlipMode == MOUSEIN_FLIP )
	{
		GASSERT ( 0 && "???? ????? ?? ??? ?????? ?? ??? ??? ????." );
		return ;
	}*/

	CUIControl* pMouseOver = new CUIControl;
	pMouseOver->CreateSub ( this, KeyValue );
	pMouseOver->SetVisibleSingle ( FALSE );
	RegisterControl ( pMouseOver );
	m_pMouseOver = pMouseOver;
}

void	CBasicTextBoxEx::SetSensitive ( const bool& bSensitive )
{
	m_bSensitive = bSensitive;

	SetSensitiveView ( bSensitive );
}

void	CBasicTextBoxEx::SetSelectInit( const bool& bSelectInit ) 
{
	m_bSelectInit = bSelectInit; 
}

void	CBasicTextBoxEx::SetSensitiveView ( const bool& bSensitive )
{
	m_bSensitiveView = bSensitive;
}

void	CBasicTextBoxEx::UpdateMousePos ( int x, int y )
{
	if( m_bSelectInit ) m_nMouseOverLine = nNOT_VISIBLE;

	DWORD dwMsg = GetMessageEx ();	
	if ( !CHECK_MOUSE_IN ( dwMsg ) ) return ;

	const UIRECT& rcGlobalPos = GetGlobalPos ();

	int nPartIndex, nLineIndex;
	FindStartIndex ( nPartIndex, nLineIndex, GetCurLine() );

	for ( int i = nPartIndex; i < GetCount(); i++ )
	{
		UIRECT rcPart;
		GetPartInfo ( i, rcPart );

		if ( !m_bSensitive )
		{
			rcPart = UIRECT ( rcGlobalPos.left, rcPart.top,
				rcGlobalPos.sizeX, rcPart.sizeY );
		}

		if ( (x >rcPart.left && x<rcPart.right) &&
			(y> rcPart.top && y<rcPart.bottom) )
		{
			m_nMouseOverLine = i;
			return ;
		}
	}		
}

void CBasicTextBoxEx::UpdateMouseClickImage ()
{
	if ( m_nMouseOverLine < GetTotalLine () )
	{
		DWORD dwMsg = GetMessageEx ();
		if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
		{
			m_nSELECTED_INDEX = m_nMouseOverLine;
		}		

		if ( m_nSELECTED_INDEX == nNOT_VISIBLE ) return ;

		int nCurLine = GetCurLine ();
		int nVisibleLine = GetVisibleLine ();
		if ( m_nSELECTED_INDEX < nCurLine || (nCurLine + nVisibleLine) < m_nSELECTED_INDEX )
		{
			m_pMouseClickImage->SetVisibleSingle ( FALSE );
			m_pMouseClickImage0->SetVisibleSingle ( FALSE );
			
			return ;
		}

		UIRECT rcMousePos;
		if ( !GetPartInfo ( m_nSELECTED_INDEX, rcMousePos ) ) return ;

		if ( !m_bSensitiveView )
		{
			const UIRECT& rcGlobalPos = GetGlobalPos ();
			rcMousePos = UIRECT ( rcGlobalPos.left, rcMousePos.top,
				rcGlobalPos.sizeX, rcMousePos.sizeY );
		}

		m_pMouseClickImage->SetGlobalPos ( rcMousePos );
		m_pMouseClickImage->SetVisibleSingle ( TRUE );
		m_pMouseClickImage0->SetVisibleSingle ( TRUE );
		m_pMouseClickImage->SetDiffuse ( m_dwMouseOverImageColor );	
	}
}

void CBasicTextBoxEx::UpdateMouseOverImage ( )
{	
	if ( m_nMouseOverLine == nNOT_VISIBLE ) return ;
	if ( m_nMouseOverLine < GetTotalLine () )
	{
		UIRECT rcMousePos;
		if ( !GetPartInfo ( m_nMouseOverLine, rcMousePos ) ) return ;

		if ( !m_bSensitive )
		{
			const UIRECT& rcGlobalPos = GetGlobalPos ();
			rcMousePos = UIRECT ( rcGlobalPos.left, rcMousePos.top,
				rcGlobalPos.sizeX, rcMousePos.sizeY );	
		}

		m_pMouseOverImage->SetGlobalPos ( rcMousePos );
		m_pMouseOverImage0->SetVisibleSingle ( TRUE );
		if ( m_pMouseOverImage ) 
		{
			m_pMouseOverImage->SetVisibleSingle ( FALSE );
			m_pMouseOverImage->SetDiffuse ( m_dwMouseOverImageColor );
		}
	}
}

void CBasicTextBoxEx::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	CBasicTextBox::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	if( m_bSelectInit ) m_nMouseOverLine = nNOT_VISIBLE;
	if ( m_pMouseOverImage ) m_pMouseOverImage->SetVisibleSingle ( FALSE );    
	if ( m_pMouseOverImage0 ) m_pMouseOverImage0->SetVisibleSingle ( FALSE );    

	if ( bFirstControl )
	{
		UpdateMousePos ( x, y );
		if ( m_pMouseOverImage ) UpdateMouseOverImage ();
		if ( m_pMouseOverImage0 ) UpdateMouseOverImage ();
		if ( m_pMouseClickImage ) UpdateMouseClickImage ();
		if ( m_pMouseClickImage0 ) UpdateMouseClickImage ();
		CDebugSet::ToView ( 1, 5, "m_nMouseOverLine : %d", m_nMouseOverLine/*GetSelectPos()*/ );
	}	
}

int	 CBasicTextBoxEx::GetSelectPos ()
{
	return m_nMouseOverLine;
}

void	CBasicTextBoxEx::ClearText ()
{
	CBasicTextBox::ClearText ();
	m_nMouseOverLine = nNOT_VISIBLE;
	m_nSELECTED_INDEX = nNOT_VISIBLE;

	if ( m_pMouseOverImage ) m_pMouseOverImage->SetVisibleSingle ( FALSE );
	if ( m_pMouseOverImage0 ) m_pMouseOverImage0->SetVisibleSingle ( FALSE );
	if ( m_pMouseClickImage ) m_pMouseClickImage->SetVisibleSingle ( FALSE );
	if ( m_pMouseClickImage0 ) m_pMouseClickImage0->SetVisibleSingle ( FALSE );
}