#ifndef GLOGIXEX_H_
#define GLOGIXEX_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// GLCHARLOGIC, GLCROWLOGIC
//	�� Ŭ������ �������� ������ �Լ����� �������� �ٿ� "�ɸ��� ������", "NPC ������" ����
//	���� �޾Ƽ� ���� ������ �Լ��� �����ϰԵ�. ���� �Լ� �κ��� ���� ���� ����. ( ���� ȿ���� ���. )
//	��, �Լ� ������ �����ϰ� ����. ( �ٸ� ������ �ٿ����� ���������� �ζ��� �Լ� ���Ǹ� �����ϰ��Ͽ� ����. )
//
//
//
//

#include <hash_map>

#include "./GLSKILL.h"
#include "./GLCharData.h"
#include "./GLCrowData.h"
#include "./GLogicData.h"
#include "./GLContrlMsg.h"

#ifndef GLSKILL
	struct GLSKILL;
#endif //GLSKILL

typedef stdext::hash_map<DWORD,float>	DELAY_MAP;
typedef DELAY_MAP::iterator				DELAY_MAP_ITER;

class STARDIST
{
public:
	float		m_fDIST;
	STARGETID	m_sTARGETID;

public:
	STARDIST () {}

	STARDIST ( const float fDIST, const STARGETID &sTARID )
	{
		m_fDIST = fDIST;
		m_sTARGETID = sTARID;
	}

	bool operator < ( const STARDIST &rvalue )
	{
		return m_fDIST < rvalue.m_fDIST;
	}
};

typedef std::vector<STARDIST>			TARGETMAP;
typedef TARGETMAP::iterator				TARGETMAP_ITER;

typedef std::vector<STARGETID>						DETECTMAP;
typedef DETECTMAP::iterator							DETECTMAP_ITER;
typedef std::pair<DETECTMAP_ITER,DETECTMAP_ITER>	DETECTMAP_RANGE;

class STAR_ORDER
{
public:
	bool operator() ( const STARGETID &lvalue, const STARGETID &rvalue )
	{
		return lvalue.emCrow < rvalue.emCrow;
	}

	bool operator() ( const STARGETID &lvalue, const EMCROW emCROW )
	{
		return lvalue.emCrow < emCROW;
	}

	bool operator() ( const EMCROW emCROW, const STARGETID &rvalue )
	{
		return emCROW < rvalue.emCrow;
	}
};

//	������.
struct SPLAYHOSTILE
{
	BOOL	bBAD;
	float	fTIME;

	SPLAYHOSTILE ()
	{
		bBAD     = FALSE;
		fTIME    = 0;
	}
};

typedef stdext::hash_map<DWORD,SPLAYHOSTILE*>	MAPPLAYHOSTILE;
typedef MAPPLAYHOSTILE::iterator				MAPPLAYHOSTILE_ITER;

struct SSUM_ITEM
{
	GLPADATA	gdDamage;
	int			nDefense;

	int			nAvoidRate;
	int			nHitRate;

	int			nHP;
	int			nMP;
	int			nSP;
	int			nCP; //add cp

	float		fIncR_HP;
	float		fIncR_MP;
	float		fIncR_SP;

	float		fInc_HP;
	float		fInc_MP;
	float		fInc_SP;
	float		fCP_GAIN; //add cp

	SCHARSTATS	sStats;
	SRESIST		sResist;

	int			nPA;
	int			nSA;
	int			nMA;

	float		fIncR_MoveSpeed;
	float		fIncR_AtkSpeed;
	float		fIncR_Critical;
	float		fIncR_CrushingBlow;

	float		fInc_MoveSpeed;
	float		fInc_AtkSpeed;

	float		fInc_Critical_Dmg;//add critdmg
	float		fInc_CrushingBlow_Dmg;//add blowdmg

	//add directval
	int		nHP_Potion_Rate;
	int		nMP_Potion_Rate;
	int		nSP_Potion_Rate;

	//add directval hitrate and avoidrate
	float		fRateHit_Per;
	float		fRateAvoid_Per;

	SSUM_ITEM () :
		nDefense(0),

		nAvoidRate(0),
		nHitRate(0),
		nHP(0),
		nMP(0),
		nSP(0),
		nCP(0), //add cp
		fIncR_HP(0.0f),
		fIncR_MP(0.0f),
		fIncR_SP(0.0f),
		fCP_GAIN(0.0f), //add cp

		fInc_HP(0.0f),
		fInc_MP(0.0f),
		fInc_SP(0.0f),

		nPA(0),
		nSA(0),
		nMA(0),
		fIncR_MoveSpeed(0.0f),
		fIncR_AtkSpeed(0.0f),
		fIncR_Critical(0.0f),
		fIncR_CrushingBlow(0.0f),
		fInc_MoveSpeed(0.0f),
		fInc_AtkSpeed(0.0f),
		fInc_Critical_Dmg(0.0f),//add critdmg
		fInc_CrushingBlow_Dmg(0.0f),//add blowdmg
		nHP_Potion_Rate(0), //add directval
		nMP_Potion_Rate(0),//add directval
		nSP_Potion_Rate(0),//add directval
		fRateHit_Per(0.0f), //add directval
		fRateAvoid_Per(0.0f)//add directval
	{
	}

	void RESET ()
	{
		*this = SSUM_ITEM();
	}
};

namespace GLOGICEX
{
	EM_BRIGHT_FB GLSPACEGAP ( const EMBRIGHT emACTOR, const EMBRIGHT emRECEP, const EMBRIGHT emSPACE );

	//	���� ������ ���.
	int GLHITRATE ( const int nHit, const int nAvoid, const EM_BRIGHT_FB bFB );

	//	���� ��ũ Ȯ��.
	BOOL CHECKSHOCK ( const int AttackerLev, const int DefenserLev, const int nDamage, const bool bCritical );

	//	���� ���.
	int GLDEFENSE ( const int nDEFENSE, const EMBRIGHT emACTOR, const EMBRIGHT emRecep, const EMBRIGHT emSPACE );

	//	����ġ ȹ�� ( Ÿ�ݽ� )
	int GLATTACKEXP ( int AttackerLev, int DefenserLev, DWORD dwDamage, DWORD wMaxHP, DWORD dwBonusExp=0 );

	//	����ġ ȹ�� ( ���Ž� )
	int	GLKILLEXP ( int AttackerLev, int DefenserLev, DWORD dwBonusExp=0 );

	//	����ġ ȹ�� ( Ÿ�ݽ� )
	int CALCATTACKEXP ( WORD wMYLEVEL, const STARGETID &cTargetID, const GLLandMan* pLandMan, DWORD dwDamage );
	
	//	����ġ ȹ�� ( ���Ž� )
	int CALCKILLEXP ( WORD wMYLEVEL, const STARGETID &cTargetID, const GLLandMan* pLandMan );

	int CALCKILLCP ( const STARGETID &cTargetID, const GLLandMan* pLandMan ); //add cp

