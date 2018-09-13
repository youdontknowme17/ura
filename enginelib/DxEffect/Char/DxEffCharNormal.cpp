#include "stdafx.h"
#include "../DxEffect/Single/DxEffSingle.h"
#include "../Single/DxEffSinglePropGMan.h"
#include "../Common/SerialFile.h"

#include "../DxEffect/DxLoadShader.h"
#include "../DxCommon/DxRenderStates.h"

#include "../DxCommon/DxLightMan.h"
#include "../DxCommon/DxViewPort.h"
#include "../DxEffect/DxEffectMan.h"

#include "../DxCommon/RENDERPARAM.h"
#include "../Meshs/DxCharPart.h"
#include "../Meshs/DxSkinPieceContainer.h"

#include "./DxEffCharHLSL.h"
#include "../Meshs/DxSkinMesh9_HLSL.h"

#include "./DxEffCharNormal.h"

#include "Crypt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD		DxEffCharNormal::TYPEID			= EMEFFCHAR_NORMALMAP;
DWORD		DxEffCharNormal::VERSION			= 0x0100;
DWORD		DxEffCharNormal::VERSION_SAVE		= 0x0100;
char		DxEffCharNormal::NAME[MAX_PATH]	= "EffCharNormal";

DxEffCharNormal::DxEffCharNormal(void) :
	DxEffChar(),
	m_pSkinMesh(NULL),
	m_pmcMesh(NULL)
{
}

DxEffCharNormal::~DxEffCharNormal(void)
{
	CleanUp ();
}

DxEffChar* DxEffCharNormal::CloneInstance ( LPDIRECT3DDEVICEQ pd3dDevice, DxCharPart* pCharPart, DxSkinPiece* pSkinPiece )
{
	HRESULT hr;
	DxEffCharNormal *pEffChar = new DxEffCharNormal;
	pEffChar->SetLinkObj ( pCharPart, pSkinPiece );
	pEffChar->SetProperty ( &m_Property );
	pEffChar->m_sMaterialHLSL.Clone( pd3dDevice, &m_sMaterialHLSL );

	hr = pEffChar->Create ( pd3dDevice );
	if ( FAILED(hr) )
	{
		SAFE_DELETE(pEffChar);
		return NULL;
	}

	return pEffChar;
}

HRESULT DxEffCharNormal::InitDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	HRESULT hr = S_OK;

	if ( m_pCharPart )
	{
		m_pSkinMesh = m_pCharPart->m_pSkinMesh;
		m_pmcMesh = m_pCharPart->m_pmcMesh;
	}
	else if ( m_pSkinPiece )
	{
		m_pSkinMesh = m_pSkinPiece->m_pSkinMesh;
		m_pmcMesh = m_pSkinPiece->m_pmcMesh;
	}

	if( !m_pmcMesh )	return S_OK;
	m_sMaterialHLSL.OnCreateDevice( pd3dDevice, m_pmcMesh->pMaterials, m_pmcMesh->GetNumMaterials() );

	return S_OK;
}

HRESULT DxEffCharNormal::RestoreDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	return S_OK;
}

HRESULT DxEffCharNormal::InvalidateDeviceObjects ()
{
	return S_OK;
}

HRESULT DxEffCharNormal::DeleteDeviceObjects ()
{
	m_sMaterialHLSL.OnDestroyDevice();

	return S_OK;
}

HRESULT DxEffCharNormal::FrameMove ( float fTime, float fElapsedTime )
{
	HRESULT hr = S_OK;

	return S_OK;
}

void DxEffCharNormal::Render( LPDIRECT3DDEVICEQ pd3dDevice, BOOL bPieceRender )
{
	if( !m_pmcMesh )		return;
	if( !m_pSkinMesh )		return;

    // Note : ReloadTexture - Loading�� �� �� ���� ���� ��ġ.
	m_sMaterialHLSL.ReloadTexture();

	m_pSkinMesh->SetDrawState( FALSE, FALSE, FALSE, FALSE );
	m_pSkinMesh->DrawMeshNORMALMAP( pd3dDevice, m_pmcMesh, NULL, &m_sMaterialHLSL, bPieceRender );
}

HRESULT	DxEffCharNormal::LoadFile ( basestream &SFile, LPDIRECT3DDEVICEQ pd3dDevice ,bool bTool,bool bRan2 )
{
	DWORD dwVer, dwSize;

	SFile >> dwVer;
	SFile >> dwSize;

	//	Note : ������ ��ġ�� ���. 
	if( dwVer == VERSION )
	{
		//	Note : ����Ʈ�� Property �� ����.
		SFile.ReadBuffer ( &m_Property, sizeof(m_Property) );
		m_sMaterialHLSL.Load( pd3dDevice, SFile );

		//	Note : Device �ڿ��� �����Ѵ�.
		HRESULT hr(S_OK);

		if (!bTool)
		{
			hr = Create( pd3dDevice );
			if ( FAILED(hr) )	return hr;
		}
		/*hr = Create ( pd3dDevice );
		if ( FAILED(hr) )	return hr;*/
	}
	else
	{
		CString strTemp = "";
		strTemp.Format( "DxEffCharNormal::LoadFile Error! Name:%s Version:%d Size:%d Current Version:%d" ,SFile.GetFileName(),dwVer,dwSize,VERSION);
		if ( CCrypt::bEngineDebug )  MessageBox (NULL,strTemp,"Need New Version",MB_OK);
		if ( CCrypt::bEngineDebugLog )  CDebugSet::ToFile ( "EngineLoadError.txt",strTemp );
		strTemp.Empty();
		//
		SFile.SetOffSet ( SFile.GetfTell()+dwSize );
		
		return E_FAIL;
	}

	return S_OK;
}

HRESULT	DxEffCharNormal::SaveFile ( basestream &SFile ,bool bTool)
{
	SFile << TYPEID;
	SFile << VERSION_SAVE;

	SFile.BeginBlock( EMBLOCK_00 );
	{
		SFile.WriteBuffer ( &m_Property, sizeof(m_Property) );
		m_sMaterialHLSL.Save( SFile );
	}
	SFile.EndBlock( EMBLOCK_00 );

	return S_OK;
}

