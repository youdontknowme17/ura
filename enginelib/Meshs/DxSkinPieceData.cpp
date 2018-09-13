#include "stdafx.h"
#include "../Common/SerialFile.h"
#include "../DxCommon/EDITMESHS.h"
#include "../../EngineLib/Common/StringUtils.h"

#include "../G-Logic/GLogic.h"
#include "DxSkinPieceContainer.h"

#include "SAnimation.h"

#include "DxSkinPieceData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DxSkinPieceDataContainer& DxSkinPieceDataContainer::GetInstance()
{
	static DxSkinPieceDataContainer Instance;
	return Instance;
}

//-----------------------------------------------------------------------------------[DxSkinPieceData]
//
DxSkinPieceData::DxSkinPieceData(void) :
	m_dwRef(0),

	m_bWorldObj(FALSE),
	m_fScale(1.0f),
	m_vMax(7.f,20.f,7.f),
	m_vMin(-7.f,0.f,-7.f),
	m_fHeight(20.f),
	m_fRadius(7.f),
	m_dwBONE(0)

	,m_dwType(0)
	,m_dwWeaponSlot(0)

	,m_fRotX(0.0f)
	,m_fRotY(0.0f)
	,m_fRotZ(0.0f)

	,m_fRootX(0.0f)
	,m_fRootY(0.0f)
	,m_fRootZ(0.0f)

	,m_fRotX_F(0.0f)
	,m_fRotY_F(0.0f)
	,m_fRotZ_F(0.0f)

	,m_fRootX_F(0.0f)
	,m_fRootY_F(0.0f)
	,m_fRootZ_F(0.0f)
{
	memset( m_szFileName, 0, FILE_LENGTH );

	m_strAbf2 = "";
	m_strCPS2 = "";
	m_strSkeleton2 = "";
	m_strBoneLink2 = "";

	for ( int i=0; i<PIECE_SIZE; ++i )
	{
		m_pPieceArray[i] = NULL;
	}
	for ( DWORD n=0; n<MAX_BONE; ++n )
	{
		m_fBONE1[n] = 0.0f;
		m_fBONE2[n] = 0.0f;
		m_fBONE3[n] = 0.0f;
		m_strBONE[n] = "";
	}
}

DxSkinPieceData::~DxSkinPieceData(void)
{
	m_dwBONE = 0;
	ClearAll ();
}

HRESULT DxSkinPieceData::ClearAll ()
{
	m_dwRef = 0;
	m_fScale = 1.0f;

	m_vMax = D3DXVECTOR3(7,20,7);
	m_vMin = D3DXVECTOR3(-7,0,-7);

	m_vecANIFILE.clear();

	for ( int i=0; i<PIECE_SIZE; ++i )
	{
		//	Note : ���� �̹� ���� Piece Type �� ������ Piece �� �ִٸ� �����ش�.
		//
		if ( m_pPieceArray[i] )
		{
			DxSkinPieceContainer::GetInstance().ReleasePiece ( m_pPieceArray[i]->m_szFileName );
		}
		m_pPieceArray[i] = NULL;
	}

	DxSkinAniControl::ClearAll ();

	return S_OK;
}

HRESULT DxSkinPieceData::SetPiece ( const char* szFile, LPDIRECT3DDEVICEQ pd3dDevice, const BOOL bThread, DWORD dwFlag )
{
	//	Note : ���� ó�� �ε�� ���� �����ϰ� ������ �ε��Ҷ����� ���� �ε�Ȱ� �����ش�.
	//
	if ( dwFlag&FG_MUSTNEWLOAD )
	{
		for ( int i=0; i<PIECE_SIZE; ++i )
		{
			if ( m_pPieceArray[i] && m_pPieceArray[i]->m_szFileName )
			{
				if ( !strcmp(m_pPieceArray[i]->m_szFileName,szFile) )
				{
					m_pPieceArray[i] = NULL;
				}
			}
		}

		DxSkinPieceContainer::GetInstance().DeletePiece ( szFile );
	}

	//	Note : Piece�� �о�´�.
	//
	DxSkinPiece* pSkinPiece = DxSkinPieceContainer::GetInstance().LoadPiece( szFile, pd3dDevice, bThread );
	if ( !pSkinPiece )	return E_FAIL;

	if ( dwFlag&FG_BONEFILE_CHECK )
	{
		if ( pSkinPiece->m_pmcMesh->pSkinInfo )
		{
			CString strFile1( m_strSkeleton.c_str() );
			strFile1.MakeLower();

			CString strFile2( pSkinPiece->m_szSkeleton );
			strFile2.MakeLower();

			if( _tcscmp(strFile1.GetString(),strFile2.GetString()) != 0 )
			{
				CDebugSet::ToLogFile( "Bone ����ġ %s, %s", strFile1.GetString(), strFile2.GetString() );
				return E_DIFFERENT_BONEFILE;
			}
		}
	}

	//	Note : ���� �̹� ���� Piece Type �� ������ Piece �� �ִٸ� �����ش�.
	//
	if ( m_pPieceArray[pSkinPiece->m_emType] )
	{
		DxSkinPieceContainer::GetInstance().ReleasePiece ( szFile );
	}

	//	Note : Piece �� ����.
	//
	m_pPieceArray[pSkinPiece->m_emType] = pSkinPiece;

	return S_OK;
}

