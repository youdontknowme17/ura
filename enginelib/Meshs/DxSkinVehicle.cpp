#include "stdafx.h"

#include "DxSkinMeshMan.h"
#include "DxSkinDefine.h"

#include "../DxCommon/DxInputDevice.h"
//#include "../DxOctree/DxLandMan.h"
//#include "../DxCommon/DxLightMan.h"
#include "../DxEffect/DxEffectMan.h"
#include "../DxCommon/RENDERPARAM.h"
#include "../DxCommon/DxDynamicVB.h"

#include "../DxCommon/EDITMESHS.h"
//#include "../DxCommon/COLLISION.h"

#include "DxSkinVehicle.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD	DxSkinVehicle::g_dwLOD = 0;
DWORD	DxSkinVehicle::g_dwHIGHDRAW_NUM = 0;

extern BOOL g_bOBJECT100;
extern TSTRING	g_strPICK_BONE;

DxSkinVehicle::DxSkinVehicle(void) :
	m_fScale(1.0f),
	m_dwVertexNUM(0),
	m_dwBONE(0)
{
	memset( m_szFileName, 0, FILE_LENGTH );

	for ( DWORD n=0; n<MAX_BONE; ++n )
	{
		m_fBONE1[n] = 0.0f;
		m_fBONE2[n] = 0.0f;
		m_fBONE3[n] = 0.0f;
		m_strBONE[n] = "";
	}
}

DxSkinVehicle::~DxSkinVehicle(void)
{
	for ( DWORD n=0; n<m_dwBONE; ++n )
	{
		m_fBONE1[n] = 0.0f;
		m_fBONE2[n] = 0.0f;
		m_fBONE3[n] = 0.0f;
		m_strBONE[n] = "";
	}
	Reset();
	ClearAll();
	DeleteDeviceObjects();
}

HRESULT DxSkinVehicle::ClearAll ()
{
	m_fScale = 1.0f;

	m_EffAniPlayer.RemoveAllEff();

	for ( int i=0; i<PIECE_SIZE; ++i )
	{
		if ( m_PartArray[i].GetFileName() )
		{
			m_PartArray[i].ReSetPart ();
		}
	}

	DxSkinAniControl::ClearAll ();

	return S_OK;
}

void DxSkinVehicle::ResetPiece ( int i )
{
	GASSERT(i<PIECE_SIZE);

	if ( m_PartArray[i].GetFileName() )
	{
		m_PartArray[i].ReSetPart ();
	}
}

