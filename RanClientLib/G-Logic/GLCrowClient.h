#ifndef GLCROWCLIENT_H_
#define GLCROWCLIENT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GLCrowData.h"
#include "GLCOPY.h"
#include "GLogicEx.h"

#include "../../EngineLib/NaviMesh/Actor.h"
#include "DxSkinChar.h"

#ifndef GLLandManClient
	class GLLandManClient;
	class GLClientNode;
	typedef SQUADNODE<GLClientNode> CLIENTQUADNODE;
#endif //GLLandMan

#define _SKILLFACT_DEFAULTTIME		(-1.0f)

class GLCrowClient : public GLCROWLOGIC, public GLCOPY
{
protected:
	LPDIRECT3DDEVICEQ	m_pd3dDevice;
	Actor				m_actorMove;
	DxSkinChar*			m_pSkinChar;

	D3DXVECTOR3			m_vDir;
	D3DXVECTOR3			m_vDirOrig;
	D3DXVECTOR3			m_vPos;
	D3DXMATRIX			m_matTrans;
	float				m_fScale;

	D3DXVECTOR3			m_vServerPos;

	//	Note : AABB
protected:
	D3DXVECTOR3		m_vMaxOrg;
	D3DXVECTOR3		m_vMinOrg;

public:
	D3DXVECTOR3		m_vMax;
	D3DXVECTOR3		m_vMin;
	float			m_fHeight;

	//	Note : ���� �ڵ�.
	//
public:
	DWORD				m_dwGlobID;				//	���� �޸� �ε����� ( �ش� GLLandMan������ ����ũ�� )
	DWORD				m_dwCeID;				//	�� ID.

	GLLandManClient*		m_pLandManClient;	//	�� �ν��Ͻ� ������.
	SGLNODE<GLCrowClient*>*	m_pGlobNode;		//	�� ����Ʈ ���.
	CLIENTQUADNODE*			m_pQuadNode;		//	���� Ʈ�� ���.
	SGLNODE<GLCrowClient*>*	m_pCellNode;		//	�� ����Ʈ ���.

	//	Note : �ൿ.
protected:
	float				m_fAge;					//	��� �ð�.
	EMACTIONTYPE		m_Action;				//	�ൿ.
	DWORD				m_dwActState;			//	�ൿ ����.

	STARGETID			m_sTargetID;			//	��ǥ ����.

	WORD				m_wTARNUM;				//	Ÿ�� ����.
	STARID				m_sTARIDS[EMTARGET_NET];//	Ÿ�ٵ�.
	D3DXVECTOR3			m_vTARPOS;				//	Ÿ�� ��ġ.

	SCROWATTACK*		m_pAttackProp;			//	���� �Ӽ�.

protected:
	float				m_fIdleTime;			//	�޽� �ð�.
	float				m_fStayTimer;			//	�޽� ������.
	float				m_fAttackTimer;			//	���� ������.
	float				m_fTargetTimer;			//	Ÿ�� �˻� Ÿ�̸�.
	float				m_fMoveDelay;			//	�̵� ������.

public:
	BOOL IsValidBody ();
	BOOL IsDie ();
	BOOL IsHaveVisibleBody ();

	BOOL IsVisibleDetect ();

public:
	BOOL IsSTATE ( DWORD dwState )					{ return m_dwActState&dwState; }
	virtual BOOL IsACTION ( EMACTIONTYPE emCur )	{ return m_Action == emCur; }

protected:
	void SetSTATE ( DWORD dwState )					{ m_dwActState |= dwState; }
	void ReSetSTATE ( DWORD dwState )				{ m_dwActState &= ~dwState; }

public:
	HRESULT CreateCrow ( GLLandManClient *pLandManClient, SDROP_CROW *pCrowDrop, LPDIRECT3DDEVICEQ pd3dDevice );

public:
	void SetPosition ( D3DXVECTOR3 &vPos );
	virtual const D3DXVECTOR3 &GetPosition ()		{ return m_vPos; }
	float GetDirection ();
	DxSkinChar* GetCharSkin ()						{ return m_pSkinChar; }

public:
	float GetMoveVelo ();
	void TurnAction ( EMACTIONTYPE toAction );

public:
	virtual EMCROW GetCrow () const			{ return GETCROW(); }
	virtual DWORD GetCtrlID () const		{ return m_dwGlobID; }
	virtual const char* GetName () const	{ return m_pCrowData->GetName(); }
	virtual GLPWDATA GetHp () const			{ return GLPWDATA(m_dwNowHP,m_pCrowData->m_dwHP); }  //original gldwdata
	virtual GLPWDATA GetHpSg () const			{ return GLPWDATA(m_dwNowHPSG,m_pCrowData->m_dwHP); }
	virtual GLPWDATA GetHpMp () const			{ return GLPWDATA(m_dwNowHPMP,m_pCrowData->m_dwHP); }
	virtual GLPWDATA GetHpPhx () const			{ return GLPWDATA(m_dwNowHPPHX,m_pCrowData->m_dwHP); }
	virtual WORD GetSchool() const				{ return 0; }

