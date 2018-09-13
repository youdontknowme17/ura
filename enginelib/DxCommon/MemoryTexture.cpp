#include "stdafx.h"
#include "MemoryTexture.h"
#include "DebugSet.h"
#include "Crypt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CMemoryTexture::CMemoryTexture()
{
	m_Buffer = NULL;
	m_nSize = 0;
}

CMemoryTexture::~CMemoryTexture()
{
	if (m_Buffer)	SAFE_DELETE_ARRAY( m_Buffer );
}

bool CMemoryTexture::LoadFileDec( const char* szFileName )
{
	// ���Ͽ���
	FILE* pFile = NULL;
	pFile = fopen( szFileName, "rb" );
	if ( !pFile )
	{
		CDebugSet::ToLogFile( "Texture File Open Error", szFileName );	
		return false;
	}

	if ( !DecryptTexture( pFile ) )
	{
		CDebugSet::ToLogFile( "Texture File Decrypt Error", szFileName );	
		return false;
	}

	fclose ( pFile );
	
	return true;
}

bool CMemoryTexture::SaveFileEnc( const char* szFileName )
{
	// ���Ͽ���
	FILE* pFile = NULL;
	pFile = fopen( szFileName, "rb" );
	if ( !pFile )
	{
		CDebugSet::ToLogFile( "Texture File Open Error", szFileName );	
		return false;
	}

	if ( !EncryptTexture( pFile, szFileName ) )
	{
		CDebugSet::ToLogFile( "Texture File Decrypt Error", szFileName );	
		return false;
	}

	fclose ( pFile );
	
	return true;
}

bool CMemoryTexture::DecryptTexture( FILE* pFile )
{
	char g_szFileExt[CCrypt::TEXTURE_TYPE][MAX_PATH] = { ".dds",".tga",".png" };

	fseek ( pFile, 0, SEEK_END );
	int nSize = ftell ( pFile );

	fseek ( pFile, 0, SEEK_SET );	
	
	// ����б�
	int nVersion = 0;
	int nReadSize = 0;
	int nFileType = 0;
	
	fread( &nVersion, sizeof(int), 1, pFile );
    fread( &nReadSize, sizeof(int), 1, pFile );
	fread( &nFileType, sizeof(int), 1, pFile );

	if ( nVersion != CCrypt::TEX_VERSION || nSize != nReadSize + CCrypt::TEX_HEADER_SIZE )
	{
		return false;
	}	
	
	m_nSize = nReadSize;
	
	m_Buffer = new BYTE[nReadSize];

	// �����б�
	fread( m_Buffer, sizeof(BYTE), nReadSize, pFile );
	
	// XOR��ȯ
	for ( int i = 0; i < nReadSize; ++i )
	{
		m_Buffer[i] += CCrypt::TEX_DIFF_DATA;
		m_Buffer[i] = m_Buffer[i] ^ CCrypt::TEX_XOR_DATA;
	}

	return true;
}

bool CMemoryTexture::EncryptTexture( FILE* pFile, const char* szFileName )
{
	char g_szFileExt[CCrypt::TEXTURE_TYPE][MAX_PATH] = { ".dds",".tga" };

	fseek ( pFile, 0, SEEK_END );
	int nSize = ftell ( pFile );

	fseek ( pFile, 0, SEEK_SET );	
	
	// ����߰�	
	BYTE* pBuffer = new BYTE[CCrypt::TEX_HEADER_SIZE+nSize];

	int nVersion = CCrypt::TEX_VERSION;
	int nFileType = CCrypt::TEXTURE_DDS;

	CString strFileExt = szFileName;

	strFileExt = strFileExt.Right( strFileExt.GetLength() - strFileExt.ReverseFind( '.' ) );
	strFileExt.MakeLower();

	if ( !strcmp( strFileExt, g_szFileExt[CCrypt::TEXTURE_DDS] ) )	nFileType = CCrypt::TEXTURE_DDS;
	else if ( !strcmp( strFileExt, g_szFileExt[CCrypt::TEXTURE_TGA] ) ) nFileType = CCrypt::TEXTURE_TGA;
	
	memcpy( pBuffer, &nVersion, sizeof( int ) );
	memcpy( pBuffer+4 , &nSize, sizeof(int) );
	memcpy( pBuffer+8, &nFileType, sizeof(int) );	
	
	// ���̱�
	fread( pBuffer+CCrypt::TEX_HEADER_SIZE, sizeof(BYTE), nSize, pFile );
	fclose( pFile );

	DeleteFile( szFileName );

	// XOR��ȯ
	for ( int i = 0; i < nSize; ++i )
	{
		pBuffer[CCrypt::TEX_HEADER_SIZE+i] = pBuffer[CCrypt::TEX_HEADER_SIZE+i] ^ CCrypt::TEX_XOR_DATA;
		pBuffer[CCrypt::TEX_HEADER_SIZE+i] -= CCrypt::TEX_DIFF_DATA;
	}

	CString strOutFile = szFileName;
	strOutFile = strOutFile.Left( strOutFile.ReverseFind('.'));
	strOutFile += ".mtf";

	// ��������
	FILE* pFileOutput = NULL;
	pFileOutput = fopen( strOutFile, "wb" );
	if ( !pFileOutput )
	{
		delete[] pBuffer;
		return FALSE;
	}
	
	fwrite( pBuffer, sizeof(BYTE), nSize+CCrypt::TEX_HEADER_SIZE, pFileOutput );
	fclose( pFileOutput );

	delete[] pBuffer;
	return TRUE;
}