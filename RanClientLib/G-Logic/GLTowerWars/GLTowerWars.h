#pragma once

#include "GLTowerWarsDefine.h"

#include <vector>

#define SCHOOL_RANK_NUM		10

struct GLTWID
{
	DWORD	m_dwID;				
	DWORD	m_dwTWID;

	GLTWID () :
		m_dwID(UINT_MAX),
		m_dwTWID(GLSCHOOL_NUM)
	{
	}
};

typedef std::vector<GLTWID>	VECTWID_DB;

class GLTowerWars
{
public:
	enum 
	{		
		MAX_TIME = 99,
	};

	enum EM_BATTLE_STATE 
	{
		BATTLE_NOSTART		= 0,
		BATTLE_READY		= 1,
		BATTLE_START		= 2,	
		BATTLE_END_AWARD	= 3,	
		BATTLE_END			= 4,	
	};

	enum EM_FORCE_EXIT
	{
		FORCE_EXIT_NO = 0,
		FORCE_EXIT_YES = 1,
	};

public:
	DWORD				m_dwID;	
	std::string			m_strName;		
//	DWORD				m_dwTowerWarsMap;

	DWORD				m_dwClubMap;		
	DWORD				m_dwCLubMapGate;		

	DWORD				m_dwWinClub;	
	DWORD				m_dwBattleTimeNext;
	DWORD				m_dwBattleTimePrevious;
	DWORD				m_dwNextTime;
	DWORD				m_dwReviveGate[CTF_MAX_REVIVE_GATE];

	DWORD				m_dwBattleTime;
	TW_TIME				m_sTwTIME[MAX_TIME];	

	STWP_AWARD_ITEM		m_sTwPAwardItem;
	
	TWP_AWARD_CHAR		m_vecCtfAwardChar;
	TWP_RANK_INFO_MAP	m_mapCtfScore;
	TWP_RANK_INFO_MAP   m_mapTwWinner;

	SCHOOLWAR_TOWER_STATUS_MAP		m_mapTowerStatus;


	bool				m_bNotifyOneHour;
	bool				m_bNotifyHalfHour;
	bool				m_bNotify10MinHour;

	EM_BATTLE_STATE		m_emBattleState;
	EM_FORCE_EXIT		m_emForceExit;
	DWORD				m_dwBattleOrder;	
	DWORD				m_dwLastBattleHour;
	float				m_fBattleTimer;

	float				m_fCHECK_TIMER;			
	float				m_fCHECK_TIME;		

	float				m_fRankingUpdate;	

	WORD				Award_1st_Prem;
	WORD				Award_2nd_Prem;
	WORD				Award_3rd_Prem;
	WORD				Award_etc_Prem;

	bool				bBattleEnd;

public:
	GLTowerWars ();

	GLTowerWars ( const GLTowerWars &value )
	{
		operator= ( value );
	}

	GLTowerWars& operator= ( const GLTowerWars& value );

	bool operator < ( const GLTowerWars& sSchoolWAR )
	{
		return m_dwID < sSchoolWAR.m_dwID;
	}

public:
	bool Load ( std::string strFile );
	int	 m_nCTFWinner;
	void SetCTFWinner(int nSchool);
	void AddWinner( STWP_RANK_INFO sRANK );

public:
	void AddSWPScore( DWORD dwKillChar, DWORD dwKillGaeaID, DWORD dwDeathChar, DWORD dwDeathGaeaID );
	void AddSWPDamageScore( DWORD dwGaeaID, DWORD dwDamage );
	void AddSWPHealScore( DWORD dwGaeaID, DWORD dwDeathGaeaID, DWORD dwHeal );
	void AddSWPResuScore( DWORD dwGaeaID, DWORD dwDeathGaeaID );
	BOOL AddTower( DWORD dwCHARID, SNATIVEID sID, SW_SCHOOL_DATA emHolder );

	STWP_RANK_INFO* GetCTFRankInof( DWORD dwCharID );
	SSCHOOLWAR_TOWER_STATUS* GetTowerStatus( SNATIVEID sID );
	SW_SCHOOL_DATA GetTowerSchool( SNATIVEID sID );

