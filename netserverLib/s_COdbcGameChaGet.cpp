#include "stdafx.h"
#include "s_COdbcManager.h"
#include "s_CDbAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/**	
* ĳ���� ������ �����´�. ĳ������ ������� (���̳ʸ� ����)
* \param nUserNum ����ڹ�ȣ
* \param nChaNum ĳ���͹�ȣ
* \param pChaData2 ĳ���� �����͸� ���� ����Ʈ
* \return DB_OK, DB_ERROR
*/
int COdbcManager::GetCharacterInfo(int nUserNumber,
								   int nChaNum, 
								   SCHARDATA2* pChaData2)
{
	if (nUserNumber <= 0 || nChaNum <= 0)
	{
		return DB_ERROR;
	}

	int nRowCount = 0;
//-------------------------------------------------------
	SQLRETURN sReturn	= 0;
	ODBC_STMT* pConn	= m_pGameDB->GetConnection();
	if (!pConn) return DB_ERROR;

	SQLRETURN sReturn2	= 0;
	ODBC_STMT* pConn2	= m_pUserDB->GetConnection();
	if (!pConn2) return DB_ERROR;

	/*SQLRETURN sReturn3	= 0;
	ODBC_STMT* pConn3	= m_pUserDB->GetConnection();
	if (!pConn3) return DB_ERROR;*/
//-------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------
	TCHAR szTemp[1024] = {0};
	_snprintf( szTemp, 1024, "SELECT UserNum,SGNum,ChaName,ChaTribe,ChaClass,"
							"ChaBright,ChaLevel,ChaDex,ChaIntel,ChaPower,"
							"ChaStrong,ChaSpirit,ChaStrength,ChaStRemain,ChaAttackP,"							
							"ChaDefenseP,ChaFightA,ChaShootA,ChaSkillPoint,ChaHP,"
							"ChaMP,ChaSP,ChaPK,ChaStartMap,ChaStartGate,"
							"ChaPosX,ChaPosY,ChaPosZ,ChaMoney,ChaExp,"
							"ChaSaveMap,ChaSavePosX,ChaSavePosY,ChaSavePosZ,ChaSchool,"
							"ChaHair,ChaFace,ChaLiving,ChaInvenLine,"
							"ChaReturnMap,ChaReturnPosX,ChaReturnPosY,ChaReturnPosZ,GuNum,"
							"ChaGuName,ChaSex,ChaHairColor, ChaReExp, ChaSpMID, ChaSpSID,"
							"ChaReborn, ChaPkWin, ChaPkLoss, ChaVotePoint, ChaPremiumPoint,ChaContributionPoint, ChaCP "
							"FROM ChaInfo WHERE ChaNum=%d AND UserNum=%d", nChaNum, nUserNumber );
	TCHAR szTemp2[128]	= { 0 };
	_snprintf(
		szTemp2,
		128,
		"SELECT ChaSaldo "
		"FROM UserInfo WHERE UserNum=%d",
		nUserNumber
	);

//----------------------------------------------------
	sReturn = ::SQLExecDirect(
		pConn->hStmt,
		(SQLCHAR*) szTemp,
		SQL_NTS
	);

	sReturn2 = ::SQLExecDirect(
		pConn2->hStmt,
		(SQLCHAR*)szTemp2,
		SQL_NTS
	);

	/*sReturn3 = ::SQLExecDirect(
		pConn3->hStmt,
		(SQLCHAR*)szTemp3,
		SQL_NTS
	);*/
//----------------------------------------------------
//---------------------------------------------------------------------------------
	if ((sReturn != SQL_SUCCESS) && (sReturn != SQL_SUCCESS_WITH_INFO)) 
	{
        Print(szTemp);		
		Print(GetErrorString(pConn->hStmt));
		m_pGameDB->FreeConnection(pConn);

//		strTemp.freeze( false );	// Note : std::strstream�� freeze. �� �ϸ� Leak �߻�.
        return DB_ERROR;
	}

	if ((sReturn2 != SQL_SUCCESS) && (sReturn2 != SQL_SUCCESS_WITH_INFO))
	{
		Print(szTemp2);
		Print(GetErrorString(pConn2->hStmt));
		m_pUserDB->FreeConnection(pConn2);

		return DB_ERROR;
	}

	/*if ((sReturn3 != SQL_SUCCESS) && (sReturn3 != SQL_SUCCESS_WITH_INFO))
	{
		Print(szTemp3);
		Print(GetErrorString(pConn3->hStmt));
		m_pUserDB->FreeConnection(pConn3);

		return DB_ERROR;
	}*/
//---------------------------------------------------------------------------------

	SQLCHAR    szChaName[CHR_ID_LENGTH+1] = {0}; SQLINTEGER cbChaName = SQL_NTS; // ĳ���͸�
	SQLCHAR    szChaGuName[CHAR_SZNAME+1] = {0}; SQLINTEGER cbChaGuName = SQL_NTS;// Ŭ�������� �г���
	
    SQLINTEGER nUserNum = 0, cbUserNum=SQL_NTS;
	SQLINTEGER nChaTribe = 0, cbChaTribe=SQL_NTS;
	SQLINTEGER nChaClass = 0, cbChaClass=SQL_NTS;
	SQLINTEGER nSGNum = 0, cbSGNum=SQL_NTS;

    SQLINTEGER nChaLevel = 0, cbChaLevel=SQL_NTS;
	SQLINTEGER nChaPower = 0, cbChaPower=SQL_NTS; 
	SQLINTEGER nChaStrong = 0, cbChaStrong=SQL_NTS;   
	SQLINTEGER nChaStrength = 0, cbChaStrength=SQL_NTS; 
	SQLINTEGER nChaSpirit = 0, cbChaSpirit=SQL_NTS;

	SQLINTEGER nChaDex = 0, cbChaDex=SQL_NTS;
	SQLINTEGER nChaIntel = 0, cbChaIntel=SQL_NTS;
	SQLINTEGER nChaStRemain = 0, cbChaStRemain=SQL_NTS; 
	SQLINTEGER nChaHP = 0, cbChaHP=SQL_NTS;
	SQLINTEGER nChaMP = 0, cbChaMP=SQL_NTS;
	
	SQLINTEGER nChaStartMap = 0, cbChaStartMap=SQL_NTS;
	SQLINTEGER nChaStartGate = 0, cbChaStartGate=SQL_NTS;
	SQLFLOAT fChaPosX = 0; SQLINTEGER cbChaPosX=SQL_NTS;
	SQLFLOAT fChaPosY = 0; SQLINTEGER cbChaPosY=SQL_NTS;
	SQLFLOAT fChaPosZ = 0; SQLINTEGER cbChaPosZ=SQL_NTS;

	// SQLINTEGER nGuNum, cbGuNum=SQL_NTS;
	SQLINTEGER nChaBright = 0, cbChaBright=SQL_NTS;
	SQLINTEGER nChaAttackP = 0, cbChaAttackP=SQL_NTS;
	SQLINTEGER nChaDefenseP = 0, cbChaDefenseP=SQL_NTS;
	SQLINTEGER nChaFightA = 0, cbChaFightA=SQL_NTS;

	SQLINTEGER nChaShootA = 0, cbChaShootA=SQL_NTS;
	SQLINTEGER nChaSP = 0, cbChaSP=SQL_NTS;
	SQLINTEGER nChaPK = 0, cbChaPK=SQL_NTS;
	SQLINTEGER nChaSkillPoint = 0, cbChaSkillPoint=SQL_NTS;
	SQLINTEGER nChaSaveMap = 0, cbChaSaveMap=SQL_NTS;

	SQLFLOAT fChaSavePosX = 0; SQLINTEGER cbChaSavePosX=SQL_NTS;
	SQLFLOAT fChaSavePosY = 0; SQLINTEGER cbChaSavePosY=SQL_NTS;
	SQLFLOAT fChaSavePosZ = 0; SQLINTEGER cbChaSavePosZ=SQL_NTS;

	SQLINTEGER nChaSchool = 0, cbChaSchool = SQL_NTS;
	SQLINTEGER nChaHair = 0, cbChaHair = SQL_NTS;
	SQLINTEGER nChaFace = 0, cbChaFace = SQL_NTS;
	SQLINTEGER nChaLiving = 0, cbChaLiving = SQL_NTS;
	
	// 2005-12-29 Jgkim:Add ChaSex, ChaHairColor 
	SQLSMALLINT nChaSex = 0; SQLINTEGER cbChaSex = SQL_NTS;
	SQLINTEGER nChaHairColor =0, cbChaHairColor = SQL_NTS;
    
	LONGLONG   llChaExp = 0, llChaMoney = 0;
	SQLINTEGER cbChaExp=SQL_NTS, cbChaMoney=SQL_NTS; 
    
    SQLINTEGER nChaInvenLine = 0; SQLINTEGER cbChaInvenLine = SQL_NTS;
    SQLINTEGER nGuNum = 0; SQLINTEGER cbGuNum = SQL_NTS;

    SQLINTEGER nChaReturnMap  = 0; SQLINTEGER cbChaReturnMap  = SQL_NTS;
	SQLFLOAT   fChaReturnPosX = 0; SQLINTEGER cbChaReturnPosX = SQL_NTS;
	SQLFLOAT   fChaReturnPosY = 0; SQLINTEGER cbChaReturnPosY = SQL_NTS;
	SQLFLOAT   fChaReturnPosZ = 0; SQLINTEGER cbChaReturnPosZ = SQL_NTS;

	// Add to ChaReExp
	LONGLONG   llChaReExp = 0;
	SQLINTEGER cbChaReExp=SQL_NTS;

	// Add to ChaSpMID, ChaSpSID
	SQLINTEGER nChaSpMID = 0, cbChaSpMID=SQL_NTS;
	SQLINTEGER nChaSpSID = 0, cbChaSpSID=SQL_NTS;

	//add reborn
	SQLINTEGER nChaReborn = 0, cbChaReborn=SQL_NTS;
	//add pk
	SQLINTEGER nChaPkWin = 0, cbChaPkWin=SQL_NTS;
	SQLINTEGER nChaPkLoss = 0, cbChaPkLoss=SQL_NTS;

	//add money 2
	LONGLONG   llChaMoney2 = 0;
	SQLINTEGER cbChaMoney2=SQL_NTS; 

	//add money 3
	LONGLONG   llChaMoney3 = 0;
	SQLINTEGER cbChaMoney3=SQL_NTS; 

	//add contributionpoint by CNDev
	LONGLONG   llChaContributionPoint = 0;
	SQLINTEGER cbChaContributionPoint=SQL_NTS;

	SQLINTEGER nChaCP = 0, cbChaCP=SQL_NTS; //add cp
	LONGLONG			llChaSaldo							= 0;		SQLINTEGER cbChaSaldo		= SQL_NTS;

	while (true)
	{
//----------------------------------------------------------------------------
		sReturn = ::SQLFetch(pConn->hStmt);
		if (sReturn == SQL_ERROR || sReturn == SQL_SUCCESS_WITH_INFO)
        {	
            Print(szTemp);		
			Print(GetErrorString(pConn->hStmt));
            m_pGameDB->FreeConnection(pConn);

//			strTemp.freeze( false );	// Note : std::strstream�� freeze. �� �ϸ� Leak �߻�.
            return DB_ERROR;
		}

		sReturn2 = ::SQLFetch(pConn2->hStmt);
		if (sReturn2 == SQL_ERROR || sReturn2 == SQL_SUCCESS_WITH_INFO)
		{
			Print(szTemp2);
			Print(GetErrorString(pConn2->hStmt));
			m_pUserDB->FreeConnection(pConn2);

			return DB_ERROR;
		}

		/*sReturn3 = ::SQLFetch(pConn3->hStmt);
		if (sReturn3 == SQL_ERROR || sReturn3 == SQL_SUCCESS_WITH_INFO)
		{
			Print(szTemp3);
			Print(GetErrorString(pConn3->hStmt));
			m_pUserDB->FreeConnection(pConn3);

			return DB_ERROR;
		}*/
//----------------------------------------------------------------------------

		if (sReturn == SQL_SUCCESS || sReturn == SQL_SUCCESS_WITH_INFO)
		{		
			::SQLGetData(pConn->hStmt, 1, SQL_C_LONG, &nUserNum,       0, &cbUserNum);
			::SQLGetData(pConn->hStmt, 2, SQL_C_LONG, &nSGNum,         0, &cbSGNum);
            ::SQLGetData(pConn->hStmt, 3, SQL_C_CHAR, szChaName   ,  CHR_ID_LENGTH+1, &cbChaName); 
            ::SQLGetData(pConn->hStmt, 4, SQL_C_LONG, &nChaTribe,      0, &cbChaTribe);
            ::SQLGetData(pConn->hStmt, 5, SQL_C_LONG, &nChaClass,      0, &cbChaClass);

			::SQLGetData(pConn->hStmt, 6, SQL_C_LONG, &nChaBright,     0, &cbChaBright);
            ::SQLGetData(pConn->hStmt, 7, SQL_C_LONG, &nChaLevel,      0, &cbChaLevel);
			::SQLGetData(pConn->hStmt, 8, SQL_C_LONG, &nChaDex,        0, &cbChaDex);
			::SQLGetData(pConn->hStmt, 9, SQL_C_LONG, &nChaIntel,      0, &cbChaIntel);
            ::SQLGetData(pConn->hStmt,10, SQL_C_LONG, &nChaPower,      0, &cbChaPower);

            ::SQLGetData(pConn->hStmt,11, SQL_C_LONG, &nChaStrong,     0, &cbChaStrong);
			::SQLGetData(pConn->hStmt,12, SQL_C_LONG, &nChaSpirit,     0, &cbChaSpirit);
            ::SQLGetData(pConn->hStmt,13, SQL_C_LONG, &nChaStrength,   0, &cbChaStrength);
			::SQLGetData(pConn->hStmt,14, SQL_C_LONG, &nChaStRemain,   0, &cbChaStRemain);
			::SQLGetData(pConn->hStmt,15, SQL_C_LONG, &nChaAttackP,    0, &cbChaAttackP);		
                        
            ::SQLGetData(pConn->hStmt,16, SQL_C_LONG, &nChaDefenseP,   0, &cbChaDefenseP);
			::SQLGetData(pConn->hStmt,17, SQL_C_LONG, &nChaFightA,     0, &cbChaFightA);
			::SQLGetData(pConn->hStmt,18, SQL_C_LONG, &nChaShootA,     0, &cbChaShootA);
			::SQLGetData(pConn->hStmt,19, SQL_C_LONG, &nChaSkillPoint, 0, &cbChaSkillPoint);
			::SQLGetData(pConn->hStmt,20, SQL_C_LONG, &nChaHP,			0, &cbChaHP);

			::SQLGetData(pConn->hStmt,21, SQL_C_LONG, &nChaMP,			0, &cbChaMP);
			::SQLGetData(pConn->hStmt,22, SQL_C_LONG, &nChaSP,			0, &cbChaSP);
			::SQLGetData(pConn->hStmt,23, SQL_C_LONG, &nChaPK,			0, &cbChaPK);
			::SQLGetData(pConn->hStmt,24, SQL_C_LONG, &nChaStartMap,	0, &cbChaStartMap);
			::SQLGetData(pConn->hStmt,25, SQL_C_LONG, &nChaStartGate,  0, &cbChaStartGate);
			
			::SQLGetData(pConn->hStmt,26, SQL_C_DOUBLE, &fChaPosX,		0, &cbChaPosX);
			::SQLGetData(pConn->hStmt,27, SQL_C_DOUBLE, &fChaPosY,		0, &cbChaPosY);
			::SQLGetData(pConn->hStmt,28, SQL_C_DOUBLE, &fChaPosZ,		0, &cbChaPosZ);			
            ::SQLGetData(pConn->hStmt,29, SQL_C_SBIGINT, &llChaMoney,  0, &cbChaMoney);
            ::SQLGetData(pConn->hStmt,30, SQL_C_SBIGINT, &llChaExp,    0, &cbChaExp);
			
			::SQLGetData(pConn->hStmt,31, SQL_C_LONG, &nChaSaveMap,    0, &cbChaSaveMap);
			::SQLGetData(pConn->hStmt,32, SQL_C_DOUBLE, &fChaSavePosX,   0, &cbChaSavePosX);
			::SQLGetData(pConn->hStmt,33, SQL_C_DOUBLE, &fChaSavePosY,   0, &cbChaSavePosY);
			::SQLGetData(pConn->hStmt,34, SQL_C_DOUBLE, &fChaSavePosZ,   0, &cbChaSavePosZ);

			::SQLGetData(pConn->hStmt,35, SQL_C_LONG, &nChaSchool,		0, &cbChaSchool);
			::SQLGetData(pConn->hStmt,36, SQL_C_LONG, &nChaHair,		0, &cbChaHair);
			::SQLGetData(pConn->hStmt,37, SQL_C_LONG, &nChaFace,		0, &cbChaFace);
			::SQLGetData(pConn->hStmt,38, SQL_C_LONG, &nChaLiving,		0, &cbChaLiving);

            ::SQLGetData(pConn->hStmt,39, SQL_C_LONG, &nChaInvenLine, 0, &cbChaInvenLine);
            
            ::SQLGetData(pConn->hStmt,40, SQL_C_LONG,   &nChaReturnMap,  0, &cbChaReturnMap);
			::SQLGetData(pConn->hStmt,41, SQL_C_DOUBLE, &fChaReturnPosX, 0, &cbChaReturnPosX);
			::SQLGetData(pConn->hStmt,42, SQL_C_DOUBLE, &fChaReturnPosY, 0, &cbChaReturnPosY);
			::SQLGetData(pConn->hStmt,43, SQL_C_DOUBLE, &fChaReturnPosZ, 0, &cbChaReturnPosZ);

            ::SQLGetData(pConn->hStmt,44, SQL_C_LONG, &nGuNum, 0, &cbGuNum);
            ::SQLGetData(pConn->hStmt,45, SQL_C_CHAR, szChaGuName, CHAR_SZNAME, &cbChaGuName); 

			// 2005-12-29 Jgkim:Add nChaSex, nChaHairColor
			::SQLGetData(pConn->hStmt,46, SQL_SMALLINT, &nChaSex,       0, &cbChaSex);
			::SQLGetData(pConn->hStmt,47, SQL_C_LONG,   &nChaHairColor, 0, &cbChaHairColor);

			// Add to ChaReExp
			::SQLGetData(pConn->hStmt,48, SQL_C_SBIGINT, &llChaReExp, 0, &cbChaReExp);

			// Add to SpMID, SpSID
			::SQLGetData(pConn->hStmt,49, SQL_C_LONG, &nChaSpMID, 0, &cbChaSpMID);
			::SQLGetData(pConn->hStmt,50, SQL_C_LONG, &nChaSpSID, 0, &cbChaSpSID);	

			//add reborn
			::SQLGetData(pConn->hStmt,51, SQL_C_LONG, &nChaReborn, 0, &cbChaReborn);	
			//add pk
			::SQLGetData(pConn->hStmt,52, SQL_C_LONG, &nChaPkWin, 0, &cbChaPkWin);	
			::SQLGetData(pConn->hStmt,53, SQL_C_LONG, &nChaPkLoss, 0, &cbChaPkLoss);

			//add money 2
			::SQLGetData(pConn->hStmt,54, SQL_C_SBIGINT, &llChaMoney2,  0, &cbChaMoney2);
			//add money 3
			::SQLGetData(pConn->hStmt,55, SQL_C_SBIGINT, &llChaMoney3,  0, &cbChaMoney3);
			//add contributionpoint by CNDev
			::SQLGetData(pConn->hStmt,56, SQL_C_SBIGINT, &llChaContributionPoint, 0, &cbChaContributionPoint);

			::SQLGetData(pConn->hStmt,57, SQL_C_LONG, &nChaCP,			0, &cbChaCP);//add cp


            pChaData2->m_dwCharID			= (DWORD) nChaNum; // ĳ���͹�ȣ
			pChaData2->SetUserID( (DWORD) nUserNum ); // ����ڹ�ȣ
            pChaData2->m_dwGuild            = (DWORD) nGuNum; // Ŭ����ȣ

            if (cbChaName != 0 && cbChaName != -1) // ĳ���͸�
				::StringCchCopy(pChaData2->m_szName, CHR_ID_LENGTH, (const char*) szChaName);

            if (cbChaGuName != 0 && cbChaGuName != -1) // Ŭ������ �г���
                ::StringCchCopy(pChaData2->m_szNick, CHAR_SZNAME, (const char*) szChaGuName);

            pChaData2->m_emTribe			= EMTRIBE(nChaTribe);
            pChaData2->m_emClass			= EMCHARCLASS(nChaClass);
            
            pChaData2->m_wLevel				= (WORD) nChaLevel;           
            pChaData2->m_sStats.dwPow		= (DWORD) nChaPower;
            pChaData2->m_sStats.dwStr		= (DWORD) nChaStrong;
            pChaData2->m_sStats.dwSta		= (DWORD) nChaStrength;
            pChaData2->m_sStats.dwSpi		= (DWORD) nChaSpirit;

            pChaData2->m_sStats.dwDex		= (DWORD) nChaDex;
            pChaData2->m_sStats.dwInt		= (DWORD) nChaIntel;
            pChaData2->m_wStatsPoint		= (DWORD) nChaStRemain;
            pChaData2->m_lnMoney            = llChaMoney;
            pChaData2->m_sExperience.lnNow  = llChaExp;

			pChaData2->m_sHP.dwNow			= (DWORD) nChaHP;
			pChaData2->m_sMP.dwNow			= (DWORD) nChaMP;
			pChaData2->m_sStartMapID.dwID	= (DWORD) nChaStartMap;
			pChaData2->m_dwStartGate		= (DWORD) nChaStartGate;
			pChaData2->m_vStartPos.x		= (float) fChaPosX;

			pChaData2->m_vStartPos.y		= (float) fChaPosY;
			pChaData2->m_vStartPos.z		= (float) fChaPosZ,	
			pChaData2->m_nBright			= nChaBright;
			pChaData2->m_wAP				= (DWORD) nChaAttackP;
			pChaData2->m_wDP				= (DWORD) nChaDefenseP;

			pChaData2->m_wPA				= (DWORD) nChaFightA;
			pChaData2->m_wSA				= (DWORD) nChaShootA;
			pChaData2->m_dwSkillPoint		= (DWORD) nChaSkillPoint;
			pChaData2->m_sSP.dwData 		= (DWORD) nChaSP;
			pChaData2->m_wPK				= (WORD) nChaPK;
			
			pChaData2->m_sSaveMapID.dwID	= (DWORD) nChaSaveMap;
			pChaData2->m_vSavePos.x			= (float) fChaSavePosX; 
			pChaData2->m_vSavePos.y			= (float) fChaSavePosY; 
			pChaData2->m_vSavePos.z			= (float) fChaSavePosZ;

            pChaData2->m_sLastCallMapID.dwID= (DWORD) nChaReturnMap;
			pChaData2->m_vLastCallPos.x		= (float) fChaReturnPosX; 
			pChaData2->m_vLastCallPos.y		= (float) fChaReturnPosY; 
			pChaData2->m_vLastCallPos.z		= (float) fChaReturnPosZ;

			pChaData2->m_wSchool			= (WORD) nChaSchool;

			pChaData2->m_wFace				= (WORD) nChaFace;
			pChaData2->m_wHair				= (WORD) nChaHair;
			pChaData2->m_nLiving			= (int) nChaLiving;
			pChaData2->m_wHairColor         = (WORD) nChaHairColor;
			pChaData2->m_wSex               = (WORD) nChaSex;

			
			if ( pChaData2->m_emClass == GLCC_FIGHTER_M ||
				pChaData2->m_emClass == GLCC_ARMS_M ||
				pChaData2->m_emClass == GLCC_EXTREME_M ||
				pChaData2->m_emClass == GLCC_ARCHER_M ||
				pChaData2->m_emClass == GLCC_SPIRIT_M ||
				pChaData2->m_emClass == GLCC_SCIENCE_M ||
				pChaData2->m_emClass == GLCC_ASSASIN_M ||
				pChaData2->m_emClass == GLCC_TESTING_M  ||
				pChaData2->m_emClass == GLCC_SHAPER_M
				)
			{
				pChaData2->m_wSex               = 1;
			}
			else if ( pChaData2->m_emClass == GLCC_FIGHTER_W ||
				pChaData2->m_emClass == GLCC_ARMS_W ||
				pChaData2->m_emClass == GLCC_EXTREME_W ||
				pChaData2->m_emClass == GLCC_ARCHER_W ||
				pChaData2->m_emClass == GLCC_SPIRIT_W ||
				pChaData2->m_emClass == GLCC_SCIENCE_W ||
				pChaData2->m_emClass == GLCC_ASSASIN_W ||
				pChaData2->m_emClass == GLCC_TESTING_W ||
				pChaData2->m_emClass == GLCC_SHAPER_W 
				)
			{
				pChaData2->m_wSex               = 0;
			}


			if( nChaSpMID != 0 && nChaSpSID != 0 )
			{
				// Add to SpMID, SpSID
				pChaData2->m_sSummonPosionID.wMainID = (WORD) nChaSpMID;
				pChaData2->m_sSummonPosionID.wSubID = (WORD) nChaSpSID;
			}

			// add reborn
			pChaData2->m_dwReborn = (DWORD) nChaReborn;
			//add pk
			pChaData2->m_dwPkWin = (DWORD) nChaPkWin;
			pChaData2->m_dwPkLoss = (DWORD) nChaPkLoss;

			//add money 2
			pChaData2->m_lnVoteP  = llChaMoney2;
			//add money 3
			pChaData2->m_lnPremP  = llChaMoney3;
			//add contributionpoint by CNDev
			pChaData2->m_lnContribP = llChaContributionPoint;

            // �κ��丮 ����
            pChaData2->m_wINVENLINE = (WORD) nChaInvenLine; 
			pChaData2->m_lnReExp = llChaReExp;
			pChaData2->m_sCP.dwNow	= (DWORD) nChaCP; //add cp

			nRowCount++;
		}
		else
		{
			break;
		}

		if (sReturn2 == SQL_SUCCESS || sReturn2 == SQL_SUCCESS_WITH_INFO)
		{
		::SQLGetData(pConn2->hStmt, 1, SQL_C_SBIGINT, &llChaSaldo, 0, &cbChaSaldo);

		pChaData2->m_lnSaldo = llChaSaldo;
		}
		else
		{
			break;
		}
		Sleep( 0 );
	}
	m_pGameDB->FreeConnection(pConn);
	m_pUserDB->FreeConnection(pConn2);
	//m_pUserDB->FreeConnection(pConn3);

	if (nRowCount != 1)
	{
		return DB_ERROR;
	}
#if defined(KRT_PARAM)
	// ĳ������ ��ȭ��ȣ�� �����´�.
	GetChaPhoneNumber( nChaNum, pChaData2->m_szPhoneNumber );
#endif

#if defined(VN_PARAM) //vietnamtest%%%
	// ��Ʈ���� �ʿ��� �߰� ������ �����´�.
	GetVTCharInfo( nChaNum,	pChaData2 );
#endif

#if defined(TW_PARAM) || defined(_RELEASED) || defined(HK_PARAM) 
	GetLastCharInfo( nUserNumber, nChaNum, pChaData2 );
#endif


	CByteStream ByteStream;
	
	// Skill	
	sReturn = m_pGameDB->ReadImage("ChaInfo.ChaSkills", nChaNum, ByteStream);
	if (sReturn == DB_ERROR) 
		return DB_ERROR;
	else 
		pChaData2->SETEXPSKILLS_BYBUF(ByteStream);

	// Skill Quick Slot		
	sReturn = m_pGameDB->ReadImage("ChaInfo.ChaSkillSlot", nChaNum, ByteStream);
	if (sReturn == DB_ERROR)
		return DB_ERROR;
	else
        pChaData2->SETSKILL_QUICKSLOT(ByteStream);
	
	// 2003-11-27 : Add
	// Action Quick Slot	
	sReturn = m_pGameDB->ReadImage("ChaInfo.ChaActionSlot", nChaNum, ByteStream);
	if (sReturn == DB_ERROR)
		return DB_ERROR;
	else
		pChaData2->SETACTION_QUICKSLOT(ByteStream);

	// Quest
	sReturn = m_pGameDB->ReadImage("ChaInfo.ChaQuest", nChaNum, ByteStream);
	if (sReturn == DB_ERROR)
		return DB_ERROR;
	else
		pChaData2->SETQUESTPLAY(ByteStream);


	// ���������
	//std::strstream strPutOnItems;
	//strPutOnItems << "SELECT ChaInfo.ChaPutOnItems FROM ChaInfo where (ChaNum=" <<  nChaNum << ")";
	//strPutOnItems << std::ends;
	sReturn = m_pGameDB->ReadImage("ChaInfo.ChaPutOnItems", nChaNum, ByteStream);
	//strPutOnItems.freeze( false );	// Note : std::strstream�� freeze. �� �ϸ� Leak �߻�.

	if (sReturn == DB_ERROR)
		return DB_ERROR;
	else
		SETPUTONITEMS_BYBUF(pChaData2->m_PutOnItems,ByteStream);

	// Character Quest information

	// ĳ���� �κ��丮
	//std::strstream strChaInven;
	//strChaInven << "SELECT ChaInfo.ChaInven FROM ChaInfo where (ChaNum=" <<  nChaNum << ")";
	//strChaInven << std::ends;
	sReturn = m_pGameDB->ReadImage("ChaInfo.ChaInven", nChaNum, ByteStream);
	//strChaInven.freeze( false );	// Note : std::strstream�� freeze. �� �ϸ� Leak �߻�.
	if (sReturn == DB_ERROR)
		return DB_ERROR;
	else
		pChaData2->SETINVENTORY_BYBUF(ByteStream);

#if defined(VN_PARAM) //vietnamtest%%%
	// ��Ʈ���� �ʿ��� �߰� ������ �����´�.
	sReturn = m_pGameDB->ReadImage("ChaInfo.VTAddInven", nChaNum, ByteStream);

	if (sReturn == DB_ERROR)
		return DB_ERROR;
	else
		pChaData2->SETVTADDINVENTORY_BYBUF(ByteStream);
#endif

	
	sReturn = m_pGameDB->ReadImage("ChaInfo.ChaCoolTime", nChaNum, ByteStream);
	//strChaInven.freeze( false );	// Note : std::strstream�� freeze. �� �ϸ� Leak �߻�.
	if (sReturn == DB_ERROR)
		return DB_ERROR;
	else
		pChaData2->SETITEMCOOLTIME_BYBUF(ByteStream);

	return DB_OK;
}

