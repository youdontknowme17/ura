#include "stdafx.h"

#include "../Common/SerialFile.h"
#include "DxSkinMeshMan.h"
#include "DxSkinDefine.h"

#include "../DxCommon/DxInputDevice.h"
#include "../G-Logic/GLPeriod.h"
#include "../DxOctree/DxLandMan.h"
#include "../DxCommon/DxLightMan.h"
#include "../DxEffect/DxEffectMan.h"
#include "../DxCommon/RENDERPARAM.h"
#include "../DxCommon/DxDynamicVB.h"
#include "GLCharDefine.h"
#include "../DxCommon/EDITMESHS.h"
#include "../DxCommon/COLLISION.h"

#include "DxSkinChar.h"
//add abl element by CNDev
#include "../DxEffect/Char/DxEffSelfEffect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD	DxSkinChar::g_dwLOD = 0;
DWORD	DxSkinChar::g_dwHIGHDRAW_NUM = 0;

extern BOOL g_bOBJECT100;
extern TSTRING	g_strPICK_BONE;

DxSkinChar::DxSkinChar(void) :
	m_fScale(1.0f),
	m_fHeight(20.f),
	m_bWorldObj(FALSE),
	m_dwVertexNUM(0),
	m_dwBONE(0),			//add abl
	m_bBigHead(FALSE),		//add abl
	m_pSkinVehicle(NULL),	//add vcf
	m_fLengthSRC(0.f)		//add vcf
{
	memset( m_szFileName, 0, FILE_LENGTH );

	//add abl
	for ( int i=0; i<PIECE_SIZE; ++i )
	{
		m_pSkinCharPiece[i] = NULL;
		m_strBoneLink[i] = "";
		m_dwWeaponSlot[i] = 0;
		m_dwPieceType[i] = 0;
	}

	for ( DWORD n=0; n<MAX_BONE; ++n )
	{
		m_fBONE1[n] = 0.0f;
		m_fBONE2[n] = 0.0f;
		m_fBONE3[n] = 0.0f;
		m_strBONE[n] = "";
	}
}

DxSkinChar::~DxSkinChar(void)
{
	//add abl
	m_dwBONE = 0;
	for ( int i=0; i<PIECE_SIZE; ++i )
	{
		m_strBoneLink[i] = "";
		m_dwWeaponSlot[i] = 0;
		m_dwPieceType[i] = 0;
		SAFE_DELETE( m_pSkinCharPiece[ i ] );
	}
}

//add abl
HRESULT DxSkinChar::Clear( EMPIECECHAR emType )
{
	if ( emType > PIECE_SIZE ) return E_FAIL;

	m_strBoneLink[emType] = "";
	m_dwWeaponSlot[emType] = 0;
	m_dwPieceType[emType] = 0;
	SAFE_DELETE( m_pSkinCharPiece[ emType ] );

	if ( !m_pSkinCharPiece[PIECE_RHAND] )
	{
		m_strBoneLink[PIECE_LHAND] = "";
		m_dwWeaponSlot[PIECE_LHAND] = 0;
		m_dwPieceType[PIECE_LHAND] = 0;
		SAFE_DELETE( m_pSkinCharPiece[ PIECE_LHAND ] );
	}
	//Extreme Reboot
	if ( !m_pSkinCharPiece[PIECE_REBOOT_RHAND] )
	{
		m_strBoneLink[PIECE_REBOOT_LHAND] = "";
		m_dwWeaponSlot[PIECE_REBOOT_LHAND] = 0;
		m_dwPieceType[PIECE_REBOOT_LHAND] = 0;
		SAFE_DELETE( m_pSkinCharPiece[ PIECE_REBOOT_LHAND ] );
	}

	return S_OK;
}

