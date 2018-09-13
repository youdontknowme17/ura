#include "stdafx.h"
#include "./GLGaeaserver.h"

#include ".\glsummonfield.h"

DWORD GLSummonField::SELECT_SKILLSLOT ( SNATIVEID skill_id )
{
	SKILLREALFACT_SIZE;
	DWORD dwSELECT = UINT_MAX;

	//	Note : ��ų�� �� �⺻ ���� ����.
	//
	for ( DWORD i=0; i<SKILLREALFACT_SIZE; ++i )
	{
		if ( m_sSKILLFACT[i].sNATIVEID==skill_id )
		{
			return dwSELECT = i;
		}
	}

	float fAGE = FLT_MAX;
	for ( DWORD i=0; i<SKILLREALFACT_SIZE; ++i )
	{
		if ( m_sSKILLFACT[i].sNATIVEID==SNATIVEID(false) )
		{
			return dwSELECT = i;
		}

		if ( m_sSKILLFACT[i].fAGE < fAGE )
		{
			fAGE = m_sSKILLFACT[i].fAGE;
			dwSELECT = i;
		}
	}

	return dwSELECT;
}


BOOL GLSummonField::RECEIVE_SKILLFACT ( const SNATIVEID skill_id, const WORD wlevel, DWORD &dwSELECT )
{
	dwSELECT = SKILLFACT_SIZE;

	//	��ų ���� ������.
	PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( skill_id.wMainID, skill_id.wSubID );
	if ( !pSkill )				return FALSE;
	SKILL::CDATA_LVL &sSKILL_DATA = pSkill->m_sAPPLY.sDATA_LVL[wlevel];

	BOOL bHOLD = FALSE;
	SSKILLFACT sSKILLEF;
	switch ( pSkill->m_sAPPLY.emBASIC_TYPE )
	{
	case SKILL::EMFOR_VARHP:
	case SKILL::EMFOR_VARMP:
	case SKILL::EMFOR_VARSP:
	case SKILL::EMFOR_DEFENSE:
	case SKILL::EMFOR_HITRATE:
	case SKILL::EMFOR_AVOIDRATE:
	case SKILL::EMFOR_VARAP:
	case SKILL::EMFOR_VARDAMAGE:
	case SKILL::EMFOR_VARDEFENSE:
	case SKILL::EMFOR_PA:
	case SKILL::EMFOR_SA:
	case SKILL::EMFOR_MA:
	case SKILL::EMFOR_CP_RATE: //add cp
	case SKILL::EMFOR_CP_GAIN: //add cp
		bHOLD = TRUE;
		sSKILLEF.emTYPE = pSkill->m_sAPPLY.emBASIC_TYPE;
		sSKILLEF.fMVAR = sSKILL_DATA.fBASIC_VAR;
		break;
	};

	int _nIndex = 0;
	for(int a = 0; a < SKILL::MAX_LEVEL; a++ )
	{
		if ( pSkill->m_sAPPLY.sADDONS[a].emADDON != EMIMPACTA_NONE )
		{
			bHOLD = TRUE;
			sSKILLEF.emADDON[_nIndex] = pSkill->m_sAPPLY.sADDONS[a].emADDON;
			sSKILLEF.fADDON_VAR[_nIndex] = pSkill->m_sAPPLY.sADDONS[a].fADDON_VAR[wlevel];
			_nIndex += 1;
		}
	}

	int nIndex = 0;
	for(int a = 0; a < SKILL::MAX_LEVEL; a++)
	{
		switch ( pSkill->m_sAPPLY.sSPECS[a].emSPEC )
		{
		case EMSPECA_REFDAMAGE:
		case EMSPECA_NONBLOW:
		case EMSPECA_PIERCE:
		case EMSPECA_TARRANGE:
		case EMSPECA_MOVEVELO:
		case EMSPECA_STUN:
		case EMSPECA_IMMUNE:
		case EMSPECA_CONTINUEDAMAGE:
		case EMSPECA_CURSE:
		case EMSPECA_ATTACK_INT:
		case EMSPECA_ATTACK_RATE:
		case EMSPECA_DEF_INT:
		case EMSPECA_DEF_RATE:
		case EMSPECA_ACCURACY_RATE:
		case EMSPECA_EVASION_RATE:
			bHOLD = TRUE;
			sSKILLEF.emSPEC[nIndex] = pSkill->m_sAPPLY.sSPECS[a].emSPEC;
			sSKILLEF.fSPECVAR1[nIndex] = pSkill->m_sAPPLY.sSPECS[a].sSPEC[wlevel].fVAR1;
			sSKILLEF.fSPECVAR2[nIndex] = pSkill->m_sAPPLY.sSPECS[a].sSPEC[wlevel].fVAR2;
			sSKILLEF.dwSPECFLAG[nIndex] = pSkill->m_sAPPLY.sSPECS[a].sSPEC[wlevel].dwFLAG;
			sSKILLEF.dwNativeID[nIndex] = pSkill->m_sAPPLY.sSPECS[a].sSPEC[wlevel].dwNativeID;
			nIndex++;
			break;
		};
	}

	if ( bHOLD )
	{
		sSKILLEF.sNATIVEID	= skill_id;
		sSKILLEF.wLEVEL		= wlevel;
		sSKILLEF.fAGE		= sSKILL_DATA.fLIFE;

		dwSELECT = SELECT_SKILLSLOT ( skill_id );
		m_sSKILLFACT[dwSELECT] = sSKILLEF;
	}

	return TRUE;
}