	//	�׾����� ����ġ ����.
	float GLDIE_DECEXP ( WORD wACTLEV );

	//	����ġ ���� ����
	float GLDIE_RECOVERYEXP ( WORD wACTLEV );

	//  ����ġ1�� ��
	float GLDIE_EXPMONEY ( WORD wACTLEV );

	//	LevelUp�� �ϱ� ���� ����ġ
	LONGLONG GLNEEDEXP ( WORD wLev );
	LONGLONG GLNEEDEXP2 ( WORD wLev );

	//	�����̻� �߻��� �� �ִ� Ȯ�� ����.
	BOOL CHECKSTATEBLOW ( float fACTRATE, WORD wACTLEVEL, WORD wLEVEL, WORD wRESIST );

	//	��ų ������ ����.
	float SKILLDELAY ( DWORD dwSKILL_GRADE, WORD wSKILL_LEV, WORD wCHAR_LEVEL, float fDelay );

	//	Note : �Ķ��Ÿ ����.
	//		���ÿ��� int ������ ����� �����Ͽ� ����� �ս��� ���´�.
	//		�� ����ÿ��� WORD �� ���� ���� �̳��� ����.
	inline WORD VARIATION ( WORD &wNow, const WORD wMax, const int nValue )
	{
		int nOld = int(wNow);

		int nNew = nOld + nValue;

		//	WORD �� ���� ���� �̳��� ����.
		if ( nNew < 0 )					nNew = 0;
		if ( nNew > USHRT_MAX )			nNew = USHRT_MAX;

		wNow = nNew;
		if ( wNow >= wMax )				wNow = wMax;

		WORD wDX = (WORD) abs(nOld-int(wNow));
		return wDX;
	}

	//	Note : �Ķ��Ÿ ����.
	//		���ÿ��� int ������ ����� �����Ͽ� ����� �ս��� ���´�.
	//		�� ����ÿ��� WORD �� ���� ���� �̳��� ����.
	inline DWORD VARIATION ( DWORD &dwNow, const DWORD dwMax, const int nValue )
	{
		int nOld = int(dwNow);

		int nNew = nOld + nValue;

		//	WORD �� ���� ���� �̳��� ����.
		if ( nNew < 0 )					nNew = 0;

		dwNow = nNew;
		if ( dwNow >= dwMax )			dwNow = dwMax;

		DWORD dwDX = (DWORD) abs(nOld-int(dwNow));
		return dwDX;
	}

	//	������ �Ӽ��� ������.
	float WEATHER_ELEMENT_POW ( EMELEMENT emElement, DWORD dwWeather, BOOL bWeatherActive );

	float WEATHER_BLOW_POW ( EMSTATE_BLOW emBlow, DWORD dwWeather, BOOL bWeatherActive );

	//	Note : ü�� ���� ��ġ���� ��ȭ���� ���� �������ش�.
	//		���ÿ��� int ������ ����� �����Ͽ� ����� �ս��� ���´�.
	//		�� ����ÿ��� WORD �� ���� ���� �̳��� ����.
	inline void UPDATE_POINT ( GLPADATA &sPOINT, float &fELP_VAR, const float fVAR, const WORD wLOW_LMT )
	{
		fELP_VAR += fVAR;
		int nNEWP = int(sPOINT.dwNow) + int(fELP_VAR);
		fELP_VAR -= int(fELP_VAR);

		//	WORD �� ���� ���� �̳��� ����.
		if ( nNEWP < 0 )			nNEWP = 0;
		//if ( nNEWP > USHRT_MAX )	nNEWP = USHRT_MAX;

		if ( nNEWP < wLOW_LMT )		nNEWP = wLOW_LMT;

		sPOINT.dwNow = nNEWP;
		sPOINT.LIMIT ();
	}

	//	Note : ü�� ���� ��ġ���� ��ȭ���� ���� �������ش�.
	//		���ÿ��� int ������ ����� �����Ͽ� ����� �ս��� ���´�.
	//		�� ����ÿ��� WORD �� ���� ���� �̳��� ����.
	inline void UPDATE_POINT ( DWORD &dwNowP, float &fELP_VAR, const DWORD MaxP, const float fVAR, const DWORD dwLOW_LMT )
	{
		fELP_VAR += fVAR;
		int nNEWP = int(dwNowP) + int(fELP_VAR);
		fELP_VAR -= int(fELP_VAR);

		//	WORD �� ���� ���� �̳��� ����.
		if ( nNEWP < 0 )				nNEWP = 0;
		if ( nNEWP < (int)dwLOW_LMT )	nNEWP = dwLOW_LMT;

		dwNowP = nNEWP;
		if ( dwNowP > MaxP )		dwNowP = MaxP;
	}

	inline void UPDATE_POINT ( WORD &wNowP, float &fELP_VAR, const WORD MaxP, const float fVAR, const WORD wLOW_LMT )
	{
		fELP_VAR += fVAR;
		int nNEWP = int(wNowP) + int(fELP_VAR);
		fELP_VAR -= int(fELP_VAR);

		//	WORD �� ���� ���� �̳��� ����.
		if ( nNEWP < 0 )			nNEWP = 0;
		if ( nNEWP > USHRT_MAX )	nNEWP = USHRT_MAX;

		if ( nNEWP < wLOW_LMT )		nNEWP = wLOW_LMT;

		wNowP = nNEWP;
		if ( wNowP > MaxP )			wNowP = MaxP;
	}
};

enum EMBEGINATTACK_FB
{
	EMBEGINA_OK		= 0,
	EMBEGINA_SP		= 1,
	EMBEGINA_ARROW	= 2,
	EMBEGINA_BULLET	= 3, //add bullet
};


//	Note : ���� �߰��ÿ�. RESET_DATA() �Լ��� ���� �ʱ�ȭ �־�ߵ�.
//
struct GLCHARLOGIC :public SCHARDATA2
{	
	EMCHARINDEX			m_CHARINDEX;
	DELAY_MAP			m_SKILLDELAY;					//	���� ��ų�� ������.
	SSKILLFACT			m_sSKILLFACT[SKILLFACT_SIZE];	//	��ųȿ����.
	SSKILLFACT			m_sPETSKILLFACT;				//  ���� ������ųȿ��
	SSKILLFACT			m_sPETSKILLFACT2;				//  ���� ������ųȿ��
	SLANDEFFECT			m_sLandEffect[EMLANDEFFECT_MULTI];	//  ���� �̻� ȿ����
	DWORD				m_dwTransfromSkill;				//  ���� ��ų �ѹ�
	float				m_fHP_RATE;						//	active ��ų�� HP ������.
	float				m_fMP_RATE;						//	active ��ų�� MP ������.
	float				m_fSP_RATE;						//	active ��ų�� SP ������.
	float				m_fCP_RATE;	
	float				m_fCP_GAIN;		//add cp
	float				m_fCP_SKILLGAIN;		//add cp

