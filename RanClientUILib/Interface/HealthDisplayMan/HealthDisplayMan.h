#pragma	once

#include "../EngineUIlib/GUInterface/UIGroup.h"
#include "../RanClientLib/G-Logic/GLCrowClient.h"

const int	nMAX_HP_DISPLAY	= 1000;

class CBasicProgressBar;
class CHealthDisplayMan : public CUIGroup
{
public:
	CHealthDisplayMan ();
	virtual	~CHealthDisplayMan ();
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

	HRESULT InitDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice );

public:
	void	LoadHP(PGLCROWCLIENT pCrow);
	
private:
	LPDIRECT3DDEVICEQ	m_pd3dDevice;

	typedef	std::pair<PGLCROWCLIENT, CBasicProgressBar*>			HP_PAIR;
	typedef	std::list<HP_PAIR>										HP_LIST;
	typedef	HP_LIST::iterator										HP_LIST_ITER;

	HP_LIST															m_HPList;
	UIGUID															m_NEWID;
	void	UPDATE_HEALTH_POS(PGLCROWCLIENT pCrow, CBasicProgressBar* pHP);
};