void DxSkinPieceData::ResetPiece ( LPDIRECT3DDEVICEQ pd3dDevice, int i )
{
	GASSERT(i<PIECE_SIZE);

	PDXSKINPIECE pSkinPiece = GetPiece (i);
	
	//	Note : ������ �÷��� Piece �� �ִٸ� �̸� �����ش�.
	//
	if ( pSkinPiece )
	{
		DxSkinPieceContainer::GetInstance().ReleasePiece ( pSkinPiece->m_szFileName );
		m_pPieceArray[i] = NULL;
	}
}

BOOL DxSkinPieceData::SaveFile_B ( const char* szFile, LPDIRECT3DDEVICEQ pd3dDevice )
{
	char szPathName[MAX_PATH] = "";
	StringCchCopy( szPathName, MAX_PATH, DxSkinPieceDataContainer::GetInstance().GetPath() );
	StringCchCat( szPathName, MAX_PATH, szFile );

	CSerialFile SFile;
	if ( !SFile.OpenFile ( FOT_WRITE, szPathName ) )		return FALSE;

	SFile << VERSION;

	SFile << m_fScale;

	SFile << PIECE_SIZE;
	for ( int i=0; i<PIECE_SIZE; ++i )
	{
		SFile << BOOL(m_pPieceArray[i]!=NULL);
		if ( !m_pPieceArray[i] )	continue;

		std::string strPieceFile = m_pPieceArray[i]->m_szFileName;
		SFile << strPieceFile;
	}

	SFile << m_strSkeleton;

	//	Note :���ϸ��̼� ����.
	//
	SFile << DxSkinAniControl::GETANIAMOUNT();
	
	for ( size_t n=0; n<m_vecAnim.size(); ++n )
	{
		std::string strNAME = m_vecAnim[n]->pAnimCont->m_szName;
		SFile << strNAME;
	}

	//	Note : ��� �ڽ� ���.
	//
	SFile << (BOOL) TRUE;
	//CalculateBoundingBox ( pd3dDevice, m_vMax, m_vMin );

	SFile << m_vMax;
	SFile << m_vMin;

	// Note : �߰�.
	SFile << (BOOL)TRUE;
	SFile << m_bWorldObj;

	// Note : �߰�.
	SFile << (BOOL)TRUE;
	SFile << m_fHeight;
	SFile << m_fRadius;

	//	Note : ����Ʈ�� �ٸ� �߰� ��ü�� �� ���� ����ؼ� ���� ����Ʈ�� ���� �Ǹ�
	//		TRUE ���� �����ϰ� �ȴ�.
	//
	SFile << (BOOL)FALSE;

	//	Note : ��Ų �ɸ��� �̸� ����.
	StringCchCopy( m_szFileName, FILE_LENGTH, szFile );

	return TRUE;
}

