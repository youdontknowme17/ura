#include "stdafx.h"
#include "../Common/SerialFile.h"

#include "../G-Logic/GLogic.h"

#include "DxSkinPieceRootData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DxSkinPieceRootData::DxSkinPieceRootData(void):
	m_dwType(0)
	,m_dwWeaponSlot(0)

	,m_bCPS(FALSE)

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
	m_strAbf = "";
	m_strCPS = "";
	m_strSkeleton = "";
	m_strBoneLink = "";
}

DxSkinPieceRootData::~DxSkinPieceRootData(void)
{
}

HRESULT DxSkinPieceRootData::ClearAll ()
{
	SAFE_DELETE( m_pSkinPieceData );
	DxSkinPieceDataContainer::GetInstance().CleanUp();
	return S_OK;
}

BOOL DxSkinPieceRootData::SaveFile ( const char* szFile, LPDIRECT3DDEVICEQ pd3dDevice )
{
	char szPathName[MAX_PATH] = "";
	StringCchCopy( szPathName, MAX_PATH, DxSkinPieceDataContainer::GetInstance().GetPath() );
	StringCchCat( szPathName, MAX_PATH, szFile );

	CSerialFile SFile;
	if ( !SFile.OpenFile ( FOT_WRITE, szPathName ) )		return FALSE;

	SFile << VERSION;

	SFile << m_strAbf;
	SFile << m_strCPS;
	SFile << m_strSkeleton;
	SFile << m_strBoneLink;
	SFile << m_dwType;
	SFile << m_dwWeaponSlot;

	SFile << m_fRotX;
	SFile << m_fRotY;
	SFile << m_fRotZ;
	SFile << m_fRootX;
	SFile << m_fRootY;
	SFile << m_fRootZ;
	SFile << m_fRotX_F;
	SFile << m_fRotY_F;
	SFile << m_fRotZ_F;
	SFile << m_fRootX_F;
	SFile << m_fRootY_F;
	SFile << m_fRootZ_F;

	return TRUE;
}

BOOL DxSkinPieceRootData::LOAD_Ver100( basestream &SFile, LPDIRECT3DDEVICEQ pd3dDevice )
{
	SFile >> m_strAbf;
	SFile >> m_strCPS;
	SFile >> m_strSkeleton;
	SFile >> m_strBoneLink;
	SFile >> m_dwType;
	SFile >> m_dwWeaponSlot;

	SFile >> m_fRotX;
	SFile >> m_fRotY;
	SFile >> m_fRotZ;
	SFile >> m_fRootX;
	SFile >> m_fRootY;
	SFile >> m_fRootZ;
	SFile >> m_fRotX_F;
	SFile >> m_fRotY_F;
	SFile >> m_fRotZ_F;
	SFile >> m_fRootX_F;
	SFile >> m_fRootY_F;
	SFile >> m_fRootZ_F;

	return TRUE;

}

BOOL DxSkinPieceRootData::LoadFile ( const char* szFile, LPDIRECT3DDEVICEQ pd3dDevice, const BOOL bThread, bool bREPRESH )
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


	DWORD dwVer;
	SFile >> dwVer;


	switch ( dwVer )
	{
	case VERSION:
		LOAD_Ver100( SFile, pd3dDevice );
		break;
	default:
		return FALSE;
	};

	DxSkinPieceData* pCharData;

	m_pSkeleton = DxBoneCollector::GetInstance().Load ( m_strSkeleton.c_str(), pd3dDevice );
	
	if ( !m_pSkeleton )		return FALSE;
	
	DxBoneTrans* pBone;
	pBone = new DxBoneTrans;
	
	pBone = m_pSkeleton->FindBone( m_strBoneLink.c_str() );
	if ( !pBone ) return FALSE;

	pCharData = new DxSkinPieceData;
	pCharData = DxSkinPieceDataContainer::GetInstance().LoadData( m_strAbf.c_str() , pd3dDevice , true );
	if ( !pCharData )
	{
		SAFE_DELETE(pCharData);
		CDebugSet::ToLogFile( "Load ABF File : %s Fail",m_strAbf.c_str() );
		return FALSE;
	}

	if ( strcmp( m_strCPS.c_str() , _T("") ) != 0 )
	{
		m_bCPS = TRUE;
	}
	
	pCharData->m_strAbf2 = m_strAbf;
	pCharData->m_strCPS2 = m_strCPS;
	pCharData->m_strSkeleton2 = m_strSkeleton;
	pCharData->m_strBoneLink2 = m_strBoneLink;

	pCharData->m_bCPS = m_bCPS;

	pCharData->m_dwType = m_dwType;
	pCharData->m_dwWeaponSlot = m_dwWeaponSlot;

	pCharData->m_fRotX = m_fRotX;
	pCharData->m_fRotY = m_fRotY;
	pCharData->m_fRotZ = m_fRotZ;

	pCharData->m_fRootX = m_fRootX;
	pCharData->m_fRootY = m_fRootY;
	pCharData->m_fRootZ = m_fRootZ;

	pCharData->m_fRotX_F = m_fRotX_F;
	pCharData->m_fRotY_F = m_fRotY_F;
	pCharData->m_fRotZ_F = m_fRotZ_F;

	pCharData->m_fRootX_F = m_fRootX_F;
	pCharData->m_fRootY_F = m_fRootY_F;
	pCharData->m_fRootZ_F = m_fRootZ_F;

	pCharData->m_dwRef++;

	StringCchCopy( m_szFileName, FILE_LENGTH, szFile );

	m_pSkinPieceData = pCharData;

	return TRUE;
}

