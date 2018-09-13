#pragma	once

#include "../EngineUIlib/GUInterface/UIGroup.h"

class	CBasicProgressBar;

class	CSimpleHP : public CUIGroup
{
public:
	CSimpleHP ();
	virtual	~CSimpleHP ();

public:
	void	SetHP ( DWORD wNOW, DWORD wMAX );
	void	CreateSubControl ();
	void	SetUseSummonHP( bool bUseSummonHP ) { m_bUseSummonHP = bUseSummonHP; }

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

private:
	CBasicProgressBar*	m_pHP;
	bool				m_bUseSummonHP;
};