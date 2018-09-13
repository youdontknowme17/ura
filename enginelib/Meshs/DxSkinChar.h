#pragma once

#include "DxSkinMesh9.h"
#include "DxSkinObject.h"
#include "DxSkinAniMan.h"
#include "DxSkinPieceContainer.h"
#include "DxSkinCharData.h"
#include "DxCharPart.h"
#include "DxSkinEffResult.h"
#include "DxSkinCharPiece.h"

#include "DxSkinCharPiece.h"	//add abl
#include "GLCharDefine.h"
#include "DxSkinVehicle.h"		//add vcf
#include "DxSkinVehicleData.h"	//add vcf

#include "../DxEffect/EffAni/DxEffAniPlayer.h"
#include "../DxEffect/EffAni/DxEffAni.h"
#include "../DxEffect/EffKeep/DxEffKeepPlayer.h"
#include "../RanClientUILib/Interface/UIWindowEx.h"
#include "../../RanClientLib/G-Logic/GLCharDefine.h"

class DxEffKeepDataMain;

// Note : ĳ����
class DxSkinChar : public DxSkinAniControl
{
private:
	enum		{ FILE_LENGTH = 64 };

	BOOL			m_bWorldObj;
	float			m_fScale;

	DxEffAniPlayer	m_EffAniPlayer;

public:
	enum				{ MAX_BONE = 32 }; //add abl

	DWORD				m_dwBONE;
	float				m_fBONE1[MAX_BONE];
	float				m_fBONE2[MAX_BONE];
	float				m_fBONE3[MAX_BONE];
	std::string			m_strBONE[MAX_BONE];

public:
	std::string			m_strBoneLink[PIECE_SIZE];
	DWORD				m_dwPieceType[PIECE_SIZE];
	DWORD				m_dwWeaponSlot[PIECE_SIZE];

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
	
	TCHAR				m_szFileName[FILE_LENGTH];
	float				m_fHeight;
	D3DXVECTOR3			m_vMax, m_vMin;
	DxCharPart			m_PartArray[PIECE_SIZE];
	DxSkinCharPiece*	m_pSkinCharPiece[PIECE_SIZE];	//add abl
	DxSkinVehicle*		m_pSkinVehicle;					//add vcf
	BOOL				m_bBigHead;						//add abl
	float				m_fLengthSRC;					//add vcf
//
private:
	DWORD			m_dwVertexNUM;

public:
	HRESULT ClearAll ();
	HRESULT Clear( EMPIECECHAR emType );//add abl

public:
	HRESULT SetPieceWeapon ( const TCHAR* szFile, LPDIRECT3DDEVICEQ pd3dDevice, DWORD dwFlag = 0x0, int nLevel=0, bool bCharacterData = FALSE );//Added by CNDev (Extreme Reboot)
	//add abl element by CNDev
	//HRESULT SetPiece ( const TCHAR* szFile, LPDIRECT3DDEVICEQ pd3dDevice, DWORD dwFlag = 0x0, int nLevel=0, bool bCharacterData = FALSE );
	HRESULT SetPiece ( const TCHAR* szFile, LPDIRECT3DDEVICEQ pd3dDevice, DWORD dwFlag = 0x0, int nLevel=0, bool bCharacterData = FALSE, std::string strEffect = "" );

	HRESULT SetPieceGrindLevel ( int nIndex, int nLevel );
	void SetHairColor( WORD wColor );
	WORD GetHairColor();
	void SetColor( EMPIECECHAR emIndex , WORD wColor );
	void SetColorAll( WORD wColor );
	WORD GetColor( EMPIECECHAR emIndex );
	
	float GetScale(){ return m_fScale; };	//add mouse effect

	PDXCHARPART GetPiece ( int i )		{ return &m_PartArray[i]; }
	void ResetPiece ( int i );	//
	void ResetABL ( int i );	//add abl
	void ResetCPS ( int i );	//add abl
	void ResetVCF ();			//add vcf

	//add upgradeeffect
	//HRESULT SetGradeEffect ( int nType , int nSex , int nUpgradeColor, LPDIRECT3DDEVICEQ pd3dDevice , bool bCharacterData = FALSE );
	HRESULT SetGradeEffect ( int nType ,EMCHARINDEX nClassIndex , int nUpgradeColor, LPDIRECT3DDEVICEQ pd3dDevice , bool bCharacterData = FALSE );

	void SetPartRend ( EMPIECECHAR emPIECE, BOOL bRender );

	const TCHAR* GetFileName()			{ return m_szFileName; }
	DxEffAniPlayer* GetEffAniPlayer()	{ return &m_EffAniPlayer; }
	void SetWorldObj( BOOL bUse )		{ m_bWorldObj = bUse; }
	BOOL GetWorldObj()					{ return m_bWorldObj; }
	
	//add abl element by CNDev
	//BOOL SetCharPieceData( DxSkinPieceData* pCharData, LPDIRECT3DDEVICEQ pd3dDevice , std::string szBoneLink , DWORD WeaponSlot , DWORD PieceType , DWORD dwFlag, int nLevel , bool bCharacterData = FALSE);
	BOOL SetCharPieceData( DxSkinPieceData* pCharData, LPDIRECT3DDEVICEQ pd3dDevice , std::string szBoneLink , DWORD WeaponSlot , DWORD PieceType , DWORD dwFlag, int nLevel , bool bCharacterData = FALSE, std::string strEffect = "" );
	void SetCharData ( DxSkinCharData* pCharData, LPDIRECT3DDEVICEQ pd3dDevice, bool bCharaterData = FALSE );
	//add abl
//	BOOL SetCharPieceData( DxSkinPieceData* pCharData, LPDIRECT3DDEVICEQ pd3dDevice , std::string szBoneLink , DWORD WeaponSlot , DWORD PieceType , DWORD dwFlag, int nLevel , bool bCharacterData = FALSE);
	//add vcf
	HRESULT SetVehicleData( DxSkinVehicleData* pData, LPDIRECT3DDEVICEQ pd3dDevice, bool bCharaterData = FALSE );

	void SetRadiusHeight( DxSkinCharData* pCharData );
	

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
	//add vcf
	HRESULT RenderVehicle( const LPDIRECT3DDEVICEQ pd3dDevice, const D3DXMATRIX &matRot, const BOOL bShadow=FALSE, const BOOL bEff=TRUE, const BOOL bReverse = FALSE );
	//add mouse effect
	D3DXMATRIX GetMarLocal( const D3DXMATRIX &matRot );

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
	DxSkinChar(void);
	~DxSkinChar(void);
private:
	CUIControl*	CreateControl( const char* szControl );

public:
	void CreateSubControl ();
};



