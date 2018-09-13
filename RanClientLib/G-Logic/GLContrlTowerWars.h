#pragma once

#include "GLContrlBaseMsg.h"

//dmk14 | 1-11-17
enum EMCTF_RANKING_UPDATE_FB
{
	EMCTF_RANKING_UPDATE_FB_END,
	EMCTF_RANKING_UPDATE_FB_ADD,
	EMCTF_RANKING_UPDATE_FB_CLEAR,
};

namespace GLMSG
{
	#pragma pack(1)
	struct SNETPC_SERVER_NEXTWAR
	{
		NET_MSG_GENERIC						nmg;
		float					fProgressTime;
		float					fNextStartTime;
		float					fNextEndTime;
		float					fPreviousStartTime;
		float					fPreviousEndTime;

		SNETPC_SERVER_NEXTWAR ()
			: fProgressTime(0.0f),
		      fNextStartTime(0.0f),
			  fNextEndTime(0.0f),
			  fPreviousStartTime(0.0f),
			  fPreviousEndTime(0.0f)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_SERVER_NEXTWAR;
		}
	};

	struct SNET_SCHOOLWAR_NEXTWAR_REQ
	{
		NET_MSG_GENERIC						nmg;    
		float					fProgressTime;
		float					fNextStartTime;
		float					fNextEndTime;
		float					fPreviousStartTime;
		float					fPreviousEndTime;

		SNET_SCHOOLWAR_NEXTWAR_REQ () 
			: fProgressTime(0.0f),
		      fNextStartTime(0.0f),
			  fNextEndTime(0.0f),
			  fPreviousStartTime(0.0f),
			  fPreviousEndTime(0.0f)
		{
			nmg.dwSize = sizeof(SNET_SCHOOLWAR_NEXTWAR_REQ);
			nmg.nType = NET_MSG_GCTRL_SCHOOLWAR_NEXTWAR_REQ;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);			
		}       
	};

	struct SNET_SCHOOLWAR_NEXTWAR_BRD
	{
		NET_MSG_GENERIC			nmg;
		float					fProgressTime;
		float					fNextStartTime;
		float					fNextEndTime;
		float					fPreviousStartTime;
		float					fPreviousEndTime;

		SNET_SCHOOLWAR_NEXTWAR_BRD ()
			: fProgressTime(0.0f),
		      fNextStartTime(0.0f),
			  fNextEndTime(0.0f),
			  fPreviousStartTime(0.0f),
			  fPreviousEndTime(0.0f)
		{
			nmg.dwSize = sizeof ( SNET_SCHOOLWAR_NEXTWAR_BRD );
			nmg.nType  = NET_MSG_GCTRL_SCHOOLWAR_NEXTWAR_BRD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};
	
	struct SNET_TOWERWARS_START_BRD
	{
		enum { TEXT_LEN = 33, };

		NET_MSG_GENERIC			nmg;
		
		int						nTIME;
		char					szName[TEXT_LEN];

		SNET_TOWERWARS_START_BRD () 
			: nTIME(0)
		{
			nmg.dwSize = sizeof(SNET_TOWERWARS_START_BRD);
			nmg.nType = NET_MSG_GCTRL_TOWERWARS_START_BRD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			memset(szName, 0, sizeof(char) * TEXT_LEN);
		}
	};

	struct SNET_TOWERWARS_READY_FLD
	{
		NET_MSG_GENERIC			nmg;

		DWORD					dwID;

		SNET_TOWERWARS_READY_FLD () 
			: dwID(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_TOWERWARS_READY_FLD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_TOWERWARS_START_FLD
	{
		NET_MSG_GENERIC			nmg;

		DWORD					dwID;

		SNET_TOWERWARS_START_FLD () 
			: dwID(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_TOWERWARS_START_FLD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_TOWERWARS_END_FLD
	{
		NET_MSG_GENERIC			nmg;
		DWORD					dwID;
		DWORD                   dwSCHOOL;
		bool					bForceExit;					

		SNET_TOWERWARS_END_FLD () 
			: dwID(0)
			, dwSCHOOL(MAX_SCHOOL)
			, bForceExit(false)
		{
			nmg.dwSize = sizeof(SNET_TOWERWARS_END_FLD);
			nmg.nType = NET_MSG_GCTRL_TOWERWARS_END_FLD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_TOWERWARS_END_BRD
	{
		enum { TEXT_LEN = 33, };

		NET_MSG_GENERIC			nmg;
		char					szName[TEXT_LEN];
		char					szClubName[CHAR_SZNAME];
		DWORD                   wSCHOOL;

		SNET_TOWERWARS_END_BRD ()
			: wSCHOOL(-1)
		{
			nmg.dwSize = sizeof(SNET_TOWERWARS_END_BRD);
			nmg.nType = NET_MSG_GCTRL_TOWERWARS_END_BRD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			memset(szName, 0, sizeof(char) * TEXT_LEN);
			memset(szClubName, 0, sizeof(char) * CHAR_SZNAME);
		}
	};

	struct SNET_TOWERWARS_REMAIN_BRD
	{
		NET_MSG_GENERIC			nmg;

		DWORD					dwTime;

		SNET_TOWERWARS_REMAIN_BRD ()
			: dwTime(0)
		{
			nmg.dwSize = sizeof ( SNET_TOWERWARS_REMAIN_BRD );
			nmg.nType  = NET_MSG_GCTRL_TOWERWARS_REMAIN_BRD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_TOWERWARS_CTFPOINT_UPDATE
	{
		NET_MSG_GENERIC			nmg;
		
		DWORD					dwDamage;
		DWORD					dwHeal;
		bool					bKillPoint;
		bool					bDamagePoint;
		bool					bHealPoint;
		bool					bResuPoint;

		SNET_TOWERWARS_CTFPOINT_UPDATE () 
			: dwDamage(0)
			, dwHeal(0)
			, bKillPoint(false)
			, bDamagePoint(false)	
			, bHealPoint(false)
			, bResuPoint(false)
		{
			nmg.dwSize = sizeof(SNET_TOWERWARS_CTFPOINT_UPDATE);
			nmg.nType = NET_MSG_GCTRL_TOWERWARS_CTFPOINT_UPDATE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);			
		}        
	};

	struct SNET_TOWERWARS_MYCTFRANK_UPDATE
	{
		NET_MSG_GENERIC			nmg;
		
		STWP_RANK_EX			sMySwPRank;		

		SNET_TOWERWARS_MYCTFRANK_UPDATE () 
		{
			nmg.dwSize = sizeof(SNET_TOWERWARS_MYCTFRANK_UPDATE);
			nmg.nType = NET_MSG_GCTRL_TOWERWARS_MYCTFRANK_UPDATE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);			
		}        
	};

	//dmk14 | 1-11-17
	struct SNET_TOWERWARS_CTFRANKING_UPDATE
	{
		NET_MSG_GENERIC				nmg;

		EMCTF_RANKING_UPDATE_FB		emFB;
		STWP_RANK					sSwPRank;

		SNET_TOWERWARS_CTFRANKING_UPDATE ()
			: emFB(EMCTF_RANKING_UPDATE_FB_END)
		{
			nmg.dwSize = sizeof ( SNET_TOWERWARS_CTFRANKING_UPDATE );
			nmg.nType  = NET_MSG_GCTRL_TOWERWARS_CTFRANKING_UPDATE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_TOWERWARS_CTFRANKING_REQ
	{
		NET_MSG_GENERIC		nmg;    
		
		DWORD				dwMapID;		
		
		SNET_TOWERWARS_CTFRANKING_REQ () 
			: dwMapID(UINT_MAX)		
		{
			nmg.dwSize = sizeof(SNET_TOWERWARS_CTFRANKING_REQ);
			nmg.nType = NET_MSG_GCTRL_TOWERWARS_CTFRANKING_REQ;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);			
		}       

	};

	struct SNETPC_SERVER_TOWERWARS_INFO
	{
		NET_MSG_GENERIC		nmg;
		bool				bTowerWars;

		SNETPC_SERVER_TOWERWARS_INFO () :
			bTowerWars(false)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_SERVER_TOWERWARS_INFO;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_SERVER_TOWERWARS_REMAIN_AG
	{
		NET_MSG_GENERIC		nmg;
		DWORD				dwGaeaID;

		SNETPC_SERVER_TOWERWARS_REMAIN_AG ()
			: dwGaeaID(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_SERVER_TOWERWARS_REMAIN_AG;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_TOWERWARS_SETWINSCHOOL_BRD
	{
		NET_MSG_GENERIC			nmg;
		TOWERWARS_SCHOOL_WINNER sTwWinner;


		SNET_TOWERWARS_SETWINSCHOOL_BRD () 
			// wSchool(GLSCHOOL_NUM)
			//: bEnd(false)
		{
			nmg.dwSize = sizeof(SNET_TOWERWARS_SETWINSCHOOL_BRD);
			nmg.nType = NET_MSG_GCTRL_TOWERWARS_SETWINSCHOOL_BRD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};
	struct SNET_TOWERWARS_WINNER_REQ
	{
		NET_MSG_GENERIC						nmg;    
		DWORD								dwMapID;		
		int									nSchool;

		SNET_TOWERWARS_WINNER_REQ () 
			: dwMapID(UINT_MAX)
			, nSchool(-1)
		{
			nmg.dwSize = sizeof(SNET_TOWERWARS_WINNER_REQ);
			nmg.nType = NET_MSG_GCTRL_TOWERWARS_WINNER_REQ;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);			
		}       
	};
	struct SNETPC_TOWERWARS_SERVER_WINNER
	{
		NET_MSG_GENERIC						nmg;
		TOWERWARS_SCHOOL_WINNER		        sTwWinner;
		STWP_RANK                           sPlayerWinner[RANKING_TWNUM];
		WORD								wRankNum;

		SNETPC_TOWERWARS_SERVER_WINNER ()
			:wRankNum(0)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_TOWERWARS_SERVER_WINNER;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);	
		}
	};
	struct SNET_TOWERWARS_CERTIFIED_AGT
	{
		NET_MSG_GENERIC			nmg;
		DWORD					dwID;
		DWORD					dwSCHOOL;

		SNET_TOWERWARS_CERTIFIED_AGT () 
			: dwID(0)
			, dwSCHOOL(3)
		{
			nmg.dwSize = sizeof(SNET_TOWERWARS_CERTIFIED_AGT);
			nmg.nType = NET_MSG_GCTRL_TOWERWARS_CERTIFIED_AGT;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_TOWERWARS_DOBATTLEEND_AGT
	{
		NET_MSG_GENERIC			nmg;
		DWORD					dwID;
		DWORD					dwSCHOOL;
		bool					bEnd;

		SNET_TOWERWARS_DOBATTLEEND_AGT () 
			: dwID(0)
			, dwSCHOOL(3)
			, bEnd(false)
		{
			nmg.dwSize = sizeof(SNET_TOWERWARS_DOBATTLEEND_AGT);
			nmg.nType = NET_MSG_GCTRL_TOWERWARS_DOBATTLEEND_AGT;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_TOWERWARS_LASTCAPTURE_AGT
	{
		NET_MSG_GENERIC			nmg;
		DWORD					dwID;
		DWORD					dwSCHOOL;
		bool					bLastCapture;

		SNET_TOWERWARS_LASTCAPTURE_AGT () 
			: dwID(0)
			, dwSCHOOL(3)
			, bLastCapture(false)
		{
			nmg.dwSize = sizeof(SNET_TOWERWARS_LASTCAPTURE_AGT);
			nmg.nType = NET_MSG_GCTRL_TOWERWARS_LASTCAPTURE_AGT;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_TOWERWARS_DOBATTLEEND_FLD
	{
		NET_MSG_GENERIC			nmg;
		DWORD					dwID;
		DWORD					dwSCHOOL;
		bool					bEnd;

		SNET_TOWERWARS_DOBATTLEEND_FLD () 
			: dwID(0)
			, dwSCHOOL(3)
			, bEnd(false)
		{
			nmg.dwSize = sizeof(SNET_TOWERWARS_DOBATTLEEND_FLD);
			nmg.nType = NET_MSG_GCTRL_TOWERWARS_DOBATTLEEND_FLD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_TOWERWARS_LASTCAPTURE_FLD
	{
		NET_MSG_GENERIC			nmg;
		DWORD					dwID;
		DWORD					dwSCHOOL;
		bool					bLastCapture;

		SNET_TOWERWARS_LASTCAPTURE_FLD () 
			: dwID(0)
			, dwSCHOOL(3)
			, bLastCapture(false)
		{
			nmg.dwSize = sizeof(SNET_TOWERWARS_LASTCAPTURE_FLD);
			nmg.nType = NET_MSG_GCTRL_TOWERWARS_LASTCAPTURE_FLD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_SERVER_TOWERWARS_CAPTURE_AG
	{
		enum { TEXT_LEN = 33, };

		NET_MSG_GENERIC		nmg;
		DWORD		dwID;
		SNATIVEID	sID;
		char		szName[TEXT_LEN];
		char		szNameCrow[TEXT_LEN];
		SW_SCHOOL_DATA	emHOLDER;

		SNETPC_SERVER_TOWERWARS_CAPTURE_AG ()
			: dwID(0)
			, emHOLDER(SW_SCHOOL_MAX)
		{
			memset(szName, 0, sizeof(char) * TEXT_LEN);
			memset(szNameCrow, 0, sizeof(char) * TEXT_LEN);

			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_TO_AGENT_TOWERWARS_CAPTURE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_TOWERWARS_CAPTURE_BRD
	{
		enum { TEXT_LEN = 33, };

		NET_MSG_GENERIC			nmg;
		SNATIVEID	sID;
		char		szName[TEXT_LEN];
		char		szNameCrow[TEXT_LEN];
		SW_SCHOOL_DATA	emHOLDER;

		SNET_TOWERWARS_CAPTURE_BRD ()
			: emHOLDER(SW_SCHOOL_MAX)
		{
			memset(szName, 0, sizeof(char) * TEXT_LEN);
			memset(szNameCrow, 0, sizeof(char) * TEXT_LEN);

			nmg.dwSize = sizeof ( SNET_TOWERWARS_CAPTURE_BRD );
			nmg.nType  = NET_MSG_GCTRL_TOWERWARS_CAPTURE_BRD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_TOWERWARS_LASTCAPTURE_BRD
	{
		NET_MSG_GENERIC			nmg;
		DWORD					dwID;
		DWORD					dwSCHOOL;
		bool					bLastCapture;

		SNET_TOWERWARS_LASTCAPTURE_BRD ()
			: dwID(0)
			, dwSCHOOL(3)
			, bLastCapture(false)
		{
			nmg.dwSize = sizeof ( SNET_TOWERWARS_LASTCAPTURE_BRD );
			nmg.nType  = NET_MSG_GCTRL_TOWERWARS_LASTCAPTURE_BRD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNET_TOWERWARS_TOWER_UPDATE
	{
		NET_MSG_GENERIC		nmg;
		WORD				wNum;		
		SSCHOOLWAR_TOWER_STATUS	sTower[SW_TOWER_NUM];
	
		SNET_TOWERWARS_TOWER_UPDATE () 
			: wNum(0)
		{
			nmg.dwSize = sizeof(NET_MSG_GENERIC) + sizeof(WORD);
			nmg.nType = NET_MSG_GCTRL_TOWERWARS_TOWER_UPDATE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}

		bool ADDTOWER ( const SSCHOOLWAR_TOWER_STATUS& sTOWER )
		{
			if ( SW_TOWER_NUM==wNum )		return false;

			sTower[wNum] = sTOWER;

			++wNum;

			nmg.dwSize = sizeof(NET_MSG_GENERIC) + sizeof(WORD) + sizeof(SSCHOOLWAR_TOWER_STATUS)*wNum;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			return true;
		}

		void RESET ()
		{
			wNum = 0;
			nmg.dwSize = sizeof(NET_MSG_GENERIC) + sizeof(WORD);
		}
	};

	struct SNET_TOWERWARS_CTFICON_REQ
	{
		NET_MSG_GENERIC		nmg;    
		
		DWORD				dwMapID;		
		
		SNET_TOWERWARS_CTFICON_REQ () 
			: dwMapID(UINT_MAX)		
		{
			nmg.dwSize = sizeof(SNET_TOWERWARS_CTFICON_REQ);
			nmg.nType = NET_MSG_GCTRL_TOWERWARS_CTFICON_REQ;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);			
		}       

	};

	struct SNET_TOWERWARS_CERTIFIED_FLD
	{
		NET_MSG_GENERIC			nmg;
		DWORD					dwID;
		DWORD					dwSCHOOL;

		SNET_TOWERWARS_CERTIFIED_FLD () 
			: dwID(0)
			, dwSCHOOL(3)
		{
			nmg.dwSize = sizeof(SNET_TOWERWARS_CERTIFIED_FLD);
			nmg.nType = NET_MSG_GCTRL_TOWERWARS_CERTIFIED_FLD;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
	};

	struct SNETPC_UPDATE_HP_MULTIPLIER
	{
		NET_MSG_GENERIC		nmg;
		float				fMultiplier[3];

		SNETPC_UPDATE_HP_MULTIPLIER() 
		{
			for(int i=0;i<3;i++)	fMultiplier[i]=0.0f;

			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_UPDATE_HP_MULTIPLIER;
		}
	};
	// generate TowerWars school winner Eduj kun
	struct SNET_TOWER_WINNER_UPDATE
	{
		NET_MSG_GENERIC		nmg;
		WORD				wRankNum;		
		STWP_RANK	sRank[RANKING_TWNUM];

		SNET_TOWER_WINNER_UPDATE () 
			: wRankNum(0)
		{
			nmg.dwSize = sizeof(NET_MSG_GENERIC) + sizeof(WORD);
			nmg.nType = NET_MSG_GCTRL_TOWERWARS_WINNER_PLAYER_UPDATE;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);
		}
		bool ADDRANK ( const STWP_RANK& sRANK )
		{
			if ( RANKING_TWNUM==wRankNum )		return false;

			sRank[wRankNum] = sRANK;

			++wRankNum;

			nmg.dwSize = sizeof(NET_MSG_GENERIC) + sizeof(WORD) + sizeof(STWP_RANK)*wRankNum;
			GASSERT(nmg.dwSize<=NET_DATA_BUFSIZE);

			return true;
		}	
		void RESET ()
		{
			wRankNum = 0;
			nmg.dwSize = sizeof(NET_MSG_GENERIC) + sizeof(WORD);
		}
	};
	// Revert to default structure packing
	#pragma pack()
};