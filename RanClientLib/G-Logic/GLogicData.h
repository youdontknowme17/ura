#pragma once

#include <string>

#include "./GLCharData.h"
#include "./GLParty.h"
#include "./GLInventory.h"
#include "./GLPet.h"
#include "./GLVEHICLE.h"
#include "./GLSUMMON.h"

#include "../enginelib/Common/SFileSystem.h"
#include "../EngineLib/Meshs/DxSkinAniMan.h"
#include "../NetClientLib/s_NetGlobal.h"

namespace GLOGIC
{
	extern BOOL			bGLOGIC_ZIPFILE;
	extern std::string	strGLOGIC_ZIPFILE;
	extern std::string	strQUEST_ZIPFILE;
	extern std::string	strLEVEL_ZIPFILE;
	extern std::string	strNPCTALK_ZIPFILE;

	void SetFullPath( BOOL bPack );
};

enum GLCONST
{
	MAX_SERVERCHAR		= MAX_ONESERVERCHAR_NUM,
	MAX_SELECTCHAR		= 4, // ĳ���� ���� ȭ�鿡�� ���̴� �ִ� ĳ������ ��, ���� 4��
	MAX_VIEWRANGE		= 300, // �ִ� ���� (��Ƽ�� ���� �����Ѱ�?...)

	MAX_HEAD			= 50,
	MAX_HAIR			= 50,

	MAX_SCHOOL			= 10,
	MAX_LEVEL			= 999,

	MAX_CLUBSTORAGE		= 5,
	MAX_CLUBRANK		= 10,

	GRADE_NORMAL		= 4,
	GRADE_HIGH			= 11,
	GRADE_LIMIT_MAX		= 12,
};

//	Note : ĳ���� ������ �⺻ ���.
//
struct GLCONST_CHARCLASS
{
	float				fWALKVELO;		//	ĳ���� �ȱ� �ӵ�.
	float				fRUNVELO;		//	ĳ���� �ٱ� �ӵ�.

	float				fHP_STR;		//	str(ü��) -> HP ȯ�� ����.
	float				fMP_SPI;		//	spi(����) -> MP ȯ�� ����.
	float				fSP_STA;		//	sta(�ٷ�) -> SP ȯ�� ����.
	float				fHIT_DEX;		//	dex(��ø) -> HIT rate ȯ�� ����.
	float				fHIT_POW;
	float				fAVOID_DEX;		//	dex(��ø) -> AVOID rate ȯ�� ����.
	float				fAVOID_POW;
	float				fDEFENSE_DEX;	//	dex(��ø) -> DEFENSE ȯ�� ����.
	float				fDEFENSE_POW;
	float				fDEFENSE_SPI;

	float				fPA_POW;		//	pow(��) -> PA(����ġ) ȯ�� ����.
	float				fPA_DEX;		//	dex(��ø) -> PA(����ġ) ȯ�� ����.

	float				fSA_POW;		//	pow(��) -> SA(���ġ) ȯ�� ����.
	float				fSA_DEX;		//	dex(��ø) -> SA(���ġ) ȯ�� ����.

	float				fMA_DEX;		//	dex(��ø) -> MA(����ġ) ȯ�� ����.
	float				fMA_SPI;		//	spi(����) -> MA(����ġ) ȯ�� ����.
	float				fMA_INT;		//	int(����) -> MA(����ġ) ȯ�� ����.

	float				fCONV_AP;		//	ȯ�� �⺻ ���ݷ�.
	float				fCONV_DP;		//	ȯ�� �⺻ ����.
	float				fCONV_PA;		//	ȯ�� �⺻ ����ġ.
	float				fCONV_SA;		//	ȯ�� �⺻ ���ġ.

	SCHARSTATS			sBEGIN_STATS;	//	STATS �ʱ� ��ġ.
	FCHARSTATS			sLVLUP_STATS;	//	������ STATS �⺻ ���� ��ġ.

	WORD				wBEGIN_AP;		//	�ʱ� ���ݷ�.
	WORD				wBEGIN_DP;		//	�ʱ� ����.
	WORD				wBEGIN_PA;		//	�ʱ� ����ġ.
	WORD				wBEGIN_SA;		//	�ʱ� ���ġ.

	float				fLVLUP_AP;		//	������ ���ݷ� ����.
	float				fLVLUP_DP;		//	������ ���� ����.
	float				fLVLUP_PA;		//	������ ����ġ ����.
	float				fLVLUP_SA;		//	������ ���ġ ����.

	//	���ϸ��̼� ����. ( �ð�/���� )
	//	server ������ �ʿ��� ����.
	VECANIATTACK		m_ANIMATION[AN_TYPE_SIZE][AN_SUB_00_SIZE];

	DWORD				dwHEADNUM;
	DWORD				dwHEADNUM_SELECT;
	std::string			strHEAD_CPS[MAX_HEAD];

	DWORD				dwHAIRNUM;
	DWORD				dwHAIRNUM_SELECT;
	std::string			strHAIR_CPS[MAX_HAIR];

	std::string			strCLASS_EFFECT;

	void LoadAniSet ( char* szSkinObj );
	void ClearAniSet ();

	BOOL LOADFILE ( const char* szFileName );

	GLCONST_CHARCLASS () :
		fWALKVELO(12),
		fRUNVELO(34),

		fHP_STR(0),
		fMP_SPI(0),
		fSP_STA(0),
//		fSP_POW(0),
		fHIT_DEX(0),
		fHIT_POW(0),
		fAVOID_DEX(0),
		fAVOID_POW(0),
		fPA_POW(0),
		fSA_DEX(0),
		
//		fASPEED_DEX(0),	//	Attack speed Dex
//		fMSPEED_AGI(0),	//	Movement speed Agi
		fMA_DEX(0),
		fMA_SPI(0),
		fMA_INT(0),

		fCONV_AP(0),
		fCONV_DP(0),
		fCONV_PA(0),
		fCONV_SA(0),

		wBEGIN_AP(0),
		wBEGIN_DP(0),
		wBEGIN_PA(0),
		wBEGIN_SA(0),

		fLVLUP_AP(0),
		fLVLUP_DP(0),
		fLVLUP_PA(0),
		fLVLUP_SA(0),

		dwHEADNUM(0),
		dwHEADNUM_SELECT(0),

		dwHAIRNUM(0),
		dwHAIRNUM_SELECT(0)
	{
	}
	