HRESULT DxSkinChar::SetPiece ( const TCHAR* szFile, LPDIRECT3DDEVICEQ pd3dDevice, DWORD dwFlag, int nLevel, bool bCharacterData /*= FALSE*/, std::string strEffect /*= "" */)
{
	//	Note : ���� ó�� �ε�� ���� �����ϰ� ������ �ε��Ҷ����� ���� �ε�Ȱ� �����ش�.
	//
	if ( dwFlag&FG_MUSTNEWLOAD )
	{
		for ( int i=0; i<PIECE_SIZE; ++i )
		{
			SAFE_DELETE( m_pSkinCharPiece[ i ] );//add abl

			if ( m_PartArray[i].GetFileName() )
			{
				if ( !_tcscmp(m_PartArray[i].GetFileName(),szFile) )
				{
					m_PartArray[i].ReSetPart ();
				}
			}
		}

		DxSkinPieceContainer::GetInstance().DeletePiece ( szFile );
	}

	//	Note : Piece�� �о�´�.
	//
	DxSkinPiece* pSkinPiece = DxSkinPieceContainer::GetInstance().LoadPiece( szFile, pd3dDevice, TRUE );
	if ( !pSkinPiece )	return E_FAIL;

	if ( dwFlag&FG_BONEFILE_CHECK )
	{
		if ( _tcscmp(pSkinPiece->m_szSkeleton,m_strSkeleton.c_str()) )	return E_DIFFERENT_BONEFILE;
	}

	//add abl
	SAFE_DELETE( m_pSkinCharPiece[ pSkinPiece->m_emType ] );
	//Extreme Reboot
	if ( pSkinPiece->m_emType == PIECE_RHAND || pSkinPiece->m_emType == PIECE_LHAND || 
		pSkinPiece->m_emType == PIECE_REBOOT_RHAND || pSkinPiece->m_emType == PIECE_REBOOT_LHAND)
	{
		//LEFT
		m_strBoneLink[PIECE_LHAND] = "";
		m_dwWeaponSlot[PIECE_LHAND] = 0;
		m_dwPieceType[PIECE_LHAND] = 0;
		SAFE_DELETE( m_pSkinCharPiece[ PIECE_LHAND ] );
		//RIGHT
		m_strBoneLink[PIECE_RHAND] = "";
		m_dwWeaponSlot[PIECE_RHAND] = 0;
		m_dwPieceType[PIECE_RHAND] = 0;
		SAFE_DELETE( m_pSkinCharPiece[ PIECE_RHAND ] );
		//Extreme Reboot
		m_strBoneLink[PIECE_REBOOT_LHAND] = "";
		m_dwWeaponSlot[PIECE_REBOOT_LHAND] = 0;
		m_dwPieceType[PIECE_REBOOT_LHAND] = 0;
		SAFE_DELETE( m_pSkinCharPiece[ PIECE_REBOOT_LHAND ] );
		//Extreme Reboot
		m_strBoneLink[PIECE_REBOOT_RHAND] = "";
		m_dwWeaponSlot[PIECE_REBOOT_RHAND] = 0;
		m_dwPieceType[PIECE_REBOOT_RHAND] = 0;
		SAFE_DELETE( m_pSkinCharPiece[ PIECE_REBOOT_RHAND ] );
	}
	else
	{
		SAFE_DELETE( m_pSkinCharPiece[ pSkinPiece->m_emType ] );
		m_strBoneLink[pSkinPiece->m_emType] = "";
		m_dwWeaponSlot[pSkinPiece->m_emType] = 0;
		m_dwPieceType[pSkinPiece->m_emType] = 0;
	}

	if ( !m_pSkinCharPiece[PIECE_RHAND] )
	{
		SAFE_DELETE( m_pSkinCharPiece[ PIECE_LHAND ] );
		m_strBoneLink[PIECE_LHAND] = "";
		m_dwWeaponSlot[PIECE_LHAND] = 0;
		m_dwPieceType[PIECE_LHAND] = 0;
	}
	//Extreme Reboot
	if ( !m_pSkinCharPiece[PIECE_REBOOT_RHAND] )
	{
		SAFE_DELETE( m_pSkinCharPiece[ PIECE_REBOOT_LHAND ] );
		m_strBoneLink[PIECE_REBOOT_LHAND] = "";
		m_dwWeaponSlot[PIECE_REBOOT_LHAND] = 0;
		m_dwPieceType[PIECE_REBOOT_LHAND] = 0;
	}

	//	Note : ���� �̹� ���� Piece Type �� ������ Piece �� �ִٸ� �����ش�.
	//
	//dual spear 
	if ( pSkinPiece->m_emType == PIECE_RHAND )
	{
		if ( m_PartArray[PIECE_LHAND].GetFileName() )
		{
			m_PartArray[PIECE_LHAND].ReSetPart ();
		}
	}
	else
	{
		if ( m_PartArray[pSkinPiece->m_emType].GetFileName() )
		{
			m_PartArray[pSkinPiece->m_emType].ReSetPart ();
		}
	}
	
	// Note : ���ڿ� ������ �̻��ϰ� �Ǿ �̷��� �ؾ� �Ѵ�.
	if( pSkinPiece->m_emType == PIECE_HAIR )
	{
		if( m_PartArray[PIECE_HEADGEAR].GetFileName() )
		{
			// �ϴ� ����
			m_PartArray[PIECE_HEADGEAR].ReSetPart ();

			// ������ ����
			DxSkinPiece* pSkinPieceHG = DxSkinPieceContainer::GetInstance().LoadPiece( m_PartArray[PIECE_HEADGEAR].GetFileName(), pd3dDevice, TRUE );
			if ( !pSkinPieceHG )	goto _RETURN;
			m_PartArray[PIECE_HEADGEAR].SetPart( pSkinPieceHG, pd3dDevice, bCharacterData );
		}
	}

_RETURN:

	//	Note : Piece �� ����.
	//
	m_PartArray[pSkinPiece->m_emType].SetPart ( pSkinPiece, pd3dDevice, bCharacterData );
	m_PartArray[pSkinPiece->m_emType].SetGrindLevel( nLevel );
	
	//add abl element by CNDev
	if( strEffect.size() != 0 )	
	{
		DxEffSelfEffectMan::GetInstance().PutEffect ( &m_PartArray[pSkinPiece->m_emType], strEffect.c_str() );
	}
	// Note : ���� ������ �� ����
	CollectVertexNUM();

	return S_OK;
}
//add upgradeeffect
HRESULT DxSkinChar::SetGradeEffect ( int nType , EMCHARINDEX nClassIndex, int nUpgradeColor, LPDIRECT3DDEVICEQ pd3dDevice , bool bCharacterData )
{
	int nSex = 0;
	switch ( nClassIndex )
	{
	case GLCI_FIGHTER_M:
	case GLCI_ARMS_M:
	case GLCI_EXTREME_M:
	case GLCI_ARCHER_M:
	case GLCI_SPIRIT_M:
	case GLCI_SCIENCE_M:
	case GLCI_ASSASIN_M:
	case GLCI_TESTING_M:
	case GLCI_SHAPER_M:		nSex = 1;	break;
	default:	nSex = 0;	break;
	};

	CString strCps;
	//strCps.Format ("Effect_%d_%d_%d.cps",nSex,nType,nUpgradeColor);

	std::string strSex[2] = 
	{
		"w",
		"m",
	};

	std::string strType[4] = 
	{
		"body",
		"leg",
		"hand",
		"foot",
	};

	strCps.Format ("+%d_%s_%s.cps",4+nUpgradeColor,strSex[nSex].c_str(),strType[nType].c_str());

	DxSkinPiece* pSkinPiece = DxSkinPieceContainer::GetInstance().LoadPiece( strCps.GetString(), pd3dDevice, TRUE );
	if ( !pSkinPiece )	return E_FAIL;

	if ( m_PartArray[pSkinPiece->m_emType].GetFileName() )
	{
		m_PartArray[pSkinPiece->m_emType].ReSetPart ();
	}
	
	StringCchCopy( pSkinPiece->m_szFileName,FILE_LENGTH, strCps.GetString() );
	
	m_PartArray[pSkinPiece->m_emType].SetPart ( pSkinPiece, pd3dDevice, bCharacterData );
	m_PartArray[pSkinPiece->m_emType].SetGrindLevel( nUpgradeColor );
	
	CollectVertexNUM();
	return S_OK;
}
HRESULT DxSkinChar::SetPieceGrindLevel ( int nIndex, int nLevel )
{
	if ( PIECE_SIZE >= nIndex )	return E_FAIL;

	m_PartArray[nIndex].SetGrindLevel(nLevel);

	return S_OK;
}

void DxSkinChar::SetHairColor( WORD wColor )
{
	WORD wR, wG, wB;
	wR = wColor >> 10;
	wG = wColor >> 5;
	wG = wG&0x1f;
	wB = wColor&0x1f;
	
	wR = (WORD)( wR * 8.225806f );	//<< 3;
	wG = (WORD)( wG * 8.225806f );	//<< 3;
	wB = (WORD)( wB * 8.225806f );	//<< 3;

	DWORD dwColor = 0xff000000 + (wR<<16) + (wG<<8) + wB;

	m_PartArray[PIECE_HAIR].SetHairColor( dwColor );
}

WORD DxSkinChar::GetHairColor()
{
	DWORD dwColor = m_PartArray[PIECE_HAIR].GetHairColor();

	// ���� ����
	WORD wR, wG, wB;
	wR = (WORD)((dwColor&0xff0000)>>16);
	wG = (WORD)((dwColor&0xff00)>>8);
	wB = (WORD)(dwColor&0xff);

	// �����
	wR = wR >> 3;
	wG = wG >> 3;
	wB = wB >> 3;
	return (wR<<10) + (wG<<5) + wB;
}
//add item color
void DxSkinChar::SetColor( EMPIECECHAR emIndex , WORD wColor )
{
	WORD wR, wG, wB;
	wR = wColor >> 10;
	wG = wColor >> 5;
	wG = wG&0x1f;
	wB = wColor&0x1f;
	
	wR = (WORD)( wR * 8.225806f );	//<< 3;
	wG = (WORD)( wG * 8.225806f );	//<< 3;
	wB = (WORD)( wB * 8.225806f );	//<< 3;

	DWORD dwColor = 0xff000000 + (wR<<16) + (wG<<8) + wB;

	m_PartArray[emIndex].SetHairColor( dwColor );
	//enable coloring for abl also
	if ( m_pSkinCharPiece[emIndex] ) m_pSkinCharPiece[emIndex]->SetColor(dwColor);
}

