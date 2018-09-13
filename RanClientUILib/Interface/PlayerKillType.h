#pragma	once

#include "../EngineUIlib/GUInterface/UIGroup.h"

class	CPlayerKillStreakType : public CUIGroup
{
	enum
	{
		X1CONTROL = NO_ID,
		X2CONTROL,
		X4CONTROL,
		X2_1CONTROL,
		X1_1CONTROL,
		XCONTROL_SIZE = 5
	};

	bool		m_bPLAY;
	float		m_fPlayedTime;
	float		m_fBEGIN_TIME[XCONTROL_SIZE];
	float		m_fEND_TIME[XCONTROL_SIZE];
	CUIControl*	m_pXControl[XCONTROL_SIZE];

public:
	CPlayerKillStreakType ();
	virtual	~CPlayerKillStreakType ();

public:
	void	CreateSubControl ( CString strKeyword );

public:
	void	START();
	void	STOP()				{ m_bPLAY = false; }
	bool	ISPLAYING()			{ return m_bPLAY; }
	void	RESET();

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
};