BOOL DxSkinPieceData::SaveFile ( const char* szFile, LPDIRECT3DDEVICEQ pd3dDevice )
{
	char szPathName[MAX_PATH] = "";
	StringCchCopy( szPathName, MAX_PATH, DxSkinPieceDataContainer::GetInstance().GetPath() );
	StringCchCat( szPathName, MAX_PATH, szFile );

	CSerialFile SFile;
	if ( !SFile.OpenFile ( FOT_WRITE, szPathName ) )		return FALSE;

	SFile << VERSION;

	SFile << m_fScale;

	SFile << PIECE_SIZE;
	for ( int i=0; i<PIECE_SIZE; ++i )
	{
		SFile << BOOL(m_pPieceArray[i]!=NULL);
		if ( !m_pPieceArray[i] )	continue;

		std::string strPieceFile = m_pPieceArray[i]->m_szFileName;
		SFile << strPieceFile;
	}

	SFile << m_strSkeleton;

	//	Note :���ϸ��̼� ����.
	//
	SFile << DxSkinAniControl::GETANIAMOUNT();
	
	for ( size_t n=0; n<m_vecAnim.size(); ++n )
	{
		std::string strNAME = m_vecAnim[n]->pAnimCont->m_szName;
		SFile << strNAME;
	}

	//	Note : ��� �ڽ� ���.
	//
	SFile << (BOOL) TRUE;
	//CalculateBoundingBox ( pd3dDevice, m_vMax, m_vMin );

	SFile << m_vMax;
	SFile << m_vMin;

	// Note : �߰�.
	SFile << (BOOL)TRUE;
	SFile << m_bWorldObj;

	// Note : �߰�.
	SFile << (BOOL)TRUE;
	SFile << m_fHeight;
	SFile << m_fRadius;

	//	Note : ����Ʈ�� �ٸ� �߰� ��ü�� �� ���� ����ؼ� ���� ����Ʈ�� ���� �Ǹ�
	//		TRUE ���� �����ϰ� �ȴ�.
	//
	SFile << (BOOL)FALSE;

	//	Note : ��Ų �ɸ��� �̸� ����.
	StringCchCopy( m_szFileName, FILE_LENGTH, szFile );

	return TRUE;
}

BOOL DxSkinPieceData::LOAD_Ver100( basestream &SFile, LPDIRECT3DDEVICEQ pd3dDevice )
{
    DWORD dwPieceSize;

    //SFile >> m_fScale;
    SFile >> dwPieceSize;
    if ( dwPieceSize > PIECE_SIZE )        return FALSE;

    //    Note : ?????? ????.
    //
    BOOL bExit;
    for ( DWORD i=0; i<dwPieceSize; ++i )
    {
        SFile >> bExit;
        if ( !bExit )
        {
            m_strPIECE[i].clear();
            continue;
        }

        SFile >> m_strPIECE[i];
    }

    //    Note : ?????? ????.
    //
    SFile >> m_strSkeleton;

    //    Note :?????????? ????.
    //
    DWORD dwAniNum(0);
    SFile >> dwAniNum;

    m_vecANIFILE.clear();
    m_vecANIFILE.reserve ( dwAniNum );

    for ( DWORD i=0; i<dwAniNum; ++i )
    {
        //std::string strANIFILE;
        //SFile >> strANIFILE;

        // GForceFX 17-06-2013 Fix ABL read .cfg extension animation 
        std::string strTEMP ;
        SFile >> strTEMP ;

        CString strANIFILE;
        bool bCFG;
        bCFG = STRUTIL::ChangeExt ( strTEMP.c_str(), ".cfg", strANIFILE, ".x" );
        if ( !bCFG )
        {
            strANIFILE = strTEMP.c_str();
        }
        m_vecANIFILE.push_back ( strANIFILE.GetString() );
    }
              
        SFile >> m_fScale;    

    BOOL bExist = FALSE;
    SFile >> bExist;
    if( bExist )    
    {
        SFile >> m_fHeight;
        SFile >> m_fRadius;
    }
    else            return TRUE;

    SFile >> m_dwBONE;

    if ( m_dwBONE > MAX_BONE ) return FALSE;

    for ( i=0; i < m_dwBONE ; ++i )
    {
        SFile >> m_fBONE1[i];
        SFile >> m_fBONE2[i];
        SFile >> m_fBONE3[i];
        SFile >> m_strBONE[i];
    }

    //    Note : ?????? ???? ???? ????.
    //

    BOOL bBoundBox(FALSE);
    SFile >> bBoundBox;
    if ( bBoundBox )
    {
        SFile >> m_vMax;
        SFile >> m_vMin;
    }
    else
    {
        m_vMax = D3DXVECTOR3(7.f,20.f,7.f);
        m_vMin = D3DXVECTOR3(-7.f,0.f,-7.f);
    }

    bExist = FALSE;
    SFile >> bExist;
    if( bExist )    SFile >> m_bWorldObj;
    else            return TRUE;

    // Note : ????
    SFile >> bExist;

    //CDebugSet::ToLogFile( "load version_0000 : %d asp okey" );

    return TRUE;
}