HRESULT DxSkinVehicle::SetPiece ( const TCHAR* szFile, LPDIRECT3DDEVICEQ pd3dDevice, DWORD dwFlag, int nLevel, bool bCharacterData /*= FALSE*/ )
{
	if ( strlen(szFile) == 0 || !szFile ) return E_FAIL;
	if ( dwFlag&FG_MUSTNEWLOAD )
	{
		for ( int i=0; i<PIECE_SIZE; ++i )
		{
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

	DxSkinPiece* pSkinPiece = DxSkinPieceContainer::GetInstance().LoadPiece( szFile, pd3dDevice, TRUE );
	if ( !pSkinPiece )
	{
		CDebugSet::ToLogFile( "Can't Load Piece %s",szFile );
		return E_FAIL;
	}

	if ( dwFlag&FG_BONEFILE_CHECK )
	{
		if ( _tcscmp(pSkinPiece->m_szSkeleton,m_strSkeleton.c_str()) )	return E_DIFFERENT_BONEFILE;
	}

	if ( m_PartArray[pSkinPiece->m_emType].GetFileName() )
	{
		m_PartArray[pSkinPiece->m_emType].ReSetPart ();
	}
	
	if( pSkinPiece->m_emType == PIECE_HAIR )
	{
		if( m_PartArray[PIECE_HEADGEAR].GetFileName() )
		{
			m_PartArray[PIECE_HEADGEAR].ReSetPart ();

			DxSkinPiece* pSkinPieceHG = DxSkinPieceContainer::GetInstance().LoadPiece( m_PartArray[PIECE_HEADGEAR].GetFileName(), pd3dDevice, TRUE );
			if ( !pSkinPieceHG )	goto _RETURN;
			m_PartArray[PIECE_HEADGEAR].SetPart( pSkinPieceHG, pd3dDevice, bCharacterData );
		}
	}

_RETURN:

	m_PartArray[pSkinPiece->m_emType].SetPart ( pSkinPiece, pd3dDevice, bCharacterData );
	m_PartArray[pSkinPiece->m_emType].SetGrindLevel( nLevel );
	
	CollectVertexNUM();

	return S_OK;
}

HRESULT DxSkinVehicle::SetPieceGrindLevel ( int nIndex, int nLevel )
{
	if ( PIECE_SIZE >= nIndex )	return E_FAIL;

	m_PartArray[nIndex].SetGrindLevel(nLevel);

	return S_OK;
}

void DxSkinVehicle::SetPartRend ( EMPIECECHAR emPIECE, BOOL bRender )
{
	m_PartArray[emPIECE].SetRender ( bRender );
}

void DxSkinVehicle::SetColor( WORD wColor, WORD wColor1, WORD wColor2, WORD wColor3, WORD wColor4, WORD wColor5 )
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
	//------------------------------------------------------------------------
	WORD wR1, wG1, wB1;
	wR1 = wColor1 >> 10;
	wG1 = wColor1 >> 5;
	wG1 = wG1&0x1f;
	wB1 = wColor1&0x1f;
	
	wR1 = (WORD)( wR1 * 8.225806f );	//<< 3;
	wG1 = (WORD)( wG1 * 8.225806f );	//<< 3;
	wB1 = (WORD)( wB1 * 8.225806f );	//<< 3;

	DWORD dwColor1 = 0xff000000 + (wR1<<16) + (wG1<<8) + wB1;
	//------------------------------------------------------------------------
	WORD wR2, wG2, wB2;
	wR2 = wColor2 >> 10;
	wG2 = wColor2 >> 5;
	wG2 = wG2&0x1f;
	wB2 = wColor2&0x1f;
	
	wR2 = (WORD)( wR2 * 8.225806f );	//<< 3;
	wG2 = (WORD)( wG2 * 8.225806f );	//<< 3;
	wB2 = (WORD)( wB2 * 8.225806f );	//<< 3;

	DWORD dwColor2 = 0xff000000 + (wR2<<16) + (wG2<<8) + wB2;
	//------------------------------------------------------------------------
	WORD wR3, wG3, wB3;
	wR3 = wColor3 >> 10;
	wG3 = wColor3 >> 5;
	wG3 = wG3&0x1f;
	wB3 = wColor3&0x1f;
	
	wR3 = (WORD)( wR3 * 8.225806f );	//<< 3;
	wG3 = (WORD)( wG3 * 8.225806f );	//<< 3;
	wB3 = (WORD)( wB3 * 8.225806f );	//<< 3;

	DWORD dwColor3 = 0xff000000 + (wR3<<16) + (wG3<<8) + wB3;
	//------------------------------------------------------------------------
	WORD wR4, wG4, wB4;
	wR4 = wColor4 >> 10;
	wG4 = wColor4 >> 5;
	wG4 = wG4&0x1f;
	wB4 = wColor4&0x1f;
	
	wR4 = (WORD)( wR4 * 8.225806f );	//<< 3;
	wG4 = (WORD)( wG4 * 8.225806f );	//<< 3;
	wB4 = (WORD)( wB4 * 8.225806f );	//<< 3;

	DWORD dwColor4 = 0xff000000 + (wR4<<16) + (wG4<<8) + wB4;
	//------------------------------------------------------------------------
	WORD wR5, wG5, wB5;
	wR5 = wColor5 >> 10;
	wG5 = wColor5 >> 5;
	wG5 = wG5&0x1f;
	wB5 = wColor5&0x1f;
	
	wR5 = (WORD)( wR5 * 8.225806f );	//<< 3;
	wG5 = (WORD)( wG5 * 8.225806f );	//<< 3;
	wB5 = (WORD)( wB5 * 8.225806f );	//<< 3;

	DWORD dwColor5 = 0xff000000 + (wR5<<16) + (wG5<<8) + wB5;
	//------------------------------------------------------------------------

	m_PartArray[0].SetHairColor( dwColor );
	m_PartArray[1].SetHairColor( dwColor1 );
	m_PartArray[2].SetHairColor( dwColor2 );
	m_PartArray[3].SetHairColor( dwColor3 );
	m_PartArray[4].SetHairColor( dwColor4 );
	m_PartArray[5].SetHairColor( dwColor5 );
}

void DxSkinVehicle::SetColorAll( WORD wColor )
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

	for ( int i=0; i<PIECE_REV01; ++i )
	{
		m_PartArray[i].SetHairColor( dwColor );
	}
}

