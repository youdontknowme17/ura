#include "StdAfx.h"
#include "NameDisplay.h"
#include "BasicLineBoxEx.h"
#include "../EngineUILib/GUInterface/BasicTextBox.h"
#include "../EngineLib/DxCommon/DxFontMan.h"
#include "../EngineLib/DxCommon/d3dfontx.h"
#include "UITextControl.h"
#include "InnerInterface.h"
#include "DxClubMan.h"
#include "GLGaeaClient.h"
#include "RANPARAM.h"
#include "d3dfont.h"
#include "../EngineUILib/GUInterface/BasicProgressBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CNameDisplay::CNameDisplay ()
	: m_bUsedMemPool( false )
	, m_bDISP( true )
//	, m_pNameLineBox(NULL)
	, m_pNameLineBoxCD(NULL)
	, m_bClub( false )
{
	memset( m_pSchoolMark, 0, sizeof ( CUIControl* ) * nSCHOOLMARK );
	memset( m_pPartyMark, 0, sizeof ( CUIControl* ) * nPARTYMARK );
	memset( m_pRankMark, 0, sizeof ( CUIControl* ) * nRANKMARK ); //add pkrank
}

CNameDisplay::~CNameDisplay ()
{
}

void CNameDisplay::INITIALIZE()
{
	m_bDISP = true;
	m_bClub = false;

	m_pNameBox->ClearText();
	m_pNameBox->SetLocalPos( m_rcNameBox );

	for ( int i = 0; i < nSCHOOLMARK; ++i )
	{
		m_pSchoolMark[i]->SetVisibleSingle( FALSE );
		m_pSchoolMark[i]->SetLocalPos( m_rcSchoolMark[i] );
	}

	for ( i = 0; i < nPARTYMARK; ++i )
	{
		m_pPartyMark[i]->SetVisibleSingle( FALSE );
		m_pPartyMark[i]->SetLocalPos( m_rcPartyMark[i] );
	}

	for ( i = 0; i < nRANKMARK; ++i ) //add pkrank
	{
		m_pRankMark[i]->SetVisibleSingle( FALSE );
		m_pRankMark[i]->SetLocalPos( m_rcRankMark[i] );
	}

	m_pClubMark->SetVisibleSingle( FALSE );

	m_pNameLineBox->SetVisibleSingle( FALSE );
	m_pNameLineBoxCD->SetVisibleSingle( FALSE );
}

