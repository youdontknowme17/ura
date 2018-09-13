#include "stdafx.h"
#include "s_COdbcManager.h"
#include "s_CDbAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// ĳ���͸� �����Ѵ�
int	COdbcManager::SaveCharacter(LPVOID _pbuffer)
{	
	if (_pbuffer == NULL) return DB_ERROR;

	SCHARDATA2* pCharData2 = reinterpret_cast<SCHARDATA2*> ( _pbuffer );	

	DWORD dwChaNum = pCharData2->m_dwCharID;
	
	// ��ŷ�� ĳ������ ��� ������ 0 ���� ���õ� ���ɼ��� ����.	
	if (dwChaNum == 0 || pCharData2->m_wLevel == 0)
	{
		return DB_ERROR;
	}
	
	char szNick[CHAR_SZNAME] = {0}; // Club nick name
	DWORD dwUserNum = pCharData2->GetUserID();
	DWORD dwSvrNum = pCharData2->m_dwServerID;
	DWORD dwChaID = pCharData2->m_dwCharID;	

	StringCchCopy(szNick, CHAR_SZNAME, pCharData2->m_szNick);

	// ��Ʈ�� Ž������ �ý����� �߰��� ĳ���� �����ϴ� �κ��� ����

		TCHAR szTemp[1024] = {0};
		_snprintf( szTemp, 1024, "UPDATE ChaInfo SET ChaBright=%d, ChaLevel=%u, ChaMoney=%I64d, "
					"ChaDex=%u, ChaIntel=%u, ChaSchool=%u, ChaHair=%u, ChaFace=%u, "
					"ChaLiving=%d, ChaStrong=%u, ChaPower=%u, ChaSpirit=%u, ChaStrength=%u, "
					"ChaStRemain=%u, ChaAttackP=%u, ChaDefenseP=%u, ChaFightA=%u, ChaShootA=%u, "
					"ChaExp=%I64d, ChaSkillPoint=%u, ChaHP=%u, ChaMP=%u, ChaSP=%u, ChaPK=%u, "
					"ChaStartMap=%u, ChaStartGate=%u, ChaPosX=%f, ChaPosY=%f, ChaPosZ=%f, "
					"ChaSaveMap=%d, ChaSavePosX=%f, ChaSavePosY=%f, ChaSavePosZ=%f, ChaReturnMap=%d, "
					"ChaReturnPosX=%f, ChaReturnPosY=%f, ChaReturnPosZ=%f, ChaGuName='%s', ChaReExp=%I64d, "
					"ChaSpMID = %u, ChaSpSID = %u, ChaReborn = %u, ChaPkWin = %u, ChaPkLoss = %u, "
					"ChaVotePoint=%I64d, ChaPremiumPoint=%I64d, ChaContributionPoint=%I64d, ChaCP=%u "
					"WHERE ChaNum=%u", 
		pCharData2->m_nBright, 
		pCharData2->m_wLevel, 
		pCharData2->m_lnMoney, 
		pCharData2->m_sStats.dwDex, 
		pCharData2->m_sStats.dwInt, 
		pCharData2->m_wSchool, 
		pCharData2->m_wHair, 
		pCharData2->m_wFace, 
		pCharData2->m_nLiving, 
		pCharData2->m_sStats.dwStr, 
		pCharData2->m_sStats.dwPow, 
		pCharData2->m_sStats.dwSpi, 
		pCharData2->m_sStats.dwSta, 
		pCharData2->m_wStatsPoint, 
		pCharData2->m_wAP, 
		pCharData2->m_wDP, 
		pCharData2->m_wPA, 
		pCharData2->m_wSA,
		pCharData2->m_sExperience.lnNow, 
		pCharData2->m_dwSkillPoint, 
		pCharData2->m_sHP.dwData, 
		pCharData2->m_sMP.dwData, 
		pCharData2->m_sSP.dwData, 
		pCharData2->m_wPK, 
		pCharData2->m_sStartMapID.dwID,
		pCharData2->m_dwStartGate, 
		pCharData2->m_vStartPos.x, 
		pCharData2->m_vStartPos.y, 
		pCharData2->m_vStartPos.z,
		(int) pCharData2->m_sSaveMapID.dwID, 
		pCharData2->m_vSavePos.x, 
		pCharData2->m_vSavePos.y, 
		pCharData2->m_vSavePos.z, 
		(int) pCharData2->m_sLastCallMapID.dwID, 
		pCharData2->m_vLastCallPos.x,
		pCharData2->m_vLastCallPos.y, 
		pCharData2->m_vLastCallPos.z, 
		szNick,
		pCharData2->m_lnReExp,
		pCharData2->m_sSummonPosionID.wMainID,
		pCharData2->m_sSummonPosionID.wSubID,

		pCharData2->m_dwReborn, 	//add reborn
		pCharData2->m_dwPkWin, 		//add pk
		pCharData2->m_dwPkLoss, 	//add pk
		pCharData2->m_lnVoteP,	//add money 2
		pCharData2->m_lnPremP,  //add money 3
		pCharData2->m_lnContribP, //add contributionpoint by CNDev
		pCharData2->m_sCP.dwData,	//add cp

		dwChaNum );
	/////////////////////////////////////////////////////////////////////////////////////////////

	CString strUserID = pCharData2->m_szUID;
	strUserID.Trim(_T(" "));
	strUserID.Replace(_T("'"), _T("''"));

	TCHAR szTemp2[256] = { 0 };

	_snprintf(szTemp2, 256, "UPDATE UserInfo "
							"SET ChaSaldo=%I64d "
							"WHERE UserNum = %d "
							"EXEC OdbcTopUp02 @szUserID = '%s' ",
							pCharData2->m_lnSaldo,
							dwUserNum,
							strUserID.GetString());

			

	if (m_pGameDB->ExecuteSQL(szTemp) != DB_OK)
	{
//		strTemp.freeze( false );	// Note : std::strstream�� freeze. �� �ϸ� Leak �߻�.
		return DB_ERROR;
	}

	if (m_pUserDB->ExecuteSQL(szTemp2) != DB_OK)
	{
		return DB_ERROR;
	}

	// ĳ���� �⺻ ���� Log�� �����.
	CConsoleMessage::GetInstance()->Write( LOG_TEXT_CHARSAVE, szTemp );

//	strTemp.freeze( false );	// Note : std::strstream�� freeze. �� �ϸ� Leak �߻�.
	
	CByteStream ByteStream;
	LPBYTE pBuffer = NULL;
	DWORD dwSize = 0;
	
	// Skill 
	pCharData2->GETEXPSKILLS_BYBUF(ByteStream);
	ByteStream.GetBuffer(pBuffer, dwSize);
	if (pBuffer != NULL)
	{
		//std::strstream strSkills;
		//strSkills << _T("UPDATE ChaInfo SET ChaInfo.ChaSkills=? WHERE (ChaNum=" <<  dwChaNum << ")");
		//strSkills << std::ends;

		TCHAR szTemp[128] = {0};
		_snprintf( szTemp, 128, "UPDATE ChaInfo SET ChaInfo.ChaSkills=? WHERE (ChaNum=%u)", dwChaNum );

		m_pGameDB->WriteImage( szTemp, dwChaNum, (BYTE *) pBuffer, dwSize );

		//strSkills.freeze( false );	// Note : std::strstream�� freeze. �� �ϸ� Leak �߻�.
		pBuffer = NULL;
	}

	// Skill Quick Slot	
	pCharData2->GETSKILL_QUICKSLOT(ByteStream);
	ByteStream.GetBuffer(pBuffer, dwSize);
	if (pBuffer != NULL)
	{
		//std::strstream strSkillSlot;
		//strSkillSlot << _T("UPDATE ChaInfo SET ChaInfo.ChaSkillSlot=? WHERE (ChaNum=" <<  dwChaNum << ")");
		//strSkillSlot << std::ends;

		TCHAR szTemp[128] = {0};
		_snprintf( szTemp, 128, "UPDATE ChaInfo SET ChaInfo.ChaSkillSlot=? WHERE (ChaNum=%u)", dwChaNum );

		m_pGameDB->WriteImage( szTemp, dwChaNum, (BYTE *) pBuffer, dwSize );

		//strSkillSlot.freeze( false );	// Note : std::strstream�� freeze. �� �ϸ� Leak �߻�.
		pBuffer = NULL;
	}

	// 2003-11-27 : Add
	// Action Quick Slot
	pCharData2->GETACTION_QUICKSLOT(ByteStream);
	ByteStream.GetBuffer(pBuffer, dwSize);	
	if (pBuffer != NULL)
	{
		//std::strstream strActionSlot;
		//strActionSlot << _T("UPDATE ChaInfo SET ChaInfo.ChaActionSlot=? WHERE (ChaNum=" <<  dwChaNum << ")");
		//strActionSlot << std::ends;

		TCHAR szTemp[128] = {0};
		_snprintf( szTemp, 128, "UPDATE ChaInfo SET ChaInfo.ChaActionSlot=? WHERE (ChaNum=%u)", dwChaNum );

		m_pGameDB->WriteImage( szTemp, dwChaNum, (BYTE *) pBuffer, dwSize );

		//strActionSlot.freeze( false );	// Note : std::strstream�� freeze. �� �ϸ� Leak �߻�.
		pBuffer = NULL;
	}

	// Put on item
	pCharData2->GETPUTONITEMS_BYBUF(ByteStream);
	ByteStream.GetBuffer(pBuffer, dwSize);
	if (pBuffer != NULL)
	{
		//std::strstream strPutOnItems;
		//strPutOnItems << _T("UPDATE ChaInfo SET ChaInfo.ChaPutOnItems=? WHERE (ChaNum=" <<  dwChaNum << ")");
		//strPutOnItems << std::ends;

		TCHAR szTemp[128] = {0};
		_snprintf( szTemp, 128, "UPDATE ChaInfo SET ChaInfo.ChaPutOnItems=? WHERE (ChaNum=%u)", dwChaNum );

		m_pGameDB->WriteImage( szTemp, dwChaNum, (BYTE *) pBuffer, dwSize );		

		//strPutOnItems.freeze( false );	// Note : std::strstream�� freeze. �� �ϸ� Leak �߻�.
		pBuffer = NULL;
	}	

	// Inventory
	ByteStream.ClearBuffer();
	pCharData2->GETINVENTORYE_BYBUF(ByteStream);
	ByteStream.GetBuffer(pBuffer, dwSize);	
	if (pBuffer != NULL)
	{
		//std::strstream strChaInven;
		//strChaInven << _T("UPDATE ChaInfo SET ChaInfo.ChaInven=? WHERE (ChaNum=" <<  dwChaNum << ")");
		//strChaInven << std::ends;

		TCHAR szTemp[128] = {0};
		_snprintf( szTemp, 128, "UPDATE ChaInfo SET ChaInfo.ChaInven=? WHERE (ChaNum=%u)", dwChaNum );

		m_pGameDB->WriteImage( szTemp, dwChaNum, (BYTE *) pBuffer, dwSize );		

		//strChaInven.freeze( false );	// Note : std::strstream�� freeze. �� �ϸ� Leak �߻�.
		pBuffer = NULL;
	}	

#if defined(VN_PARAM) //vietnamtest%%%
	// VT ADD Inventory Save
	ByteStream.ClearBuffer();
	pCharData2->GETVTADDINVENTORYE_BYBUF(ByteStream);
	ByteStream.GetBuffer(pBuffer, dwSize);	
	if (pBuffer != NULL)
	{
		//std::strstream strChaInven;
		//strChaInven << _T("UPDATE ChaInfo SET ChaInfo.ChaInven=? WHERE (ChaNum=" <<  dwChaNum << ")");
		//strChaInven << std::ends;

		TCHAR szTemp[128] = {0};
		_snprintf( szTemp, 128, "UPDATE ChaInfo SET ChaInfo.VTAddInven=? WHERE (ChaNum=%u)", dwChaNum );

		m_pGameDB->WriteImage( szTemp, dwChaNum, (BYTE *) pBuffer, dwSize );		

		//strChaInven.freeze( false );	// Note : std::strstream�� freeze. �� �ϸ� Leak �߻�.
		pBuffer = NULL;
	}
#endif

#if defined(TW_PARAM) || defined(_RELEASED)	|| defined(HK_PARAM) 
	// �븸 ��ŷ������ ���ؼ� �α׾ƿ� �ϱ��� ChaLevel�̶� ChaMoney �� DB�� ����

	// ������ GameMoney�� ��ȣȭ �Ѵ�.
	char szTempMoney[CHAR_TEMPMONEY];
	LONGLONG lnTempMoney = pCharData2->m_lnMoney;
	//LONGLONG lnTempValue = (dwChaNum * pCharData2->m_wLevel);

	lnTempMoney <<= 0x04;
	//lnTempMoney += lnTempValue;

	sprintf( szTempMoney, "%I64d", lnTempMoney );
	size_t i;
	for( i = 0; i < strlen(szTempMoney); i++ )
	{
		szTempMoney[i] += 20;
	}

	TCHAR szTemp1[256] = {0};
	_snprintf( szTemp1, 512, "Exec sp_InsertChaLastInfo %d, '%s', %u",
		pCharData2->m_wLevel,szTempMoney,dwChaNum );

	if (m_pGameDB->ExecuteSQL(szTemp1) != DB_OK)
	{
		//		strTemp.freeze( false );	// Note : std::strstream�� freeze. �� �ϸ� Leak �߻�.
		return DB_ERROR;
	}

	lnTempMoney = pCharData2->m_lnStorageMoney;
//	lnTempValue = (dwUserNum);

	lnTempMoney <<= 0x04;
//	lnTempMoney += lnTempValue;	

	sprintf( szTempMoney, "%I64d", lnTempMoney );
	for( i = 0; i < strlen(szTempMoney); i++ )
	{
		szTempMoney[i] += 20;
	}

	TCHAR szTemp2[256] = {0};
	_snprintf( szTemp2, 512, "Exec sp_InsertUserLastInfo %u, '%s'",
		pCharData2->GetUserID(),szTempMoney);

	if (m_pGameDB->ExecuteSQL(szTemp2) != DB_OK)
	{
		return DB_ERROR;
	}
#endif

	// Character Quest information
	pCharData2->GETQUESTPLAY(ByteStream);
	ByteStream.GetBuffer(pBuffer, dwSize);
	if (pBuffer != NULL)
	{
		//std::strstream strQuest;
		//strQuest << "UPDATE ChaInfo SET ChaInfo.ChaQuest=? WHERE (ChaNum=" <<  dwChaNum << ")";
		//strQuest << std::ends;

		TCHAR szTemp[128] = {0};
		_snprintf( szTemp, 128, "UPDATE ChaInfo SET ChaInfo.ChaQuest=? WHERE (ChaNum=%u)", dwChaNum );

		m_pGameDB->WriteImage( szTemp, dwChaNum, (BYTE *) pBuffer, dwSize );

		pBuffer = NULL;
		//strQuest.freeze( false );	// Note : std::strstream�� freeze. �� �ϸ� Leak �߻�.
	}

	pCharData2->GETITEMCOOLTIME_BYBUF(ByteStream);
	ByteStream.GetBuffer(pBuffer, dwSize);
	if (pBuffer != NULL)
	{
		//std::strstream strQuest;
		//strQuest << "UPDATE ChaInfo SET ChaInfo.ChaQuest=? WHERE (ChaNum=" <<  dwChaNum << ")";
		//strQuest << std::ends;

		TCHAR szTemp[128] = {0};
		_snprintf( szTemp, 128, "UPDATE ChaInfo SET ChaInfo.ChaCoolTime=? WHERE (ChaNum=%u)", dwChaNum );

		m_pGameDB->WriteImage( szTemp, dwChaNum, (BYTE *) pBuffer, dwSize );

		pBuffer = NULL;
		//strQuest.freeze( false );	// Note : std::strstream�� freeze. �� �ϸ� Leak �߻�.
	}	

    // ����� �κ��丮 ����
	if ( pCharData2->m_bServerStorage )
	{
		pCharData2->GETSTORAGE_BYBUF(ByteStream);
		ByteStream.GetBuffer(pBuffer, dwSize);		
		if (pBuffer != NULL)
		{
			WriteUserInven(CCfg::GetInstance()->GetServerGroup(), 
							pCharData2->m_lnStorageMoney, 
							pCharData2->GetUserID() , 
							(BYTE *) pBuffer, 
							dwSize);
		}
	}

	return DB_OK;
}