int	COdbcManager::GetChaBInfo(int nUserNum,
							  int nChaNum, 
							  SCHARINFO_LOBBY* sci)
{
	if (nUserNum <= 0 || nChaNum <= 0)
	{
		return DB_ERROR;
	}
    
	SQLRETURN sReturn = 0;
	int nRowCount = 0;
	ODBC_STMT* pConn = m_pGameDB->GetConnection();
	if (!pConn) return DB_ERROR;
	
	// 2003-11-27 Jgkim:Add
	// 2005-12-29 Jgkim:Add ChaSex, ChaHairColor 
	//std::strstream strTemp;
	//strTemp << "SELECT ChaName, ChaClass, ChaSchool, ChaDex, ChaIntel, ChaPower,";
	//strTemp << "ChaStrong, ChaSpirit, ChaStrength, ChaLevel, ChaHair,";	
	//strTemp << "ChaFace, ChaBright, ChaSex, ChaHairColor, ChaExp,";
	//strTemp << "ChaSaveMap, ChaHP FROM ChaInfo ";
	//strTemp << "WHERE ChaNum=" << nChaNum;
	//strTemp << " AND UserNum=" << nUserNum;
	//strTemp << std::ends;

	TCHAR szTemp[512] = {0};
	_snprintf( szTemp, 512, "SELECT ChaName, ChaClass, ChaSchool, ChaDex, ChaIntel, ChaPower,"
							"ChaStrong, ChaSpirit, ChaStrength, ChaLevel, ChaHair,"
							"ChaFace, ChaBright, ChaSex, ChaHairColor, ChaExp,"
							"ChaSaveMap, ChaHP, ChaReborn FROM ChaInfo "
							"WHERE ChaNum=%d AND UserNum=%d",
							nChaNum, nUserNum );
	
	sReturn = ::SQLExecDirect(pConn->hStmt,
							(SQLCHAR*)szTemp, 
							SQL_NTS);

	if ((sReturn != SQL_SUCCESS) && (sReturn != SQL_SUCCESS_WITH_INFO)) 
	{
        Print(szTemp);		
		Print(GetErrorString(pConn->hStmt));
		m_pGameDB->FreeConnection(pConn);

		//strTemp.freeze( false );	// Note : std::strstream�� freeze. �� �ϸ� Leak �߻�.
		return DB_ERROR;
	}

	SQLCHAR    szChaName[CHR_ID_LENGTH+1] = {0}; SQLINTEGER cbChaName = SQL_NTS;
	SQLINTEGER nChaClass = 0, cbChaClass=SQL_NTS;
	SQLINTEGER nChaSchool = 0, cbChaSchool=SQL_NTS;
	SQLINTEGER nChaDex = 0, cbChaDex=SQL_NTS;
	SQLINTEGER nChaIntel = 0, cbChaIntel=SQL_NTS;
	SQLINTEGER nChaPower = 0, cbChaPower=SQL_NTS; 
	
	SQLINTEGER nChaStrong = 0, cbChaStrong=SQL_NTS;   
	SQLINTEGER nChaSpirit = 0, cbChaSpirit=SQL_NTS;
	SQLINTEGER nChaStrength = 0, cbChaStrength=SQL_NTS; 
	SQLINTEGER nChaLevel = 0, cbChaLevel=SQL_NTS;
	SQLINTEGER nChaHair = 0, cbChaHair = SQL_NTS;
	
	SQLINTEGER nChaFace = 0, cbChaFace = SQL_NTS;	
	SQLINTEGER nChaBright = 0, cbChaBright=SQL_NTS;
	
	// 2005-12-29 Jgkim:Add ChaSex, ChaHairColor 
	SQLSMALLINT nChaSex = 0; SQLINTEGER cbChaSex = SQL_NTS;
	SQLINTEGER nChaHairColor =0, cbChaHairColor = SQL_NTS;

	//add reborn
	SQLINTEGER nChaReborn =0, cbChaReborn = SQL_NTS;

	SQLINTEGER nChaSaveMap = 0, cbChaSaveMap=SQL_NTS;
	SQLINTEGER nChaHP = 0, cbChaHP=SQL_NTS;

	LONGLONG   llChaExp = 0; SQLINTEGER cbChaExp=SQL_NTS;

	while (true)
	{
		sReturn = ::SQLFetch(pConn->hStmt);
		if (sReturn == SQL_ERROR || sReturn == SQL_SUCCESS_WITH_INFO)
        {
            Print(szTemp);		
			Print(GetErrorString(pConn->hStmt));
            m_pGameDB->FreeConnection(pConn);

			//strTemp.freeze( false );	// Note : std::strstream�� freeze. �� �ϸ� Leak �߻�.
            return DB_ERROR;
		}

 		if (sReturn == SQL_SUCCESS || sReturn == SQL_SUCCESS_WITH_INFO)
		{	
			// Bind data
			sci->m_dwCharID = nChaNum;
			::SQLGetData(pConn->hStmt,  1, SQL_C_CHAR, szChaName, CHR_ID_LENGTH+1, &cbChaName);
			::SQLGetData(pConn->hStmt,  2, SQL_C_LONG, &nChaClass, 0, &cbChaClass);
			::SQLGetData(pConn->hStmt,  3, SQL_C_LONG, &nChaSchool, 0, &cbChaSchool);
			::SQLGetData(pConn->hStmt,  4, SQL_C_LONG, &nChaDex,   0, &cbChaDex);
			::SQLGetData(pConn->hStmt,  5, SQL_C_LONG, &nChaIntel, 0, &cbChaIntel);
			::SQLGetData(pConn->hStmt,  6, SQL_C_LONG, &nChaPower, 0, &cbChaPower);

			::SQLGetData(pConn->hStmt,  7, SQL_C_LONG, &nChaStrong, 0, &cbChaStrong);
			::SQLGetData(pConn->hStmt,  8, SQL_C_LONG, &nChaSpirit, 0, &cbChaSpirit);
			::SQLGetData(pConn->hStmt,  9, SQL_C_LONG, &nChaStrength, 0, &cbChaStrength);
			::SQLGetData(pConn->hStmt, 10, SQL_C_LONG, &nChaLevel,    0, &cbChaLevel);
			::SQLGetData(pConn->hStmt, 11, SQL_C_LONG, &nChaHair,	  0, &cbChaHair);

			::SQLGetData(pConn->hStmt, 12, SQL_C_LONG,	&nChaFace,     0, &cbChaFace);
			::SQLGetData(pConn->hStmt, 13, SQL_C_LONG,  &nChaBright,   0, &cbChaBright);

			// 2005-12-29 Jgkim:Add nChaSex, nChaHairColor
			::SQLGetData(pConn->hStmt, 14, SQL_SMALLINT, &nChaSex,       0, &cbChaSex);
			::SQLGetData(pConn->hStmt, 15, SQL_C_LONG,	 &nChaHairColor, 0, &cbChaHairColor);			

			::SQLGetData(pConn->hStmt, 16, SQL_C_SBIGINT,&llChaExp,    0, &cbChaExp);

			::SQLGetData(pConn->hStmt, 17, SQL_C_LONG, &nChaSaveMap,    0, &cbChaSaveMap);
			::SQLGetData(pConn->hStmt, 18, SQL_C_LONG, &nChaHP,			0, &cbChaHP);

			//add reborn
			::SQLGetData(pConn->hStmt, 19, SQL_C_LONG, &nChaReborn,			0, &cbChaReborn);

			 if (cbChaName != 0 && cbChaName != -1) 				 
				::StringCchCopy(sci->m_szName, CHR_ID_LENGTH, (const char*) szChaName);

			 sci->m_emClass = EMCHARCLASS(nChaClass);
			 sci->m_wSchool = (WORD) nChaSchool;
			 sci->m_sStats.dwDex = (DWORD) nChaDex;
			 sci->m_sStats.dwInt = (DWORD) nChaIntel;
			 sci->m_sStats.dwPow = (DWORD) nChaPower;

			 sci->m_sStats.dwStr = (DWORD) nChaStrong;
			 sci->m_sStats.dwSpi = (DWORD) nChaSpirit;
			 sci->m_sStats.dwSta = (DWORD) nChaStrength;
			 sci->m_wLevel      = (WORD) nChaLevel;
			 sci->m_wHair		= (WORD) nChaHair;

			 sci->m_wFace       = (WORD) nChaFace;
			 sci->m_nBright     = nChaBright;

			 // 2005-12-29 Jgkim:Add nChaSex, nChaHairColor
			 sci->m_wSex        = (WORD) nChaSex;
			 sci->m_wHairColor  = (WORD) nChaHairColor;

			 sci->m_sHP.dwNow	= (DWORD) nChaHP;			 
			 sci->m_sSaveMapID  = (DWORD) nChaSaveMap;

			 sci->m_sExperience.lnNow  = llChaExp;

			 //add reborn
			 sci->m_dwReborn  = (DWORD) nChaReborn;

			 nRowCount++;
		}
		else
		{
			break;
		}
		Sleep( 0 );
	}
	//strTemp.freeze( false );	// Note : std::strstream�� freeze. �� �ϸ� Leak �߻�.

	m_pGameDB->FreeConnection(pConn);

	// �ݵ�� nRowCount �� 1 �̾�� �Ѵ�.
	if (nRowCount != 1)
	{
		return DB_ERROR;
	}

	// ��������� ����
	CByteStream ByteStream;	
	//std::strstream strPutOnItems;
	//strPutOnItems << "SELECT ChaInfo.ChaPutOnItems FROM ChaInfo where (ChaNum=" <<  nChaNum << ")";
	//strPutOnItems << std::ends;
	sReturn = m_pGameDB->ReadImage("ChaInfo.ChaPutOnItems", nChaNum, ByteStream);
	//strPutOnItems.freeze( false );	// Note : std::strstream�� freeze. �� �ϸ� Leak �߻�.

	if (sReturn == DB_ERROR)
		return DB_ERROR;
	else
		SETPUTONITEMS_BYBUF(sci->m_PutOnItems,ByteStream);

	return DB_OK;
}