	DWORD				m_dwHOLDBLOW;					//	�����̻��� ������.
	BOOL				m_bSTATEBLOW;					//	�����̻��� �ϳ� �̻� �߻�������.
	SSTATEBLOW			m_sSTATEBLOWS[EMBLOW_MULTI];	//	�����̻�.
	float				m_fSTATE_MOVE;					//	���� �̻� �̵��ӵ�. ( 1.0f == 100% )
	float				m_fSTATE_DELAY;					//	���� �̻� ������. ( 1.0f == 100% ) (+)
	float				m_fSTATE_DAMAGE;				//	���� �̻� ����� ����.
	SQITEM_FACT			m_sQITEMFACT;					//	? ������ ���밪.
	SEVENT_FACT			m_sEVENTFACT;					//	�̺�Ʈ ? ������ ���밪.
	//SEventState			m_sEventState;
	bool				m_bSTATE_PANT;					//	�����̻� �涱��.
	bool				m_bSTATE_STUN;					//	�����̻� ����.
	bool				m_bTRANSFORM_EFF;				//Transform Function 
	float				m_fDamage;
	float				m_fDamageElapsed;				//Continue Damage 

	float				m_fDelay;						//Skill Delay recode 

	bool				m_bINVISIBLE;					//	����.
	bool				m_bRECVISIBLE;					//	���� Ȯ��.
	float				m_fSKILL_MOVE;					//	��ų �̵��ӵ�.
	float				m_fOPTION_MOVE;					//	�߰����� �̵��ӵ�. ���忡���Ϳ��� ���.
	DEFENSE_SKILL		m_sDefenseSkill;				//  �ߵ��� ��ų
	SCHARSTATS			m_sSUMSTATS;					//	�ջ� Stats.
	DWORD				m_wSUM_AP;						//	�⺻ ���ݷ�.
	DWORD				m_wSUM_DP;						//	�⺻ ����.
	DWORD				m_wSUM_PA;						//	����ġ.
	DWORD				m_wSUM_SA;						//	���ġ.
	DWORD				m_wSUM_MA;						//	����ġ.
	SRESIST				m_sSUMRESIST;					//	���װ�. ( ������ + �нú� )
	SRESIST				m_sSUMRESIST_SKILL;				//  ��ų ���װ� ( ������ + �нú� + ���� ) 
	SSUM_ITEM			m_sSUMITEM;						//	������ ���갪.
	SPASSIVE_SKILL_DATA	m_sSUM_PASSIVE;					//	��ú� ���갪.
	int					m_nSUM_PIERCE;					//	���� ���� �ջ�.
	float				m_fSUM_TARRANGE;				//	���� ���ɰŸ� �ջ�.
	DWORD				m_wACCEPTP;						//	���� �����ۿ� �ɷ����� ��� ��ġ.
	WORD				m_wSUM_DisSP;					//	�Ҹ� SP �ջ�.
	int					m_nHIT;							//	������.
	int					m_nSUM_HIT;						//	������ + ��ųȿ��.
	int					m_nAVOID;						//	ȸ����.
	int					m_nSUM_AVOID;					//	ȸ���� + ��ųȿ��.
	int					m_nDEFENSE_BODY;				//	���� ( DP + DEX+��� ).
	int					m_nDEFENSE;						//	���� ( DP + DEX*��� + ITEM_DEF ) * �������� ����.
	int					m_nDEFENSE_SKILL;				//	���� ( DP + DEX*��� + ITEM_DEF ) * �������� ���� + ��ų.
	GLPADATA			m_gdDAMAGE;						//	�⺻ ���ݷ�. ( AP + ��ú� ��ų�� ����� ���� )
	GLPADATA			m_gdDAMAGE_SKILL;				//	�⺻ ���ݷ� + temp ��ųȿ��.
	GLPADATA			m_gdDAMAGE_PHYSIC;				//	�⺻ ���ݷ� + ������� + (����ġor���ġ).
	float				m_fDamageRate;					//	���ݷ� ��ȭ��.
	float				m_fDefenseRate;					//	���� ��ȭ��.
	DAMAGE_SPEC			m_sDamageSpec;					//	������ ��� �� �ݻ�
	float				m_fINCR_HP;						//	HP ȸ����.
	float				m_fINCR_MP;						//	MP ȸ����.
	float				m_fINCR_SP;						//	SP ȸ����.		
	float				m_fATTVELO;						//	��ų ���ݼӵ� ��ȭ.
	WORD				m_wATTRANGE;					//	���� ���� �Ÿ�.
	WORD				m_wSUM_ATTRANGE;				//	���� ���� �Ÿ�.
	GLITEM_ATT			m_emITEM_ATT;					//	������ ���� �Ӽ�.
	float				m_fSKILLDELAY;
	float				m_fEXP_RATE;					//	����ġ ������.
	float				m_fGEN_RATE;					//	������ �߻���.
	EMANI_SUBTYPE		m_emANISUBTYPE;					//	Item ���뿡 ���� ���ϸ��̼� ����.
	SNATIVEID			m_idACTIVESKILL;				//	���� ��Ƽ��� ��ų.
	EMANI_MAINTYPE		m_emANIMAINSKILL;				//	��ų�� ���� ���ϸ��̼�.
	EMANI_SUBTYPE		m_emANISUBSKILL;				//	��ų�� ���� ���ϸ��̼�.

	SITEM*				m_pITEMS[SLOT_TSIZE];

	BOOL				m_bUseArmSub;					// �ذ��θ� ���� ���� ���� ��뿩��

	//	Note : ȸ���Ǵ� ��ġ ����� �ӽ� ����.
	float				m_fIncHP;
	float				m_fIncMP;
	float				m_fIncSP;

	MAPPLAYHOSTILE		m_mapPlayHostile;				//	���� ������. ( �ֹ� )

//	Ż�� �߰�
	BOOL				m_bVehicle;						//  Ż�� Ȱ��ȭ ����
	float				m_fVehicleSpeedRate;
	float				m_fVehicleSpeedVol;
	bool				m_bSafeZone;
	bool				m_bBTGactivated;
	float				m_fBTGAttackRate;

	bool				m_bAntiPot; //add antipot
	bool				m_bAntiSkill; //add antiskill

	float				m_fCrit_Dmg;//add critdmg
	float				m_fBlow_Dmg;//add blowdmg
	float				m_fCrit_Dmg_Skill;//add critdmgskill
	float				m_fBlow_Dmg_Skill;//add blowdmgskill
	float				m_fCrit_Rate;//add critrate
	float				m_fCrit_Rate_Skill;//add critrateskill
	float				m_fBlow_Rate;//add blowrate
	float				m_fBlow_Rate_Skill;//add blowrateskill

	float				m_fIncR_MoveSpeed;
	float				m_fIncR_AtkSpeed;

	float				m_fInc_MoveSpeed;
	float				m_fInc_AtkSpeed;

	//add afk reward
	float				m_fGameTimer;
	float				m_fPremTimer;
	float				m_fLandBonusMSpeed;
	GLCHARLOGIC () :
		m_CHARINDEX(GLCI_FIGHTER_M),