void DxSkinChar::SetColorAll( WORD wColor )
{
	WORD wR, wG, wB;
	wR = wColor >> 10;
	wG = wColor >> 5;
	wG = wG&0x1f;
	wB = wColor&0x1f;
	
	wR = (WORD)( wR * 8.225806f );	//<< 3;
	wG = (WORD)( wG * 8.225806f );	//<< 3;
	wB = (WORD)( wB * 8.225806f );	//<< 3;

	DWORD dwColor = 0xff000000 + (wR<<16) + (wG<<8) + wB;

	for ( int i=0; i<PIECE_SIZE; ++i )
	{
		if ( i != PIECE_HAIR )
		{
			m_PartArray[i].SetHairColor( dwColor );
		}
	}
}

HRESULT DxSkinChar::ClearAll ()
{
	m_fScale = 1.0f;
	m_bWorldObj = FALSE;

	m_EffAniPlayer.RemoveAllEff();

	for ( int i=0; i<PIECE_SIZE; ++i )
	{
		//	Note : ���� �̹� ���� Piece Type �� ������ Piece �� �ִٸ� �����ش�.
		//
		if ( m_PartArray[i].GetFileName() )
		{
			m_PartArray[i].ReSetPart ();
		}
		//add abl
		m_strBoneLink[i] = "";
		m_dwWeaponSlot[i] = 0;
		m_dwPieceType[i] = 0;
		SAFE_DELETE( m_pSkinCharPiece[ i ] );
	}

	DxSkinAniControl::ClearAll ();

	return S_OK;
}

void DxSkinChar::ResetPiece ( int i )
{
	GASSERT(i<PIECE_SIZE);

	//add abl
	SAFE_DELETE( m_pSkinCharPiece[ i ] );

	m_strBoneLink[i] = "";
	m_dwWeaponSlot[i] = 0;
	m_dwPieceType[i] = 0;

	if ( !m_pSkinCharPiece[PIECE_RHAND] )
	{
		SAFE_DELETE( m_pSkinCharPiece[ PIECE_LHAND ] );
		m_strBoneLink[PIECE_LHAND] = "";
		m_dwWeaponSlot[PIECE_LHAND] = 0;
		m_dwPieceType[PIECE_LHAND] = 0;
	}
	//Extreme Reboot
	if ( !m_pSkinCharPiece[PIECE_REBOOT_RHAND] )
	{
		SAFE_DELETE( m_pSkinCharPiece[ PIECE_REBOOT_LHAND ] );
		m_strBoneLink[PIECE_REBOOT_LHAND] = "";
		m_dwWeaponSlot[PIECE_REBOOT_LHAND] = 0;
		m_dwPieceType[PIECE_REBOOT_LHAND] = 0;
	}

	if ( m_PartArray[i].GetFileName() )
	{
		m_PartArray[i].ReSetPart ();
	}
}

void DxSkinChar::ResetVCF ()
{
	SAFE_DELETE( m_pSkinVehicle );//add vcf 
}

void DxSkinChar::ResetCPS ( int i )
{
	if ( m_PartArray[i].GetFileName() )
	{
		m_PartArray[i].ReSetPart ();
	}
}

void DxSkinChar::ResetABL ( int i )
{
	GASSERT(i<PIECE_SIZE);

	//add abl
	SAFE_DELETE( m_pSkinCharPiece[ i ] );

	m_strBoneLink[i] = "";
	m_dwWeaponSlot[i] = 0;
	m_dwPieceType[i] = 0;

	if ( !m_pSkinCharPiece[PIECE_RHAND] )
	{
		SAFE_DELETE( m_pSkinCharPiece[ PIECE_LHAND ] );
		m_strBoneLink[PIECE_LHAND] = "";
		m_dwWeaponSlot[PIECE_LHAND] = 0;
		m_dwPieceType[PIECE_LHAND] = 0;
	}
	//Extreme Reboot
	if ( !m_pSkinCharPiece[PIECE_REBOOT_RHAND] )
	{
		SAFE_DELETE( m_pSkinCharPiece[ PIECE_REBOOT_LHAND ] );
		m_strBoneLink[PIECE_REBOOT_LHAND] = "";
		m_dwWeaponSlot[PIECE_REBOOT_LHAND] = 0;
		m_dwPieceType[PIECE_REBOOT_LHAND] = 0;
	}

}

void DxSkinChar::SetPartRend ( EMPIECECHAR emPIECE, BOOL bRender )///
{
	m_PartArray[emPIECE].SetRender ( bRender );
}

void DxSkinChar::SetCharData ( DxSkinCharData* pCharData, LPDIRECT3DDEVICEQ pd3dDevice, bool bCharaterData /*= FALSE*/ )
{
	if ( !pCharData ) return; //add abl

	StringCchCopy( m_szFileName, FILE_LENGTH, pCharData->GetFileName() );

	m_EffAniPlayer.InitDeviceObjects ( pd3dDevice );

	EMANI_MAINTYPE MType = DxSkinAniControl::GETCURMTYPE();
	EMANI_SUBTYPE SType = DxSkinAniControl::GETCURSTYPE();
	if ( MType == AN_NONE )	MType = AN_GUARD_N;

	//add vcf
	if ( m_pSkinVehicle )
	{
		m_pSkinVehicle->SELECTANI( AN_GUARD_N , AN_SUB_NONE );
	}

	DxSkinAniControl::ClearAll ();

	float fCurTime = DxSkinAniControl::GETCURTIME();

	m_fScale = pCharData->m_fScale;
	m_vMax = pCharData->m_vMax;
	m_vMin = pCharData->m_vMin;
	m_fHeight = pCharData->m_fHeight;

	m_bWorldObj = pCharData->m_bWorldObj;

	//add abl
	m_dwBONE = pCharData->m_dwBONE;
	for( DWORD n = 0 ; n < m_dwBONE ; ++n )
	{
		m_fBONE1[n] = pCharData->m_fBONE1[n];
		m_fBONE2[n] = pCharData->m_fBONE2[n];
		m_fBONE3[n] = pCharData->m_fBONE3[n];
		m_strBONE[n] = pCharData->m_strBONE[n];
	}

	for ( int i=0; i<PIECE_SIZE; ++i )
	{
		//add abl
		SAFE_DELETE( m_pSkinCharPiece[ i ] );

		if ( pCharData->m_pSkinCharPiece[i] )
		{
			//set
			m_pSkinCharPiece[i] = pCharData->m_pSkinCharPiece[i];
			m_strBoneLink[i] = pCharData->m_strBoneLink[i];
			m_dwWeaponSlot[i] = pCharData->m_dwWeaponSlot[i];
			m_dwPieceType[i] = pCharData->m_dwPieceType[i];
			continue;
		}

		if ( pCharData->GetPiece(i) )
		{
			m_PartArray[i].SetPart ( pCharData->GetPiece(i), pd3dDevice, bCharaterData );
		}
		else
		{
			m_PartArray[i].ReSetPart ();
		}
	}

	//	Note : ���ϸ��̼� ����.
	//
	m_strSkeleton = pCharData->GetSkeletonName();
	m_pSkeleton = DxBoneCollector::GetInstance().Load ( m_strSkeleton.c_str(), pd3dDevice );

	DxSkinAniControl::VECNODE& vecANI = pCharData->GetAniList ();
	size_t nSize = vecANI.size();
	for ( size_t n=0; n<nSize; ++n )
	{
		DxSkinAniControl::LoadAnimation ( vecANI[n]->pAnimCont->m_szName, pd3dDevice );

		m_EffAniPlayer.CheckCreateEff( vecANI[n]->pAnimCont );	
	}

	DxSkinAniControl::ClassifyAnimation ();

	//	Note : �ʱ� ���ϸ��̼� ����.
	//
	DxSkinAniControl::SETCURTIME ( fCurTime );
	DxSkinAniControl::SELECTANI ( MType, SType );

	if ( !GETCURANIMNODE() )
	{
		DxSkinAniControl::SELECTANI ( AN_PLACID, SType );
	}

	if ( !GETCURANIMNODE() )
	{
		CDebugSet::ToLogFile ( _T("ERORR : current animation node null point error [ %s ] [ M %d S %d ]"), pCharData->GetFileName(), MType, SType );

		if( m_vecAnim.size() )		DEFAULTANI();
	}

	CollectVertexNUM();
}
//add abl element by CNDev
BOOL DxSkinChar::SetCharPieceData( DxSkinPieceData* pCharData, LPDIRECT3DDEVICEQ pd3dDevice , std::string szBoneLink , DWORD WeaponSlot , DWORD PieceType , DWORD dwFlag, int nLevel, bool bCharacterData , std::string strEffect /*= "" */)
{
	if ( !pCharData || PieceType > PIECE_SIZE ) return FALSE;

	SAFE_DELETE( m_pSkinCharPiece[ PieceType ] );
	if ( PieceType == PIECE_RHAND )
	{
		SAFE_DELETE( m_pSkinCharPiece[ PIECE_LHAND ] );
	}
	//Extreme Reboot
	if ( PieceType == PIECE_REBOOT_RHAND )
	{
		SAFE_DELETE( m_pSkinCharPiece[ PIECE_REBOOT_LHAND ] );
	}

	m_strBoneLink[ PieceType ] = szBoneLink;
	m_dwWeaponSlot[ PieceType ] = WeaponSlot;
	m_dwPieceType[ PieceType ] = PieceType;

	m_pSkinCharPiece[ PieceType ] = new DxSkinCharPiece;
	m_pSkinCharPiece[ PieceType ]->SetCharData( pCharData , pd3dDevice, dwFlag, nLevel, bCharacterData );
	//add abl element by CNDev
	if( strEffect.size() != 0 )	
	{
		DxEffSelfEffectMan::GetInstance().PutEffect ( m_pSkinCharPiece[ PieceType ], strEffect.c_str() );
	}

	return TRUE;
}