int COdbcManager::GetChaPhoneNumber(
	int nChaNum,
	TCHAR* szPhoneNumber )
{
	SQLRETURN sReturn = 0;
	ODBC_STMT* pConn = m_pGameDB->GetConnection();
	if (!pConn) return DB_ERROR;
	
	// ĳ���� ������ �����´�.
	//std::strstream strTemp;
	//strTemp << "SELECT ChaPhone ";
	//strTemp << " FROM ChaSmsInfo WHERE ChaNum=" << nChaNum;
	//strTemp << std::ends;

	TCHAR szTemp[128] = {0};
	_snprintf( szTemp, 128, "SELECT ChaPhone FROM ChaSmsInfo WHERE ChaNum=%d", nChaNum );

	sReturn = ::SQLExecDirect(pConn->hStmt,
							(SQLCHAR*)szTemp,
							SQL_NTS);

	if ((sReturn != SQL_SUCCESS) && (sReturn != SQL_SUCCESS_WITH_INFO)) 
	{
        Print(szTemp);		
		Print(GetErrorString(pConn->hStmt));
		m_pGameDB->FreeConnection(pConn);

		//strTemp.freeze( false );	// Note : std::strstream�� freeze. �� �ϸ� Leak �߻�.
        return DB_ERROR;
	}

	SQLCHAR szChaPhNum[SMS_RECEIVER] = {0}; SQLINTEGER cbChaPhNum = SQL_NTS;

	while (true)
	{
		sReturn = ::SQLFetch(pConn->hStmt);
		if (sReturn == SQL_ERROR || sReturn == SQL_SUCCESS_WITH_INFO)
        {
            Print(szTemp);		
			Print(GetErrorString(pConn->hStmt));
            m_pGameDB->FreeConnection(pConn);

			//strTemp.freeze( false );	// Note : std::strstream�� freeze. �� �ϸ� Leak �߻�.
            return DB_ERROR;
		}

 		if (sReturn == SQL_SUCCESS || sReturn == SQL_SUCCESS_WITH_INFO)
		{	
			// Bind data			
			::SQLGetData(pConn->hStmt,  1, SQL_C_CHAR, szChaPhNum, SMS_RECEIVER, &cbChaPhNum);			

			if (cbChaPhNum != 0 && cbChaPhNum != -1) 				 
				::StringCchCopy( szPhoneNumber, SMS_RECEIVER, (const char*) szChaPhNum );			
		}
		else
		{
			break;
		}
		Sleep( 0 );
	}
	//strTemp.freeze( false );	// Note : std::strstream�� freeze. �� �ϸ� Leak �߻�.

	m_pGameDB->FreeConnection(pConn);

	return DB_OK;
}