	GLCONST_CHARCLASS ( float _fHP_STR, float _fMP_SPI, float _fSP_STA, float _fHIT_DEX, float _fHIT_POW,
		float _fAVOID_DEX,float _fAVOID_POW, float _fDEFENSE_DEX, float _fDEFENSE_POW, float _fDEFENSE_SPI, float _fPA_POW, float _fSA_DEX,
		SCHARSTATS _fBEGIN_STATS, FCHARSTATS _fLVLUP_STATS,
		WORD _wBEGIN_AP, WORD _wBEGIN_DP, WORD _wBEGIN_PA, WORD _wBEGIN_SA,
		float _fLVLUP_AP, float _fLVLUP_DP, float _fLVLUP_PA, float _fLVLUP_SA ) :
		fWALKVELO(12),
		fRUNVELO(34),

		fHP_STR(_fHP_STR),
		fMP_SPI(_fMP_SPI),
		fSP_STA(_fSP_STA),
//		fSP_POW(_fSP_POW),
		fHIT_DEX(_fHIT_DEX),
		fHIT_POW(_fHIT_POW),
		fAVOID_DEX(_fAVOID_DEX),
		fAVOID_POW(_fAVOID_POW),
		fDEFENSE_DEX(_fDEFENSE_DEX),
		fDEFENSE_POW(_fDEFENSE_POW),
		fDEFENSE_SPI(_fDEFENSE_SPI),
		fPA_POW(_fPA_POW),
		fSA_DEX(_fSA_DEX),
		
//		fASPEED_DEX(_fASPEED_DEX),
//		fMSPEED_AGI(_fASPEED_DEX),

		sBEGIN_STATS(_fBEGIN_STATS),
		sLVLUP_STATS(_fLVLUP_STATS),
		wBEGIN_AP(_wBEGIN_AP),
		wBEGIN_DP(_wBEGIN_DP),
		wBEGIN_PA(_wBEGIN_PA),
		wBEGIN_SA(_wBEGIN_SA),

		fLVLUP_AP(_fLVLUP_AP),
		fLVLUP_DP(_fLVLUP_DP),
		fLVLUP_PA(_fLVLUP_PA),
		fLVLUP_SA(_fLVLUP_SA)
	{
	}

	~GLCONST_CHARCLASS()
	{
	}

private:
	//	���� ������ ���Ͼ�� ��õ������ ��������.
	GLCONST_CHARCLASS& operator= ( GLCONST_CHARCLASS &Input )	{ GASSERT(0); };	
};

struct GLCLUBRANK
{
	DWORD	m_dwMasterLvl;
	DWORD	m_dwLivingPoint;
	DWORD	m_dwPay;
	DWORD	m_dwMember;

	GLCLUBRANK () :
		m_dwMasterLvl(0),
		m_dwLivingPoint(0),
		m_dwPay(0),
		m_dwMember(0)
	{
	}

	GLCLUBRANK ( DWORD dwMLvl, DWORD dwLP, DWORD dwPY, DWORD dwMem ) :
		m_dwMasterLvl(dwMLvl),
		m_dwLivingPoint(dwLP),
		m_dwPay(dwPY),
		m_dwMember(dwMem)
	{
	}
};
struct GLCOMPOUND_RECIPE
{
	SNATIVEID			sNativeID;
	WORD				wNum;

	GLCOMPOUND_RECIPE()
		: sNativeID( NATIVEID_NULL() )
		, wNum(0)
	{
	}
};

struct GLCOMPOUND
{
	DWORD				dwID;
	
	GLCOMPOUND_RECIPE	glRecipe[5];
	
	SNATIVEID			sNativeID;
	WORD				wNum;

	float				fRate;
	LONGLONG			lnPrice;			

	GLCOMPOUND () 
		: dwID(0)
		, sNativeID( NATIVEID_NULL() )
		, wNum( 0 )
		, fRate( 0.0f )
		, lnPrice( 0 )
	{
	}
};
struct GLSHOP_TICKET
{
	SNATIVEID			sNativeID;
	WORD				wNum;

	GLSHOP_TICKET()
		: sNativeID( NATIVEID_NULL() )
		, wNum(0)
	{
	}
};

struct GLSHOP
{
	DWORD				dwID;
	
	GLSHOP_TICKET		glTicket[5];
	
	SNATIVEID			sNativeID;
	WORD				wNum;

	float				fRate;
	LONGLONG			lnPrice;			

	GLSHOP () 
		: dwID(0)
		, sNativeID( NATIVEID_NULL() )
		, wNum( 0 )
		, fRate( 0.0f )
		, lnPrice( 0 )
	{
	}
};
struct SPLAYERKILL
{
	DWORD		dwLEVEL;
	DWORD		dwNAME_COLOR;
	int			nPKPOINT;
	std::string	strNAME;
	float		fPK_EXP_RATE;

	DWORD		dwITEM_DROP_NUM;
	float		fITEM_DROP_RATE;
	
	float		fSHOP_2BUY_RATE;
	float		fSHOP_2SALE_RATE;

	SPLAYERKILL () :
		dwLEVEL(0),
		dwNAME_COLOR(0),
		nPKPOINT(0),
		fPK_EXP_RATE(0),

		dwITEM_DROP_NUM(0),
		fITEM_DROP_RATE(0),
		
		fSHOP_2BUY_RATE(0),
		fSHOP_2SALE_RATE(0)
	{
	}

	SPLAYERKILL ( DWORD _dwLEVEL, DWORD _dwNAME_COLOR, int _nPKPOINT, std::string _strNAME, float _fPK_EXP_RATE,
		DWORD _dwITEM_DROP_NUM, float _fITEM_DROP_RATE, float _fSHOP_2BUY_RATE, float _fSHOP_2SALE_RATE ) :
		dwLEVEL(_dwLEVEL),
		dwNAME_COLOR(_dwNAME_COLOR),
		nPKPOINT(_nPKPOINT),
		strNAME(_strNAME),
		fPK_EXP_RATE(_fPK_EXP_RATE),

		dwITEM_DROP_NUM(_dwITEM_DROP_NUM),
		fITEM_DROP_RATE(_fITEM_DROP_RATE),
		
		fSHOP_2BUY_RATE(_fSHOP_2BUY_RATE),
		fSHOP_2SALE_RATE(_fSHOP_2SALE_RATE)
	{
	}

	SPLAYERKILL ( const SPLAYERKILL &value )
	{
		operator= ( value );
	}

