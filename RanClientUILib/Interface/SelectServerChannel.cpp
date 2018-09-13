#include "StdAfx.h"
#include "SelectServerChannel.h"
#include "BasicTextButton.h"
#include "GameTextControl.h"
#include "BasicTextBoxEx.h"
#include "BasicScrollBarEx.h"
#include "../EngineUIlib/GUInterface/BasicScrollThumbFrame.h"
#include "UITextControl.h"
#include "../EngineLib/DxCommon/DxFontMan.h"
#include "ModalWindow.h"
#include "OuterInterface.h"
#include "s_NetClient.h"
#include "DxGlobalStage.h"
#include "RANPARAM.h"
#include "BasicLineBoxEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const	float	CSelectServerChannel::fLIST_PART_INTERVAL = 10.0f;
const D3DCOLOR	CSelectServerChannel::dwCLICKCOLOR = D3DCOLOR_ARGB(140,255,255,255);
const	int		CSelectServerChannel::nOUTOFRANGE = -1;
const	float	CSelectServerChannel::fNORMAL_PERCENT = 0.4f;
const	float	CSelectServerChannel::fMAX_PERCENT = 0.7f;

CSelectServerChannel::CSelectServerChannel () :
	m_nIndex ( nOUTOFRANGE )
{
}

CSelectServerChannel::~CSelectServerChannel ()
{
}

CBasicTextButton*	 CSelectServerChannel::CreateTextButton ( char* szButton, UIGUID ControlID, char* szText )
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

void CSelectServerChannel::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );

		CreateControl ( "TRANG_CHON_MAYCHU_BG" );
//		CreateControl ( "TRANG_CHON_MAYCHU_DANHSACH_BG" );
		CreateControl ( "TRANG_CHON_MAYCHU_THANHCUON_BG" );
	{	
		m_pGate = new CUIControl;
		m_pGate->CreateSub ( this, "TRANG_CHON_MAYCHU_DANHSACH_BG", UI_FLAG_DEFAULT, TRANG_CHON_MAYCHU_DANHSACH_BG );
		m_pGate->SetTransparentOption( TRUE );
		m_pGate->SetVisibleSingle(FALSE);
		RegisterControl ( m_pGate );
	}
	
	{	// den Xam Background
		CBasicLineBoxEx* pBasicLineBoxEx = new CBasicLineBoxEx;
		pBasicLineBoxEx->CreateSub ( this, "BLACKCATYB_DEN_XAM_GIUA", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBoxEx->BlackCatYBDenXamBox ( "TRANG_CHON_MAYCHU_EXIT_BG" );
		RegisterControl ( pBasicLineBoxEx );
	}
	
	CreateTextButton ( "SELECT_CHANNEL_CONNECT", SELECT_SERVER_CONNECT, (char*)ID2GAMEWORD( "SELECT_SERVER_CONNECTQUIT", 0 ) );

/*	CBasicTextButton* pOKButton = new CBasicTextButton;
	pOKButton->CreateSub ( this, "BASIC_TEXT_BUTTON18", UI_FLAG_XSIZE, SELECT_SERVER_CONNECT );
	pOKButton->CreateBaseButton("SELECT_CHANNEL_CONNECT", 
								CBasicTextButton::SIZE18, 
								CBasicButton::CLICK_FLIP, 
								(char*)ID2GAMEWORD ( "SELECT_SERVER_CONNECTQUIT", 0 ) );
	pOKButton->SetShortcutKey ( DIK_RETURN, DIK_NUMPADENTER );
	RegisterControl ( pOKButton );*/

	{
		m_pServerList = new CBasicTextBoxEx;
		m_pServerList->CreateSub ( this, "DANHSACH_CHON_MAYCHU", UI_FLAG_DEFAULT, SELECT_SERVER_LIST );
		m_pServerList->SetFont ( pFont9 );
		m_pServerList->SetTextAlign ( TEXT_ALIGN_LEFT | TEXT_ALIGN_TOP );
		m_pServerList->SetPartInterval ( fLIST_PART_INTERVAL );
//		m_pServerList->MouseClick ( "DANHSACH_CHON_MAYCHU_BG" );
		m_pServerList->CreateMouseClick ( dwCLICKCOLOR );
		m_pServerList->CreateMouseOver ( dwCLICKCOLOR );
		m_pServerList->SetSensitive ( true );
		m_pServerList->SetSensitiveView ( false );
		RegisterControl ( m_pServerList );
	}

	{
		m_pServerState = new CBasicTextBox;
		m_pServerState->CreateSub ( this, "DANHSACH_CHON_MAYCHU_TRANGTHAI" );
		m_pServerState->SetFont ( pFont9 );
		m_pServerState->SetTextAlign ( TEXT_ALIGN_RIGHT | TEXT_ALIGN_TOP );
		m_pServerState->SetPartInterval ( fLIST_PART_INTERVAL );
		RegisterControl ( m_pServerState );
	}

//	CreateControl ( "SELECT_SERVER_BOTTOM" );
}

void CSelectServerChannel::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case SELECT_SERVER_CONNECT:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				AddMessageEx ( UIMSG_MOUSEIN_LOGIN );
			}
		}
		break;

	case SELECT_SERVER_LIST:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				int nIndex = m_pServerList->GetSelectPos ();				
				if ( nIndex < 0 || m_pServerList->GetCount () <= nIndex ) return ;

				if ( dwMsg & UIMSG_LB_UP )
				{
					m_nIndex = nIndex;
				}

				if ( dwMsg & UIMSG_LB_DUP )
				{
					AddMessageEx ( UIMSG_MOUSEIN_LOGIN );
				}
			}
		}
		break;
	}
}

void CSelectServerChannel::ResetAll ()
{
	if ( m_pServerList )	m_pServerList->ClearText ();
	if ( m_pServerState )	m_pServerState->ClearText ();
	
	m_pGate->SetVisibleSingle(FALSE);

	m_nIndex = nOUTOFRANGE;
}

void CSelectServerChannel::AddChannel(	const CString& strChannelName, 
										const DWORD dwColor, 
										const CString& strState, 
										const DWORD dwStateColor, 
										const DWORD dwChannel )
{
	m_pGate->SetVisibleSingle(TRUE);
	int nIndex = m_pServerList->AddText ( strChannelName, dwColor );
	m_pServerList->SetTextData ( nIndex, dwChannel );
	m_pServerState->AddText ( strState, dwStateColor );
}

DWORD CSelectServerChannel::GetSelectedChannel ( const int nIndex )
{
	m_pGate->SetVisibleSingle(FALSE);
	return m_pServerList->GetTextData ( nIndex );
}