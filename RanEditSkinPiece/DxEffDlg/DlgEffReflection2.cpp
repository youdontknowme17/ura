// DlgEffReflection2.cpp : implementation file
//

#include "stdafx.h"
#include "../RanEditSkinPiece.h"
#include "DlgEffReflection2.h"
#include "../RanEditSkinPieceView.h"
#include "DxEffChar.h"
#include "EtcFunction.h"
#include "../ToolTab.h"
#include ".\dlgeffreflection2.h"

#include "../SelColorDlg.h"
// CDlgEffReflection2 dialog

IMPLEMENT_DYNAMIC(CDlgEffReflection2, CPropertyPage)
CDlgEffReflection2::CDlgEffReflection2( LOGFONT logfont )
	: CPropertyPage(CDlgEffReflection2::IDD)
	, m_pFont( NULL )
	, m_pPiece( NULL )
	, bNEWEFF( FALSE )
	, m_nMatEdit( -1 )
{
	m_bDlgInit = FALSE;
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CDlgEffReflection2::~CDlgEffReflection2()
{
}

void CDlgEffReflection2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control( pDX, IDC_LIST_SPECULAR, m_list_Material );
}


BEGIN_MESSAGE_MAP(CDlgEffReflection2, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_APPLY, OnBnClickedButtonDlgeffApply)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_SAVE, OnBnClickedButtonDlgeffSave)
	ON_BN_CLICKED(IDC_BUTTON_DLGEFF_CANCEL, OnBnClickedButtonDlgeffCancel)
	ON_BN_CLICKED(IDC_BUTTON_SPECULAR, OnBnClickedButtonSpecular)
	ON_BN_CLICKED(IDC_BUTTON_SPECULAR_OK, OnBnClickedButtonSpecularOk)
	ON_BN_CLICKED(IDC_BUTTON_COLOR, OnBnClickedButtonColor)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SPECULAR, OnNMDblclkListSpecular)
	ON_BN_CLICKED(IDC_BUTTON_TEXTURE, OnBnClickedButtonTexture)
END_MESSAGE_MAP()


// CDlgEffReflection2 message handlers
BOOL CDlgEffReflection2::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_bDlgInit = TRUE;

	{
		RECT rectCtrl;
		LONG lnWidth;
		LV_COLUMN lvColumn;

		m_list_Material.SetExtendedStyle ( m_list_Material.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );
		m_list_Material.GetClientRect ( &rectCtrl );
		lnWidth = rectCtrl.right - rectCtrl.left;

		const int nColumnCount = 4;	
		char* szColumnName1[nColumnCount] = { "Material", "Used", "Effect", "Power" };
		int nColumnWidthPercent[nColumnCount] = { 40,10,40,10 };

		for ( int i = 0; i < nColumnCount; i++ )
		{		
			lvColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
			lvColumn.fmt = LVCFMT_LEFT;
			lvColumn.pszText = szColumnName1[i];
			lvColumn.iSubItem = i;
			lvColumn.cx = ( lnWidth*nColumnWidthPercent[i] ) / 100;
			m_list_Material.InsertColumn (i, &lvColumn );
		}
	}

	std::string strCOLOROP[3] = 
	{
		"Modulate",
		"Modulate(2X)",
		"Modulate(4X)",
	};

	SetWin_Combo_Init( this, IDC_COMBO_COLOR_OP, strCOLOROP, 3 );

	return TRUE;
}

void CDlgEffReflection2::SetData( DxSkinPiece* pData, DxEffCharReflection2* pEff, BOOL bNEW )
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
			if ( bNEWEFF )
			{
				//if effect is new then create new materials also
				//derive from mesh materials
				DWORD dwMAT = m_pPiece->m_pmcMesh->GetNumMaterials();
				DXMATERIAL_CHAR_EFF* pMaterials = new DXMATERIAL_CHAR_EFF[dwMAT];

				for ( DWORD i=0; i<dwMAT; i++ )
				{
					StringCchCopy( pMaterials[i].szTexture,	MAX_PATH, m_pPiece->m_pmcMesh->pMaterials[i].pTextureFilename );
				}

				m_pEff->SetMaterials( CRanEditSkinPieceView::GetView()->Get3DDevice(), dwMAT, pMaterials );
			}


			ShowData();
		}
	}
}

