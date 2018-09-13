#include "stdafx.h"
#include "../Common/SerialFile.h"
#include "../DxCommon/EDITMESHS.h"

#include "../G-Logic/GLogic.h"
#include "DxSkinPieceContainer.h"
#include "../Common/StringUtils.h"
#include "SAnimation.h"//add abl

#include "DxSkinCharData.h" 
#include "DxSkinPieceData.h" //add abl
#include "DxSkinPieceRootData.h" //add abl

#include "Crypt.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DxSkinCharDataContainer& DxSkinCharDataContainer::GetInstance()
{
	static DxSkinCharDataContainer Instance;
	return Instance;
}

//-----------------------------------------------------------------------------------[DxSkinCharData]
//
DxSkinCharData::DxSkinCharData(void) :
	m_dwRef(0),

	m_bWorldObj(FALSE),
	m_fScale(1.0f),
	m_vMax(7.f,20.f,7.f),
	m_vMin(-7.f,0.f,-7.f),
	m_fHeight(20.f),
	m_fRadius(7.f),
	m_dwBONE(0) //add abl
{
	memset( m_szFileName, 0, FILE_LENGTH );

	for ( int i=0; i<PIECE_SIZE; ++i )
	{
		m_pPieceArray[i] = NULL;
		//add abl
		m_pSkinCharPiece[i] = NULL;
		m_strBoneLink[i] = "";
		m_dwWeaponSlot[i] = 0;
		m_dwPieceType[i] = 0;
	}
	//add abl
	for ( DWORD n=0; n<MAX_BONE; ++n )
	{
		m_fBONE1[n] = 0.0f;
		m_fBONE2[n] = 0.0f;
		m_fBONE3[n] = 0.0f;
		m_strBONE[n] = "";
	}
}

DxSkinCharData::~DxSkinCharData(void)
{
	m_dwBONE = 0;//add abl
	ClearAll ();
}

HRESULT DxSkinCharData::ClearAll ()
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
		//add abl
		m_pSkinCharPiece[i] = NULL;
		m_strBoneLink[i] = "";
		m_dwWeaponSlot[i] = 0;
		m_dwPieceType[i] = 0;
	}

	DxSkinAniControl::ClearAll ();

	return S_OK;
}