	SPLAYERKILL& operator= ( const SPLAYERKILL &value )
	{
		dwLEVEL = value.dwLEVEL;
		dwNAME_COLOR = value.dwNAME_COLOR;
		nPKPOINT = value.nPKPOINT;
		strNAME = value.strNAME;
		fPK_EXP_RATE = value.fPK_EXP_RATE;

		dwITEM_DROP_NUM = value.dwITEM_DROP_NUM;
		fITEM_DROP_RATE = value.fITEM_DROP_RATE;
		
		fSHOP_2BUY_RATE = value.fSHOP_2BUY_RATE;
		fSHOP_2SALE_RATE = value.fSHOP_2SALE_RATE;

		return *this;
	}
};

//	Note : ĳ���� ���� �⺻ ���.
//
namespace GLCONST_CHAR
{
	enum
	{
		DIE_DECEXP_NUM	= 30,
	};

	extern BOOL			bTESTSERVER;			// �׽�Ʈ �������� ����.
	extern INT			nUI_KEYBOARD;			// �������̽� Ű���� ����
	extern BOOL			bBATTLEROYAL;			// ��Ʋ�ξ�
	extern INT			nMAX_FRIEND_NUMBER;		// �ִ� ģ��, ���� ��� ����
	extern WORD			wLEVEL_FOR_EXTREME;		// �ذ��θ� �����ϱ� ���� �����Ǿ�� �ϴ� ����(lv.192)

	//	Note : �Ϲ� ���.
	//
	extern float		fFIELD_NEXTPICK;		//	�㿡 �ִ� ������(��) ������ ó�� �޽��� ������ ���� ���� �ð�.
	extern WORD			wSERVER_NORMAL;
	extern WORD			wSERVER_CONGEST;
	extern WORD			wSERVER_NOVACANCY;

	//---------------------------------------------------------------------------------------[�ɸ��� �⺻]
	extern WORD			wSCHOOLNUM;
	extern std::string	strSCHOOLNAME[MAX_SCHOOL];
	extern SNATIVEID	nidSTARTMAP[MAX_SCHOOL];
	extern DWORD		dwSTARTGATE[MAX_SCHOOL];
	//extern float		fCHAR_HEIGHT[GLCI_NUM_NEWSEX];
	//add class
	extern float		fCHAR_HEIGHT[GLCI_NUM_2015];
    //lv req for battlefieldUI tyranny - Eduj
	extern WORD          wTyrannyLevREQ;

	//---------------------------------------------------------------------------------------[]
	extern BOOL			bPARTY_2OTHERSCHOOL;
	extern BOOL			bCLUB_2OTHERSCHOOL;

//	extern float		fDISPRICE;				//	������ ���Ǹ� ����.
    extern WORD			wMAXITEM_AGE;			//	ITEM, �ִ� ���ӽð�.
	extern WORD			wMAXITEM_HOLD;			//	ITEM, �ӽ� ������ ��ȣ �ð�.
	extern WORD			wMAXMOB_TRACKING;		//	�ִ� �� ���� ��.

	extern WORD			wMAX_LEVEL;				//	���� �ִ� ����.
	extern WORD			wMAX_EXTREME_LEVEL;		//	�ذ��� ���� �ִ� ����.
	extern WORD			wLVL_STATS_P;			//	������ ���� ����Ʈ �ο���.
	extern WORD			wLVL_2ndSTATS_P;			//	������ ���� ����Ʈ �ο���.
	extern float		fLVL_EXP_G;				//	���� ����ġ ���⿡ �ʿ��� ȯ�� ����.
	extern float		fLVL_EXP_S;				//	���� ����ġ ���⿡ �ʿ��� ȯ�� ����.
	extern float		fKILL_EXP_RATE;			//	"��ȹ�� ���� ����ġ" * ���϶� ����ġ.


	extern float		fDIE_DECEXP[DIE_DECEXP_NUM];	//	����� ����ġ ���� ����.

	extern float		fDIE_RECOVERYEXP[DIE_DECEXP_NUM];	// ����ġ ���� ����
	extern float		fEXP_RATE_MONEY[DIE_DECEXP_NUM];	// ����ġ ���� ���

	extern float		fREC_EXP_RATE;			//	������ ȸ������ �ٶ� ȹ���ϴ� ����ġ�� ����.

	//---------------------------------------------------------------------------------------[����]

	extern WORD			wGRADE_MAX;				// ���� �Ҽ� �ִ� ��� ����ġ 
	extern WORD			wGRADE_MAX_REGI;		// ���� �Ҽ� �ִ� ��� ����ġ(����) 
	
	extern WORD			wDAMAGE_GRADE;			//	�� ��޴� ���� ���ݷ� ��ġ.
	extern WORD			wDEFENSE_GRADE;			//	�� ��޴� ���� ���� ��ġ.

	extern float		fDAMAGE_GRADE;			//	�� ��޴� ������ ���ݷ� ��ġ.
	extern float		fDEFENSE_GRADE;			//	�� ��޴� ������ ���� ��ġ.

	extern float		fSORTTIME;
	extern float		fDAMAGE_GRADE_TOP[GRADE_LIMIT_MAX-GRADE_HIGH];		// �ֻ��� ��� ���ݷ� ������
	extern float		fDEFENSE_GRADE_TOP[GRADE_LIMIT_MAX-GRADE_HIGH];	// �ֻ��� ��� ���� ������
	extern	WORD		wUSE_GRADE_NUM[GRADE_LIMIT_MAX-GRADE_HIGH];			// �ֻ��� ��� �ʿ��� ������ ����

	extern WORD			wRESIST_FIRE_GRADE;		//	�� ��޴� ���� ����(ȭ) ��ġ.
	extern WORD			wRESIST_ICE_GRADE;		//	�� ��޴� ���� ����(��) ��ġ.
	extern WORD			wRESIST_ELEC_GRADE;		//	�� ��޴� ���� ����(��) ��ġ.
	extern WORD			wRESIST_POISON_GRADE;	//	�� ��޴� ���� ����(��) ��ġ.
	extern WORD			wRESIST_SPIRIT_GRADE;	//	�� ��޴� ���� ����(��) ��ġ.

	//---------------------------------------------------------------------------------------
	extern WORD			wLVL_SKILL_P;			//	������ ��ų ����Ʈ �ο���.
	extern WORD			wLVL_2ndSKILL_P;			//	������ ��ų ����Ʈ �ο���.

	extern WORD			wMAXATRANGE_SHORT;		//	���� ���ݰŸ�.
	extern float		fPUSHPULL_VELO;			//	�аų� ��涧 �̵� �ӵ�.