void CNameDisplay::CreateSubControl ()
{
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	CBasicLineBoxEx* pLineBox = new CBasicLineBoxEx;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_EX_NAME", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxChat ( "BASIC_NAME_LINE_BOX" );
	pLineBox->SetVisibleSingle( FALSE );
	RegisterControl ( pLineBox );
	m_pNameLineBox = pLineBox;

	CBasicLineBoxEx* pLineBoxCD = new CBasicLineBoxEx;
	pLineBoxCD->CreateSub ( this, "BASIC_LINE_BOX_EX_NAME", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBoxCD->CreateBaseBoxNameCD ( "BASIC_NAME_LINE_BOX_CD" );
	pLineBoxCD->SetVisibleSingle( FALSE );
	RegisterControl ( pLineBoxCD );
	m_pNameLineBoxCD = pLineBoxCD;

	CBasicTextBox* pNameBox = new CBasicTextBox;
	pNameBox->CreateSub ( this, "NAME_DISPLAY_NAMEBOX", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
    pNameBox->SetFont ( pFont8 );
	pNameBox->SetPartInterval ( 1.0f );
	pNameBox->SetLineInterval ( 0.0f );
	RegisterControl ( pNameBox );
	m_pNameBox = pNameBox;

	m_rcNameBox = m_pNameBox->GetLocalPos(); // MEMO : �⺻ ���������� ����

	{
		CString strSchoolMark[] = 
		{
			"NAME_DISPLAY_SCHOOL1",
			"NAME_DISPLAY_SCHOOL2",
			"NAME_DISPLAY_SCHOOL3"
		};

		for ( int i = 0; i < nSCHOOLMARK; ++i )
		{
			m_pSchoolMark[i] = CreateControl ( strSchoolMark[i].GetString(), UI_FLAG_LEFT );
			m_pSchoolMark[i]->SetVisibleSingle ( FALSE );
			m_rcSchoolMark[i] = m_pSchoolMark[i]->GetLocalPos();
		}
	}

	{
		CString strPartyMark[] = 
		{
			"NAME_DISPLAY_PARTY_MEMBER",
			"NAME_DISPLAY_PARTY_MASTER",
			"NAME_DISPLAY_PARTY_MEMBER_TARGET",
			"NAME_DISPLAY_PARTY_MASTER_TARGET",
			"NAME_DISPLAY_PARTY_MASTER_GENERAL"
		};

		for ( int i = 0; i < nPARTYMARK; ++i )
		{
			m_pPartyMark[i] = CreateControl ( strPartyMark[i].GetString(), UI_FLAG_RIGHT );
			m_pPartyMark[i]->SetVisibleSingle ( FALSE );

			m_rcPartyMark[i] = m_pPartyMark[i]->GetLocalPos();
		}
	}

	{ //add pkrank
		CString strRankMark[] = 
		{
			"RANK_DISPLAY_1",
			"RANK_DISPLAY_2",
			"RANK_DISPLAY_3",
			"RANK_DISPLAY_4",
			"RANK_DISPLAY_5",
			"RANK_DISPLAY_6",
			"RANK_DISPLAY_7",
			"RANK_DISPLAY_8",
			"RANK_DISPLAY_9",
			"RANK_DISPLAY_10",
			"RANK_DISPLAY_11",
			"RANK_DISPLAY_12",
			"RANK_DISPLAY_13",
			"RANK_DISPLAY_14",
			"RANK_DISPLAY_15",
			"RANK_DISPLAY_16",
			"RANK_DISPLAY_17",
			"RANK_DISPLAY_18",
			"RANK_DISPLAY_19",
			"RANK_DISPLAY_20",
		};

		for ( int i = 0; i < nRANKMARK; ++i )
		{
			m_pRankMark[i] = CreateControl ( strRankMark[i].GetString(), UI_FLAG_RIGHT );
			m_pRankMark[i]->SetVisibleSingle ( FALSE );

			m_rcRankMark[i] = m_pRankMark[i]->GetLocalPos();
		}
	}

	m_pClubMark = CreateControl ( "NAME_DISPLAY_CLUB_MARK", UI_FLAG_DEFAULT );
	m_pClubMark->SetVisibleSingle ( FALSE );
	CBasicProgressBar* pHP = new CBasicProgressBar;
	pHP->CreateSub ( this, "HEALTH_DISPLAY" );
	pHP->CreateOverImage ( "HEALTH_DISPLAY_OVERIMAGE" );
	pHP->SetControlNameEx ( "HP" );
	pHP->SetAlignFlag ( UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	RegisterControl ( pHP );
	m_pHP = pHP;
	m_pHP->SetVisibleSingle(FALSE);

	SetFocus(false);
}

void CNameDisplay::SetHP(float fPercent, BOOL bFocus )
{
	if ( m_pHP )
	{
		m_pHP->SetPercent(fPercent);
		m_pHP->SetVisibleSingle(bFocus);
	}

}

CUIControl*	CNameDisplay::CreateControl ( const char* szControl, WORD wAlignFlag )
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, szControl, wAlignFlag );
	RegisterControl ( pControl );
	return pControl;
}
void CNameDisplay::SetItemLineBox()
{
	m_pNameLineBox->SetVisibleSingle( TRUE );
}
void CNameDisplay::SetName ( const CString& strName,const DWORD m_dwPkWin, const BOOL bChar, const BOOL bRankName, const BOOL bRankMark, const D3DCOLOR& dwColor, WORD wSchoolMark, DISP_PARTY emnPartyMark )
{
	//if ( !GLCONST_CHAR::bEnableRankName ) bRankName = FALSE; //disable feature in global way
	//if ( !GLCONST_CHAR::bEnableRankMark ) bRankMark = FALSE; //disable feature in global way

	if ( wSchoolMark == NODATA && emnPartyMark == NODATA )
	{
		m_pNameBox->SetTextAlign ( TEXT_ALIGN_CENTER_X );
	}

	const float fGapMarkName		= 20.0f;
	const float fGapMarkNameOrig	= 2.0f;
	const float fGapMarkNameRank	= 3.0f;

	float fSchoolMarkEdit = 0;
	if ( bRankMark && GLCONST_CHAR::bEnableRankMark )
	{
		fSchoolMarkEdit = m_pSchoolMark[0]->GetLocalPos ().sizeX + fGapMarkName;
	}
	else
	{
		fSchoolMarkEdit = m_pSchoolMark[0]->GetLocalPos ().sizeX + fGapMarkNameOrig;
	}

	const float fSchoolMark =  fSchoolMarkEdit;
	const float fRankMark = m_pRankMark[0]->GetLocalPos ().sizeX + fGapMarkNameRank; //add pkrank
	const float fPartyMark = m_pPartyMark[0]->GetLocalPos ().sizeX;


	if (bChar)
	{
		if ( bRankName && GLCONST_CHAR::bEnableRankName )
		{

			CString strTemp = "";
			CString strTemp2 = "";
			strTemp.Format ( "<%s>", GLCONST_CHAR::strRank_Name[CalculateVal(m_dwPkWin)].c_str() );
			strTemp2.Format ( " %s", strName.GetString() );
			int Index = m_pNameBox->AddText ( strTemp,  NS_UITEXTCOLOR::DODGERBLUE );	
			m_pNameBox->AddString ( Index, strTemp2, dwColor );
			//m_pNameBox->AddTextNoSplit ( strTemp, dwColor );
		}
		else
		{
			m_pNameBox->AddTextNoSplit ( strName, dwColor );
		}
	}
	else
	{
		m_pNameBox->AddTextNoSplit ( strName, dwColor );
	}

	const UIRECT& GetControlVec = m_pNameBox->GetLocalPos ();
	const float& fLineInterval = m_pNameBox->GetPartInterval ();
	long lLineSize = m_pNameBox->GetLongestLine ();


	if ( (NODATA != wSchoolMark) && (wSchoolMark < nSCHOOLMARK) )
	{
		m_pSchoolMark[wSchoolMark]->SetVisibleSingle ( TRUE );

		if ( m_bClub )
		{
			const UIRECT& rcClubMark = m_pClubMark->GetLocalPos();
			const UIRECT& rcMark = m_pSchoolMark[wSchoolMark]->GetLocalPos();
			m_pSchoolMark[wSchoolMark]->SetLocalPos ( D3DXVECTOR2(rcMark.left, rcMark.top + rcClubMark.sizeY + fLineInterval + 1.0f) );
		}

		lLineSize += long ( fSchoolMark );
		m_pNameBox->SetLocalPos ( D3DXVECTOR2( GetControlVec.left + fSchoolMark, GetControlVec.top ) );
		m_pNameBox->SetTextAlign ( TEXT_ALIGN_LEFT );
	}

	if ( PARTY_MASTER == emnPartyMark || PARTY_MASTER_TARGET == emnPartyMark || PARTY_MASTER_GENERAL == emnPartyMark )
	{
		m_pPartyMark[emnPartyMark]->SetVisibleSingle ( TRUE );

		if ( m_bClub )
		{
			const CBasicTextBox::STEXTPART& sPART1 = m_pNameBox->GetTextPart ( 0 );
			const CBasicTextBox::STEXTPART& sPART2 = m_pNameBox->GetTextPart ( 1 );

			long Line1Size = sPART1.dequeLine[0].strSize.cx;
			long Line2Size = long(sPART2.dequeLine[0].strSize.cx + fPartyMark);

			if ( Line2Size > Line1Size )
			{
				lLineSize += long ( fPartyMark );
			}
		}
		else
		{
			lLineSize += long ( fPartyMark );
		}	
	}

	float fGapX = GetControlVec.sizeX - float(lLineSize);
	float fCLUBSIZE = 0.0f;
	if ( m_bClub )
	{
		fCLUBSIZE = m_pNameBox->CalcMaxHEIGHT(0,0) + 2.0f;
	}

	const UIRECT& rcLocalPosOld = GetLocalPos ();
	UIRECT rcLocalPosNew( rcLocalPosOld.left, rcLocalPosOld.top,
					rcLocalPosOld.sizeX - fGapX + 2.0f, rcLocalPosOld.sizeY + fCLUBSIZE );

	AlignSubControl ( rcLocalPosOld, rcLocalPosNew );
	SetLocalPos ( rcLocalPosNew );

	if( (NODATA != wSchoolMark) && (wSchoolMark < nSCHOOLMARK) )
	{
		const UIRECT& rcLocalPos = m_pNameBox->GetLocalPos ();
		m_pNameBox->SetLocalPos ( UIRECT(rcLocalPos.left, rcLocalPos.top - 2, rcLocalPos.sizeX - fSchoolMark, rcLocalPos.sizeY) );
	}

	if ( bRankMark && GLCONST_CHAR::bEnableRankMark )
	{
		m_pRankMark[CalculateVal(m_dwPkWin)]->SetVisibleSingle ( TRUE );
		const UIRECT& rcNewMark2 = m_pNameBox->GetLocalPos ();

		if ( m_bClub )
		{
			m_pRankMark[CalculateVal(m_dwPkWin)]->SetLocalPos ( D3DXVECTOR2(rcNewMark2.left - fRankMark, rcNewMark2.top + 15.0f ) );
		}
		else
		{
			m_pRankMark[CalculateVal(m_dwPkWin)]->SetLocalPos ( D3DXVECTOR2(rcNewMark2.left - fRankMark, rcNewMark2.top + 2) );
		}
	}

	if ( m_bClub )
	{
		if ( (NODATA != emnPartyMark) && (emnPartyMark < nPARTYMARK) )
		{
			float fRIGHT = 0.0f;

			const UIRECT& rcLocalPos = m_pNameBox->GetLocalPos ();
			const float fHEIGHT = m_pNameBox->CalcMaxHEIGHT ( 0, 2 );

			m_pNameBox->SetGlobalPos ( UIRECT ( rcLocalPos.left, rcLocalPos.top, rcLocalPos.sizeX, fHEIGHT ) );
			m_pNameBox->SetLocalPos ( UIRECT ( rcLocalPos.left, rcLocalPos.top, rcLocalPos.sizeX, fHEIGHT ) );

			if ( m_pNameBox->GetCount () > 1 )
			{
				UIRECT rcPart;
				m_pNameBox->GetPartInfo ( 1, rcPart );
				fRIGHT = rcPart.right;
			}
			else if ( m_pNameBox->GetCount () == 1 )
			{
				UIRECT rcPart;
				m_pNameBox->GetPartInfo ( 0, rcPart );
				fRIGHT = rcPart.right;
			}
			else
			{
				GASSERT ( 0 && "CNameDisplay::SetName(), m_pNameBox->GetCount <= 0" );
				return ;
			}

			const UIRECT& rcClubMark = m_pClubMark->GetLocalPos();
			const UIRECT& rcMark = m_pPartyMark[emnPartyMark]->GetLocalPos();
			m_pPartyMark[emnPartyMark]->SetLocalPos ( D3DXVECTOR2(fRIGHT, rcMark.top + rcClubMark.sizeY + fLineInterval + 1.0f) );
		}
	}

	SetGlobalPos ( rcLocalPosNew );
}

void CNameDisplay::SetClubName ( const CString& strName, const D3DCOLOR& dwColor )
{
	if ( m_pNameBox->GetCount () )
	{
		GASSERT ( 0 && "Ŭ�� �̸� ���� ������ �������� �ʽ��ϴ�." );
		return ;
	}
	
	m_pNameBox->AddTextNoSplit( strName, dwColor );
	m_pClubMark->SetVisibleSingle ( TRUE );
	m_bClub = true; // MEMO : Ŭ������ ����
}

void CNameDisplay::SetPetOwnerName( const CString& strName, const D3DCOLOR& dwColor )
{
	if ( m_pNameBox->GetCount () )
	{
		GASSERT ( 0 && "�� �̸� ���� ������ �������� �ʽ��ϴ�." );
		return ;
	}

	m_pNameBox->AddTextNoSplit( strName, dwColor );
	m_bClub = true; // MEMO : Ŭ������ ����
}
/*void CNameDisplay::SetRankName ( const CString& strname, const D3DCOLOR& dwColor )
{
	if ( m_pNameBox->GetCount () )
	{
		GASSERT ( 0 && "�� �̸� ���� ������ �������� �ʽ��ϴ�." );
		return ;
	}

	m_pNameBox->AddTextNoSplit( strName, dwColor );
	m_bClub = true; // MEMO : Ŭ������ ����
}*/
HRESULT CNameDisplay::Render ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	GASSERT( pd3dDevice );

	if( !m_sDISP_INFO.IsCLUB() ) return CUIGroup::Render ( pd3dDevice );

	HRESULT hr = S_OK;

	// MEMO : �Ʒ� �ڵ�� �׽�Ʈ ���Դϴ�.
	// ���߿� �޼����� ���� ���� ����ǵ��� �����ؾ� �մϴ�.
	//part of addshadow project
	//remove linebox in namedisplay
	m_pNameLineBox->SetVisibleSingle( FALSE );
	m_pNameLineBoxCD->SetVisibleSingle( FALSE );
	if( IsCDCERTIFY() ) m_pNameLineBoxCD->SetVisibleSingle( TRUE );
	//else				m_pNameLineBox->SetVisibleSingle( TRUE );

	m_pClubMark->SetVisibleSingle ( FALSE );
	{
		hr = CUIGroup::Render ( pd3dDevice );
		if ( FAILED ( hr ) ) return hr;
	}
	m_pClubMark->SetVisibleSingle ( TRUE );

	const int nSERVER = GLGaeaClient::GetInstance().GetCharacter()->m_dwServerID;
	const DxClubMan::DXDATA& sMarkData = DxClubMan::GetInstance().GetClubData ( pd3dDevice, nSERVER, m_sDISP_INFO.m_dwCLUB, m_sDISP_INFO.m_dwCLUB_MARK_VER );

	m_pClubMark->SetTexturePos ( 0, sMarkData.vTex_1_LU );
	m_pClubMark->SetTexturePos ( 1, sMarkData.vTex_2_RU );
	m_pClubMark->SetTexturePos ( 3, sMarkData.vTex_3_LD );
	m_pClubMark->SetTexturePos ( 2, sMarkData.vTex_4_RD );

	const UIRECT& rcGlobalPos = m_pClubMark->GetGlobalPos ();
	m_pClubMark->SetGlobalPos( UIRECT( ceil(rcGlobalPos.left), ceil(rcGlobalPos.top),
										ceil(rcGlobalPos.sizeX), ceil(rcGlobalPos.sizeY) ) );

	//	��������
	m_pClubMark->SetTexture ( sMarkData.pddsTexture );
	hr = m_pClubMark->Render ( pd3dDevice );

	return hr;
}

