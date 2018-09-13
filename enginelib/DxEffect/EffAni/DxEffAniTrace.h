#pragma once

#include "DxEffAni.h"

class DxSkinChar;
class DxSkinCharPiece;	//add abl
class DxSkinVehicle;	//add vcf

class DxEffSingleGroup;

struct EFFANI_PROPERTY_TRACE : public EFFANI_PROPERTY
{
	float		m_fScale;
	std::string m_strTexName;	// ���� �ִ�. 

	EFFANI_PROPERTY_TRACE () :
		m_fScale(1.f)
	{
		m_strTexName = "";
	}
};

class DxEffAniData_Trace : public DxEffAniData
{
public:
	static DWORD		VERSION;
	static DWORD		VERSION_SAVE;
	static DWORD		TYPEID;
	static char			NAME[64];

public:
	virtual DWORD GetTypeID ()		{ return TYPEID; }
	virtual DWORD GetFlag ()		{ return NULL; }
	virtual const char* GetName ()	{ return NAME; }

public:
	union
	{
		struct
		{
			EFFANI_PROPERTY_TRACE		m_Property;
		};

		struct
		{
			float		m_fScale;
			std::string m_strTexName;	// ���� �ִ�. 
		};
	};

public:
	virtual void SetProperty ( EFFANI_PROPERTY *pProperty )
	{
		m_Property = *((EFFANI_PROPERTY_TRACE*)pProperty);
	}
	
	virtual EFFANI_PROPERTY* GetProperty ()
	{
		return &m_Property;
	}

public:
	virtual void SetEffAniData ( DxEffAniData*	pData );

public:
	virtual DxEffAni* NEWOBJ ( SAnimContainer* pAnimContainer );	//	Note : ����Ʈ ������.
	virtual HRESULT LoadFile ( basestream &SFile, LPDIRECT3DDEVICEQ pd3dDevice ,bool bTool);
	virtual HRESULT SaveFile ( CSerialFile &SFile );

public:
	virtual HRESULT InitDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice );
	virtual HRESULT DeleteDeviceObjects ();

public:
	DxEffAniData_Trace () :
		DxEffAniData()
	{
	}
};

class DxEffAniTrace : public DxEffAni
{
public:
	float		m_fScale;
	std::string m_strTexName;

public:
	static HRESULT CreateDevice ( LPDIRECT3DDEVICEQ pd3dDevice );
	static HRESULT ReleaseDevice ( LPDIRECT3DDEVICEQ pd3dDevice );

public:
	virtual HRESULT OneTimeSceneInit ();
	virtual HRESULT InitDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice );
	virtual HRESULT RestoreDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice );
	virtual HRESULT InvalidateDeviceObjects ();
	virtual HRESULT DeleteDeviceObjects ();
	virtual HRESULT FinalCleanup ();

public:
	virtual HRESULT FrameMove( float fTime, float fElapsedTime );
	virtual HRESULT	Render( const LPDIRECT3DDEVICEQ pd3dDevice, DxSkinChar* pSkinChar, const D3DXMATRIX& matCurPos );
	//add abl
	virtual HRESULT Render( const LPDIRECT3DDEVICEQ pd3dDevice, DxSkinCharPiece* pSkinCharPiece, const D3DXMATRIX& matCurPos );
	//add vcf
	virtual HRESULT Render( const LPDIRECT3DDEVICEQ pd3dDevice, DxSkinVehicle* pSkinVehicle, const D3DXMATRIX& matCurPos );

public:
	DxEffAniTrace(void);
	~DxEffAniTrace(void);
};

