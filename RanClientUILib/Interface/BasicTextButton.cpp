#include "StdAfx.h"
#include "BasicTextButton.h"
#include "BasicTextButtonImage.h"
#include "../EngineUIlib/GUInterface/BasicTextBox.h"
#include "UITextControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CBasicTextButton::CBasicTextButton ()
{
}

CBasicTextButton::~CBasicTextButton ()
{
}

void CBasicTextButton::SetRadioButton ()
{
	SetUseDynamic ( FALSE );

	m_TextColor = NS_UITEXTCOLOR::DARKGRAY;
	m_TextFlipColor = NS_UITEXTCOLOR::WHITE;
}

void CBasicTextButton::SetGeneralButton ()
{
	SetUseDynamic ( TRUE );

	m_TextColor = NS_UITEXTCOLOR::WHITE;
	m_TextFlipColor = NS_UITEXTCOLOR::GOLD;
}

void CBasicTextButton::SetFlipPosition ()
{
	if ( m_pButtonFlip )
	{
		if ( !IsFlip () )
		{
			const UIRECT& rcBackPos = m_pButton->GetGlobalPos ();
			m_pButtonFlip->SetGlobalPos ( D3DXVECTOR2 ( rcBackPos.left + fBUTTON_CLICK, rcBackPos.top + fBUTTON_CLICK ) );

			const UIRECT& rcBackTextPos = m_pButtonTextBack->GetGlobalPos ();
			m_pButtonText->SetGlobalPos ( D3DXVECTOR2 ( rcBackTextPos.left + fBUTTON_CLICK, rcBackTextPos.top + fBUTTON_CLICK ) );
		}
	}
}

void CBasicTextButton::ResetFlipPosition ()
{
	if ( m_pButtonFlip )
	{
		if ( IsFlip () )
		{
			const UIRECT& rcBackPos = m_pButton->GetGlobalPos ();
			m_pButtonFlip->SetGlobalPos ( D3DXVECTOR2 ( rcBackPos.left, rcBackPos.top ) );

			const UIRECT& rcBackTextPos = m_pButtonTextBack->GetGlobalPos ();
			m_pButtonText->SetGlobalPos ( D3DXVECTOR2 ( rcBackTextPos.left, rcBackTextPos.top ) );
		}
	}
}

void CBasicTextButton::SetFlip ( BOOL bFlip )
{
	if ( m_pButton )	m_pButton->SetVisibleSingle ( !bFlip );
	if ( m_pButtonFlip )m_pButtonFlip->SetVisibleSingle ( bFlip );

	if ( m_pButtonText )
	{
		m_pButtonText->SetUseOverColor ( bFlip );
		
		if ( bFlip )	 m_pButtonText->SetOverColor ( 0, m_TextFlipColor );
		else			 m_pButtonText->SetOverColor ( 0, m_TextColor );
	}

	if ( IsUseDynamic() )
	{
		if ( bFlip )
		{
			SetFlipPosition ();
		}
	}	

	m_bFlip = bFlip;


}

void CBasicTextButton::SetFlip2 ( BOOL bFlip )
{
	if ( m_pButton )	m_pButton->SetVisibleSingle ( !bFlip );
	if ( m_pButtonFlip )m_pButtonFlip->SetVisibleSingle ( bFlip );

	if ( m_pButtonText )
	{
		m_pButtonText->SetUseOverColor ( bFlip );
		
		if ( bFlip )	 m_pButtonText->SetOverColor ( 0, m_TextFlipColor );
		else			 m_pButtonText->SetOverColor ( 0, m_TextColor );
	}

	if ( IsUseDynamic() )
	{
		if ( bFlip )
		{
			SetFlipPosition ();
		}
	}	

	m_bFlip = bFlip;

	m_TextColor = NS_UITEXTCOLOR::WHITE;
	m_TextFlipColor = NS_UITEXTCOLOR::BLACK;

}

void CBasicTextButton::SetFlipBlack ( BOOL bFlip )
{
	if ( m_pButton )	m_pButton->SetVisibleSingle ( !bFlip );
	if ( m_pButtonFlip )m_pButtonFlip->SetVisibleSingle ( bFlip );

	if ( m_pButtonText )
	{
		m_pButtonText->SetUseOverColor ( bFlip );
		
		if ( bFlip )	 m_pButtonText->SetOverColor ( 0, m_TextFlipColor );
		else			 m_pButtonText->SetOverColor ( 0, m_TextColor );
	}

	if ( IsUseDynamic() )
	{
		if ( bFlip )
		{
			SetFlipPosition ();
		}
	}	

	m_bFlip = bFlip;

	m_TextColor = NS_UITEXTCOLOR::WHITE;
	m_TextFlipColor = NS_UITEXTCOLOR::WHITE;

}
void CBasicTextButton::SetFlipYellow ( BOOL bFlip )
{
	if ( m_pButton )	m_pButton->SetVisibleSingle ( !bFlip );
	if ( m_pButtonFlip )m_pButtonFlip->SetVisibleSingle ( bFlip );

	if ( m_pButtonText )
	{
		m_pButtonText->SetUseOverColor ( bFlip );
		
		if ( bFlip )	 m_pButtonText->SetOverColor ( 0, m_TextFlipColor );
		else			 m_pButtonText->SetOverColor ( 0, m_TextColor );
	}

	if ( IsUseDynamic() )
	{
		if ( bFlip )
		{
			SetFlipPosition ();
		}
	}	

	m_bFlip = bFlip;

	m_TextColor = NS_UITEXTCOLOR::WHITE;
	m_TextFlipColor = NS_UITEXTCOLOR::GOLD;

}



HRESULT CBasicTextButton::Render ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	//	�ǵ������� CBasicButton::Render�� ������ ���Դϴ�.
	// MEMO : CBasicButton::Render ����.
	return CUIGroup::Render ( pd3dDevice );
}

void CBasicTextButton::SetAlignText( int Align )
{
	if ( m_pButtonText ) m_pButtonText->SetTextAlign( Align );
}
void CBasicTextButton::SetOneLineText ( CString strTemp, const D3DCOLOR& TextColor )
{
	if ( TextColor )
		if ( m_pButtonText ) m_pButtonText->SetOneLineText ( strTemp ,TextColor );
	else
		if ( m_pButtonText ) m_pButtonText->SetOneLineText ( strTemp );
}

void CBasicTextButton::SetRadioButtonEx ()
{
	SetUseDynamic ( FALSE );

	m_TextColor = NS_UITEXTCOLOR::BLACK;
	m_TextFlipColor = NS_UITEXTCOLOR::BLACK;
}

void CBasicTextButton::SetGeneralButtonEx ()
{
	SetUseDynamic ( TRUE );

	m_TextColor = NS_UITEXTCOLOR::BLACK;
	m_TextFlipColor = NS_UITEXTCOLOR::BLACK;
}
void CBasicTextButton::CreateTextBox( char* szControlKeyword, CD3DFontPar* pFont, int nAlign )
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub ( this, szControlKeyword, UI_FLAG_DEFAULT );
	pStaticText->SetFont ( pFont );
	pStaticText->SetTextAlign ( nAlign );	
	RegisterControl ( pStaticText );
	m_pButtonText = pStaticText;
}