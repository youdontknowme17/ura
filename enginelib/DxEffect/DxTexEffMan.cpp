#include "stdafx.h"

#include "./DxTexEffDiffuse.h"
#include "./DxTexEffFlowUV.h"
#include "./DxTexEffRotate.h"
#include "./DxTexEffSpecular.h"
#include "./DxTexEffVisualMaterial.h"

#include "./DxTexEffMan.h"

#include "Crypt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DxTexEffMan& DxTexEffMan::GetInstance()
{
	static DxTexEffMan Instance;
	return Instance;
}

// Note : DxTextureEffMan
DxTexEffMan::DxTexEffMan() :
	pTexEffTypeList(NULL)
{
	RegistType( DxTexEffDiffuse::TYPEID,	DxTexEffDiffuse::NAME );
	RegistType( DxTexEffFlowUV::TYPEID,		DxTexEffFlowUV::NAME );
	RegistType( DxTexEffRotate::TYPEID,		DxTexEffRotate::NAME );
	RegistType( DxTexEffSpecular::TYPEID,	DxTexEffSpecular::NAME );
	RegistType( DxTexEffVisualMaterial::TYPEID,	DxTexEffVisualMaterial::NAME );
}

DxTexEffMan::~DxTexEffMan()
{
	SAFE_DELETE(pTexEffTypeList);
}

void DxTexEffMan::OnCreateDevice( LPDIRECT3DDEVICEQ pd3dDevice )
{
	DxTexEffDiffuse::OnCreateDevice( pd3dDevice );
	DxTexEffFlowUV::OnCreateDevice( pd3dDevice );
	DxTexEffRotate::OnCreateDevice( pd3dDevice );
	DxTexEffSpecular::OnCreateDevice( pd3dDevice );
	DxTexEffVisualMaterial::OnCreateDevice( pd3dDevice );
}

void DxTexEffMan::OnReleaseDevice( LPDIRECT3DDEVICEQ pd3dDevice )
{
	DxTexEffDiffuse::OnReleaseDevice( pd3dDevice );
	DxTexEffFlowUV::OnReleaseDevice( pd3dDevice );
	DxTexEffRotate::OnReleaseDevice( pd3dDevice );
	DxTexEffSpecular::OnReleaseDevice( pd3dDevice );
	DxTexEffVisualMaterial::OnReleaseDevice( pd3dDevice );
}

void DxTexEffMan::RegistType( const DWORD TypeID, const char *Name )
{
	//	Note : �� ��� ������ ����.
	DxTexEffType *pNewType = new DxTexEffType;
	pNewType->TYPEID = TypeID;
	StringCchCopy( pNewType->NAME, MAX_PATH, Name );

	//	Note : ����Ʈ�� ���.
	//
	pNewType->pNextType = pTexEffTypeList;
	pTexEffTypeList = pNewType;
}

DxTexEffBase* DxTexEffMan::CreateEffInstance( DWORD TypeID )
{
	DxTexEffBase* pTexEffBase = NULL;

	if( DxTexEffDiffuse::TYPEID == TypeID )			pTexEffBase = new DxTexEffDiffuse();
	else if( DxTexEffFlowUV::TYPEID == TypeID )		pTexEffBase = new DxTexEffFlowUV();
	else if( DxTexEffRotate::TYPEID == TypeID )		pTexEffBase = new DxTexEffRotate();
	else if( DxTexEffSpecular::TYPEID == TypeID )	pTexEffBase = new DxTexEffSpecular();
	else if( DxTexEffVisualMaterial::TYPEID == TypeID )	pTexEffBase = new DxTexEffVisualMaterial();
	else
	{
		CString strTemp = "";
		strTemp.Format( "DxTexEffMan::CreateEffInstance New Effect Type ID :%d" ,TypeID);
		if ( CCrypt::bEngineDebug )  MessageBox (NULL,strTemp,"Need New Version",MB_OK);
		if ( CCrypt::bEngineDebugLog )  CDebugSet::ToFile ( "EngineLoadError.txt",strTemp );
		strTemp.Empty();
	}

	return pTexEffBase;
}

DxTexEffBase* DxTexEffMan::AdaptToDxFrame( DWORD TypeID, LPDIRECT3DDEVICEQ pd3dDevice, PBYTE pProperty )
{
	//	Note : Effect Instance ����.
	DxTexEffBase* pEffBase = CreateEffInstance( TypeID );
	if( !pEffBase ) return NULL;

	//	Note : Property, Affine  ���� ����.
	if( pProperty )		pEffBase->SetProperty( pd3dDevice, pProperty );

	return pEffBase;
}



