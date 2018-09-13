#include "stdafx.h"
#include "../DxEffect/Single/DxEffSingle.h"
#include "../Single/DxEffSinglePropGMan.h"
#include "../Common/SerialFile.h"

#include "../DxCommon/DxViewPort.h"

#include "./DxEffCharShock.h"

#include "Crypt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD		DxEffCharShock::TYPEID			= EMEFFCHAR_SHOCK;
DWORD		DxEffCharShock::VERSION			= 0x0101;
DWORD		DxEffCharShock::VERSION_SAVE	= 0x0101;
char		DxEffCharShock::NAME[MAX_PATH]	= "EffCharShock";

DxEffCharShock::DxEffCharShock(void) :
	DxEffChar(),
	m_fElapsedTime(0.f),
	m_vDirection(1,0,0),
	m_fTarDirection(0.f),
	m_bLightEnable7(TRUE)
{
}


DxEffCharShock::~DxEffCharShock(void)
{
	CleanUp ();
}

DxEffChar* DxEffCharShock::CloneInstance ( LPDIRECT3DDEVICEQ pd3dDevice, DxCharPart* pCharPart, DxSkinPiece* pSkinPiece )
{
	HRESULT hr;
	DxEffCharShock *pEffChar = new DxEffCharShock;
	pEffChar->SetLinkObj ( pCharPart, pSkinPiece );
	pEffChar->SetProperty ( &m_Property );

	hr = pEffChar->Create ( pd3dDevice );
	if ( FAILED(hr) )
	{
		SAFE_DELETE(pEffChar);
		return NULL;
	}

	return pEffChar;
}

HRESULT DxEffCharShock::CreateDevice ( LPDIRECT3DDEVICEQ pd3dDevice )
{	
	return S_OK;
}

HRESULT DxEffCharShock::ReleaseDevice ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	return S_OK;
}

HRESULT DxEffCharShock::FrameMove ( float fTime, float fElapsedTime )
{
	HRESULT hr = S_OK;

	m_fElapsedTime += fElapsedTime;

	return S_OK;
}

HRESULT	DxEffCharShock::SettingState ( LPDIRECT3DDEVICEQ pd3dDevice, SKINEFFDATA& sSKINEFFDATA )
{
	pd3dDevice->GetLightEnable	( 5, &m_bLightEnable7 );
	pd3dDevice->GetLight		( 5, &m_sLight7 );

	D3DLIGHTQ	sLight;

	sLight.Type        = D3DLIGHT_DIRECTIONAL;
	sLight.Diffuse.a   = 1.0f;
	sLight.Diffuse.r   = m_cColor.r;
	sLight.Diffuse.g   = m_cColor.g;
	sLight.Diffuse.b   = m_cColor.b;
	sLight.Ambient.a    = 0.0f;
	sLight.Ambient.r	= 0.0f;
	sLight.Ambient.g	= 0.0f;
	sLight.Ambient.b	= 0.0f;
	sLight.Specular.a   = 1.0f;
	sLight.Specular.r	= m_cColor.r;
	sLight.Specular.g	= m_cColor.g;
	sLight.Specular.b	= m_cColor.b;

	sLight.Direction	= m_vDirection;

	pd3dDevice->LightEnable ( 7, TRUE );
	pd3dDevice->SetLight ( 7, &sLight );

	return S_OK;
}

HRESULT	DxEffCharShock::RestoreState ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	pd3dDevice->LightEnable ( 5, m_bLightEnable7 );
	pd3dDevice->SetLight ( 5, &m_sLight7 );

	//	Note : ȿ���� ������ ���
	//
	if ( m_fElapsedTime > m_fFullTime )
	{
		m_fElapsedTime = m_fFullTime;
		SetEnd();
	}

	return S_OK;
}

HRESULT	DxEffCharShock::LoadFile ( basestream &SFile, LPDIRECT3DDEVICEQ pd3dDevice ,bool bTool,bool bRan2 )
{
	HRESULT hr;
	DWORD dwVer, dwSize;

	SFile >> dwVer;
	SFile >> dwSize;

	//	Note : ������ ��ġ�� ���. 
	//
	if ( dwVer == VERSION )
	{
		//	Note : ����Ʈ�� Property �� ����.
		//
		SFile.ReadBuffer ( &m_Property, sizeof(m_Property) );

		//	Note : Device �ڿ��� �����Ѵ�.
		//
		if (!bTool)
		{
			hr = Create( pd3dDevice );
			if ( FAILED(hr) )	return hr;
		}
		/*hr = Create ( pd3dDevice );
		if ( FAILED(hr) )	return hr;*/
	}
	else if ( dwVer == 0x0100 )
	{
		SFile.SetOffSet ( SFile.GetfTell()+dwSize );	// �׳� �ǳ� ���.

		return S_OK;
	}
	else
	{
		CString strTemp = "";
		strTemp.Format( "DxEffCharShock::LoadFile Error! Name:%s Version:%d Size:%d Current Version:%d" ,SFile.GetFileName(),dwVer,dwSize,VERSION);
		if ( CCrypt::bEngineDebug )  MessageBox (NULL,strTemp,"Need New Version",MB_OK);
		if ( CCrypt::bEngineDebugLog )  CDebugSet::ToFile ( "EngineLoadError.txt",strTemp );
		strTemp.Empty();
		//
		SFile.SetOffSet ( SFile.GetfTell()+dwSize );
		
		return E_FAIL;
	}

	return S_OK;
}

HRESULT	DxEffCharShock::SaveFile ( basestream &SFile ,bool bTool)
{
	SFile << TYPEID;
	SFile << VERSION_SAVE;

	//	Note : DATA�� ����� ����Ѵ�. Load �ÿ� ������ Ʋ�� ��� ����.
	//
	SFile << (DWORD) ( sizeof(m_Property) );

	//	Note : ����Ʈ�� Property �� ����.
	//
	SFile.WriteBuffer ( &m_Property, sizeof(m_Property) );

	return S_OK;
}