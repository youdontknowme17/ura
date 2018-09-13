#pragma once

#include "DxSkinMesh9.h"
#include "DxSkinObject.h"
#include "DxSkinAniMan.h"
#include "DxSkinPieceContainer.h"
#include "DxSkinVehicleData.h"
#include "DxCharPart.h"
#include "DxSkinEffResult.h"

#include "../DxEffect/EffAni/DxEffAniPlayer.h"
#include "../DxEffect/EffAni/DxEffAni.h"
#include "../DxEffect/EffKeep/DxEffKeepPlayer.h"

class DxEffKeepDataMain;

// Note : ĳ����
class DxSkinVehicle : public DxSkinAniControl
{
private:
	enum		{ FILE_LENGTH = 64 };

	float			m_fScale;

	DxEffAniPlayer	m_EffAniPlayer;
public:
	enum				{ MAX_BONE = 32 };

	DWORD				m_dwBONE;
	float				m_fBONE1[MAX_BONE];
	float				m_fBONE2[MAX_BONE];
	float				m_fBONE3[MAX_BONE];
	std::string			m_strBONE[MAX_BONE];

//------------------------------------------------------------
//				E	f	f		K	e	e	p
private:
	DxEffKeepPlayer	m_EffKeepPlayer;

public:
	SKINEFFDATA		m_sSkinEffData;

public: 
	void CreateEff( DxEffKeepDataMain* pEff );
	void DeleteEff( DxEffKeepDataMain* pEff );
	void Reset();

public:
	static DWORD				g_dwLOD;
	static DWORD				g_dwHIGHDRAW_NUM;

public:
	TCHAR			m_szFileName[FILE_LENGTH];
	std::string		m_strPIECE_P[PIECE_SIZE];
	
	D3DXVECTOR3			m_vRotation_Font, m_vTransform_Font;
	D3DXVECTOR3			m_vRotation_Back, m_vTransform_Back;
	std::string			m_strChar_Font;
	std::string			m_strChar_Back;

	DxCharPart		m_PartArray[PIECE_SIZE];
private:
	DWORD			m_dwVertexNUM;

public:
	HRESULT ClearAll ();

public:
	void SetColor( WORD wColor, WORD wColor1, WORD wColor2, WORD wColor3, WORD wColor4, WORD wColor5 );
	void SetColorAll( WORD wColor );

	HRESULT SetPiece ( const TCHAR* szFile, LPDIRECT3DDEVICEQ pd3dDevice, DWORD dwFlag = 0x0, int nLevel=0, bool bCharacterData = FALSE );
	HRESULT SetPieceGrindLevel ( int nIndex, int nLevel );

	PDXCHARPART GetPiece ( int i )		{ return &m_PartArray[i]; }
	void ResetPiece ( int i );
	void SetPartRend ( EMPIECECHAR emPIECE, BOOL bRender );

	const TCHAR* GetFileName()			{ return m_szFileName; }
	DxEffAniPlayer* GetEffAniPlayer()	{ return &m_EffAniPlayer; }

	void SetCharData ( DxSkinVehicleData* pCharData, LPDIRECT3DDEVICEQ pd3dDevice, DWORD dwFlag = 0x0, int nLevel=0, bool bCharacterData = FALSE );

private:
	void CollectVertexNUM();	// ������ �ٲ��� ���ؽ� ������ ������Ѵ�.
	void AvailableVB_InitVB();	// VB�� ��ȿ���� Ȯ�� �� VB�� �ʱ�ȭ.

public:
	HRESULT FrameMove( const float fTime, const float fElapsedTime, const BOOL bContinue=TRUE, BOOL const bFreeze=FALSE, const BOOL bAttackMode=TRUE );
	HRESULT Render( const LPDIRECT3DDEVICEQ pd3dDevice, const D3DXMATRIX &matRot, const BOOL bShadow=FALSE, const BOOL bEff=TRUE, const BOOL bReverse = FALSE );
	HRESULT RenderShadow( const LPDIRECT3DDEVICEQ pd3dDevice, const D3DXMATRIX &matRot, const BOOL bDay );
	HRESULT RenderReflect( const LPDIRECT3DDEVICEQ pd3dDevice, const D3DXMATRIX &matRot );

	template<class TYPE>
	HRESULT RenderEX( const LPDIRECT3DDEVICEQ pd3dDevice, const D3DXMATRIX &matRot, const TCHAR* pBoneName, const TYPE& BONEROT )
	{
		D3DXMATRIXA16	matLocalRot, matScale;
		D3DXMatrixIdentity( &matScale );
		matScale._11 = m_fScale;
		matScale._22 = m_fScale;
		matScale._33 = m_fScale;
		D3DXMatrixMultiply( &matLocalRot, &matScale, &matRot );
	
		//	Note : Skin Animation.
		DxSkinAniControl::Render( matLocalRot, pBoneName, BONEROT );

		RenderSKIN( pd3dDevice, matLocalRot );

		return S_OK;
	}
	HRESULT RenderSKIN( const LPDIRECT3DDEVICEQ pd3dDevice, const D3DXMATRIX& matLocalRot );
	void RenderBone( LPDIRECT3DDEVICEQ pd3dDevice );

public:
	HRESULT InitDeviceObjects( LPDIRECT3DDEVICEQ pd3dDevice );
	HRESULT RestoreDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice );
	HRESULT InvalidateDeviceObjects ();
	HRESULT DeleteDeviceObjects ();

public:
	DxSkinVehicle(void);
	~DxSkinVehicle(void);
};




