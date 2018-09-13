#pragma once

#include <set>
#include "./GLLevelFile.h"
#include "../../EngineLib/DxOctree/DxLandGateMan.h"

class GLAGLandMan : public GLLevelFile
{
public:
	typedef std::set<DWORD>		SETPC;
	typedef SETPC::iterator		SETPC_ITER;

public:
	SETPC		m_setPC;
	DWORD		m_dwClubMapID;				//	���� �� CDM ID
	DWORD		m_dwSchoolWarMapID;
	DWORD		m_dwRoyalRumbleMapID;
	bool		m_bGuidBattleMap;			//	���� Ŭ�� ���� ��.
	bool		m_bClubDeathMatchMap;		//	Ŭ��������ġ ��
	bool		m_bSchoolWarsMap;
	bool		m_bRoyalRumbleMap;
	DWORD		m_dwTowerWarsMapID;
	bool		m_bTowerWarsMap;

public:
	DxLandGateMan* GetLandGateMan ()				{ return &m_cLandGateMan; }

public:
	bool DropPC ( DWORD dwID );
	bool DropOutPC ( DWORD dwID );

public:
	void SENDTOALLCLIENT ( LPVOID nmg );

protected:
	BOOL LoadWldFile ( const char* szWldFile, bool bLandGateLoad );

public:
	BOOL LoadFile ( const char *szFile );
	void CleanUp ();

public:
	GLAGLandMan(void);
	~GLAGLandMan(void);
};
