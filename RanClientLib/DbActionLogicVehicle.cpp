#include "stdafx.h"
#include "./DbActionLogic.h"

#include "../NetServerLib/s_CSessionServer.h"
#include "../NetServerLib/s_CFieldServer.h"
#include "../NetServerLib/s_CAgentServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/**
* ���ο� Ż���� �����Ѵ�.
* \param nChaNum  Ż�� ������ ĳ���� ��ȣ
* \param szVehicleName Ż�� �̸�
* \param nVehicleType Ż���� Ÿ��
* \param nVehicleMID Ż�� MID
* \param nVehicleSID Ż�� SID
* \param nVehicleCardMID Ż���� ������ MID
* \param nVehicleCardSID Ż���� ������ SID
* \return DB_ERROR �Ǵ� ������������ Ż���� ������ȣ
*/
CCreateVehicle::CCreateVehicle(
	DWORD dwClientID,
	DWORD dwCharNum,
	const TCHAR* szVehicleName,
	int nVehicleType,
	int nVehicleCardMID,
	int nVehicleCardSID
	)
	: m_dwCharNum ( dwCharNum )
	, m_nVehicleType(nVehicleType)
	, m_nVehicleCardMID(nVehicleCardMID)
	, m_nVehicleCardSID(nVehicleCardSID)
{
	m_dwClient = dwClientID;
	m_strVehicleName = szVehicleName;
	m_strVehicleName.Trim(_T(" ")); // �յ� ��������
}

int CCreateVehicle::Execute(CServer* pServer)
{
	int nResult = m_pDbManager->CreateVehicle( m_dwCharNum,
		m_strVehicleName.GetString(),
		m_nVehicleType,
		m_nVehicleCardMID, 
		m_nVehicleCardSID
		);	
	
	GLMSG::SNET_VEHICLE_CREATE_FROMDB_FB InternalMsg;
	
	// ������������ ���� �޽��� �߼�
	if ( nResult != DB_ERROR )
	{
		InternalMsg.emTYPE = (VEHICLE_TYPE)m_nVehicleType;
		InternalMsg.sVehicleID.wMainID = m_nVehicleCardMID;
		InternalMsg.sVehicleID.wSubID = m_nVehicleCardSID;
		InternalMsg.dwVehicleID = nResult;
		CFieldServer* pTemp = reinterpret_cast<CFieldServer*> (pServer);
		pTemp->InsertMsg ( m_dwClient, (char*) &InternalMsg );
	}
	else
	{

		// �����ϸ� ����ī���� �ֹ�ȣ�� �ٽ� 0���� �ǵ��� ���´�. (����� �����ϰ� �ϱ� ����)
		PGLCHAR pOwner = GLGaeaServer::GetInstance().GetCharID ( m_dwCharNum );
		if ( pOwner )
		{
			SITEMCUSTOM& sItemCustom = pOwner->m_PutOnItems[SLOT_VEHICLE];
			if ( sItemCustom.sNativeID != NATIVEID_NULL() ) sItemCustom.dwVehicleID = 0;

			pOwner->m_bGetVehicleFromDB = false;
		}
	}

	return nResult;
}


/**
* Ż���� ������ �����´�.
*/
CGetVehicle::CGetVehicle(
	GLVEHICLE* pVehicle,
	DWORD dwVehicleNum,
	DWORD dwClientID,
	DWORD dwCharNum,
	bool  bLMTItemCheck,
	bool  bCardInfo,
	bool  bTrade
	)
	: m_pVehicle( pVehicle )
	, m_dwVehicleNum ( dwVehicleNum )
	, m_dwCharNum ( dwCharNum )
	, m_bLMTItemCheck ( bLMTItemCheck )
	, m_bCardInfo ( bCardInfo )
	, m_bTrade ( bTrade )

{
	m_dwClient = dwClientID;
}

int CGetVehicle::Execute(CServer* pServer)
{
	int nResult = m_pDbManager->GetVehicle( m_pVehicle,
		m_dwVehicleNum,
		m_dwCharNum );

	if ( nResult == DB_OK )
	{
		GLMSG::SNET_VEHICLE_GET_FROMDB_FB InternalMsg;
		InternalMsg.pVehicle = m_pVehicle;
		InternalMsg.dwVehicleNum = m_dwVehicleNum;
		InternalMsg.bLMTItemCheck = m_bLMTItemCheck;
		InternalMsg.bCardInfo = m_bCardInfo;
		InternalMsg.bTrade = m_bTrade;

		CFieldServer* pTemp = reinterpret_cast<CFieldServer*> (pServer);
		pTemp->InsertMsg ( m_dwClient, (char*) &InternalMsg );
	}
	else if ( nResult == DB_ROWCNT_ERROR )
	{

		GLMSG::SNET_VEHICLE_GET_FROMDB_ERROR InternalMsg;		

		InternalMsg.bLMTItemCheck = m_bLMTItemCheck;
		InternalMsg.bCardInfo	  = m_bCardInfo;
		InternalMsg.bTrade		  = m_bTrade;

		CFieldServer* pTemp = reinterpret_cast<CFieldServer*> (pServer);
		pTemp->InsertMsg ( m_dwClient, (char*) &InternalMsg );

		SAFE_DELETE ( m_pVehicle );		
	}
	else
	{
		PGLCHAR pOwner = GLGaeaServer::GetInstance().GetCharID ( m_dwCharNum );
		if ( pOwner )
		{
			pOwner->m_bGetVehicleFromDB = false;
		}
		
		SAFE_DELETE ( m_pVehicle );
	}

	

	return nResult;
}

