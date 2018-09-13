// DlgEffCloneBlur.cpp : implementation file
//

#include "stdafx.h"
#include "../RanEditSkinPiece.h"
#include "DlgEffCloneBlur.h"
#include "../RanEditSkinPieceView.h"
#include "DxEffChar.h"
#include "EtcFunction.h"
#include "../ToolTab.h"
#include ".\dlgeffcloneblur.h"

#include "../SelColorDlg.h"
// CDlgEffCloneBlur dialog

IMPLEMENT_DYNAMIC(CDlgEffCloneBlur, CPropertyPage)
CDlgEffCloneBlur::CDlgEffCloneBlur( LOGFONT logfont )
	: CPropertyPage(CDlgEffCloneBlur::IDD)
	, m_pFont( NULL )
	, m_pPiece( NULL )
	, bNEWEFF( FALSE )
{
	m_bDlgInit = FALSE;
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CDlgEffCloneBlur::~CDlgEffCloneBlur()
{
}

void CDlgEffCloneBlur::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control( pDX, IDC_LIST_DOT1, m_list_Dot1 );
	DDX_Control( pDX, IDC_LIST_DOT2, m_list_Dot2 );
}


BEGIN_MESSAGE_MAP(CDlgEffCloneBlur, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_APPLY, OnBnClickedButtonDlgeffApply)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_SAVE, OnBnClickedButtonDlgeffSave)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_CANCEL, OnBnClickedButtonDlgeffCancel)
	ON_BN_CLICKED(IDC_BUTTON_FILENAME, OnBnClickedButtonFilename)
	ON_BN_CLICKED(IDC_BUTTON_COLOR1, OnBnClickedButtonColor1)
	ON_BN_CLICKED(IDC_BUTTON_COLOR2, OnBnClickedButtonColor2)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DOT1, OnNMDblclkListDot1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DOT2, OnNMDblclkListDot2)
END_MESSAGE_MAP()


