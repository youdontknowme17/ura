#include "StdAfx.h"
#include "./UIOuterWindow.h"
#include "./OuterInterface.h"
#include "./BasicLineBox.h"
#include "../EngineUIlib/GUInterface/BasicTextBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CUIOuterWindow::CUIOuterWindow ()
{
}

CUIOuterWindow::~CUIOuterWindow ()
{
}

void CUIOuterWindow::CreateBaseWidnow ( char* szWindowKeyword, char* szWindowName )
{
	CreateTitle ( "CUASO_NGOAI_TIEUDE", "CUASO_NGOAI_TIEUDE_TRAI", "CUASO_NGOAI_TIEUDE_GIUA", "CUASO_NGOAI_TIEUDE_PHAI", "CUASO_NGOAI_TEXTBOX", szWindowName );
	CreateBody ( "CUASO_NGOAI_THAN", "CUASO_NGOAI_THAN_TRAI", "CUASO_NGOAI_THAN_TREN", "CUASO_NGOAI_THAN_GIUA", "CUASO_NGOAI_THAN_DUOI", "CUASO_NGOAI_THAN_PHAI" );	
	
	ResizeControl ( szWindowKeyword );

//	SetTitleAlign ( CBasicTextBox::CENTER );
}

CBasicTextBox*	CUIOuterWindow::CreateStaticControl ( char* szControlKeyword, CD3DFontPar* pFont, D3DCOLOR dwColor, int nAlign )
{
	CBasicTextBox* pStaticText = new CBasicTextBox;
	pStaticText->CreateSub ( this, szControlKeyword );
	pStaticText->SetFont ( pFont );
	pStaticText->SetTextAlign ( nAlign );	
	RegisterControl ( pStaticText );
	return pStaticText;
}

CUIControl*	CUIOuterWindow::CreateControl ( char* szControl )
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, szControl );
	RegisterControl ( pControl );
	return pControl;
}


void CUIOuterWindow::ResizeControl ( char* szWindowKeyword )
{
    CUIControl TempControl;
	TempControl.Create ( 1, szWindowKeyword );
	const UIRECT& rcParentOldPos = GetLocalPos ();
	const UIRECT& rcParentNewPos = TempControl.GetLocalPos ();
	AlignSubControl ( rcParentOldPos, rcParentNewPos );

	SetLocalPos ( D3DXVECTOR2 ( rcParentNewPos.left, rcParentNewPos.top ) );
}

void CUIOuterWindow::TranslateUIMessage( UIGUID ControlID, DWORD dwMsg )
{
	CUIWindow::TranslateUIMessage ( ControlID, dwMsg );

	switch ( ControlID )
	{
	case ET_CONTROL_BUTTON:
		{
			if( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				COuterInterface::GetInstance().HideGroup( GetWndID() );
			}
		}
		break;
	}
}