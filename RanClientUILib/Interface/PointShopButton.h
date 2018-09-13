#pragma once

#include "../EngineUILib/GUInterface/UIGroup.h"
class	CBasicButton;

class CPointShop : public CUIGroup
{
public:
	CPointShop(void);
	~CPointShop(void);

public:
	enum
	{
		POINTSHOP_BUTTON	= NO_ID + 1,
	};

public:
	void	CreateSubControl ();


public:
	virtual	void	TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void	Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

private:
	CBasicButton*		m_pPointShopButton;
	LPDIRECT3DDEVICEQ	m_pd3dDevice;
	
};

