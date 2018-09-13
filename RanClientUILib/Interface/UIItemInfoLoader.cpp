#include "StdAfx.h"
#include "UIInfoLoader.h"
#include "GLGaeaClient.h"
#include "GLItemMan.h"
#include "GLItemDef.h"
#include "GLItem.h"
#include "GameTextControl.h"
#include "UITextControl.h"
#include "InnerInterface.h"
#include "../EngineUiLib/GUInterface/BasicTextBox.h"
#include "DxGlobalStage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace NS_ITEMINFO
{
    SITEMCUSTOM	m_sItemCustomBACK;
	BOOL		m_bShopOpenBACK;
    BOOL		m_bInMarketBACK;
	BOOL		m_bInPrivateMarketBACK;
	BOOL		m_bIsWEAR_ITEMBACK;
	WORD		m_wPosXBACK;
	WORD		m_wPosYBACK;
	BOOL		m_bITEMLINK;

	void SetItemInBox ( SNATIVEID sICONINDEX, const char* szTexture, int nIndex )		{ CInnerInterface::GetInstance().SetItemInBox( sICONINDEX, szTexture, nIndex ); }
	void SetItemInRandomBox ( SNATIVEID sICONINDEX, const char* szTexture, int nIndex ) { CInnerInterface::GetInstance().SetItemInRandomBox( sICONINDEX, szTexture, nIndex ); }
	void ResetItemInBox()																{ CInnerInterface::GetInstance().ResetItemInBox(); }
	void ResetItemInRandomBox()															{ CInnerInterface::GetInstance().ResetItemInRandomBox(); }
	void AddItemRender ( SNATIVEID sICONINDEX, const char* szTexture, SITEMCUSTOM sCustom )	{ CInnerInterface::GetInstance().SetItemImage ( sICONINDEX, szTexture, sCustom ); } 
	void ResetItemRender ()																{ CInnerInterface::GetInstance().ReSetItemImage (); }
	void AddItemName ( CString strText, D3DCOLOR dwColor )								{ CInnerInterface::GetInstance().SetItemName ( strText, dwColor ); }
	void AddTextNoSplit ( CString strText, D3DCOLOR dwColor )							{ CInnerInterface::GetInstance().ADDTEXT_NOSPLIT ( strText, dwColor ); }
	void AddTextLongestLineSplit ( CString strText, D3DCOLOR dwColor )					{ CInnerInterface::GetInstance().ADDTEXT_LONGESTLINE_SPLIT ( strText, dwColor ); }
	void AddMultiText ( CBasicTextBox::MULTITEXT strText )								{ CInnerInterface::GetInstance().ADDMULTITEXT ( strText ); }
	void AddMultiTextLongestLineSplit ( CBasicTextBox::MULTITEXT mlText )				{ CInnerInterface::GetInstance().ADDMULTITEXTLONGESTLINESPLIT( mlText ); }

	void RemoveAllInfo ()																
	{ 
		CInnerInterface::GetInstance().CLEAR_TEXT (); 
		ResetItemRender();
		ResetItemInBox ();
		ResetItemInRandomBox();
	}

	void	RESET ()
	{	
		m_bITEMLINK = FALSE;
		m_sItemCustomBACK.sNativeID = NATIVEID_NULL ();
		m_bShopOpenBACK = FALSE;
		m_bInMarketBACK = FALSE;
		m_bInPrivateMarketBACK = FALSE;
		m_bIsWEAR_ITEMBACK = FALSE;
		ResetItemRender();
		ResetItemInBox ();
		ResetItemInRandomBox();
	}

	char*	GetNumberWithSign ( int nValue )
	{
		static	char szNumber[128];

		if ( 0 < nValue )
		{
			StringCchPrintf ( szNumber, 128, "(+%d)", nValue );
		}
		else if ( nValue < 0 )
		{
			StringCchPrintf ( szNumber, 128, "(%d)", nValue );
		}

		return szNumber;
	}

	void AddInfoItemAddon ( int nBasic, int nAddon, CString strFormat )
	{
		CString Text;
		if ( nAddon )
		{
			Text.Format ( "%s:%d%s", strFormat, nBasic, GetNumberWithSign ( nAddon ) );
			AddTextNoSplit ( Text, NS_UITEXTCOLOR::PRIVATE );
		}
		else if ( nBasic )
		{
			Text.Format ( "%s:%d", strFormat, nBasic );
			AddTextNoSplit ( Text, NS_UITEXTCOLOR::WHITE );
		}
	}

	void APPEND_ITEM_GRADE ( CString& strOrigin, BYTE uGRADE )
	{
		if ( !uGRADE ) return ;

		CString strGRADE;
		strGRADE.Format ( "[+%d]", uGRADE );
		strOrigin += strGRADE;
	}

	void AddInfoItemAddon ( int nBasic, int nAddon, BYTE uGRADE, CString strFormat )
	{
		CString Text;
		if ( nAddon )
		{
			Text.Format ( "%s:%d%s", strFormat, nBasic, GetNumberWithSign( nAddon ) );
			APPEND_ITEM_GRADE ( Text, uGRADE );
			AddTextNoSplit ( Text, NS_UITEXTCOLOR::PRIVATE );
		}
		else if ( nBasic )
		{
			Text.Format ( "%s:%d", strFormat, nBasic );
			APPEND_ITEM_GRADE ( Text, uGRADE );
			AddTextNoSplit ( Text, NS_UITEXTCOLOR::WHITE );
		}
	}

	void AddInfoItemAddonRange ( int nBasicMin, int nBasicMax, int nAddon, BYTE uGRADE, CString strFormat )
	{
		CString Text;

		if ( nAddon )
		{
			Text.Format ( "%s:%d%s~%d%s", strFormat, nBasicMin, GetNumberWithSign( nAddon ), nBasicMax, GetNumberWithSign( nAddon ) );
            APPEND_ITEM_GRADE ( Text, uGRADE );
			AddTextNoSplit ( Text, NS_UITEXTCOLOR::PRIVATE );
		}
		else if ( nBasicMin || nBasicMax )
		{
			Text.Format ( "%s:%d~%d", strFormat, nBasicMin, nBasicMax );
			APPEND_ITEM_GRADE ( Text, uGRADE );
			AddTextNoSplit ( Text, NS_UITEXTCOLOR::WHITE );
		}
	}

	void AddItemTurnInfo ( const SITEMCUSTOM &sItemCustom, const BOOL bInMarket, const BOOL bInPrivateMarket )
	{
		SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sItemCustom.sNativeID );

		CString strText;


			if ( pItemData->sDrugOp.wPileNum > 1 )
			{
				WORD wPileNum = pItemData->sDrugOp.wPileNum;
				WORD wTurnNum = sItemCustom.wTurnNum;				
				if ( bInPrivateMarket )	//	?? ??
				{
					strText.Format("%s:%d", ID2GAMEWORD("ITEM_TURN_INFO", 0 ), wTurnNum);
					AddTextNoSplit(strText,NS_UITEXTCOLOR::CHARTREUSE);

					return ;
				}

				if ( bInMarket )	wTurnNum = pItemData->GETAPPLYNUM();
				strText.Format("%s:%d/%d", ID2GAMEWORD("ITEM_TURN_INFO", 0 ), wTurnNum, wPileNum);
				AddTextNoSplit(strText,NS_UITEXTCOLOR::WHITE);
			}
			//break;
		//}
	}

	void AddInfoPetSkillItem( const SITEMCUSTOM &sItemCustom )
	{
		CString strText;

		SNATIVEID sNativeID = sItemCustom.sNativeID;
		GLPetClient* pPetClient = GLGaeaClient::GetInstance().GetPetClient();
		SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sNativeID );
		SNATIVEID sSkillID = pItemData->sSkillBookOp.sSkill_ID;

		//	Note : ?? ?? ???.
		PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( sSkillID.wMainID, sSkillID.wSubID );
		if ( pSkill )
		{
			if ( pPetClient->ISLEARNED_SKILL( sSkillID ) )
			{
				AddTextNoSplit ( ID2GAMEWORD ( "ITEM_SKILL_CONDITION", 0 ), NS_UITEXTCOLOR::RED );	
				return ;
			}
		}
	}

	void AddInfoSkillItem ( const SITEMCUSTOM &sItemCustom )
	{
		CString strText;

		SNATIVEID sNativeID = sItemCustom.sNativeID;
		GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();	
		SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sNativeID );
		SNATIVEID sSkillID = pItemData->sSkillBookOp.sSkill_ID;

		//	Note : ?? ?? ???.
		PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( sSkillID.wMainID, sSkillID.wSubID );
		if ( pSkill )
		{
			//	?? ??
			{
				//	2. ??
				strText.Format("%s:%d",ID2GAMEWORD("SKILL_BASIC_INFO", 1), pSkill->m_sBASIC.dwGRADE);
				AddTextNoSplit(strText,NS_UITEXTCOLOR::PALEGREEN);

				//	3. ??
				strText.Format("%s:%s",ID2GAMEWORD("SKILL_BASIC_INFO", 2), COMMENT::BRIGHT[pSkill->m_sLEARN.emBRIGHT].c_str());
				AddTextNoSplit(strText,NS_UITEXTCOLOR::PALEGREEN);

				//	4. ??
				strText.Format ("%s:", ID2GAMEWORD("SKILL_BASIC_INFO", 3) );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_FIGHTER_M )		strText.AppendFormat ( "%s", COMMENT::CHARCLASS[0].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_FIGHTER_W )		strText.AppendFormat ( "%s", COMMENT::CHARCLASS[6].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_ARMS_M )			strText.AppendFormat ( "%s", COMMENT::CHARCLASS[1].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_ARMS_W )			strText.AppendFormat ( "%s", COMMENT::CHARCLASS[7].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_ARCHER_M )		strText.AppendFormat ( "%s", COMMENT::CHARCLASS[8].c_str() );				
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_ARCHER_W )		strText.AppendFormat ( "%s", COMMENT::CHARCLASS[2].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_SPIRIT_M )		strText.AppendFormat ( "%s", COMMENT::CHARCLASS[9].c_str() );				
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_SPIRIT_W )		strText.AppendFormat ( "%s", COMMENT::CHARCLASS[3].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_EXTREME_M )	strText.AppendFormat ( "%s", COMMENT::CHARCLASS[4].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_EXTREME_W )	strText.AppendFormat ( "%s", COMMENT::CHARCLASS[5].c_str() );
				//add class
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_SCIENCE_M )	strText.AppendFormat ( "%s", COMMENT::CHARCLASS[10].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_SCIENCE_W )	strText.AppendFormat ( "%s", COMMENT::CHARCLASS[11].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_ASSASIN_M )	strText.AppendFormat ( "%s", COMMENT::CHARCLASS[12].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_ASSASIN_W )	strText.AppendFormat ( "%s", COMMENT::CHARCLASS[13].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_TESTING_M )	strText.AppendFormat ( "%s", COMMENT::CHARCLASS[14].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_TESTING_W )	strText.AppendFormat ( "%s", COMMENT::CHARCLASS[15].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_SHAPER_M )	strText.AppendFormat ( "%s", COMMENT::CHARCLASS[16].c_str() );
				if ( pSkill->m_sLEARN.dwCLASS & GLCC_SHAPER_W )	strText.AppendFormat ( "%s", COMMENT::CHARCLASS[17].c_str() );

				AddTextNoSplit(strText,NS_UITEXTCONTROL::GetEvaluateColor ( pItemData->sBasicOp.dwReqCharClass & pCharacter->m_emClass ) );
			}

			{
				//	?? ????, ??, ??? ??
				const SKILL::SBASIC& sBASIC = pSkill->m_sBASIC;		

				if ( sBASIC.wTARRANGE )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_TARGET_RANGE", 0), sBASIC.wTARRANGE);
					AddTextNoSplit(strText, NS_UITEXTCOLOR::WHITE );
				}

				if ( (sBASIC.emUSE_LITEM && (sBASIC.emUSE_LITEM != ITEMATT_NOCARE)) || 
					(sBASIC.emUSE_RITEM && (sBASIC.emUSE_RITEM != ITEMATT_NOCARE)) )
				{
					AddTextNoSplit(ID2GAMEWORD("SKILL_CATEGORY", 1),NS_UITEXTCOLOR::LIGHTSKYBLUE);
				}

				CString strUSEITEM;
				if ( sBASIC.emUSE_LITEM )
				{
					if ( sBASIC.emUSE_LITEM != ITEMATT_NOCARE )
					{
						strText.Format( "%s:%s", 
										ID2GAMEWORD( "SKILL_ADVANCED_USE_ITEM_LR_HAND", 0), 
										ID2GAMEWORD( "SKILL_ADVANCED_USE_ITEM", sBASIC.emUSE_LITEM - 1 ) );
						AddTextNoSplit(strText, NS_UITEXTCOLOR::WHITE );
					}
				}

				if ( sBASIC.emUSE_RITEM )
				{
					if ( sBASIC.emUSE_RITEM != ITEMATT_NOCARE )
					{
						strText.Format ( "%s:%s", 
										ID2GAMEWORD( "SKILL_ADVANCED_USE_ITEM_LR_HAND", 1), 
										ID2GAMEWORD( "SKILL_ADVANCED_USE_ITEM", sBASIC.emUSE_RITEM - 1 ) );
						AddTextNoSplit(strText, NS_UITEXTCOLOR::WHITE );
					}
				}
			}

			//	?? ?? ??
			if ( pCharacter->GETLEARNED_SKILL ( sSkillID ) )
			{
				AddTextNoSplit ( ID2GAMEWORD ( "ITEM_SKILL_CONDITION", 0 ), NS_UITEXTCOLOR::RED );	
				return ;
			}

			//	??? ??
			{
				AddTextNoSplit( ID2GAMEWORD("SKILL_CATEGORY", 7), NS_UITEXTCOLOR::LIGHTSKYBLUE);

				const WORD wLevel = 0;
				SKILL::SLEARN& sLEARN = pSkill->m_sLEARN;
				SKILL::SLEARN_LVL& sLVL = sLEARN.sLVL_STEP[wLevel];
			
				//	1. ??????
				SNATIVEID NeedSkillID = pSkill->m_sLEARN.sSKILL;			
				if ( NeedSkillID != NATIVEID_NULL() )
				{
					BOOL bVALID = FALSE;
					BOOL bNeedSkillLevel = FALSE;

					CString strNeedSkillName;
					CString strNeedSkillLevel;

					PGLSKILL pNeedSkill = GLSkillMan::GetInstance().GetData ( NeedSkillID.wMainID, NeedSkillID.wSubID );
					strNeedSkillName.Format("%s:%s", ID2GAMEWORD( "SKILL_ADVANCED_INFO", 0), pNeedSkill->GetName() );
					bVALID = pCharacter->ISLEARNED_SKILL ( NeedSkillID );			

					//	2. ????????
					if ( 0 < sLVL.dwSKILL_LVL )
					{
						strNeedSkillLevel.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO", 1), (sLVL.dwSKILL_LVL + 1) );
						bNeedSkillLevel = TRUE;

						SCHARDATA2::SKILL_MAP& map = pCharacter->m_ExpSkills;				
						SCHARDATA2::SKILL_MAP_ITER iter = map.find ( NeedSkillID.dwID );
						if ( iter != map.end() )
						{
							SCHARSKILL& rCharSkill = (*iter).second;

							//	? ??
							bVALID = rCharSkill.wLevel >= sLVL.dwSKILL_LVL;						
						}
					}

					AddTextNoSplit(strNeedSkillName,NS_UITEXTCONTROL::GetEvaluateColor ( bVALID ));

					if ( bNeedSkillLevel )
						AddTextNoSplit(strNeedSkillLevel, NS_UITEXTCONTROL::GetEvaluateColor ( bVALID ) );
				}

				//	3. ?????
				if ( 0 < sLVL.dwSKP )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO", 2), sLVL.dwSKP);				
					AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_dwSkillPoint >= sLVL.dwSKP ) );
				}

				//	4. ????
				if ( 0 < sLVL.dwLEVEL )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO", 3), sLVL.dwLEVEL);
					AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->GETLEVEL () >= int(sLVL.dwLEVEL) ) );
				}

				if ( 0 < sLVL.dwREBORN )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO", 4), sLVL.dwREBORN);
					AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->GETREBORN () >= int(sLVL.dwREBORN) ) );
				}

				/*if ( 0 < sLVL.dwMoneyReq )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO", 5), sLVL.dwMoneyReq );
					AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_lnMoney >= int(sLVL.dwMoneyReq ) ) );
				}

				if ( 0 < sLVL.dwPremPoint )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO", 6), sLVL.dwPremPoint );
					AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_lnPremP >= int(sLVL.dwPremPoint ) ) );
				}
				if ( 0 < sLVL.dwVotePoint )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO", 7), sLVL.dwVotePoint );
					AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_lnVoteP >= int(sLVL.dwVotePoint ) ) );
				}*/

				//	5. ?? - ??
				BOOL bValue = TRUE;
				strText.Format ( "%s", COMMENT::BRIGHT[pItemData->sBasicOp.emReqBright].c_str() );
				if ( pItemData->sBasicOp.emReqBright != BRIGHT_BOTH )
				{
					if ( pCharacter->GETBRIGHT() != pItemData->sBasicOp.emReqBright )
					{
						bValue = FALSE;
					}
				}
				AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( bValue ) );


				//	Stats
				//	1. ???
				if ( 0 < sLVL.sSTATS.dwPow )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 0), sLVL.sSTATS.dwPow);
					AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.dwPow >= sLVL.sSTATS.dwPow ) );
				}

				//	2. ????
				if ( 0 < sLVL.sSTATS.dwStr )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 1), sLVL.sSTATS.dwStr);
					AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.dwStr >= sLVL.sSTATS.dwStr ) );
				}

				//	3. ????
				if ( 0 < sLVL.sSTATS.dwSpi )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 2), sLVL.sSTATS.dwSpi);
					AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.dwSpi >= sLVL.sSTATS.dwSpi ));
				}

				//	4. ????
				if ( 0 < sLVL.sSTATS.dwDex )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 3), sLVL.sSTATS.dwDex);
					AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.dwDex >= sLVL.sSTATS.dwDex ) );
				}

				//	5. ????
				if ( 0 < sLVL.sSTATS.dwInt )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 4), sLVL.sSTATS.dwInt);
					AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.dwInt >= sLVL.sSTATS.dwInt ) );
				}

				//	6. ????
				if ( 0 < sLVL.sSTATS.dwSta )
				{
					strText.Format("%s:%d",ID2GAMEWORD( "SKILL_ADVANCED_INFO_STATS", 5), sLVL.sSTATS.dwSta);
					AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pCharacter->m_sSUMSTATS.dwSta >= sLVL.sSTATS.dwSta ) );
				}
			}
		}
	}

	void AddTextAddValue( const SITEMCUSTOM & sItemCustom, const ITEM::SSUIT & sSUIT )
	{
		CString strText;
		BOOL bLEAST_HAVE_ONE(TRUE);
		BOOL bADD_HP(FALSE), bADD_MP(FALSE), bADD_SP(FALSE), bADD_MA(FALSE);
		INT arrVALUE[EMADD_SIZE];
		SecureZeroMemory( arrVALUE, sizeof(arrVALUE) );

		for ( int i = 0; i < ITEM::SSUIT::ADDON_SIZE; ++i )
		{
			EMITEM_ADDON emTYPE = sSUIT.sADDON[i].emTYPE;

			if ( emTYPE != EMADD_NONE )
			{
				//	NOTE
				//		????? ??????? ????
				//		???? ??, ??? ??? ????
				//		???? ?? ????.
				if ( bLEAST_HAVE_ONE )
				{
					//AddTextNoSplit( ID2GAMEWORD ( "ITEM_CATEGORY", 10 ),NS_UITEXTCOLOR::LIGHTSKYBLUE);
					bLEAST_HAVE_ONE = FALSE;
				}

				int nVALUE = sSUIT.sADDON[i].nVALUE;

				switch ( emTYPE )
				{
				case EMADD_HP:
					if( !bADD_HP )
					{
						arrVALUE[EMADD_HP] = sItemCustom.GETADDHP();
						bADD_HP = TRUE;
					}
					break;
				case EMADD_MP:
					if( !bADD_MP )
					{
						arrVALUE[EMADD_MP] = sItemCustom.GETADDMP();
						bADD_MP = TRUE;
					}
					break;
				case EMADD_SP:
					if( !bADD_SP )
					{
						arrVALUE[EMADD_SP] = sItemCustom.GETADDSP();
						bADD_SP = TRUE;
					}
					break;
				case EMADD_MA:
					if( !bADD_MA )
					{
						arrVALUE[EMADD_MA] = sItemCustom.GETADDMA();
						bADD_MA = TRUE;
					}
					break;
				case EMADD_HITRATE:		arrVALUE[EMADD_HITRATE] += nVALUE;		break;
				case EMADD_AVOIDRATE:	arrVALUE[EMADD_AVOIDRATE] += nVALUE;	break;
				case EMADD_DAMAGE:		arrVALUE[EMADD_DAMAGE] += nVALUE;		break;
				case EMADD_DEFENSE:		arrVALUE[EMADD_DEFENSE] += nVALUE;		break;
				case EMADD_STATS_POW:	arrVALUE[EMADD_STATS_POW] += nVALUE;	break;
				case EMADD_STATS_STR:	arrVALUE[EMADD_STATS_STR] += nVALUE;	break;
				case EMADD_STATS_SPI:	arrVALUE[EMADD_STATS_SPI] += nVALUE;	break;
				case EMADD_STATS_DEX:	arrVALUE[EMADD_STATS_DEX] += nVALUE;	break;
				case EMADD_STATS_INT:	arrVALUE[EMADD_STATS_INT] += nVALUE;	break;
				case EMADD_STATS_STA:	arrVALUE[EMADD_STATS_STA] += nVALUE;	break;
				case EMADD_PA:			arrVALUE[EMADD_PA] += nVALUE;			break;
				case EMADD_SA:			arrVALUE[EMADD_SA] += nVALUE;			break;
				case EMADD_CP:			arrVALUE[EMADD_CP] += nVALUE;			break; //add cp
				};
			}
		}

		for( int i=1; i<EMADD_SIZE; ++i)
		{
			if( arrVALUE[i] != 0 )
			{
				strText.Format( "%s:%s", ID2GAMEWORD( "ITEM_ADDON_INFO", i ), GetNumberWithSign( arrVALUE[i] ) );
				AddTextNoSplit( strText, NS_UITEXTCOLOR::ORANGE );
			}
		}

		if ( sItemCustom.nidDISGUISE!=SNATIVEID(false) )
		{
			SITEM* pJDisguiseData = GLItemMan::GetInstance().GetItem ( sItemCustom.nidDISGUISE );
			//if ( pJDisguiseData )
			//{
				//if ( pJDisguiseData->sBasicOp.IsCostumeCombine() )
				//{
					//add jitem
					INT arrVALUECOS[EMADD_SIZE];
					SecureZeroMemory( arrVALUECOS, sizeof(arrVALUECOS) );
	
					for ( int j = 0; j < ITEM::SSUIT::ADDON_SIZE; ++j )
					{
						EMITEM_ADDON emTYPEJ = pJDisguiseData->sSuitOp.sADDON[j].emTYPE;

						if ( emTYPEJ != EMADD_NONE )
						{
							if ( bLEAST_HAVE_ONE )
							{
								AddTextNoSplit( ID2GAMEWORD ( "ITEM_CATEGORY", 13 ),NS_UITEXTCOLOR::PALEGREEN);
								bLEAST_HAVE_ONE = FALSE;
							}

							int nVALUECOS = pJDisguiseData->sSuitOp.sADDON[j].nVALUE;
							switch ( emTYPEJ )
							{
								case EMADD_HP:			arrVALUECOS[EMADD_HP] += nVALUECOS;			break;
								case EMADD_MP:			arrVALUECOS[EMADD_MP] += nVALUECOS;			break;
								case EMADD_SP:			arrVALUECOS[EMADD_SP] += nVALUECOS;			break;
								case EMADD_MA:			arrVALUECOS[EMADD_MA] += nVALUECOS;			break;
								case EMADD_HITRATE:		arrVALUECOS[EMADD_HITRATE] += nVALUECOS;		break;
								case EMADD_AVOIDRATE:	arrVALUECOS[EMADD_AVOIDRATE] += nVALUECOS;		break;
								case EMADD_DAMAGE:		arrVALUECOS[EMADD_DAMAGE] += nVALUECOS;		break;
								case EMADD_DEFENSE:		arrVALUECOS[EMADD_DEFENSE] += nVALUECOS;		break;
								case EMADD_STATS_POW:	arrVALUECOS[EMADD_STATS_POW] += nVALUECOS;		break;
								case EMADD_STATS_STR:	arrVALUECOS[EMADD_STATS_STR] += nVALUECOS;		break;
								case EMADD_STATS_SPI:	arrVALUECOS[EMADD_STATS_SPI] += nVALUECOS;		break;
								case EMADD_STATS_DEX:	arrVALUECOS[EMADD_STATS_DEX] += nVALUECOS;		break;
								case EMADD_STATS_INT:	arrVALUECOS[EMADD_STATS_INT] += nVALUECOS;		break;
								case EMADD_STATS_STA:	arrVALUECOS[EMADD_STATS_STA] += nVALUECOS;		break;
								case EMADD_PA:			arrVALUECOS[EMADD_PA] += nVALUECOS;			break;
								case EMADD_SA:			arrVALUECOS[EMADD_SA] += nVALUECOS;			break;
								case EMADD_CP:			arrVALUECOS[EMADD_CP] += nVALUECOS;			break; //add cp
							};
						}
					}

					CString strTextCos;
					for( int i=1; i<EMADD_SIZE; ++i)
					{
						if( arrVALUECOS[i] != 0 )
						{
							strTextCos.Format( "%s:%s", ID2GAMEWORD( "ITEM_ADDON_INFOCOS", i ), GetNumberWithSign( arrVALUECOS[i] ) );
							AddTextNoSplit( strTextCos, NS_UITEXTCOLOR::PALEGREEN );
						}		
					}
				}
			}	
		//}
	//}

	void LOAD_SIMPLE ( const SITEMCUSTOM &sItemCustom )
	{
		if ( m_sItemCustomBACK == sItemCustom ) return ;

		m_sItemCustomBACK = sItemCustom;

		RemoveAllInfo();

		SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sItemCustom.sNativeID );
		if ( !pItemData ) return ;
		
		DWORD dwLevel = pItemData->sBasicOp.emLevel;
		//	??
		AddTextNoSplit ( pItemData->GetName(), COMMENT::ITEMCOLOR[dwLevel] );
	}

	void LOAD ( const SITEMCUSTOM &sItemCustom, const BOOL bShopOpen, const BOOL bInMarket, const BOOL bInPrivateMarket, const BOOL bIsWEAR_ITEM, WORD wPosX, WORD wPosY, SNATIVEID sNpcNativeID, BOOL bITEMLINK )
	{	
		SITEM* pItem = GLItemMan::GetInstance().GetItem(sItemCustom.sNativeID);
		if ( pItem )
		{
			int nIndex = -1;
			//bool bXEM = false;
			const EMCHARCLASS emCharClass = DxGlobalStage::GetInstance().GetGameStage()->GetCharJoinData().m_CharData2.m_emClass;
			const int nClassType = CharClassToIndex ( emCharClass );
			if( nClassType == GLCI_EXTREME_W || nClassType == GLCI_EXTREME_M )
			{
				//bXEM = true;
				switch(pItem->sSuitOp.emSuit)
				{
				case SUIT_HEADGEAR: nIndex = 0; break;
				case SUIT_UPPER: nIndex = 1; break;
				case SLOT_WAIST: nIndex = 2; break;
				case SLOT_LOWER: nIndex = 3; break;
				case SLOT_HAND: nIndex = 4; break;
				case SLOT_FOOT: nIndex = 5; break;
				case SLOT_LACCESSORY: nIndex = 6; break;
				case SLOT_LEARRING: nIndex = 7; break;
				case SLOT_REARRING: nIndex = 8; break;
				case SLOT_NECK: nIndex = 9; break;
				case SLOT_WRIST: nIndex = 10; break;
				case SLOT_LFINGER: nIndex = 11; break;
				case SLOT_RFINGER: nIndex = 12; break;
				case SLOT_RACCESSORY: nIndex = 13; break;
				case SLOT_RHAND: nIndex = 14; break;
				case SLOT_LHAND: nIndex = 15; break;
				case SLOT_RHAND_S: nIndex = 16; break;
				case SLOT_LHAND_S: nIndex = 17; break;
				case SLOT_VEHICLE: nIndex = 18; break;
				}
			}
			else
			{
				switch(pItem->sSuitOp.emSuit)
				{
				case SUIT_HEADGEAR: nIndex = 0; break;
				case SUIT_UPPER: nIndex = 1; break;
				case SLOT_WAIST: nIndex = 2; break;
				case SLOT_LOWER: nIndex = 3; break;
				case SLOT_HAND: nIndex = 4; break;
				case SLOT_FOOT: nIndex = 5; break;
				case SLOT_LACCESSORY: nIndex = 6; break;
				case SLOT_LEARRING: nIndex = 7; break;
				case SLOT_REARRING: nIndex = 8; break;
				case SLOT_NECK: nIndex = 9; break;
				case SLOT_WRIST: nIndex = 10; break;
				case SLOT_LFINGER: nIndex = 11; break;
				case SLOT_RFINGER: nIndex = 12; break;
				case SLOT_RACCESSORY: nIndex = 13; break;
				case SLOT_RHAND: nIndex = 14; break;
				case SLOT_LHAND: nIndex = 15; break;
				case SLOT_VEHICLE: nIndex = 16; break;
				}
			}

		/*	if ( nIndex != -1 && pItem->sBasicOp.emItemType == ITEM_SUIT ) 
			{
				CInnerInterface::GetInstance().SetIndex(nIndex);
				CInnerInterface::GetInstance().SetIndexClass(bXEM);
			}*/
		}

		if ( m_sItemCustomBACK == sItemCustom && m_bShopOpenBACK == bShopOpen
			&& m_bInMarketBACK == bInMarket && m_bInPrivateMarketBACK == bInPrivateMarket
			&& m_bIsWEAR_ITEMBACK == bIsWEAR_ITEM && m_wPosXBACK == wPosX && m_wPosYBACK == wPosY && m_bITEMLINK == bITEMLINK )	return ;

		m_sItemCustomBACK = sItemCustom;
		m_bShopOpenBACK = bShopOpen;
		m_bInMarketBACK = bInMarket;
		m_bInPrivateMarketBACK = bInPrivateMarket;
		m_bIsWEAR_ITEMBACK = bIsWEAR_ITEM;
		m_wPosXBACK = wPosX;
		m_wPosYBACK = wPosY;
		m_bITEMLINK = bITEMLINK;

		RemoveAllInfo();

		CString strText, strText2;
		BOOL bValue = FALSE;
		int nExtraValue = 0;

		GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
		SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sItemCustom.sNativeID );

		{
			AddTextNoSplit ( "", NS_UITEXTCOLOR::ORANGE );

			SCHARSTATS& rItemStats = pItemData->sBasicOp.sReqStats;
			SCHARSTATS& rCharStats = pCharacter->m_sSUMSTATS;
			EMITEM_TYPE emItemType = pItemData->sBasicOp.emItemType;

			{	//School and Class Requirement
				switch ( emItemType )
				{
				case ITEM_SUIT:
				case ITEM_REVIVE:
				case ITEM_ANTI_DISAPPEAR:
				case ITEM_VEHICLE:
					{
						CString strSchool, strSchoolText, strClass, strClassText;
						BOOL bReqSchool, bNeedDisguise, bReqClass = FALSE;
						DWORD dwCLASSFIVE = (GLCC_FIGHTER_M|GLCC_ARMS_M|GLCC_ARCHER_W|GLCC_SPIRIT_W|GLCC_FIGHTER_W|GLCC_ARMS_W|GLCC_ARCHER_M|GLCC_SPIRIT_M|GLCC_EXTREME_M|GLCC_EXTREME_W);
						
						if( sItemCustom.nidDISGUISE != NATIVEID_NULL() )
						{
							SITEM* pItemDisguise = GLItemMan::GetInstance().GetItem ( sItemCustom.nidDISGUISE );
							DWORD dwReqCharClass_Disguise = pItemDisguise->sBasicOp.dwReqCharClass;
							DWORD dwReqCharClass = pItemData->sBasicOp.dwReqCharClass;
							bNeedDisguise = TRUE;

							if ((pItemData->sBasicOp.dwReqSchool!=GLSCHOOL_ALL)&&(pItemDisguise->sBasicOp.dwReqSchool!=GLSCHOOL_ALL))
							{
								if ((pItemData->sBasicOp.dwReqSchool & GLSCHOOL_00)&&(pItemDisguise->sBasicOp.dwReqSchool & GLSCHOOL_00)) strSchool.AppendFormat ( "%s ",ID2GAMEWORD("ACADEMY_NAME",0));
								if ((pItemData->sBasicOp.dwReqSchool & GLSCHOOL_01)&&(pItemDisguise->sBasicOp.dwReqSchool & GLSCHOOL_01)) strSchool.AppendFormat ( "%s ",ID2GAMEWORD("ACADEMY_NAME",1));
								if ((pItemData->sBasicOp.dwReqSchool & GLSCHOOL_02)&&(pItemDisguise->sBasicOp.dwReqSchool & GLSCHOOL_02)) strSchool.AppendFormat ( "%s ",ID2GAMEWORD("ACADEMY_NAME",2));
								bReqSchool = TRUE;
							}
							else
							{
								strSchool = ID2GAMEWORD("ACADEMY_ALL",0);
								bReqSchool = TRUE;
							}

							if ( dwReqCharClass )
							{
								if ((dwReqCharClass==GLCC_ALL_2015) && (dwReqCharClass_Disguise==GLCC_ALL_2015)) 
								{
									bReqClass = TRUE;
									strClass.Format ( "%s", ID2GAMEWORD("ITEM_ALL_CLASS",0));
								}
								else if ((dwReqCharClass==GLCC_MAN) && (dwReqCharClass_Disguise==GLCC_MAN))
								{
									bReqClass = TRUE;
									strClass.Format ( "%s", ID2GAMEWORD("ITEM_ALL_CLASS",1));
								}
								else if ((dwReqCharClass==GLCC_WOMAN) && (dwReqCharClass_Disguise==GLCC_WOMAN))
								{
									bReqClass = TRUE;
									strClass.Format ( "%s", ID2GAMEWORD("ITEM_ALL_CLASS",2));
								}
								else if ((dwReqCharClass == dwCLASSFIVE) && (dwReqCharClass_Disguise== dwCLASSFIVE))
								{
									bReqClass = TRUE;
									strClass.Format ( "%s", ID2GAMEWORD("ITEM_ALL_CLASS",0));
								}
								else
								{
									bReqClass = TRUE;
									//Brawler
									if ((dwReqCharClass&GLCC_FIGHTER_M)&&(dwReqCharClass_Disguise&GLCC_FIGHTER_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_FIGHTER_M)].c_str());
									if ((dwReqCharClass&GLCC_FIGHTER_W)&&(dwReqCharClass_Disguise&GLCC_FIGHTER_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_FIGHTER_W)].c_str());
									//Swordsman
									if ((dwReqCharClass&GLCC_ARMS_M)&&(dwReqCharClass_Disguise&GLCC_ARMS_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARMS_M)].c_str());
									if ((dwReqCharClass&GLCC_ARMS_W)&&(dwReqCharClass_Disguise&GLCC_ARMS_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARMS_W)].c_str());
									//Archer
									if ((dwReqCharClass&GLCC_ARCHER_M)&&(dwReqCharClass_Disguise&GLCC_ARCHER_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_M)].c_str());
									if ((dwReqCharClass&GLCC_ARCHER_W)&&(dwReqCharClass_Disguise&GLCC_ARCHER_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_W)].c_str());
									//Shaman
									if ((dwReqCharClass&GLCC_SPIRIT_M)&&(dwReqCharClass_Disguise&GLCC_SPIRIT_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_SPIRIT_M)].c_str());
									if ((dwReqCharClass&GLCC_SPIRIT_W)&&(dwReqCharClass_Disguise&GLCC_SPIRIT_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_SPIRIT_W)].c_str());
									//Extreme
									if ((dwReqCharClass&GLCC_EXTREME_M)&&(dwReqCharClass_Disguise&GLCC_EXTREME_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_M)].c_str());
									if ((dwReqCharClass&GLCC_EXTREME_W)&&(dwReqCharClass_Disguise&GLCC_EXTREME_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_W)].c_str());
									//Scientist
									if ((dwReqCharClass&GLCC_SCIENCE_M)&&(dwReqCharClass_Disguise&GLCC_SCIENCE_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_SCIENCE_M)].c_str());
									if ((dwReqCharClass&GLCC_SCIENCE_W)&&(dwReqCharClass_Disguise&GLCC_SCIENCE_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_SCIENCE_W)].c_str());
									//Assassin
									if ((dwReqCharClass&GLCC_ASSASIN_M)&&(dwReqCharClass_Disguise&GLCC_ASSASIN_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASIN_M)].c_str());
									if ((dwReqCharClass&GLCC_ASSASIN_W)&&(dwReqCharClass_Disguise&GLCC_ASSASIN_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASIN_W)].c_str());
									//Tricker
									if ((dwReqCharClass&GLCC_TESTING_M)&&(dwReqCharClass_Disguise&GLCC_TESTING_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_TESTING_M)].c_str());
									if ((dwReqCharClass&GLCC_TESTING_W)&&(dwReqCharClass_Disguise&GLCC_TESTING_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_TESTING_W)].c_str());
									//Shaper
									if ((dwReqCharClass&GLCC_TESTING_M)&&(dwReqCharClass_Disguise&GLCC_SHAPER_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_SHAPER_M)].c_str());
									if ((dwReqCharClass&GLCC_TESTING_W)&&(dwReqCharClass_Disguise&GLCC_SHAPER_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_SHAPER_W)].c_str());
									
									if (	(dwReqCharClass&GLCC_FIGHTER_M)		&& (dwReqCharClass_Disguise&GLCC_FIGHTER_M) 	&&
											(dwReqCharClass&GLCC_ARMS_M)  		&& (dwReqCharClass_Disguise&GLCC_ARMS_M) 		&&	
											(dwReqCharClass&GLCC_ARCHER_M)  	&& (dwReqCharClass_Disguise&GLCC_ARCHER_M)  	&&	
											(dwReqCharClass&GLCC_SPIRIT_M)  	&& (dwReqCharClass_Disguise&GLCC_SPIRIT_M)  	&&	
											(dwReqCharClass&GLCC_EXTREME_M) 	&& (dwReqCharClass_Disguise&GLCC_EXTREME_M) 	&&
											(dwReqCharClass&GLCC_SCIENCE_M) 	&& (dwReqCharClass_Disguise&GLCC_SCIENCE_M) 	&&	
											(dwReqCharClass&GLCC_ASSASIN_M) 	&& (dwReqCharClass_Disguise&GLCC_ASSASIN_M) 	&&	
											(dwReqCharClass&GLCC_TESTING_M)  	&& (dwReqCharClass_Disguise&GLCC_TESTING_M) 	&&	
											(dwReqCharClass&GLCC_SHAPER_M)  	&& (dwReqCharClass_Disguise&GLCC_SHAPER_M)			)
									{
										strClass.Format( "%s", ID2GAMEWORD("ITEM_CLASS_GENDER",0));
									}
									else if ((dwReqCharClass&GLCC_FIGHTER_W) 	&& (dwReqCharClass_Disguise&GLCC_FIGHTER_W) 	&&
											(dwReqCharClass&GLCC_ARMS_W)  		&& (dwReqCharClass_Disguise&GLCC_ARMS_W) 		&&	
											(dwReqCharClass&GLCC_ARCHER_W)  	&& (dwReqCharClass_Disguise&GLCC_ARCHER_W)  	&&	
											(dwReqCharClass&GLCC_SPIRIT_W)  	&& (dwReqCharClass_Disguise&GLCC_SPIRIT_W)  	&&	
											(dwReqCharClass&GLCC_EXTREME_W)  	&& (dwReqCharClass_Disguise&GLCC_EXTREME_W) 	&&
											(dwReqCharClass&GLCC_SCIENCE_W)  	&& (dwReqCharClass_Disguise&GLCC_SCIENCE_W) 	&&	
											(dwReqCharClass&GLCC_ASSASIN_W)  	&& (dwReqCharClass_Disguise&GLCC_ASSASIN_W) 	&&	
											(dwReqCharClass&GLCC_TESTING_W)  	&& (dwReqCharClass_Disguise&GLCC_TESTING_W) 	&&	
											(dwReqCharClass&GLCC_SHAPER_W)  	&& (dwReqCharClass_Disguise&GLCC_SHAPER_W)			)
									{
										strClass.Format( "%s", ID2GAMEWORD("ITEM_CLASS_GENDER",1));
									}
								}
							}
						}
						else
						{
							DWORD dwReqCharClass = pItemData->sBasicOp.dwReqCharClass;

							if (pItemData->sBasicOp.dwReqSchool!=GLSCHOOL_ALL)
							{
								if (pItemData->sBasicOp.dwReqSchool & GLSCHOOL_00) strSchool.AppendFormat ( "%s ",ID2GAMEWORD("ACADEMY_NAME",0));
								if (pItemData->sBasicOp.dwReqSchool & GLSCHOOL_01) strSchool.AppendFormat ( "%s ",ID2GAMEWORD("ACADEMY_NAME",1));
								if (pItemData->sBasicOp.dwReqSchool & GLSCHOOL_02) strSchool.AppendFormat ( "%s ",ID2GAMEWORD("ACADEMY_NAME",2));
								bReqSchool = TRUE;
							}
							else
							{
								strSchool = ID2GAMEWORD("ACADEMY_ALL",0);
								bReqSchool = TRUE;
							}

							if ( dwReqCharClass )
							{
								if ( dwReqCharClass==GLCC_ALL_2015)
								{
									bReqClass = TRUE;
									strClass.Format ( "%s", ID2GAMEWORD("ITEM_ALL_CLASS"));
								}
								else if (dwReqCharClass==GLCC_MAN)
								{
									bReqClass = TRUE;
									strClass.Format ( "%s", ID2GAMEWORD("ITEM_ALL_CLASS",1));
								}
								else if (dwReqCharClass==GLCC_WOMAN)
								{
									bReqClass = TRUE;
									strClass.Format ( "%s", ID2GAMEWORD("ITEM_ALL_CLASS",2));
								}
								else if (dwReqCharClass == dwCLASSFIVE)
								{
									bReqClass = TRUE;
									strClass.Format ( "%s", ID2GAMEWORD("ITEM_ALL_CLASS",0));
								}
								else
								{
									bReqClass = TRUE;
									//Brawler
									if ((dwReqCharClass&GLCC_FIGHTER_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_FIGHTER_M)].c_str());
									if ((dwReqCharClass&GLCC_FIGHTER_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_FIGHTER_W)].c_str());
									//Swordsman
									if ((dwReqCharClass&GLCC_ARMS_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARMS_M)].c_str());
									if ((dwReqCharClass&GLCC_ARMS_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARMS_W)].c_str());
									//Archer
									if ((dwReqCharClass&GLCC_ARCHER_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_M)].c_str());
									if ((dwReqCharClass&GLCC_ARCHER_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_W)].c_str());
									//Shaman
									if ((dwReqCharClass&GLCC_SPIRIT_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_SPIRIT_M)].c_str());
									if ((dwReqCharClass&GLCC_SPIRIT_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_SPIRIT_W)].c_str());
									//Extreme
									if ((dwReqCharClass&GLCC_EXTREME_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_M)].c_str());
									if ((dwReqCharClass&GLCC_EXTREME_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_W)].c_str());
									//Scientist
									if ((dwReqCharClass&GLCC_SCIENCE_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_SCIENCE_M)].c_str());
									if ((dwReqCharClass&GLCC_SCIENCE_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_SCIENCE_W)].c_str());
									//Assassin
									if ((dwReqCharClass&GLCC_ASSASIN_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASIN_M)].c_str());
									if ((dwReqCharClass&GLCC_ASSASIN_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASIN_W)].c_str());
									//Tricker
									if ((dwReqCharClass&GLCC_TESTING_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_TESTING_M)].c_str());
									if ((dwReqCharClass&GLCC_TESTING_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_TESTING_W)].c_str());
									//Shaper
									if ((dwReqCharClass&GLCC_SHAPER_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_SHAPER_M)].c_str());
									if ((dwReqCharClass&GLCC_SHAPER_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_SHAPER_W)].c_str());
								
									if (	(dwReqCharClass&GLCC_FIGHTER_M)		&&
											(dwReqCharClass&GLCC_ARMS_M)  		&&
											(dwReqCharClass&GLCC_ARCHER_M)  	&&
											(dwReqCharClass&GLCC_SPIRIT_M)  	&&
											(dwReqCharClass&GLCC_EXTREME_M) 	&&
											(dwReqCharClass&GLCC_SCIENCE_M) 	&&
											(dwReqCharClass&GLCC_ASSASIN_M) 	&&
											(dwReqCharClass&GLCC_TESTING_M) 	&&
											(dwReqCharClass&GLCC_SHAPER_M)			)
									{
										strClass.Format( "%s", ID2GAMEWORD("ITEM_CLASS_GENDER",0));
									}
									else if ((dwReqCharClass&GLCC_FIGHTER_W) 	&&
											(dwReqCharClass&GLCC_ARMS_W)  		&&
											(dwReqCharClass&GLCC_ARCHER_W)  	&&	
											(dwReqCharClass&GLCC_SPIRIT_W)  	&&	
											(dwReqCharClass&GLCC_EXTREME_W)  	&&
											(dwReqCharClass&GLCC_SCIENCE_W)  	&&
											(dwReqCharClass&GLCC_ASSASIN_W)  	&&	
											(dwReqCharClass&GLCC_TESTING_W)  	&&	
											(dwReqCharClass&GLCC_SHAPER_W)  		)
									{
										strClass.Format( "%s", ID2GAMEWORD("ITEM_CLASS_GENDER",1));
									}
								}
							}
						}

						strSchoolText.Format ( "%s", strSchool );
						strClassText.Format ( "%s", strClass );

						CString strCombine;
						if ( bReqSchool )
						{
							CString strCombine;
							BOOL bSCHOOLVALID;
							const GLCHARLOGIC& sCharData = GLGaeaClient::GetInstance().GetCharacterLogic();
							EMSCHOOLFLAG emSCHOOL = index2school(sCharData.m_wSchool);
							DWORD dwReqSchool = pItemData->sBasicOp.dwReqSchool;

							if( sItemCustom.nidDISGUISE != NATIVEID_NULL() )
							{
								SITEM* pItemDisguise = GLItemMan::GetInstance().GetItem ( sItemCustom.nidDISGUISE );
								DWORD dwReqSchool_B = pItemDisguise->sBasicOp.dwReqSchool;
								bSCHOOLVALID =	(dwReqSchool & index2school(pCharacter->m_wSchool) &&
																							 dwReqSchool_B & index2school(pCharacter->m_wSchool)) ? TRUE : FALSE;
							}
							else bSCHOOLVALID = dwReqSchool & index2school(pCharacter->m_wSchool);

							if ( bNeedDisguise && bReqClass )
							{
								if( sItemCustom.nidDISGUISE != NATIVEID_NULL() ) //With Costume
								{
									CBasicTextBox::MULTITEXT mlText;
									SITEM* pItemDisguise = GLItemMan::GetInstance().GetItem ( sItemCustom.nidDISGUISE );
									strCombine.Format( "             %s", strSchoolText );
									mlText.strText[0] = strCombine;
									mlText.strText[1] = " / ";
									mlText.strText[2] = strClassText;
									mlText.dwColor[0] = NS_UITEXTCONTROL::GetEvaluateColor(bSCHOOLVALID);
									mlText.dwColor[1] = NS_UITEXTCOLOR::WHITE;
									mlText.dwColor[2] = NS_UITEXTCONTROL::GetEvaluateColor((pItemData->sBasicOp.dwReqCharClass&pCharacter->m_emClass)&& 
																						   (pItemDisguise->sBasicOp.dwReqCharClass&pCharacter->m_emClass));
									AddMultiText( mlText );
								}
								else
								{
									CBasicTextBox::MULTITEXT mlText;
									strCombine.Format( "             %s", strSchoolText );
									mlText.strText[0] = strCombine;
									mlText.strText[1] = " / ";
									mlText.strText[2] = strClassText;
									mlText.dwColor[0] = NS_UITEXTCONTROL::GetEvaluateColor(bSCHOOLVALID);
									mlText.dwColor[1] = NS_UITEXTCOLOR::WHITE;
									mlText.dwColor[2] = NS_UITEXTCONTROL::GetEvaluateColor((pItemData->sBasicOp.dwReqCharClass&pCharacter->m_emClass));
									AddMultiText( mlText );
								}
							}
							else if ( !bNeedDisguise && bReqClass )
							{
								CBasicTextBox::MULTITEXT mlText;
								strCombine.Format( "             %s ", strSchoolText );
								mlText.strText[0] = strCombine;
								mlText.strText[1] = " / ";
								mlText.strText[2] = strClassText;
								mlText.dwColor[0] = NS_UITEXTCONTROL::GetEvaluateColor(bSCHOOLVALID);
								mlText.dwColor[1] = NS_UITEXTCOLOR::WHITE;
								mlText.dwColor[2] = NS_UITEXTCONTROL::GetEvaluateColor((pItemData->sBasicOp.dwReqCharClass&pCharacter->m_emClass));
								AddMultiText( mlText );
							}
							else if ( !bNeedDisguise && !bReqClass )
							{
								CBasicTextBox::MULTITEXT mlText;
								strCombine.Format( "             %s ", strSchoolText );
								mlText.strText[0] = strCombine;
								mlText.dwColor[0] = NS_UITEXTCONTROL::GetEvaluateColor( (pItemData->sBasicOp.dwReqSchool & index2school(pCharacter->m_wSchool)));
								AddMultiText( mlText );
							}

						}

						{
							if ( pItemData->sBasicOp.wReqLevelDW || pItemData->sBasicOp.wReqLevelUP )
							{
								bool bReqLevel = true;
								strText.Format( "             %s %d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION", 0), pItemData->sBasicOp.wReqLevelDW );
													
								if ( pItemData->sBasicOp.wReqLevelUP )
								{
									strText2.Format ( "~%d",pItemData->sBasicOp.wReqLevelUP );
									strText += strText2; 
									bReqLevel = (pItemData->sBasicOp.wReqLevelUP >= pCharacter->m_wLevel);
								}

								AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pItemData->sBasicOp.wReqLevelDW <= pCharacter->m_wLevel && bReqLevel ) );
							}
							else
							{
								AddTextNoSplit ( "", NS_UITEXTCOLOR::ORANGE );		
							}
						}

						{
							CString strText;
							if ( 0 != rItemStats.dwPow )
							{
								strText.Format( "%s %d ", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 0 ), rItemStats.dwPow );
								AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.dwPow <= rCharStats.dwPow ) );
							}
							if ( 0 != rItemStats.dwStr )
							{
								strText.Format("%s %d ", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 1 ), rItemStats.dwStr );
								AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.dwStr <= rCharStats.dwStr ) );
							}
							if ( 0 != rItemStats.dwSpi)
							{
								strText.Format("%s %d ", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 2 ), rItemStats.dwSpi );
								AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.dwSpi <= rCharStats.dwSpi ) );
							}
							if ( 0 != rItemStats.dwDex )
							{
								strText.Format("%s %d ", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 3 ), rItemStats.dwDex );
								AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.dwDex <= rCharStats.dwDex ) );
							}
							if ( 0 != rItemStats.dwInt  )
							{
								strText.Format("%s %d ", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 4 ), rItemStats.dwInt );
								AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.dwInt <= rCharStats.dwInt ) );
							}
							if ( 0 != rItemStats.dwSta )
							{
								strText.Format("%s %d ", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION_STATS", 5 ), rItemStats.dwSta );
								AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( rItemStats.dwSta <= rCharStats.dwSta ) );
							}
						}
						
						if ( 0 != pItemData->sBasicOp.dwRebornReq )
						{
							CString strText;
							strText.Format( "%s %d ", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION", 3), pItemData->sBasicOp.dwRebornReq );
							AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pItemData->sBasicOp.dwRebornReq <= pCharacter->m_dwReborn  ) );
						}

						{
							CString strText, strTextRange;
							if ( pItemData->sSuitOp.emSuit == SUIT_HANDHELD )
							{
								if ( pItemData->sSuitOp.emAttack != ITEMATT_NOTHING )
								{
									DWORD dwLevel = (DWORD)pItemData->sBasicOp.emLevel;

									D3DCOLOR dwCOLOR;
									if ( pItemData->sBasicOp.emLevel==LEVEL_RARE)
									{
										strText.Format("[Rare] Weapon Type(%s)", COMMENT::ITEMATTACK[pItemData->sSuitOp.emAttack].c_str());
										AddTextNoSplit (strText, NS_UITEXTCOLOR::RARE );
										AddTextNoSplit ( "", NS_UITEXTCOLOR::WHITE );
									}
									else if ( pItemData->sBasicOp.emLevel==LEVEL_HIGH_CLASS)
									{
										strText.Format("[High Class] Weapon Type(%s)", COMMENT::ITEMATTACK[pItemData->sSuitOp.emAttack].c_str());
										AddTextNoSplit (strText, NS_UITEXTCOLOR::HIGHCLASS );
										AddTextNoSplit ( "", NS_UITEXTCOLOR::WHITE );
									}
									else if ( pItemData->sBasicOp.emLevel==LEVEL_HEROIC)
									{
										strText.Format("[Heroic] Weapon Type(%s)", COMMENT::ITEMATTACK[pItemData->sSuitOp.emAttack].c_str());
										AddTextNoSplit (strText, NS_UITEXTCOLOR::HEROIC );
										AddTextNoSplit ( "", NS_UITEXTCOLOR::WHITE );
									}
									else if ( pItemData->sBasicOp.emLevel==LEVEL_ANCIENT)
									{
										strText.Format("[Ancient] Weapon Type(%s)", COMMENT::ITEMATTACK[pItemData->sSuitOp.emAttack].c_str());
										AddTextNoSplit (strText, NS_UITEXTCOLOR::ANCIENT );
										AddTextNoSplit ( "", NS_UITEXTCOLOR::WHITE );
									}
									else if ( pItemData->sBasicOp.emLevel==LEVEL_LEGENDARY)
									{
										strText.Format("[Legendary] Weapon Type(%s)", COMMENT::ITEMATTACK[pItemData->sSuitOp.emAttack].c_str());
										AddTextNoSplit (strText, NS_UITEXTCOLOR::LEGENDARY );
										AddTextNoSplit ( "", NS_UITEXTCOLOR::WHITE );
									}
									else if ( pItemData->sBasicOp.emLevel==LEVEL_VOTE_ITEM)
									{
										strText.Format("[Vote] Weapon Type(%s)", COMMENT::ITEMATTACK[pItemData->sSuitOp.emAttack].c_str());
										AddTextNoSplit (strText, NS_UITEXTCOLOR::VOTEITEM );
										AddTextNoSplit ( "", NS_UITEXTCOLOR::WHITE );
									}
									else if ( pItemData->sBasicOp.emLevel==LEVEL_PREMIUM_ITEM)
									{
										strText.Format("[Premium] Weapon Type(%s)", COMMENT::ITEMATTACK[pItemData->sSuitOp.emAttack].c_str());
										AddTextNoSplit (strText, NS_UITEXTCOLOR::PREMIUMITEM );
										AddTextNoSplit ( "", NS_UITEXTCOLOR::WHITE );
									}
									else if ( pItemData->sBasicOp.emLevel==LEVEL_CONTRIBUTION_ITEM)
									{
										strText.Format("[Contribution] Weapon Type(%s)", COMMENT::ITEMATTACK[pItemData->sSuitOp.emAttack].c_str());
										AddTextNoSplit (strText, NS_UITEXTCOLOR::LITEBLUE );
										AddTextNoSplit ( "", NS_UITEXTCOLOR::WHITE );
									}
									else
									{
										strText.Format("[Normal] Weapon Type(%s)", COMMENT::ITEMATTACK[pItemData->sSuitOp.emAttack].c_str());
										AddTextNoSplit (strText, NS_UITEXTCOLOR::NORMAL );
										AddTextNoSplit ( "", NS_UITEXTCOLOR::WHITE );
									}

								}
								
								if ( sItemCustom.GETATTRANGE() )
								{
									strTextRange.Format("%s :%dm",ID2GAMEWORD("ITEM_ADVANCED_INFO", 2), sItemCustom.GETATTRANGE() );
									AddTextNoSplit ( strTextRange, NS_UITEXTCOLOR::WHITE );
								}
							}
							else
							{
								DWORD dwLevel = (DWORD)pItemData->sBasicOp.emLevel;

								D3DCOLOR dwCOLOR;

									if ( pItemData->sBasicOp.emLevel==LEVEL_RARE)
									{
										AddTextNoSplit ( "", NS_UITEXTCOLOR::WHITE );
										strText.Format("[Rare] %s", COMMENT::ITEMSUIT[pItemData->sSuitOp.emSuit].c_str() );
										AddTextNoSplit (strText, NS_UITEXTCOLOR::RARE );
										AddTextNoSplit ( "", NS_UITEXTCOLOR::WHITE );
									}
									else if ( pItemData->sBasicOp.emLevel==LEVEL_HIGH_CLASS)
									{
										AddTextNoSplit ( "", NS_UITEXTCOLOR::WHITE );
										strText.Format("[High Class] %s", COMMENT::ITEMSUIT[pItemData->sSuitOp.emSuit].c_str() );
										AddTextNoSplit (strText, NS_UITEXTCOLOR::HIGHCLASS );
										AddTextNoSplit ( "", NS_UITEXTCOLOR::WHITE );
									}
									else if ( pItemData->sBasicOp.emLevel==LEVEL_HEROIC)
									{
										AddTextNoSplit ( "", NS_UITEXTCOLOR::WHITE );
										strText.Format("[Heroic] %s", COMMENT::ITEMSUIT[pItemData->sSuitOp.emSuit].c_str() );
										AddTextNoSplit (strText, NS_UITEXTCOLOR::HEROIC );
										AddTextNoSplit ( "", NS_UITEXTCOLOR::WHITE );
									}
									else if ( pItemData->sBasicOp.emLevel==LEVEL_ANCIENT)
									{
										AddTextNoSplit ( "", NS_UITEXTCOLOR::WHITE );
										strText.Format("[Ancient] %s", COMMENT::ITEMSUIT[pItemData->sSuitOp.emSuit].c_str() );
										AddTextNoSplit (strText, NS_UITEXTCOLOR::ANCIENT );
										AddTextNoSplit ( "", NS_UITEXTCOLOR::WHITE );
									}
									else if ( pItemData->sBasicOp.emLevel==LEVEL_LEGENDARY)
									{
										AddTextNoSplit ( "", NS_UITEXTCOLOR::WHITE );
										strText.Format("[Legendary] %s", COMMENT::ITEMSUIT[pItemData->sSuitOp.emSuit].c_str() );
										AddTextNoSplit (strText, NS_UITEXTCOLOR::LEGENDARY );
										AddTextNoSplit ( "", NS_UITEXTCOLOR::WHITE );
									}
									else if ( pItemData->sBasicOp.emLevel==LEVEL_VOTE_ITEM)
									{
										AddTextNoSplit ( "", NS_UITEXTCOLOR::WHITE );
										strText.Format("[Vote] %s", COMMENT::ITEMSUIT[pItemData->sSuitOp.emSuit].c_str() );
										AddTextNoSplit (strText, NS_UITEXTCOLOR::VOTEITEM );
										AddTextNoSplit ( "", NS_UITEXTCOLOR::WHITE );
									}
									else if ( pItemData->sBasicOp.emLevel==LEVEL_PREMIUM_ITEM)
									{
										AddTextNoSplit ( "", NS_UITEXTCOLOR::WHITE );
										strText.Format("[Premium] %s", COMMENT::ITEMSUIT[pItemData->sSuitOp.emSuit].c_str() );
										AddTextNoSplit (strText, NS_UITEXTCOLOR::PREMIUMITEM );
										AddTextNoSplit ( "", NS_UITEXTCOLOR::WHITE );
									}
									else if ( pItemData->sBasicOp.emLevel==LEVEL_CONTRIBUTION_ITEM)
									{
										AddTextNoSplit ( "", NS_UITEXTCOLOR::WHITE );
										strText.Format("[Contribution] %s", COMMENT::ITEMSUIT[pItemData->sSuitOp.emSuit].c_str() );
										AddTextNoSplit (strText, NS_UITEXTCOLOR::LITEBLUE );
										AddTextNoSplit ( "", NS_UITEXTCOLOR::WHITE );
									}
									else
									{
										AddTextNoSplit ( "", NS_UITEXTCOLOR::WHITE );
										strText.Format("[Normal] %s", COMMENT::ITEMSUIT[pItemData->sSuitOp.emSuit].c_str() );
										AddTextNoSplit (strText, NS_UITEXTCOLOR::NORMAL );
										AddTextNoSplit ( "", NS_UITEXTCOLOR::WHITE );
									}
							}
						}

						{
							BYTE uGRADE = 0;
							
							if ( pItemData->sSuitOp.emSuit == SUIT_HANDHELD)
							{
								GLPADATA &sDamage = pItemData->sSuitOp.gdDamage;
								nExtraValue = sItemCustom.GETGRADE_DAMAGE();
								int nValue = 0;
								uGRADE = sItemCustom.GETGRADE(EMGRINDING_DAMAGE);
								CBasicTextBox::MULTITEXT mlText;
								CString strCombine;
								bool bRandExist = false;
								
								if( sItemCustom.IsSetRandOpt())
								{
									bRandExist = true;

									INT nOpt1 = sItemCustom.GETOptTYPE1();
									INT nOpt2 = sItemCustom.GETOptTYPE2();
									INT nOpt3 = sItemCustom.GETOptTYPE3();
									INT nOpt4 = sItemCustom.GETOptTYPE4();

									if ( nOpt1 && nOpt1 == EMR_OPT_DAMAGE_RATE )
									{
										float fVal = sItemCustom.GETOptVALUE1();
										if ( fVal > 0 )
											nValue = int((100.0f+fVal)*0.01f);
									}

									if ( nOpt2 && nOpt2 == EMR_OPT_DAMAGE_RATE )
									{
										float fVal = sItemCustom.GETOptVALUE2();
										if ( fVal > 0 )
											nValue = int((100.0f+fVal)*0.01f);
									}

									if ( nOpt3 && nOpt3 == EMR_OPT_DAMAGE_RATE )
									{
										float fVal = sItemCustom.GETOptVALUE3();
										if ( fVal > 0 )
											nValue = int((100.0f+fVal)*0.01f);
									}

									if ( nOpt4 && nOpt4 == EMR_OPT_DAMAGE_RATE )
									{
										float fVal = sItemCustom.GETOptVALUE4();
										if ( fVal > 0 )
											nValue = int((100.0f+fVal)*0.01f);
									}

									if ( nOpt1 && nOpt1 == EMR_OPT_DAMAGE )
									{
										float fVal = sItemCustom.GETOptVALUE4();
										if ( fVal > 0 )
											nValue += int(fVal);
									}

									if ( nOpt2 && nOpt2 == EMR_OPT_DAMAGE )
									{
										float fVal = sItemCustom.GETOptVALUE2();
										if ( fVal > 0 )
											nValue += int(fVal);
									}

									if ( nOpt3 && nOpt3 == EMR_OPT_DAMAGE )
									{
										float fVal = sItemCustom.GETOptVALUE3();
										if ( fVal > 0 )
											nValue += int(fVal);
									}

									if ( nOpt4 && nOpt4 == EMR_OPT_DAMAGE )
									{
										float fVal = sItemCustom.GETOptVALUE4();
										if ( fVal > 0 )
											nValue += int(fVal);
									}
								}

								CString strVAL, strVALIDATE, strMax;

								strCombine.Format("%s :%d", ID2GAMEWORD("ITEM_ADVANCED_INFO",0), sDamage.dwLow);
								strMax.Format( " ~ %d", sDamage.dwHigh );

								if ( !nExtraValue && !bRandExist )
								{
									mlText.strText[0] = strCombine;
									mlText.dwColor[0] = NS_UITEXTCOLOR::WHITE;

									mlText.strText[1] = strMax;
									mlText.dwColor[1] = NS_UITEXTCOLOR::WHITE;
								}
								else if ( nExtraValue && !bRandExist )
								{
									mlText.strText[0] = strCombine;
									mlText.dwColor[0] = NS_UITEXTCOLOR::WHITE;

									strVALIDATE = nExtraValue > 0 ? "+" : "-";
									strVAL.Format("(%s%d)", strVALIDATE, nExtraValue); 
									mlText.strText[1] = strVAL;
									mlText.dwColor[1] = NS_UITEXTCOLOR::YELLOW;
									
									mlText.strText[2] = strMax;
									mlText.dwColor[2] = NS_UITEXTCOLOR::WHITE;
									
									mlText.strText[3] = strVAL;
									mlText.dwColor[3] = NS_UITEXTCOLOR::YELLOW;
                                    
									if ( uGRADE > 0 )
									{
								    CString Upgrade;
								    Upgrade.Format ( "+%d",uGRADE );
									mlText.strText[6] = "[";
									mlText.strText[7] = Upgrade;
									mlText.strText[8] = "]";
									mlText.dwColor[6] = NS_UITEXTCOLOR::WHITE;
									mlText.dwColor[7] = NS_UITEXTCOLOR::YELLOW;
									mlText.dwColor[8] = NS_UITEXTCOLOR::WHITE;
									}
								}
								else if ( !nExtraValue && bRandExist )
								{
									mlText.strText[0] = strCombine;
									mlText.dwColor[0] = NS_UITEXTCOLOR::WHITE;
									
									strVAL.Format("(+%d)", nValue);
									mlText.strText[1] = strVAL;
									mlText.dwColor[1] = NS_UITEXTCOLOR::AQUA;
									
									mlText.strText[2] = strMax;
									mlText.dwColor[2] = NS_UITEXTCOLOR::WHITE;
									
									mlText.strText[3] = strVAL;
									mlText.dwColor[3] = NS_UITEXTCOLOR::AQUA;
								}
								else if ( nExtraValue && bRandExist )
								{
									mlText.strText[0] = strCombine;
									mlText.dwColor[0] = NS_UITEXTCOLOR::WHITE;
									
									strVALIDATE = nExtraValue > 0 ? "+" : "-";
									strVAL.Format("(%s%d)", strVALIDATE, nExtraValue); 
									mlText.strText[1] = strVAL;
									mlText.dwColor[1] = NS_UITEXTCOLOR::YELLOW;
									
									CString strVAL2;
									strVAL2.Format("(+%d)", nValue);
									mlText.strText[2] = strVAL2;
									mlText.dwColor[2] = NS_UITEXTCOLOR::AQUA;
									
									mlText.strText[3] = strMax;
									mlText.dwColor[3] = NS_UITEXTCOLOR::WHITE;
									
									mlText.strText[4] = strVAL;
									mlText.dwColor[4] = NS_UITEXTCOLOR::YELLOW;
									
									mlText.strText[5] = strVAL2;
									mlText.dwColor[5] = NS_UITEXTCOLOR::AQUA;
									if ( uGRADE > 0 )
									{
								    CString Upgrade;
								    Upgrade.Format ( "+%d",uGRADE );
									mlText.strText[6] = "[";
									mlText.strText[7] = Upgrade;
									mlText.strText[8] = "]";
									mlText.dwColor[6] = NS_UITEXTCOLOR::WHITE;
									mlText.dwColor[7] = NS_UITEXTCOLOR::YELLOW;
									mlText.dwColor[8] = NS_UITEXTCOLOR::WHITE;
									}


								}
								AddMultiText ( mlText );
							}

							nExtraValue = sItemCustom.GETGRADE_DAMAGE();
							if ( nExtraValue )
							{
								uGRADE = sItemCustom.GETGRADE(EMGRINDING_DAMAGE);
								strText.Format ( "%s :+%d", ID2GAMEWORD("ITEM_ADVANCED_INFO", 8), nExtraValue );
								APPEND_ITEM_GRADE ( strText, uGRADE );
								AddTextNoSplit ( strText, NS_UITEXTCOLOR::WHITE );
							}
                             
							short nDefense = sItemCustom.getdefense();
							nExtraValue = sItemCustom.GETGRADE_DEFENSE();
							uGRADE = sItemCustom.GETGRADE(EMGRINDING_DEFENSE);
                            BOOL bRandExist = false;

							if ( pItemData->sSuitOp.emSuit != SUIT_HANDHELD )
							{
							int nValue = 0;
							CString strText;
							CBasicTextBox::MULTITEXT mlText;
							strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO",1), nDefense);
                            CString strVAL, strVALIDATE;
							

								if( sItemCustom.IsSetRandOpt())
								{
									bRandExist = true;

									INT nOpt1 = sItemCustom.GETOptTYPE1();
									INT nOpt2 = sItemCustom.GETOptTYPE2();
									INT nOpt3 = sItemCustom.GETOptTYPE3();
									INT nOpt4 = sItemCustom.GETOptTYPE4();

									if ( nOpt1 && nOpt1 == EMR_OPT_DEFENSE_RATE )
									{
										float fVal = sItemCustom.GETOptVALUE1();
										if ( fVal > 0 )
											nValue = int((100.0f+fVal)*0.01f);
									}

									if ( nOpt2 && nOpt2 == EMR_OPT_DEFENSE_RATE )
									{
										float fVal = sItemCustom.GETOptVALUE2();
										if ( fVal > 0 )
											nValue = int((100.0f+fVal)*0.01f);
									}

									if ( nOpt3 && nOpt3 == EMR_OPT_DEFENSE_RATE )
									{
										float fVal = sItemCustom.GETOptVALUE3();
										if ( fVal > 0 )
											nValue = int((100.0f+fVal)*0.01f);
									}

									if ( nOpt4 && nOpt4 == EMR_OPT_DEFENSE_RATE )
									{
										float fVal = sItemCustom.GETOptVALUE4();
										if ( fVal > 0 )
											nValue = int((100.0f+fVal)*0.01f);
									}

									if ( nOpt1 && nOpt1 == EMR_OPT_DEFENSE )
									{
										float fVal = sItemCustom.GETOptVALUE4();
										if ( fVal > 0 )
											nValue += int(fVal);
									}

									if ( nOpt2 && nOpt2 == EMR_OPT_DEFENSE )
									{
										float fVal = sItemCustom.GETOptVALUE2();
										if ( fVal > 0 )
											nValue += int(fVal);
									}

									if ( nOpt3 && nOpt3 == EMR_OPT_DEFENSE )
									{
										float fVal = sItemCustom.GETOptVALUE3();
										if ( fVal > 0 )
											nValue += int(fVal);
									}

									if ( nOpt4 && nOpt4 == EMR_OPT_DEFENSE )
									{
										float fVal = sItemCustom.GETOptVALUE4();
										if ( fVal > 0 )
											nValue += int(fVal);
									}
								}
								if ( !nExtraValue && !bRandExist )
								{
									mlText.strText[0] = strText;
									mlText.dwColor[0] = NS_UITEXTCOLOR::WHITE;

								}
								else if ( nExtraValue && !bRandExist )
								{
									mlText.strText[0] = strText;
									mlText.dwColor[0] = NS_UITEXTCOLOR::WHITE;

									strVALIDATE = nExtraValue > 0 ? "+" : "-";
									strVAL.Format("(%s%d)", strVALIDATE, nExtraValue); 

									mlText.strText[1] = strVAL;
									mlText.dwColor[1] = NS_UITEXTCOLOR::YELLOW;		

									if ( uGRADE > 0 )
									{
									CString Upgrade;
								    Upgrade.Format ( "+%d",uGRADE );
									mlText.strText[6] = "[";
									mlText.strText[7] = Upgrade;
									mlText.strText[8] = "]";
									mlText.dwColor[6] = NS_UITEXTCOLOR::WHITE;
									mlText.dwColor[7] = NS_UITEXTCOLOR::YELLOW;
									mlText.dwColor[8] = NS_UITEXTCOLOR::WHITE;
									}
									
								}
								else if ( !nExtraValue && bRandExist )
								{
									mlText.strText[0] = strText;
									mlText.dwColor[0] = NS_UITEXTCOLOR::WHITE;
									
									strVAL.Format("(+%d)", nValue);
									mlText.strText[1] = strVAL;
									mlText.dwColor[1] = NS_UITEXTCOLOR::AQUA;
										
								}
								else if ( nExtraValue && bRandExist )
								{
									mlText.strText[0] = strText;
									mlText.dwColor[0] = NS_UITEXTCOLOR::WHITE;
									
									strVALIDATE = nExtraValue > 0 ? "+" : "-";
									strVAL.Format("(%s%d)", strVALIDATE, nExtraValue); 
									mlText.strText[1] = strVAL;
									mlText.dwColor[1] = NS_UITEXTCOLOR::YELLOW;
									
									CString strVAL2;
									strVAL2.Format("(+%d)", nValue);
									mlText.strText[2] = strVAL2;
									mlText.dwColor[2] = NS_UITEXTCOLOR::AQUA;

									if ( uGRADE > 0 )
									{
									CString Upgrade;
								    Upgrade.Format ( "+%d",uGRADE );
									mlText.strText[6] = "[";
									mlText.strText[7] = Upgrade;
									mlText.strText[8] = "]";
									mlText.dwColor[6] = NS_UITEXTCOLOR::WHITE;
									mlText.dwColor[7] = NS_UITEXTCOLOR::YELLOW;
									mlText.dwColor[8] = NS_UITEXTCOLOR::WHITE;
									}
									
								}

								AddMultiText ( mlText );
							}

							if ( sItemCustom.GETHITRATE() )
							{
								nExtraValue = 0;
								AddInfoItemAddon ( sItemCustom.GETHITRATE(), nExtraValue, ID2GAMEWORD("ITEM_ADVANCED_INFO", 3) );		
							}

							if ( sItemCustom.GETAVOIDRATE() )
							{
								nExtraValue = 0;
								AddInfoItemAddon ( sItemCustom.GETAVOIDRATE(), nExtraValue, ID2GAMEWORD("ITEM_ADVANCED_INFO", 4) );
							}

							const WORD wReqSP = sItemCustom.GETREQ_SP();
							if ( 0 < wReqSP )
							{
								strText.Format("%s :%d", ID2GAMEWORD("ITEM_ADVANCED_INFO", 7), wReqSP );
								AddTextNoSplit ( strText, NS_UITEXTCOLOR::WHITE  );
							}

							bValue = TRUE;
							strText.Format ( "%s", COMMENT::BRIGHT[pItemData->sBasicOp.emReqBright].c_str() );
							if ( pItemData->sBasicOp.emReqBright != BRIGHT_BOTH )
							{
								if ( pCharacter->GETBRIGHT() != pItemData->sBasicOp.emReqBright )
								{
									bValue = FALSE;
								}
							}
							AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( bValue ) );					

							if ( pItemData->sBasicOp.wReqPA )
							{
								strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION", 1), pItemData->sBasicOp.wReqPA );
								AddTextNoSplit ( strText, NS_UITEXTCONTROL::GetEvaluateColor ( pItemData->sBasicOp.wReqPA <= pCharacter->m_wSUM_PA ) );
							}
							if ( pItemData->sBasicOp.wReqSA )
							{
								strText.Format("%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION", 2 ), pItemData->sBasicOp.wReqSA );
								AddTextNoSplit(strText,NS_UITEXTCONTROL::GetEvaluateColor ( pItemData->sBasicOp.wReqSA <= pCharacter->m_wSUM_SA ));
							}
						}
						
						bool bCanPreview = CInnerInterface::GetInstance().PreviewCheckSimple( pItemData->sBasicOp.sNativeID );
						if ( bCanPreview )
						{
							CBasicTextBox::MULTITEXT mlText;
							mlText.strText[0] = ID2GAMEWORD( "ITEM_USEPREVIEW", 0 );
							mlText.dwColor[0] = NS_UITEXTCOLOR::WHITE;
							AddMultiTextLongestLineSplit( mlText );
						}
				};
				};
			}

			AddTextNoSplit( "", NS_UITEXTCOLOR::BLACK );
			if ( sItemCustom.nidDISGUISE!=SNATIVEID(false) )
			{
				SITEM* pDisguiseData = GLItemMan::GetInstance().GetItem ( sItemCustom.nidDISGUISE );

				strText.Format ( "%s :(%s)", ID2GAMEWORD ( "ITEM_BASIC_INFO", 3 ), pDisguiseData->GetName() );			
				AddTextNoSplit ( strText, NS_UITEXTCOLOR::GREENYELLOW );
			}

			if ( sItemCustom.tDISGUISE!=0 )
			{				
				CTime cTime(sItemCustom.tDISGUISE);
				if ( cTime.GetYear()!=1970 )
				{
					CString strExpireDate;
					strExpireDate = CInnerInterface::GetInstance().MakeString( ID2GAMEWORD("ITEM_EXPIRE_DATE"),
																				(cTime.GetYear ()%2000), 
																				cTime.GetMonth (), 
																				cTime.GetDay (), 
																				cTime.GetHour (), 
																				cTime.GetMinute () );

					strText.Format ( "%s:%s", ID2GAMEWORD ( "ITEM_BASIC_INFO", 7 ), strExpireDate );			
					AddTextNoSplit ( strText, NS_UITEXTCOLOR::DARKORANGE );
				}
			}

			DWORD dwCOMMISSION_MONEY = 0;

			AddItemTurnInfo ( sItemCustom, bInMarket, bInPrivateMarket );

			if ( pItemData->sBasicOp.IsCoolTime() )
			{
				CString strTime = "";
				CTimeSpan cCoolTime( pItemData->sBasicOp.dwCoolTime );
				
				if ( cCoolTime.GetHours() > 0 )	
					strTime += CInnerInterface::GetInstance().MakeString( "%d%s ", cCoolTime.GetHours(), ID2GAMEWORD ( "ITEM_BASIC_TIME", 0 ) );
				if ( cCoolTime.GetMinutes() > 0 )	
					strTime += CInnerInterface::GetInstance().MakeString( "%d%s ", cCoolTime.GetMinutes(), ID2GAMEWORD ( "ITEM_BASIC_TIME", 1 ) );
				if ( cCoolTime.GetSeconds() > 0 )	
					strTime += CInnerInterface::GetInstance().MakeString( "%d%s ", cCoolTime.GetSeconds(), ID2GAMEWORD ( "ITEM_BASIC_TIME", 2 ) );

				strText.Format ( "%s : %s", ID2GAMEWORD ( "ITEM_BASIC_INFO", 8 ), strTime );			
				AddTextNoSplit ( strText, NS_UITEXTCOLOR::DARKORANGE );	
			}

			

			switch ( emItemType )
			{
			case ITEM_SUIT:
			case ITEM_REVIVE:
			case ITEM_ANTI_DISAPPEAR:
				{
					AddTextAddValue( sItemCustom, pItemData->sSuitOp );

					{
						BOOL bSPAC = sItemCustom.GETINCHP() || sItemCustom.GETINCMP() || sItemCustom.GETINCSP() || sItemCustom.GETINCAP();
						BOOL bVAR_HP(FALSE), bVAR_MP(FALSE), bVAR_SP(FALSE), bVAR_AP(FALSE);

						EMITEM_VAR emITEM_VAR = pItemData->sSuitOp.sVARIATE.emTYPE;
						EMITEM_VAR emITEM_VOL = pItemData->sSuitOp.sVOLUME.emTYPE;
						if ( (emITEM_VAR!=EMVAR_NONE) || (emITEM_VOL!=EMVAR_NONE) || bSPAC )
						{
						}
						
						if ( sItemCustom.nidDISGUISE!=SNATIVEID(false) )
						{
							SITEM* pJDisguiseData = GLItemMan::GetInstance().GetItem ( sItemCustom.nidDISGUISE );
						//	if ( pJDisguiseData )
						//	{
						//		if ( pJDisguiseData->sBasicOp.IsCostumeCombine() )
								{
									EMITEM_VAR emITEMCOS_VAR = pJDisguiseData->sSuitOp.sVARIATE.emTYPE;
									EMITEM_VAR emITEMCOS_VOL = pJDisguiseData->sSuitOp.sVOLUME.emTYPE;
								//add jitem

									float fVAR_SCALE(0);
									if ( emITEMCOS_VAR != EMVAR_NONE )
									{			
										
										fVAR_SCALE = pJDisguiseData->sSuitOp.sVARIATE.fVariate;
										CString strText2;
										fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEMCOS_VAR];
										strText2.Format("Costume ADD:%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEMCOS_VAR ), fVAR_SCALE );
										if ( COMMENT::IsITEMVAR_SCALE(emITEMCOS_VAR) )		strText2 += "%";
										AddTextNoSplit(strText2,NS_UITEXTCOLOR::GREEN);
									}

									if ( emITEMCOS_VOL != EMVAR_NONE)
									{
										CString strText3;
										float fVOLUME = pJDisguiseData->sSuitOp.sVOLUME.fVolume;
										strText3.Format("Costume ADD:%s %.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD_VOL", emITEMCOS_VOL ), fVOLUME );
										AddTextNoSplit(strText3,NS_UITEXTCOLOR::GREEN);
									}
								}
							}
						//}

						float fVAR_SCALE(0);
						if ( emITEM_VAR != EMVAR_NONE )
						{
							switch ( emITEM_VAR )
							{
							case EMVAR_HP:
								bVAR_HP = TRUE;
								fVAR_SCALE = sItemCustom.GETINCHP();
								break;
							case EMVAR_MP:
								bVAR_MP = TRUE;
								fVAR_SCALE = sItemCustom.GETINCMP();
								break;
							case EMVAR_SP:
								bVAR_SP = TRUE;
								fVAR_SCALE = sItemCustom.GETINCSP();
								break;
							case EMVAR_AP:
								bVAR_AP = TRUE;
								fVAR_SCALE = sItemCustom.GETINCAP();
								break;
							default:
								fVAR_SCALE = pItemData->sSuitOp.sVARIATE.fVariate;
								break;
							};

							fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
							strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
							if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::GREEN);
						}

						if ( !bVAR_HP && sItemCustom.GETINCHP() )
						{
							fVAR_SCALE = sItemCustom.GETINCHP();
							emITEM_VAR = EMVAR_HP;
							fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
							strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
							if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::GREEN);
						}

						if ( !bVAR_MP && sItemCustom.GETINCMP() )
						{
							fVAR_SCALE = sItemCustom.GETINCMP();
							emITEM_VAR = EMVAR_MP;
							fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
							strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
							if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::GREEN);
						}

						if ( !bVAR_SP && sItemCustom.GETINCSP() )
						{
							fVAR_SCALE = sItemCustom.GETINCSP();
							emITEM_VAR = EMVAR_SP;
							fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
							strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
							if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::GREEN);
						}

						if ( !bVAR_AP && sItemCustom.GETINCAP() )
						{
							fVAR_SCALE = sItemCustom.GETINCAP();
							emITEM_VAR = EMVAR_AP;
							fVAR_SCALE *= COMMENT::ITEMVAR_SCALE[emITEM_VAR];
							strText.Format("%s %2.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD", emITEM_VAR ), fVAR_SCALE );
							if ( COMMENT::IsITEMVAR_SCALE(emITEM_VAR) )		strText += "%";
							AddTextNoSplit(strText,NS_UITEXTCOLOR::GREEN);
						}

						float fExpMultiple = pItemData->GetExpMultiple();
						if( fExpMultiple != 1.0f )
						{
							strText.Format("%s :+%2.2f", ID2GAMEWORD( "ITEM_ADVANCED_INFO_SPECIAL_EX", 0 ), fExpMultiple);
							strText += "%";

							AddTextNoSplit(strText,NS_UITEXTCOLOR::GREEN);
						}

						if ( emITEM_VOL != EMVAR_NONE)
						{
							if ( emITEM_VOL == EMVAR_MOVE_SPEED )
							{
								float fVOLUME = sItemCustom.GETMOVESPEED();
								strText.Format("%s %.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD_VOL", emITEM_VOL ), fVOLUME );
								AddTextNoSplit(strText,NS_UITEXTCOLOR::GREEN);
							}
							else
							{
								float fVOLUME = pItemData->sSuitOp.sVOLUME.fVolume;
								strText.Format("%s %.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD_VOL", emITEM_VOL ), fVOLUME );
								AddTextNoSplit(strText,NS_UITEXTCOLOR::GREEN);
							}
							
						}
					}

					/*if( sItemCustom.IsSetRandOpt() )
					{
						CString strDescText;
						INT nRandOptType = sItemCustom.GETOptTYPE1();
						if( (EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );

							float fVal = sItemCustom.GETOptVALUE1();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::AQUA );
							}
						}

						nRandOptType = sItemCustom.GETOptTYPE2();
						if( (EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );

							float fVal = sItemCustom.GETOptVALUE2();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::AQUA );
							}
						}

						nRandOptType = sItemCustom.GETOptTYPE3();
						if( (EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );

							float fVal = sItemCustom.GETOptVALUE3();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::AQUA );
							}
						}

						nRandOptType = sItemCustom.GETOptTYPE4();
						if( (EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );

							float fVal = sItemCustom.GETOptVALUE4();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::AQUA );
							}
						}
					}*/

					for(int i = 0; i < 4; i++)
					{
						if ( !sItemCustom.IsSetRandOpt() ) 
							break;

						CString strDescText;
						EMRANDOM_OPT emOpt[4] = 
						{
							sItemCustom.GETOptTYPE1()
							, sItemCustom.GETOptTYPE2()
							, sItemCustom.GETOptTYPE3()
							, sItemCustom.GETOptTYPE4()
						};

						INT nRandOptType = emOpt[i];

						if( (EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE) )
						{
							float fVal[4] =
							{
								sItemCustom.GETOptVALUE1()
								, sItemCustom.GETOptVALUE2()
								, sItemCustom.GETOptVALUE3()
								, sItemCustom.GETOptVALUE4()
							};

							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );
							if( fVal[i] != 0.0f )
							{
								if ( nRandOptType == EMR_OPT_HMS_INC_RATE || 
									nRandOptType == EMR_OPT_HP_INC_RATE  || 
									nRandOptType == EMR_OPT_MP_INC_RATE  || 
									nRandOptType == EMR_OPT_SP_INC_RATE	 )
								{
									fVal[i] = (fVal[i]*100.0f);
								}

								

								if( fVal[i] > 0.0f) 
									strDescText += _T('+');

								if( sItemCustom.IsPerRandOpt( nRandOptType ) )
								{
									strDescText.AppendFormat( "%.2f", fVal[i] );
									strDescText += _T('%');
								}
								else if ( nRandOptType == EMR_OPT_HIT_RATE || 
									nRandOptType == EMR_OPT_AVOID_RATE 	 )
								{
									strDescText.AppendFormat( "%.2f", fVal[i] );
									strDescText += _T('%');
								}
								else
									strDescText.AppendFormat( "%d", int(fVal[i] ));

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::AQUA );
							}
						}
					}

					EMSTATE_BLOW emBLOW = pItemData->sSuitOp.sBLOW.emTYPE;
					if ( emBLOW !=EMBLOW_NONE )
					{
						CString strBlow;
						AddTextNoSplit("",NS_UITEXTCOLOR::BLACK);
						float fVAR1 = pItemData->sSuitOp.sBLOW.fVAR1 * COMMENT::BLOW_VAR1_SCALE[emBLOW];
						float fVAR2 = pItemData->sSuitOp.sBLOW.fVAR2 * COMMENT::BLOW_VAR2_SCALE[emBLOW];
						int nRATE = int(pItemData->sSuitOp.sBLOW.fRATE);
						int nLIFE = int(pItemData->sSuitOp.sBLOW.fLIFE);

						int nVAR1 = int(fVAR1);
						int nVAR2 = int(fVAR2);

						switch(emBLOW)
						{
						case EMBLOW_NUMB:
							{
								strBlow.Format( "%d%% chance Paralysis effect, decreased movement and attack speed %d for %d seconds.",
												nRATE, nVAR2, nRATE);
							}
							break;
						case EMBLOW_STUN:
							{
								strBlow.Format ( "%d%% chance Faint effect, unable to act for 1.5 seconds.",
										nRATE);			
							}
							break;
						case EMBLOW_STONE:
							{
								strBlow.Format ( "%d%% chance Fossil effect, decreased movement and attack speed %d for %d seconds.",
										nRATE, nVAR1, nLIFE);			
							}
							break;
						case EMBLOW_BURN:
							{
								strBlow.Format ( "%d%% chance Flame effect, %d seconds to give %d damage per second.",
										nRATE, nLIFE, nVAR2);			
							}
							break;
						case EMBLOW_FROZEN:
							{
								strBlow.Format ( "%d%% chance Frost effect, decreased %d movement and attack speed for %d seconds and give %d damage per second.",
										nRATE, nVAR1, nLIFE, nVAR2);			
							}
							break;
						case EMBLOW_MAD:
							{
								strBlow.Format ( "%d%% chance Confusion effect for %d seconds.",
										nRATE, nLIFE);			
							}
							break;
						case EMBLOW_POISON:
							{
								strBlow.Format ( "%d%% chance Intoxication effect, %d damage per second for %d seconds.",
										nRATE, nVAR2, nLIFE);			
							}
							break;
						case EMBLOW_CURSE:
							{
								strBlow.Format ( "%d%% chance Curse effect, %d seconds %d damage per second, MP cannot be restored",
										nRATE, nLIFE, nVAR2);			
							}
							break;
						};

						AddTextLongestLineSplit( strBlow, NS_UITEXTCOLOR::GOLD );
					}
				}
				break;

			case ITEM_CHARM:
				{					
					if ( pItemData->sBasicOp.emLevel == LEVEL_LEGENDARY )
					{
						AddTextNoSplit( "[Legendary] Arrow Item",NS_UITEXTCOLOR::LEGENDARY);
					}
					else if ( pItemData->sBasicOp.emLevel == LEVEL_ANCIENT )
					{
						AddTextNoSplit( "[Ancient] Arrow Item",NS_UITEXTCOLOR::ANCIENT);
					}
					else if ( pItemData->sBasicOp.emLevel == LEVEL_HEROIC )
					{
						AddTextNoSplit( "[Heroic] Arrow Item",NS_UITEXTCOLOR::HEROIC);
					}
					else if ( pItemData->sBasicOp.emLevel == LEVEL_RARE )
					{
						AddTextNoSplit( "[Rare] Arrow Item",NS_UITEXTCOLOR::RARE);
					}
					else if ( pItemData->sBasicOp.emLevel == LEVEL_HIGH_CLASS )
					{
						AddTextNoSplit( "[High Class] Arrow Item",NS_UITEXTCOLOR::HIGHCLASS);
					}
					else
					{
						AddTextNoSplit ( "[Normal] Arrow Item", NS_UITEXTCOLOR::WHITE );
					}
					AddTextNoSplit( "",NS_UITEXTCOLOR::WHITE);
					{
						CString strSchool, strClass;
						DWORD dwReqCharClass = pItemData->sBasicOp.dwReqCharClass;

						if (pItemData->sBasicOp.dwReqSchool!=GLSCHOOL_ALL)
						{
							if (pItemData->sBasicOp.dwReqSchool & GLSCHOOL_00) strSchool.AppendFormat ( "%s ",ID2GAMEWORD("ACADEMY_NAME",0));
							if (pItemData->sBasicOp.dwReqSchool & GLSCHOOL_01) strSchool.AppendFormat ( "%s ",ID2GAMEWORD("ACADEMY_NAME",1));
							if (pItemData->sBasicOp.dwReqSchool & GLSCHOOL_02) strSchool.AppendFormat ( "%s ",ID2GAMEWORD("ACADEMY_NAME",2));
						}
						else
						{
							strSchool = ID2GAMEWORD("ACADEMY_ALL",0);
						}

						if ( dwReqCharClass )
						{
							if ( dwReqCharClass==GLCC_ALL_2015)
							{
								strClass.Format ( "%s", ID2GAMEWORD("ITEM_ALL_CLASS"));
							}
							else
							{
								//Brawler
								if ((dwReqCharClass&GLCC_FIGHTER_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_FIGHTER_M)].c_str());
								if ((dwReqCharClass&GLCC_FIGHTER_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_FIGHTER_W)].c_str());
								//Swordsman
								if ((dwReqCharClass&GLCC_ARMS_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARMS_M)].c_str());
								if ((dwReqCharClass&GLCC_ARMS_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARMS_W)].c_str());
								//Archer
								if ((dwReqCharClass&GLCC_ARCHER_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_M)].c_str());
								if ((dwReqCharClass&GLCC_ARCHER_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_W)].c_str());
								//Shaman
								if ((dwReqCharClass&GLCC_SPIRIT_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_SPIRIT_M)].c_str());
								if ((dwReqCharClass&GLCC_SPIRIT_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_SPIRIT_W)].c_str());
								//Extreme
								if ((dwReqCharClass&GLCC_EXTREME_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_M)].c_str());
								if ((dwReqCharClass&GLCC_EXTREME_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_W)].c_str());
								//Scientist
								if ((dwReqCharClass&GLCC_SCIENCE_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_SCIENCE_M)].c_str());
								if ((dwReqCharClass&GLCC_SCIENCE_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_SCIENCE_W)].c_str());
								//Assassin
								if ((dwReqCharClass&GLCC_ASSASIN_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASIN_M)].c_str());
								if ((dwReqCharClass&GLCC_ASSASIN_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASIN_W)].c_str());
								//Tricker
								if ((dwReqCharClass&GLCC_TESTING_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_TESTING_M)].c_str());
								if ((dwReqCharClass&GLCC_TESTING_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_TESTING_W)].c_str());
								//Shaper
								if ((dwReqCharClass&GLCC_SHAPER_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_SHAPER_M)].c_str());
								if ((dwReqCharClass&GLCC_SHAPER_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_SHAPER_W)].c_str());
								
								if (	(dwReqCharClass&GLCC_FIGHTER_M)		&&
										(dwReqCharClass&GLCC_ARMS_M)  		&&
										(dwReqCharClass&GLCC_ARCHER_M)  	&&
										(dwReqCharClass&GLCC_SPIRIT_M)  	&&
										(dwReqCharClass&GLCC_EXTREME_M) 	&&
										(dwReqCharClass&GLCC_SCIENCE_M) 	&&
										(dwReqCharClass&GLCC_ASSASIN_M) 	&&
										(dwReqCharClass&GLCC_TESTING_M) 	&&
										(dwReqCharClass&GLCC_SHAPER_M)			)
								{
									strClass.Format( "%s", ID2GAMEWORD("ITEM_CLASS_GENDER",0));
								}
								else if ((dwReqCharClass&GLCC_FIGHTER_W) 	&&
										(dwReqCharClass&GLCC_ARMS_W)  		&&
										(dwReqCharClass&GLCC_ARCHER_W)  	&&	
										(dwReqCharClass&GLCC_SPIRIT_W)  	&&	
										(dwReqCharClass&GLCC_EXTREME_W)  	&&
										(dwReqCharClass&GLCC_SCIENCE_W)  	&&
										(dwReqCharClass&GLCC_ASSASIN_W)  	&&	
										(dwReqCharClass&GLCC_TESTING_W)   	&&	
										(dwReqCharClass&GLCC_SHAPER_W) 		)
								{
									strClass.Format( "%s", ID2GAMEWORD("ITEM_CLASS_GENDER",1));
								}
							}
						}

						CString strCombine;
						strCombine.Format ( "%s / %s", strSchool, strClass );
						AddTextNoSplit(strCombine, NS_UITEXTCONTROL::GetEvaluateColor ( (pItemData->sBasicOp.dwReqCharClass&pCharacter->m_emClass) &&
																						(pItemData->sBasicOp.dwReqSchool&index2school(pCharacter->m_wSchool))));
					}

					if ( pItemData->sBasicOp.wReqLevelDW || pItemData->sBasicOp.wReqLevelUP )
					{
						bool bReqLevel = true;
						strText.Format( "%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION", 0), pItemData->sBasicOp.wReqLevelDW );
											
						if ( pItemData->sBasicOp.wReqLevelUP )
						{
							strText2.Format ( "~%d",pItemData->sBasicOp.wReqLevelUP );
							strText += strText2; 
							bReqLevel = (pItemData->sBasicOp.wReqLevelUP >= pCharacter->m_wLevel);
						}

						AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pItemData->sBasicOp.wReqLevelDW <= pCharacter->m_wLevel && bReqLevel ) );
					}



					GLPADATA &sDAMAGE = sItemCustom.GETDAMAGE();
					if ( sDAMAGE.dwLow || sDAMAGE.dwHigh  )
					{
						strText.Format("Talisman %s :%s ~ %s ", ID2GAMEWORD("ITEM_ARROW_INFO", 0 ), 
													GetNumberWithSign ( sDAMAGE.dwLow ), 
													GetNumberWithSign ( sDAMAGE.dwHigh ) );

						AddTextNoSplit(strText,NS_UITEXTCOLOR::WHITE);
					}


 
					AddTextAddValue( sItemCustom, pItemData->sSuitOp );					
				}
				break;

			case ITEM_ARROW:
				{
							if ( pItemData->sBasicOp.emLevel == LEVEL_LEGENDARY )
							{
							AddTextNoSplit( "[Legendary] Arrow Item",NS_UITEXTCOLOR::LEGENDARY);
							}
							else if ( pItemData->sBasicOp.emLevel == LEVEL_ANCIENT )
							{
							AddTextNoSplit( "[Ancient] Arrow Item",NS_UITEXTCOLOR::ANCIENT);
							}
							else if ( pItemData->sBasicOp.emLevel == LEVEL_HEROIC )
							{
							AddTextNoSplit( "[Heroic] Arrow Item",NS_UITEXTCOLOR::HEROIC);
							}
							else if ( pItemData->sBasicOp.emLevel == LEVEL_RARE )
							{
							AddTextNoSplit( "[Rare] Arrow Item",NS_UITEXTCOLOR::RARE);
							}
							else if ( pItemData->sBasicOp.emLevel == LEVEL_HIGH_CLASS )
							{
							AddTextNoSplit( "[High Class] Arrow Item",NS_UITEXTCOLOR::HIGHCLASS);
							}
							else
							{
								AddTextNoSplit ( "[Normal] Arrow Item", NS_UITEXTCOLOR::WHITE );
							}
					AddTextNoSplit( "",NS_UITEXTCOLOR::WHITE);
					{
						CString strSchool, strClass;
						DWORD dwReqCharClass = pItemData->sBasicOp.dwReqCharClass;

						if (pItemData->sBasicOp.dwReqSchool!=GLSCHOOL_ALL)
						{
							if (pItemData->sBasicOp.dwReqSchool & GLSCHOOL_00) strSchool.AppendFormat ( "%s ",ID2GAMEWORD("ACADEMY_NAME",0));
							if (pItemData->sBasicOp.dwReqSchool & GLSCHOOL_01) strSchool.AppendFormat ( "%s ",ID2GAMEWORD("ACADEMY_NAME",1));
							if (pItemData->sBasicOp.dwReqSchool & GLSCHOOL_02) strSchool.AppendFormat ( "%s ",ID2GAMEWORD("ACADEMY_NAME",2));
						}
						else
						{
							strSchool = ID2GAMEWORD("ACADEMY_ALL",0);
						}

						if ( dwReqCharClass )
						{
							if ( dwReqCharClass==GLCC_ALL_2015)
							{
								strClass.Format ( "%s", ID2GAMEWORD("ITEM_ALL_CLASS"));
							}
							else
							{
								//Brawler
								if ((dwReqCharClass&GLCC_FIGHTER_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_FIGHTER_M)].c_str());
								if ((dwReqCharClass&GLCC_FIGHTER_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_FIGHTER_W)].c_str());
								//Swordsman
								if ((dwReqCharClass&GLCC_ARMS_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARMS_M)].c_str());
								if ((dwReqCharClass&GLCC_ARMS_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARMS_W)].c_str());
								//Archer
								if ((dwReqCharClass&GLCC_ARCHER_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_M)].c_str());
								if ((dwReqCharClass&GLCC_ARCHER_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_W)].c_str());
								//Shaman
								if ((dwReqCharClass&GLCC_SPIRIT_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_SPIRIT_M)].c_str());
								if ((dwReqCharClass&GLCC_SPIRIT_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_SPIRIT_W)].c_str());
								//Extreme
								if ((dwReqCharClass&GLCC_EXTREME_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_M)].c_str());
								if ((dwReqCharClass&GLCC_EXTREME_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_W)].c_str());
								//Scientist
								if ((dwReqCharClass&GLCC_SCIENCE_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_SCIENCE_M)].c_str());
								if ((dwReqCharClass&GLCC_SCIENCE_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_SCIENCE_W)].c_str());
								//Assassin
								if ((dwReqCharClass&GLCC_ASSASIN_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASIN_M)].c_str());
								if ((dwReqCharClass&GLCC_ASSASIN_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASIN_W)].c_str());
								//Tricker
								if ((dwReqCharClass&GLCC_TESTING_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_TESTING_M)].c_str());
								if ((dwReqCharClass&GLCC_TESTING_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_TESTING_W)].c_str());
								//Shaper
								if ((dwReqCharClass&GLCC_SHAPER_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_SHAPER_M)].c_str());
								if ((dwReqCharClass&GLCC_SHAPER_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_SHAPER_W)].c_str());
								
								if (	(dwReqCharClass&GLCC_FIGHTER_M)		&&
										(dwReqCharClass&GLCC_ARMS_M)  		&&
										(dwReqCharClass&GLCC_ARCHER_M)  	&&
										(dwReqCharClass&GLCC_SPIRIT_M)  	&&
										(dwReqCharClass&GLCC_EXTREME_M) 	&&
										(dwReqCharClass&GLCC_SCIENCE_M) 	&&
										(dwReqCharClass&GLCC_ASSASIN_M) 	&&
										(dwReqCharClass&GLCC_TESTING_M) 	&&
										(dwReqCharClass&GLCC_SHAPER_M)			)
								{
									strClass.Format( "%s", ID2GAMEWORD("ITEM_CLASS_GENDER",0));
								}
								else if ((dwReqCharClass&GLCC_FIGHTER_W) 	&&
										(dwReqCharClass&GLCC_ARMS_W)  		&&
										(dwReqCharClass&GLCC_ARCHER_W)  	&&	
										(dwReqCharClass&GLCC_SPIRIT_W)  	&&	
										(dwReqCharClass&GLCC_EXTREME_W)  	&&
										(dwReqCharClass&GLCC_SCIENCE_W)  	&&
										(dwReqCharClass&GLCC_ASSASIN_W)  	&&	
										(dwReqCharClass&GLCC_TESTING_W)   	&&	
										(dwReqCharClass&GLCC_SHAPER_W) 		)
								{
									strClass.Format( "%s", ID2GAMEWORD("ITEM_CLASS_GENDER",1));
								}
							}
						}

						CString strCombine;
						strCombine.Format ( "%s / %s", strSchool, strClass );
						AddTextNoSplit(strCombine, NS_UITEXTCONTROL::GetEvaluateColor ( (pItemData->sBasicOp.dwReqCharClass&pCharacter->m_emClass) &&
																						(pItemData->sBasicOp.dwReqSchool&index2school(pCharacter->m_wSchool))));
					}

					if ( pItemData->sBasicOp.wReqLevelDW || pItemData->sBasicOp.wReqLevelUP )
					{
						bool bReqLevel = true;
						strText.Format( "%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION", 0), pItemData->sBasicOp.wReqLevelDW );
											
						if ( pItemData->sBasicOp.wReqLevelUP )
						{
							strText2.Format ( "~%d",pItemData->sBasicOp.wReqLevelUP );
							strText += strText2; 
							bReqLevel = (pItemData->sBasicOp.wReqLevelUP >= pCharacter->m_wLevel);
						}

						AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pItemData->sBasicOp.wReqLevelDW <= pCharacter->m_wLevel && bReqLevel ) );
					}

					GLPADATA &sDAMAGE = sItemCustom.GETDAMAGE();
					if ( sDAMAGE.dwLow || sDAMAGE.dwHigh  )
					{
						strText.Format("Arrow %s :%s ~ %s ", ID2GAMEWORD("ITEM_ARROW_INFO", 0 ), 
													GetNumberWithSign ( sDAMAGE.dwLow ), 
													GetNumberWithSign ( sDAMAGE.dwHigh ) );

						AddTextNoSplit(strText,NS_UITEXTCOLOR::WHITE);
					}
 
					AddTextAddValue( sItemCustom, pItemData->sSuitOp );					
				}
				break;
				//add bullet
			case ITEM_BULLET:
				{			
					if ( pItemData->sBasicOp.emLevel == LEVEL_LEGENDARY )
					{
						AddTextNoSplit( "[Legendary] Arrow Item",NS_UITEXTCOLOR::LEGENDARY);
					}
					else if ( pItemData->sBasicOp.emLevel == LEVEL_ANCIENT )
					{
						AddTextNoSplit( "[Ancient] Arrow Item",NS_UITEXTCOLOR::ANCIENT);
					}
					else if ( pItemData->sBasicOp.emLevel == LEVEL_HEROIC )
					{
						AddTextNoSplit( "[Heroic] Arrow Item",NS_UITEXTCOLOR::HEROIC);
					}
					else if ( pItemData->sBasicOp.emLevel == LEVEL_RARE )
					{
						AddTextNoSplit( "[Rare] Arrow Item",NS_UITEXTCOLOR::RARE);
					}
					else if ( pItemData->sBasicOp.emLevel == LEVEL_HIGH_CLASS )
					{
						AddTextNoSplit( "[High Class] Arrow Item",NS_UITEXTCOLOR::HIGHCLASS);
					}
					else
					{
						AddTextNoSplit ( "[Normal] Arrow Item", NS_UITEXTCOLOR::WHITE );
					}

					AddTextNoSplit( "",NS_UITEXTCOLOR::WHITE);
					{
						CString strSchool, strClass;
						DWORD dwReqCharClass = pItemData->sBasicOp.dwReqCharClass;

						if (pItemData->sBasicOp.dwReqSchool!=GLSCHOOL_ALL)
						{
							if (pItemData->sBasicOp.dwReqSchool & GLSCHOOL_00) strSchool.AppendFormat ( "%s ",ID2GAMEWORD("ACADEMY_NAME",0));
							if (pItemData->sBasicOp.dwReqSchool & GLSCHOOL_01) strSchool.AppendFormat ( "%s ",ID2GAMEWORD("ACADEMY_NAME",1));
							if (pItemData->sBasicOp.dwReqSchool & GLSCHOOL_02) strSchool.AppendFormat ( "%s ",ID2GAMEWORD("ACADEMY_NAME",2));
						}
						else
						{
							strSchool = ID2GAMEWORD("ACADEMY_ALL",0);
						}

						if ( dwReqCharClass )
						{
							if ( dwReqCharClass==GLCC_ALL_2015)
							{
								strClass.Format ( "%s", ID2GAMEWORD("ITEM_ALL_CLASS"));
							}
							else
							{
								//Brawler
								if ((dwReqCharClass&GLCC_FIGHTER_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_FIGHTER_M)].c_str());
								if ((dwReqCharClass&GLCC_FIGHTER_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_FIGHTER_W)].c_str());
								//Swordsman
								if ((dwReqCharClass&GLCC_ARMS_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARMS_M)].c_str());
								if ((dwReqCharClass&GLCC_ARMS_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARMS_W)].c_str());
								//Archer
								if ((dwReqCharClass&GLCC_ARCHER_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_M)].c_str());
								if ((dwReqCharClass&GLCC_ARCHER_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_W)].c_str());
								//Shaman
								if ((dwReqCharClass&GLCC_SPIRIT_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_SPIRIT_M)].c_str());
								if ((dwReqCharClass&GLCC_SPIRIT_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_SPIRIT_W)].c_str());
								//Extreme
								if ((dwReqCharClass&GLCC_EXTREME_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_M)].c_str());
								if ((dwReqCharClass&GLCC_EXTREME_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_W)].c_str());
								//Scientist
								if ((dwReqCharClass&GLCC_SCIENCE_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_SCIENCE_M)].c_str());
								if ((dwReqCharClass&GLCC_SCIENCE_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_SCIENCE_W)].c_str());
								//Assassin
								if ((dwReqCharClass&GLCC_ASSASIN_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASIN_M)].c_str());
								if ((dwReqCharClass&GLCC_ASSASIN_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASIN_W)].c_str());
								//Tricker
								if ((dwReqCharClass&GLCC_TESTING_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_TESTING_M)].c_str());
								if ((dwReqCharClass&GLCC_TESTING_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_TESTING_W)].c_str());
								//Shaper
								if ((dwReqCharClass&GLCC_SHAPER_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_SHAPER_M)].c_str());
								if ((dwReqCharClass&GLCC_SHAPER_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_SHAPER_W)].c_str());
								
								if (	(dwReqCharClass&GLCC_FIGHTER_M)		&&
										(dwReqCharClass&GLCC_ARMS_M)  		&&
										(dwReqCharClass&GLCC_ARCHER_M)  	&&
										(dwReqCharClass&GLCC_SPIRIT_M)  	&&
										(dwReqCharClass&GLCC_EXTREME_M) 	&&
										(dwReqCharClass&GLCC_SCIENCE_M) 	&&
										(dwReqCharClass&GLCC_ASSASIN_M) 	&&
										(dwReqCharClass&GLCC_TESTING_M) 	&&
										(dwReqCharClass&GLCC_SHAPER_M)			)
								{
									strClass.Format( "%s", ID2GAMEWORD("ITEM_CLASS_GENDER",0));
								}
								else if ((dwReqCharClass&GLCC_FIGHTER_W) 	&&
										(dwReqCharClass&GLCC_ARMS_W)  		&&
										(dwReqCharClass&GLCC_ARCHER_W)  	&&	
										(dwReqCharClass&GLCC_SPIRIT_W)  	&&	
										(dwReqCharClass&GLCC_EXTREME_W)  	&&
										(dwReqCharClass&GLCC_SCIENCE_W)  	&&
										(dwReqCharClass&GLCC_ASSASIN_W)  	&&	
										(dwReqCharClass&GLCC_TESTING_W)  	&&	
										(dwReqCharClass&GLCC_SHAPER_W)  		)
								{
									strClass.Format( "%s", ID2GAMEWORD("ITEM_CLASS_GENDER",1));
								}
							}
						}

						CString strCombine;
						strCombine.Format ( "%s / %s", strSchool, strClass );
						AddTextNoSplit(strCombine, NS_UITEXTCONTROL::GetEvaluateColor ( (pItemData->sBasicOp.dwReqCharClass&pCharacter->m_emClass) &&
																						(pItemData->sBasicOp.dwReqSchool&index2school(pCharacter->m_wSchool))));
					}

					if ( pItemData->sBasicOp.wReqLevelDW || pItemData->sBasicOp.wReqLevelUP )
					{
						bool bReqLevel = true;
						strText.Format( "%s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION", 0), pItemData->sBasicOp.wReqLevelDW );
											
						if ( pItemData->sBasicOp.wReqLevelUP )
						{
							strText2.Format ( "~%d",pItemData->sBasicOp.wReqLevelUP );
							strText += strText2; 
							bReqLevel = (pItemData->sBasicOp.wReqLevelUP >= pCharacter->m_wLevel);
						}

						AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pItemData->sBasicOp.wReqLevelDW <= pCharacter->m_wLevel && bReqLevel ) );
					}

					GLPADATA &sDAMAGE = sItemCustom.GETDAMAGE();
					if ( sDAMAGE.dwLow || sDAMAGE.dwHigh  )
					{
						strText.Format("Bullet %s :%s ~ %s ", ID2GAMEWORD("ITEM_ARROW_INFO", 0 ), 
													GetNumberWithSign ( sDAMAGE.dwLow ), 
													GetNumberWithSign ( sDAMAGE.dwHigh ) );

						AddTextNoSplit(strText,NS_UITEXTCOLOR::WHITE);
					}
 
					AddTextAddValue( sItemCustom, pItemData->sSuitOp );					
				}
				break;

				case ITEM_BOX:
				case ITEM_RANDOMITEM:
					{
						if ( pItemData->sBasicOp.wReqLevelDW || pItemData->sBasicOp.wReqLevelUP )
						{
							bool bReqLevel = true;
							strText.Format( "           %s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_CONDITION", 0), pItemData->sBasicOp.wReqLevelDW );
												
							if ( pItemData->sBasicOp.wReqLevelUP )
							{
								strText2.Format ( "~%d",pItemData->sBasicOp.wReqLevelUP );
								strText += strText2; 
								bReqLevel = (pItemData->sBasicOp.wReqLevelUP >= pCharacter->m_wLevel);
							}

							AddTextNoSplit(strText, NS_UITEXTCONTROL::GetEvaluateColor ( pItemData->sBasicOp.wReqLevelDW <= pCharacter->m_wLevel && bReqLevel ) );
						}
						else
						{
							AddTextNoSplit ( "", NS_UITEXTCOLOR::WHITE );
						}

						{
							CString strSchool, strClass;
							DWORD dwReqCharClass = pItemData->sBasicOp.dwReqCharClass;

							if (pItemData->sBasicOp.dwReqSchool!=GLSCHOOL_ALL)
							{
								if (pItemData->sBasicOp.dwReqSchool & GLSCHOOL_00) strSchool.AppendFormat ( "%s ",ID2GAMEWORD("ACADEMY_NAME",0));
								if (pItemData->sBasicOp.dwReqSchool & GLSCHOOL_01) strSchool.AppendFormat ( "%s ",ID2GAMEWORD("ACADEMY_NAME",1));
								if (pItemData->sBasicOp.dwReqSchool & GLSCHOOL_02) strSchool.AppendFormat ( "%s ",ID2GAMEWORD("ACADEMY_NAME",2));
							}
							else
							{
								strSchool = ID2GAMEWORD("ACADEMY_ALL",0);
							}

							if ( dwReqCharClass )
							{
								if ( dwReqCharClass==GLCC_ALL_2015)
								{
									strClass.Format ( "%s", ID2GAMEWORD("ITEM_ALL_CLASS"));
								}
								else
								{
									//Brawler
									if ((dwReqCharClass&GLCC_FIGHTER_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_FIGHTER_M)].c_str());
									if ((dwReqCharClass&GLCC_FIGHTER_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_FIGHTER_W)].c_str());
									//Swordsman
									if ((dwReqCharClass&GLCC_ARMS_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARMS_M)].c_str());
									if ((dwReqCharClass&GLCC_ARMS_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARMS_W)].c_str());
									//Archer
									if ((dwReqCharClass&GLCC_ARCHER_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_M)].c_str());
									if ((dwReqCharClass&GLCC_ARCHER_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_ARCHER_W)].c_str());
									//Shaman
									if ((dwReqCharClass&GLCC_SPIRIT_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_SPIRIT_M)].c_str());
									if ((dwReqCharClass&GLCC_SPIRIT_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_SPIRIT_W)].c_str());
									//Extreme
									if ((dwReqCharClass&GLCC_EXTREME_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_M)].c_str());
									if ((dwReqCharClass&GLCC_EXTREME_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_EXTREME_W)].c_str());
									//Scientist
									if ((dwReqCharClass&GLCC_SCIENCE_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_SCIENCE_M)].c_str());
									if ((dwReqCharClass&GLCC_SCIENCE_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_SCIENCE_W)].c_str());
									//Assassin
									if ((dwReqCharClass&GLCC_ASSASIN_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASIN_M)].c_str());
									if ((dwReqCharClass&GLCC_ASSASIN_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_ASSASIN_W)].c_str());
									//Tricker
									if ((dwReqCharClass&GLCC_TESTING_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_TESTING_M)].c_str());
									if ((dwReqCharClass&GLCC_TESTING_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_TESTING_W)].c_str());
									//Shaper
									if ((dwReqCharClass&GLCC_SHAPER_M)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_SHAPER_M)].c_str());
									if ((dwReqCharClass&GLCC_SHAPER_W)) strClass.AppendFormat("%s ", COMMENT::CHARCLASS[CharClassToIndex(GLCC_SHAPER_W)].c_str());
									
									if (	(dwReqCharClass&GLCC_FIGHTER_M)		&&
											(dwReqCharClass&GLCC_ARMS_M)  		&&
											(dwReqCharClass&GLCC_ARCHER_M)  	&&
											(dwReqCharClass&GLCC_SPIRIT_M)  	&&
											(dwReqCharClass&GLCC_EXTREME_M) 	&&
											(dwReqCharClass&GLCC_SCIENCE_M) 	&&
											(dwReqCharClass&GLCC_ASSASIN_M) 	&&
											(dwReqCharClass&GLCC_TESTING_M) 	&&
											(dwReqCharClass&GLCC_SHAPER_M)			)
									{
										strClass.Format( "%s", ID2GAMEWORD("ITEM_CLASS_GENDER",0));
									}
									else if ((dwReqCharClass&GLCC_FIGHTER_W) 	&&
											(dwReqCharClass&GLCC_ARMS_W)  		&&
											(dwReqCharClass&GLCC_ARCHER_W)  	&&	
											(dwReqCharClass&GLCC_SPIRIT_W)  	&&	
											(dwReqCharClass&GLCC_EXTREME_W)  	&&
											(dwReqCharClass&GLCC_SCIENCE_W)  	&&
											(dwReqCharClass&GLCC_ASSASIN_W)  	&&	
											(dwReqCharClass&GLCC_TESTING_W)  	&&	
											(dwReqCharClass&GLCC_SHAPER_W)  		)
									{
										strClass.Format( "%s", ID2GAMEWORD("ITEM_CLASS_GENDER",1));
									}
								}
							}

							CString strCombine;
							strCombine.Format ( "%s / %s", strSchool, strClass );
							AddTextNoSplit(strCombine, NS_UITEXTCONTROL::GetEvaluateColor ( (pItemData->sBasicOp.dwReqCharClass&pCharacter->m_emClass) &&
																							(pItemData->sBasicOp.dwReqSchool&index2school(pCharacter->m_wSchool))));
						}

						SITEM* pItem = GLItemMan::GetInstance().GetItem ( sNpcNativeID );

						if ( sNpcNativeID != sItemCustom.sNativeID ) 
						{
							sNpcNativeID = sItemCustom.sNativeID;
					
							if ( sNpcNativeID.IsValidNativeID() )
							{
								SITEM* pItem = GLItemMan::GetInstance().GetItem ( sNpcNativeID );
								if ( !pItem )
								{
									return ;
									ResetItemRender ();	
									ResetItemInBox ();
									ResetItemInRandomBox();
								}

								if ( pItem->sBasicOp.emItemType == ITEM_BOX  )
								{
									ResetItemRender ();	
									ResetItemInBox ();
									ResetItemInRandomBox();
									
									AddTextNoSplit ( "",NS_UITEXTCOLOR::BLACK );
									AddTextNoSplit ( "",NS_UITEXTCOLOR::BLACK );
									AddTextNoSplit ( "",NS_UITEXTCOLOR::BLACK );

									for ( int i=0; i<ITEM::SBOX::ITEM_SIZE; ++i )
									{
										SITEMCUSTOM sCUSTOM;
										sCUSTOM.sNativeID = pItem->sBox.sITEMS[i].nidITEM;

										if ( sCUSTOM.sNativeID==SNATIVEID(false) )				continue;

										SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sCUSTOM.sNativeID );
										if ( pItemData ) 
										{
											SetItemInBox ( pItemData->sBasicOp.sICONID, pItemData->GetInventoryFile(), i );
										}
									}
								    SITEMCUSTOM sCUSTOM;
									AddItemRender ( pItem->sBasicOp.sICONID, pItem->GetInventoryFile(),  sCUSTOM );
									
									{
										bool bCanPreview = CInnerInterface::GetInstance().PreviewCheckSimple( pItemData->sBasicOp.sNativeID );
										if ( bCanPreview )
										{
											AddTextLongestLineSplit ( ID2GAMEWORD( "ITEM_USEPREVIEW", 0 ), NS_UITEXTCOLOR::WHITE );
										}
									}
									AddTextNoSplit ( "Alt + Left-Click to Show Item Preview in your character. Items aren't Random.", NS_UITEXTCOLOR::WHITE );
								}
								else if ( pItem->sBasicOp.emItemType == ITEM_RANDOMITEM )
								{
									ResetItemRender ();	
									ResetItemInBox ();
									ResetItemInRandomBox();

									int nVALUEBOX = int(pItemData->sRandomBox.vecBOX.size());
									if ( nVALUEBOX <= 10 )
									{
										AddTextNoSplit ( " ",NS_UITEXTCOLOR::BLACK );
										AddTextNoSplit ( " ",NS_UITEXTCOLOR::BLACK );
										AddTextNoSplit ( " ",NS_UITEXTCOLOR::BLACK );
									}
									else if ( nVALUEBOX <= 20 )
									{
										AddTextNoSplit ( " ",NS_UITEXTCOLOR::BLACK );
										AddTextNoSplit ( " ",NS_UITEXTCOLOR::BLACK );
										AddTextNoSplit ( " ",NS_UITEXTCOLOR::BLACK );
										AddTextNoSplit ( " ",NS_UITEXTCOLOR::BLACK );
										AddTextNoSplit ( " ",NS_UITEXTCOLOR::BLACK );
										AddTextNoSplit ( " ",NS_UITEXTCOLOR::BLACK );
									}
									else if ( nVALUEBOX <= 30 )
									{
										AddTextNoSplit ( " ",NS_UITEXTCOLOR::BLACK );
										AddTextNoSplit ( " ",NS_UITEXTCOLOR::BLACK );
										AddTextNoSplit ( " ",NS_UITEXTCOLOR::BLACK );
										AddTextNoSplit ( " ",NS_UITEXTCOLOR::BLACK );
										AddTextNoSplit ( " ",NS_UITEXTCOLOR::BLACK );
										AddTextNoSplit ( " ",NS_UITEXTCOLOR::BLACK );
										AddTextNoSplit ( " ",NS_UITEXTCOLOR::BLACK );
										AddTextNoSplit ( " ",NS_UITEXTCOLOR::BLACK );
										AddTextNoSplit ( " ",NS_UITEXTCOLOR::BLACK );
									}

									for ( DWORD i=0; i<pItem->sRandomBox.vecBOX.size(); ++i )
									{
										SITEMCUSTOM sCUSTOM;
										ITEM::SRANDOMITEM sITEMBOX = pItem->sRandomBox.vecBOX[i];
										sCUSTOM.sNativeID = sITEMBOX.nidITEM;

										if ( sCUSTOM.sNativeID==SNATIVEID(false) )				continue;

										SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sCUSTOM.sNativeID );
										if ( pItemData ) 
										{
											SetItemInRandomBox ( pItemData->sBasicOp.sICONID, pItemData->GetInventoryFile(), i );
										}
									}
									
									
									{
										bool bCanPreview = CInnerInterface::GetInstance().PreviewCheckSimple( pItemData->sBasicOp.sNativeID );
										if ( bCanPreview )
										{
											AddTextLongestLineSplit ( ID2GAMEWORD( "ITEM_USEPREVIEW", 0 ), NS_UITEXTCOLOR::WHITE );
										}
									}
                                    SITEMCUSTOM sCUSTOM;
									AddItemRender ( pItem->sBasicOp.sICONID, pItem->GetInventoryFile(), sCUSTOM );
									AddTextNoSplit ( "Alt + Left-Click to Show Item Preview in your character. Items here are Randomized.", NS_UITEXTCOLOR::WHITE );
								}
							}
						}
					}
					break;

			case ITEM_CURE:
				{
					AddTextNoSplit( "",NS_UITEXTCOLOR::WHITE);
					if ( pItemData->sBasicOp.emLevel == LEVEL_LEGENDARY )
					{
					AddTextNoSplit( "[Legendary] Potion Item",NS_UITEXTCOLOR::LEGENDARY);
					}
					else if ( pItemData->sBasicOp.emLevel == LEVEL_ANCIENT )
					{
					AddTextNoSplit( "[Ancient] Potion Item",NS_UITEXTCOLOR::ANCIENT);
					}
					else if ( pItemData->sBasicOp.emLevel == LEVEL_HEROIC )
					{
					AddTextNoSplit( "[Heroic] Potion Item",NS_UITEXTCOLOR::HEROIC);
					}
					else if ( pItemData->sBasicOp.emLevel == LEVEL_RARE )
					{
					AddTextNoSplit( "[Rare] Potion Item",NS_UITEXTCOLOR::RARE);
					}
					else if ( pItemData->sBasicOp.emLevel == LEVEL_HIGH_CLASS )
					{
					AddTextNoSplit( "[High Class] Potion Item",NS_UITEXTCOLOR::HIGHCLASS);
					}
					else
					{
						AddTextNoSplit ( "[Normal] Potion Item", NS_UITEXTCOLOR::WHITE );
					}
					if( pItemData->sDrugOp.wCureVolume != 0 )
					{
						strText.Format("%s :%d",COMMENT::ITEMDRUG[pItemData->sDrugOp.emDrug].c_str(), pItemData->sDrugOp.wCureVolume);
					}
					AddTextNoSplit( "",NS_UITEXTCOLOR::WHITE);

					AddTextNoSplit(strText, NS_UITEXTCOLOR::WHITE);
				}
				break;

			case ITEM_SKILL:
				{
					AddInfoSkillItem ( sItemCustom );
				}
				break;

			case ITEM_PET_SKILL:
				{
					DWORD dwLevel = (DWORD)pItemData->sBasicOp.emLevel;

					D3DCOLOR dwCOLOR;

					AddTextNoSplit( "",NS_UITEXTCOLOR::WHITE);
					if ( pItemData->sBasicOp.emLevel == LEVEL_LEGENDARY )
					{
					AddTextNoSplit( "[Legendary] Pet Item",NS_UITEXTCOLOR::LEGENDARY);
					AddInfoPetSkillItem( sItemCustom );
					}
					else if ( pItemData->sBasicOp.emLevel == LEVEL_ANCIENT )
					{
					AddTextNoSplit( "[Ancient] Pet Item",NS_UITEXTCOLOR::ANCIENT);
					AddInfoPetSkillItem( sItemCustom );
					}
					else if ( pItemData->sBasicOp.emLevel == LEVEL_HEROIC )
					{
					AddTextNoSplit( "[Heroic] Pet Item",NS_UITEXTCOLOR::HEROIC);
					AddInfoPetSkillItem( sItemCustom );
					}
					else if ( pItemData->sBasicOp.emLevel == LEVEL_RARE )
					{
					AddTextNoSplit( "[Rare] Pet Item",NS_UITEXTCOLOR::RARE);
					AddInfoPetSkillItem( sItemCustom );
					}
					else if ( pItemData->sBasicOp.emLevel == LEVEL_HIGH_CLASS )
					{
					AddTextNoSplit( "[High Class] Pet Item",NS_UITEXTCOLOR::HIGHCLASS);
					AddInfoPetSkillItem( sItemCustom );
					}
					else
					{
						AddTextNoSplit ( "[Normal] Pet Item", NS_UITEXTCOLOR::WHITE );
						AddInfoPetSkillItem( sItemCustom );
					}
					AddTextNoSplit( "",NS_UITEXTCOLOR::WHITE);
				}
				break;

			case ITEM_MATERIALS:
				{
					DWORD dwLevel = (DWORD)pItemData->sBasicOp.emLevel;

					D3DCOLOR dwCOLOR;
					
					AddTextNoSplit( "",NS_UITEXTCOLOR::WHITE);
					if ( pItemData->sBasicOp.emLevel == LEVEL_LEGENDARY )
					{
					AddTextNoSplit( "[Legendary] Material Item",NS_UITEXTCOLOR::LEGENDARY);
					}
					else if ( pItemData->sBasicOp.emLevel == LEVEL_ANCIENT )
					{
					AddTextNoSplit( "[Ancient] Material Item",NS_UITEXTCOLOR::ANCIENT);
					}
					else if ( pItemData->sBasicOp.emLevel == LEVEL_HEROIC )
					{
					AddTextNoSplit( "[Heroic] Material Item",NS_UITEXTCOLOR::HEROIC);
					}
					else if ( pItemData->sBasicOp.emLevel == LEVEL_RARE )
					{
					AddTextNoSplit( "[Rare] Material Item",NS_UITEXTCOLOR::RARE);
					}
					else if ( pItemData->sBasicOp.emLevel == LEVEL_HIGH_CLASS )
					{
					AddTextNoSplit( "[High Class] Material Item",NS_UITEXTCOLOR::HIGHCLASS);
					}
					else
					{
						AddTextNoSplit ( "[Normal] Material Item", NS_UITEXTCOLOR::WHITE );
					}
					AddTextNoSplit( "",NS_UITEXTCOLOR::WHITE);
				}
				break;

			case ITEM_TICKET:
				{
					DWORD dwLevel = (DWORD)pItemData->sBasicOp.emLevel;

					D3DCOLOR dwCOLOR;
					
					AddTextNoSplit( "",NS_UITEXTCOLOR::WHITE);
					if ( pItemData->sBasicOp.emLevel == LEVEL_LEGENDARY )
					{
					AddTextNoSplit( "[Legendary] Ticket Item",NS_UITEXTCOLOR::LEGENDARY);
					}
					else if ( pItemData->sBasicOp.emLevel == LEVEL_ANCIENT )
					{
					AddTextNoSplit( "[Ancient] Ticket Item",NS_UITEXTCOLOR::ANCIENT);
					}
					else if ( pItemData->sBasicOp.emLevel == LEVEL_HEROIC )
					{
					AddTextNoSplit( "[Heroic] Ticket Item",NS_UITEXTCOLOR::HEROIC);
					}
					else if ( pItemData->sBasicOp.emLevel == LEVEL_RARE )
					{
					AddTextNoSplit( "[Rare] Ticket Item",NS_UITEXTCOLOR::RARE);
					}
					else if ( pItemData->sBasicOp.emLevel == LEVEL_HIGH_CLASS )
					{
					AddTextNoSplit( "[High Class] Ticket Item",NS_UITEXTCOLOR::HIGHCLASS);
					}
					else
					{
						AddTextNoSplit ( "[Normal] Ticket Item", NS_UITEXTCOLOR::WHITE );
					}
					AddTextNoSplit( "",NS_UITEXTCOLOR::WHITE);
				}
				break;


			case ITEM_INVEN_CARD:
				{
					DWORD dwLevel = (DWORD)pItemData->sBasicOp.emLevel;

					D3DCOLOR dwCOLOR;
					
					AddTextNoSplit( "",NS_UITEXTCOLOR::WHITE);
					if ( pItemData->sBasicOp.emLevel == LEVEL_LEGENDARY )
					{
					AddTextNoSplit( "[Legendary] Backpack Item",NS_UITEXTCOLOR::LEGENDARY);
					}
					else if ( pItemData->sBasicOp.emLevel == LEVEL_ANCIENT )
					{
					AddTextNoSplit( "[Ancient] Backpack Item",NS_UITEXTCOLOR::ANCIENT);
					}
					else if ( pItemData->sBasicOp.emLevel == LEVEL_HEROIC )
					{
					AddTextNoSplit( "[Heroic] Backpack Item",NS_UITEXTCOLOR::HEROIC);
					}
					else if ( pItemData->sBasicOp.emLevel == LEVEL_RARE )
					{
					AddTextNoSplit( "[Rare] Backpack Item",NS_UITEXTCOLOR::RARE);
					}
					else if ( pItemData->sBasicOp.emLevel == LEVEL_HIGH_CLASS )
					{
					AddTextNoSplit( "[High Class] Backpack Item",NS_UITEXTCOLOR::HIGHCLASS);
					}
					else
					{
						AddTextNoSplit ( "[Normal] Backpack Item", NS_UITEXTCOLOR::WHITE );
					}
					
					AddTextNoSplit( "",NS_UITEXTCOLOR::WHITE);
				}
				break;

			case ITEM_PRIVATEMARKET:
				{
					DWORD dwLevel = (DWORD)pItemData->sBasicOp.emLevel;

					D3DCOLOR dwCOLOR;
					
					AddTextNoSplit( "",NS_UITEXTCOLOR::WHITE);
					if ( pItemData->sBasicOp.emLevel == LEVEL_LEGENDARY )
					{
					AddTextNoSplit( "[Legendary] Personal Store Item",NS_UITEXTCOLOR::LEGENDARY);
					}
					else if ( pItemData->sBasicOp.emLevel == LEVEL_ANCIENT )
					{
					AddTextNoSplit( "[Ancient] Personal Store Item",NS_UITEXTCOLOR::ANCIENT);
					}
					else if ( pItemData->sBasicOp.emLevel == LEVEL_HEROIC )
					{
					AddTextNoSplit( "[Heroic] Personal Store Item",NS_UITEXTCOLOR::HEROIC);
					}
					else if ( pItemData->sBasicOp.emLevel == LEVEL_RARE )
					{
					AddTextNoSplit( "[Rare] Personal Store Item",NS_UITEXTCOLOR::RARE);
					}
					else if ( pItemData->sBasicOp.emLevel == LEVEL_HIGH_CLASS )
					{
					AddTextNoSplit( "[High Class] Personal Store Item",NS_UITEXTCOLOR::HIGHCLASS);
					}
					else
					{
						AddTextNoSplit ( "[Normal] Personal Store Item", NS_UITEXTCOLOR::WHITE );
					}
					
					AddTextNoSplit( "",NS_UITEXTCOLOR::WHITE);
				}
				break;

			case ITEM_LOUDSPEAKER:
				{
					DWORD dwLevel = (DWORD)pItemData->sBasicOp.emLevel;

					D3DCOLOR dwCOLOR;
					
					AddTextNoSplit( "",NS_UITEXTCOLOR::WHITE);
					if ( pItemData->sBasicOp.emLevel == LEVEL_LEGENDARY )
					{
					AddTextNoSplit( "[Legendary] Loudspeaker Item",NS_UITEXTCOLOR::LEGENDARY);
					}
					else if ( pItemData->sBasicOp.emLevel == LEVEL_ANCIENT )
					{
					AddTextNoSplit( "[Ancient] Loudspeaker Item",NS_UITEXTCOLOR::ANCIENT);
					}
					else if ( pItemData->sBasicOp.emLevel == LEVEL_HEROIC )
					{
					AddTextNoSplit( "[Heroic] Loudspeaker Item",NS_UITEXTCOLOR::HEROIC);
					}
					else if ( pItemData->sBasicOp.emLevel == LEVEL_RARE )
					{
					AddTextNoSplit( "[Rare] Loudspeaker Item",NS_UITEXTCOLOR::RARE);
					}
					else if ( pItemData->sBasicOp.emLevel == LEVEL_HIGH_CLASS )
					{
					AddTextNoSplit( "[High Class] Loudspeaker Item",NS_UITEXTCOLOR::HIGHCLASS);
					}
					else
					{
						AddTextNoSplit ( "[Normal] Loudspeaker Item", NS_UITEXTCOLOR::WHITE );
					}
					
					AddTextNoSplit( "",NS_UITEXTCOLOR::WHITE);
				}
				break;

			case ITEM_CLEANSER:
				{
					DWORD dwLevel = (DWORD)pItemData->sBasicOp.emLevel;

					D3DCOLOR dwCOLOR;
					
					
					AddTextNoSplit( "",NS_UITEXTCOLOR::WHITE);
					if ( pItemData->sBasicOp.emLevel == LEVEL_LEGENDARY )
					{
					AddTextNoSplit( "[Legendary] Cleaner Item",NS_UITEXTCOLOR::LEGENDARY);
					}
					else if ( pItemData->sBasicOp.emLevel == LEVEL_ANCIENT )
					{
					AddTextNoSplit( "[Ancient] Cleaner Item",NS_UITEXTCOLOR::ANCIENT);
					}
					else if ( pItemData->sBasicOp.emLevel == LEVEL_HEROIC )
					{
					AddTextNoSplit( "[Heroic] Cleaner Item",NS_UITEXTCOLOR::HEROIC);
					}
					else if ( pItemData->sBasicOp.emLevel == LEVEL_RARE )
					{
					AddTextNoSplit( "[Rare] Loudspeaker Item",NS_UITEXTCOLOR::RARE);
					}
					else if ( pItemData->sBasicOp.emLevel == LEVEL_HIGH_CLASS )
					{
					AddTextNoSplit( "[High Class] Cleaner Item",NS_UITEXTCOLOR::HIGHCLASS);
					}
					else
					{
						AddTextNoSplit ( "[Normal] Cleaner Item", NS_UITEXTCOLOR::WHITE );
					}
					
					AddTextNoSplit( "",NS_UITEXTCOLOR::WHITE);
				}
				break;

			case ITEM_GRINDING:
				{
					AddTextNoSplit( "",NS_UITEXTCOLOR::WHITE);
					AddTextNoSplit ( ID2GAMEWORD ( "ITEM_CATEGORY_EX", 0 ), NS_UITEXTCOLOR::DARKORANGE );
				}
				break;

			case ITEM_VIETNAM_EXPGET:
				{
					if ( pItemData->sDrugOp.bRatio )
					{
						strText.Format(ID2GAMEWORD ("ITEM_CATEGORY_VIETNAM_EXP", 1) , pItemData->sDrugOp.wCureVolume );						
					}
					else
					{
						strText.Format(ID2GAMEWORD ("ITEM_CATEGORY_VIETNAM_EXP", 0), pItemData->sDrugOp.wCureVolume );
					}

					AddTextNoSplit(strText,NS_UITEXTCOLOR::LIGHTSKYBLUE);
				}
				break;
			case ITEM_VIETNAM_ITEMGET:
				{
					strText.Format(ID2GAMEWORD ("ITEM_CATEGORY_VIETNAM_ITEM", 0) , pItemData->sDrugOp.wCureVolume );
					AddTextNoSplit(strText,NS_UITEXTCOLOR::LIGHTSKYBLUE);
				}
				break;

			case ITEM_VEHICLE:
				{					
					EMITEM_VAR emITEM_VOL = pItemData->sSuitOp.sVOLUME.emTYPE;

					if ( emITEM_VOL != EMVAR_NONE)
					{
						if ( emITEM_VOL == EMVAR_MOVE_SPEED )
						{
							float fVOLUME = sItemCustom.GETMOVESPEED();
							strText.Format("%s :%.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD_VOL", emITEM_VOL ), fVOLUME );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::GREEN );
						}
						else
						{
							float fVOLUME = pItemData->sSuitOp.sVOLUME.fVolume;
							strText.Format("%s :%.2f", ID2GAMEWORD("ITEM_ADVANCED_INFO_SPECIAL_ADD_VOL", emITEM_VOL ), fVOLUME );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::PRIVATE);
						}
					}


					bool bInfo(true);

					
					VEHICLEITEMINFO_MAP_ITER iter = GLGaeaClient::GetInstance().GetCharacter()->m_mapVEHICLEItemInfo.find ( sItemCustom.dwVehicleID );
					if ( iter == GLGaeaClient::GetInstance().GetCharacter()->m_mapVEHICLEItemInfo.end() )
					{
						iter = GLGaeaClient::GetInstance().GetCharacter()->m_mapVEHICLEItemInfoTemp.find ( sItemCustom.dwVehicleID );
						if ( iter == GLGaeaClient::GetInstance().GetCharacter()->m_mapVEHICLEItemInfoTemp.end() )	bInfo = false;
					}


					if ( bInfo ) 
					{
						SVEHICLEITEMINFO sVehicleItemInfo = (*iter).second;
						for ( int i = 0; i < VEHICLE_ACCETYPE_SIZE; ++i ) 
						{
							SITEM* pItem = GLItemMan::GetInstance().GetItem ( sVehicleItemInfo.m_PutOnItems[i].sNativeID );
							if ( pItem )
							{
								strText.Format("%s %s", ID2GAMEWORD ("ITEM_VEHICLE_SLOT",i), pItem->GetName () );
								AddTextNoSplit(strText,NS_UITEXTCOLOR::WHITE);
							}
						}

						strText.Format( "%s %.2f%%", ID2GAMEWORD ("ITEM_VEHICLE_BATTERY",0), sVehicleItemInfo.m_nFull/10.0f );
						AddTextNoSplit(strText,NS_UITEXTCOLOR::WHITE);
					}			

					/*if( sItemCustom.IsSetRandOpt() )
					{
						CString strDescText;
						INT nRandOptType = sItemCustom.GETOptTYPE1();
						if( (EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );

							float fVal = sItemCustom.GETOptVALUE1();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::AQUA );
							}
						}

						nRandOptType = sItemCustom.GETOptTYPE2();
						if( (EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );

							float fVal = sItemCustom.GETOptVALUE2();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::AQUA );
							}
						}

						nRandOptType = sItemCustom.GETOptTYPE3();
						if( (EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );

							float fVal = sItemCustom.GETOptVALUE3();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::AQUA );
							}
						}

						nRandOptType = sItemCustom.GETOptTYPE4();
						if( (EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE) )
						{
							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );

							float fVal = sItemCustom.GETOptVALUE4();
							if( fVal != 0.0f )
							{
								if( fVal > 0.0f) strDescText += _T('+');

								strDescText.AppendFormat( "%.2f", fVal );
								if( sItemCustom.IsPerRandOpt( nRandOptType ) ) strDescText += _T('%');

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::AQUA );
							}
						}
					}*/

					for(int i = 0; i < 4; i++)
					{
						if ( !sItemCustom.IsSetRandOpt() ) 
							break;

						CString strDescText;
						EMRANDOM_OPT emOpt[4] = 
						{
							sItemCustom.GETOptTYPE1()
							, sItemCustom.GETOptTYPE2()
							, sItemCustom.GETOptTYPE3()
							, sItemCustom.GETOptTYPE4()
						};

						INT nRandOptType = emOpt[i];

						if( (EMR_OPT_NULL < nRandOptType) && (nRandOptType < EMR_OPT_SIZE) )
						{
							float fVal[4] =
							{
								sItemCustom.GETOptVALUE1()
								, sItemCustom.GETOptVALUE2()
								, sItemCustom.GETOptVALUE3()
								, sItemCustom.GETOptVALUE4()
							};

							strDescText.Format( "%s:", ID2GAMEWORD("ITEM_RANDOM_OPTION", nRandOptType ) );
							if( fVal[i] != 0.0f )
							{
								if ( nRandOptType == EMR_OPT_HMS_INC_RATE || 
									nRandOptType == EMR_OPT_HP_INC_RATE  || 
									nRandOptType == EMR_OPT_MP_INC_RATE  || 
									nRandOptType == EMR_OPT_SP_INC_RATE	 )
								{
									fVal[i] = (fVal[i]*100.0f);
								}

								if( fVal[i] > 0.0f) 
									strDescText += _T('+');

								if( sItemCustom.IsPerRandOpt( nRandOptType ) )
								{
									strDescText.AppendFormat( "%.2f", fVal[i] );
									strDescText += _T('%');
								}
								else
									strDescText.AppendFormat( "%d", int(fVal[i] ));

								AddTextNoSplit( strDescText, NS_UITEXTCOLOR::AQUA );
							}
						}
					}
				}
				break;

			case ITEM_PET_CARD:
				{
					AddTextNoSplit( "",NS_UITEXTCOLOR::WHITE);
					if ( sItemCustom.dwPetID == 0 ) break;
					PETCARDINFO_MAP_ITER iter = GLGaeaClient::GetInstance().GetCharacter()->m_mapPETCardInfo.find ( sItemCustom.dwPetID );
					if ( iter == GLGaeaClient::GetInstance().GetCharacter()->m_mapPETCardInfo.end() )
					{
						iter = GLGaeaClient::GetInstance().GetCharacter()->m_mapPETCardInfoTemp.find ( sItemCustom.dwPetID );
						if ( iter == GLGaeaClient::GetInstance().GetCharacter()->m_mapPETCardInfoTemp.end() ) break;
					}

					SPETCARDINFO sPetCardInfo = (*iter).second;

					strText.Format( "%s %s", ID2GAMEWORD ("ITEM_PET_NAME",0), sPetCardInfo.m_szName );
					AddTextNoSplit(strText,NS_UITEXTCOLOR::WHITE);

					strText.Format( "%s %s", ID2GAMEWORD ("ITEM_PET_TYPE",0), COMMENT::PET_TYPE[sPetCardInfo.m_emTYPE].c_str() );
					AddTextNoSplit(strText,NS_UITEXTCOLOR::WHITE);

					strText.Format( "%s %.2f%%", ID2GAMEWORD ("ITEM_PET_FULL",0), sPetCardInfo.m_nFull/10.0f );
					AddTextNoSplit(strText,NS_UITEXTCOLOR::WHITE);
					
					SITEM* pItem = GLItemMan::GetInstance().GetItem ( sPetCardInfo.m_PutOnItems[PET_ACCETYPE_A].sNativeID );
					if ( pItem )
					{
						strText.Format("%s %s", ID2GAMEWORD ("ITEM_PET_SLOTA",0), pItem->GetName () );
						AddTextNoSplit(strText,NS_UITEXTCOLOR::WHITE);
					}

					pItem = GLItemMan::GetInstance().GetItem ( sPetCardInfo.m_PutOnItems[PET_ACCETYPE_B].sNativeID );
					if ( pItem )
					{
						strText.Format("%s %s", ID2GAMEWORD ("ITEM_PET_SLOTB",0), pItem->GetName () );
						AddTextNoSplit(strText,NS_UITEXTCOLOR::WHITE);
					}

					if ( sItemCustom.tDISGUISE != 0 )
					{
						CTime currentTime = GLGaeaClient::GetInstance().GetCurrentTime();
						CTime startTime   = sItemCustom.tBORNTIME;
						CTimeSpan timeSpan = currentTime - startTime;
						if( timeSpan.GetTotalSeconds() < sItemCustom.tDISGUISE )
						{
							strText.Format("[%s]", ID2GAMEWORD ("ITEM_PET_USE_SKINPACK",0) );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::GOLD);
							strText.Format("%s : %dsec", ID2GAMEWORD ("CLUB_BATTLE_TIME",0), sItemCustom.tDISGUISE - timeSpan.GetTotalSeconds() );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::GOLD);
						}
					}

					AddTextNoSplit( ID2GAMEWORD ("ITEM_PET_SKILLS", 0 ), NS_UITEXTCOLOR::WHITE );

					PETSKILL_MAP_CITER pos = sPetCardInfo.m_ExpSkills.begin();
					PETSKILL_MAP_CITER pos_end = sPetCardInfo.m_ExpSkills.end();
					for ( ;pos != pos_end; ++pos )
					{
						const PETSKILL& sPetSkill = (*pos).second;
						PGLSKILL pSKILL = GLSkillMan::GetInstance().GetData( sPetSkill.sNativeID );
						if ( pSKILL )
						{
							strText.Format("%s", pSKILL->GetName() );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::WHITE);
						}
					}
				}
				break;

				/*case ITEM_SUMMON_CARD:
				{
					AddTextNoSplit("",NS_UITEXTCOLOR::WHITE);
					if ( pItemData->sBasicOp.sSumID!=SNATIVEID(false) )
					{
						SCROWDATA* pCrow =GLCrowDataMan::GetInstance().GetCrowData ( pItemData->sBasicOp.sSumID );

						if (pCrow)
						{
							{
								strText.Format("%s %s ", ID2GAMEWORD ("ITEM_SUMMON_INFO",0), pCrow->GetName () );
								AddTextNoSplit(strText,NS_UITEXTCOLOR::WHITE);
							}

							{
								strText.Format("%s %s ", ID2GAMEWORD ("ITEM_SUMMON_INFO",1),COMMENT::SUMMONTYPE[pItemData->sBasicOp.m_emSumType].c_str() );
								AddTextNoSplit(strText,NS_UITEXTCOLOR::WHITE);
							}

							{
								strText.Format("%s %d", ID2GAMEWORD ("ITEM_SUMMON_INFO",2), pCrow->m_sBasic.m_dwHP );
								AddTextNoSplit(strText,NS_UITEXTCOLOR::WHITE);
							}

							{
									strText.Format("%s %d ~ %d"
									, ID2GAMEWORD ("ITEM_SUMMON_INFO",3)
									, pCrow->m_sCrowAttack[0].sDamage.dwLow
									, pCrow->m_sCrowAttack[0].sDamage.dwHigh
									);
								AddTextNoSplit(strText,NS_UITEXTCOLOR::WHITE);
							}
						}
					}

				}
				break;*/
			}
			
			if( emItemType != ITEM_PET_SKILL )
			{
				if( pItemData->sDrugOp.emDrug == ITEM_DRUG_CALL_REGEN )
				{
					AddTextNoSplit("",NS_UITEXTCOLOR::WHITE);
					GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();	

					CString strName( GLGaeaClient::GetInstance().GetMapName( pCharacter->m_sStartMapID ) );
					if( strName == _T("(null)") ) strName.Empty();

					strText.Format( "%s :%s", ID2GAMEWORD("ITEM_CALL_MAP", 0 ), strName );
					AddTextNoSplit( strText, NS_UITEXTCOLOR::WHITE);

					SMAPNODE *pMapNode = GLGaeaClient::GetInstance().FindMapNode ( pCharacter->m_sStartMapID );
					if( pMapNode )
					{
						GLLevelFile cLevelFile;
						BOOL bOk = cLevelFile.LoadFile( pMapNode->strFile.c_str(), TRUE, NULL );
						if( bOk )
						{	
							D3DXVECTOR3 vStartPos;
							PDXLANDGATE pGate = cLevelFile.GetLandGateMan().FindLandGate( pCharacter->m_dwStartGate );
							if( pGate )
							{
								vStartPos = pGate->GetGenPos ( DxLandGate::GEN_RENDUM );
							}
							else
							{
								pGate = cLevelFile.GetLandGateMan().FindLandGate( DWORD(0) );
								if( pGate ) vStartPos = pGate->GetGenPos ( DxLandGate::GEN_RENDUM );
							}

							int nPosX(0), nPosY(0);
							cLevelFile.GetMapAxisInfo().Convert2MapPos( vStartPos.x, vStartPos.z, nPosX, nPosY );

							strText.Format( "%s:%d,%d", ID2GAMEWORD("ITEM_CALL_MAP", 1 ), nPosX, nPosY );
							AddTextNoSplit( strText, NS_UITEXTCOLOR::WHITE);
						}
					}
				}
				else if( pItemData->sDrugOp.emDrug == ITEM_DRUG_CALL_LASTCALL )
				{
					AddTextNoSplit("",NS_UITEXTCOLOR::WHITE);
					GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();

					if( pCharacter->m_sLastCallMapID.IsValidNativeID() )
					{
						CString strName( GLGaeaClient::GetInstance().GetMapName( pCharacter->m_sLastCallMapID ) );
						if( strName == _T("(null)") ) strName.Empty();

						strText.Format( "%s:%s", ID2GAMEWORD("ITEM_CALL_MAP", 0 ), strName );
						AddTextNoSplit( strText, NS_UITEXTCOLOR::WHITE);

						SMAPNODE *pMapNode = GLGaeaClient::GetInstance().FindMapNode ( pCharacter->m_sLastCallMapID );
						if( pMapNode )
						{
							GLLevelFile cLevelFile;
							BOOL bOk = cLevelFile.LoadFile( pMapNode->strFile.c_str(), TRUE, NULL );
							if( bOk )
							{	
								int nPosX(0), nPosY(0);
								cLevelFile.GetMapAxisInfo().Convert2MapPos( pCharacter->m_vLastCallPos.x, pCharacter->m_vLastCallPos.z, nPosX, nPosY );

								strText.Format( "%s:%d,%d", ID2GAMEWORD("ITEM_CALL_MAP", 1 ), nPosX, nPosY );
								AddTextNoSplit( strText, NS_UITEXTCOLOR::WHITE);
							}
						}
					}
					else
					{
						AddTextNoSplit("",NS_UITEXTCOLOR::WHITE);
						AddTextNoSplit( ID2GAMEWORD("ITEM_CALL_MAP", 2 ), NS_UITEXTCOLOR::RED);
					}
				}
				else if ( pItemData->sDrugOp.emDrug == ITEM_DRUG_CALL_TELEPORT )
				{
					CString strName( GLGaeaClient::GetInstance().GetMapName( pItemData->sBasicOp.sSubID ) );
					if( strName == _T("(null)") ) strName.Empty();

					strText.Format( "           %s :%s", ID2GAMEWORD("ITEM_TELEPORT_MAP", 0 ), strName );
					AddTextNoSplit( strText, NS_UITEXTCOLOR::WHITE);

					strText.Format( "%s :%d,%d", ID2GAMEWORD("ITEM_TELEPORT_MAP", 1 ), pItemData->sBasicOp.wPosX, pItemData->sBasicOp.wPosY );
					AddTextNoSplit( strText, NS_UITEXTCOLOR::WHITE);
				}

				BOOL bAccept(FALSE);

				if( sItemCustom.nidDISGUISE != NATIVEID_NULL() )
				{
					bAccept = pCharacter->ACCEPT_ITEM( sItemCustom.sNativeID, sItemCustom.nidDISGUISE );
				}
				else
				{
					bAccept = pCharacter->ACCEPT_ITEM( sItemCustom.sNativeID );
				}

				if ( bAccept )
				{
					DWORD dwNeedSP = pCharacter->CALC_ACCEPTP ( sItemCustom.sNativeID );
					if ( 0 < dwNeedSP )
					{
						strText.Format ( "%s:%d", ID2GAMEWORD("ITEM_NEEDSP" ), dwNeedSP );
						AddTextNoSplit(strText, NS_UITEXTCOLOR::RED );
					}
				}
			}

			{
				switch ( emItemType )
				{
				case ITEM_SUIT:
				case ITEM_REVIVE:
				case ITEM_ANTI_DISAPPEAR:
					{
						/*const int nELEC   = sItemCustom.GETRESIST_ELEC();
						const int nFIRE   = sItemCustom.GETRESIST_FIRE();
						const int nICE    = sItemCustom.GETRESIST_ICE();
						const int nPOISON = sItemCustom.GETRESIST_POISON();
						const int nSPIRIT = sItemCustom.GETRESIST_SPIRIT();

						if ( nELEC || nFIRE || nICE || nPOISON || nSPIRIT )
						{
							strText.Format ( "Resistance :" );

							if ( nELEC )
							{
								strText.AppendFormat(" %s %d", ID2GAMEWORD("ITEM_ADVANCED_INFO_RESIST", 0 ), nELEC );	
								BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDER_RESIST_ELEC);
								APPEND_ITEM_GRADE ( strText, uGRADE );
							}
							if ( nFIRE )
							{
								strText.AppendFormat(" %s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_RESIST", 1 ), nFIRE );
								BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDER_RESIST_FIRE);
								APPEND_ITEM_GRADE ( strText, uGRADE );
							}
							if ( nICE )
							{
								strText.AppendFormat(" %s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_RESIST", 2 ), nICE );
								BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDER_RESIST_ICE);
								APPEND_ITEM_GRADE ( strText, uGRADE );
							}
							if ( nPOISON )
							{
								strText.AppendFormat(" %s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_RESIST", 3 ), nPOISON );
								BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDER_RESIST_POISON);
								APPEND_ITEM_GRADE ( strText, uGRADE );
							}
							if ( nSPIRIT )
							{
								strText.AppendFormat(" %s:%d", ID2GAMEWORD("ITEM_ADVANCED_INFO_RESIST", 4 ), nSPIRIT );
								BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDER_RESIST_SPIRIT);
								APPEND_ITEM_GRADE ( strText, uGRADE );
							}

							AddTextNoSplit( "", NS_UITEXTCOLOR::RED );
							AddTextNoSplit(strText,NS_UITEXTCOLOR::GREEN);
						}*/

						const int nELEC   = sItemCustom.GETRESIST_ELEC();
						const int nFIRE   = sItemCustom.GETRESIST_FIRE();
						const int nICE    = sItemCustom.GETRESIST_ICE();
						const int nPOISON = sItemCustom.GETRESIST_POISON();
						const int nSPIRIT = sItemCustom.GETRESIST_SPIRIT();
						
						CBasicTextBox::MULTITEXT mlText;
						if ( nELEC || nFIRE || nICE || nPOISON || nSPIRIT )
						{
							mlText.strText[0] = "";
							mlText.dwColor[0] = NS_UITEXTCOLOR::WHITE;

							if ( nELEC )
							{
								CString strAdd;
								strAdd.Format ( "Elec:%d", nELEC );
								mlText.strText[1] = strAdd;
								mlText.dwColor[1] = NS_UITEXTCOLOR::GREEN;
								BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_RESIST_ELEC);
								if ( uGRADE == 0 )
								{
								CString strCombine;
								strCombine.Format( "(+0) ", uGRADE);
								mlText.strText[2] = strCombine;
								mlText.dwColor[2] = NS_UITEXTCOLOR::YELLOW;
								}
								else if ( uGRADE > 0 )
								{
									CString strCombine;
									strCombine.Format( "(+%d) ", uGRADE);
									mlText.strText[2] = strCombine;
									mlText.dwColor[2] = NS_UITEXTCOLOR::YELLOW;
								}
							}
						
							if ( nFIRE )
							{						
								CString strAdd;
								strAdd.Format ( "Fire:%d", nFIRE );
								mlText.strText[3] = strAdd;
								mlText.dwColor[3] = NS_UITEXTCOLOR::GREEN;
								BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_RESIST_FIRE);
								if ( uGRADE == 0 )
								{
								CString strCombine;
								strCombine.Format( "(+0) ", uGRADE);
								mlText.strText[4] = strCombine;
								mlText.dwColor[4] = NS_UITEXTCOLOR::YELLOW;
								}
								else if ( uGRADE > 0 )
								{
									CString strCombine;
									strCombine.Format( "(+%d) ", uGRADE);
									mlText.strText[4] = strCombine;
									mlText.dwColor[4] = NS_UITEXTCOLOR::YELLOW;
								}
							}
						
							if ( nICE )
							{							
								CString strAdd;
								strAdd.Format ( "Ice:%d", nICE );
								mlText.strText[5] = strAdd;
								mlText.dwColor[5] = NS_UITEXTCOLOR::GREEN;
								BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_RESIST_ICE);
								if ( uGRADE == 0 )
								{
								CString strCombine;
								strCombine.Format( "(+0) ", uGRADE);
								mlText.strText[6] = strCombine;
								mlText.dwColor[6] = NS_UITEXTCOLOR::YELLOW;
								}
								else if ( uGRADE > 0 )
								{
									CString strCombine;
									strCombine.Format( "(+%d) ", uGRADE);
									mlText.strText[6] = strCombine;
									mlText.dwColor[6] = NS_UITEXTCOLOR::YELLOW;
								}
							}
						
							if ( nPOISON )
							{
								if ( nPOISON==0 )
								{							
								CString strAdd;
								strAdd.Format ( "Poison:%d", nPOISON );
								mlText.strText[7] = strAdd;
								mlText.dwColor[7] = NS_UITEXTCOLOR::GREEN;
								BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_RESIST_POISON);
								if ( uGRADE == 0 )
								{
								CString strCombine;
								strCombine.Format( "(+0) ", uGRADE);
								mlText.strText[8] = strCombine;
								mlText.dwColor[8] = NS_UITEXTCOLOR::YELLOW;
								}
								else if ( uGRADE > 0 )
								{
									CString strCombine;
									strCombine.Format( "(+%d) ", uGRADE);
									mlText.strText[8] = strCombine;
									mlText.dwColor[8] = NS_UITEXTCOLOR::YELLOW;
								}
							}
						
							if ( nSPIRIT )
							{								
								CString strAdd;
								strAdd.Format ( "Wind:%d", nSPIRIT );
								mlText.strText[9] = strAdd;
								mlText.dwColor[9] = NS_UITEXTCOLOR::GREEN;
								BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_RESIST_SPIRIT);
								if ( uGRADE == 0 )
								{
								CString strCombine;
								strCombine.Format( "(+0) ", uGRADE);
								mlText.strText[10] = strCombine;
								mlText.dwColor[10] = NS_UITEXTCOLOR::YELLOW;
								}
								else if ( uGRADE > 0 )
								{
									CString strCombine;
									strCombine.Format( "(+%d)", uGRADE);
									mlText.strText[10] = strCombine;
									mlText.dwColor[10] = NS_UITEXTCOLOR::YELLOW;
								}
							}
						}
							//AddTextNoSplit( "", NS_UITEXTCOLOR::RED );
							AddMultiText ( mlText );
						}

					}					break;
				};

				GLCharacter* pCharacter = GLGaeaClient::GetInstance().GetCharacter();
				if ( pCharacter && pCharacter->IsCoolTime( pItemData->sBasicOp.sNativeID ) )
				{
					CString strTime = "";
					__time64_t tCurTime =  GLGaeaClient::GetInstance().GetCurrentTime().GetTime();
					__time64_t tCoolTime = pCharacter->GetMaxCoolTime( pItemData->sBasicOp.sNativeID );				

					CTimeSpan cReTime( tCoolTime - tCurTime );

					if ( cReTime.GetHours() > 0 )	
						strTime += CInnerInterface::GetInstance().MakeString( "%d%s ", cReTime.GetHours(), ID2GAMEWORD ( "ITEM_BASIC_TIME", 0 ) );
					if ( cReTime.GetMinutes() > 0 )	
						strTime += CInnerInterface::GetInstance().MakeString( "%d%s ", cReTime.GetMinutes(), ID2GAMEWORD ( "ITEM_BASIC_TIME", 1 ) );
					if ( cReTime.GetSeconds() > 0 )	
						strTime += CInnerInterface::GetInstance().MakeString( "%d%s ", cReTime.GetSeconds(), ID2GAMEWORD ( "ITEM_BASIC_TIME", 2 ) );

					strText.Format ( "%s : %s", ID2GAMEWORD ( "ITEM_BASIC_INFO", 9 ), strTime );			
					AddTextNoSplit ( strText, NS_UITEXTCOLOR::DARKORANGE );
				}

				if ( pItemData->IsTIMELMT () )
				{
					CTime cTime(sItemCustom.tBORNTIME);
					if ( cTime.GetYear()!=1970 )
					{
						CTimeSpan sLMT(pItemData->sDrugOp.tTIME_LMT);
						cTime += sLMT;

						CString strExpireDate;
						strExpireDate = CInnerInterface::GetInstance().MakeString ( ID2GAMEWORD("ITEM_EXPIRE_DATE"),
							(cTime.GetYear ()%2000), cTime.GetMonth (), cTime.GetDay (), cTime.GetHour (), cTime.GetMinute () );
						strText.Format ( "%s:%s", ID2GAMEWORD ( "ITEM_BASIC_INFO", 4 ), strExpireDate );			
						AddTextNoSplit ( strText, NS_UITEXTCOLOR::DARKORANGE );
					}
				}
							DWORD dwCOMMISSION_MONEY = 0;

			}

			//add money 2
			{

				DWORD dwFlags = pItemData->sBasicOp.dwFlags;
                DWORD IsCanDropTrade = sItemCustom.cISDROPITEM;
                DWORD IsDrop =   dwFlags & TRADE_THROW;
                DWORD IsTrade = dwFlags & TRADE_EXCHANGE;

				CString szFlag;
					
				if ( dwFlags )
				{
					CBasicTextBox::MULTITEXT mlText;

					mlText.strText[0] = ID2GAMEWORD( "ITEM_FLAG", 0 );
					mlText.strText[1] = " / ";
					mlText.strText[2] = ID2GAMEWORD( "ITEM_FLAG", 1 );
					mlText.strText[3] = " / ";
					mlText.strText[4] = ID2GAMEWORD( "ITEM_FLAG", 2 );
					mlText.strText[5] = " / ";
					mlText.strText[6] = ID2GAMEWORD( "ITEM_FLAG", 3 );
					mlText.strText[7] = " / ";
					mlText.strText[8] = ID2GAMEWORD( "ITEM_FLAG", 4 );
					mlText.strText[9] = " / ";
					mlText.strText[10] = ID2GAMEWORD( "ITEM_FLAG", 5 );
					mlText.strText[11] = " / ";
					mlText.strText[12] = ID2GAMEWORD( "ITEM_FLAG", 8 );
					mlText.strText[13] = " / ";
					mlText.strText[14] = ID2GAMEWORD( "ITEM_FLAG", 9 );
					mlText.strText[15] = " / ";
					mlText.strText[16] = ID2GAMEWORD( "ITEM_FLAG", 10 );

					mlText.dwColor[0] = (dwFlags&TRADE_SALE) ? NS_UITEXTCOLOR::WHITE : NS_UITEXTCOLOR::DARKGRAY;
					mlText.dwColor[1] = NS_UITEXTCOLOR::WHITE;
					//modification for undrop and nontrade upgrade ^_^ 
					mlText.dwColor[2] = ( !IsDrop && IsCanDropTrade==0 || IsDrop && IsCanDropTrade==1 ) ? NS_UITEXTCOLOR::DARKGRAY : NS_UITEXTCOLOR::WHITE;
					mlText.dwColor[3] = NS_UITEXTCOLOR::DARKGRAY;
					//modification for undrop and nontrade upgrade ^_^ 
					mlText.dwColor[4] = ( !IsTrade && IsCanDropTrade ==0 || IsTrade && IsCanDropTrade ==1 ) ? NS_UITEXTCOLOR::DARKGRAY : NS_UITEXTCOLOR::WHITE;
					mlText.dwColor[5] = NS_UITEXTCOLOR::DARKGRAY;
					mlText.dwColor[6] = (dwFlags&TRADE_EVENT_SGL) ? NS_UITEXTCOLOR::WHITE : NS_UITEXTCOLOR::DARKGRAY;
					mlText.dwColor[7] = NS_UITEXTCOLOR::WHITE;
					mlText.dwColor[8] = (dwFlags&TRADE_GARBAGE) ? NS_UITEXTCOLOR::WHITE : NS_UITEXTCOLOR::DARKGRAY;
					mlText.dwColor[9] = NS_UITEXTCOLOR::WHITE;
					mlText.dwColor[10] = (dwFlags&TRADE_LOCKER) ? NS_UITEXTCOLOR::WHITE : NS_UITEXTCOLOR::DARKGRAY;
					mlText.dwColor[11] = NS_UITEXTCOLOR::WHITE;
					mlText.dwColor[12] = (dwFlags&ITEM_OWNERSHIP) ? NS_UITEXTCOLOR::WHITE : NS_UITEXTCOLOR::DARKGRAY;
					mlText.dwColor[13] = NS_UITEXTCOLOR::WHITE;
					mlText.dwColor[14] = (dwFlags&ITEM_POINT_RECOVER) ? NS_UITEXTCOLOR::WHITE : NS_UITEXTCOLOR::DARKGRAY;
					mlText.dwColor[15] = NS_UITEXTCOLOR::WHITE;
					mlText.dwColor[16] = (dwFlags&ITEM_COSTUME_COMBINE) ? NS_UITEXTCOLOR::WHITE : NS_UITEXTCOLOR::DARKGRAY;

					AddMultiTextLongestLineSplit( mlText );
				}

				if (pItemData->sBasicOp.emLevel == LEVEL_VOTE_ITEM )	szFlag.AppendFormat( "%s ", ID2GAMEWORD( "ITEM_FLAG", 6 ));
				if (pItemData->sBasicOp.emLevel == LEVEL_PREMIUM_ITEM )	szFlag.AppendFormat( "%s ", ID2GAMEWORD( "ITEM_FLAG", 7 ));
				if (pItemData->sBasicOp.emLevel == LEVEL_CONTRIBUTION_ITEM )	szFlag.AppendFormat( "%s ", ID2GAMEWORD( "ITEM_FLAG", 8 ));
				
				strText.Format( "%s", szFlag.GetString() );
				AddTextNoSplit ( strText, NS_UITEXTCOLOR::WHITE );

				CBasicTextBox::MULTITEXT mlText;

				if ( dwFlags )
				{
					
				}

			}

			{

				DWORD dwOwner = sItemCustom.dwOwnerID;
				DWORD dwOwnerID =GLGaeaClient::GetInstance().GetCharacter()->m_dwCharID;

				if ( pItemData->sBasicOp.IsOwner() )
				{
					if (pItemData->sBasicOp.emLevel == LEVEL_VOTE_ITEM )
					{
						strText.Format( "Vote Item Owner ID: %d", dwOwner );
						if ( !( dwOwnerID == dwOwner))
						{
							AddTextNoSplit ( strText, NS_UITEXTCOLOR::RED );
						}
						else
						{
							AddTextNoSplit ( strText, NS_UITEXTCOLOR::WHITE );
						}
					}

					if (pItemData->sBasicOp.emLevel == LEVEL_PREMIUM_ITEM )
					{
						strText.Format( "Premium Item Owner ID: %d", dwOwner );
						if ( !( dwOwnerID == dwOwner))
						{
							AddTextNoSplit ( strText, NS_UITEXTCOLOR::RED );
						}
						else
						{
							AddTextNoSplit ( strText, NS_UITEXTCOLOR::WHITE );
						}
					}

				}
			}

			if ( pItemData->sBasicOp.dwBuyPrice || bInPrivateMarket )
			{
				if ( bInPrivateMarket )
				{
					bool bOPENER;
					DWORD dwPrivateMarketID;
					CInnerInterface::GetInstance().GetPrivateMarketInfo ( bOPENER, dwPrivateMarketID );
					if ( bOPENER )	
					{
						GLPrivateMarket &sPMarket = GLGaeaClient::GetInstance().GetCharacter()->m_sPMarket;

						const SSALEITEM *pSALE = sPMarket.GetItem ( SNATIVEID(wPosX,wPosY) ); 
						if ( pSALE )
						{
							CString strMoney = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( pSALE->llPRICE, 3, "," );
							strText.Format ( "%s :%s", ID2GAMEWORD ( "ITEM_BASIC_INFO", 5 ), strMoney );			
							AddTextNoSplit ( strText, NS_UITEXTCOLOR::GOLD );
						}						
					}
					else
					{
						PGLCHARCLIENT pCLIENT = GLGaeaClient::GetInstance().GetChar ( dwPrivateMarketID );
						if ( !pCLIENT ) return ;

						GLPrivateMarket &sPMarket = pCLIENT->m_sPMarket;

						const SSALEITEM *pSALE = sPMarket.GetItem ( SNATIVEID(wPosX,wPosY) ); 
						if ( pSALE )
						{
							CString strMoney = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( pSALE->llPRICE, 3, "," );
							strText.Format ( "%s :%s", ID2GAMEWORD ( "ITEM_BASIC_INFO", 5 ), strMoney );	
							AddTextNoSplit ( strText, NS_UITEXTCOLOR::GOLD );
						}
					}
				}				
				else if ( bShopOpen && bInMarket )	// npc open , check item inside inventory ( item price inside npc)
				{
					LONGLONG dwNpcSellPrice = 0;

					volatile LONGLONG dwPrice = 0;
					volatile float fSHOP_RATE = GLGaeaClient::GetInstance().GetCharacter()->GetBuyRate();
					volatile float fSHOP_RATE_C = fSHOP_RATE * 0.01f;

					//if( sNpcNativeID.wMainID != 0 && sNpcNativeID.wSubID != 0 ) //npc itemprice fix
					{
						PCROWDATA pCrowData = GLCrowDataMan::GetInstance().GetCrowData ( sNpcNativeID );
						if( pCrowData != NULL )
						{
							LONGLONG dwNpcPrice = pCrowData->GetNpcSellPrice( pItemData->sBasicOp.sNativeID.dwID );
							//add money 2
							//no tax for vote item
							if ( pItemData->sBasicOp.emLevel == LEVEL_VOTE_ITEM )
							{
								dwPrice = pItemData->sBasicOp.dwBuyPrice;
							}
							//add money 3
							//no tax for premium item
							else if ( pItemData->sBasicOp.emLevel == LEVEL_PREMIUM_ITEM )
							{
								dwPrice = pItemData->sBasicOp.dwBuyPrice;
							}
							//add contributionpoint by CNDev
							else if ( pItemData->sBasicOp.emLevel == LEVEL_CONTRIBUTION_ITEM )
							{
								dwPrice = pItemData->sBasicOp.dwBuyPrice;
							}
							else
							{			
								if( dwNpcPrice == 0 )
								{								
									dwNpcSellPrice = pItemData->sBasicOp.dwBuyPrice;
									dwPrice = DWORD ( (float)dwNpcSellPrice * fSHOP_RATE_C );
								}
								else
								{
									dwNpcSellPrice = dwNpcPrice;
									dwPrice = dwNpcSellPrice;								
								}
							}
						}
					}				

					
					CString strMoney = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( dwPrice, 3, "," );

					/*strText.Format( "%s:%s", ID2GAMEWORD("ITEM_BASIC_INFO", 1), strMoney );
					*/

					//add money 2 checking price red 
		
					//	커미션 액수
					//add money 2
					//no tax for vote item
					if ( pItemData->sBasicOp.emLevel == LEVEL_VOTE_ITEM )
					{
						dwCOMMISSION_MONEY  = 0;
					}
					//add money 3
					//no tax for premium item
					else if ( pItemData->sBasicOp.emLevel == LEVEL_PREMIUM_ITEM )
					{
						dwCOMMISSION_MONEY  = 0;
					}
					//add contributionpoint by CNDev
					else if ( pItemData->sBasicOp.emLevel == LEVEL_CONTRIBUTION_ITEM )
					{
						dwCOMMISSION_MONEY  = 0;
					}
					else
					{
						dwCOMMISSION_MONEY = 0/*dwPrice - dwNpcSellPrice*/;
					}
					
					D3DCOLOR dwColor = NS_UITEXTCOLOR::RED;
					/*if( dwPrice <= GLGaeaClient::GetInstance().GetCharacterLogic().m_lnMoney )
					{
						dwColor = NS_UITEXTCOLOR::PALEGREEN;
					}*/
					//add money 2

					// done price color respond with item gold price or item  premium price
					//add money 2
					if ( pItemData->sBasicOp.emLevel == LEVEL_VOTE_ITEM ) // check price color for premium items
					{
						strText.Format( "%s:%s", ID2GAMEWORD("ITEM_BASIC_INFO", 11), strMoney );

						if( dwPrice <= GLGaeaClient::GetInstance().GetCharacterLogic().m_lnVoteP )
						{
							dwColor = NS_UITEXTCOLOR::LIGHTSKYBLUE;
						}
					}
					//add money 3
					else if ( pItemData->sBasicOp.emLevel == LEVEL_PREMIUM_ITEM ) // check price color for premium items
					{
						strText.Format( "%s:%s", ID2GAMEWORD("ITEM_BASIC_INFO", 13), strMoney );

						if( dwPrice <= GLGaeaClient::GetInstance().GetCharacterLogic().m_lnPremP )
						{
							dwColor = NS_UITEXTCOLOR::LIGHTSKYBLUE;
						}
					}
					//add contributionpoint by CNDev
					else if ( pItemData->sBasicOp.emLevel == LEVEL_CONTRIBUTION_ITEM ) // check price color for premium items
					{
						strText.Format( "%s:%s", ID2GAMEWORD("ITEM_BASIC_INFO", 16), strMoney );

						if( dwPrice <= GLGaeaClient::GetInstance().GetCharacterLogic().m_lnContribP )
						{
							dwColor = NS_UITEXTCOLOR::LIGHTSKYBLUE;
						}
					}

					else
					{
						strText.Format( "%s:%s", ID2GAMEWORD("ITEM_BASIC_INFO", 1), strMoney );

						if( dwPrice <= GLGaeaClient::GetInstance().GetCharacterLogic().m_lnMoney )
						{
							dwColor = NS_UITEXTCOLOR::PALEGREEN;
						}
					}
				
					AddTextNoSplit ( strText, dwColor );
				}
				else if ( bShopOpen ) // npc open (check item price of items inside inventory not inside npc)	// 
				{
					if ( pItemData->sBasicOp.IsPointRecover() )
					{//add and modified for //add point sell
						volatile DWORD dwPrice = pItemData->GETSELLPRICE ( sItemCustom.wTurnNum );
						dwCOMMISSION_MONEY = 0;
						CString strMoney = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( dwPrice, 3, "," );

						if ( pItemData->sBasicOp.emLevel == LEVEL_VOTE_ITEM )
						{	
							strText.Format( "%s:%s", ID2GAMEWORD("ITEM_BASIC_INFO", 14), strMoney );
							AddTextNoSplit ( strText, NS_UITEXTCOLOR::PALEGREEN );
						}
						else if ( pItemData->sBasicOp.emLevel == LEVEL_PREMIUM_ITEM )
						{	
							strText.Format( "%s:%s", ID2GAMEWORD("ITEM_BASIC_INFO", 15), strMoney );
							AddTextNoSplit ( strText, NS_UITEXTCOLOR::PALEGREEN );
						}
						else if ( pItemData->sBasicOp.emLevel == LEVEL_CONTRIBUTION_ITEM )
						{
							strText.Format( "%s:%s", ID2GAMEWORD("ITEM_BASIC_INFO", 17), strMoney );
							AddTextNoSplit ( strText, NS_UITEXTCOLOR::PALEGREEN );
						}
						else
						{
							AddTextNoSplit ( "Wrong item setting, enable only point recover if item is set to premium or vote", NS_UITEXTCOLOR::RED );
						}
					}
					else
					{//default
						volatile float fSHOP_RATE = GLGaeaClient::GetInstance().GetCharacter()->GetSaleRate();
						volatile float fSALE_DISCOUNT = fSHOP_RATE * 0.01f;

						volatile DWORD dwPrice = pItemData->GETSELLPRICE ( sItemCustom.wTurnNum );
						volatile DWORD dwSALE_PRICE = DWORD ( dwPrice * fSALE_DISCOUNT );					

						//	커미션 액수
						volatile DWORD dwDISPRICE = pItemData->GETSELLPRICE ( sItemCustom.wTurnNum );
						dwCOMMISSION_MONEY = dwDISPRICE - dwSALE_PRICE;

						CString strMoney = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( dwSALE_PRICE, 3, "," );
						strText.Format( "%s:%s", ID2GAMEWORD("ITEM_BASIC_INFO", 12), strMoney );

						AddTextNoSplit ( strText, NS_UITEXTCOLOR::PALEGREEN );
					}

					//CString strErr;
					//strErr.Format ( "fSHOP_RATE:%f / fSALE_DISCOUNT:%f / dwPrice:%d /dwSALE_PRICE:%d ",
					//fSHOP_RATE,fSALE_DISCOUNT,dwPrice,dwSALE_PRICE );
					//MessageBox ( NULL, strErr.GetString(), "ERROR", MB_OK );

				}
			}

			//	수수료 표시
			if ( dwCOMMISSION_MONEY )
			{
				CString strMoney = NS_UITEXTCONTROL::MAKE_MONEY_FORMAT ( dwCOMMISSION_MONEY, 3, "," );
				strText.Format( "%s:%s", ID2GAMEWORD("ITEM_BASIC_INFO", 6), strMoney );

				AddTextNoSplit ( strText, NS_UITEXTCOLOR::PALEGREEN );
			}

			{ // ??
				const TCHAR * pszComment(NULL);

				if ( (emItemType != ITEM_SKILL) || (emItemType != ITEM_PET_SKILL) ) pszComment = pItemData->GetComment();
				else
				{
					SNATIVEID sNativeID = sItemCustom.sNativeID;
					SITEM* pItemData = GLItemMan::GetInstance().GetItem ( sNativeID );
					SNATIVEID sSkillID = pItemData->sSkillBookOp.sSkill_ID;

					PGLSKILL pSkill = GLSkillMan::GetInstance().GetData ( sSkillID.wMainID, sSkillID.wSubID );
					if ( pSkill ) pszComment = pSkill->GetDesc();
				}

				if( pszComment != "" )
				{
					int StrLength = static_cast<int>( strlen( pszComment ) );
					if( StrLength )
					{
						CBasicTextBox::MULTITEXT mlTEXT;
						mlTEXT.strText[0] = pszComment;
						mlTEXT.dwColor[0] = NS_UITEXTCOLOR::WHITE;
						AddMultiTextLongestLineSplit( mlTEXT );
						//AddTextNoSplit ( "", NS_UITEXTCOLOR::LIGHTSKYBLUE );
						//AddTextLongestLineSplit( pszComment, NS_UITEXTCOLOR::WHITE );
					}
				}
			}

			{	//Name Display
				CString strName, strCombine;
				DWORD dwLevel = (DWORD)pItemData->sBasicOp.emLevel;
				BYTE uGRADE = sItemCustom.GETGRADE(EMGRINDING_NUNE);

				if ( bIsWEAR_ITEM )
				{
						strName.Format ( "          %s(Equipped)", pItemData->GetName());
				}
				else
				{
					if ( uGRADE == 0 ) 
					{
							strName.Format ( "          %s", pItemData->GetName());
					}
					else
					{
							strName.Format ( "         +%d %s", uGRADE, pItemData->GetName());
					}
				}

				D3DCOLOR dwCOLOR;
				switch ( pItemData->sBasicOp.emLevel )
				{
					case LEVEL_NORMAL:				{	dwCOLOR = NS_UITEXTCOLOR::NORMAL;		}	break;
					case LEVEL_RARE:				{	dwCOLOR = NS_UITEXTCOLOR::RARE;			}	break;
					case LEVEL_HEROIC:				{	dwCOLOR = NS_UITEXTCOLOR::HEROIC;		}	break;
					case LEVEL_ANCIENT:				{	dwCOLOR = NS_UITEXTCOLOR::ANCIENT;			}	break;
					case LEVEL_LEGENDARY:			{	dwCOLOR = NS_UITEXTCOLOR::LEGENDARY;		}	break;
					case LEVEL_VOTE_ITEM:			{	dwCOLOR = NS_UITEXTCOLOR::VOTEITEM;		}	break;
					case LEVEL_PREMIUM_ITEM:		{	dwCOLOR = NS_UITEXTCOLOR::PREMIUMITEM;	}	break;
					case LEVEL_CONTRIBUTION_ITEM:	{	dwCOLOR = NS_UITEXTCOLOR::LITEBLUE;	}	break;
					case LEVEL_HIGH_CLASS:			{	dwCOLOR	= NS_UITEXTCOLOR::HIGHCLASS; } break;
				}

				AddItemName ( strName, dwCOLOR );
			}	//End of Name Display

			{	//Item Texture
				if ( sNpcNativeID != sItemCustom.sNativeID ) 
				{
					sNpcNativeID = sItemCustom.sNativeID;
			
					if ( sNpcNativeID.IsValidNativeID() )
					{
						SITEM* pItem = GLItemMan::GetInstance().GetItem ( sNpcNativeID );
						if ( !pItem )
						{
							GASSERT ( 0 && "" );
							return ;
							ResetItemRender ();
							ResetItemInBox ();
							ResetItemInRandomBox();
						}
                        SITEMCUSTOM sCUSTOM;
						AddItemRender ( pItem->sBasicOp.sICONID, pItem->GetInventoryFile(), sCUSTOM );	 
					}
				}
			}	//End of Item Texture
		}
	}
};