// Container!!!!!!!!!!!!!!!!>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\=======================

DxSkinPieceRootDataContainer::DxSkinPieceRootDataContainer ()
{
	memset( m_szPath, 0, sizeof(char)*MAX_PATH );
}

DxSkinPieceRootDataContainer::~DxSkinPieceRootDataContainer ()
{
	//CleanUp();
}

DxSkinPieceRootDataContainer& DxSkinPieceRootDataContainer::GetInstance()
{
	static DxSkinPieceRootDataContainer Instance;
	return Instance;
}

HRESULT DxSkinPieceRootDataContainer::CleanUp ()
{
	SKINPIECEROOTDATAMAP_ITER iter = m_mapSkinRootData.begin ();
	SKINPIECEROOTDATAMAP_ITER iterEnd = m_mapSkinRootData.end ();
	
	for ( ; iter!=iterEnd; iter++ )
	{
		delete (*iter).second;
	}

	m_mapSkinRootData.clear ();

	return S_OK;
}

DxSkinPieceRootData* DxSkinPieceRootDataContainer::FindData ( const char* szFile )
{
	CString strTemp;
	strTemp.Format( "%s",szFile );

	SKINPIECEROOTDATAMAP_ITER iter = m_mapSkinRootData.find ( strTemp );
	if ( iter != m_mapSkinRootData.end() )
	{
		return iter->second;
	}

	return NULL;
}

BOOL DxSkinPieceRootDataContainer::DeleteData ( const char* szFile )
{
	CString strTemp;
	strTemp.Format( "%s",szFile );

	SKINPIECEROOTDATAMAP_ITER iter = m_mapSkinRootData.find ( strTemp );
	if ( iter == m_mapSkinRootData.end() )	return FALSE;

	delete (*iter).second;
	m_mapSkinRootData.erase ( iter );

	return TRUE;
}

void DxSkinPieceRootDataContainer::ReleaseData ( const char* szFile )
{
	DxSkinPieceRootData* pRes = FindData ( szFile );
	if ( !pRes )	return;
	
	if ( pRes->GetData()->m_dwRef<=1 )
	{
		DeleteData ( szFile );
		return;
	}

	pRes->GetData()->m_dwRef--;
	return;
}

DxSkinPieceRootData* DxSkinPieceRootDataContainer::LoadData( const char* szFile, LPDIRECT3DDEVICEQ pd3dDevice, const BOOL bThread )
{
	if ( !szFile ) return NULL;
	if ( strlen(szFile) == 0 ) return NULL;

	DxSkinPieceRootData* pRootData;

	pRootData = FindData( szFile );
	if ( pRootData )
	{
		pRootData->GetData()->m_dwRef++;
		return pRootData;
	}

	pRootData = new DxSkinPieceRootData;
	BOOL bOk = pRootData->LoadFile( szFile, pd3dDevice, bThread );
	if ( !bOk )
	{
		SAFE_DELETE(pRootData);
		return NULL;
	}
	pRootData->GetData()->m_dwRef++;

	CString strTemp;
	strTemp.Format( "%s",szFile );

	m_mapSkinRootData[ strTemp ] = pRootData;

	return pRootData;
}