//! ������ �켱���� �ֱ����� ������ �α� ���
void GLSummonField::VAR_BODY_POINT ( const EMCROW emACrow, const DWORD dwAID, const BOOL bPartySkill, int nvar_hp, int nvar_mp, int nvar_sp )
{
	if ( m_dwNowHP==0 )		return;

	DWORD dwDxHP = GLOGICEX::VARIATION ( m_dwNowHP, GETMAXHP(), nvar_hp );
	GLOGICEX::VARIATION ( m_wNowMP, GETMAXMP(), nvar_mp );
	GLOGICEX::VARIATION ( m_wNowSP, GETMAXSP(), nvar_sp );

	BOOL bDamage = nvar_hp<0;

	if ( bDamage )
	{
		//	�ڽ��� ������ ��� ���.
		m_sAssault.emCrow = emACrow;
		m_sAssault.dwID = dwAID;

		STARGETID sTargetID = STARGETID ( emACrow, dwAID );
		if( !IsSTATE ( EM_SUMMONACT_ATTACK ) && GetAttackTarget( sTargetID ) )
		{
			SetAttackTarget( sTargetID );
		}

		//	������ ���.
		/*if ( emACrow == CROW_PC )
		{
			PGLCHAR pChar = GLGaeaServer::GetInstance().GetChar ( dwAID );
			if ( pChar )
			{
				AddDamageLog      ( m_cDamageLog, dwAID, pChar->GetUserID(), dwDxHP );
				AddDamageLogParty ( m_cDamageLogParty, pChar->m_dwPartyID, dwDxHP );
			}
		}*/
	}

	//HRESULT hrESCAPE = S_OK;
	//EMCROWACT_UP emCROWACT = GetActPattern();
	//int nDamageHP = abs(nvar_hp);
	//if ( bDamage )		hrESCAPE = ESCAPE ( STARGETID ( emACrow, dwAID ), nDamageHP );

	//if ( EMCROWACT_UP_ESCAPE != emCROWACT || hrESCAPE!=S_OK )
	//{
	//	//	Note :Ÿ���� �������� ������ �����ڰ� ������ Ÿ������ ��� �õ�.
	//	//
	//	if ( !IsFLAG(EMTARGET) && bDamage )
	//	{
	//		PGLCHAR pTarget = NULL;
	//		if ( emACrow==CROW_PC )		pTarget = GLGaeaServer::GetInstance().GetChar(dwAID);

	//		if ( pTarget )
	//		{
	//			STARGETID sTARID(CROW_PC,pTarget->GetCtrlID(),pTarget->GetPosition());
	//			NewTarget ( sTARID );
	//		}
	//	}
	//}
}

void GLSummonField::CURE_STATEBLOW ( DWORD dwCUREFLAG )
{
	BOOL bChanged = FALSE;

	for ( int i=0; i<EMBLOW_MULTI; ++i )
	{
		if ( m_sSTATEBLOWS[i].emBLOW!=EMBLOW_NONE )		continue;

		bChanged = TRUE;
		if ( STATE_TO_DISORDER(m_sSTATEBLOWS[i].emBLOW)&dwCUREFLAG )	m_sSTATEBLOWS[i].emBLOW = EMBLOW_NONE;
	}

	if ( !bChanged )	return;

	//	Note : ���� �̻� ��ȭ MsgFB.
	//
	GLMSG::SNETPC_CURESTATEBLOW_BRD NetMsgBRD;
	NetMsgBRD.dwCUREFLAG = dwCUREFLAG;

	//	Note : �ֺ� Ŭ���̾�Ʈ�鿡�� �޼��� ����.
	//
	SNDMSGAROUND ( (NET_MSG_GENERIC*) &NetMsgBRD );
}

void GLSummonField::STATEBLOW ( const SSTATEBLOW &sStateBlow )
{
	int nIndex = 0;

	if ( sStateBlow.emBLOW <= EMBLOW_SINGLE )	nIndex = 0;
	else										nIndex = sStateBlow.emBLOW-EMBLOW_SINGLE;

	m_sSTATEBLOWS[nIndex] = sStateBlow;

	//	�����̻� ( ���� ) �� ������ ���� ���� �ߴ�.
	if ( sStateBlow.emBLOW == EMBLOW_STUN )
	{
		TurnAction ( GLAT_IDLE );
		SetSTATE( EM_SUMMONACT_STOP );		
	}
}