	extern float		fMOB_TRACING;			//	�� ���� �Ÿ�.
	extern float		fLOW_SEED_DAMAGE;		//	damage ���� ���� ���� damage.
	extern float		fCONFT_SPTY_EXP;		//	�б� ��� �¸��ÿ� ��ü ����ġ �й�.

	extern WORD			wBODYRADIUS;			//	ĳ���� ��ü �ݰ�.

	extern float		fREACT_VALID_SCALE;		//	�׼��� ��ȿ �Ÿ� ����.

	extern float		fUNIT_TIME;				//	ȸ�� ���� �ð�.
	extern float		fHP_INC_PER;			//	�����ð��� HP ȸ����(%)
	extern float		fMP_INC_PER;			//	�����ð��� MP ȸ����(%)
	extern float		fSP_INC_PER;			//	�����ð��� SP ȸ����(%)

	extern float		fHP_INC;				//	�����ð��� HP ȸ����.
	extern float		fMP_INC;				//	�����ð��� MP ȸ����.
	extern float		fSP_INC;				//	�����ð��� SP ȸ����.

	extern float		fRESIST_PHYSIC_G;		//	���� ���ݽ� �Ӽ� ����ġ ���� ����.
	extern float		fRESIST_G;				//	�Ӽ� ����ġ ���� ����.
	extern WORD			wBASIC_DIS_SP;			//	�⺻ �Ҹ� SP.

	extern float		fLOWSP_MOTION;			//	���¹̳� (%) ���� ������ ��� ó��.
	extern float		fLOWSP_DAMAGE;			//	���¹̳� ������ Damage ����.
	extern float		fLOWSP_HIT_DROP;		//	���¹̳� ������ ������ ����.
	extern float		fLOWSP_AVOID_DROP;		//	���¹̳� ������ ȸ���� ����.
	extern DWORD		dwACCEPT_LOWERSTATS;	//	stats �������� ���¹̳��� Ŀ���� �� �ִ� ���� �ѵ�. ( ���� ���⿡ ����. )

	extern float		fDAMAGE_DEC_RATE;		//	����� ������.
	extern float		fDAMAGE_GRADE_K;		//	(�⺻����+�������) ����� �ݿ���.

	extern DWORD		dwCRITICAL_DAMAGE;		//	ũ��Ƽ�� ������ ����
	extern DWORD		dwCRITICAL_MAX;			//	ũ��Ƽ�� Ȯ�� MAX
	extern DWORD		dwCRUSHING_BLOW_DAMAGE;	//  ����Ÿ�� ������ ����
	extern DWORD		dwCRUSHING_BLOW_MAX;	//  ����Ÿ�� Ȯ�� max
	extern float		fCRUSH_BLOW_RANGE;		//  ����Ÿ�ݽ� �з����� �Ÿ� ( ������ ) 
		

	//	Note : ----------------------------------------------------------------���
	extern float		fCONFRONT_TIME;			//	��� ���� �ð�.
	extern float		fCONFRONT_ELAP;			//	���� ��� ���� ���� �ð�.
	extern DWORD		dwCONFRONT_MAX;			//	���� ��� �� ���� Ƚ��.
	extern float		fCONFRONT_RESET;		//	���� ��� ���� ���� �ð�.
	extern float		fCONFRONT_ONE_DIST;		//	��� �Ÿ� ����. ( 0�� ��� ���Ѵ�. )
	extern float		fCONFRONT_PY_DIST;		//	��� �Ÿ� ����. ( 0�� ��� ���Ѵ�. )
	extern float		fCONFRONT_CLB_DIST;		//	��� �Ÿ� ����. ( 0�� ��� ���Ѵ�. )

	extern int			nCONFRONT_WIN_LP;		//	��� �¸��� living point ( ��Ȱ ���� ).
	extern int			nCONFRONT_LOSS_LP;		//	��� �й�� living point ( ��Ȱ ���� ).

	extern int			nCONFRONT_CLB_WIN_LP;	//	Ŭ�� ��� �¸��� living point
	extern int			nCONFRONT_CLB_LOSS_LP;	//	Ŭ�� ��� �й�� living point

	//	Note : ----------------------------------------------------------------���
	enum { EMCONFT_RC_TYPENUM = 10, EMCONFT_STATE_TYPENUM = 8 };
	extern WORD			wCONFT_RC_TYPE[EMCONFT_RC_TYPENUM];			//	��� ȸ���� ��밡�� Ƚ��.
	extern float		fCONFT_STATE_TYPE[EMCONFT_STATE_TYPENUM];	//	��� ü�� Ȯ�� ����.

	extern WORD			wCONFRONT_SCHOOL_LIMIT_NUM;					//	�б��� ��� �ּ� �ο�.
	extern WORD			wCONFRONT_SCHOOL_LIMIT_LEVEL;				//	�б��� ��� �ּ� ����.
	extern float		fCONFRONT_SCHOOL_EXP_SCALE;					//	�б��� ��ý� ���� ����ġ ������.

	extern WORD			wCONFRONT_SCHOOL_WIN_NUM;
	extern WORD			wCONFRONT_SCHOOL_WIN_RATE;
	extern float		fCONFRONT_SCHOOL_GENITEM;

	//	Note : ----------------------------------------------------------------pk
	enum { EMPK_STATE_LEVEL = 5, };
	extern BOOL			bPK_MODE;				//	pk on/off.
	extern BOOL			bPKLESS;				//  �������� PK on/off.
	extern int			nPK_LIMIT_LEVEL;		//	pk ���� ����.
	extern int			nPK_LIMIT_DX_LEVEL;		//	pk ������ ���� ����.
	extern int			nPK_TRY_BRIGHT_POINT;	//	pk �õ��� ����Ʈ.
	extern int			nPK_KILL_BRIGHT_POINT;	//	pk �׿��� ��� ����Ʈ.
	extern int			nPK_TRY_LIVING_POINT;	//	pk �õ��� ����Ʈ.
	extern int			nPK_KILL_LIVING_POINT;	//	pk �׿��� ��� ����Ʈ.
	extern float		fNONPK_ITEM_DROP;		//	non pk �� ������ �����.
	extern float		fPK_ITEM_DROP;			//	pk �� ������ �����.

	//	 ( pk level : 0 ~ 4 )
	//	0 - �Ϲ��л�
	//	1 - �ҷ��л�
	//	2 - ������
	//	3 - ���θ�
	//	4 - ���α�
	extern DWORD		dwPK_RECALL_ENABLE_LEVEL;	//	��ȯ ��� ���� ���� ����.
	extern DWORD		dwPK_DRUG_ENABLE_LEVEL;		//	ȸ���� ��� ���� ���� ����.