	virtual WORD GetBodyRadius ();
	virtual float GetBodyHeight()			{ return m_fHeight; }
	//add mouse effect
	virtual float GetScale()			{ return ( GetCharSkin() != NULL ) ? GetCharSkin()->GetScale() : 0.f; }
	virtual DxSkinChar* GetSkinCharF() { return GetCharSkin(); }
	virtual D3DXVECTOR3 GetDir () { return m_vDir; }
	virtual D3DXVECTOR3 GetDirOrig () { return m_vDirOrig; };

	virtual D3DXVECTOR3 GetPosBodyHeight ()	{ return D3DXVECTOR3( m_vPos.x, m_vPos.y+m_fHeight, m_vPos.z ); }

public:
	virtual void MsgProcess ( NET_MSG_GENERIC* nmg );
	virtual void ReceiveDamage ( DWORD wDamage, DWORD dwDamageFlag, STARGETID sACTOR );
	virtual void ReceiveAVoid ();
	virtual void ReceiveSwing ();

protected:
	HRESULT UpateAnimation ( float fTime, float fElapsedTime );
	
	BOOL RECEIVE_SKILLFACT ( const SNATIVEID skill_id, const WORD wlevel, const DWORD dwSELECT, float fAge = _SKILLFACT_DEFAULTTIME );

protected:
	float m_fattTIMER;
	int m_nattSTEP;

protected:
	void StartAttackProc ();
	BOOL AttackProc ( float fElapsedTime );
	void AttackEffect ( const SANIMSTRIKE &sStrikeEff );

protected:
	void StartSkillProc ();
	BOOL SkillProc ( float fElapsedTime );

	void SKT_EFF_HOLDOUT ( STARGETID sTarget, DWORD dwDamageFlag );

	void SK_EFF_TARG ( const PGLSKILL pSkill, const SANIMSTRIKE &sStrikeEff, const STARGETID &sTarget );
	void SK_EFF_SELFZONE ( const SANIMSTRIKE &sStrikeEff, const SKILL::EMEFFECTPOS emPOS, const char* const szZONE_EFF, STARGETID *pTarget=NULL );
	void SK_EFF_TARZONE ( const STARGETID &sTarget, const SKILL::EMEFFECTPOS emPOS, const char* const szZONE_EFF );
	void SkillEffect ( const SANIMSTRIKE &sStrikeEff );

public:
	BOOL IsCollisionVolume ();
	HRESULT FrameMove ( float fTime, float fElapsedTime );

	HRESULT RenderShadow ( LPDIRECT3DDEVICEQ pd3dDevice, CLIPVOLUME &cv );
	HRESULT Render ( LPDIRECT3DDEVICEQ pd3dDevice, CLIPVOLUME &cv, BOOL bRendAABB );

public:
	HRESULT RestoreDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice );
	HRESULT InvalidateDeviceObjects ();
//Transform Function 
public:
	BOOL	DoTransform(PGLSKILL pSkill);
	void	ResetTransform();
	BOOL	IsHit();

protected:
	bool		m_bSkinUpdate;
	D3DXVECTOR3	m_vMinY_;
	D3DXVECTOR3	m_vMaxY_;
//Attack checker by NaJDeV
protected:
	bool		m_bVALIDTARGET;
	DWORD		m_dwTarID;

public:
	bool		IsTARGETVALID() { return m_bVALIDTARGET; }

	DWORD		GetTarID() { return m_dwTarID; }

public:
	GLCrowClient(void);
	~GLCrowClient(void);
};

typedef GLCrowClient* PGLCROWCLIENT;

typedef CGLLIST<PGLCROWCLIENT>	GLCROWCLIENTLIST;
typedef SGLNODE<PGLCROWCLIENT>	GLCROWCLIENTNODE;

#endif // GLCROWCLIENT_H_