HRESULT DxSkinCharData::SetPiece ( const char* szFile, LPDIRECT3DDEVICEQ pd3dDevice, const BOOL bThread, DWORD dwFlag )
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
	//add abl
	for ( int i=0; i<PIECE_SIZE; ++i )
	{
		m_pSkinCharPiece[i] = NULL;
		m_strBoneLink[i] = "";
		m_dwWeaponSlot[i] = 0;
		m_dwPieceType[i] = 0;
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

void DxSkinCharData::ResetPiece ( LPDIRECT3DDEVICEQ pd3dDevice, int i )
{
	GASSERT(i<PIECE_SIZE);

	PDXSKINPIECE pSkinPiece = GetPiece (i);
	
	//	Note : ������ �÷��� Piece �� �ִٸ� �̸� �����ش�.
	//
	if ( pSkinPiece )
	{
		DxSkinPieceContainer::GetInstance().ReleasePiece ( pSkinPiece->m_szFileName );
		m_pPieceArray[i] = NULL;
		//add abl
		m_pSkinCharPiece[i] = NULL;
		m_strBoneLink[i] = "";
		m_dwWeaponSlot[i] = 0;
		m_dwPieceType[i] = 0;
	}
}

BOOL DxSkinCharData::SaveFile ( const char* szFile, LPDIRECT3DDEVICEQ pd3dDevice )
{
	char szPathName[MAX_PATH] = "";
	StringCchCopy( szPathName, MAX_PATH, DxSkinCharDataContainer::GetInstance().GetPath() );
	StringCchCat( szPathName, MAX_PATH, szFile );

	CSerialFile SFile;
	if ( !SFile.OpenFile ( FOT_WRITE, szPathName ) )		return FALSE;

	SFile << VERSION_SAVE;

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

BOOL DxSkinCharData::LOAD_Ver108( basestream &SFile, LPDIRECT3DDEVICEQ pd3dDevice )
{
	DWORD dwPieceSize;
	float fUnknown;
	DWORD dwDummy;

	SFile >> dwPieceSize;
	if ( dwPieceSize > PIECE_SIZE )		return FALSE;

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

	SFile >> m_strSkeleton;

	DWORD dwAniNum(0);
	SFile >> dwAniNum;

	m_vecANIFILE.clear();
	m_vecANIFILE.reserve ( dwAniNum );

	std::string strANIFILE;
	CString strCfgFile, strTEMP;

	for ( DWORD i=0; i<dwAniNum; ++i )
	{
		SFile >> strANIFILE;

		strTEMP = strANIFILE.c_str();
		int nIndex = strTEMP.ReverseFind ( '.' );
		strCfgFile = strTEMP.Left(nIndex) + ".x";
		m_vecANIFILE.push_back ( strCfgFile.GetString() );
	}

	SFile >> m_fScale;
	SFile >> fUnknown;

	BOOL bExist = FALSE;
	bExist = FALSE;
	SFile >> bExist;
	if( bExist )	
	{
		SFile >> m_fHeight;
		SFile >> m_fRadius;
	}
	else			return TRUE;

	SFile >> m_dwBONE;

	if ( m_dwBONE > MAX_BONE ) return FALSE;

	for ( i=0; i < m_dwBONE ; ++i )
	{
		SFile >> m_fBONE1[i];
		SFile >> m_fBONE2[i];
		SFile >> m_fBONE3[i];
		SFile >> m_strBONE[i];
	}

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

	SFile >> bExist;
	if( bExist )	SFile >> m_bWorldObj;
	else			return TRUE;

	SFile >> bExist;
	SFile >> dwDummy;
	SFile >> dwDummy;

	return TRUE;
}
BOOL DxSkinCharData::LOAD_Ver107( basestream &SFile, LPDIRECT3DDEVICEQ pd3dDevice )
{
	DWORD dwPieceSize;
	float fUnknown;

	SFile >> dwPieceSize;
	if ( dwPieceSize > PIECE_SIZE )		return FALSE;

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

	SFile >> m_strSkeleton;

	DWORD dwAniNum(0);
	SFile >> dwAniNum;

	m_vecANIFILE.clear();
	m_vecANIFILE.reserve ( dwAniNum );

	std::string strANIFILE;
	CString strCfgFile, strTEMP;

	for ( DWORD i=0; i<dwAniNum; ++i )
	{
		SFile >> strANIFILE;

		strTEMP = strANIFILE.c_str();
		int nIndex = strTEMP.ReverseFind ( '.' );
		strCfgFile = strTEMP.Left(nIndex) + ".x";
		m_vecANIFILE.push_back ( strCfgFile.GetString() );
	}

	SFile >> m_fScale;
	SFile >> fUnknown;

	BOOL bExist = FALSE;
	bExist = FALSE;
	SFile >> bExist;
	if( bExist )	
	{
		SFile >> m_fHeight;
		SFile >> m_fRadius;
	}
	else			return TRUE;

	SFile >> m_dwBONE;

	if ( m_dwBONE > MAX_BONE ) return FALSE;

	for ( i=0; i < m_dwBONE ; ++i )
	{
		SFile >> m_fBONE1[i];
		SFile >> m_fBONE2[i];
		SFile >> m_fBONE3[i];
		SFile >> m_strBONE[i];
	}

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

	SFile >> bExist;
	if( bExist )	SFile >> m_bWorldObj;
	else			return TRUE;

	SFile >> bExist;

	return TRUE;
}
BOOL DxSkinCharData::LOAD_Ver106( basestream &SFile, LPDIRECT3DDEVICEQ pd3dDevice )
{
	DWORD dwPieceSize;
	DWORD dwUnKnown;

	SFile >> dwPieceSize;
	if ( dwPieceSize > PIECE_SIZE )		return FALSE;

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

	SFile >> m_strSkeleton;

	DWORD dwAniNum(0);
	SFile >> dwAniNum;

	m_vecANIFILE.clear();
	m_vecANIFILE.reserve ( dwAniNum );

	for ( DWORD i=0; i<dwAniNum; ++i )
	{
		std::string strANIFILE;
		SFile >> strANIFILE;

		m_vecANIFILE.push_back ( strANIFILE.c_str() );
	}

	SFile >> m_fScale;
	SFile >> m_dwBONE;

	if ( m_dwBONE > MAX_BONE ) return FALSE;

	for ( i=0; i < m_dwBONE ; ++i )
	{
		SFile >> m_fBONE1[i];
		SFile >> m_fBONE2[i];
		SFile >> m_fBONE3[i];
		SFile >> m_strBONE[i];
	}

	BOOL bExist = FALSE;
	bExist = FALSE;
	SFile >> bExist;
	if( bExist )	
	{
		SFile >> m_fHeight;
		SFile >> m_fRadius;
	}
	else			return TRUE;

	SFile >> dwUnKnown;

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

	SFile >> bExist;
	if( bExist )	SFile >> m_bWorldObj;
	else			return TRUE;

	SFile >> bExist;

	return TRUE;
}

BOOL DxSkinCharData::LOAD_Ver104( basestream &SFile, LPDIRECT3DDEVICEQ pd3dDevice )
{
	DWORD dwPieceSize;
	DWORD dwUnKnown = 0;

	SFile >> dwPieceSize;
	if ( dwPieceSize > PIECE_SIZE )		return FALSE;

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

	SFile >> m_strSkeleton;

	DWORD dwAniNum(0);
	SFile >> dwAniNum;

	m_vecANIFILE.clear();
	m_vecANIFILE.reserve ( dwAniNum );

	for ( DWORD i=0; i<dwAniNum; ++i )
	{
		std::string strANIFILE;
		SFile >> strANIFILE;

		m_vecANIFILE.push_back ( strANIFILE.c_str() );
	}

	SFile >> m_fScale;

	BOOL bExist = FALSE;
	bExist = FALSE;
	SFile >> bExist;
	if( bExist )	
	{
		SFile >> m_fHeight;
		SFile >> m_fRadius;
	}
	else			return TRUE;

	SFile >> m_dwBONE;

	if ( m_dwBONE > MAX_BONE ) return FALSE;

	for ( i=0; i < m_dwBONE ; ++i )
	{
		SFile >> m_fBONE1[i];
		SFile >> m_fBONE2[i];
		SFile >> m_fBONE3[i];
		SFile >> m_strBONE[i];
	}

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

	SFile >> bExist;
	if( bExist )	SFile >> m_bWorldObj;
	else			return TRUE;

	SFile >> bExist;


		return TRUE;
}

BOOL DxSkinCharData::LOAD_Ver101( basestream &SFile, LPDIRECT3DDEVICEQ pd3dDevice )
{
	DWORD dwPieceSize;

	SFile >> m_fScale;
	SFile >> dwPieceSize;
	if ( dwPieceSize > PIECE_SIZE )		return FALSE;

	//	Note : ������ �ε�.
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

	//	Note : ������ �б�.
	//
	SFile >> m_strSkeleton;

	//	Note :���ϸ��̼� �б�.
	//
	DWORD dwAniNum(0);
	SFile >> dwAniNum;

	m_vecANIFILE.clear();
	m_vecANIFILE.reserve ( dwAniNum );

	for ( DWORD i=0; i<dwAniNum; ++i )
	{
		std::string strANIFILE;
		SFile >> strANIFILE;

		m_vecANIFILE.push_back ( strANIFILE.c_str() );
	}

	//	Note : �ٿ�� �ڽ� ���� �ε�.
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

	BOOL bExist = FALSE;
	SFile >> bExist;
	if( bExist )	SFile >> m_bWorldObj;
	else			return TRUE;

	bExist = FALSE;
	SFile >> bExist;
	if( bExist )	
	{
		SFile >> m_fHeight;
		SFile >> m_fRadius;
	}
	else			return TRUE;

	// Note : ����
	SFile >> bExist;

	return TRUE;
}

BOOL DxSkinCharData::LOAD_Ver100( basestream &SFile, LPDIRECT3DDEVICEQ pd3dDevice )
{
	DWORD dwPieceSize;
	SFile >> dwPieceSize;
	if ( dwPieceSize > PIECE_SIZE )		return FALSE;

	//	Note : ������ �ε�.
	//
	BOOL bExit;
	char szPIECE[PIECE_SIZE][MAX_PATH] = {0};
	memset ( szPIECE, 0, sizeof(char)*PIECE_SIZE*MAX_PATH );
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

	//	Note : ������ �б�.
	char szSkeleton[MAX_PATH] = "";
	SFile.ReadBuffer ( szSkeleton, sizeof(char)*MAX_PATH );
	m_strSkeleton = szSkeleton;

	//	Note :���ϸ��̼� �б�.
	char szAniName[ACF_SZNAME] = "";

	DWORD dwLenght;
	SFile >> dwLenght;

	m_vecANIFILE.clear();
	m_vecANIFILE.reserve(dwLenght);

	for ( DWORD i=0; i<dwLenght; ++i )
	{
		SFile.ReadBuffer ( szAniName, sizeof(char)*ACF_SZNAME );
		m_vecANIFILE.push_back ( szAniName );
	}

	//	Note : �ٿ�� �ڽ� ���� �ε�.
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

	return TRUE;
}

//	Note : ��Ų ������ ����.
//		BOOL DxSkinDataDummy::LoadFile () ������ ���� ������ �ε��� ���ϹǷ� �� �ε�/���̺� �κ��� �ٲ��
//		�����ؼ� ���� ������ �ʿ��ϴ�.
//
BOOL DxSkinCharData::LoadFile ( const char* szFile, LPDIRECT3DDEVICEQ pd3dDevice, const BOOL bThread, bool bREPRESH )
{
	char szPathName[MAX_PATH] = "";
	StringCchCopy( szPathName, MAX_PATH, DxSkinCharDataContainer::GetInstance().GetPath() );
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
	case VERSION_NEW:
		LOAD_Ver108( SFile, pd3dDevice );
		break;
	case 0x0107:
		LOAD_Ver107( SFile, pd3dDevice );
		break;
	case 0x0106:
		LOAD_Ver106( SFile, pd3dDevice );
		break;
	case 0x0104:
		LOAD_Ver104( SFile, pd3dDevice );
		break;
	case 0x0102:
		LOAD_Ver101( SFile, pd3dDevice );
		break;
	case 0x0101:
		LOAD_Ver101( SFile, pd3dDevice );
		break;
	case 0x0100:
		LOAD_Ver100( SFile, pd3dDevice );
		break;
	default:
		CString strTemp = "";
		strTemp.Format( "DxSkinCharData::LoadFile Error! Name:%s Version:%d Current Version:%d" ,SFile.GetFileName(),dwVer,VERSION_NEW);
		if ( CCrypt::bEngineDebug )  MessageBox (NULL,strTemp,"Need New Version",MB_OK);
		if ( CCrypt::bEngineDebugLog )  CDebugSet::ToFile ( "EngineLoadError.txt",strTemp );
		strTemp.Empty();

		return FALSE;
	};


	//	Note : ������ �б�.
	//
	m_pSkeleton = DxBoneCollector::GetInstance().Load ( m_strSkeleton.c_str(), pd3dDevice );
	if ( !m_pSkeleton )		return FALSE;

	DWORD i;
	for ( i=0; i<PIECE_SIZE; ++i )
	{
		if ( !m_strPIECE[i].empty() )
		{ //add abl
			m_pSkinCharPiece[i] = NULL;
			DxSkinPieceRootData* pPieceDataRoot;
			pPieceDataRoot = DxSkinPieceRootDataContainer::GetInstance().LoadData( m_strPIECE[i].c_str() , pd3dDevice, bThread );
			if ( pPieceDataRoot )
			{
				if ( pPieceDataRoot->GetUseCPS() )
				{
					SetPiece ( pPieceDataRoot->GetCpsName().c_str() , pd3dDevice, bThread );
				}
				DxSkinPieceData* pSkinCharPiece = pPieceDataRoot->GetData();
				m_pSkinCharPiece[i] = new DxSkinCharPiece;
				m_pSkinCharPiece[i]->SetCharData ( pSkinCharPiece, pd3dDevice, TRUE );
				m_strBoneLink[i] = pPieceDataRoot->GetBoneLink();
				m_dwWeaponSlot[i] = pPieceDataRoot->GetWeaponSlot();
				m_dwPieceType[i] = pPieceDataRoot->GetType();
			}
			else
			{
				SetPiece ( m_strPIECE[i].c_str(), pd3dDevice, bThread );
			}
		}
	}

	for ( i=0; i<(DWORD)m_vecANIFILE.size(); ++i )
	{
		DxSkinAniControl::LoadAnimation ( m_vecANIFILE[i].c_str(), pd3dDevice, bREPRESH );
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

HRESULT DxSkinCharData::CalculateBoundingBox ( LPDIRECT3DDEVICEQ pd3dDevice, D3DXVECTOR3 &_vMax, D3DXVECTOR3 &_vMin )
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

void DxSkinCharData::SetRadiusHeight()
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

void DxSkinCharData::RenderAABBBox( LPDIRECT3DDEVICEQ pd3dDevice )
{
	if( !strlen( m_szFileName ) )	return;

	EDITMESHS::RENDERAABB( pd3dDevice, m_vMax, m_vMin );
}

//--------------------------------------------------------------------------[DxSkinCharDataContainer]
//
DxSkinCharDataContainer::DxSkinCharDataContainer ()
{
	memset( m_szPath, 0, sizeof(char)*MAX_PATH );
}

DxSkinCharDataContainer::~DxSkinCharDataContainer ()
{
}

HRESULT DxSkinCharDataContainer::CleanUp ()
{
	SKINCHARDATAMAP_ITER iter = m_mapSkinCharData.begin ();
	SKINCHARDATAMAP_ITER iterEnd = m_mapSkinCharData.end ();
	
	for ( ; iter!=iterEnd; iter++ )
	{
		delete (*iter).second;
	}

	m_mapSkinCharData.clear ();

	return S_OK;
}

DxSkinCharData* DxSkinCharDataContainer::FindData ( const char* szFile )
{
	SKINCHARDATAMAP_ITER iter = m_mapSkinCharData.find ( std::string(szFile) );
	if ( iter != m_mapSkinCharData.end() )	return iter->second;

	return NULL;
}

BOOL DxSkinCharDataContainer::DeleteData ( const char* szFile )
{
	SKINCHARDATAMAP_ITER iter = m_mapSkinCharData.find ( std::string(szFile) );
	if ( iter == m_mapSkinCharData.end() )	return FALSE;

	delete (*iter).second;
	m_mapSkinCharData.erase ( iter );

	return TRUE;
}

void DxSkinCharDataContainer::ReleaseData ( const char* szFile )
{
	DxSkinCharData* pRes = FindData ( szFile );
	if ( !pRes )	return;
	
	if ( pRes->m_dwRef<=1 )
	{
		DeleteData ( szFile );
		return;
	}

	pRes->m_dwRef--;
	return;
}

DxSkinCharData* DxSkinCharDataContainer::LoadData( const char* szFile, LPDIRECT3DDEVICEQ pd3dDevice, const BOOL bThread )
{
	DxSkinCharData* pCharData;

	pCharData = FindData( szFile );
	if ( pCharData )
	{
		pCharData->m_dwRef++;
		return pCharData;
	}

	pCharData = new DxSkinCharData;
	BOOL bOk = pCharData->LoadFile( szFile, pd3dDevice, bThread );
	if ( !bOk )
	{
		SAFE_DELETE(pCharData);
		return NULL;
	}
	pCharData->m_dwRef++;

	CDebugSet::ToListView ( "DxSkinCharData::LoadFile, %s", szFile );

	//	Note : Ʈ���� �ޱ�.
	//
	m_mapSkinCharData[std::string(pCharData->GetFileName())] = pCharData;

	return pCharData;
}