/**
* Ż���� ���͸��� �����Ѵ�. ���͸��� 0-1000 ����
* \param nVehicleBttery ���͸��� ������ Ż���� ��ȣ
* \param nVehicleBttery ���͸�
*/
CSetVehicleBattery::CSetVehicleBattery(
	DWORD dwClientID,
	DWORD dwCharNum,
	int nVehicleNum,
	int nVehicleBttery
	)
	: m_dwCharNum ( dwCharNum )
	, m_nVehicleNum( nVehicleNum )
	, m_nVehicleBttery( nVehicleBttery )
{
	m_dwClient = dwClientID;
}

int CSetVehicleBattery::Execute(CServer* pServer)
{
	return m_pDbManager->SetVehicleBattery( m_nVehicleNum, m_dwCharNum, m_nVehicleBttery );
}

CSetVehicleColor::CSetVehicleColor( //add bike color
	DWORD dwClientID,
	DWORD dwCharNum,
	int nVehicleNum,
	int nVehicleColor,
	int nVehicleColor1,
	int nVehicleColor2,
	int nVehicleColor3,
	int nVehicleColor4,
	int nVehicleColor5
	)
	: m_dwCharNum ( dwCharNum )
	, m_nVehicleNum( nVehicleNum )
	, m_nVehicleColor( nVehicleColor )
	, m_nVehicleColor1( nVehicleColor1 )
	, m_nVehicleColor2( nVehicleColor2 )
	, m_nVehicleColor3( nVehicleColor3 )
	, m_nVehicleColor4( nVehicleColor4 )
	, m_nVehicleColor5( nVehicleColor5 )
{
	m_dwClient = dwClientID;
}

int CSetVehicleColor::Execute(CServer* pServer)
{
	return m_pDbManager->SetVehicleColor( m_nVehicleNum, m_dwCharNum, m_nVehicleColor, m_nVehicleColor1, m_nVehicleColor2, m_nVehicleColor3, m_nVehicleColor4, m_nVehicleColor5 );
}


CGetVehicleBattery::CGetVehicleBattery( 
	DWORD dwClientID, 
	DWORD dwCharNum,
	int	  nVehicleNum, 
	SNATIVEID	sItemID,
	WORD  wCureVolume, 
	BOOL  bRatio, 
	SNATIVEID sBatteryID,
	VEHICLE_TYPE emType )
	: m_dwCharNum ( dwCharNum )
	, m_nVehicleNum( nVehicleNum )	
	, m_sItemID ( sItemID )
	, m_wCureVolume( wCureVolume )
	, m_bRatio ( bRatio )
	, m_sBatteryID ( sBatteryID ) 
	, m_emType( emType )
{
	m_dwClient = dwClientID;
}


int CGetVehicleBattery::Execute(CServer* pServer)
{
	
	int nResult = m_pDbManager->GetVehicleBattery( m_nVehicleNum, m_dwCharNum );

	GLMSG::SNET_VEHICLE_REQ_GET_BATTERY_FROMDB_FB InternalMsg;

	if ( nResult != DB_ERROR )
	{
		InternalMsg.dwVehicleID	 = m_nVehicleNum;
		InternalMsg.sItemID		 = m_sItemID;	
		InternalMsg.nFull		 = nResult;
		InternalMsg.wCureVolume  = m_wCureVolume;
		InternalMsg.bRatio		 = m_bRatio;
		InternalMsg.sBatteryID   = m_sBatteryID;
		InternalMsg.emFB		 = EMVEHICLE_REQ_GET_BATTERY_FROMDB_OK;
		InternalMsg.emType		 = m_emType;
	}
	else
	{
		InternalMsg.emFB		 = EMVEHICLE_REQ_GET_BATTERY_FROMDB_FAIL;
	}

	CFieldServer* pTemp = reinterpret_cast<CFieldServer*> (pServer);
	pTemp->InsertMsg ( m_dwClient, (char*) &InternalMsg );

	
	return 0;
}




/**
* Ż���� �����Ѵ�.
* \param nVehicle ������ Ż���� ��ȣ
*/
CDeleteVehicle::CDeleteVehicle(
	DWORD dwClientID,
	DWORD dwCharNum,
	int nVehicleNum
	)
	: m_dwCharNum ( dwCharNum )
	, m_nVehicleNum( nVehicleNum )
{
	m_dwClient = dwClientID;
}

int CDeleteVehicle::Execute(CServer* pServer)
{
	return m_pDbManager->DeleteVehicle( m_nVehicleNum, m_dwCharNum );
}



/**
* Ż���� �κ��丮 ������Ʈ
*/
CSetVehicleInven::CSetVehicleInven(		
	DWORD dwClientID,
	DWORD dwCharNum,
	int nVehicleNum,	
	GLVEHICLE* pVehicle
	)
	: m_dwCharNum ( dwCharNum )
	, m_nVehicleNum(nVehicleNum)
{	
	m_dwClient = dwClientID;
	pVehicle->GETPUTONITEMS_BYBUF( m_ByteStream );
}

int CSetVehicleInven::Execute(CServer* pServer)
{	
	return m_pDbManager->SetVehicleInven(
		m_nVehicleNum,
		m_dwCharNum,
		m_ByteStream);
}