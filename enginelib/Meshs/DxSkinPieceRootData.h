#pragma once
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "DxSkinAniMan.h"
#include "DxSkinPieceContainer.h"

#include "basestream.h"
#include "DxSkinPieceData.h"

#define FG_BONEFILE_CHECK	(0x0001)
#define FG_MUSTNEWLOAD		(0x0002)

class DxSkinPieceRootData
{
public:
	enum { VERSION = 0x0100, };

private:
	enum		{ FILE_LENGTH = 64 };
	char		m_szFileName[FILE_LENGTH];

	std::string			m_strAbf;
	std::string			m_strCPS;
	std::string			m_strSkeleton;
	std::string			m_strBoneLink;

	DWORD				m_dwType;
	DWORD				m_dwWeaponSlot;

	BOOL				m_bCPS;

	float				m_fRotX;
	float				m_fRotY;
	float				m_fRotZ;

	float				m_fRootX;
	float				m_fRootY;
	float				m_fRootZ;

	float				m_fRotX_F;
	float				m_fRotY_F;
	float				m_fRotZ_F;

	float				m_fRootX_F;
	float				m_fRootY_F;
	float				m_fRootZ_F;

public:
	DxSkeleton*			m_pSkeleton;

protected:
	DxSkinPieceData*	m_pSkinPieceData;

public:
	const char*				GetFileName(){ return m_szFileName; }
	std::string				GetCpsName(){ return m_strCPS; }
	BOOL					GetUseCPS(){ return m_bCPS; }
	std::string				GetBoneLink(){ return m_strBoneLink; }
	DWORD					GetType(){ return m_dwType; }
	DWORD					GetWeaponSlot(){ return m_dwWeaponSlot; }
	DxSkinPieceData*		GetData(){ return m_pSkinPieceData; }
	BOOL					SetData( DxSkinPieceData* pSkin ){ m_pSkinPieceData = pSkin; }

public:
	HRESULT					ClearAll ();

protected:
	BOOL LOAD_Ver102( basestream &SFile, LPDIRECT3DDEVICEQ pd3dDevice );
	BOOL DUMMY( basestream &SFile, LPDIRECT3DDEVICEQ pd3dDevice );
	BOOL LOAD_Ver101( basestream &SFile, LPDIRECT3DDEVICEQ pd3dDevice );
	BOOL LOAD_Ver100( basestream &SFile, LPDIRECT3DDEVICEQ pd3dDevice );
public:
	BOOL LoadFile ( const char* szFile, LPDIRECT3DDEVICEQ pd3dDevice, const BOOL bThread, bool bREPRESH=false  );
	BOOL SaveFile ( const char* szFile, LPDIRECT3DDEVICEQ pd3dDevice );
public:
	DxSkinPieceRootData ();
	~DxSkinPieceRootData ();
};

class DxSkinPieceRootDataContainer
{
private:
	typedef std::map<CString,DxSkinPieceRootData*>			SKINPIECEROOTDATAMAP;
	typedef std::map<CString,DxSkinPieceRootData*>::iterator	SKINPIECEROOTDATAMAP_ITER;

protected:
	char			m_szPath[MAX_PATH];

	SKINPIECEROOTDATAMAP	m_mapSkinRootData;

public:
	void			SetPath ( char* szPath ) { StringCchCopy( m_szPath, MAX_PATH, szPath ); }
	char*			GetPath ()	{ return m_szPath; }

public:
	DxSkinPieceRootData* FindData ( const char* szFile );
	DxSkinPieceRootData* LoadData ( const char* szFile, LPDIRECT3DDEVICEQ pd3dDevice, const BOOL bThread );

	void ReleaseData ( const char* szFile );
	BOOL DeleteData ( const char* szFile );

	HRESULT CleanUp ();

protected:
	DxSkinPieceRootDataContainer ();

public:
	~DxSkinPieceRootDataContainer ();

public:
	static DxSkinPieceRootDataContainer& GetInstance();
};