//! ĳ������ ��ġ������ �����Ѵ�.
//! \param dwChaNum ĳ���� ��ȣ
//! \param dwStartmapID �����ϴ� �� ��ȣ
//! \param dwStartGate �����ϴ� ����Ʈ ��ȣ
//! \param fStartPosX �����ϴ� X ��ǥ
//! \param fStartPosY �����ϴ� Y ��ǥ
//! \param fStartPosZ �����ϴ� Z ��ǥ
//! \param dwSaveMapID ���������� �÷����� �� ��ȣ
//! \param fSavePosX ���������� �÷����� X ��ǥ
//! \param fSavePosY ���������� �÷����� Y ��ǥ
//! \param fSavePosZ ���������� �÷����� Z ��ǥ
//! \param dwLastCallMapID ���� ��ȯī�忡 ����� �� ��ȣ
//! \param fLastCallPosX ���� ��ȯī�忡 ����� X ��ǥ
//! \param fLastCallPosY ���� ��ȯī�忡 ����� Y ��ǥ
//! \param fLastCallPosZ ���� ��ȯī�忡 ����� Z ��ǥ
//! \return DB_OK, DB_ERROR
int	COdbcManager::SaveChaPos(
	DWORD dwChaNum, 
	DWORD dwStartMapID,
	DWORD dwStartGate,
	FLOAT fStartPosX,
	FLOAT fStartPosY,
	FLOAT fStartPosZ,
	DWORD dwSaveMapID,
	FLOAT fSavePosX,
	FLOAT fSavePosY,
	FLOAT fSavePosZ,
	DWORD dwLastCallMapID,
	FLOAT fLastCallPosX,
	FLOAT fLastCallPosY,
	FLOAT fLastCallPosZ )
{
	//std::strstream strTemp;
	//strTemp << "UPDATE ChaInfo SET ";
	//strTemp << "  ChaStartMap="   << (int) dwStartMapID;
	//strTemp << ", ChaStartGate="  << (int) dwStartGate;
	//strTemp << ", ChaPosX="       << fStartPosX;
	//strTemp << ", ChaPosY="       << fStartPosY;
	//strTemp << ", ChaPosZ="       << fStartPosZ;
 //   strTemp << ", ChaSaveMap="    << (int) dwSaveMapID;
 //   strTemp << ", ChaSavePosX="   << fSavePosX;
 //   strTemp << ", ChaSavePosY="   << fSavePosY;
 //   strTemp << ", ChaSavePosZ="   << fSavePosZ;
 //   strTemp << ", ChaReturnMap="  << (int) dwLastCallMapID;
 //   strTemp << ", ChaReturnPosX=" << fLastCallPosX;
 //   strTemp << ", ChaReturnPosY=" << fLastCallPosY;
 //   strTemp << ", ChaReturnPosZ=" << fLastCallPosZ;
	//strTemp << " WHERE ChaNum=" << dwChaNum;
	//strTemp << std::ends;

	TCHAR szTemp[512] = {0};
	_snprintf( szTemp, 512, "UPDATE ChaInfo SET "
							 "ChaStartMap=%d"
							 ", ChaStartGate=%d"
							 ", ChaPosX=%f"
							 ", ChaPosY=%f"
							 ", ChaPosZ=%f"
							 ", ChaSaveMap=%d"
							 ", ChaSavePosX=%f"
							 ", ChaSavePosY=%f"
							 ", ChaSavePosZ=%f"
							 ", ChaReturnMap=%d"
							 ", ChaReturnPosX=%f"
							 ", ChaReturnPosY=%f"
							 ", ChaReturnPosZ=%f"
							 " WHERE ChaNum=%u",
							(int) dwStartMapID,
							(int) dwStartGate,
							fStartPosX,
							fStartPosY,
							fStartPosZ,
							(int) dwSaveMapID,
							fSavePosX,
							fSavePosY,
							fSavePosZ,
							(int) dwLastCallMapID,
							fLastCallPosX,
							fLastCallPosY,
							fLastCallPosZ,
							dwChaNum );

	if (m_pGameDB->ExecuteSQL(szTemp) != DB_OK)
	{
		//strTemp.freeze( false );	// Note : std::strstream�� freeze. �� �ϸ� Leak �߻�.
		return DB_ERROR;
	}
	else
	{
		//strTemp.freeze( false );	// Note : std::strstream�� freeze. �� �ϸ� Leak �߻�.
		return DB_OK;
	}
}