BOOL DxSkinPieceData::LoadFile ( const char* szFile, LPDIRECT3DDEVICEQ pd3dDevice, const BOOL bThread, bool bREPRESH )
{
	char szPathName[MAX_PATH] = "";
	StringCchCopy( szPathName, MAX_PATH, DxSkinPieceDataContainer::GetInstance().GetPath() );
	StringCchCat( szPathName, MAX_PATH, szFile );

	std::auto_ptr<basestream> pBStream( GLOGIC::openfile_basestream(GLOGIC::bENGLIB_ZIPFILE, 
																	GLOGIC::strSKINOBJ_ZIPFILE.c_str(), 
																	szPathName, 
																	szFile ) );

	if ( !pBStream.get() )
		return FALSE;

	basestream &SFile = *pBStream;

	// Note : Init Data
	m_fHeight = 20.f;
	m_fRadius = 7.f;

	DWORD dwVer;
	SFile >> dwVer;

	switch ( dwVer )
	{
	case VERSION:
		LOAD_Ver100( SFile, pd3dDevice );
		break;
	default:
		CString strTemp = "";
		strTemp.Format( "DxSkinPieceData Load Error:%s ver:%d" ,SFile.GetFileName(),dwVer);
		MessageBox (NULL,strTemp,"Need New Version",MB_OK);
		return FALSE;
	};


	//	Note : ������ �б�.
	//
	m_pSkeleton = DxBoneCollector::GetInstance().Load ( m_strSkeleton.c_str(), pd3dDevice );
	if ( !m_pSkeleton )		return FALSE;

	DWORD i;
	for ( i=0; i<PIECE_SIZE; ++i )
	{
		m_strPIECE_P[i] = m_strPIECE[i];
		if ( !m_strPIECE[i].empty() )
		{
			SetPiece ( m_strPIECE[i].c_str(), pd3dDevice, bThread );
		}
	}

	for ( i=0; i<(DWORD)m_vecANIFILE.size(); ++i )
	{
		BOOL bLoad = DxSkinAniControl::LoadAnimation ( m_vecANIFILE[i].c_str(), pd3dDevice, bREPRESH );
		//CDebugSet::ToLogFile( "Load Anim %d %s : %d",i,m_vecANIFILE[i].c_str(),bLoad );
	}

	DxSkinAniControl::ClassifyAnimation ();

	//	Note : �ʱ� ���ϸ��̼� ����.
	//
	DxSkinAniControl::SELECTANI ( AN_GUARD_N );

	if ( !DxSkinAniControl::GETCURANIMNODE() )
	{
		CDebugSet::ToLogFile ( "select ani fail %s", szFile );
	}

	//	Note : ��Ų �ɸ��� �̸� ����.
	StringCchCopy( m_szFileName, FILE_LENGTH, szFile );

	return TRUE;
}

HRESULT DxSkinPieceData::CalculateBoundingBox ( LPDIRECT3DDEVICEQ pd3dDevice, D3DXVECTOR3 &_vMax, D3DXVECTOR3 &_vMin )
{
	D3DXVECTOR3 vLastMax(-FLT_MAX,-FLT_MAX,-FLT_MAX), vLastMin(FLT_MAX,FLT_MAX,FLT_MAX);
	D3DXVECTOR3 vMax, vMin;

	for ( int i=0; i<PIECE_SIZE; ++i )
	{
		if ( !m_pPieceArray[i] )
		{
			continue;//	������ �Ϻκ� �������� �����Ƿ� ������ ������. ( �������� �Ǵ�. )
		}
		
		HRESULT hr = m_pPieceArray[i]->CalculateBoundingBox ( pd3dDevice, vMax, vMin );
		
		if ( FAILED(hr) )
		{
			continue;//	������ �Ϻκ� �������� �����Ƿ� ������ ������. ( �������� �Ǵ�. )
		}

		if ( vLastMax.x < vMax.x )	vLastMax.x = vMax.x;
		if ( vLastMax.y < vMax.y )	vLastMax.y = vMax.y;
		if ( vLastMax.z < vMax.z )	vLastMax.z = vMax.z;

		if ( vLastMin.x > vMin.x )	vLastMin.x = vMin.x;
		if ( vLastMin.y > vMin.y )	vLastMin.y = vMin.y;
		if ( vLastMin.z > vMin.z )	vLastMin.z = vMin.z;
	}

	_vMax = vLastMax;
	_vMin = vLastMin;

	return S_OK;
}

