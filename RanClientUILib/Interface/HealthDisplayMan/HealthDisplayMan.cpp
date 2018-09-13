#include "StdAfx.h"
#include "HealthDisplayMan.h"
#include "../EngineUILib/GUInterface/BasicProgressBar.h"
#include "DxViewPort.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CHealthDisplayMan::CHealthDisplayMan ()
	: m_NEWID( nMAX_HP_DISPLAY )
{
}

CHealthDisplayMan::~CHealthDisplayMan ()
{
}

void CHealthDisplayMan::Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !m_HPList.empty())
	{
		HP_LIST_ITER iter = m_HPList.begin ();
		HP_LIST_ITER iter_end = m_HPList.end ();
		for( ; iter != iter_end; ++iter )
		{
			if ( !m_NEWID ) break;

			PGLCROWCLIENT pCrow = iter->first;
			if ( !pCrow ) continue;

			CBasicProgressBar* pHP = iter->second;
			pHP->SetVisibleSingle(FALSE);
			UPDATE_HEALTH_POS(pCrow, pHP);
		}
	}
}

void CHealthDisplayMan::LoadHP(PGLCROWCLIENT pCrow)
{
	if( !pCrow ) return;

	if ( !m_HPList.empty())
	{
		HP_LIST_ITER iter = m_HPList.begin ();
		HP_LIST_ITER iter_end = m_HPList.end ();
		for( ; iter != iter_end; ++iter )
		{
			CBasicProgressBar* pHP = iter->second;
			pHP->SetVisibleSingle(FALSE);
			pHP->UnLoadTexture();
		}

		m_HPList.clear();
		DeleteControl( m_NEWID, 0 );
	}


	if ( pCrow )
	{
		m_NEWID++;
		CBasicProgressBar* pHP = new CBasicProgressBar;
		pHP->CreateSub( this, "HEALTH_DISPLAY", UI_FLAG_DEFAULT, m_NEWID );
		pHP->CreateOverImage ( "HEALTH_DISPLAY_OVERIMAGE" );
		pHP->InitDeviceObjects ( m_pd3dDevice );
		pHP->RestoreDeviceObjects ( m_pd3dDevice );
		pHP->SetVisibleSingle( FALSE );
		RegisterControl( pHP );

		HP_PAIR pair(pCrow, pHP);
		m_HPList.push_back(pair);
	}
}

void CHealthDisplayMan::UPDATE_HEALTH_POS(PGLCROWCLIENT pCrow, CBasicProgressBar* pHP)
{
	GASSERT( pHP );
	if ( !pCrow ) return;

	D3DXVECTOR3 vScreen;
	vScreen = DxViewPort::GetInstance().ComputeVec3Project ( &pCrow->GetPosition(), NULL );

	const UIRECT& rcOriginPos = pHP->GetGlobalPos ();

	D3DXVECTOR2 vPos;
	vPos.x = floor(vScreen.x - ( rcOriginPos.sizeX * 0.5f )); 
	vPos.y = floor(vScreen.y - rcOriginPos.sizeY);
	
	if( abs( rcOriginPos.left - vPos.x ) > 1 || abs( rcOriginPos.top - vPos.y ) > 1 )
		pHP->SetGlobalPos( vPos );

	float fPercent = float(pCrow->GetHp().dwNow) / float(pCrow->GetHp().dwMax);
	pHP->SetPercent(fPercent);
	pHP->SetVisibleSingle(TRUE);
}

HRESULT CHealthDisplayMan::InitDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	GASSERT( pd3dDevice );
	m_pd3dDevice = pd3dDevice;

	HRESULT hr = S_OK;
	hr = CUIGroup::InitDeviceObjects ( pd3dDevice );
	if ( FAILED ( hr ) ) return hr;

	return S_OK;
}