	extern float		fPK_JUSTNESS_TIME;		//	�ݰ� ������ ������� �ð�.
	extern float		fPK_SAFE_TIME;			//	pk ���� �ð�.

	extern DWORD		dwPK_NORMAL_NAME_COLOR;	//	pk ��ġ�� ������ �̸� ����.
	extern float		fPK_POINT_DEC_RATE;		//	pk ��ġ ������. ( �ð���. )
	extern float		fPK_POINT_DEC_PHY;		//  pk ������ ������ ( ���� ).
	extern float		fPK_POINT_DEC_MAGIC;	//  pk ������ ������ ( ���� ).
	
	extern SPLAYERKILL	sPK_STATE[EMPK_STATE_LEVEL];

	//	Note : ----------------------------------------------------------------�����̻�
	enum { EMSTATEBLOW_LEVEL_SIZE = 10, EMSTATEBLOW_LEVEL_BASE = 1, };
	extern int			nSTATEBLOW_LEVEL[EMSTATEBLOW_LEVEL_SIZE];

	//	Note : ----------------------------------------------------------------����ġ Table
	//
	enum { EXPTABLE_SIZE = 52, EXPTABLE_LVL = 19, EXPBASEINDEX = 10, EXP_LVL_STEP = 5,
	EXPTABLE_RANGE = 61,  EXPTABLE_RANGE_BASE = 30 };
	extern WORD			wEXPTABLE[EXPTABLE_SIZE][EXPTABLE_LVL];
	extern float		fEXP_RATE_TABLE[EXPTABLE_RANGE];

	extern LONGLONG		lnEXP_MAX_TABLE[MAX_LEVEL];
	extern LONGLONG		lnEXP_MAX_TABLE_2nd[MAX_LEVEL];
	extern DWORD		dwEXP_PC_TABLE[MAX_LEVEL];

	extern LONGLONG		lnSKILL_PREREQ[EMSKILLCLASS_NSIZE][GLSkillMan::MAX_CLASSSKILL];
	extern std::string	strTRANSFORM_SKIN[EMSKILLCLASS_NSIZE][GLSkillMan::MAX_CLASSSKILL];
	extern SNATIVEID	sSkillID_Transform[EMSKILLCLASS_NSIZE][GLSkillMan::MAX_CLASSSKILL];
	extern BOOL			bSTUN[EMSKILLCLASS_NSIZE][GLSkillMan::MAX_CLASSSKILL];
	extern BOOL			bDANCE[EMSKILLCLASS_NSIZE][GLSkillMan::MAX_CLASSSKILL];
	extern BOOL			bHIT[EMSKILLCLASS_NSIZE][GLSkillMan::MAX_CLASSSKILL];
	//	Note : ----------------------------------------------------------------��Ƽ ����ġ.
	extern float		fPARTYEXPRANGE;				//	����ġ ������ ������ �ִ� �Ÿ�.
	extern float		fPARTYEXP_S;				//	��Ƽ ����ġ �й�� �������̿� ���� ���Һ���.
	extern WORD			aPARTYEXP_ONE[MAXPARTY];
	extern WORD			aPARTYKILLEXP_ONE[MAXPARTY];

	//	Note : ----------------------------------------------------------------Ŭ��
	extern DWORD		dwCLUB_PARTYNUM;			//	Ŭ�� â���� �ּ� ��Ƽ(�ʱ�) �ο�.
	extern DWORD		dwCLUB_DISSOLUTION_DAY;		//	Ŭ�� ��ü ���� ��.
	extern DWORD		dwCLUB_JOINBLOCK_DAY;		//	Ŭ�� Ż��� ���� ���� ��.
	extern DWORD		dwMAX_CLUBRANK;
	extern GLCLUBRANK	sCLUBRANK[MAX_CLUBRANK];	//	Ŭ�� ��ũ�� ���.
	extern DWORD		dwCLUB_AUTHORITY;			//	Ŭ�������� ���� ���� �ð�			
	extern DWORD		dwCLUB_ALLIANCE_SEC;		//	Ŭ�� ���� Ż��, ����� �簡�� ���ѽð�.
	extern DWORD		dwCLUB_ALLIANCE_DIS;		//	Ŭ�� ���� ��ü�� �Ἲ ���ѽð�.
	extern DWORD		dwCLUB_ALLIANCE_NUM;		//	Ŭ�� ���� �ִ� ����.

	extern DWORD		bCLUB_BATTLE;				//	Ŭ�� ��Ʋ ���� ����
	extern DWORD		dwCLUB_BATTLE_MAX;			//  Ŭ�� ��Ʋ ���� ���� ����
	extern DWORD		dwCLUB_BATTLE_RANK;			//  Ŭ�� ��Ʋ ��ũ ����
	extern DWORD		dwCLUB_BATTLE_TIME;			//  Ŭ�� ��Ʋ �ð�(��)
	extern DWORD		dwCLUB_BATTLE_TIMEMIN;		//	Ŭ�� ��Ʋ �ּҽð�(��)
	extern DWORD		dwCLUB_BATTLE_TIMEMAX;		//	Ŭ�� ��Ʋ �ִ�ð�(��)
	extern DWORD		dwCLUB_BATTLE_GUID_TIME;	//  Ŭ�� ��Ʋ ������ ���� �ð�(��)
	extern DWORD		dwCLUB_BATTLE_DIS_TIME;		//  Ŭ�� ��Ʋ ���� �� �׺� ������ �ð�(��)
	extern BOOL			bCLUB_BATTLE_ALLIANCE;		//  Ŭ�� ��Ʋ�� ���� �ڵ� ���� ���

	extern BOOL			bCLUB_DEATHMATCH;			//	Ŭ�� ������ġ ���� ����
	extern DWORD		dwCLUB_DEATHMATCH_MEM;		//	Ŭ�� ������ġ ���� �����
	

	//	Note : ----------------------------------------------------------------������
	extern float		fMAX_COMMISSION;			//	�ִ� ���� ���� ������ ����.
	extern float		fDEFAULT_COMMISSION;		//	�� ���� ������ ������.
	extern float		fEARNING_RATE;				//	������ ������.
	extern float		fEARNING_RATE_NPC;			//	NPC ��ȯ ������ ������.
	extern float		fCDCERTIFY_DIST;			//	���� ���� �Ÿ�.
	extern float		fCDCERTIFY_DIST2;			//	���� ���� �Ÿ�.
	extern float		fCDCERTIFY_TIME;			//	���� �ҿ� �ð�.