void DxSkinVehicle::SetCharData( DxSkinVehicleData* pCharData, LPDIRECT3DDEVICEQ pd3dDevice, DWORD dwFlag, int nLevel, bool bCharacterData /* FALSE */)
{
	if ( !pCharData ) return ;

	StringCchCopy( m_szFileName, FILE_LENGTH, pCharData->GetFileName() );

	m_EffAniPlayer.InitDeviceObjects ( pd3dDevice );
	InitDeviceObjects( pd3dDevice );

	EMANI_MAINTYPE MType = DxSkinAniControl::GETCURMTYPE();
	EMANI_SUBTYPE SType = DxSkinAniControl::GETCURSTYPE();
	if ( MType == AN_NONE )	MType = AN_GUARD_N;

	DxSkinAniControl::ClearAll ();

	float fCurTime = DxSkinAniControl::GETCURTIME();

	m_fScale = pCharData->m_fScale;

	m_dwBONE = pCharData->m_dwBONE;
	for( DWORD n = 0 ; n < m_dwBONE ; ++n )
	{
		m_fBONE1[n] = pCharData->m_fBONE1[n];
		m_fBONE2[n] = pCharData->m_fBONE2[n];
		m_fBONE3[n] = pCharData->m_fBONE3[n];
		m_strBONE[n] = pCharData->m_strBONE[n];
	}

	m_strChar_Font = pCharData->m_strChar_Font;
	m_strChar_Back = pCharData->m_strChar_Back;

	m_vRotation_Font = pCharData->m_vRotation_Font;
	m_vTransform_Font = pCharData->m_vTransform_Font;

	m_vRotation_Back = pCharData->m_vRotation_Back;
	m_vTransform_Back = pCharData->m_vTransform_Back;

	for ( int i=0; i<PIECE_SIZE; ++i )
	{
		m_strPIECE_P[i] = pCharData->m_strPIECE[i];

		if ( pCharData->GetPiece(i) )
		{
			m_PartArray[i].SetPart ( pCharData->GetPiece(i), pd3dDevice, bCharacterData );
		}
		else
		{
			m_PartArray[i].ReSetPart ();
		}
	}

	m_strSkeleton = pCharData->GetSkeletonName();
	m_pSkeleton = DxBoneCollector::GetInstance().Load ( m_strSkeleton.c_str(), pd3dDevice );

	DxSkinAniControl::VECNODE& vecANI = pCharData->GetAniList ();
	size_t nSize = vecANI.size();
	for ( size_t n=0; n<nSize; ++n )
	{
		BOOL bLoad = DxSkinAniControl::LoadAnimation ( vecANI[n]->pAnimCont->m_szName, pd3dDevice );

		m_EffAniPlayer.CheckCreateEff( vecANI[n]->pAnimCont );
	}

	DxSkinAniControl::ClassifyAnimation ();

	DxSkinAniControl::SETCURTIME ( fCurTime );
	DxSkinAniControl::SELECTANI ( MType, SType );

	DxSkinAniControl::SELECTANI ( AN_PLACID, SType );

	if ( !GETCURANIMNODE() )
	{
		CDebugSet::ToLogFile ( _T("ERORR : current animation node null point error [ %s ] [ M %d S %d ]"), pCharData->GetFileName(), MType, SType );

		if( m_vecAnim.size() )		DEFAULTANI();
	}
	CollectVertexNUM();
}

void DxSkinVehicle::CollectVertexNUM()
{
	m_dwVertexNUM = 0;

	for( int i=0; i<PIECE_SIZE; ++i )
	{
		m_dwVertexNUM += m_PartArray[i].GetVertexNUM();
	}
}

void DxSkinVehicle::AvailableVB_InitVB()
{
	switch( RENDERPARAM::emCharRenderTYPE )
	{
	case EMCRT_SOFTWARE:

		DWORD dwVertexSizeFULL = m_dwVertexNUM * sizeof(VERTEX);
		if( DxDynamicVB::m_sVB_CHAR.nOffsetToLock + dwVertexSizeFULL > DxDynamicVB::m_sVB_CHAR.nFullByte )
		{
			DxDynamicVB::m_sVB_CHAR.nOffsetToLock = DxDynamicVB::m_sVB_CHAR.nFullByte+1;	
		}
		break;
	}
}

HRESULT DxSkinVehicle::FrameMove ( const float fTime, const float fElapsedTime, const BOOL bContinue, const BOOL bFreeze, const BOOL bAttackMode )
{
	int nPrevKeyTime = DxSkinAniControl::GetPrevKeyFrame();

	if ( bFreeze )		DxSkinAniControl::FrameMove ( 0.0f, bContinue );
	else				DxSkinAniControl::FrameMove ( fElapsedTime, bContinue );

	DxSkinAniControl::SetPrevKeyFrame();

	m_sSkinEffData.Reset();		

	for ( int i=0; i<PIECE_REV01; ++i )
	{
		m_PartArray[i].FrameMove ( fTime, fElapsedTime, GETCURMTYPE(), GETCURSTYPE(), GETCURKEYTIME() );
	}

	m_EffAniPlayer.FrameMove ( DxSkinAniControl::GETCURANIM(), fTime, fElapsedTime, m_sSkinEffData );
	m_EffAniPlayer.CheckCreateEff ( DxSkinAniControl::GETCURANIM(), nPrevKeyTime, DxSkinAniControl::GETCURKEYTIME() );

	m_EffKeepPlayer.FrameMove( fElapsedTime, m_sSkinEffData );

	return S_OK;
}

