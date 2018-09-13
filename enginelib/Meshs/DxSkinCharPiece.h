#pragma once

#include "DxSkinMesh9.h"
#include "DxSkinObject.h"
#include "DxSkinAniMan.h"
#include "DxSkinPieceContainer.h"
#include "DxSkinPieceData.h"
#include "DxCharPart.h"
#include "DxSkinEffResult.h"

#include "../DxEffect/EffAni/DxEffAniPlayer.h"
#include "../DxEffect/EffAni/DxEffAni.h"
#include "../DxEffect/EffKeep/DxEffKeepPlayer.h"

class DxEffKeepDataMain;

// Note : ĳ����
class DxSkinCharPiece : public DxSkinAniControl
{
private:
	enum		{ FILE_LENGTH = 64 };

	BOOL			m_bWorldObj;
	float			m_fScale;

	DxEffAniPlayer	m_EffAniPlayer;
public:
	enum				{ MAX_BONE = 32 };

	DWORD				m_dwBONE;
	float				m_fBONE1[MAX_BONE];
	float				m_fBONE2[MAX_BONE];
	float				m_fBONE3[MAX_BONE];
	std::string			m_strBONE[MAX_BONE];

public:
	std::string			m_strPIECE_P[PIECE_SIZE];
	DWORD				m_dwType;
	DWORD				m_dwWeaponSlot;
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
	BOOL				m_bAttackLocal;

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
	float			m_fHeight;
	D3DXVECTOR3		m_vMax, m_vMin;
	
	D3DXVECTOR3		m_vVelocityPiece;
	D3DXVECTOR3		m_vVelocityPiece_F;

	DxCharPart		m_PartArray[PIECE_SIZE];

	//DxSkinCharPiece*	m_pSkinCharPiece[PIECE_SIZE];

private:
	DWORD			m_dwVertexNUM;

public:
	HRESULT ClearAll ();

public:
	HRESULT SetPiece ( const TCHAR* szFile, LPDIRECT3DDEVICEQ pd3dDevice, DWORD dwFlag = 0x0, int nLevel=0, bool bCharacterData = FALSE );
	HRESULT SetPieceGrindLevel ( int nIndex, int nLevel );
	void SetHairColor( WORD wColor );
	WORD GetHairColor();
//	void SetColor(WORD wColor );
	void SetColorAll( WORD wColor );
	void SetColor( WORD wColor );

	PDXCHARPART GetPiece ( int i )		{ return &m_PartArray[i]; }
	void ResetPiece ( int i );
	void SetPartRend ( EMPIECECHAR emPIECE, BOOL bRender );

	const TCHAR* GetFileName()			{ return m_szFileName; }
	DxEffAniPlayer* GetEffAniPlayer()	{ return &m_EffAniPlayer; }
	void SetWorldObj( BOOL bUse )		{ m_bWorldObj = bUse; }
	BOOL GetWorldObj()					{ return m_bWorldObj; }

	//void SetCharData ( DxSkinPieceData* pCharData, LPDIRECT3DDEVICEQ pd3dDevice, bool bCharaterData = FALSE );
	void SetCharData ( DxSkinPieceData* pCharData, LPDIRECT3DDEVICEQ pd3dDevice, DWORD dwFlag = 0x0, int nLevel=0, bool bCharacterData = FALSE );
	void SetRadiusHeight( DxSkinPieceData* pCharData );

	void GetAABBBox( D3DXVECTOR3& vMax, D3DXVECTOR3& vMin )
	{
		vMax = m_vMax;
		vMin = m_vMin;
	}

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
	DxSkinCharPiece(void);
	~DxSkinCharPiece(void);
};