		m_dwHOLDBLOW(NULL),
		m_bSTATEBLOW(FALSE),
		m_fSTATE_MOVE(1.0f),
		m_fSTATE_DELAY(1.0f),
		m_fSTATE_DAMAGE(1.0f),
		m_bSTATE_PANT(false),
		m_bSTATE_STUN(false),
		m_bTRANSFORM_EFF(false), //Transform Function 
		m_fDamage(0.0f),
		m_fDamageElapsed(0.0f),
		m_fDelay(0.0f), //Skill Delay recode 
		m_bINVISIBLE(false),
		m_bRECVISIBLE(false),
		m_fHP_RATE(0),
		m_fMP_RATE(0),
		m_fSP_RATE(0),
		m_fCP_GAIN(1), //add cp
		m_fCP_SKILLGAIN(0), //add cp
		m_fCP_RATE(0), //add cp
		m_fSKILL_MOVE(0.0f),
		m_fOPTION_MOVE(0.0f),
		m_wSUM_AP(0),
		m_wSUM_DP(0),
		m_wSUM_PA(0),
		m_wSUM_SA(0),
		m_wSUM_MA(0),
		m_nSUM_PIERCE(0),
		m_fSUM_TARRANGE(0),
		m_nHIT(0),
		m_nSUM_HIT(0),
		m_nAVOID(0),
		m_nSUM_AVOID(0),
		m_wACCEPTP(0),
		m_wSUM_DisSP(0),
		m_nDEFENSE(0),
		m_fDamageRate(1.0f),
		m_fDefenseRate(1.0f),
		m_fINCR_HP(0.0f),
		m_fINCR_MP(0.0f),
		m_fINCR_SP(0.0f),
		m_fIncHP(0.0f),
		m_fIncMP(0.0f),
		m_fIncSP(0.0f),
		m_fATTVELO(0.0f),
		m_wATTRANGE(4),
		m_wSUM_ATTRANGE(4),
		m_fSKILLDELAY(0.0f),
		m_fEXP_RATE(1),
		m_fGEN_RATE(1),
		m_emITEM_ATT(ITEMATT_NOTHING),
		m_emANISUBTYPE(AN_SUB_NONE),
		m_idACTIVESKILL(NATIVEID_NULL()),
		m_emANIMAINSKILL(AN_NONE),
		m_emANISUBSKILL(AN_SUB_NONE),
		m_fLandBonusMSpeed(0.0f),
		m_bUseArmSub(FALSE),
		m_bVehicle(FALSE),
		m_fVehicleSpeedRate( 0.0f ),
		m_fVehicleSpeedVol( 0.0f ),
		m_bSafeZone(false),
		m_bBTGactivated(false),
		m_fBTGAttackRate( 0.0f ),
		m_bAntiPot(false), //add antipot
		m_bAntiSkill(false), //add antiskill
		m_fCrit_Dmg(0.0f),//add critdmg
		m_fBlow_Dmg(0.0f),//add blowdmg
		m_fCrit_Dmg_Skill(0.0f),//add critdmgskill
		m_fBlow_Dmg_Skill(0.0f),//add blowdmgskill
		m_fCrit_Rate(0.0f),//add critrate
		m_fCrit_Rate_Skill(0.0f),//add critrateskill
		m_fBlow_Rate(0.0f),//add blowrate
		m_fBlow_Rate_Skill(0.0f),//add blowrateskill
		m_fIncR_MoveSpeed(0.0f),
		m_fIncR_AtkSpeed(0.0f),
		m_fInc_MoveSpeed(0.0f),
		m_fInc_AtkSpeed(0.0f),
		//add afk reward
		m_fGameTimer(0.0f),
		m_fPremTimer(0.0f)
	{
		memset ( m_pITEMS, 0x00, sizeof(SITEM*)*SLOT_TSIZE );
	}

	~GLCHARLOGIC ()
	{
		DEL_PLAYHOSTILE_ALL();
	}
protected:	//Invisible Fix
	EMACTIONTYPE		m_Action;
	DWORD				m_dwActState;
public:		//Invisible Fix
	virtual BOOL IsACTION ( EMACTIONTYPE emCur )	{ return m_Action == emCur; }
	virtual BOOL IsSTATE ( DWORD dwState )		 			{ return m_dwActState&dwState; }
	virtual void SetSTATE ( DWORD dwState )					{ m_dwActState |= dwState; }
	virtual void ReSetSTATE ( DWORD dwState )				{ m_dwActState &= ~dwState; }
public:
	float GetQuestionTime()			{ return m_sQITEMFACT.fTime; }

public:
	EMCROW GETCROW () const			{ return CROW_PC; }
	WORD GETLEVEL () const			{ return m_wLevel; }

	WORD GETSCHOOL () const			{ return m_wSchool; }

	DWORD GETREBORN () const		{ return m_dwReborn; }

	DWORD GETHP () const			{ return m_sHP.dwNow; }
	DWORD GETMAXHP () const			{ return m_sHP.dwMax; }

	DWORD GETMP () const			{ return m_sMP.dwNow; }
	DWORD GETMAXMP () const			{ return m_sMP.dwMax; }

	DWORD GETSP () const			{ return m_sSP.dwNow; }
	DWORD GETMAXSP () const			{ return m_sSP.dwMax; }

	DWORD GETCP () const			{ return m_sCP.dwNow; } //add cp
	DWORD GETMAXCP () const			{ return m_sCP.dwMax; }	//add cp

	int GETHIT () const				{ return m_nSUM_HIT; }
	int GETAVOID () const			{ return m_nSUM_AVOID; }
	EMBRIGHT GETBRIGHT () const		{ if(m_nBright<0) return BRIGHT_DARK; return BRIGHT_LIGHT; }
	int GETBRIGHTPER () const		{ return m_nBright; }
	int GETFORCE_LOW () const		{ return m_gdDAMAGE_PHYSIC.dwLow; }
	int GETFORCE_HIGH () const		{ return m_gdDAMAGE_PHYSIC.dwHigh; }
	int GETDEFENSE () const			{ return m_nDEFENSE_SKILL; }
	
	float GETSUM_TARRANGE () const	{ return m_fSUM_TARRANGE; }		//	��Ÿ� ����(��ų)���� ����.
	int GETSUM_PIERCE () const		{ return m_nSUM_PIERCE; }

	WORD GETBODYRADIUS () const		{ return GLCONST_CHAR::wBODYRADIUS; }
	WORD GETATTACKRANGE () const	{ return m_wSUM_ATTRANGE; }	

	const SCHARSTATS& GETSTATS_ADD () const;
	const SCHARSTATS& GETSTATS_ITEM () const;

	void OptionMoveUp();
	void OptionMoveDown();

	void SetUseArmSub( BOOL bSub )	{ m_bUseArmSub = bSub; }
	BOOL IsUseArmSub() const		{ return m_bUseArmSub; }

	EMSLOT GetCurRHand();
	EMSLOT GetCurLHand();

	BOOL IsCurUseArm( EMSLOT emSlot );

