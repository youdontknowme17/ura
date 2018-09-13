#pragma	once

#include "../EngineUIlib/GUInterface/UIGroup.h"

class	CQuestionItemType : public CUIGroup
{
	enum
	{
		X1CONTROL = NO_ID,
		X2CONTROL,
		X4CONTROL,
		XCONTROL_SIZE = 3
	};

	bool		m_bPLAY;
	bool		m_bPLAY2;
	float		m_fPlayedTime;
	float		m_fBEGIN_TIME[XCONTROL_SIZE];
	float		m_fEND_TIME[XCONTROL_SIZE];
	CUIControl*	m_pXControl[XCONTROL_SIZE];

public:
	CQuestionItemType ();
	virtual	~CQuestionItemType ();

public:
	void	CreateSubControl ( CString strKeyword );

public:
	void	START();
	void	STOP()				{ m_bPLAY = false; }
	bool	ISPLAYING()			{ return m_bPLAY; }
	bool	TowerDownMSG()			{ return m_bPLAY2; }
	void	RESET();

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
};