void CDlgEffReflection2::ShowData()
{
	if ( m_pPiece && m_pEff )
	{
		EFFCHAR_PROPERTY_REFLECTION2* pProperty = (EFFCHAR_PROPERTY_REFLECTION2*) m_pEff->GetProperty();
		if ( pProperty )
		{
			SetWin_Combo_Sel( this, IDC_COMBO_COLOR_OP, pProperty->m_dwColorOP - 4 );
			SetWin_Num_float( this, IDC_EDIT_COLOR_A, pProperty->m_cSpecular.a );
			SetWin_Num_float( this, IDC_EDIT_COLOR_R, pProperty->m_cSpecular.r );
			SetWin_Num_float( this, IDC_EDIT_COLOR_G, pProperty->m_cSpecular.g );
			SetWin_Num_float( this, IDC_EDIT_COLOR_B, pProperty->m_cSpecular.b );
			SetWin_Text( this, IDC_EDIT_TEXTURE, pProperty->m_szTexture );
		}
		ShowMaterialList();
	}
}

void CDlgEffReflection2::ShowMaterialList()
{
	if ( m_pPiece && m_pEff )
	{
		EFFCHAR_PROPERTY_REFLECTION2* pProperty = (EFFCHAR_PROPERTY_REFLECTION2*) m_pEff->GetProperty();
		DXMATERIAL_CHAR_EFF*	pMAT = m_pEff->GetMaterials();

		std::string str[2] = {"No","Yes"};

		if ( pProperty&&pMAT )
		{
			m_list_Material.DeleteAllItems();
			char szTempChar[512];

			for ( DWORD dw = 0; dw < pProperty->m_dwMaterials; ++dw )
			{
				m_list_Material.InsertItem( dw, pMAT[dw].szTexture );
				m_list_Material.SetItemText( dw, 1, str[pMAT[dw].bEffUse].c_str() );
				m_list_Material.SetItemText( dw, 2, pMAT[dw].szEffTex );

				sprintf( szTempChar, "%2.2f", pMAT[dw].d3dMaterial.Power );
				m_list_Material.SetItemText( dw, 3, szTempChar );
			}

			SetWin_Enable( this, IDC_CHECK_SPECULAR, FALSE );
			SetWin_Enable( this, IDC_BUTTON_SPECULAR, FALSE );
			SetWin_Enable( this, IDC_EDIT_SPECULAR, FALSE );
			SetWin_Enable( this, IDC_EDIT_SPECULARPOWER, FALSE );
			SetWin_Enable( this, IDC_BUTTON_SPECULAR_OK, FALSE );
			m_nMatEdit = -1;
		}
	}
}