//add vcf 
HRESULT DxSkinChar::SetVehicleData( DxSkinVehicleData* pData, LPDIRECT3DDEVICEQ pd3dDevice, bool bCharaterData )
{
	SAFE_DELETE( m_pSkinVehicle );
	if ( !pData ) return E_FAIL;
	m_pSkinVehicle = new DxSkinVehicle;
	m_pSkinVehicle->SetCharData( pData , pd3dDevice , bCharaterData );
	return S_OK;
}
void DxSkinChar::SetRadiusHeight( DxSkinCharData* pCharData )
{
	m_vMin = pCharData->m_vMin;
	m_vMax = pCharData->m_vMax;
	m_fHeight = pCharData->m_fHeight;
}

void DxSkinChar::CollectVertexNUM()
{
	m_dwVertexNUM = 0;

	for( int i=0; i<PIECE_SIZE; ++i )
	{
		m_dwVertexNUM += m_PartArray[i].GetVertexNUM();
	}
}

void DxSkinChar::AvailableVB_InitVB()
{
	switch( RENDERPARAM::emCharRenderTYPE )
	{
	case EMCRT_SOFTWARE:

		// Note : �迭���� ũ�ٸ� ȸ��.~!
		//			������ �������� �༭ ���� �ٷ� ���� dwFlag = D3DLOCK_DISCARD; üũ�� �� �� �ֵ��� �����ش�.
		DWORD dwVertexSizeFULL = m_dwVertexNUM * sizeof(VERTEX);
		if( DxDynamicVB::m_sVB_CHAR.nOffsetToLock + dwVertexSizeFULL > DxDynamicVB::m_sVB_CHAR.nFullByte )
		{
			//DxDynamicVB::m_sVB_CHAR.nVertexCount = 0;
			DxDynamicVB::m_sVB_CHAR.nOffsetToLock = DxDynamicVB::m_sVB_CHAR.nFullByte+1;	
		}
		break;
	}
}

HRESULT DxSkinChar::FrameMove ( const float fTime, const float fElapsedTime, const BOOL bContinue, const BOOL bFreeze, const BOOL bAttackMode )
{
	int nPrevKeyTime = DxSkinAniControl::GetPrevKeyFrame();

	if ( bFreeze )		DxSkinAniControl::FrameMove ( 0.0f, bContinue );
	else				DxSkinAniControl::FrameMove ( fElapsedTime, bContinue );

	DxSkinAniControl::SetPrevKeyFrame();

	m_sSkinEffData.Reset();		// Data Reset.

	float fSkinAniElap = fElapsedTime; //add abl

	for ( int i=0; i<PIECE_REV01; ++i )
	{
		m_PartArray[i].FrameMove ( fTime, fElapsedTime, GETCURMTYPE(), GETCURSTYPE(), GETCURKEYTIME() );

		if ( m_pSkinCharPiece[i] ) //add abl
		{
			m_pSkinCharPiece[i]->FrameMove( fTime, fSkinAniElap, bContinue, bFreeze, bAttackMode );
		}
	}

	//add vcf 
	if ( m_pSkinVehicle )
	{
		m_pSkinVehicle->FrameMove( fTime, fSkinAniElap, bContinue, bFreeze, bAttackMode );
	}

	m_PartArray[PIECE_RHAND].SetAttackMode ( bAttackMode );
	m_PartArray[PIECE_LHAND].SetAttackMode ( bAttackMode );
	//Extreme Reboot
	m_PartArray[PIECE_REBOOT_RHAND].SetAttackMode ( bAttackMode );
	m_PartArray[PIECE_REBOOT_LHAND].SetAttackMode ( bAttackMode );

	m_EffAniPlayer.FrameMove ( DxSkinAniControl::GETCURANIM(), fTime, fElapsedTime, m_sSkinEffData );
	m_EffAniPlayer.CheckCreateEff ( DxSkinAniControl::GETCURANIM(), nPrevKeyTime, DxSkinAniControl::GETCURKEYTIME() );

	// Note : Edge Color ����Ʈ ����.
	m_EffKeepPlayer.FrameMove( fElapsedTime, m_sSkinEffData );

	return S_OK;
}