//void	CNameDisplay::SetNameColor ( const D3DCOLOR& dwColor )
//{
//	if ( m_pNameBox->GetCount () > 1 )
//	{
//		m_pNameBox->SetUseTextColor ( 1, TRUE );
//		m_pNameBox->SetTextColor ( 1, dwColor );
//	}
//}
//
//void	CNameDisplay::SetClubNameColor ( const D3DCOLOR& dwColor )
//{
//	if ( m_pNameBox->GetCount () > 1 )
//	{
//		m_pNameBox->SetUseTextColor ( 0, TRUE );
//		m_pNameBox->SetTextColor ( 0, dwColor );
//	}
//}
/*void CNameDisplay::SetRankNameColor ( const D3DCOLOR& dwColor )
{
	if ( m_pNameBox->GetCount () > 1 )
	{
		m_pNameBox->SetUseTextColor ( 1, TRUE );
		m_pNameBox->SetTextColor ( 1, dwColor );
	}
}*/
bool CNameDisplay::DIFFERENT ( const CROWREN &sINFO )
{
	return m_sDISP_INFO.DIFFERENT(sINFO);
}

void CNameDisplay::UPDATE ( DWORD _dwCOUNT, D3DXVECTOR3 vPOS )
{
	m_sDISP_INFO.m_dwCOUNT	= _dwCOUNT;
	m_sDISP_INFO.m_vPOS		= vPOS;
}