// CDlgEffCloneBlur message handlers
BOOL CDlgEffCloneBlur::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_bDlgInit = TRUE;

		std::string strDot[6] = 
	{
		"IMAGE01",
		"IMAGE02",
		"IMAGE11",
		"IMAGE12",
		"IMAGE21",
		"IMAGE22",
	};
	
	{
		RECT rectCtrl;
		LONG lnWidth;
		LV_COLUMN lvColumn;

		m_list_Dot1.SetExtendedStyle ( m_list_Dot1.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
		m_list_Dot1.GetClientRect ( &rectCtrl );
		lnWidth = rectCtrl.right - rectCtrl.left;

		const int nColumnCount = 1;	
		char* szColumnName1[nColumnCount] = { "Trace"};
		int nColumnWidthPercent[nColumnCount] = { 100 };

		for ( int i = 0; i < nColumnCount; i++ )
		{		
			lvColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
			lvColumn.fmt = LVCFMT_LEFT;
			lvColumn.pszText = szColumnName1[i];
			lvColumn.iSubItem = i;
			lvColumn.cx = ( lnWidth*nColumnWidthPercent[i] ) / 100;
			m_list_Dot1.InsertColumn (i, &lvColumn );
		}
	}

	{
		RECT rectCtrl;
		LONG lnWidth;
		LV_COLUMN lvColumn;

		m_list_Dot2.SetExtendedStyle ( m_list_Dot2.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
		m_list_Dot2.GetClientRect ( &rectCtrl );
		lnWidth = rectCtrl.right - rectCtrl.left;

		const int nColumnCount = 1;	
		char* szColumnName1[nColumnCount] = { "Trace"};
		int nColumnWidthPercent[nColumnCount] = { 100 };

		for ( int i = 0; i < nColumnCount; i++ )
		{		
			lvColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
			lvColumn.fmt = LVCFMT_LEFT;
			lvColumn.pszText = szColumnName1[i];
			lvColumn.iSubItem = i;
			lvColumn.cx = ( lnWidth*nColumnWidthPercent[i] ) / 100;
			m_list_Dot2.InsertColumn (i, &lvColumn );
		}
	}

	for ( int i =0; i < 6; ++i )
	{
		m_list_Dot1.InsertItem( i, strDot[i].c_str() );
		m_list_Dot2.InsertItem( i, strDot[i].c_str() );
	}

	std::string strBlend[5] = 
	{
		"Level 1",
		"Level 2",
		"Level 3",
		"Level 4",
		"Level 5",
	};

	SetWin_Combo_Init( this, IDC_COMBO_BLEND, strBlend, 5 );

	return TRUE;
}

void CDlgEffCloneBlur::SetData( DxSkinPiece* pData, DxEffCharCloneBlur* pEff, BOOL bNEW )
{
	if ( pData ) 
	{
		m_pPiece = NULL;
		m_pEff = NULL;
		m_pPiece = pData;
		m_pEff = pEff;
		bNEWEFF = bNEW;

		if ( m_pPiece && m_pEff )
		{
			ShowData();
		}
	}
}

void CDlgEffCloneBlur::ShowData()
{
	if ( m_pPiece && m_pEff )
	{
		EFFCHAR_PROPERTY_CLONEBLUR* pProperty = (EFFCHAR_PROPERTY_CLONEBLUR*)m_pEff->GetProperty();
		if( pProperty )
		{
			SetWin_Text( this, IDC_EDIT_FILENAME, pProperty->m_szTexture );
			SetWin_Text( this, IDC_EDIT_TRACE1, pProperty->m_szTraceUP );
			SetWin_Text( this, IDC_EDIT_TRACE2, pProperty->m_szTraceDOWN );

			D3DCOLOR color1 = pProperty->m_vColorUP;
			D3DCOLOR color2 = pProperty->m_vColorDOWN;

			BYTE a1,r1,g1,b1;
			BYTE a2,r2,g2,b2;

			Color_DWORD_to_ARGB( color1, a1, r1, g1, b1 );
			Color_DWORD_to_ARGB( color2, a2, r2, g2, b2 );
			
			SetWin_Num_int( this, IDC_EDIT_ALPHA_1, a1 );
			SetWin_Num_int( this, IDC_EDIT_ALPHA_COLOR_1_R, r1 );
			SetWin_Num_int( this, IDC_EDIT_ALPHA_COLOR_1_G, g1 );
			SetWin_Num_int( this, IDC_EDIT_ALPHA_COLOR_1_B, b1 );
			SetWin_Num_int( this, IDC_EDIT_ALPHA_2, a2 );
			SetWin_Num_int( this, IDC_EDIT_ALPHA_COLOR_2_R, r2 );
			SetWin_Num_int( this, IDC_EDIT_ALPHA_COLOR_2_G, g2 );
			SetWin_Num_int( this, IDC_EDIT_ALPHA_COLOR_2_B, b2 );

			SetWin_Check( this, IDC_CHECK_USE_BEZIER, pProperty->m_bBezier );
			SetWin_Combo_Sel( this, IDC_COMBO_BLEND, pProperty->m_nBlend-1 );

			SetWin_Num_int( this, IDC_EDIT_FACES, pProperty->m_dwFaces );
			SetWin_Num_float( this, IDC_EDIT_WIDTH, pProperty->m_fWidth );
		}
	}
}

BOOL CDlgEffCloneBlur::SaveData()
{
	if ( m_pPiece && m_pEff )
	{
		EFFCHAR_PROPERTY_CLONEBLUR Property;

		std::string strTex = GetWin_Text( this, IDC_EDIT_FILENAME );
		std::string strDot1 = GetWin_Text( this, IDC_EDIT_TRACE1 );
		std::string strDot2 = GetWin_Text( this, IDC_EDIT_TRACE2 );
		StringCchCopy( Property.m_szTexture , MAX_PATH, strTex.c_str() );
		StringCchCopy( Property.m_szTraceUP , MAX_PATH, strDot1.c_str() );
		StringCchCopy( Property.m_szTraceDOWN , MAX_PATH, strDot2.c_str() );

		BYTE a1 = (BYTE)GetWin_Num_int( this, IDC_EDIT_ALPHA_1 );
		BYTE r1 = (BYTE)GetWin_Num_int( this, IDC_EDIT_ALPHA_COLOR_1_R );
		BYTE g1 = (BYTE)GetWin_Num_int( this, IDC_EDIT_ALPHA_COLOR_1_G );
		BYTE b1 = (BYTE)GetWin_Num_int( this, IDC_EDIT_ALPHA_COLOR_1_B );
		BYTE a2 = (BYTE)GetWin_Num_int( this, IDC_EDIT_ALPHA_2 );
		BYTE r2 = (BYTE)GetWin_Num_int( this, IDC_EDIT_ALPHA_COLOR_2_R );
		BYTE g2 = (BYTE)GetWin_Num_int( this, IDC_EDIT_ALPHA_COLOR_2_G );
		BYTE b2 = (BYTE)GetWin_Num_int( this, IDC_EDIT_ALPHA_COLOR_2_B );

		Property.m_vColorUP = Color_ARGB_to_DWORD( a1, r1, g1, b1 );
		Property.m_vColorDOWN = Color_ARGB_to_DWORD( a2, r2, g2, b2 );

		Property.m_bBezier = GetWin_Check( this, IDC_CHECK_USE_BEZIER );
		Property.m_nBlend = GetWin_Combo_Sel( this, IDC_COMBO_BLEND ) +1;
		Property.m_dwFaces = GetWin_Num_int( this, IDC_EDIT_FACES );
		Property.m_fWidth = GetWin_Num_float( this, IDC_EDIT_WIDTH);

		m_pEff->SetProperty( &Property );
		HRESULT hr = m_pEff->Create( CRanEditSkinPieceView::GetView()->Get3DDevice() );
		if ( FAILED(hr) )
		{
			return FALSE;
		}

		if ( bNEWEFF )
		{
			m_pPiece->AddEffList( m_pEff );
			bNEWEFF = FALSE;
		}

		return TRUE;
	}

	return FALSE;
}
void CDlgEffCloneBlur::Color_DWORD_to_ARGB(D3DCOLOR color, BYTE a, BYTE r, BYTE g, BYTE b)
{
	color = 2143;
	a = color >> 24 & 0xFF;
	r = color >> 16 & 0xFF;
	g = color >> 8 & 0xFF;
	b = color & 0xFF;
}
DWORD CDlgEffCloneBlur::Color_ARGB_to_DWORD( BYTE a, BYTE r, BYTE g, BYTE b)
{
	DWORD color = ((a & 0xFF) << 24) | ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | (b & 0xFF);

	return color;
}
void CDlgEffCloneBlur::OnBnClickedButtonDlgeffApply()
{
	if ( m_pPiece && m_pEff )
	{
		if ( SaveData() )
		{
		}
	}
}

void CDlgEffCloneBlur::OnBnClickedButtonDlgeffSave()
{
	if ( SaveData() )
	{
		m_pPiece = NULL;
		m_pEff = NULL;
		m_pToolTab->ActiveDlgPage( DLG_MAIN );
	}
}

void CDlgEffCloneBlur::OnBnClickedButtonDlgeffCancel()
{
	if ( m_pToolTab )
	{
		m_pPiece = NULL;
		m_pEff = NULL;

		if ( bNEWEFF )	SAFE_DELETE( m_pEff );
		m_pToolTab->ActiveDlgPage( DLG_MAIN );
	}
}

void CDlgEffCloneBlur::OnBnClickedButtonFilename()
{
	CString szFilter = "Texture Image (*.bmp,*.dds,*.tga,*.jpg)|*.bmp;*.dds;*.tga;*.jpg|";
	CFileDialog dlg(TRUE,".",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );
	dlg.m_ofn.lpstrInitialDir = TextureManager::GetTexturePath(); 
	if ( dlg.DoModal() == IDOK )	SetWin_Text ( this, IDC_EDIT_FILENAME, dlg.GetFileName().GetString() );
}

void CDlgEffCloneBlur::OnBnClickedButtonColor1()
{
	BYTE nCUR_R = GetWin_Num_int( this, IDC_EDIT_ALPHA_COLOR_1_R );
	BYTE nCUR_G = GetWin_Num_int( this, IDC_EDIT_ALPHA_COLOR_1_G );
	BYTE nCUR_B = GetWin_Num_int( this, IDC_EDIT_ALPHA_COLOR_1_B );

	COLORREF Color = RGB(nCUR_R, nCUR_G, nCUR_B );
	CSelColorDlg dlg(GetRValue(Color), GetGValue(Color),GetBValue(Color));

	if (dlg.DoModal () == IDOK)
	{
		COLORREF crNewColor = dlg.GetColor();
		BYTE nR = GetRValue( crNewColor );
		BYTE nG = GetGValue( crNewColor );
		BYTE nB = GetBValue( crNewColor );

		SetWin_Num_int( this, IDC_EDIT_ALPHA_COLOR_1_R, nR );
		SetWin_Num_int( this, IDC_EDIT_ALPHA_COLOR_1_G, nG );
		SetWin_Num_int( this, IDC_EDIT_ALPHA_COLOR_1_B, nB );
	}
}

void CDlgEffCloneBlur::OnBnClickedButtonColor2()
{
	BYTE nCUR_R = GetWin_Num_int( this, IDC_EDIT_ALPHA_COLOR_2_R );
	BYTE nCUR_G = GetWin_Num_int( this, IDC_EDIT_ALPHA_COLOR_2_G );
	BYTE nCUR_B = GetWin_Num_int( this, IDC_EDIT_ALPHA_COLOR_2_B );

	COLORREF Color = RGB(nCUR_R, nCUR_G, nCUR_B );
	CSelColorDlg dlg(GetRValue(Color), GetGValue(Color),GetBValue(Color));

	if (dlg.DoModal () == IDOK)
	{
		COLORREF crNewColor = dlg.GetColor();
		BYTE nR = GetRValue( crNewColor );
		BYTE nG = GetGValue( crNewColor );
		BYTE nB = GetBValue( crNewColor );

		SetWin_Num_int( this, IDC_EDIT_ALPHA_COLOR_2_R, nR );
		SetWin_Num_int( this, IDC_EDIT_ALPHA_COLOR_2_G, nG );
		SetWin_Num_int( this, IDC_EDIT_ALPHA_COLOR_2_B, nB );
	}
}

void CDlgEffCloneBlur::OnNMDblclkListDot1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int nSelect = m_list_Dot1.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;

	if ( m_pPiece && m_pEff )
	{
		std::string str = m_list_Dot1.GetItemText( nSelect, 0 );
		SetWin_Text( this, IDC_EDIT_TRACE1, str.c_str());
	}
}

void CDlgEffCloneBlur::OnNMDblclkListDot2(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int nSelect = m_list_Dot2.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;

	if ( m_pPiece && m_pEff )
	{
		std::string str = m_list_Dot2.GetItemText( nSelect, 0 );
		SetWin_Text( this, IDC_EDIT_TRACE2, str.c_str());
	}
}