HRESULT DxSkinChar::Render ( const LPDIRECT3DDEVICEQ pd3dDevice, const D3DXMATRIX &matRot, const BOOL bShadow, const BOOL bEff, const BOOL bReverse )
{
	D3DXMATRIXA16	matLocalRot, matScale;
	D3DXMatrixIdentity( &matScale );
	matScale._11 = m_fScale;
	matScale._22 = m_fScale;
	matScale._33 = m_fScale;
	D3DXMatrixMultiply( &matLocalRot, &matScale, &matRot );

	//	Note : Skin Animation.
	DxSkinAniControl::Render ( matLocalRot );

	//add mouse effect
	DxBoneTrans *pBoneCur = NULL;

	if ( m_bBigHead ) //add abl
	{
		DxBoneTrans *pBoneCur = m_pSkeleton->FindBone( _T("Bip01_Head") );
		if ( pBoneCur )
		{
#ifdef USE_ANI_QUATERNION
			pBoneCur->m_pQuatPosCUR.m_vScale.x *= m_fBIGHEAD;
			pBoneCur->m_pQuatPosCUR.m_vScale.y *= m_fBIGHEAD;
			pBoneCur->m_pQuatPosCUR.m_vScale.z *= m_fBIGHEAD;
			pBoneCur->m_pQuatPosPREV.m_vScale.x *= m_fBIGHEAD;
			pBoneCur->m_pQuatPosPREV.m_vScale.y *= m_fBIGHEAD;
			pBoneCur->m_pQuatPosPREV.m_vScale.z *= m_fBIGHEAD;
#else
			float fScale = 1.f/m_fBIGHEAD;
			pBoneCur->matRot._41 *= fScale;
			pBoneCur->matRot._42 *= fScale;
			pBoneCur->matRot._43 *= fScale;
			pBoneCur->matRot._44 *= fScale;
#endif
			}
		m_pSkeleton->UpdateBones( m_pSkeleton->pBoneRoot, matRot );
	}

	//add abl
	for ( DWORD n=0; n<m_dwBONE; ++n )
	{
		pBoneCur = m_pSkeleton->FindBone( m_strBONE[n].c_str() );
		if ( pBoneCur )
		{
			pBoneCur->m_pQuatPosCUR.m_vScale.x *= m_fBONE1[n];
			pBoneCur->m_pQuatPosCUR.m_vScale.y *= m_fBONE2[n];
			pBoneCur->m_pQuatPosCUR.m_vScale.z *= m_fBONE3[n];
			pBoneCur->m_pQuatPosPREV.m_vScale.x *= m_fBONE1[n];
			pBoneCur->m_pQuatPosPREV.m_vScale.y *= m_fBONE2[n];
			pBoneCur->m_pQuatPosPREV.m_vScale.z *= m_fBONE3[n];
		}
	}

	if ( m_dwBONE > 0 )
		m_pSkeleton->UpdateBones( m_pSkeleton->pBoneRoot, matRot );



	g_dwLOD = 0;
#ifdef USE_SKINMESH_LOD
	// Note : �׸����� ���� �������� �Ѹ���.
	if( bShadow )
	{
		g_dwLOD = 1;
	}
	else if( g_dwHIGHDRAW_NUM > 10 )
	{
		// Note : �Ѱ谡 �Ѿ��� ��� �ּ��� ������ ��� �ϴ� �ֵ��� ����
		D3DXVECTOR3 vPos;
		D3DXVECTOR3& vFromPt = DxViewPort::GetInstance().GetFromPt();
		vPos.x = matLocalRot._41;
		vPos.y = matLocalRot._42;
		vPos.z = matLocalRot._43;

		float fLengthSRC = DxViewPort::GetInstance().GetDistance();
		//fLengthSRC += 30.f;
		//add vcf 
		fLengthSRC += m_fLengthSRC;
		fLengthSRC = fLengthSRC*fLengthSRC;
		
		D3DXVECTOR3 vDir = vPos - vFromPt;
		float fLength = vDir.x*vDir.x + vDir.y*vDir.y + vDir.z*vDir.z;
		if( fLength > fLengthSRC )	// �Ÿ��� (ī�޶�Ÿ�+30.f) �̻��� ��� LOD ������ �ٲ۴�.
		{
			g_dwLOD = 1;
		}
	}
	else
	{
		D3DXVECTOR3 vPos;
		D3DXVECTOR3& vFromPt = DxViewPort::GetInstance().GetFromPt();
		vPos.x = matLocalRot._41;
		vPos.y = matLocalRot._42;
		vPos.z = matLocalRot._43;

		float fLengthSRC = DxViewPort::GetInstance().GetDistance();
		//fLengthSRC += 150.f;
		//add vcf
		fLengthSRC += m_fLengthSRC;
		fLengthSRC = fLengthSRC*fLengthSRC;
		
		D3DXVECTOR3 vDir = vPos - vFromPt;
		float fLength = vDir.x*vDir.x + vDir.y*vDir.y + vDir.z*vDir.z;
		if( fLength > fLengthSRC )	// �Ÿ��� (ī�޶�Ÿ�+150.f) �̻��� ��� LOD ������ �ٲ۴�.
		{
			g_dwLOD = 1;
		}
	}

	if( g_dwLOD==0 )
	{
		++g_dwHIGHDRAW_NUM;
	}
#endif
	
	int nStart=0, nEnd = 0, nAdd = 0;
    
	// �ǽ� ������ �Ųٷ� �׸���. ( �� ���� )
	if ( bReverse )
	{
		nStart = PIECE_VEHICLE;
		nEnd = -1;
		nAdd = -1;
	}
	else
	{
		nStart = 0;	
		nEnd = PIECE_REV01;
		nAdd = 1;
	}

	int i = nStart;


	// Note : ������ ��ȭ�� ���� �ʴ´�.
	if( m_bWorldObj )
	{
		DWORD	dwNormalizeNormals;
		pd3dDevice->GetRenderState( D3DRS_NORMALIZENORMALS, &dwNormalizeNormals );
		pd3dDevice->SetRenderState( D3DRS_NORMALIZENORMALS, TRUE );
		{
			AvailableVB_InitVB();
				
			i = nStart;
			while ( i != nEnd )
			{
				m_PartArray[i].Render ( pd3dDevice, m_pSkeleton, m_sSkinEffData, bShadow, bEff );
				i += nAdd;
			}

			i = nStart;
			while ( i != nEnd )
			{
				m_PartArray[i].RenderGhosting ( pd3dDevice, this, matLocalRot );
				i += nAdd;

			}

			m_EffAniPlayer.Render ( pd3dDevice, this, matLocalRot );
		}
		pd3dDevice->SetRenderState( D3DRS_NORMALIZENORMALS, dwNormalizeNormals );


		//	Note : Glow 
		for ( int i=0; i<PIECE_REV01; ++i )
		{
			m_PartArray[i].RenderGlow( pd3dDevice, m_pSkeleton );
		}

		//	Note : Single Eff
		for ( int i=0; i<PIECE_REV01; ++i )
		{
			m_PartArray[i].RenderEff( pd3dDevice, m_pSkeleton, m_fScale );
		}

		return S_OK;
	}

	D3DLIGHTQ	sSrcLight00;
	D3DLIGHTQ	sDestLight00;

	pd3dDevice->GetLight ( 0, &sSrcLight00 );

	sDestLight00 = sSrcLight00;

	sDestLight00.Ambient.r += sSrcLight00.Diffuse.r * 0.2f;
	sDestLight00.Ambient.g += sSrcLight00.Diffuse.g * 0.2f;
	sDestLight00.Ambient.b += sSrcLight00.Diffuse.b * 0.2f;
	sDestLight00.Ambient.r += sSrcLight00.Ambient.r * 0.2f;
	sDestLight00.Ambient.g += sSrcLight00.Ambient.g * 0.2f;
	sDestLight00.Ambient.b += sSrcLight00.Ambient.b * 0.2f;
	sDestLight00.Diffuse.r = sSrcLight00.Diffuse.r * 0.6f;
	sDestLight00.Diffuse.g = sSrcLight00.Diffuse.g * 0.6f;
	sDestLight00.Diffuse.b = sSrcLight00.Diffuse.b * 0.6f;

	//sDestLight01.Ambient.r += sSrcLight01.Diffuse.r * 0.15f;
	//sDestLight01.Ambient.g += sSrcLight01.Diffuse.g * 0.15f;
	//sDestLight01.Ambient.b += sSrcLight01.Diffuse.b * 0.15f;
	//sDestLight01.Diffuse.r = sSrcLight01.Diffuse.r * 0.75f;	//0.9f;	// Item ������ ���� �����.
	//sDestLight01.Diffuse.g = sSrcLight01.Diffuse.g * 0.75f;	//0.9f;	// Item ������ ���� �����.
	//sDestLight01.Diffuse.b = sSrcLight01.Diffuse.b * 0.75f;	//0.9f;	// Item ������ ���� �����.

	//sDestLight02.Ambient.r += sSrcLight02.Diffuse.r * 0.15f;
	//sDestLight02.Ambient.g += sSrcLight02.Diffuse.g * 0.15f;
	//sDestLight02.Ambient.b += sSrcLight02.Diffuse.b * 0.15f;
	//sDestLight02.Diffuse.r = sSrcLight02.Diffuse.r * 0.75f;	//0.9f;	// Item ������ ���� �����.
	//sDestLight02.Diffuse.g = sSrcLight02.Diffuse.g * 0.75f;	//0.9f;	// Item ������ ���� �����.
	//sDestLight02.Diffuse.b = sSrcLight02.Diffuse.b * 0.75f;	//0.9f;	// Item ������ ���� �����.

	pd3dDevice->SetLight ( 0, &sDestLight00 );

	pd3dDevice->LightEnable( 1, FALSE );
	pd3dDevice->LightEnable( 2, FALSE );
	pd3dDevice->LightEnable( 3, FALSE );
	pd3dDevice->LightEnable( 4, FALSE );
	pd3dDevice->LightEnable( 5, FALSE );
	pd3dDevice->LightEnable( 6, FALSE );
	pd3dDevice->LightEnable( 7, FALSE );

	D3DXVECTOR3 vCharPos( matLocalRot._41, matLocalRot._42, matLocalRot._43 );
	DxLightMan::GetInstance()->SetCharStaticPLight( pd3dDevice, vCharPos );
	DxLightMan::GetInstance()->SetCharDynamicPLight( pd3dDevice, vCharPos );

	DWORD	dwFogEnable;
	pd3dDevice->GetRenderState( D3DRS_FOGENABLE, &dwFogEnable );
	pd3dDevice->SetRenderState( D3DRS_FOGENABLE, FALSE );

	{
		DWORD	dwNormalizeNormals;
		pd3dDevice->GetRenderState( D3DRS_NORMALIZENORMALS, &dwNormalizeNormals );
		pd3dDevice->SetRenderState( D3DRS_NORMALIZENORMALS, TRUE );

		AvailableVB_InitVB();

		switch( m_sSkinEffData.m_bNewHeadDRAW )
		{
		case TRUE:

			i = nStart;
			while ( i != nEnd )
			{
				m_PartArray[i].Render( pd3dDevice, m_pSkeleton, m_sSkinEffData, bShadow, bEff );
				i += nAdd;
			}	
			break;

		case FALSE:
			i = nStart;
			while ( i != nEnd )
			{
				m_PartArray[i].Render( pd3dDevice, m_pSkeleton, m_sSkinEffData, bShadow, bEff );
				i += nAdd;
			}
			break;
		};
		
		for ( int i=0; i<PIECE_REV01; ++i )
		{
			m_PartArray[i].RenderGhosting ( pd3dDevice, this, matLocalRot );
		}
		m_EffAniPlayer.Render( pd3dDevice, this, matLocalRot );
		m_EffKeepPlayer.Render( pd3dDevice );

		pd3dDevice->SetRenderState( D3DRS_NORMALIZENORMALS, dwNormalizeNormals );
	}

	//	Note : Glow 
	for ( int i=0; i<PIECE_REV01; ++i )
	{
		m_PartArray[i].RenderGlow( pd3dDevice, m_pSkeleton );
	}

	//	Note : Single Eff
	for ( int i=0; i<PIECE_REV01; ++i )
	{
		m_PartArray[i].RenderEff( pd3dDevice, m_pSkeleton, m_fScale );
	}

	for ( int iil=0; iil<PIECE_SIZE; ++iil )
	{
		if ( m_pSkinCharPiece[iil] )
		{
			pBoneCur = m_pSkeleton->FindBone ( m_strBoneLink[iil].c_str() );

			if ( iil == PIECE_RHAND || iil == PIECE_LHAND ||
				 iil == PIECE_REBOOT_RHAND || iil == PIECE_REBOOT_LHAND )
				if ( m_pSkinCharPiece[iil]->m_bAttackLocal == FALSE )
					pBoneCur = m_pSkeleton->FindBone ( SKINTYPE_STING::m_szWHEREBACKTYPE[ m_dwWeaponSlot[iil] ] );

			if ( pBoneCur && m_pSkinCharPiece[iil] )
				m_pSkinCharPiece[iil]->Render( pd3dDevice , pBoneCur->matCombined );
		}
	}
	pd3dDevice->SetRenderState( D3DRS_FOGENABLE, dwFogEnable );

	pd3dDevice->SetLight ( 0, &sSrcLight00 );

	pd3dDevice->LightEnable( 1, FALSE );
	pd3dDevice->LightEnable( 2, FALSE );
	pd3dDevice->LightEnable( 3, FALSE );
	pd3dDevice->LightEnable( 4, FALSE );
	pd3dDevice->LightEnable( 5, FALSE );

	

	return S_OK;

	//EDITMESHS::RENDERAABB ( pd3dDevice, m_vMax, m_vMin );

	//float	fDirectDiffuseFact = 0.6f;	//0.7f;	// Item ������ ���� �����.
	//float	fDirectAmbientFact = 0.25f;	//0.3f;	// Item ������ ���� �����.

	////	�� �� ����. �����׸��ڰ� ���� ���� ��츸 <������ ���� ĳ���͸� ��Ӱ�> �Ѵ�.
	//if ( GLPeriod::GetInstance().IsOffLight() && DxEffectMan::GetInstance().GetSSDetail() )
	//{
	//	DxLandMan*	pLandMan = DxEffectMan::GetInstance().GetLandMan();
	//	if ( pLandMan )
	//	{
	//		D3DXVECTOR3	vDirect;
	//		vDirect = DxLightMan::GetInstance()->GetDirectLight()->m_Light.Direction;
	//		vDirect.x *= -10000.f;
	//		vDirect.y *= -10000.f;
	//		vDirect.z *= -10000.f;

	//		BOOL		bCollision;
	//		D3DXVECTOR3	vCollision;
	//		D3DXVECTOR3	vPoint1(matLocalRot._41+vDirect.x,matLocalRot._42+8.f+vDirect.y,matLocalRot._43+vDirect.z);
	//		D3DXVECTOR3	vPoint2(matLocalRot._41,matLocalRot._42+8.f,matLocalRot._43);
	//		LPDXFRAME	pDxFrame = NULL;

	//		pLandMan->IsCollision ( vPoint1, vPoint2, vCollision, bCollision, pDxFrame );

	//		if ( bCollision )	// ���𰡿� ��Ҵٸ�... ��Ӱ� �����.
	//		{
	//			fDirectDiffuseFact *= 0.7f;
	//			fDirectAmbientFact *= 0.7f;
	//		}
	//	}
	//}
}

