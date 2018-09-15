#include "StdAfx.h"
#include "UIInfoLoader.h"
#include "InnerInterface.h"
#include "GLGaeaClient.h"
#include "UITextControl.h"
#include "GameTextControl.h"
#include "../EngineUiLib/GUInterface/BasicTextBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace NS_SKILLINFO
{
	SNATIVEID	m_sNativeIDBack;
	BOOL		m_bNextLevel_BEFORE_FRAME = FALSE;

	bool		m_bLink = false;

	void AddMultiTextLongestLineSplit( CBasicTextBox::MULTITEXT mlText ) { CInnerInterface::GetInstance().ADDMULTITEXTLONGESTLINESPLIT( mlText ); }

	void AddItemName ( CString strText, D3DCOLOR dwColor )
	{
		CInnerInterface::GetInstance().SetItemName ( strText, dwColor );
	}

	void AddItemRender ( SNATIVEID sICONINDEX, const char* szTexture ) 
	{ 
		CInnerInterface::GetInstance().SetItemImage ( sICONINDEX, szTexture, SITEMCUSTOM() ); 
	} 

	void ResetItemRender () 
	{ 
		CInnerInterface::GetInstance().ReSetItemImage ();     
	}

	void RESET ()
	{
		CInnerInterface::GetInstance().CLEAR_TEXT();
		m_sNativeIDBack = NATIVEID_NULL();
		m_bNextLevel_BEFORE_FRAME = FALSE;
		ResetItemRender();
		m_bLink = false;
	}

	void AddTextNoSplit ( CString strText, D3DCOLOR dwColor )
	{
		CInnerInterface::GetInstance().ADDTEXT_NOSPLIT ( strText, dwColor );
	}
	     
	void AddTextLongestLineSplit ( CString strText, D3DCOLOR dwColor )
	{
		CInnerInterface::GetInstance().ADDTEXT_LONGESTLINE_SPLIT ( strText, dwColor );
	}

	void RemoveAllInfo ()
	{
		CInnerInterface::GetInstance().CLEAR_TEXT ();
	}

	void LOAD ( GLSKILL* const pSkill, SCHARSKILL* const pCharSkill, BOOL bNextLevel )
	{
		CString	strText;

		WORD wDispLevel = 0;			
		if ( pCharSkill )
			wDispLevel = pCharSkill->wLevel + 1;				
		else
		{
			if ( bNextLevel ) wDispLevel = 0;
		}

		if ( !pSkill ) 
        {     
            return ; 
            ResetItemRender (); 
        } 

        AddItemRender ( pSkill->m_sEXT_DATA.sICONINDEX, pSkill->m_sEXT_DATA.strICONFILE.c_str() );  
		
		BOOL bMASTER = GLGaeaClient::GetInstance().GetCharacter()->ISMASTER_SKILL ( pSkill->m_sBASIC.sNATIVEID );

		{
			CString strText;
			strText.Format("          %s", pSkill->GetName()); //Name of Skill
			AddItemName( strText, NS_UITEXTCOLOR::WHITE );
		}

		CString szSKillFlag;
		CString strBright;
		CString	strText1;

		{
			if ( pSkill->m_sBASIC.emROLE == SKILL::EMROLE_NORMAL )
				szSKillFlag+= "Physical Attack skills";
			else
				szSKillFlag+= COMMENT::SKILL_ROLE[SKILL::EMROLE_PASSIVE].c_str();
		}

		if ( bNextLevel )
		{
			if ( bMASTER )
			{
				bNextLevel = FALSE;
				AddTextNoSplit ( "", NS_UITEXTCOLOR::PALEGREEN );
				strText1.Format( "             %s/ %dLevel/ %s", szSKillFlag.GetString(), pSkill->m_sBASIC.dwGRADE,
					COMMENT::ELEMENT[pSkill->m_sAPPLY.emELEMENT].c_str());
			}
			else
			{
				if ( wDispLevel )
				{				
					AddTextNoSplit ( "", NS_UITEXTCOLOR::PALEGREEN );
					strText.Format ("             %s:%d", ID2GAMEWORD("SKILL_NEXTLEVEL"), wDispLevel + 1 );
					AddTextNoSplit(strText, NS_UITEXTCOLOR::RED);
					AddTextNoSplit ( "", NS_UITEXTCOLOR::PALEGREEN );
				}
				
				if ( pCharSkill )
				{
					strText1.Format( "%s/ %dLevel/ %s", szSKillFlag.GetString(), pSkill->m_sBASIC.dwGRADE,
						COMMENT::ELEMENT[pSkill->m_sAPPLY.emELEMENT].c_str());
					AddTextNoSplit ( strText1, NS_UITEXTCOLOR::WHITE );
				}
				else
				{
					AddTextNoSplit ( "", NS_UITEXTCOLOR::PALEGREEN );
					strText1.Format( "             %s/ %dLevel/ %s", szSKillFlag.GetString(), pSkill->m_sBASIC.dwGRADE,
						COMMENT::ELEMENT[pSkill->m_sAPPLY.emELEMENT].c_str());
					AddTextNoSplit ( strText1, NS_UITEXTCOLOR::WHITE );
					AddTextNoSplit ( "", NS_UITEXTCOLOR::PALEGREEN );
				}
			}
		}
		else
		{
			AddTextNoSplit ( "", NS_UITEXTCOLOR::PALEGREEN );
			strText1.Format( "             %s/ %dLevel/ %s", szSKillFlag.GetString(), pSkill->m_sBASIC.dwGRADE, COMMENT::ELEMENT[pSkill->m_sAPPLY.emELEMENT].c_str());
			AddTextNoSplit ( strText1, NS_UITEXTCOLOR::WHITE );
			AddTextNoSplit ( "", NS_UITEXTCOLOR::PALEGREEN );
		}

		{	//Weaponry
			const SKILL::SBASIC& sBASIC = pSkill->m_sBASIC;		

			if ( sBASIC.emUSE_LITEM )
			{
				if ( sBASIC.emUSE_LITEM != ITEMATT_NOCARE )
				{
					if ( pSkill->m_sLEARN.sWEAPON.wMainID != 65535 && pSkill->m_sLEARN.sWEAPON.wSubID != 65535 ) strText.Format ( "%s:%s / Fist", ID2GAMEWORD( "SKILL_ADVANCED_USE_ITEM_LR_HAND", 0), ID2GAMEWORD( "SKILL_ADVANCED_USE_ITEM", sBASIC.emUSE_LITEM - 1 ) );
					else strText.Format ( "%s:%s",	ID2GAMEWORD( "SKILL_ADVANCED_USE_ITEM_LR_HAND", 0), ID2GAMEWORD( "SKILL_ADVANCED_USE_ITEM", sBASIC.emUSE_LITEM - 1 ) );
					AddTextNoSplit(strText, NS_UITEXTCOLOR::WHITE );
				}
			}

			if ( sBASIC.emUSE_RITEM )
			{
				if ( sBASIC.emUSE_RITEM != ITEMATT_NOCARE )
				{
					if ( pSkill->m_sLEARN.sWEAPON.wMainID != 65535 && pSkill->m_sLEARN.sWEAPON.wSubID != 65535 ) strText.Format ( "%s:%s / Fist", ID2GAMEWORD( "SKILL_ADVANCED_USE_ITEM_LR_HAND", 1), ID2GAMEWORD( "SKILL_ADVANCED_USE_ITEM", sBASIC.emUSE_RITEM - 1 ) );
					else strText.Format ( "%s:%s",	ID2GAMEWORD( "SKILL_ADVANCED_USE_ITEM_LR_HAND", 1), ID2GAMEWORD( "SKILL_ADVANCED_USE_ITEM", sBASIC.emUSE_RITEM - 1 ) );
					AddTextNoSplit(strText, NS_UITEXTCOLOR::WHITE );
				}
			}
		}

		const SKILL::SAPPLY& sAPPLY = pSkill->m_sAPPLY;

		if ( pCharSkill )
		{
			WORD wLevel = pCharSkill->wLevel;
			if ( bNextLevel ) wLevel = pCharSkill->wLevel + 1;
			const SKILL::CDATA_LVL& sDATA_LVL = sAPPLY.sDATA_LVL[wLevel];

			if ( pSkill->m_sBASIC.emROLE != SKILL::EMROLE_PASSIVE ) //Consumption Requirements
			{
				CString strConText; 
				strConText.Format ( "Consumption Requirements :" );

				if ( 0 < sDATA_LVL.wUSE_ARROWNUM	)	strConText.AppendFormat("%s %d ",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 0), sDATA_LVL.wUSE_ARROWNUM);
				if ( 0 < sDATA_LVL.wUSE_CHARMNUM	)	strConText.AppendFormat("%s %d ",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 1), sDATA_LVL.wUSE_CHARMNUM);
				if ( 0 < sDATA_LVL.wUSE_HP			)	strConText.AppendFormat("%s %d ",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 3), sDATA_LVL.wUSE_HP);
				if ( 0 < sDATA_LVL.wUSE_MP			)	strConText.AppendFormat("%s %d ",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 4), sDATA_LVL.wUSE_MP);
				if ( 0 < sDATA_LVL.wUSE_SP			)	strConText.AppendFormat("%s %d ",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 5), sDATA_LVL.wUSE_SP);
				if ( 0 < sDATA_LVL.wUSE_CP			)	strConText.AppendFormat("%s %d ",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 9), sDATA_LVL.wUSE_CP);
				if ( 0 < sDATA_LVL.wUSE_BULLET		)	strConText.AppendFormat("%s %d ",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 10), sDATA_LVL.wUSE_BULLET);
				if ( 0 < sDATA_LVL.wUSE_EXTRA		)	strConText.AppendFormat("%s %d ",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 11), sDATA_LVL.wUSE_EXTRA);

				AddTextLongestLineSplit ( strConText, NS_UITEXTCOLOR::WHITE );
			}

			if ( 0 < sDATA_LVL.fDELAYTIME ) //Delay / Cooldown
			{
				strText.Format("%s :%2.1fSecond",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC", 0), sDATA_LVL.fDELAYTIME);
				AddTextNoSplit(strText, NS_UITEXTCOLOR::WHITE );
			}
		}
		else
		{
			WORD wLevel = 0;
			const SKILL::CDATA_LVL& sDATA_LVL = sAPPLY.sDATA_LVL[wLevel];

			if ( pSkill->m_sBASIC.emROLE != SKILL::EMROLE_PASSIVE ) //Consumption Requirements
			{
				CString strConText; 
				strConText.Format ( "Consumption Requirements :" );

				if ( 0 < sDATA_LVL.wUSE_ARROWNUM	)	strConText.AppendFormat("%s %d ",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 0), sDATA_LVL.wUSE_ARROWNUM);
				if ( 0 < sDATA_LVL.wUSE_CHARMNUM	)	strConText.AppendFormat("%s %d ",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 1), sDATA_LVL.wUSE_CHARMNUM);
				if ( 0 < sDATA_LVL.wUSE_HP			)	strConText.AppendFormat("%s %d ",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 3), sDATA_LVL.wUSE_HP);
				if ( 0 < sDATA_LVL.wUSE_MP			)	strConText.AppendFormat("%s %d ",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 4), sDATA_LVL.wUSE_MP);
				if ( 0 < sDATA_LVL.wUSE_SP			)	strConText.AppendFormat("%s %d ",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 5), sDATA_LVL.wUSE_SP);
				if ( 0 < sDATA_LVL.wUSE_CP			)	strConText.AppendFormat("%s %d ",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 9), sDATA_LVL.wUSE_CP);
				if ( 0 < sDATA_LVL.wUSE_BULLET		)	strConText.AppendFormat("%s %d ",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 10), sDATA_LVL.wUSE_BULLET);
				if ( 0 < sDATA_LVL.wUSE_EXTRA		)	strConText.AppendFormat("%s %d ",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC_USE_VALUE", 11), sDATA_LVL.wUSE_EXTRA);

				AddTextNoSplit ( strConText, NS_UITEXTCOLOR::WHITE );
			}

			if ( 0 < sDATA_LVL.fDELAYTIME ) //Delay / Cooldown
			{
				strText.Format("%s :%2.1fSecond",ID2GAMEWORD( "SKILL_CDATA_CVL_BASIC", 0), sDATA_LVL.fDELAYTIME);
				AddTextNoSplit(strText, NS_UITEXTCOLOR::WHITE );
			}
		}

		BOOL bMaster = bMASTER;

		BOOL bNOT_LEARN = wDispLevel==0;

		if ( (!bMaster && bNextLevel) || bNOT_LEARN )
		{
			const SKILL::SBASIC& sBASIC = pSkill->m_sBASIC;		

			if ( sBASIC.emUSE_LITEM || sBASIC.emUSE_RITEM )
			{
				if ( sBASIC.emUSE_LITEM != ITEMATT_NOCARE || sBASIC.emUSE_RITEM != ITEMATT_NOCARE )
				{
					if ( pSkill->m_sBASIC.emROLE != SKILL::EMROLE_PASSIVE )
					{
						AddTextNoSplit("",NS_UITEXTCOLOR::WHITE);
					}
				}
			}

			CBasicTextBox::MULTITEXT mlTEXT;
			mlTEXT.strText[0] = ID2GAMEWORD("SKILL_CATEGORY", 7);
			mlTEXT.strText[0] += " :";
			mlTEXT.dwColor[0] = NS_UITEXTCOLOR::WHITE;

			WORD wNextLevel = wDispLevel;
			GLCharacter* const pCharacter = GLGaeaClient::GetInstance().GetCharacter();
			SKILL::SLEARN& sLEARN = pSkill->m_sLEARN;
			SKILL::SLEARN_LVL& sLVL = sLEARN.sLVL_STEP[wNextLevel];			

			SNATIVEID NeedSkillID = pSkill->m_sLEARN.sSKILL;			
			if ( NeedSkillID != NATIVEID_NULL() )
			{
				BOOL bVALID = FALSE;
				BOOL bNeedSkillLevel = FALSE;

				CString strNeedSkillName;
				CString strNeedSkillLevel;

				PGLSKILL pNeedSkill = GLSkillMan::GetInstance().GetData ( NeedSkillID.wMainID, NeedSkillID.wSubID );
				if( pNeedSkill ) strNeedSkillName.Format("%s", pNeedSkill->GetName() );
				bVALID = pCharacter->ISLEARNED_SKILL ( NeedSkillID );			

				if ( 0 < sLVL.dwSKILL_LVL )
				{
					strNeedSkillLevel.Format(" %dLv ", (sLVL.dwSKILL_LVL + 1) );
					bNeedSkillLevel = TRUE;

					SCHARDATA2::SKILL_MAP& map = pCharacter->m_ExpSkills;				
					SCHARDATA2::SKILL_MAP_ITER iter = map.find ( NeedSkillID.dwID );
					if ( iter != map.end() )
					{
						SCHARSKILL& rCharSkill = (*iter).second;

						bVALID = rCharSkill.wLevel >= sLVL.dwSKILL_LVL;						
					}
				}

				CString strCombine;

				if ( bNeedSkillLevel ) strCombine.Format ( "%s%s",strNeedSkillName,strNeedSkillLevel);
				else strCombine.Format ( "%s",strNeedSkillName);

				mlTEXT.strText[1] = strCombine;
				mlTEXT.dwColor[1] = NS_UITEXTCONTROL::GetEvaluateColor ( bVALID );
			}

			if ( 0 < sLVL.dwSKP )
			{
				strText.Format("%s %d ",ID2GAMEWORD( "SKILL_ADVANCED_INFO", 2), sLVL.dwSKP);		
				mlTEXT.strText[2] = strText;
				mlTEXT.dwColor[2] = NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_dwSkillPoint >= sLVL.dwSKP );		
			}

			if ( 0 < sLVL.dwLEVEL )
			{
				strText.Format("%s %d ",ID2GAMEWORD( "SKILL_ADVANCED_INFO", 3), sLVL.dwLEVEL);
				mlTEXT.strText[3] = strText;
				mlTEXT.dwColor[3] = NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->GETLEVEL () >= int(sLVL.dwLEVEL) );	
			}

			if ( 0 < sLVL.dwREBORN )
			{
				strText.Format("%s %d ",ID2GAMEWORD( "SKILL_ADVANCED_INFO", 4), sLVL.dwREBORN);
				mlTEXT.strText[4] = strText;
				mlTEXT.dwColor[4] = NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->GETREBORN () >= int(sLVL.dwREBORN) );	
			}

			if ( 0 < sLVL.sSTATS.dwPow )
			{
				strText.Format("%s %d ",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 0), sLVL.sSTATS.dwPow);
				mlTEXT.strText[5] = strText;
				mlTEXT.dwColor[5] = NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.dwPow >= sLVL.sSTATS.dwPow );	
				//AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.dwPow >= sLVL.sSTATS.dwPow ) );
			}

			if ( 0 < sLVL.sSTATS.dwStr )
			{
				strText.Format("%s %d ",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 1), sLVL.sSTATS.dwStr);
				mlTEXT.strText[6] = strText;
				mlTEXT.dwColor[6] = NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.dwStr >= sLVL.sSTATS.dwStr );	
				AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.dwStr >= sLVL.sSTATS.dwStr ) );
			}

			if ( 0 < sLVL.sSTATS.dwSpi )
			{
				strText.Format("%s %d ",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 2), sLVL.sSTATS.dwSpi);
				mlTEXT.strText[7] = strText;
				mlTEXT.dwColor[7] = NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.dwSpi >= sLVL.sSTATS.dwSpi );	
				//AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.dwSpi >= sLVL.sSTATS.dwSpi ));
			}

			if ( 0 < sLVL.sSTATS.dwDex )
			{
				strText.Format("%s %d ",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 3), sLVL.sSTATS.dwDex);
				mlTEXT.strText[8] = strText;
				mlTEXT.dwColor[8] = NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.dwDex >= sLVL.sSTATS.dwDex );	
				//AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.dwDex >= sLVL.sSTATS.dwDex ) );
			}

			if ( 0 < sLVL.sSTATS.dwInt )
			{
				strText.Format("%s %d ",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 4), sLVL.sSTATS.dwInt);
				mlTEXT.strText[9] = strText;
				mlTEXT.dwColor[9] = NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.dwInt >= sLVL.sSTATS.dwInt );	
				//AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.dwInt >= sLVL.sSTATS.dwInt ) );
			}

			if ( 0 < sLVL.sSTATS.dwSta )
			{
				strText.Format("%s %d ",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 5), sLVL.sSTATS.dwSta);
				mlTEXT.strText[10] = strText;
				mlTEXT.dwColor[10] = NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.dwSta >= sLVL.sSTATS.dwSta );	
				//AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.dwSta >= sLVL.sSTATS.dwSta ) );
			}
			
			AddMultiTextLongestLineSplit( mlTEXT );
			SCHARSKILL* pCharSkill = GLGaeaClient::GetInstance().GetCharacter()->GETLEARNED_SKILL ( pSkill->m_sBASIC.sNATIVEID );	
			if ( !pCharSkill )
			{
				if ( 0 < GLCONST_CHAR::lnSKILL_PREREQ[pSkill->m_sBASIC.sNATIVEID.wMainID][pSkill->m_sBASIC.sNATIVEID.wSubID] )
				{
					CString strTEMP;
					strTEMP.Format( "%d", GLCONST_CHAR::lnSKILL_PREREQ[pSkill->m_sBASIC.sNATIVEID.wMainID][pSkill->m_sBASIC.sNATIVEID.wSubID]); 
					CBasicTextBox::MULTITEXT mlText;
					mlText.strText[0] = ID2GAMEWORD( "SKILL_ADVANCED_INFO", 5);
					mlText.strText[0] += " ";
					mlText.dwColor[0] = NS_UITEXTCOLOR::WHITE;
					mlText.strText[1] = strTEMP.GetString();
					mlText.dwColor[1] = NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_lnMoney >= int(GLCONST_CHAR::lnSKILL_PREREQ[pSkill->m_sBASIC.sNATIVEID.wMainID][pSkill->m_sBASIC.sNATIVEID.wSubID]));
					AddMultiTextLongestLineSplit( mlText );
				}
			}
		}

		//Skill Range
		{
			WORD wLevel = 0;
			if ( pCharSkill ) 
				wLevel = !bNextLevel ? pCharSkill->wLevel : pCharSkill->wLevel + 1;

			AddTextNoSplit("[The Range Of Skill]", NS_UITEXTCOLOR::WHITE );
			
			const SKILL::CDATA_LVL& sDATA_LVL = sAPPLY.sDATA_LVL[wLevel];
			const SKILL::SBASIC& sBASIC = pSkill->m_sBASIC;	

				
			if ((pSkill->m_sBASIC.emIMPACT_TAR == TAR_SPEC || pSkill->m_sBASIC.emIMPACT_TAR == TAR_SELF_TOSPEC) || 
				(pSkill->m_sBASIC.emIMPACT_TAR == TAR_SELF || pSkill->m_sBASIC.emIMPACT_TAR == REALM_ZONE))
			{
				strText.Format("Within %dm range from target and %dm radius in target-centered, %d enemies/friend.",
					sDATA_LVL.wAPPLYRANGE, sBASIC.wTARRANGE, sDATA_LVL.wTARNUM );
			}
			else if (pSkill->m_sBASIC.emIMPACT_TAR == TAR_ZONE_SPEC || pSkill->m_sBASIC.emIMPACT_TAR == TAR_ZONE)
			{
				strText.Format("The %dm within %dm and %d of attack range.",
					sDATA_LVL.wTARNUM, sDATA_LVL.wAPPLYRANGE, sBASIC.wTARRANGE );
			}
			else
				strText.Format("Self / Friend");

			AddTextNoSplit(strText, NS_UITEXTCOLOR::WHITE );
		}

		//Skill Effect
		WORD wLevel = 0;

		if ( pCharSkill )
			wLevel = !bNextLevel ? pCharSkill->wLevel : pCharSkill->wLevel + 1;
		
		const SKILL::CDATA_LVL& sDATA_LVL 	= sAPPLY.sDATA_LVL[wLevel];

		bool bEFFECT	= false;

		for(int a = 0; a < SKILL::MAX_LEVEL; a++ )
		{
			if ( sAPPLY.sADDONS[a].emADDON != EMIMPACTA_NONE || sAPPLY.sSPECS[a].emSPEC != EMSPECA_NULL ) bEFFECT = true;
			if ( bEFFECT )	break;
		}

		if ( sDATA_LVL.fBASIC_VAR != 0 || sAPPLY.emSTATE_BLOW != EMBLOW_NONE || bEFFECT ) 
			AddTextNoSplit("[Skill Effects]", NS_UITEXTCOLOR::GREEN );
					
		//Skill Damage
		if ( sDATA_LVL.fBASIC_VAR != 0 )
		{
			if ( sDATA_LVL.fBASIC_VAR > 0 )
			{
				float fBASIC_VAR_SCALE = sDATA_LVL.fBASIC_VAR * COMMENT::SKILL_TYPES_SCALE[sAPPLY.emBASIC_TYPE];
				if ( pSkill->m_sBASIC.emIMPACT_TAR == TAR_SELF )
				{
					if ( COMMENT::IsSKILL_PER(sAPPLY.emBASIC_TYPE) )
						strText.Format("Let %s decrease %2.1f%%", COMMENT::SKILL_TYPES[sAPPLY.emBASIC_TYPE].c_str(), fBASIC_VAR_SCALE );
					else
						strText.Format("Let %s decrease %2.1f", COMMENT::SKILL_TYPES[sAPPLY.emBASIC_TYPE].c_str(), fBASIC_VAR_SCALE );
				}
				else
				{
					if ( COMMENT::IsSKILL_PER(sAPPLY.emBASIC_TYPE) )
						strText.Format("Let target's %s decrease %2.1f%%", COMMENT::SKILL_TYPES[sAPPLY.emBASIC_TYPE].c_str(), fBASIC_VAR_SCALE );
					else
						strText.Format("Let target's %s decrease %2.1f", COMMENT::SKILL_TYPES[sAPPLY.emBASIC_TYPE].c_str(), fBASIC_VAR_SCALE );
				}

				AddTextNoSplit(strText, NS_UITEXTCOLOR::GREEN );
			}
			else
			{
				float fBASIC_VAR_SCALE = sDATA_LVL.fBASIC_VAR * COMMENT::SKILL_TYPES_SCALE[sAPPLY.emBASIC_TYPE];
				if ( pSkill->m_sBASIC.emIMPACT_TAR == TAR_SELF )
				{
					if ( COMMENT::IsSKILL_PER(sAPPLY.emBASIC_TYPE) )
						strText.Format("Let %s increase %2.1f%%", COMMENT::SKILL_TYPES[sAPPLY.emBASIC_TYPE].c_str(), fBASIC_VAR_SCALE );
					else
						strText.Format("Let %s increase %2.1f", COMMENT::SKILL_TYPES[sAPPLY.emBASIC_TYPE].c_str(), fBASIC_VAR_SCALE );
				}
				else
				{
					if ( COMMENT::IsSKILL_PER(sAPPLY.emBASIC_TYPE) )
						strText.Format("Let target's %s increase %2.1f%%", COMMENT::SKILL_TYPES[sAPPLY.emBASIC_TYPE].c_str(), fBASIC_VAR_SCALE );
					else
						strText.Format("Let target's %s increase %2.1f", COMMENT::SKILL_TYPES[sAPPLY.emBASIC_TYPE].c_str(), fBASIC_VAR_SCALE );
				}

				AddTextNoSplit(strText, NS_UITEXTCOLOR::GREEN );
			}
		}

		//State Trouble
		if ( sAPPLY.emSTATE_BLOW != EMBLOW_NONE )
		{
			AddTextNoSplit(COMMENT::BLOW [ sAPPLY.emSTATE_BLOW ].c_str(), NS_UITEXTCOLOR::GREEN );

			const SKILL::SSTATE_BLOW& sSTATE_BLOW = sAPPLY.sSTATE_BLOW[wLevel];
			if ( sSTATE_BLOW.fRATE > 0 )
			{			
				int nRATE = int(sSTATE_BLOW.fRATE);
				if ( nRATE >= 100 )
					strText.Format ( "%s: %3.2f%s", ID2GAMEWORD( "SKILL_STATE_BLOW", 0), sSTATE_BLOW.fRATE, "%" );
				else
					strText.Format ( "%s: %2.2f%s", ID2GAMEWORD( "SKILL_STATE_BLOW", 0), sSTATE_BLOW.fRATE, "%" );

				AddTextNoSplit(strText, NS_UITEXTCOLOR::GREEN );
			}

			if ( sSTATE_BLOW.fVAR1 != 0 )
			{
				int nRATE = int(sSTATE_BLOW.fVAR1);
				if ( nRATE > 0 )
					strText.Format ( "%s: +%d", COMMENT::BLOW_VAR1[sAPPLY.emSTATE_BLOW].c_str(), nRATE );
				else
					strText.Format ( "%s: %d", COMMENT::BLOW_VAR1[sAPPLY.emSTATE_BLOW].c_str(), nRATE );

				AddTextNoSplit(strText, NS_UITEXTCOLOR::GREEN );
			}

			if ( sSTATE_BLOW.fVAR2 != 0 )
			{
				int nVAR = int(sSTATE_BLOW.fVAR2);
				if ( nVAR > 0 )
					strText.Format ( "%s: +%d", COMMENT::BLOW_VAR2[sAPPLY.emSTATE_BLOW].c_str(), nVAR );
				else
					strText.Format ( "%s: %d", COMMENT::BLOW_VAR2[sAPPLY.emSTATE_BLOW].c_str(), nVAR );

				AddTextNoSplit(strText, NS_UITEXTCOLOR::GREEN );
			}
		}

		//Side Effect
		for(int _a = 0; _a < SKILL::MAX_LEVEL; _a++ )
		{
			if ( sAPPLY.sADDONS[_a].emADDON != EMIMPACTA_NONE )
			{
				float fADDON_VAR = sAPPLY.sADDONS[_a].fADDON_VAR[wLevel] * 100.0f;
				bool bPercent = true;
				
				switch(sAPPLY.sADDONS[_a].emADDON)
				{
				case EMIMPACTA_HITRATE:
				case EMIMPACTA_DAMAGE:
				case EMIMPACTA_DEFENSE:
				case EMIMPACTA_AVOIDRATE:
				case EMIMPACTA_PA:
				case EMIMPACTA_SA:
				case EMIMPACTA_MA:
				case EMIMPACTA_RESIST:
				case EMIMPACTA_HPPOT:
				case EMIMPACTA_MPPOT:
				case EMIMPACTA_SPPOT:
				case EMIMPACTA_CPPOT:
				case EMIMPACTA_CPVAR:
				case EMIMPACTA_CP_RATE:
					fADDON_VAR = sAPPLY.sADDONS[_a].fADDON_VAR[wLevel];
					bPercent = false;
					break;
				};

				float fADDON_VAR_SCALE = fADDON_VAR * COMMENT::IMPACT_ADDON_SCALE[sAPPLY.sADDONS[_a].emADDON];
				if ( fADDON_VAR > 0 )
				{
					if (bPercent)
						strText.Format ( "%s: +%3.2f%s", COMMENT::IMPACT_ADDON[sAPPLY.sADDONS[_a].emADDON].c_str(), fADDON_VAR_SCALE, "%" );
					else
					{
						CString strVAL;
						int nVAL = int(fADDON_VAR_SCALE);
						if(nVAL >= 100 )
							strVAL.Format("%3d", nVAL);
						else
							strVAL.Format("%2d", nVAL);

						strText.Format ( "%s: +%s", COMMENT::IMPACT_ADDON[sAPPLY.sADDONS[_a].emADDON].c_str(), strVAL);
					}

					AddTextNoSplit(strText, NS_UITEXTCOLOR::GREEN );
				}
				else
				{
					if(bPercent) strText.Format ( "%s: %3.2f%s", COMMENT::IMPACT_ADDON[sAPPLY.sADDONS[_a].emADDON].c_str(), fADDON_VAR_SCALE, "%" );
					else
					{
						CString strVAL;
						int nVAL = int(fADDON_VAR_SCALE);
						if(nVAL >= 100 )
							strVAL.Format("%3d", nVAL);
						else
							strVAL.Format("%2d", nVAL);

						strText.Format ( "%s: %s", COMMENT::IMPACT_ADDON[sAPPLY.sADDONS[_a].emADDON].c_str(), strVAL);
					}

					AddTextNoSplit(strText, NS_UITEXTCOLOR::GREEN );
				}
			}
		}

		for(int i = 0; i < 10; i++)
		{
			const SKILL::SSPEC& sSPEC = sAPPLY.sSPECS[i].sSPEC[wLevel];

			if ( sAPPLY.sSPECS[i].emSPEC != EMSPECA_NULL )
			{
				strText.Format ( "%s:%s", ID2GAMEWORD("SKILL_SPEC_TYPE", 0), COMMENT::SPEC_ADDON[sAPPLY.sSPECS[i].emSPEC].c_str() );
				AddTextNoSplit(strText, NS_UITEXTCOLOR::GREEN );
				
				strText.Empty();
				CString strTemp;

				if ( sAPPLY.sSPECS[i].emSPEC == EMSPECA_NONBLOW || sAPPLY.sSPECS[i].emSPEC == EMSPECA_RECBLOW )
				{

					if ( sSPEC.dwFLAG & DIS_NUMB )
					{
						strTemp.Format ( "%s ", COMMENT::BLOW[EMBLOW_NUMB].c_str() );
						strText += strTemp;
					}
					if ( sSPEC.dwFLAG & DIS_STUN )
					{
						strTemp.Format ( "%s ", COMMENT::BLOW[EMBLOW_STUN].c_str() );
						strText += strTemp;
					}
					if ( sSPEC.dwFLAG & DIS_STONE )
					{
						strTemp.Format ( "%s ", COMMENT::BLOW[EMBLOW_STONE].c_str() );
						strText += strTemp;
					}
					if ( sSPEC.dwFLAG & DIS_BURN )
					{
						strTemp.Format ( "%s ", COMMENT::BLOW[EMBLOW_BURN].c_str() );
						strText += strTemp;
					}
					if ( sSPEC.dwFLAG & DIS_FROZEN )
					{
						strTemp.Format ( "%s ", COMMENT::BLOW[EMBLOW_FROZEN].c_str() );
						strText += strTemp;
					}
					if ( sSPEC.dwFLAG & DIS_MAD )
					{
						strTemp.Format ( "%s ", COMMENT::BLOW[EMBLOW_MAD].c_str() );
						strText += strTemp;
					}
					if ( sSPEC.dwFLAG & DIS_POISON )
					{
						strTemp.Format ( "%s ", COMMENT::BLOW[EMBLOW_POISON].c_str() );
						strText += strTemp;
					}
					if ( sSPEC.dwFLAG & DIS_CURSE )
					{
						strTemp.Format ( "%s ", COMMENT::BLOW[EMBLOW_CURSE].c_str() );
						strText += strTemp;
					}
				}
				else if ( sAPPLY.sSPECS[i].emSPEC == EMSPECA_DEFENSE_SKILL_ACTIVE )
				{
					GLSKILL* pSkillTemp = GLSkillMan::GetInstance().GetData( sSPEC.dwNativeID );
					if ( pSkillTemp )
					{
						strTemp.Format ( "%s Lv%d", pSkillTemp->GetName(),sSPEC.dwFLAG );
						strText += strTemp;
					}
				}

				if (strText.GetLength ())
					AddTextNoSplit(strText, NS_UITEXTCOLOR::GREEN );

				if ( 0 != sSPEC.fVAR1 )
				{
					if( COMMENT::SPEC_ADDON_VAR1[EMSPECA_NULL] != COMMENT::SPEC_ADDON_VAR1[sAPPLY.sSPECS[i].emSPEC] )
					{
						float fVAR1_SCALE = sSPEC.fVAR1 * COMMENT::SPEC_ADDON_VAR1_SCALE[sAPPLY.sSPECS[i].emSPEC];

						if ( COMMENT::IsSPEC_ADDON1_PER(sAPPLY.sSPECS[i].emSPEC) )
							strText.Format ( "%s:%2.2f%%", COMMENT::SPEC_ADDON_VAR1[sAPPLY.sSPECS[i].emSPEC].c_str(), fVAR1_SCALE );
						else
							strText.Format ( "%s:%2.2f", COMMENT::SPEC_ADDON_VAR1[sAPPLY.sSPECS[i].emSPEC].c_str(), fVAR1_SCALE );
						
						AddTextNoSplit(strText, NS_UITEXTCOLOR::GREEN );
					}
				}

				if ( 0 != sSPEC.fVAR2 )
				{
					if( COMMENT::SPEC_ADDON_VAR2[EMSPECA_NULL] != COMMENT::SPEC_ADDON_VAR2[sAPPLY.sSPECS[i].emSPEC] )
					{
						float fVAR2_SCALE = sSPEC.fVAR2 * COMMENT::SPEC_ADDON_VAR2_SCALE[sAPPLY.sSPECS[i].emSPEC];

						if ( COMMENT::IsSPEC_ADDON2_PER(sAPPLY.sSPECS[i].emSPEC) )
							strText.Format ( "%s:%2.1f%%", COMMENT::SPEC_ADDON_VAR2[sAPPLY.sSPECS[i].emSPEC].c_str(), fVAR2_SCALE );
						else
							strText.Format ( "%s:%2.1f", COMMENT::SPEC_ADDON_VAR2[sAPPLY.sSPECS[i].emSPEC].c_str(), fVAR2_SCALE );

						AddTextNoSplit(strText, NS_UITEXTCOLOR::GREEN );
					}
				}
			}
		}

		if ( pSkill->m_sSPECIAL_SKILL.emSSTYPE != 0 )
		{
			CString strAbility;
			if ( pSkill->m_sSPECIAL_SKILL.emSSTYPE == 1 || pSkill->m_sSPECIAL_SKILL.emSSTYPE == 3  )
				strAbility.Format ( "%s",("Able to Transform."));
			else if ( pSkill->m_sSPECIAL_SKILL.emSSTYPE == 2 )
				strAbility.Format ( "%s",("Able to Summon."));

			AddTextNoSplit ( strAbility, NS_UITEXTCOLOR::GREEN );
		}

		if ( bNextLevel )		 return;
		if( !pSkill->GetDesc() ) return;

		int StrLength = static_cast<int>(strlen(pSkill->GetDesc()));
		if ( StrLength )
			AddTextLongestLineSplit ( pSkill->GetDesc(), NS_UITEXTCOLOR::WHITE );
	}

	void LOADLINK ( SNATIVEID sSKILLID, bool bLink )
	{
		if ( m_bLink == bLink ) return;

		RESET();

		m_bLink = bLink;

		
		CString	strText;

		PGLSKILL pSkill = GLSkillMan::GetInstance().GetData (sSKILLID.wMainID, sSKILLID.wSubID );
		WORD wDispLevel = 0;	

		if ( !pSkill ) 
        {     
            ResetItemRender (); 
            return; 
        } 

        AddItemRender ( pSkill->m_sEXT_DATA.sICONINDEX, pSkill->m_sEXT_DATA.strICONFILE.c_str() );  
		
		{
			CString strText;
			strText.Format("          %s", pSkill->GetName()); //Name of Skill
			AddItemName( strText, NS_UITEXTCOLOR::DARKLBUE );
		}

		CString szSKillFlag;
		CString strBright;
		CString	strText1;

		if ( pSkill->m_sBASIC.emROLE == SKILL::EMROLE_NORMAL ) szSKillFlag+= "Physical Attack skills";
		else szSKillFlag+= COMMENT::SKILL_ROLE[SKILL::EMROLE_PASSIVE].c_str();

		AddTextNoSplit ( "", NS_UITEXTCOLOR::PALEGREEN );
		strText1.Format( "             %s/ %dLevel/ %s", szSKillFlag.GetString(), pSkill->m_sBASIC.dwGRADE, COMMENT::ELEMENT[pSkill->m_sAPPLY.emELEMENT].c_str());
		AddTextNoSplit ( strText1, NS_UITEXTCOLOR::WHITE );
		AddTextNoSplit ( "", NS_UITEXTCOLOR::PALEGREEN );

		const SKILL::SAPPLY& sAPPLY = pSkill->m_sAPPLY;

		//Skill Range
		{
			WORD wLevel = 0;

			AddTextNoSplit("[The Range Of Skill]", NS_UITEXTCOLOR::WHITE );
			
			const SKILL::CDATA_LVL& sDATA_LVL = sAPPLY.sDATA_LVL[wLevel];
			const SKILL::SBASIC& sBASIC = pSkill->m_sBASIC;	
				
			if ((pSkill->m_sBASIC.emIMPACT_TAR == TAR_SPEC || pSkill->m_sBASIC.emIMPACT_TAR == TAR_SELF_TOSPEC) || 
				(pSkill->m_sBASIC.emIMPACT_TAR == TAR_SELF || pSkill->m_sBASIC.emIMPACT_TAR == REALM_ZONE))
			{
				strText.Format("Within %dm range from target and %dm radius in target-centered, %d enemies/friend.",
					sDATA_LVL.wAPPLYRANGE, sBASIC.wTARRANGE, sDATA_LVL.wTARNUM );
			}
			else if (pSkill->m_sBASIC.emIMPACT_TAR == TAR_ZONE_SPEC || pSkill->m_sBASIC.emIMPACT_TAR == TAR_ZONE)
			{
				strText.Format("The %dm within %dm and %d of attack range.",
					sDATA_LVL.wTARNUM, sDATA_LVL.wAPPLYRANGE, sBASIC.wTARRANGE );
			}
			else
				strText.Format("Self / Friend");

			AddTextNoSplit(strText, NS_UITEXTCOLOR::WHITE );
		}

		//Skill Effect
		WORD wLevel = 0;
		const SKILL::CDATA_LVL& sDATA_LVL 	= sAPPLY.sDATA_LVL[wLevel];

		bool bEFFECT = false;

		for(int a = 0; a < SKILL::MAX_LEVEL; a++ )
		{
			if ( sAPPLY.sADDONS[a].emADDON != EMIMPACTA_NONE || sAPPLY.sSPECS[a].emSPEC != EMSPECA_NULL ) bEFFECT = true;
			if ( bEFFECT )	break;
		}

		if ( sDATA_LVL.fBASIC_VAR != 0 || sAPPLY.emSTATE_BLOW != EMBLOW_NONE || bEFFECT ) 
			AddTextNoSplit("[Skill Effects]", NS_UITEXTCOLOR::GREEN );
					
		//Skill Damage
		if ( sDATA_LVL.fBASIC_VAR != 0 )
		{
			if ( sDATA_LVL.fBASIC_VAR > 0 )
			{
				float fBASIC_VAR_SCALE = sDATA_LVL.fBASIC_VAR * COMMENT::SKILL_TYPES_SCALE[sAPPLY.emBASIC_TYPE];
				if ( pSkill->m_sBASIC.emIMPACT_TAR == TAR_SELF )
				{
					if ( COMMENT::IsSKILL_PER(sAPPLY.emBASIC_TYPE) )
						strText.Format("Let %s decrease %2.1f%%", COMMENT::SKILL_TYPES[sAPPLY.emBASIC_TYPE].c_str(), fBASIC_VAR_SCALE );
					else
						strText.Format("Let %s decrease %2.1f", COMMENT::SKILL_TYPES[sAPPLY.emBASIC_TYPE].c_str(), fBASIC_VAR_SCALE );
				}
				else
				{
					if ( COMMENT::IsSKILL_PER(sAPPLY.emBASIC_TYPE) )
						strText.Format("Let target's %s decrease %2.1f%%", COMMENT::SKILL_TYPES[sAPPLY.emBASIC_TYPE].c_str(), fBASIC_VAR_SCALE );
					else
						strText.Format("Let target's %s decrease %2.1f", COMMENT::SKILL_TYPES[sAPPLY.emBASIC_TYPE].c_str(), fBASIC_VAR_SCALE );
				}

				AddTextNoSplit(strText, NS_UITEXTCOLOR::GREEN );
			}
			else
			{
				float fBASIC_VAR_SCALE = sDATA_LVL.fBASIC_VAR * COMMENT::SKILL_TYPES_SCALE[sAPPLY.emBASIC_TYPE];
				if ( pSkill->m_sBASIC.emIMPACT_TAR == TAR_SELF )
				{
					if ( COMMENT::IsSKILL_PER(sAPPLY.emBASIC_TYPE) )
						strText.Format("Let %s increase %2.1f%%", COMMENT::SKILL_TYPES[sAPPLY.emBASIC_TYPE].c_str(), fBASIC_VAR_SCALE );
					else
						strText.Format("Let %s increase %2.1f", COMMENT::SKILL_TYPES[sAPPLY.emBASIC_TYPE].c_str(), fBASIC_VAR_SCALE );
				}
				else
				{
					if ( COMMENT::IsSKILL_PER(sAPPLY.emBASIC_TYPE) )
						strText.Format("Let target's %s increase %2.1f%%", COMMENT::SKILL_TYPES[sAPPLY.emBASIC_TYPE].c_str(), fBASIC_VAR_SCALE );
					else
						strText.Format("Let target's %s increase %2.1f", COMMENT::SKILL_TYPES[sAPPLY.emBASIC_TYPE].c_str(), fBASIC_VAR_SCALE );
				}

				AddTextNoSplit(strText, NS_UITEXTCOLOR::GREEN );
			}
		}

		//State Trouble
		if ( sAPPLY.emSTATE_BLOW != EMBLOW_NONE )
		{
			AddTextNoSplit(COMMENT::BLOW [ sAPPLY.emSTATE_BLOW ].c_str(), NS_UITEXTCOLOR::GREEN );

			const SKILL::SSTATE_BLOW& sSTATE_BLOW = sAPPLY.sSTATE_BLOW[wLevel];
			if ( sSTATE_BLOW.fRATE > 0 )
			{			
				int nRATE = int(sSTATE_BLOW.fRATE);
				if ( nRATE >= 100 )
					strText.Format ( "%s: %3.2f%s", ID2GAMEWORD( "SKILL_STATE_BLOW", 0), sSTATE_BLOW.fRATE, "%" );
				else
					strText.Format ( "%s: %2.2f%s", ID2GAMEWORD( "SKILL_STATE_BLOW", 0), sSTATE_BLOW.fRATE, "%" );

				AddTextNoSplit(strText, NS_UITEXTCOLOR::GREEN );
			}

			if ( sSTATE_BLOW.fVAR1 != 0 )
			{
				int nRATE = int(sSTATE_BLOW.fVAR1);
				if ( nRATE > 0 )
					strText.Format ( "%s: +%d", COMMENT::BLOW_VAR1[sAPPLY.emSTATE_BLOW].c_str(), nRATE );
				else
					strText.Format ( "%s: %d", COMMENT::BLOW_VAR1[sAPPLY.emSTATE_BLOW].c_str(), nRATE );

				AddTextNoSplit(strText, NS_UITEXTCOLOR::GREEN );
			}

			if ( sSTATE_BLOW.fVAR2 != 0 )
			{
				int nVAR = int(sSTATE_BLOW.fVAR2);
				if ( nVAR > 0 )
					strText.Format ( "%s: +%d", COMMENT::BLOW_VAR2[sAPPLY.emSTATE_BLOW].c_str(), nVAR );
				else
					strText.Format ( "%s: %d", COMMENT::BLOW_VAR2[sAPPLY.emSTATE_BLOW].c_str(), nVAR );

				AddTextNoSplit(strText, NS_UITEXTCOLOR::GREEN );
			}
		}

		//Side Effect
		for(int _a = 0; _a < SKILL::MAX_LEVEL; _a++ )
		{
			if ( sAPPLY.sADDONS[_a].emADDON != EMIMPACTA_NONE )
			{
				float fADDON_VAR = sAPPLY.sADDONS[_a].fADDON_VAR[wLevel] * 100.0f;
				bool bPercent = true;
				
				switch(sAPPLY.sADDONS[_a].emADDON)
				{
				case EMIMPACTA_HITRATE:
				case EMIMPACTA_DAMAGE:
				case EMIMPACTA_DEFENSE:
				case EMIMPACTA_AVOIDRATE:
				case EMIMPACTA_PA:
				case EMIMPACTA_SA:
				case EMIMPACTA_MA:
				case EMIMPACTA_RESIST:
				case EMIMPACTA_HPPOT:
				case EMIMPACTA_MPPOT:
				case EMIMPACTA_SPPOT:
				case EMIMPACTA_CPPOT:
				case EMIMPACTA_CPVAR:
				case EMIMPACTA_CP_RATE:
					fADDON_VAR = sAPPLY.sADDONS[_a].fADDON_VAR[wLevel];
					bPercent = false;
					break;
				};

				float fADDON_VAR_SCALE = fADDON_VAR * COMMENT::IMPACT_ADDON_SCALE[sAPPLY.sADDONS[_a].emADDON];
				if ( fADDON_VAR > 0 )
				{
					if (bPercent)
						strText.Format ( "%s: +%3.2f%s", COMMENT::IMPACT_ADDON[sAPPLY.sADDONS[_a].emADDON].c_str(), fADDON_VAR_SCALE, "%" );
					else
					{
						CString strVAL;
						int nVAL = int(fADDON_VAR_SCALE);
						if(nVAL >= 100 )
							strVAL.Format("%3d", nVAL);
						else
							strVAL.Format("%2d", nVAL);

						strText.Format ( "%s: +%s", COMMENT::IMPACT_ADDON[sAPPLY.sADDONS[_a].emADDON].c_str(), strVAL);
					}

					AddTextNoSplit(strText, NS_UITEXTCOLOR::GREEN );
				}
				else
				{
					if(bPercent) strText.Format ( "%s: %3.2f%s", COMMENT::IMPACT_ADDON[sAPPLY.sADDONS[_a].emADDON].c_str(), fADDON_VAR_SCALE, "%" );
					else
					{
						CString strVAL;
						int nVAL = int(fADDON_VAR_SCALE);
						if(nVAL >= 100 )
							strVAL.Format("%3d", nVAL);
						else
							strVAL.Format("%2d", nVAL);

						strText.Format ( "%s: %s", COMMENT::IMPACT_ADDON[sAPPLY.sADDONS[_a].emADDON].c_str(), strVAL);
					}

					AddTextNoSplit(strText, NS_UITEXTCOLOR::GREEN );
				}
			}
		}

		for(int i = 0; i < 10; i++)
		{
			const SKILL::SSPEC& sSPEC = sAPPLY.sSPECS[i].sSPEC[wLevel];

			if ( sAPPLY.sSPECS[i].emSPEC != EMSPECA_NULL )
			{
				strText.Format ( "%s:%s", ID2GAMEWORD("SKILL_SPEC_TYPE", 0), COMMENT::SPEC_ADDON[sAPPLY.sSPECS[i].emSPEC].c_str() );
				AddTextNoSplit(strText, NS_UITEXTCOLOR::GREEN );
				
				strText.Empty();
				CString strTemp;

				if ( sAPPLY.sSPECS[i].emSPEC == EMSPECA_NONBLOW || sAPPLY.sSPECS[i].emSPEC == EMSPECA_RECBLOW )
				{

					if ( sSPEC.dwFLAG & DIS_NUMB )
					{
						strTemp.Format ( "%s ", COMMENT::BLOW[EMBLOW_NUMB].c_str() );
						strText += strTemp;
					}
					if ( sSPEC.dwFLAG & DIS_STUN )
					{
						strTemp.Format ( "%s ", COMMENT::BLOW[EMBLOW_STUN].c_str() );
						strText += strTemp;
					}
					if ( sSPEC.dwFLAG & DIS_STONE )
					{
						strTemp.Format ( "%s ", COMMENT::BLOW[EMBLOW_STONE].c_str() );
						strText += strTemp;
					}
					if ( sSPEC.dwFLAG & DIS_BURN )
					{
						strTemp.Format ( "%s ", COMMENT::BLOW[EMBLOW_BURN].c_str() );
						strText += strTemp;
					}
					if ( sSPEC.dwFLAG & DIS_FROZEN )
					{
						strTemp.Format ( "%s ", COMMENT::BLOW[EMBLOW_FROZEN].c_str() );
						strText += strTemp;
					}
					if ( sSPEC.dwFLAG & DIS_MAD )
					{
						strTemp.Format ( "%s ", COMMENT::BLOW[EMBLOW_MAD].c_str() );
						strText += strTemp;
					}
					if ( sSPEC.dwFLAG & DIS_POISON )
					{
						strTemp.Format ( "%s ", COMMENT::BLOW[EMBLOW_POISON].c_str() );
						strText += strTemp;
					}
					if ( sSPEC.dwFLAG & DIS_CURSE )
					{
						strTemp.Format ( "%s ", COMMENT::BLOW[EMBLOW_CURSE].c_str() );
						strText += strTemp;
					}
				}
				else if ( sAPPLY.sSPECS[i].emSPEC == EMSPECA_DEFENSE_SKILL_ACTIVE )
				{
					GLSKILL* pSkillTemp = GLSkillMan::GetInstance().GetData( sSPEC.dwNativeID );
					if ( pSkillTemp )
					{
						strTemp.Format ( "%s Lv%d", pSkillTemp->GetName(),sSPEC.dwFLAG );
						strText += strTemp;
					}
				}

				if (strText.GetLength ())
					AddTextNoSplit(strText, NS_UITEXTCOLOR::GREEN );

				if ( 0 != sSPEC.fVAR1 )
				{
					if( COMMENT::SPEC_ADDON_VAR1[EMSPECA_NULL] != COMMENT::SPEC_ADDON_VAR1[sAPPLY.sSPECS[i].emSPEC] )
					{
						float fVAR1_SCALE = sSPEC.fVAR1 * COMMENT::SPEC_ADDON_VAR1_SCALE[sAPPLY.sSPECS[i].emSPEC];

						if ( COMMENT::IsSPEC_ADDON1_PER(sAPPLY.sSPECS[i].emSPEC) )
							strText.Format ( "%s:%2.2f%%", COMMENT::SPEC_ADDON_VAR1[sAPPLY.sSPECS[i].emSPEC].c_str(), fVAR1_SCALE );
						else
							strText.Format ( "%s:%2.2f", COMMENT::SPEC_ADDON_VAR1[sAPPLY.sSPECS[i].emSPEC].c_str(), fVAR1_SCALE );
						
						AddTextNoSplit(strText, NS_UITEXTCOLOR::GREEN );
					}
				}

				if ( 0 != sSPEC.fVAR2 )
				{
					if( COMMENT::SPEC_ADDON_VAR2[EMSPECA_NULL] != COMMENT::SPEC_ADDON_VAR2[sAPPLY.sSPECS[i].emSPEC] )
					{
						float fVAR2_SCALE = sSPEC.fVAR2 * COMMENT::SPEC_ADDON_VAR2_SCALE[sAPPLY.sSPECS[i].emSPEC];

						if ( COMMENT::IsSPEC_ADDON2_PER(sAPPLY.sSPECS[i].emSPEC) )
							strText.Format ( "%s:%2.1f%%", COMMENT::SPEC_ADDON_VAR2[sAPPLY.sSPECS[i].emSPEC].c_str(), fVAR2_SCALE );
						else
							strText.Format ( "%s:%2.1f", COMMENT::SPEC_ADDON_VAR2[sAPPLY.sSPECS[i].emSPEC].c_str(), fVAR2_SCALE );

						AddTextNoSplit(strText, NS_UITEXTCOLOR::GREEN );
					}
				}
			}
		}

		if ( pSkill->m_sSPECIAL_SKILL.emSSTYPE != 0 )
		{
			CString strAbility;
			if ( pSkill->m_sSPECIAL_SKILL.emSSTYPE == 1 || pSkill->m_sSPECIAL_SKILL.emSSTYPE == 3  )
				strAbility.Format ( "%s",("Able to Transform."));
			else if ( pSkill->m_sSPECIAL_SKILL.emSSTYPE == 2 )
				strAbility.Format ( "%s",("Able to Summon."));

			AddTextNoSplit ( strAbility, NS_UITEXTCOLOR::GREEN );
		}
	}

	void LOAD ( const SNATIVEID& sNativeID, const BOOL bNextLevel )
	{
		if ( sNativeID != NATIVEID_NULL() )
		{
			BOOL bUPDATE = FALSE;
			if (( bNextLevel != m_bNextLevel_BEFORE_FRAME ) ||
				( sNativeID != m_sNativeIDBack) )
			{			
				bUPDATE = TRUE;
			}

			if ( bUPDATE )
			{
				PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( sNativeID.wMainID, sNativeID.wSubID );
				if ( pSkill )	
				{
					RemoveAllInfo ();

					SCHARSKILL* pCharSkill = GLGaeaClient::GetInstance().GetCharacter()->GETLEARNED_SKILL ( sNativeID );					
					LOAD ( pSkill, pCharSkill, bNextLevel );
				}

				m_sNativeIDBack = sNativeID;
				m_bNextLevel_BEFORE_FRAME = bNextLevel;
			}		
		}
	}
};