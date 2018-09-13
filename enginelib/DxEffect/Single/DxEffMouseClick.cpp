#include "stdafx.h"

#include "../DxEffect/Single/DxEffSingle.h"
#include "./DxEffSinglePropGMan.h"
#include "../Common/StlFunctions.h"
#include "../DxEffect/DxEffectMan.h"
#include "./DxEffectParticleSys.h"

#include "./DxEffcharData.h"
#include "./DxEffMouseClick.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DxEffMouseClick& DxEffMouseClick::GetInstance()
{
	static DxEffMouseClick Instance;
	return Instance;
}

DxEffMouseClick::DxEffMouseClick(void) :
	m_fTimer(0.0f),
	m_pd3dDevice(NULL),
	m_bCreate(false),
	m_pEffSingleG(NULL)
{
}

DxEffMouseClick::~DxEffMouseClick(void)
{
}

D3DXVECTOR3 DxEffMouseClick::GetTargetIDPos ( STARGETID *pTargetID )
{
	D3DXVECTOR3 vPos;
	
	vPos.x = 0.f;
	vPos.y = 10.f;
	vPos.z = -50.f;

	return vPos;
}

HRESULT DxEffMouseClick::PassiveEffect ( const char* szFileName, const D3DXMATRIX &matTrans, const STARGETID &sTargetID )
{
	if ( sTargetID.dwID==EMTARGET_NULL )		return E_FAIL;
	if ( !szFileName || szFileName[0]==NULL )	return E_FAIL;

	std::string strFileName = szFileName;
	std::transform ( strFileName.begin(), strFileName.end(), strFileName.begin(), tolower );

	BOOL bExist = FALSE;

	EFFSGMAP_PAIR pairESG = m_mapESG.equal_range ( sTargetID );
	EFFSGMAP_ITER iter = pairESG.first;
	for ( ; iter != pairESG.second; ++iter )
	{
		DxEffSingleGroup* pEffSingleGroup = (*iter).second;
		if ( pEffSingleGroup->m_strFileName==strFileName )
		{
			bExist = TRUE;
		}
	}

	if ( !bExist )
	{
		NewEffGroup ( strFileName.c_str(), matTrans, &sTargetID );
	}

	return S_OK;
}

HRESULT DxEffMouseClick::DeletePassiveEffect ( const STARGETID &sTargetID )
{
	if ( sTargetID.dwID==EMTARGET_NULL )	return E_FAIL;

	EFFSGMAP_PAIR pairESG = m_mapESG.equal_range ( sTargetID );
	EFFSGMAP_ITER iter = pairESG.first;
	for ( ; iter != pairESG.second; )
	{
		DxEffSingleGroup* pEffSingleGroup = (*iter).second;
		EFFSGMAP_ITER iter_del = iter;
		++iter;

		SAFE_DELETE((*iter_del).second);
		m_mapESG.erase(iter_del);
	}

	return S_OK;
}

HRESULT DxEffMouseClick::DeletePassiveEffect ( const char* szFileName, const STARGETID &sTargetID )
{
	if ( sTargetID.dwID==EMTARGET_NULL )	return E_FAIL;

	std::string strFileName = szFileName;
	std::transform ( strFileName.begin(), strFileName.end(), strFileName.begin(), tolower );

	EFFSGMAP_PAIR pairESG = m_mapESG.equal_range ( sTargetID );
	EFFSGMAP_ITER iter = pairESG.first;
	for ( ; iter != pairESG.second; )
	{
		DxEffSingleGroup* pEffSingleGroup = (*iter).second;
		if ( pEffSingleGroup->m_strFileName==strFileName )
		{
			EFFSGMAP_ITER iter_del = iter;
			++iter;

			SAFE_DELETE((*iter_del).second);
			m_mapESG.erase(iter_del);
		}
		else
		{
			++iter;
		}
	}

	return S_OK;
}

BOOL DxEffMouseClick::FindPassiveEffect ( const char* szFileName, const STARGETID &sTargetID )
{
	if ( sTargetID.dwID==EMTARGET_NULL )	return FALSE;

	std::string strFileName = szFileName;
	std::transform ( strFileName.begin(), strFileName.end(), strFileName.begin(), tolower );

	EFFSGMAP_PAIR pairESG = m_mapESG.equal_range ( sTargetID );
	EFFSGMAP_ITER iter = pairESG.first;
	for ( ; iter != pairESG.second; ++iter )
	{
		DxEffSingleGroup* pEffSingleGroup = (*iter).second;
		if ( pEffSingleGroup->m_strFileName==strFileName )	return TRUE;
	}

	return FALSE;
}

HRESULT DxEffMouseClick::NewEffGroup ( EFF_PROPGROUP* pPropGroup, EFF_PROPERTY* pTargetProp, const D3DXMATRIX &matTrans )
{
	//	Note : Eff Group instance ����.
	//
	GASSERT(pPropGroup);
	DxEffSingleGroup* pEffSingleGroup = pPropGroup->NEWEFFGROUP ( pTargetProp );
	if ( !pEffSingleGroup )	return E_FAIL;

	//	Note : Eff Group�� ��Ʈ���� Ʈ������ ����.
	//
	pEffSingleGroup->m_matWorld = matTrans;

	//	Note : Eff Group Create Dev.
	//
	pEffSingleGroup->Create ( m_pd3dDevice );

	//	Note : ����Ʈ�� �����.
	//
	m_mapESG.insert ( std::make_pair(STARGETID(),pEffSingleGroup) );

	return S_OK;
}

