#pragma	once

#include "../EngineUILib/GUInterface/UIGroup.h"

class	CBasicTextBox;

class CBasicPkWinDisplay : public CUIGroup
{
public:
	CBasicPkWinDisplay ();
	virtual	~CBasicPkWinDisplay ();

public:
	void	CreateSubControl ();

public:
	virtual void	Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

private:
	CBasicTextBox*	m_pPkWinText;
	DWORD m_dwPkWinBACK;
};