void DxSkinPieceData::SetRadiusHeight()
{
	m_vMax = D3DXVECTOR3( 0.f, 0.f, 0.f );
	m_vMin = D3DXVECTOR3( 0.f, 0.f, 0.f );

	// Note : ���� ����
	m_vMax.y = m_fHeight;

	// Note : ������ ����
	m_vMax.x = m_fRadius;
	m_vMax.z = m_fRadius;
	m_vMin.x = -m_fRadius;
	m_vMin.z = -m_fRadius;
}

void DxSkinPieceData::RenderAABBBox( LPDIRECT3DDEVICEQ pd3dDevice )
{
	if( !strlen( m_szFileName ) )	return;

	EDITMESHS::RENDERAABB( pd3dDevice, m_vMax, m_vMin );
}

//--------------------------------------------------------------------------[DxSkinPieceDataContainer]
//
DxSkinPieceDataContainer::DxSkinPieceDataContainer ()
{
	memset( m_szPath, 0, sizeof(char)*MAX_PATH );
}

DxSkinPieceDataContainer::~DxSkinPieceDataContainer ()
{
}

HRESULT DxSkinPieceDataContainer::CleanUp ()
{
	SKINCHARDATAMAP_ITER iter = m_mapSkinCharData.begin ();
	SKINCHARDATAMAP_ITER iterEnd = m_mapSkinCharData.end ();

	for ( ; iter!=iterEnd; iter++ )
	{
		(*iter).second->ClearAll();
		delete (*iter).second;
	}

	m_mapSkinCharData.clear ();

	return S_OK;
}

DxSkinPieceData* DxSkinPieceDataContainer::FindData ( const char* szFile )
{
	//CDebugSet::ToLogFile( "Find File %s",szFile );
	SKINCHARDATAMAP_ITER iter = m_mapSkinCharData.find ( std::string(szFile) );
	if ( iter != m_mapSkinCharData.end() )	return iter->second;

	return NULL;
}

BOOL DxSkinPieceDataContainer::DeleteData ( const char* szFile )
{
	SKINCHARDATAMAP_ITER iter = m_mapSkinCharData.find ( std::string(szFile) );
	if ( iter == m_mapSkinCharData.end() )	return FALSE;

	delete (*iter).second;
	m_mapSkinCharData.erase ( iter );

	return TRUE;
}

void DxSkinPieceDataContainer::ReleaseData ( const char* szFile )
{
	DxSkinPieceData* pRes = FindData ( szFile );
	if ( !pRes )	return;
	
	if ( pRes->m_dwRef<=1 )
	{
		DeleteData ( szFile );
		return;
	}

	pRes->m_dwRef--;
	return;
}

DxSkinPieceData* DxSkinPieceDataContainer::LoadData( const char* szFile, LPDIRECT3DDEVICEQ pd3dDevice, const BOOL bThread )
{
	DxSkinPieceData* pCharData;

	/*
	pCharData = FindData( szFile );
	if ( pCharData )
	{
		//CDebugSet::ToLogFile("Find File");
		pCharData->m_dwRef++;
		return pCharData;
	}
	*/

	pCharData = new DxSkinPieceData;
	BOOL bOk = pCharData->LoadFile( szFile, pd3dDevice, bThread );

	if ( !bOk )
	{
		SAFE_DELETE(pCharData);
		return NULL;
	}
	pCharData->m_dwRef++;

	CDebugSet::ToListView ( "DxSkinPieceData::LoadFile, %s", szFile );

	//CDebugSet::ToLogFile( "Save To %s",szFile );
	//m_mapSkinCharData[std::string( szFile )] = pCharData;

	return pCharData;
}