	//	Note : ----------------------------------------------------------------���� ����
	extern float		fEXP_SCALE;					//	Note : ����ġ ����.
	extern float		fITEM_DROP_SCALE;			//	Note : ������ �����.
	extern float		fMONEY_DROP_SCALE;			//	Note : �� �����.
	extern float		fITEM_DROP_LOWER_SCALE;		//	Note : ��� ������ �����.
	//	Note : ----------------------------------------------------------------

	//	Note : ----------------------------------------------------------------�п� ���� pk
	extern bool			bSCHOOL_FREE_PK_ALWAYS;		//	�п��� ���� pk �׻� ���� ����.
	extern bool			bSCHOOL_FREE_PK;			//	�п��� ���� pk on/off.
	extern bool			bSCHOOL_FREE_PK_TIME_REAL;	//	�п��� ���� pk ���� �ð� ( on : ���� �ð�, off : ���ӽð� )
	extern bool			bSCHOOL_FREE_PK_Z_FORCED;	//	�п��� ���� pk ���� ����.
	extern DWORD		dwSCHOOL_FREE_PK_TIME_START;//	�п��� ���� pk ���� �ð�.
	extern DWORD		dwSCHOOL_FREE_PK_TIME_TERM;	//	�п��� ���� pk ���� �ð�.
	extern float		fSCHOOL_FREE_PK_ITEM_DROP;	//	�п��� ���� pk ������ ����� ���갪. ( �⺻ ����� + ���갪. )
	//	Note : ----------------------------------------------------------------

	//	Note : ----------------------------------------------------------------��� ����.
	extern bool			bENCHANT_TERMINATE_ITEM;	//	��þƮ�� ������ �ļ� ����.
	extern bool			bMONEY_DROP2FIELD;			//	�� �ٴڿ� ������ ���� ����.

	//	Note : ----------------------------------------------------------------���� Ȯ��.
	extern float		fGRADE_RATE[GRADE_LIMIT_MAX];
	extern float		fGRADE_RESET_RATE[GRADE_LIMIT_MAX];
	extern float		fGRADE_TERMINATE_RATE[GRADE_LIMIT_MAX];

	extern WORD			wMOB_COLOR_1;
	extern WORD			wMOB_COLOR_2;
	extern WORD			wMOB_COLOR_3;
	extern WORD			wMOB_COLOR_4;
	extern WORD			wMOB_COLOR_5;
	extern WORD			wMOB_COLOR_6;
	extern WORD			wMOB_COLOR_7;
	extern WORD			wMOB_COLOR_8;

	extern int			iMAX_HIT;
	extern int			iMIN_HIT;
	extern int			iBASIC;

	extern BYTE			wGRADE_ELE_MAX;

	extern int			iRESIST_MAX;
	extern int			iBLOWRESIST_MAX;

	extern bool			bDUAL_WEAPON_BRAWLER;
	extern bool			bDUAL_WEAPON_SWORDSMAN;
	extern bool			bDUAL_WEAPON_ARCHER;
	extern bool			bDUAL_WEAPON_SHAMMAN;
	extern bool			bDUAL_WEAPON_SCIENCE;
	extern bool			bDUAL_WEAPON_ASSASIN;
	extern bool			bDUAL_WEAPON_TESTING;
	extern bool			bDUAL_WEAPON_EXTREME;

	extern bool			bISLONGRANGE_ARMS;

	extern DWORD		m_sHP_MAX;
	extern DWORD		m_sMP_MAX;
	extern DWORD		m_sSP_MAX;

	extern DWORD		m_sCP_MAX; // add cp
	extern float		fCP_RATE; // add cp

	extern float		llVEND_PRICE_MAX;

	extern bool			ENABLE_LOCKER_CONTROL;

	extern WORD			WMARKET_MID;
	extern WORD			WMARKET_SID;

	extern float		fBTGMaxEventExp;
	extern float		fBTGMaxEventItem;

	//add btg attack
	extern float		fBTGMaxEventAttack;

	extern float		fMaxEventExp;
	extern float		fMaxEventItem;
	extern float		fMaxEventMoney;

	extern WORD			wMaxEventSpeed;
	extern WORD			wMaxEventAspeed;
	extern WORD			wMaxEventAttack;

	extern float		SCALE_EVENT_EXP;
	extern float		SCALE_EVENT_ITEM;
	extern float		SCALE_EVENT_MONEY;

	extern bool			EnableGMLogs;

	//add pk
	extern DWORD		PkWinMinRb_A;
	extern DWORD		PkWinMinRb_B;
	extern DWORD		PkLossMinRb_A;
	extern DWORD		PkLossMinRb_B;

	//add lobby camera control
	extern float vFromPt_x;
	extern float vFromPt_y;
	extern float vFromPt_z;

	extern float vLookatPt_x;
	extern float vLookatPt_y;
	extern float vLookatPt_z;

	extern float vUpVec_x;
	extern float vUpVec_y;
	extern float vUpVec_z;

	//
	//jdev help
	extern CString		Link1;
	extern CString		Link2_A;	
	extern CString		Link2_B;
	extern CString		Link3;
	extern DWORD		Link2_A_Level;
	extern DWORD		Link2_B_Level;

	//add rank
	extern DWORD		dwRank_Point[20];
	extern std::string	strRank_Name[20];

	//add stats limit
	extern DWORD		dwPowMax;
	extern DWORD		dwStrMax;
	extern DWORD		dwSpiMax;
	extern DWORD		dwDexMax;
	extern DWORD		dwIntMax;
	extern DWORD		dwStaMax;

	//add duel bet
	extern LONGLONG		llMaxDuelBetMoney;
	extern LONGLONG		llMaxDuelBetVoteP;
	extern LONGLONG		llMaxDuelBetPremP;


	//add pkrank
	extern BOOL			bEnableRankName;
	extern BOOL			bEnableRankMark;

	extern WORD			wGMItemMax; //add itemcmd
	extern WORD			wGMItemPass; //add itemcmd

	extern BOOL			bCreateBrawler;
	extern BOOL			bCreateSwordsman;
	extern BOOL			bCreateArcher;
	extern BOOL			bCreateShamman;
	extern BOOL			bCreateExtreme;
	extern BOOL			bCreateGunner;
	extern BOOL			bCreateAssasin;
	extern BOOL			bCreateTESTING;
	//added by CNDev
	extern BOOL			bCreateShaper;