DxEffSingleGroup* DxEffMouseClick::NewEffGroup ( EFF_PROPGROUP* pPropGroup, const D3DXMATRIX &matTrans, const STARGETID* pTargetID )
{
	//	Note : Eff Group instance ����.
	//
	GASSERT(pPropGroup);
	DxEffSingleGroup* pEffSingleGroup = pPropGroup->NEWEFFGROUP ();
	if ( !pEffSingleGroup )	return NULL;

	//	Note : Ÿ������.
	//
	pEffSingleGroup->SetTargetID ( pTargetID );

	//	Note : Eff Group�� ��Ʈ���� Ʈ������ ����.
	//
	pEffSingleGroup->m_matWorld = matTrans;

	//	Note : Eff Group Create Dev.
	//
	pEffSingleGroup->Create ( m_pd3dDevice );

	//	Note : ����Ʈ�� �����.
	//
	STARGETID sTargetID;
	if ( pTargetID )	sTargetID = *pTargetID;
	m_mapESG.insert ( std::make_pair(sTargetID,pEffSingleGroup) );

	return pEffSingleGroup;
}

DxEffSingleGroup* DxEffMouseClick::NewEffGroup ( const char* szFileName, const D3DXMATRIX &matTrans, const STARGETID* pTargetID )
{
	GASSERT(m_pd3dDevice);

	EFF_PROPGROUP* pPropGroup;
	DxEffSingleGroup* pEffSingleG;

	if ( m_pEffSingleG )
	{
		m_pEffSingleG->SetTargetID( pTargetID );
		m_pEffSingleG->m_matWorld = matTrans;
	}

	if ( m_bCreate )
	{
	}else{
		//	Note : Prop Group �� ������.
		//
		pPropGroup = DxEffSinglePropGMan::GetInstance().LoadEffectGProp ( szFileName );
		if ( !pPropGroup )	return NULL;

		//	Note : Eff Group instance ����.
		//
		pEffSingleG = NewEffGroup ( pPropGroup, matTrans, pTargetID );

		m_pEffSingleG = pEffSingleG;
		m_bCreate = true;
	}

	return pEffSingleG;
}

HRESULT DxEffMouseClick::NewEffBody ( const char* szFileName, const STARGETID* pTargetID, D3DXVECTOR3 *pDir )
{
	GASSERT(pTargetID);

	DxSkinChar* pSkinChar = DxEffectMan::GetInstance().GetSkinChar ( *pTargetID );
	if ( !pSkinChar )	return E_FAIL;

	D3DXVECTOR3 vDIR_ORG(0.f,0.f,-1.f);
	float fdir_y = 0;
	if ( pDir )		fdir_y = DXGetThetaYFromDirection ( *pDir, vDIR_ORG );

	BOOL bOk = DxEffcharDataMan::GetInstance().PutEffect ( pSkinChar, szFileName, pDir, fdir_y );
	if ( !bOk )			return E_FAIL;

	return S_OK;
}

HRESULT DxEffMouseClick::InitDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	GASSERT(pd3dDevice);
	m_pd3dDevice = pd3dDevice;

	EFFSGMAP_ITER iter = m_mapESG.begin();
	EFFSGMAP_ITER iter_end = m_mapESG.end();
	for ( ; iter!=iter_end; ++iter )
	{
		(*iter).second->InitDeviceObjects ( pd3dDevice );
	}

	return S_OK;
}

HRESULT DxEffMouseClick::RestoreDeviceObjects ()
{
	EFFSGMAP_ITER iter = m_mapESG.begin();
	EFFSGMAP_ITER iter_end = m_mapESG.end();
	for ( ; iter!=iter_end; ++iter )
	{
		(*iter).second->RestoreDeviceObjects ( m_pd3dDevice );
	}

	return S_OK;
}

HRESULT DxEffMouseClick::InvalidateDeviceObjects ()
{
	EFFSGMAP_ITER iter = m_mapESG.begin();
	EFFSGMAP_ITER iter_end = m_mapESG.end();
	for ( ; iter!=iter_end; ++iter )
	{
		(*iter).second->InvalidateDeviceObjects ();
	}

	return S_OK;
}

HRESULT DxEffMouseClick::DeleteDeviceObjects ()
{
	RemoveAllEff ();

	return S_OK;
}

HRESULT DxEffMouseClick::RemoveAllEff ()
{
	m_pEffSingleG = NULL;
	m_bCreate = false;
	std::for_each ( m_mapESG.begin(), m_mapESG.end(), std_afunc::DeleteMapObject() );
	m_mapESG.clear();

	return S_OK;
}