HRESULT DxSkinVehicle::Render ( const LPDIRECT3DDEVICEQ pd3dDevice, const D3DXMATRIX &matRot, const BOOL bShadow, const BOOL bEff, const BOOL bReverse )
{
	D3DXMATRIXA16	matLocalRot, matScale;
	D3DXMatrixIdentity( &matScale );
	matScale._11 = m_fScale;
	matScale._22 = m_fScale;
	matScale._33 = m_fScale;

	D3DXMatrixMultiply( &matLocalRot, &matScale, &matRot );

	DxSkinAniControl::Render ( matLocalRot );

	DxBoneTrans *pBoneCur = NULL;
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

	int nStart=0, nEnd = 0, nAdd = 0;
    
	nStart = 0;	
	nEnd = PIECE_REV01;
	nAdd = 1;

	int i = nStart;

	if( TRUE == FALSE )
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

	pd3dDevice->SetRenderState( D3DRS_FOGENABLE, dwFogEnable );

	pd3dDevice->SetLight ( 0, &sSrcLight00 );

	pd3dDevice->LightEnable( 1, FALSE );
	pd3dDevice->LightEnable( 2, FALSE );
	pd3dDevice->LightEnable( 3, FALSE );
	pd3dDevice->LightEnable( 4, FALSE );
	pd3dDevice->LightEnable( 5, FALSE );

	

	return S_OK;
}

HRESULT DxSkinVehicle::RenderSKIN( const LPDIRECT3DDEVICEQ pd3dDevice, const D3DXMATRIX& matLocalRot )
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

HRESULT DxSkinVehicle::RenderShadow( const LPDIRECT3DDEVICEQ pd3dDevice, const D3DXMATRIX &matRot, const BOOL bDay )
{
	DWORD dwFogEnable;
	pd3dDevice->GetRenderState( D3DRS_FOGENABLE, &dwFogEnable );
	pd3dDevice->SetRenderState( D3DRS_FOGENABLE, FALSE );

	float fHeight=0.f;
	if( !bDay )
	{
		fHeight = -matRot._42;
	}

	for ( int i=0; i<PIECE_REV01; ++i )
	{
		m_PartArray[i].RenderShadow( pd3dDevice, m_pSkeleton, fHeight );
	}

	pd3dDevice->SetRenderState( D3DRS_FOGENABLE, dwFogEnable );

	return S_OK;
}

HRESULT DxSkinVehicle::RenderReflect( const LPDIRECT3DDEVICEQ pd3dDevice, const D3DXMATRIX &matRot )
{
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

	for ( int i=0; i<PIECE_REV01; ++i )
	{
		m_PartArray[i].Render( pd3dDevice, m_pSkeleton, m_sSkinEffData, FALSE, FALSE );
	}

	pd3dDevice->SetRenderState( D3DRS_NORMALIZENORMALS, dwNormalizeNormals );
	pd3dDevice->SetRenderState( D3DRS_FOGENABLE, dwFogEnable );

	return S_OK;
}

void DxSkinVehicle::RenderBone( LPDIRECT3DDEVICEQ pd3dDevice )
{
	if( !m_pSkeleton )	return;

	pd3dDevice->SetRenderState( D3DRS_ZENABLE,		FALSE );
	pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,	FALSE );

	m_pSkeleton->EditLineSphere( pd3dDevice );

	pd3dDevice->SetRenderState( D3DRS_ZENABLE,		TRUE );
	pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,	TRUE );

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

HRESULT DxSkinVehicle::InitDeviceObjects( LPDIRECT3DDEVICEQ pd3dDevice )
{
	// Note : m_PartArray, m_EffAniPlayer �� �ٸ� ��ο��� �ʱ�ȭ�� �� �ش�.

	return S_OK;
}

HRESULT DxSkinVehicle::RestoreDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	for ( int i=0; i<PIECE_REV01; ++i )
	{
		m_PartArray[i].RestoreDeviceObjects ( pd3dDevice );
	}

	m_EffAniPlayer.RestoreDeviceObjects ();

	return S_OK;
}

HRESULT DxSkinVehicle::InvalidateDeviceObjects ()
{
	for ( int i=0; i<PIECE_REV01; ++i )
	{
		m_PartArray[i].InvalidateDeviceObjects ();
	}

	m_EffAniPlayer.InvalidateDeviceObjects ();

	return S_OK;
}

HRESULT DxSkinVehicle::DeleteDeviceObjects ()
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
void DxSkinVehicle::CreateEff( DxEffKeepDataMain* pEff )
{
	m_EffKeepPlayer.CreateEff( pEff );
}

void DxSkinVehicle::DeleteEff( DxEffKeepDataMain* pEff )
{
	
}

void DxSkinVehicle::Reset()
{
	m_EffKeepPlayer.Reset();
}