//add vcf
HRESULT DxSkinChar::RenderVehicle( const LPDIRECT3DDEVICEQ pd3dDevice, const D3DXMATRIX &matRot, const BOOL bShadow, const BOOL bEff, const BOOL bReverse )
{
	if ( m_pSkinVehicle )
	{
		m_pSkinVehicle->Render( pd3dDevice , matRot );
	}
	return S_OK;
}

HRESULT DxSkinChar::RenderSKIN( const LPDIRECT3DDEVICEQ pd3dDevice, const D3DXMATRIX& matLocalRot )
{
	D3DLIGHTQ	sSrcLight00;
	D3DLIGHTQ	sDestLight00;

	pd3dDevice->GetLight ( 0, &sSrcLight00 );

	sDestLight00 = sSrcLight00;

	sDestLight00.Ambient.r += sSrcLight00.Diffuse.r * 0.2f;
	sDestLight00.Ambient.g += sSrcLight00.Diffuse.g * 0.2f;
	sDestLight00.Ambient.b += sSrcLight00.Diffuse.b * 0.2f;
	sDestLight00.Ambient.r += sSrcLight00.Ambient.r * 0.2f;
	sDestLight00.Ambient.g += sSrcLight00.Ambient.g * 0.2f;
	sDestLight00.Ambient.b += sSrcLight00.Ambient.b * 0.2f;
	sDestLight00.Diffuse.r = sSrcLight00.Diffuse.r * 0.6f;
	sDestLight00.Diffuse.g = sSrcLight00.Diffuse.g * 0.6f;
	sDestLight00.Diffuse.b = sSrcLight00.Diffuse.b * 0.6f;

	pd3dDevice->SetLight ( 0, &sDestLight00 );

	pd3dDevice->LightEnable( 1, FALSE );
	pd3dDevice->LightEnable( 2, FALSE );
	pd3dDevice->LightEnable( 3, FALSE );
	pd3dDevice->LightEnable( 4, FALSE );
	pd3dDevice->LightEnable( 5, FALSE );
	pd3dDevice->LightEnable( 6, FALSE );
	pd3dDevice->LightEnable( 7, FALSE );

	D3DXVECTOR3 vCharPos( matLocalRot._41, matLocalRot._42, matLocalRot._43 );
	DxLightMan::GetInstance()->SetCharStaticPLight( pd3dDevice, vCharPos );
	DxLightMan::GetInstance()->SetCharDynamicPLight( pd3dDevice, vCharPos );

	DWORD	dwFogEnable;
	pd3dDevice->GetRenderState( D3DRS_FOGENABLE, &dwFogEnable );
	pd3dDevice->SetRenderState( D3DRS_FOGENABLE, FALSE );

	{
		DWORD	dwNormalizeNormals;
		pd3dDevice->GetRenderState( D3DRS_NORMALIZENORMALS, &dwNormalizeNormals );
		pd3dDevice->SetRenderState( D3DRS_NORMALIZENORMALS, TRUE );

		AvailableVB_InitVB();

		switch( m_sSkinEffData.m_bNewHeadDRAW )
		{
		case TRUE:
			for ( int i=1; i<PIECE_REV01; ++i )
			{
				m_PartArray[i].Render( pd3dDevice, m_pSkeleton, m_sSkinEffData, FALSE, TRUE );
			}
			break;

		case FALSE:
			for ( int i=0; i<PIECE_REV01; ++i )
			{
				m_PartArray[i].Render( pd3dDevice, m_pSkeleton, m_sSkinEffData, FALSE, TRUE );
			}
			break;
		};
		
		for ( int i=0; i<PIECE_REV01; ++i )
		{
			m_PartArray[i].RenderGhosting ( pd3dDevice, this, matLocalRot );
		}
		m_EffAniPlayer.Render( pd3dDevice, this, matLocalRot );
		m_EffKeepPlayer.Render( pd3dDevice );

		pd3dDevice->SetRenderState( D3DRS_NORMALIZENORMALS, dwNormalizeNormals );
	}

	//	Note : Glow 
	for ( int i=0; i<PIECE_REV01; ++i )
	{
		m_PartArray[i].RenderGlow( pd3dDevice, m_pSkeleton );
	}

	//	Note : Single Eff
	for ( int i=0; i<PIECE_REV01; ++i )
	{
		m_PartArray[i].RenderEff( pd3dDevice, m_pSkeleton, m_fScale );
	}

	pd3dDevice->SetRenderState( D3DRS_FOGENABLE, dwFogEnable );

	pd3dDevice->SetLight ( 0, &sSrcLight00 );

	pd3dDevice->LightEnable( 1, FALSE );
	pd3dDevice->LightEnable( 2, FALSE );
	pd3dDevice->LightEnable( 3, FALSE );
	pd3dDevice->LightEnable( 4, FALSE );
	pd3dDevice->LightEnable( 5, FALSE );

	return S_OK;
}