	bool IsDefenseSkill()				{ return m_sDefenseSkill.m_bActive; }
	void SetDefenseSkill(bool bDefense)	{ m_sDefenseSkill.m_bActive = bDefense; }
	
	void SetLandBonusMSpeed( float val )	{ m_fLandBonusMSpeed = val; }
public:
	void SLOT_ITEM ( const SITEMCUSTOM &sItemCustom, EMSLOT _slot );
	void RELEASE_SLOT_ITEM ( EMSLOT _slot );

	const SITEMCUSTOM& GET_SLOT_ITEM ( EMSLOT _slot )				{ return m_PutOnItems[_slot]; }
	const SNATIVEID& GET_SLOT_NID ( EMSLOT _slot )					{ return m_PutOnItems[_slot].sNativeID; }
	SITEM* GET_SLOT_ITEMDATA ( EMSLOT _slot )						{ return m_pITEMS[_slot]; }
	BOOL VALID_SLOT_ITEM ( EMSLOT _slot );

	void HOLD_ITEM ( const SITEMCUSTOM &sItemCustom );
	void RELEASE_HOLD_ITEM ();
	const SITEMCUSTOM& GET_HOLD_ITEM ()					{ return m_PutOnItems[SLOT_HOLD]; }
	BOOL VALID_HOLD_ITEM ()								{ return GET_HOLD_ITEM().sNativeID!=NATIVEID_NULL(); }
	void HOLD_ARRAY( const SITEMCUSTOM &sItemCustom, int nIndex );
	const SITEMCUSTOM& GET_HOLDARRAY_ITEM (int nIndex)					{ return m_pHoldArray[nIndex]; }
	BOOL VALID_HOLDARRAY_ITEM (int nIndex)								{ return GET_HOLDARRAY_ITEM(nIndex).sNativeID!=NATIVEID_NULL(); }
	void RELEASE_HOLDARRAY_ITEM (int nIndex);
	void SetSortTimer();

	BOOL ISLONGRANGE_ARMS ();

	BOOL ISHAVEITEM ( SNATIVEID &sNID, DWORD *pNum=NULL );
	BOOL ISREVIVE ();

	SITEM* GET_ELMT_ITEM ();	//	�޼�, ������ �������߿��� 'BLOW' �Ӽ��� �ִ� �������� ��ȯ.

	WORD GETSKILLRANGE_TAR ( const GLSKILL &sSKILL ) const;
	WORD GETSKILLRANGE_APPLY ( const GLSKILL &sSKILL, const WORD dwLEVEL ) const;

public:
	EMBEGINATTACK_FB BEGIN_ATTACK ( WORD wStrikeNum );
	BOOL VALID_LEVELUP () const;
	LONGLONG GET_LEVELUP_EXP () const;
	void LEVLEUP ( bool bInitNowExp = TRUE );
	void STATSUP ( EMSTATS emStats );
	void STATSUP ( SCHARSTATS sStats );
	void STATSUP_CMD ( EMSTATS emStats, DWORD value);		//add addstats cmd
	//add pk
	void PKWINUP ();
	void PKLOSSUP ();
	void TDCOUNTSGNUC ();
	void TDCOUNTMPNUC ();
	void TDCOUNTPHXNUC ();
	
	void TDCOUNTSGFAC ();
	void TDCOUNTMPFAC ();
	void TDCOUNTPHXFAC ();
	
	void TDCOUNTSGTHE ();
	void TDCOUNTMPTHE ();
	void TDCOUNTPHXTHE ();

	void TDTotalPHX();
	void TDTotalSG();
	void TDTotalMP();

	void TDResetPHX();
	void TDResetSG();
	void TDResetMP();
	void ShowRankName (BOOL bShowName); //add pkrank
	void ShowRankMark (BOOL bShowMark); //add pkrank
	void CP_UP( DWORD dwRCLvl , WORD wRCNum ); //add cp
	void AllowBet( BOOL bAllowBet ); //add duel bet

	void GETREWARD ( LONGLONG lnValue );

	BOOL DuelWinReward ( LONGLONG llWin, WORD wType  ); //add duel bet
	BOOL DuelLossReward ( LONGLONG llLoss, WORD wType  ); //add duel bet

	BOOL IsCharCanBet() { return m_bAllowBet; } //add duel bet

	bool RESET_STATS_SKILL ( const DWORD dwDIS_STAT=UINT_MAX );
	bool RESET_SKILL ();
	bool RESET_STATS ( const DWORD dwDIS=UINT_MAX );

protected:
	void SUM_ITEM ();
	void SUM_PASSIVE ();
	void SUM_ADDITION ( float fCONFT_POINT_RATE );
	void UPDATE_ITEMSETLIST ();//add giftset

public:
	void INIT_NEW_CHAR ( const EMCHARINDEX _emCIndex, const DWORD dwUserID, const DWORD dwServerID, const char* szCharName,
		const WORD _wSchool=0, const WORD wHair=0, const WORD wFace=0, const WORD _wHairColor=0, const WORD _wSex=0 );

	BOOL INIT_DATA ( BOOL bNEW, BOOL bReGen, float fCONFT_POINT_RATE = 1.0f, bool bInitNowExp = TRUE );

	void INIT_RECOVER ( int nRECOVER=30 );

	void RESET_DATA ();

public:
	void UPDATE_DATA ( float fTime, float fElapsedTime, BOOL bClient=FALSE, float fCONFT_POINT_RATE=1.0f );
	void UPDATE_MAX_POINT ( float fCONFT_POINT_RATE );
	bool UPDATE_QITEMFACT ( float fElapsedTime );
	float CHECK_QBOXTIME ( float fElapsedTime );//add qbox

public:
	BOOL CHECKSLOT_ITEM ( SNATIVEID sNativeID, EMSLOT emSlot );
	BOOL ACCEPT_ITEM ( SNATIVEID sNativeID );
	BOOL ACCEPT_ITEM ( SNATIVEID sNativeID, SNATIVEID sDisguiseID );
	BOOL ISEMPTY_SLOT ( SNATIVEID sNativeID, EMSLOT emSlot );
	DWORD CALC_ACCEPTP ( SNATIVEID sNativeID );
	BOOL SIMPLE_CHECK_ITEM ( SNATIVEID sNativeID );
	BOOL SIMPLE_CHECK_ITEM ( SNATIVEID sNativeID, SNATIVEID sDisguiseID );

public:
	BOOL CHECKHIT ( const STARGETID &cTargetID, const GLLandMan* pLandMan, const BOOL bLowSP );

	/**
	* ĳ���Ͱ� ���� ���������� ������ ��� ����
	*/
	DWORD CALCDAMAGE(
		int &rResultDAMAGE,
		const DWORD	dwGaeaID,
		const STARGETID &cTargetID,
		const GLLandMan* pLandMan,
		const GLSKILL* pSkill=NULL,
		const DWORD dwskill_lev=0,
		const DWORD dwWeatherFlag=NULL,
		const int dwDivCount=0);
	