// ��Ʈ�� Ž������ �ý����� �߰� ���� �ҷ����� �κ� �߰�
int COdbcManager::GetVTCharInfo( int nChaNum,
								SCHARDATA2* pChaData2 )
{
	SQLRETURN sReturn = 0;
	ODBC_STMT* pConn = m_pGameDB->GetConnection();
	if (!pConn) return DB_ERROR;

	TCHAR szTemp[128] = {0};
	_snprintf( szTemp, 128, "SELECT saveMoney, saveExp, itemCount FROM ChaInfo WHERE ChaNum=%d", nChaNum );

	sReturn = ::SQLExecDirect(pConn->hStmt,
		(SQLCHAR*)szTemp,
		SQL_NTS);

	if ((sReturn != SQL_SUCCESS) && (sReturn != SQL_SUCCESS_WITH_INFO)) 
	{
		Print(szTemp);		
		Print(GetErrorString(pConn->hStmt));
		m_pGameDB->FreeConnection(pConn);

		return DB_ERROR;
	}

	LONGLONG   llChaSaveExp = 0, llChaSaveMoney = 0;	
	SQLINTEGER cbChaSaveExp = SQL_NTS, cbChaSaveMoney = SQL_NTS; 
	SQLINTEGER nChaitemCount = 0, cbChaitemCount = SQL_NTS;

	while (true)
	{
		sReturn = ::SQLFetch(pConn->hStmt);

		if (sReturn == SQL_SUCCESS || sReturn == SQL_SUCCESS_WITH_INFO)
		{	
			// Bind data			
			::SQLGetData(pConn->hStmt,1, SQL_C_SBIGINT, &llChaSaveMoney,  0, &cbChaSaveMoney);
			::SQLGetData(pConn->hStmt,2, SQL_C_SBIGINT, &llChaSaveExp,    0, &cbChaSaveExp);
			::SQLGetData(pConn->hStmt,3, SQL_C_LONG, &nChaitemCount,    0, &cbChaitemCount);

			pChaData2->m_lVNGainSysMoney        = llChaSaveMoney;
			pChaData2->m_lVNGainSysExp			= llChaSaveExp;
			pChaData2->m_dwVietnamInvenCount	= nChaitemCount;
		}
		else
		{
			break;
		}
		Sleep( 0 );
	}

	m_pGameDB->FreeConnection(pConn);

	return DB_OK;
}