HRESULT DxSkinChar::RenderShadow( const LPDIRECT3DDEVICEQ pd3dDevice, const D3DXMATRIX &matRot, const BOOL bDay )
{
	//add vcf
	if ( m_pSkinVehicle )
	{
		m_pSkinVehicle->RenderShadow( pd3dDevice , matRot , bDay );
	}

	DWORD dwFogEnable;
	pd3dDevice->GetRenderState( D3DRS_FOGENABLE, &dwFogEnable );
	pd3dDevice->SetRenderState( D3DRS_FOGENABLE, FALSE );

	float fHeight=0.f;
	if( !bDay )
	{
		fHeight = -matRot._42;
	}

	//add abl
	DxBoneTrans* pSlotHeld = NULL;

	for ( int iil=0; iil<PIECE_SIZE; ++iil )
	{
		if ( m_pSkinCharPiece[iil] )
		{
			if ( iil == PIECE_RHAND || iil == PIECE_LHAND ||
				 iil == PIECE_REBOOT_RHAND || iil == PIECE_REBOOT_LHAND )
			{
				if ( m_pSkinCharPiece[iil]->m_bAttackLocal )
					pSlotHeld = m_pSkeleton->FindBone ( m_strBoneLink[iil].c_str() );
				else
				{
					pSlotHeld = m_pSkeleton->FindBone ( SKINTYPE_STING::m_szWHEREBACKTYPE[ m_dwWeaponSlot[iil] ] );
				}
			}else{
				pSlotHeld = m_pSkeleton->FindBone ( m_strBoneLink[iil].c_str() );
			}

			if ( pSlotHeld && m_pSkinCharPiece[iil] )
			{
				m_pSkinCharPiece[iil]->RenderShadow( pd3dDevice , pSlotHeld->matCombined , bDay );
			}
		}
	}

	for ( int i=0; i<PIECE_REV01; ++i )
	{
		m_PartArray[i].RenderShadow( pd3dDevice, m_pSkeleton, fHeight );
	}

	pd3dDevice->SetRenderState( D3DRS_FOGENABLE, dwFogEnable );

	return S_OK;
}