	DWORD CALCDAMAGE_20060328(
		int &rResultDAMAGE,
		const DWORD	dwGaeaID,
		const STARGETID &cTargetID,
		const GLLandMan* pLandMan,
		const GLSKILL* pSkill=NULL,
		const DWORD dwskill_lev=0,
		const DWORD dwWeatherFlag=NULL,
		const int dwDivCount=0);

	DWORD CALCDAMAGE_2004(
		int &rResultDAMAGE,
		const DWORD	dwGaeaID,
		const STARGETID &cTargetID,
		const GLLandMan* pLandMan,
		const GLSKILL* pSkill=NULL,
		const DWORD dwskill_lev=0,
		const DWORD dwWeatherFlag=NULL,
		const int dwDivCount=0);

	

	DWORD RECEIVE_DAMAGE ( const DWORD wDamage );

public:
	BOOL RECEIVE_QITEMFACT ( const SNATIVEID &nidITEM );
	//	Memo :	�̺�Ʈ�� ���� �޴� ?������ ȿ���� �����Ѵ�.
	void RECEIVE_EVENTFACT_BEGIN ( const EMGM_EVENT_TYPE emType, const WORD wValue );
	//	Memo :	�̺�Ʈ�� ���� �޴� ?������ ȿ���� �����Ѵ�.
	void RECEIVE_EVENTFACT_END ( const EMGM_EVENT_TYPE emType );

public:
	BOOL ISLEARNED_SKILL ( SNATIVEID skill_id, WORD wLEVEL=0 );
	BOOL ISMASTER_SKILL ( SNATIVEID skill_id );
	SCHARSKILL* GETLEARNED_SKILL ( SNATIVEID skill_id );

	EMSKILL_LEARNCHECK CHECKLEARNABLE_SKILL ( SNATIVEID skill_id );
	void LEARN_SKILL ( SNATIVEID skill_id );
	void LEARN_SKILL_NONSCROLL ( SNATIVEID skill_id );
	VOID LVLUP_SKILL ( SNATIVEID skill_id, WORD wToLevel );

	bool LEARN_SKILL_QUEST ( SNATIVEID skill_id );

public:
	float GETATTVELO ();
	float GETMOVEVELO ();
	float GETATT_ITEM (); // ����ġ ����
	float GETMOVE_ITEM (); // ����ġ ���� % 

	float GetBtgAddAttack ();//add btg attack
	void SetBtgAddAttack ( BOOL bEnabled ,float AddAttackRate );//add btg attack

public:
	//	Note : ������ ���. ( pk ���� ),	true : �ű� ��Ͻ�, false : ���� ��� ���Ž�.
	bool ADD_PLAYHOSTILE ( DWORD dwCHARID, BOOL bBAD );
	bool DEL_PLAYHOSTILE ( DWORD dwCHARID );
	void DEL_PLAYHOSTILE_ALL ();

public:
	//	Note : ���� ����Ʈ�� ��� �Ǿ� �ִ°�?
	bool IS_PLAYHOSTILE ( DWORD dwCHARID );
	//	Note : ����������?
	bool IS_HOSTILE_ACTOR ( DWORD dwCHARID );
	//	Note : �ڽ��� �������ΰ�?
	bool ISOFFENDER ();

	bool ISPLAYKILLING ();

public:
	//	Note : ���� pk ������ �˾ƺ���. ( UINT_MAX �ϰ�� pk �ش���� ����. )
	DWORD GET_PK_LEVEL ();

	//	Note : ������ ����ġ ���� pk ���.
	float GET_PK_DECEXP_RATE ();
	std::string GET_PK_NAME ();
	DWORD GET_PK_COLOR ();
	float GET_PK_SHOP2BUY ();
	float GET_PK_SHOP2SALE ();

	DWORD GET_PK_ITEMDROP_NUM ();
	float GET_PK_ITEMDROP_RATE ();

	DWORD GET_RANK_COLOR (); // add rank color

public:
	BOOL DOGRINDING(SITEMCUSTOM &sCusItem, 
					const SITEM *pHold, 
					bool &bRESET, 
					bool &bTERMINATE, 
					bool &bNOFAIL,  //add nofailups
					EMANTIDISAPPEAR &emANTIDISAPPEAR );

public:
	BOOL ISSTATEBLOW ()							{ return m_bSTATEBLOW; }
	BOOL ISANTIPOTS ()							{ return m_bAntiPot; } //added by CNDev
	BOOL ISANTISKILL ()							{ return m_bAntiSkill; } //added by CNDev
	BOOL ISANTISTUN ()							{ return m_bSTATE_STUN; } //added by CNDev
	void DISABLESKEFF ( int i );
	void DISABLEBLOW ( int i )					{ m_sSTATEBLOWS[i].emBLOW = EMBLOW_NONE; }
	void RESETAGESKEFF ( int i )				{ m_sSKILLFACT[i].fAGE = 0.0f; }
	void RESETAGEBLOW ( int i )					{ m_sSTATEBLOWS[i].fAGE = 0.0f; }

	void DISABLEALLLANDEFF();
	void ADDLANDEFF( SLANDEFFECT landEffect, int iNum );


	void UPDATESKILLDELAY ( float fElapsedTime );

	void SETACTIVESKILL ( SNATIVEID skill_id );
	SNATIVEID GETACTIVESKILL ()					{ return m_idACTIVESKILL; }
	EMSKILLCHECK CHECHSKILL ( SNATIVEID skill_id, WORD wStrikeNum, bool bNotLearn = false );
	void ACCOUNTSKILL ( SNATIVEID skill_id, WORD wStrikeNum, bool bServer=false );
	
public:
	virtual void ReSelectAnimation ()	{}
};

//	Note : ���� �߰��ÿ�. RESET_DATA() �Լ��� ���� �ʱ�ȭ �־�ߵ�.
//
struct GLCROWLOGIC
{
	SNATIVEID			m_sNativeID;					//	Crow NativeID.
	PCROWDATA			m_pCrowData;					//	Crow �⺻ ����.

	float				m_fACTIONDELAY[SCROWDATA::EMMAXATTACK];	//	���� cool time;

	SSKILLFACT			m_sSKILLFACT[SKILLFACT_SIZE];	//	��ųȿ����.

	DWORD				m_dwHOLDBLOW;					//	�����̻��� ������.

	SSTATEBLOW			m_sSTATEBLOWS[EMBLOW_MULTI];	//	�����̻�.
	float				m_fSTATE_MOVE;					//	���� �̻� �̵��ӵ�. ( 1.0f == 100% )
	float				m_fSTATE_DELAY;					//	���� �̻� ������. ( 1.0f == 100% ) (+)
	float				m_fSTATE_DAMAGE;				//	���� �̻� ����� ����.
	
	bool				m_bSTATE_PANT;					//	�����̻� �涱��.
	bool				m_bSTATE_STUN;					//	�����̻� ����.
	bool				m_bTRANSFORM_EFF;				//Transform Function 
	float				m_fDamage;
	float				m_fDamageElapsed;				//
	bool				m_bINVISIBLE;					//	����.
	bool				m_bRECVISIBLE;					//	���� Ȯ��.