BOOL CDlgEffReflection2::SaveData()
{
	if ( m_pPiece && m_pEff )
	{
		EFFCHAR_PROPERTY_REFLECTION2 Property;

		std::string str = GetWin_Text( this, IDC_EDIT_TEXTURE );
		StringCchCopy( Property.m_szTexture, MAX_PATH, str.c_str() );

		Property.m_dwColorOP = GetWin_Combo_Sel( this, IDC_COMBO_COLOR_OP) +4 ;
		Property.m_cSpecular.a = GetWin_Num_float( this, IDC_EDIT_COLOR_A );
		Property.m_cSpecular.r = GetWin_Num_float( this, IDC_EDIT_COLOR_R );
		Property.m_cSpecular.g = GetWin_Num_float( this, IDC_EDIT_COLOR_G );
		Property.m_cSpecular.b = GetWin_Num_float( this, IDC_EDIT_COLOR_B );

		Property.m_dwMaterials = m_pEff->GetMaterialNum();

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

void CDlgEffReflection2::OnBnClickedButtonDlgeffApply()
{
	if ( m_pPiece && m_pEff )
	{
		if ( SaveData() )
		{
		}
	}
}

void CDlgEffReflection2::OnBnClickedButtonDlgeffSave()
{
	if ( SaveData() )
	{
		m_pPiece = NULL;
		m_pEff = NULL;
		m_pToolTab->ActiveDlgPage( DLG_MAIN );
	}
}

void CDlgEffReflection2::OnBnClickedButtonDlgeffCancel()
{
	if ( m_pToolTab )
	{
		m_pPiece = NULL;
		m_pEff = NULL;

		if ( bNEWEFF )	SAFE_DELETE( m_pEff );
		m_pToolTab->ActiveDlgPage( DLG_MAIN );
	}
}

void CDlgEffReflection2::OnBnClickedButtonSpecular()
{
	CString szFilter = "Texture Image (*.bmp,*.dds,*.tga,*.jpg)|*.bmp;*.dds;*.tga;*.jpg|";
	CFileDialog dlg(TRUE,".",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );
	dlg.m_ofn.lpstrInitialDir = TextureManager::GetTexturePath(); 
	if ( dlg.DoModal() == IDOK )	SetWin_Text ( this, IDC_EDIT_SPECULAR, dlg.GetFileName().GetString() );
}

void CDlgEffReflection2::OnBnClickedButtonSpecularOk()
{
	if( m_nMatEdit == -1 ) return;

	if ( m_pPiece && m_pEff )
	{
		DXMATERIAL_CHAR_EFF*	pMAT = m_pEff->GetMaterials();
		if ( pMAT )
		{
			pMAT[m_nMatEdit].bEffUse = GetWin_Check( this, IDC_CHECK_SPECULAR );
			pMAT[m_nMatEdit].d3dMaterial.Power = GetWin_Num_float( this, IDC_EDIT_SPECULARPOWER );
			std::string str = GetWin_Text( this, IDC_EDIT_SPECULAR );
			StringCchCopy( pMAT[m_nMatEdit].szEffTex, MAX_PATH, str.c_str() );
			ShowMaterialList();
		}
	}
}

void CDlgEffReflection2::OnBnClickedButtonColor()
{
	float fr = GetWin_Num_float( this, IDC_EDIT_COLOR_R );
	float fg = GetWin_Num_float( this, IDC_EDIT_COLOR_G );
	float fb = GetWin_Num_float( this, IDC_EDIT_COLOR_B );

	BYTE nCUR_R = BYTE ( fr * 255.0f );
	BYTE nCUR_G = BYTE ( fg * 255.0f );
	BYTE nCUR_B = BYTE ( fb * 255.0f );

	COLORREF Color = RGB(nCUR_R, nCUR_G, nCUR_B );
	CSelColorDlg dlg(GetRValue(Color), GetGValue(Color),GetBValue(Color));

	if (dlg.DoModal () == IDOK)
	{
		COLORREF crNewColor = dlg.GetColor();
		float fR = (float)GetRValue( crNewColor );
		float fG = (float)GetGValue( crNewColor );
		float fB = (float)GetBValue( crNewColor );

		SetWin_Num_float( this, IDC_EDIT_COLOR_R, fR / 255.0f );
		SetWin_Num_float( this, IDC_EDIT_COLOR_G, fG / 255.0f );
		SetWin_Num_float( this, IDC_EDIT_COLOR_B, fB / 255.0f );
	}
}

void CDlgEffReflection2::OnNMDblclkListSpecular(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	int nSelect = m_list_Material.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ) return;

	if ( m_pPiece && m_pEff )
	{
		DXMATERIAL_CHAR_EFF*	pMAT = m_pEff->GetMaterials();
		if ( pMAT )
		{
			SetWin_Enable( this, IDC_CHECK_SPECULAR, TRUE );
			SetWin_Enable( this, IDC_BUTTON_SPECULAR, TRUE );
			SetWin_Enable( this, IDC_EDIT_SPECULAR, TRUE );
			SetWin_Enable( this, IDC_EDIT_SPECULARPOWER, TRUE );
			SetWin_Enable( this, IDC_BUTTON_SPECULAR_OK, TRUE );
			m_nMatEdit = nSelect;

			SetWin_Check( this, IDC_CHECK_SPECULAR, pMAT[nSelect].bEffUse );
			SetWin_Text( this, IDC_EDIT_SPECULAR, pMAT[nSelect].szEffTex );
			SetWin_Num_float( this, IDC_EDIT_SPECULARPOWER, pMAT[nSelect].d3dMaterial.Power );
		}
	}
}

void CDlgEffReflection2::OnBnClickedButtonTexture()
{
	CString szFilter = "Texture Image (*.bmp,*.dds,*.tga,*.jpg)|*.bmp;*.dds;*.tga;*.jpg|";
	CFileDialog dlg(TRUE,".",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );
	dlg.m_ofn.lpstrInitialDir = TextureManager::GetTexturePath(); 
	if ( dlg.DoModal() == IDOK )	SetWin_Text ( this, IDC_EDIT_TEXTURE, dlg.GetFileName().GetString() );
}