	extern DWORD		dwCHATCOLOR_NORMAL;
	extern DWORD		dwCHATCOLOR_PRIVATE;
	extern DWORD		dwCHATCOLOR_GUILD;
	extern DWORD		dwCHATCOLOR_PARTY;
	extern DWORD		dwCHATCOLOR_ALLIANCE;
	//	Note : ----------------------------------------------------------------����
	extern std::string	strGRINDING_SUCCEED;
	extern std::string	strGRINDING_FAIL;
	extern std::string	strGRINDING_RESET;
	extern std::string	strGRINDING_BROKEN;

	extern std::string	strGAMBLING_SHUFFLE;	// Monster7j
	extern std::string	strGAMBLING_WIN;		
	extern std::string	strGAMBLING_LOSE;	

	extern std::string	strITEMDROP_SUIT;
	extern std::string	strITEMDROP_WAPON;
	extern std::string	strITEMDROP_SHOES;
	extern std::string	strITEMDROP_RING;
	extern std::string	strITEMDROP_QBOX;

	extern std::string	strITEMDROP_SCROLL;
	extern std::string	strITEMDROP_COIN;
	extern std::string	strITEMDROP_DRUGS;

	extern std::string	strPICKUP_ITEM;
	extern std::string	strQITEM_FACT;
	extern std::string	strQITEM_BGM[QUESTION_SIZE];
	extern std::string	strPKSTREAK_BGM[PK_STREAK_SIZE];	//add pk streak

	//	Note : ----------------------------------------------------------------ȿ��
	extern std::string	strSELECT_CHAR;			//	�κ񿡼� �ɸ��� ���ý�.

	extern std::string	strREBIRTH_EFFECT;		//	ĳ�� ��Ȱ ����Ʈ.
	extern std::string	strLEVELUP_EFFECT;		//	���� �� ����Ʈ.
	
	extern std::string	strSRIKE_ELMT_EFFECT[EMELEMENT_MAXNUM]; //	Ÿ�ݽ� ����Ʈ.

	extern std::string	strAMBIENT_EFFECT;		//	Ÿ�ݽ� Ambient ����Ʈ
	extern std::string	strERASE_EFFECT;		//	�� ������� ����Ʈ.

	extern std::string	strSKILL_LEARN_EFFECT;	//	��ų �����.
	extern std::string	strSKILL_UP_EFFECT;		//	��ų ��.

	extern std::string strQUEST_START;			//	����Ʈ ����.
	extern std::string strQUEST_END_SUCCEED;	//	����Ʈ ����.
	extern std::string strQUEST_END_FAIL;		//	����Ʈ ����.

	extern std::string	strMOB_GEM_EFFECT;		//	�� ������ ����Ʈ.
	extern std::string	strMOB_DIE_BLOOD;		//	���� �׾�����.

	// PET
	extern std::string	strPET_GEN_EFFECT;		// �� ������ ����Ʈ

	// Vehicle
	extern std::string  strVEHICLE_GEN_EFFECT;	// ž�� ����Ʈ

	extern std::string	strCONFRONT_BOUND;		//	��� ��輱 ǥ�� ȿ��.

	extern std::string	strHALFALPHA_EFFECT;	//	���� �ɸ���.

	extern std::string	strBLOW_NUMB_EFFECT;
	extern std::string	strBLOW_STUN_EFFECT;
	extern std::string	strBLOW_STONE_EFFECT;
	extern std::string	strBLOW_BURN_EFFECT;
	extern std::string	strBLOW_FROZEN_EFFECT;

	extern std::string	strBLOW_MAD_EFFECT;
	extern std::string	strBLOW_POISON_EFFECT;
	extern std::string	strBLOW_CURSE_EFFECT;

	extern std::string	strBLOW_BODY_NUMB_EFFECT;
	extern std::string	strBLOW_BODY_STUN_EFFECT;
	extern std::string	strBLOW_BODY_STONE_EFFECT;
	extern std::string	strBLOW_BODY_BURN_EFFECT;
	extern std::string	strBLOW_BODY_FROZEN_EFFECT;

	extern std::string	strBLOW_BODY_MAD_EFFECT;
	extern std::string	strBLOW_BODY_POISON_EFFECT;
	extern std::string	strBLOW_BODY_CURSE_EFFECT;

	extern std::string	strACCOUNT_EFFECT[SW_SCHOOL_MAX];
	extern std::string	strACCOUNT_SUCCESS_EFFECT;

	//flagwars
	extern std::string strFLAGWAR_EFFECT[FW_SCHOOL_MAX];

	extern std::string strBLOW_EFFECTS[EMBLOW_SIZE];
	extern std::string strBLOW_BODY_EFFECTS[EMBLOW_SIZE];

	extern std::string strCRUSHING_BLOW_EFFECT;		// ����Ÿ�� ����Ʈ

	extern std::string strMINE_EFFECT;

	extern std::string	strCLICK_EFFECT;
	extern std::string	strTARGET_EFFECT;

	extern std::string	strTARGET_EFFECT_ENEMY;
	extern std::string	strTARGET_EFFECT_NPC;
	extern std::string	strTARGET_EFFECT_ALLIANCE;

	extern std::string	strARROW_EFFECT_ENEMY;
	extern std::string	strARROW_EFFECT_NPC;
	extern std::string	strARROW_EFFECT_ALLIANCE;

	extern std::string	strSUMMON_GEN_EFFECT;

	extern std::string	strQUEST_EFFECT;//add questeffect


	//extern GLCONST_CHARCLASS	cCONSTCLASS[GLCI_NUM_NEWSEX];	//	ĳ���� Ŭ������ ���.
	//add class
	extern GLCONST_CHARCLASS	cCONSTCLASS[GLCI_NUM_2015];

	//extern std::string strCLASS_INIT[MAX_SCHOOL][GLCI_NUM_NEWSEX];
	//add class
	extern std::string strCLASS_INIT[MAX_SCHOOL][GLCI_NUM_2015];

	extern std::vector<std::string>	vecGUIDANCE_FILE;
	extern std::vector<std::string>	vecClubDM_FILE;
	extern std::vector<std::string>	vecSW_FILE;
	extern std::vector<std::string>	vecRR_FILE;
	extern std::vector<std::string>	vecTW_FILE;

	extern std::vector<GLCOMPOUND>	vecCOMPOUND;
    BOOL LOADITEMMIX( char* szFileName );