	float				m_fATTVELO;						//	���ݼӵ�.
	float				m_fSKILL_MOVE;					//	��ų �̵��ӵ�.


	int					m_nSUM_HIT;						//	������ + ��ųȿ��.
	int					m_nSUM_AVOID;					//	ȸ���� + ��ųȿ��.

	int					m_nSUM_DEFENSE;					//	���� ( DP + POW*��� + ITEM_DEF ) * �������� ���� + ��ų.
	int					m_nDX_DAMAGE;					//	�⺻ ���ݷ�. + ��ųȿ��.
	
	SRESIST				m_sSUMRESIST;					//	���װ�.

	int					m_nSUM_PIERCE;					//	�߰� ���� ����.
	float				m_fSUM_TARRANGE;				//	���� ���ɰŸ� �ջ�.

	float				m_fDamageRate;					//	���ݷ� ��ȭ��.
	float				m_fDefenseRate;					//	���� ��ȭ��.

	DAMAGE_SPEC			m_sDamageSpec;					//	������ ��� �� �ݻ�
	DEFENSE_SKILL		m_sDefenseSkill;				//  �ߵ��� ��ų

	DWORD				m_dwAType;						//	���� �Ӽ� ����.
	
	DWORD				m_dwNowHP;						//	����.
	WORD				m_wNowMP;						//	���ŷ�.
	WORD				m_wNowSP;						//	���ŷ�.

	SNATIVEID			m_idACTIVESKILL;				//	Ȱ�� ��ų.
	WORD				m_wACTIVESKILL_LVL;				//	Ȱ�� ��ų  ����.

	//	Note : ȸ���Ǵ� ��ġ ����� �ӽ� ����.
	float				m_fIncHP;
	float				m_fIncMP;
	float				m_fIncSP;

	WORD				m_wOWNER;
	//	Note : ����E�߰��ÿ�. RESET_DATA() �Լ��� ����E�ʱ�ȭ �־��ߵ�.
	DWORD				m_dwNowHPSG;
	DWORD				m_dwNowHPMP;
	DWORD				m_dwNowHPPHX;

	SW_SCHOOL_DATA		m_emOWNER;

	//flagwars by CNDev
	FW_SCHOOL_DATA		m_emKILLER;
	//

	GLCROWLOGIC () :
		m_sNativeID(0,0),
		m_pCrowData(NULL),

		m_dwHOLDBLOW(NULL),
		m_fSTATE_MOVE(1.0f),
		m_fSTATE_DELAY(1.0f),
		m_fSTATE_DAMAGE(1.0f),

		m_bSTATE_PANT(false),
		m_bSTATE_STUN(false),
		m_bTRANSFORM_EFF(false), //Transform Function 
		m_fDamage(0.0f),
		m_fDamageElapsed(0.0f),
		m_bINVISIBLE(false),
		m_bRECVISIBLE(false),

		m_fATTVELO(0.0f),
		m_fSKILL_MOVE(0.0f),

		m_nSUM_HIT(0),
		m_nSUM_AVOID(0),

		m_nSUM_DEFENSE(0),
		m_nDX_DAMAGE(0),

		m_fSUM_TARRANGE(0),
		m_nSUM_PIERCE(0),

		m_fDamageRate(1.0f),
		m_fDefenseRate(1.0f),

		m_dwAType(0),

		m_dwNowHP(0),
		m_wNowMP(0),
		m_wNowSP(0),
		
		m_idACTIVESKILL(false),
		m_wACTIVESKILL_LVL(0),

		m_fIncHP(0.0f),
		m_fIncMP(0.0f),
		m_fIncSP(0.0f),
		m_wOWNER( CROW_OWNER_SCHOOL ),
		m_dwNowHPSG(0),
		m_dwNowHPMP(0),
		m_dwNowHPPHX(0),
		m_emOWNER( SW_SCHOOL_MAX ),
		//flagwars by CNDev
		m_emKILLER( FW_SCHOOL_MAX )
	{
		for ( int i=0; i< SCROWDATA::EMMAXATTACK; ++i )
			m_fACTIONDELAY[i] = 0.0f;
	}

	void RESET_DATA ();

	bool VALIDACTION ()
	{
		const SCROWATTACK *pAttack = m_pCrowData->m_sCrowAttack;
		for ( int i=0; i<SCROWDATA::EMMAXATTACK; ++i )
		{
			if ( pAttack[i].bUsed )		return true;
		}

		return true;
	}

	bool VALIDSKILL ()
	{
		const SCROWATTACK *pAttack = m_pCrowData->m_sCrowAttack;
		for ( int i=0; i<SCROWDATA::EMMAXATTACK; ++i )
		{
			if ( pAttack[i].bUsed && pAttack[i].skill_id!=SNATIVEID(false) )		return true;
		}

		return true;
	}

	bool VALIDATTACK ()
	{
		const SCROWATTACK *pAttack = m_pCrowData->m_sCrowAttack;
		for ( int i=0; i<SCROWDATA::EMMAXATTACK; ++i )
		{
			if ( pAttack[i].bUsed && pAttack[i].skill_id==SNATIVEID(false) )		return true;
		}

		return true;
	}

	EMCROW GETCROW () const 	{ return m_pCrowData->m_emCrow; }
	WORD GETLEVEL () const		{ return m_pCrowData->m_wLevel; }
	const char* GETNAME ();
	
	DWORD GETHP () const		{ return m_dwNowHP; }
	DWORD GETMAXHP () const		{ return m_pCrowData->m_dwHP; }

	DWORD GETHPSG () const		{ return m_dwNowHPSG; }
	DWORD GETHPMP () const		{ return m_dwNowHPMP; }
	DWORD GETHPPHX () const		{ return m_dwNowHPPHX; }
	int GETMP () const			{ return m_wNowMP; }
	int GETMAXMP () const		{ return m_pCrowData->m_wMP; }

	int GETSP () const			{ return m_wNowSP; }
	int GETMAXSP () const		{ return m_pCrowData->m_wSP; }

	int GETHIT () const			{ return m_nSUM_HIT; }
	int GETAVOID () const		{ return m_nSUM_AVOID; }

	EMBRIGHT GETBRIGHT () const	{ return m_pCrowData->m_emBright; }

	int GETFORCE_LOW () const;
	int GETFORCE_HIGH () const;
	int GETDEFENSE () const			{ return m_nSUM_DEFENSE; }

	float GETSUM_TARRANGE () const	{ return m_fSUM_TARRANGE; }		//	��Ÿ� ����(��ų)���� ����.
	int GETSUM_PIERCE () const		{ return m_nSUM_PIERCE; }

	WORD GETBODYRADIUS () const		{ return m_pCrowData->m_sAction.m_wBodyRadius; }
	WORD GETATTACKRANGE () const	{ return m_pCrowData->m_sCrowAttack[m_dwAType].wRange; }