// �븸 ��ŷ ���� �����ؼ� �ֱ� �α׾ƿ� �������� ������ DB���� �о��.
int COdbcManager::GetLastCharInfo( 
								  int nUserNum,
								  int nChaNum,
								  SCHARDATA2* pChaData2 )
{
	SQLRETURN sReturn = 0;
	{		
		ODBC_STMT* pConn = m_pGameDB->GetConnection();
		if (!pConn) return DB_ERROR;
	
		TCHAR szTemp[128] = {0};
		_snprintf( szTemp, 128, "SELECT ChaMoney,ChaLevel FROM ChaLastInfo WHERE ChaNum=%d", nChaNum );
	
		sReturn = ::SQLExecDirect(pConn->hStmt,
			(SQLCHAR*)szTemp,
			SQL_NTS);
	
		if ((sReturn != SQL_SUCCESS) && (sReturn != SQL_SUCCESS_WITH_INFO)) 
		{
			Print(szTemp);		
			Print(GetErrorString(pConn->hStmt));
			m_pGameDB->FreeConnection(pConn);
	
			return DB_ERROR;
		}
	
		// ChaLastInfo Table�� �ݾ� �÷��� ��ȣȭ�� ���ؼ� ChaMoney �÷��� DataType�� int -> varchar(100)���� �����Ѵ�.
		//	LONGLONG   llChaSaveMoney = 0;	
		//	SQLINTEGER cbChaSaveMoney = SQL_NTS; 
	
		// ĳ���� �α׾ƿ� �Ҷ��� ChaMoney
		SQLCHAR    szTempMoney[CHAR_TEMPMONEY] = {0}; SQLINTEGER cbTempMoney = SQL_NTS;
		SQLINTEGER nChaLevel = 0, cbChaLevel = SQL_NTS;
	
		while (true)
		{
			sReturn = ::SQLFetch(pConn->hStmt);
	
			if (sReturn == SQL_SUCCESS || sReturn == SQL_SUCCESS_WITH_INFO)
			{	
				// Bind data			
				//			::SQLGetData(pConn->hStmt,1, SQL_C_SBIGINT, &llChaSaveMoney,  0, &cbChaSaveMoney);
				::SQLGetData(pConn->hStmt, 1, SQL_C_CHAR, szTempMoney, CHAR_TEMPMONEY, &cbTempMoney);
				::SQLGetData(pConn->hStmt,2, SQL_C_LONG, &nChaLevel,    0, &cbChaLevel);
	
				//			pChaData2->m_lnTempMoney    = llChaSaveMoney;			
				if (cbTempMoney != 0 && cbTempMoney != -1) // TempMoney �� ����
				{
					char szTempMoney1[CHAR_TEMPMONEY], szTemp[2];
					LONGLONG lnTempMoeny = 0;
	
					::StringCchCopy(szTempMoney1, CHAR_TEMPMONEY, (const char*) szTempMoney);
	
					size_t i;
					for( i = 0; i < strlen(szTempMoney1); i++ )
					{
						szTempMoney1[i] -= 20;
						sprintf( szTemp, "%c", szTempMoney1[i] );
						lnTempMoeny = ( lnTempMoeny * 10 ) + atoi(szTemp);
					}
					/*LONGLONG lnTempValue = (nChaNum * pChaData2->m_wLevel);

					lnTempMoeny -= lnTempValue;*/
					pChaData2->m_lnTempMoney = lnTempMoeny >> 0x04;
				}
				pChaData2->m_wTempLevel		= nChaLevel;
			}
			else
			{
				break;
			}
			Sleep( 0 );
		}
	
		m_pGameDB->FreeConnection(pConn);
	}

	{	
		sReturn = 0;
		ODBC_STMT* pConn = m_pGameDB->GetConnection();
		if (!pConn) return DB_ERROR;

		TCHAR szTemp[128] = {0};
		_snprintf( szTemp, 128, "SELECT UserMoney FROM UserLastInfo WHERE UserNum=%d", nUserNum );

		sReturn = ::SQLExecDirect(pConn->hStmt,
			(SQLCHAR*)szTemp,
			SQL_NTS);

		if ((sReturn != SQL_SUCCESS) && (sReturn != SQL_SUCCESS_WITH_INFO)) 
		{
			Print(szTemp);
			Print(GetErrorString(pConn->hStmt));
			m_pGameDB->FreeConnection(pConn);

			return DB_ERROR;
		}

		// UserLastInfo Table�� �ݾ� �÷��� ��ȣȭ�� ���ؼ� ChaMoney �÷��� DataType�� int -> varchar(100)���� �����Ѵ�.
		//	LONGLONG   llUserSaveMoney = 0;
		//	SQLINTEGER cbUserSaveMoney = SQL_NTS;	
		SQLCHAR    szTempStorageMoney[CHAR_TEMPMONEY] = {0}; 
		SQLINTEGER cbTempStorageMoney = SQL_NTS;

		while (true)
		{
			sReturn = ::SQLFetch(pConn->hStmt);

			if (sReturn == SQL_SUCCESS || sReturn == SQL_SUCCESS_WITH_INFO)
			{
				// Bind data
				//			::SQLGetData(pConn->hStmt,1, SQL_C_SBIGINT, &llUserSaveMoney,  0, &cbUserSaveMoney);
				::SQLGetData(pConn->hStmt, 1, SQL_C_CHAR, szTempStorageMoney, CHAR_TEMPMONEY, &cbTempStorageMoney);

				//			pChaData2->m_lnTempStorageMoney  = llUserSaveMoney;
				if (cbTempStorageMoney != 0 && cbTempStorageMoney != -1) // TempStorageMoney �� ����
				{
					char szTempStorageMoney1[CHAR_TEMPMONEY], szTemp[2];
					LONGLONG lnTempMoeny = 0;

					::StringCchCopy(szTempStorageMoney1, CHAR_TEMPMONEY, (const char*) szTempStorageMoney);

					size_t i;
					for( i = 0; i < strlen(szTempStorageMoney1); i++ )
					{
						szTempStorageMoney1[i] -= 20;
						sprintf( szTemp, "%c", szTempStorageMoney1[i] );
						lnTempMoeny = ( lnTempMoeny * 10 ) + atoi(szTemp);
					}
					/*LONGLONG lnTempValue = (nUserNum);

					lnTempMoeny -= lnTempValue;*/
					pChaData2->m_lnTempStorageMoney = lnTempMoeny >> 0x04;
				}
			}
			else
			{
				break;
			}
			Sleep( 0 );
		}

		m_pGameDB->FreeConnection(pConn);
	}

	return DB_OK;
}