	extern std::vector<GLSHOP>	vecSHOP;
    BOOL LOADPOINTSHOP( char* szFileName );

	extern std::string	strSCHOOLEFF[CROW_OWNER_SCHOOL];
	//
	//extern char					szCharSkin[GLCI_NUM_NEWSEX][MAX_PATH];	//	ĳ���� �ʱ� ����.
	//add class
	extern char					szCharSkin[GLCI_NUM_2015][MAX_PATH];

	BOOL	LOADFILE ( char* szFileName, BOOL bServer=TRUE );
	BOOL	EXP_LOADFILE ( char* szFileName );

	BOOL	EXP_PC_LOADFILE ( char* szFileName );
	BOOL	EXP_MAX_LOADFILE ( char* szFileName );
	BOOL	EXP_MAX_2nd_LOADFILE ( char* szFileName );

	//jdev help
	BOOL LOADLINK ( char* szFileName );
	BOOL LOADRANK ( char* szFileName ); //add pkrank
	BOOL LOAD_SKILLPREREQ ( char* szFileName ); //add Skill Prerequisites 
	BOOL LOAD_TRANSFORMSKIN ( char* szFileName ); //add Skill Transform Skin Set 

	BOOL VAID_CHAR_DATA2 ( WORD wSchool, EMCHARINDEX emIndex );
	SCHARDATA2& GET_CHAR_DATA2 ( WORD wSchool, EMCHARINDEX emIndex );

	inline const char* GETSTRIKE_EFFECT ( EMELEMENT emELEMENT=EMELEMENT_SPIRIT )
	{
		GASSERT(emELEMENT>=0);
		GASSERT(emELEMENT<EMELEMENT_MAXNUM);

		return strSRIKE_ELMT_EFFECT[emELEMENT].c_str();
	}

	const char* GETSCHOOLNAME ( WORD wSCHOOL );
	void SETEXP_SCALE ( float fScale );
	float GETEXP_SCALE ();

	void SETITEM_SCALE ( float fScale );
	float GETITEM_SCALE ();

	void SETMONEY_SCALE ( float fScale );
	float GETMONEY_SCALE ();

	DWORD GETEXP ( int nAttackerLev, int nDefenserLev );
	float GETEXP_RATE ( int nAttackerLev, int nDefenserLev );

	DWORD GETEXP_PC ( WORD wLev );
		//add afk reward
	extern float		fVoteTimer;		
	extern float		fPremTimer;		
	extern WORD			wVoteReward;
	extern WORD			wPremReward;
};

struct PETSTYLE
{
	std::string	strSTYLE_CPS[MAX_HAIR];
	WORD		wSTYLE_COLOR[MAX_HAIR];
	WORD		wSTYLENum;


	PETSTYLE () :
	wSTYLENum(0)
	{
		memset( wSTYLE_COLOR,0,sizeof(wSTYLE_COLOR));
	}
};

namespace GLCONST_PET
{
	extern PGLPET		pGLPET[PETTYPE_NEWSIZE];
	extern char			szPetInit[PETTYPE_NEWSIZE][MAX_PATH];
	extern PETSTYLE		sPETSTYLE[PETTYPE_NEWSIZE];
	extern BOOL			bCHANGEABLECOLOR[PETTYPE_NEWSIZE];

	extern float		fRunArea;
	extern float		fWalkArea;
	extern float		fOwnerDistance;
	extern int			nMAXVIEWRANGE;
	extern int			nFullDecrement[PETTYPE_NEWSIZE];
	extern float		fMaginotLineHP;
	extern float		fMaginotLineMP;
	extern float		fMaginotLineSP;

	PGLPET   GetPetData ( PETTYPE emTYPE );
	PETSTYLE GetPetStyle ( PETTYPE emTYPE );
	BOOL     IsChangeablePetColor ( PETTYPE emTYPE );
};

namespace GLCONST_VEHICLE
{
	extern PGLVEHICLE		pGLVEHICLE[VEHICLE_TYPE_SIZE];
	extern char				szVehicleInit[VEHICLE_TYPE_SIZE][MAX_PATH];

	extern int				nFullDecrVehicle[VEHICLE_TYPE_SIZE]; // ������ ����ġ
	
	PGLVEHICLE		GetVehicleData ( VEHICLE_TYPE emTYPE );

};

namespace GLCONST_SUMMON
{
	extern PGLSUMMON		pGLSUMMON[SUMMON_TYPE_SIZE];
	extern char				szSummonInit[SUMMON_TYPE_SIZE][MAX_PATH];

	PGLSUMMON GetSummonData ( SUMMON_TYPE emTYPE );


};

namespace GLCONST_ATTENDANCE
{
	const int MAX_REWARD = 10;

	struct	ATTEND_REWARD
	{
		int				nComboDay;
		SNATIVEID		idReward;

		ATTEND_REWARD()
			: nComboDay ( 0 )
			, idReward( NATIVEID_NULL() )
		{
		}
	};


	extern ATTEND_REWARD	sATEEND_REWARD[MAX_REWARD];
	extern DWORD			dwMAXREWARD;
	extern DWORD			dwAttendTime;

	BOOL LoadFile( std::string strFileName, bool bServer );
};

namespace MINIGAME_ODDEVEN // �̴� ����, Ȧ¦ ����
{
	const unsigned int MAX_ROUND = 5;

	extern float	fReturnRate[MAX_ROUND];		// ���� ���� �μ�(�������� = ����*1ȸ���μ�*2ȸ���μ�...)
	extern float	fSuccessRate[MAX_ROUND];	// ���� Ȱ��
	extern float	fTimeLimit;					// ���� �ð�(��)
	extern float	fShuffleTime;				// ���� �ð�(��)
	extern UINT		uiMaxBattingMoney;			// �ִ� ���� ���� �ݾ�
};

namespace HAIRCOLOR
{
	enum { MAXHAIRCOLOR = 8 };

	//extern WORD wHairColor[GLCI_NUM_NEWSEX][MAXHAIRCOLOR];
	//add class
	extern WORD wHairColor[GLCI_NUM_2015][MAXHAIRCOLOR];

	extern WORD GetHairColor ( WORD wClass, WORD wStyle ); 
};


class GLogicData
{
public:
	HRESULT LoadData ( BOOL bServer, bool bPastLoad = FALSE );
	HRESULT ClearData ();

	HRESULT InitDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice );
	HRESULT DeleteDeviceObjects ();

protected:
	GLogicData(void);

public:
	~GLogicData(void);

public:
	static GLogicData& GetInstance();
};