	WORD GETSKILLRANGE_APPLY ( const GLSKILL &sSKILL, const WORD dwLEVEL ) const;

	BOOL INIT_DATA ();

	BOOL CHECKHIT ( const STARGETID &cTargetID, const GLLandMan* pLandMan );
	DWORD CALCDAMAGE ( int& rResultDAMAGE, const DWORD dwGaeaID, const STARGETID &cTargetID, const GLLandMan* pLandMan,
		const GLSKILL* pSkill=NULL, DWORD dwskill_lev=0, DWORD dwWeatherFlag=NULL, DWORD dwDivCount=0 );

	DWORD RECEIVE_DAMAGE ( const DWORD wDamage );
	DWORD RECEIVE_DAMAGE ( const DWORD dwDamage, const WORD wSchool );

	void DISABLESKEFF ( int i );
	void DISABLEBLOW ( int i )					{ m_sSTATEBLOWS[i].emBLOW = EMBLOW_NONE; }

	void UPDATE_DATA ( float fTime, float fElapsedTime, BOOL bClient=FALSE );

	void SETACTIVESKILL ( SNATIVEID skill_id, WORD wLevel );
	SNATIVEID GETACTIVESKILL ()						{ return m_idACTIVESKILL; }
	EMSKILLCHECK CHECHSKILL ( DWORD dwAType, bool bNotLearn = false );
	void ACCOUNTSKILL ( WORD wStrikeNum );

	float GETATTVELO ();
	float GETMOVEVELO ();

	bool IsDefenseSkill()				{ return m_sDefenseSkill.m_bActive; }
	void SetDefenseSkill(bool bDefense)	{ m_sDefenseSkill.m_bActive = bDefense; }
};


EMANI_SUBTYPE CHECK_ANISUB ( SITEM* pRHAND, SITEM* pLHAND );
EMANI_SUBTYPE CHECK_ATTACK_ANISUB ( SITEM* pRHAND, SITEM* pLHAND );

//! ������ ȹ�� ������ �ֱ�����...
//! ������ �α׸� ����ϱ� ���� ����ü
struct SDAMAGELOG
{
	DWORD dwUserID; ///< User ID
	DWORD dwGaeaID; ///< ���̾� ID
	DWORD dwDamage; ///< �� ������	

	SDAMAGELOG () 
		: dwUserID(0)
		, dwGaeaID(0)
		, dwDamage(0)
	{
	}

	SDAMAGELOG ( DWORD dwuserid, DWORD dwgaeaid, DWORD dwdamage ) 
		: dwUserID(dwuserid)
		, dwGaeaID(dwgaeaid)
		, dwDamage(dwdamage)
	{
	}
};

typedef stdext::hash_map<DWORD, SDAMAGELOG>	DAMAGELOG;
typedef DAMAGELOG::iterator					DAMAGELOG_ITER;

inline void AddDamageLog ( DAMAGELOG &cDamageLog, DWORD dwGaeaID, DWORD dwUserID, DWORD dwDamage )
{
	DAMAGELOG_ITER iter = cDamageLog.find ( dwUserID );
	DAMAGELOG_ITER iter_end = cDamageLog.end();
	
	if ( iter != iter_end )
	{
		SDAMAGELOG &sDamageLog = (*iter).second;
		if ( sDamageLog.dwUserID == dwUserID )	dwDamage += sDamageLog.dwDamage;
	}
	
	cDamageLog[dwUserID] = SDAMAGELOG(dwUserID,dwGaeaID,dwDamage);
}

//! �߰�:2006-04-12 Jgkim
//! ��Ƽ ������ �αױ���� ���� ����ü
struct SDAMAGELOGPARTY
{
	DWORD m_dwPartyID; ///< ��Ƽ ������ȣ
	DWORD m_dwDamage; ///< ��Ƽ�� �� ������

	SDAMAGELOGPARTY()
		: m_dwPartyID(0)
		, m_dwDamage(0)
	{
	}

	SDAMAGELOGPARTY(DWORD dwPartyID, DWORD dwDamage)
		: m_dwPartyID(dwPartyID)
		, m_dwDamage(dwDamage)
	{
	}
};
typedef stdext::hash_map<DWORD, SDAMAGELOGPARTY>	DAMAGELOGPARTY;
typedef DAMAGELOGPARTY::iterator					DAMAGELOGPARTY_ITER;

inline void AddDamageLogParty(DAMAGELOGPARTY &cDamageLogParty, DWORD dwPartyID, DWORD dwDamage)
{
	if (dwPartyID == PARTY_NULL) return;

	DAMAGELOGPARTY_ITER iter     = cDamageLogParty.find(dwPartyID);
	DAMAGELOGPARTY_ITER iter_end = cDamageLogParty.end();
	
	if (iter != iter_end)
	{
		SDAMAGELOGPARTY &sDamageLogParty = (*iter).second;
		if (sDamageLogParty.m_dwPartyID == dwPartyID)
		{
			dwDamage += sDamageLogParty.m_dwDamage;
		}
	}	
	cDamageLogParty[dwPartyID] = SDAMAGELOGPARTY(dwPartyID, dwDamage);
}

inline bool CHECHSKILL_ITEM ( GLITEM_ATT emSKILL, GLITEM_ATT emITEM )
{
	if ( emSKILL==ITEMATT_SWORDSABER )
	{
		return ( emITEM==ITEMATT_SWORD || emITEM==ITEMATT_SABER );
	}
	else if ( emSKILL==ITEMATT_GUN )
	{
		return ( emITEM==ITEMATT_RAIL_GUN || emITEM==ITEMATT_PORTAL_GUN || emITEM==ITEMATT_DUAL_GUN );
	}
	else if ( emSKILL==ITEMATT_FIST )
	{
		return ( emITEM==ITEMATT_SWORD || emITEM==ITEMATT_SABER  || emITEM == ITEMATT_SWORDSABER || 
				 emITEM==ITEMATT_SPEAR || emITEM==ITEMATT_STICK || emITEM==ITEMATT_GAUNT || emITEM==ITEMATT_BOW || emITEM==ITEMATT_DUAL_GUN ||
				 emITEM==ITEMATT_RAIL_GUN || emITEM==ITEMATT_PORTAL_GUN || emITEM==ITEMATT_DUALSPEAR || emITEM==ITEMATT_SHURIKEN || emITEM==ITEMATT_SCYTHE ||
				 emITEM==ITEMATT_WAND || emITEM==ITEMATT_CUBE  || emITEM==ITEMATT_WHIP || emITEM==ITEMATT_FIST || emITEM==ITEMATT_SHIELD || emITEM==ITEMATT_HAMMER || emITEM==ITEMATT_UMBRELLA );
	}

	return (emITEM==emSKILL);
}

D3DXVECTOR3 UpdateSkillDirection ( const D3DXVECTOR3 &vMyPos, const D3DXVECTOR3 vMyDir, const SNATIVEID sSKILLID, const D3DXVECTOR3 vTARPOS, const STARID *pTARID );

#endif // GLOGIXEX_H_