	bool EndBattle( WORD wSchool, bool bEnd );

public:
	bool IsBattle () { return ( m_emBattleState == BATTLE_START ); }
	bool IsBattleReady() { return ( m_emBattleState == BATTLE_READY ); }
	bool IsBattleEndAward() { return ( m_emBattleState == BATTLE_END_AWARD); } //jdev test
	bool IsBattleEnd() { return ( m_emBattleState == BATTLE_END ); }
	bool IsForceExit() { return ( m_emForceExit == FORCE_EXIT_YES ); }
	bool IsEnterMap();
	DWORD IsBattleWeekDay ( int nDayOfWeek, int nHour );
	DWORD IsFirstStart ( int nDayOfWeek, int nHour );
	DWORD GetReviveGate( WORD wSCHOOL );

	void SetBattle( EM_BATTLE_STATE emBattleState )	{ m_emBattleState = emBattleState; }
	void SetForceExit ( EM_FORCE_EXIT emForceExit ) { m_emForceExit = emForceExit; }
	void SetBattleEnd ( bool bEnd )	{ bBattleEnd = bEnd; }
	
	bool IsBattleHour ( DWORD dwORDER, int nHour );
	bool IsBattleHourMin ( DWORD dwORDER, int nHour, int nMinute );

public:
	void UpdateBattleDay ( int nDay );
	void UpdateNotifyBattle ( DWORD dwORDER, int nHour, int nMinute );

	void CheckExtraGuild ( float fElaps );
	bool IsCheckExtraGuild() { return true; }

	void CheckAwardClub( float fElaps );
	void CheckExtraClub( float fElaps );

public:
	void DoBattleStart ( DWORD dwORDER, int nDay );
	void DoBattleEnd( bool bForceOut );

public:
	bool ChangeSWSchoolID ( DWORD dwSwID );
};

class GLTowerWarsMan
{
public:
	typedef std::vector<GLTowerWars>			SW_VEC;
	typedef SW_VEC::iterator						SW_VEC_ITER;
	typedef std::pair<SW_VEC_ITER,SW_VEC_ITER>	SW_VEC_PAIR;

protected:
	SW_VEC											m_vecSchoolWAR;

public:
	bool Load ( std::vector<std::string> &vecFiles );
	bool SetState ( VECTWID_DB &vecTWID );

public:
	GLTowerWars* Find ( DWORD dwID );
	bool IsBattle ();

public:
	GLTowerWarsMan ()
	{
	}

};

class GLTowerWarsAgentMan : public GLTowerWarsMan
{
protected:
	float m_fRemainTimer, m_fTimer, m_fTime;
	float m_fNextBattleTime;
	float m_fPreviousBattleTime;
	float m_fFirstStart;
	bool  m_bNextWar;
	bool  m_bFirstStart;
	bool  m_bBattleEnd;

public:
	bool ChangeSWSchoolID ( DWORD dwID, DWORD dwSwID );
	bool EndBattle ( DWORD dwID, WORD wSchool, bool bEnd );

public:
	bool SetMapState ();

public:
	DWORD GetRemainTime () { return (DWORD)m_fRemainTimer; }

public:
	bool FrameMove ( float fElapsedAppTime );

public:
	GLTowerWarsAgentMan ()
		: m_fRemainTimer(0)
		, m_fTimer(0)
		, m_fNextBattleTime( 0.0f )
		, m_fPreviousBattleTime( 0.0f )
		, m_fTime(0.0f)
		, m_fFirstStart( 0.0f )
		, m_bNextWar(false)
        , m_bBattleEnd(false)
		, m_bFirstStart(false)
	{
	}

public:
	static GLTowerWarsAgentMan& GetInstance();
};

class GLTowerWarsFieldMan : public GLTowerWarsMan
{
public:
	bool SetMapState ();

public:
	bool ReadyBattle( DWORD dwID );
	bool BeginBattle ( DWORD dwID );
	bool EndBattleFld ( DWORD dwID, DWORD dwSCHOOL, bool bForceExit );
	bool EndBattle ( DWORD dwID, WORD wSchool, bool bEnd );


public:
	bool ChangeSWSchoolID ( DWORD dwID, DWORD dwSwID );

public:
	const std::string GetName ( DWORD dwID );

	void UpdateTower( DWORD dwID );
	void UpdateCTFRanking( DWORD dwID, DWORD dwCharID );
	void SWAwardItem( DWORD dwID, DWORD dwWINNER );

	void CheckWinner( DWORD dwID );

protected:
	GLTowerWarsFieldMan ()
	{
	}

public:
	static GLTowerWarsFieldMan& GetInstance();
};