HRESULT DxSkinChar::RenderReflect( const LPDIRECT3DDEVICEQ pd3dDevice, const D3DXMATRIX &matRot )
{
	//add vcf
	if ( m_pSkinVehicle )
	{
		m_pSkinVehicle->RenderReflect( pd3dDevice , matRot );
	}

	D3DXMATRIX	matLocalRot, matScale;
	D3DXMatrixIdentity( &matScale );
	matScale._11 = m_fScale;
	matScale._22 = m_fScale;
	matScale._33 = m_fScale;
	D3DXMatrixMultiply( &matLocalRot, &matScale, &matRot );

	//	Note : Skin Animation.
	DxSkinAniControl::Render ( matLocalRot );

	DWORD dwFogEnable;
	DWORD	dwNormalizeNormals;
	pd3dDevice->GetRenderState( D3DRS_NORMALIZENORMALS, &dwNormalizeNormals );
	pd3dDevice->SetRenderState( D3DRS_NORMALIZENORMALS, TRUE );
	pd3dDevice->GetRenderState( D3DRS_FOGENABLE, &dwFogEnable );
	pd3dDevice->SetRenderState( D3DRS_FOGENABLE, FALSE );


	DxBoneTrans* pSlotHeld = NULL; //add abl

	for ( int iil=0; iil<PIECE_SIZE; ++iil )
	{
		if ( m_pSkinCharPiece[iil] )
		{
			if ( iil == PIECE_RHAND || iil == PIECE_LHAND ||
				 iil == PIECE_REBOOT_RHAND || iil == PIECE_REBOOT_LHAND )
			{
				if ( m_pSkinCharPiece[iil]->m_bAttackLocal )
					pSlotHeld = m_pSkeleton->FindBone ( m_strBoneLink[iil].c_str() );
				else
				{
					pSlotHeld = m_pSkeleton->FindBone ( SKINTYPE_STING::m_szWHEREBACKTYPE[ m_dwWeaponSlot[iil] ] );
				}
			}else{
				pSlotHeld = m_pSkeleton->FindBone ( m_strBoneLink[iil].c_str() );
			}

			if ( pSlotHeld && m_pSkinCharPiece[iil] )
			{
				m_pSkinCharPiece[iil]->RenderReflect( pd3dDevice , pSlotHeld->matCombined );
			}
		}
	}


	for ( int i=0; i<PIECE_REV01; ++i )
	{
		m_PartArray[i].Render( pd3dDevice, m_pSkeleton, m_sSkinEffData, FALSE, FALSE );
	}

	pd3dDevice->SetRenderState( D3DRS_NORMALIZENORMALS, dwNormalizeNormals );
	pd3dDevice->SetRenderState( D3DRS_FOGENABLE, dwFogEnable );

	return S_OK;
}

void DxSkinChar::RenderBone( LPDIRECT3DDEVICEQ pd3dDevice )
{
	if( !m_pSkeleton )	return;

	pd3dDevice->SetRenderState( D3DRS_ZENABLE,		FALSE );
	pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,	FALSE );

	m_pSkeleton->EditLineSphere( pd3dDevice );

	pd3dDevice->SetRenderState( D3DRS_ZENABLE,		TRUE );
	pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,	TRUE );

	// Note : ���� Ŭ���ϱ� ����.
    if( DxInputDevice::GetInstance().GetMouseState(DXMOUSE_LEFT)&DXKEY_UP )
	{
		// Note : ��ŷ�� �Ͽ� üũ �� Bone Name�� ��´�.
		//			���� ��ŷ ������ ���� ���Ҵ�.
		D3DXVECTOR3 vTargetPt, vFromPt;
		vFromPt = DxViewPort::GetInstance().GetFromPt ();
		BOOL bTargetted = DxViewPort::GetInstance().GetMouseTargetPosWnd ( vTargetPt );
		if ( bTargetted )
		{
			m_pSkeleton->IsCollision( vFromPt, vTargetPt, g_strPICK_BONE, 0.006f*DxViewPort::GetInstance().GetDistance() );
			CDebugSet::ToView( 9, _T("Select Bone : %s"), g_strPICK_BONE.c_str() );
		}
	}

	if( m_pSkeleton && !g_bOBJECT100 )	
	{
		pd3dDevice->SetRenderState( D3DRS_ZENABLE,		FALSE );
		pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,	FALSE );

		m_pSkeleton->CheckSphere( pd3dDevice, g_strPICK_BONE.c_str() );

		pd3dDevice->SetRenderState( D3DRS_ZENABLE,		TRUE );
		pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,	TRUE );
	}
}

//add mouse effect
D3DXMATRIX DxSkinChar::GetMarLocal(  const D3DXMATRIX &matRot )
{
	D3DXMATRIXA16	matLocalRot, matScale;
	D3DXMatrixScaling( &matScale , m_vMax.x/4.5f , m_vMax.y/4.5f , m_vMax.z/4.5f );
	D3DXMatrixMultiply( &matLocalRot, &matScale, &matRot );
	return matLocalRot;
}

HRESULT DxSkinChar::InitDeviceObjects( LPDIRECT3DDEVICEQ pd3dDevice )
{
	// Note : m_PartArray, m_EffAniPlayer �� �ٸ� ��ο��� �ʱ�ȭ�� �� �ش�.

	return S_OK;
}

HRESULT DxSkinChar::RestoreDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	for ( int i=0; i<PIECE_REV01; ++i )
	{
		m_PartArray[i].RestoreDeviceObjects ( pd3dDevice );
	}

	m_EffAniPlayer.RestoreDeviceObjects ();

	return S_OK;
}

HRESULT DxSkinChar::InvalidateDeviceObjects ()
{
	for ( int i=0; i<PIECE_REV01; ++i )
	{
		m_PartArray[i].InvalidateDeviceObjects ();
	}

	m_EffAniPlayer.InvalidateDeviceObjects ();

	return S_OK;
}

HRESULT DxSkinChar::DeleteDeviceObjects ()
{
	for ( int i=0; i<PIECE_REV01; ++i )
	{
		m_PartArray[i].DeleteDeviceObjects ();
	}

	m_EffAniPlayer.DeleteDeviceObjects ();

	return S_OK;
}

//------------------------------------------------------------
//				E	f	f		K	e	e	p
void DxSkinChar::CreateEff( DxEffKeepDataMain* pEff )
{
	m_EffKeepPlayer.CreateEff( pEff );
}

void DxSkinChar::DeleteEff( DxEffKeepDataMain* pEff )
{
	
}

void DxSkinChar::Reset()
{
	m_EffKeepPlayer.Reset();
}