HRESULT DxEffMouseClick::DeleteEff ( const char* szFile )
{
	if ( !szFile )						return S_FALSE;
	if ( m_mapESG.empty() )				return S_FALSE;

	std::string strFileName = szFile;
	std::transform ( strFileName.begin(), strFileName.end(), strFileName.begin(), tolower );

	EFFSGMAP_ITER iter = m_mapESG.begin();
	EFFSGMAP_ITER iter_end = m_mapESG.end();
	for ( ; iter!=iter_end; ++iter )
	{
		if ( (*iter).second->m_strFileName==strFileName )
		{
			SAFE_DELETE ( (*iter).second );
			m_mapESG.erase ( iter );
			return S_OK;
		}
	}

	return S_OK;
}

HRESULT DxEffMouseClick::DeleteAllEff ( const char* szFile )
{
	if ( !szFile )						return S_FALSE;
	if ( m_mapESG.empty() )				return S_FALSE;

	std::string strFileName = szFile;
	std::transform ( strFileName.begin(), strFileName.end(), strFileName.begin(), tolower );

	EFFSGMAP_ITER iter = m_mapESG.begin();
	EFFSGMAP_ITER iter_end = m_mapESG.end();
	for ( ; iter!=iter_end; )
	{
		EFFSGMAP_ITER iter_cur = iter++;
		if ( (*iter_cur).second->m_strFileName==strFileName )
		{
			SAFE_DELETE ( (*iter_cur).second );
			m_mapESG.erase ( iter_cur );
		}
	}

	return S_OK;
}

void DxEffMouseClick::NewAfterEff ( DxEffSingleGroup* pEffSG )
{
	if ( pEffSG->m_listEffSAfter.empty() )	return;

	STARGETID sTargetID = pEffSG->m_TargetID;
	D3DXVECTOR3 vTARPOS = DxEffectMan::GetInstance().GetCrowPos ( sTargetID );
	vTARPOS.y += 15.0f;

	float fTARDIR = DxEffectMan::GetInstance().GetCrowDir ( sTargetID );

	DxEffSingleGroup::AFTERLIST_ITER iter = pEffSG->m_listEffSAfter.begin();
	DxEffSingleGroup::AFTERLIST_ITER iter_end = pEffSG->m_listEffSAfter.end();
	for ( ; iter!=iter_end; ++iter )
	{
		CString strTemp = (*iter).m_strEffect.c_str();
		if ( !((*iter).m_strEffect.empty()) )
		{
			switch ( (*iter).m_emType )
			{
			case EFFABODY:
				{
					DxSkinChar* pSkinChar = DxEffectMan::GetInstance().GetSkinChar ( sTargetID );
					if ( !pSkinChar )	continue;

					D3DXVECTOR3 vDir = vTARPOS - pEffSG->m_vStartPos;
					D3DXVec3Normalize ( &vDir, &vDir );

					DxEffcharDataMan::GetInstance().PutEffect
					(
						pSkinChar,
						(*iter).m_strEffect.c_str(),
						&vDir,
						fTARDIR
					);
				}
				break;

			case EFFASINGLE:
				{
					//	������ �ʴ� Ÿ���� ��� Ÿ�� ����Ʈ�� ������.
					if ( !DxEffectMan::GetInstance().IsCrowVisible(sTargetID) )	break;

					//	Ÿ���� ��ġ.
					sTargetID.vPos = vTARPOS;

					D3DXMATRIX matEffect;
					D3DXMatrixTranslation ( &matEffect, vTARPOS.x, vTARPOS.y, vTARPOS.z );

					NewEffGroup
					(
						(*iter).m_strEffect.c_str(),
						matEffect,
						&sTargetID
					);
				}
				break;
			};
		}
	}

	pEffSG->m_listEffSAfter.clear();
}

HRESULT DxEffMouseClick::FrameMove ( float fTime, float fElapsedTime )
{
	m_fTimer += fElapsedTime;

	EFFSGMAP_ITER iter = m_mapESG.begin();
	EFFSGMAP_ITER iter_end = m_mapESG.end();
	for ( ; iter!=iter_end; )
	{
		(*iter).second->FrameMove ( fTime, fElapsedTime );

		if ( (*iter).second->IsBeginAfterEffect() )
		{
			EFFSGMAP_ITER iter_del = iter;
			++iter;

			NewAfterEff ( (*iter_del).second );
		}
		else if ( (*iter).second->m_dwAliveCount==0 )
		{
			EFFSGMAP_ITER iter_del = iter;
			++iter;

			SAFE_DELETE((*iter_del).second);
			m_mapESG.erase(iter_del);
		}
		else
		{
			++iter;
		}
	}

	//CDebugSet::ToView ( 1, 2, "PARTICLE SLEEP OBJ : %d", DxEffectParticleSys::GetParticlePool()->GetAmount() );

	return S_OK;
}

HRESULT DxEffMouseClick::Render ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	EFFSGMAP_ITER iter = m_mapESG.begin();
	EFFSGMAP_ITER iter_end = m_mapESG.end();
	for ( ; iter!=iter_end; ++iter )
	{
		(*iter).second->Render ( pd3dDevice );
	}

	return S_OK;
}