int CNameDisplay::CalculateVal ( DWORD dwCalc )
{
	if  ( dwCalc >= GLCONST_CHAR::dwRank_Point[19] ) return 19;
	else if  ( dwCalc >= GLCONST_CHAR::dwRank_Point[18] ) return 18;
	else if  ( dwCalc >= GLCONST_CHAR::dwRank_Point[17] ) return 17;
	else if  ( dwCalc >= GLCONST_CHAR::dwRank_Point[16] ) return 16;
	else if  ( dwCalc >= GLCONST_CHAR::dwRank_Point[15] ) return 15;
	else if  ( dwCalc >= GLCONST_CHAR::dwRank_Point[14] ) return 14; 
	else if  ( dwCalc >= GLCONST_CHAR::dwRank_Point[13] ) return 13;
	else if  ( dwCalc >= GLCONST_CHAR::dwRank_Point[12] ) return 12;
	else if  ( dwCalc >= GLCONST_CHAR::dwRank_Point[11] ) return 11;
	else if  ( dwCalc >= GLCONST_CHAR::dwRank_Point[10] ) return 10;
	else if  ( dwCalc >= GLCONST_CHAR::dwRank_Point[9] ) return 9;
	else if  ( dwCalc >= GLCONST_CHAR::dwRank_Point[8] ) return 8;
	else if  ( dwCalc >= GLCONST_CHAR::dwRank_Point[7] ) return 7;
	else if  ( dwCalc >= GLCONST_CHAR::dwRank_Point[6] ) return 6;
	else if  ( dwCalc >= GLCONST_CHAR::dwRank_Point[5] ) return 5;
	else if  ( dwCalc >= GLCONST_CHAR::dwRank_Point[4] ) return 4;
	else if  ( dwCalc >= GLCONST_CHAR::dwRank_Point[3] ) return 3;
	else if  ( dwCalc >= GLCONST_CHAR::dwRank_Point[2] ) return 2;
	else if  ( dwCalc >= GLCONST_CHAR::dwRank_Point[1] ) return 1;
	else if  ( dwCalc >= GLCONST_CHAR::dwRank_Point[0] ) return 0;
	else return 0;
}
