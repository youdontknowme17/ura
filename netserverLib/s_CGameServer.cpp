///////////////////////////////////////////////////////////////////////////////
// s_CGameServer.cpp
//
// class CGameServer
//
// * History
// 2002.05.30 jgkim Create
//
// Copyright 2002-2003 (c) Mincoms. All rights reserved.                 
// 
// * Note
// 
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "GLChar.h"
#include "GLContrlMsg.h"
#include "GLGaeaServer.h"
#include "DxServerInstance.h"

#include "s_CGameServer.h"
#include "s_CDbAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static DWORD WINAPI CServerListenProc  (CGameServer *pThis) { return pThis->ListenProc();   }
static DWORD WINAPI CServerWorkProc    (CGameServer *pThis) { return pThis->WorkProc();     }
static DWORD WINAPI CServerUpdateProc  (CGameServer *pThis) { return pThis->UpdateProc();   }
static DWORD WINAPI CServerDatabaseProc(CGameServer *pThis) { return pThis->DatabaseProc(); }

CGameServer::CGameServer(HWND hWnd, HWND hEditBox, HWND hEditBoxInfo) 
	: CServer(hWnd, hEditBox, hEditBoxInfo),
	m_pClientManager (NULL),
	m_sSession (INVALID_SOCKET)	
{	
}

CGameServer::~CGameServer()
{
	SAFE_DELETE(m_pClientManager);	
}

// Ŭ���̾�Ʈ �������� Ŭ���� ����
int	CGameServer::StartClientManager()
{	
	SAFE_DELETE(m_pClientManager);
	m_pClientManager = new CClientGame(m_nMaxClient, m_pSendIOCP, m_pRecvIOCP, m_hIOServer);
	if (!m_pClientManager) 
	{
		m_bIsRunning = false;
		return NET_ERROR;
	}
	else
	{
		return NET_OK;
	}
}

// Create DB manager and Open DB
int	CGameServer::StartDbManager()
{
	int nRetCode;
	
	CDbExecuter::GetInstance()->SetServer((CServer*) this);
	nRetCode = COdbcManager::GetInstance()->OpenUserDB(CCfg::GetInstance()->GetUserOdbcName(),
													CCfg::GetInstance()->GetUserOdbcUser(),
													CCfg::GetInstance()->GetUserOdbcPass(),
													CCfg::GetInstance()->GetUserOdbcDatabase(),
													CCfg::GetInstance()->GetUserOdbcPoolSize(),
													CCfg::GetInstance()->GetUserOdbcResponseTime());
	if (nRetCode == DB_ERROR)
	{
		//DB_Shutdown();
		CConsoleMessage::GetInstance()->Write(C_MSG_CONSOLE, "ERROR:User DB Open Error");
		m_bIsRunning = false;
		return NET_ERROR;
	}
	
	nRetCode = COdbcManager::GetInstance()->OpenGameDB(CCfg::GetInstance()->GetGameOdbcName(),
													CCfg::GetInstance()->GetGameOdbcUser(),
													CCfg::GetInstance()->GetGameOdbcPass(),
													CCfg::GetInstance()->GetGameOdbcDatabase(),
													CCfg::GetInstance()->GetGameOdbcPoolSize(),
													CCfg::GetInstance()->GetGameOdbcResponseTime());
	if (nRetCode == DB_ERROR)
	{
		//DB_Shutdown();
		CConsoleMessage::GetInstance()->Write(C_MSG_CONSOLE, "Game DB Open Error");
		m_bIsRunning = false;
		return NET_ERROR;
	}

	CConsoleMessage::GetInstance()->Write(C_MSG_CONSOLE, "Database Open OK");
	return NET_OK;
}

int CGameServer::Start()
{
	HRESULT hr;
	
	m_bIsRunning = true;
	
	if (StartCfg()			== NET_ERROR) return NET_ERROR;	
	if (StartIOCPList()		== NET_ERROR) return NET_ERROR;    	
	if (StartMsgManager()	== NET_ERROR) return NET_ERROR;
	if (StartIOCP()			== NET_ERROR) return NET_ERROR;
	if (StartClientManager()== NET_ERROR) return NET_ERROR;	
	if (StartDbManager()    == NET_ERROR) return NET_ERROR;
	if (StartWorkThread()	== NET_ERROR) return NET_ERROR;
	if (StartUpdateThread() == NET_ERROR) return NET_ERROR;	

	///////////////////////////////////////////////////////////////////////////
	//	Note : ���̾� ���� ����.
	SERVER_UTIL::CSystemInfo SysInfo;
	hr = DxFieldInstance::Create ( SysInfo.GetAppPath(), this, CConsoleMessage::GetInstance(), COdbcManager::GetInstance() );
	if ( FAILED(hr) )
	{
		m_bIsRunning = false;
		return NET_ERROR;
	}

	if (StartListenThread() == NET_ERROR) return NET_ERROR;
	
	CConsoleMessage::GetInstance()->Write(C_MSG_CONSOLE, "=======================================================");	
	CConsoleMessage::GetInstance()->Write(C_MSG_CONSOLE, "Server Start OK");	
	CConsoleMessage::GetInstance()->Write(C_MSG_CONSOLE, "=======================================================");

	m_nStatus = S_SERVER_RUNING;

	return NET_OK;
}

int CGameServer::Stop()
{
	// Stop All Thread and exit
	DWORD dwExitCode = 0;

	CConsoleMessage::GetInstance()->Write(C_MSG_CONSOLE, "=========== Please wait until server stop =============");
	CConsoleMessage::GetInstance()->Write(C_MSG_CONSOLE, "=========== UpdateThread ���Ḧ ��ٸ��� �ֽ��ϴ�.");

	LockOn();
	m_bIsRunning = false;
	LockOff();

	while ( m_hUpdateThread )	{	Sleep(0); }
	CConsoleMessage::GetInstance()->Write(C_MSG_CONSOLE, "=========== UpdateThread�� ����Ǿ����ϴ�.");


	// Ŭ���̾�Ʈ ���� Ŭ���� ����
	SAFE_DELETE(m_pClientManager);
	CConsoleMessage::GetInstance()->Write(C_MSG_CONSOLE, "Client Manager closed OK");
	// IOCP ����
	SAFE_DELETE(m_pSendIOCP);
	SAFE_DELETE(m_pRecvIOCP);
	// Message Queue ����
	SAFE_DELETE(m_pRecvMsgManager);
	CConsoleMessage::GetInstance()->Write(C_MSG_CONSOLE, "Message Manager closed OK");
	// DB ��������
	COdbcManager::GetInstance()->ReleaseInstance();
	CConsoleMessage::GetInstance()->Write(C_MSG_CONSOLE, "Database Manager closed OK");
	// CFG class ����
	CCfg::GetInstance()->ReleaseInstance();

	// Put message to console
	CConsoleMessage::GetInstance()->Write(C_MSG_CONSOLE, "=======================================================");	
	CConsoleMessage::GetInstance()->Write(C_MSG_CONSOLE, "Server Stop OK");	
	CConsoleMessage::GetInstance()->Write(C_MSG_CONSOLE, "=======================================================");
	
	m_nStatus = S_SERVER_STOP;

	return NET_OK;
}

int	CGameServer::Pause()
{
	::closesocket(m_sServer);
	m_sServer = INVALID_SOCKET;
	CloseAllClient();
	m_nStatus = S_SERVER_PAUSE;
	// Put message to console
	CConsoleMessage::GetInstance()->Write(C_MSG_FILE_CONSOLE, "=======================================================");	
	CConsoleMessage::GetInstance()->Write(C_MSG_FILE_CONSOLE, "Server Pause OK");	
	CConsoleMessage::GetInstance()->Write(C_MSG_FILE_CONSOLE, "=======================================================");

	return NET_OK;	
}

int CGameServer::Resume()
{
	if (StartListenThread() == NET_OK)
	{
		m_nStatus = S_SERVER_RUNING;
		// Put message to console
		CConsoleMessage::GetInstance()->Write(C_MSG_FILE_CONSOLE, "=======================================================");	
		CConsoleMessage::GetInstance()->Write(C_MSG_FILE_CONSOLE, "Server Resume OK");	
		CConsoleMessage::GetInstance()->Write(C_MSG_FILE_CONSOLE, "=======================================================");

		return NET_OK;
	}
	else
	{
		// Put message to console
		CConsoleMessage::GetInstance()->Write(C_MSG_FILE_CONSOLE, "=======================================================");	
		CConsoleMessage::GetInstance()->Write(C_MSG_FILE_CONSOLE, "Server Resume Failed");	
		CConsoleMessage::GetInstance()->Write(C_MSG_FILE_CONSOLE, "=======================================================");

		return NET_ERROR;
	}
}

int	CGameServer::ReStart()
{
	Stop();
	Start();
	return NET_OK;
}

// close all client connections
void CGameServer::CloseAllClient()
{	
	if (m_pClientManager)
	{
		for (DWORD i=0; i < (DWORD) m_pClientManager->GetMaxClient(); i++)
		{
			if (m_pClientManager->IsOnline(i))
				CloseClient(i);
		}		
	}
}

CGameServer::ListenProc(void)
{
	LPPER_IO_OPERATION_DATA IOData;
	SOCKET	Accept;
	DWORD	dwRecvNumBytes = 0;
	DWORD	Flags = 0;
	HANDLE	hRetCode = NULL;
	int		nClientNumber = 0;
	int		nRetCode;

	while (m_bIsRunning)
	{
		Accept = WSAAccept(m_sServer, NULL, NULL, NULL, 0);
		if (Accept == INVALID_SOCKET) 
		{
			nRetCode = ::WSAGetLastError();
			CConsoleMessage::GetInstance()->Write("ERROR:WSAAccept %d", nRetCode);
			if (nRetCode == WSAENOTSOCK || nRetCode == WSAEINTR)
			{	
				// CConsoleMessage::GetInstance()->Write("Listen Thread Stopped");
				break;
			}
			else
			{
				continue;
			}
		}

		///////////////////////////////////////////////////////////////////////
		// Create per-handle data information structure to associate with the socket
		nClientNumber = m_pClientManager->GetFreeClientID(); // Get free client slot number
		if (nClientNumber == NET_ERROR) 
		{
			closesocket(Accept);
			CConsoleMessage::GetInstance()->Write(C_MSG_FILE_CONSOLE, "INFO:Reached Max Client Number!");
			continue;
		} 
		else 
		{
			// Set client information
			// Ŭ���̾�Ʈ�� ip, port, ���ӽð��� �����.
			m_pClientManager->SetAcceptedClient(nClientNumber, Accept);
		}

		// Associate the accepted socket with the completion port
		hRetCode = CreateIoCompletionPort((HANDLE) Accept, 
										m_hIOServer, 
										(DWORD) nClientNumber, 
										0);
		if (hRetCode == NULL) 
		{
			CConsoleMessage::GetInstance()->Write(C_MSG_CONSOLE, "CreateIoCompletionPort Error");
			closesocket(Accept);
			continue;
		} 
		
		// Start processing I/O on ther accepted socket
		// First WSARecv, TCP/IP Send 8 bytes (ignored byte)
		// Client ���� ������ �޴´�.
		dwRecvNumBytes = sizeof(NET_MSG_GENERIC);
        
		IOData = (LPPER_IO_OPERATION_DATA) GetFreeOverIO(NET_RECV_POSTED);

		if (IOData == NULL) continue;

		// IOData->DataBuf.buf = IOData->Buffer;
		IOData->dwRcvBytes		= 0;
		IOData->dwTotalBytes	= dwRecvNumBytes;
		// Head receive mode
		m_pClientManager->SetNetMode(nClientNumber, NET_PACKET_HEAD);
						
		WSARecv(Accept,
			&(IOData->DataBuf), 
			1,
			&dwRecvNumBytes,
			&Flags ,
			&(IOData->Overlapped),
			NULL);
		Sleep(0);
	}
	// ServerAcceptThread Running End
	CConsoleMessage::GetInstance()->Write ( C_MSG_CONSOLE, "ListenProc End");
	return 0;	
}

int CGameServer::UpdateProc()
{
	/*
	// ������ ������ ���� ������
	DWORD nTimeS;
	float fFrame			= 0;
	float fCount			= 0;	
	float fUpdateSec		= 30000;		// FPS ǥ�� ���Žð� 30��
	DWORD dwUsrChkTime		= 1800000;		// 30 �и��� �ѹ��� ����� ���Ӳ��� üũ
	DWORD dwUsrChkStart		= timeGetTime(); // �ƹ��͵� ���� �ʴ� ����� ���Ӳ��⸦ ���� �ð�
	DWORD dwUsrSaveTime		= 60000 * 30; // ����� ����ð� 30��
	DWORD dwUsrSaveStart	= timeGetTime(); // ����� ����ð� üũ�� ���� �ð�����
	nTimeS					= timeGetTime();

	MSG_LIST* pMsg = NULL;
	while (m_bIsRunning)
	{		
		// Flip Messge Queue
		m_pRecvMsgManager->MsgQueueFlip();	
		// Get Messages and Processing message
		pMsg = m_pRecvMsgManager->GetMsg();		
		while (pMsg != NULL)
		{
			// �޽��� ó��
			MsgProcess(pMsg);
			pMsg = m_pRecvMsgManager->GetMsg(); // �����޽��� ��������
		}	
		///////////////////////////////////////////////////////////////////////
		// Update Gaea Server
		DxFieldInstance::FrameMove();		

		///////////////////////////////////////////////////////////////////////
		// ������ ����
		if ( (timeGetTime() - nTimeS) >= fUpdateSec )
		{			
			fFrame = ( (float)((fCount * fUpdateSec) /(float)(timeGetTime() - nTimeS)));
			//nFrame = Count; 
			nTimeS = timeGetTime();
			fCount = 0;
			
			CConsoleMessage::GetInstance()->Write ( C_MSG_CONSOLE, 
							"UpdateProc %d FPS, avg %d msec , Player Char : %d ", 
							(int)(fFrame/((fUpdateSec)/1000)), 
							(int)(fUpdateSec/fFrame), GLGaeaServer::GetInstance().GetNumPC() );

			CConsoleMessage::GetInstance()->Write ( C_MSG_CONSOLE, 
							"Current User %d/%d",							
							(int) m_pClientManager->GetCurrentClientNumber(),
							(int) m_pClientManager->GetMaxClient());
		} 
		else
		{
			fCount++;
		}
		Sleep(0);
	}

	TRACE ( "[UpdateProc End]\n" );

	// ���̾� ���� ��ž
	TRACE ( "[Gaea Server CleanUp]\n" );
	GLGaeaServer::GetInstance().CleanUp ();

	//	������Ʈ ������ �����.
	m_hUpdateThread = NULL;
	*/

	return 0;
}

int CGameServer::WorkProc()
{
	LPPER_IO_OPERATION_DATA PerIoData;
	DWORD dwSndBytes;
	DWORD dwRcvBytes;
	DWORD dwByteTrans;
	DWORD Flags;
	DWORD dwClient = -1;
	int	  nRetCode=0;
	PerIoData = NULL;
	DWORD dwMaxClient = m_pClientManager->GetMaxClient();
	NET_MSG_GENERIC* pNmg = NULL;

	while (m_bIsRunning)
	{
		/////////////////////////////////////////////////////////////////////////////
		// Wait for I/O to complete on any socket
		nRetCode = GetQueuedCompletionStatus(
			m_hIOServer,
			&dwByteTrans, 
			(LPDWORD) &dwClient,
			(LPOVERLAPPED *) &PerIoData,			
			INFINITE);
 
		/////////////////////////////////////////////////////////////////////////////
		// ������ �����ϱ� ���ؼ� ����ó�� ��ƾ�� ȣ��������...
		if (dwClient == m_dwCompKey && PerIoData == NULL && dwByteTrans == 0)
		{
			// m_dwCompKey Time to work proc exit
			// ServerWorkerThread Running End		
			return 0;
		}

		/////////////////////////////////////////////////////////////////////////////
        // Illegal Message Skip
		if ((dwClient < 0) || (dwClient >= dwMaxClient))
		{			
			ReleaseOperationData(PerIoData);
			continue;
		}

		/////////////////////////////////////////////////////////////////////////////
		// GetQueuedCompletionStatus() nRetCode=0 
		if (nRetCode == 0)
		{	
			if (PerIoData == NULL) // Time to exit, stop thread
			{
				return 0;
			}
			else // �������� PerIoData != NULL && nRetCode == 0 && dwByteTrans == 0
			{
				ReleaseOperationData(PerIoData);
				CloseClient(dwClient);				
				continue;
			}
		}
		else // (nRetCode != 0)
		{
			if (PerIoData == NULL) // Ŭ���̾�Ʈ�� ���� ����������...
		    { 
				// PostQueuedCompletionStatus post an I/O packet with 
				// a NULL CompletionKey (or if we get one for any reason).  It is time to exit.
				CloseClient(dwClient);
				// SessionSndSvrCurState();
				// if (64 != GetLastError())
				// {
				// Close Client PerIoData == NULL, error code 64
				// }			
				continue;
			}
		}
		
		/////////////////////////////////////////////////////////////////////////////
		// Ŭ���̾�Ʈ�ʿ��� ���������� socket �� close ������...
		if (dwByteTrans == 0 &&
			(PerIoData->OperationType == NET_RECV_POSTED || 
			PerIoData->OperationType == NET_SEND_POSTED))
		{
			// Client Closed. Bytes Transferred 0
			ReleaseOperationData(PerIoData);
			CloseClient(dwClient);			
			continue;
		}

		switch (PerIoData->OperationType)
		{
		// �б� �Ϸ� �뺸�϶�...
		case NET_RECV_POSTED :
			{			
				// Insert to client buffer
                m_pClientManager->addRcvMsg(dwClient, PerIoData->Buffer, dwByteTrans);
                
				while (true) {
					// Get one Message
					pNmg = (NET_MSG_GENERIC*) m_pClientManager->getRcvMsg(dwClient);
					if (NULL == pNmg) {
						break;
					}
					else {
						// Insert to message queue						
						m_pRecvMsgManager->MsgQueueInsert(dwClient, pNmg, pNmg->dwSize);
					}
				}

				// OverlappedIO �޸� ��ȯ
				ReleaseOperationData(PerIoData);
				// WSARecv ��û
				Flags = 0;
				dwRcvBytes = sizeof(NET_MSG_GENERIC);
				PerIoData = (LPPER_IO_OPERATION_DATA) GetFreeOverIO(NET_RECV_POSTED);

				if (PerIoData == NULL) continue;
				
				nRetCode = WSARecv(m_pClientManager->GetSocket(dwClient),
									&(PerIoData->DataBuf), 
									1, 
									&dwRcvBytes,
									&Flags,
									&(PerIoData->Overlapped),
									NULL);
				if ((nRetCode == SOCKET_ERROR) && (WSAGetLastError() != WSA_IO_PENDING))
				{
					// ġ���� ����, �α׿� ��ϳ���
					CConsoleMessage::GetInstance()->Write(C_MSG_FILE_CONSOLE, "CLoginServer::WorkProc WSARecv %d ERROR", nRetCode);
				}
			}
			break;
		// ���� �Ϸ� �뺸 �϶�...
		case NET_SEND_POSTED :
			// �� ���� ����Ʈ��
			dwSndBytes = dwByteTrans + PerIoData->dwSndBytes;
			// ���� �Ϸ��
			if (dwSndBytes >= (DWORD) PerIoData->dwTotalBytes)
			{				
				ReleaseOperationData(PerIoData);
			}
			else // ���� �Ϸ���� ���� ���� ����Ʈ�� ����
			{
				// ���� ����Ʈ�� ������Ʈ
				PerIoData->dwSndBytes = dwSndBytes;
				// �������� ����Ÿ ������ ������Ʈ
				PerIoData->DataBuf.buf = PerIoData->Buffer + dwSndBytes;
				// �������� ����Ʈ�� ������Ʈ
				dwSndBytes = (DWORD) PerIoData->dwTotalBytes - dwSndBytes;
				// ���۱��� ������Ʈ
				PerIoData->DataBuf.len = (u_long) dwSndBytes;
				
				// ���ۿ�û
				Send(dwClient, PerIoData, dwSndBytes);
			}
			break;
		default :
			break;
		}
		Sleep(0);
	}
	// ServerWorkerThread Running End
	CConsoleMessage::GetInstance()->Write ( C_MSG_CONSOLE, "WorkProc End");
	return 0;
}

void CGameServer::Send(DWORD dwClient, LPPER_IO_OPERATION_DATA PerIoData, DWORD dwSize)
{
}

int CGameServer::SendClient ( DWORD dwClient, LPVOID pBuffer )
{
	m_pClientManager->SendClient(dwClient, pBuffer);
	return 0;
}

void CGameServer::SendAllClient(LPVOID pBuffer)
{
	DWORD dwMaxClient;
	DWORD dwStartClient;

	dwStartClient = NET_RESERVED_SLOT;
	dwMaxClient = (DWORD) m_nMaxClient;

	CConsoleMessage::GetInstance()->Write("��ü �������� �÷��̾ �޽����� �����ϴ�");

	for (dwStartClient; dwStartClient < dwMaxClient; dwStartClient++ )
	{
		// �������̸� �޽����� ������
		if (m_pClientManager->IsGaming(dwStartClient))		
			SendClient(dwStartClient, pBuffer);
	}
}

// ����ڸ� �α׾ƿ� ��Ų��.
void CGameServer::CloseClient(DWORD dwClient)
{
	int nUserNum;
	int	nGameTime;	
	DWORD dwGaeaID;
	char* szUserID;	

	if ((m_pClientManager->IsOnline(dwClient) == false) || (dwClient<0))
		return;

	// ������ �������
	if ( (dwClient >= 0) && (dwClient < NET_RESERVED_SLOT) )
	{
		CConsoleMessage::GetInstance()->Write("%d ������ ����� ����Ǿ����ϴ�", dwClient);
		if (dwClient == NET_RESERVED_SESSION)
		{
			m_sSession = INVALID_SOCKET;
		}
		m_pClientManager->CloseClient(dwClient); // lock
	}
	else // �Ϲ� ����� ����
	{
		nUserNum = m_pClientManager->GetUserNum(dwClient);
		CConsoleMessage::GetInstance()->Write("����ڰ� ������ �����߽��ϴ�");
		
		szUserID  = m_pClientManager->GetUserID(dwClient);
		
		// SessionSndLogOutInfoSimple(szUserID);
		
		if ( nUserNum > 0 )
		{		
			nGameTime = m_pClientManager->GetLoginTime(dwClient);
			
			// ����ڰ� ����Ǿ����� �˸�			

			SessionSndLogOutInfo(szUserID, nUserNum, nGameTime);
			CConsoleMessage::GetInstance()->Write("����ڸ� �α׾ƿ� ��ŵ�ϴ�");
			// if ( m_pDB->UserLogout(szUserID, nUserNum, nGameTime) == DB_ERROR )	return;

			CConsoleMessage::GetInstance()->Write ( C_MSG_CONSOLE, "ĳ���͸� ���ӿ��� Drop ( ClientID %d ) ��ŵ�ϴ�", dwClient );
			dwGaeaID = m_pClientManager->GetGaeaID(dwClient);
			if ( dwGaeaID != GAEAID_NULL )
			{
				// ���̾Ƽ��� ĳ���� ��� ����
				GLGaeaServer::GetInstance().ReserveDropOutPC ( dwGaeaID ); // lock
			}
		}
		SessionSndSvrCurState();
		m_pClientManager->CloseClient(dwClient); // lock
	}
}

int CGameServer::MsgProcess(MSG_LIST* pMsg)
{	
	NET_MSG_GENERIC* nmg;
	DWORD dwClient;

	nmg = reinterpret_cast<NET_MSG_GENERIC*> (pMsg->Buffer);
	dwClient = pMsg->dwClient;
	
	// ������ ��� �޽���
	if (dwClient >= 0 && dwClient < NET_RESERVED_SLOT)
	{
		ServerMsgProcess(pMsg);
		return 0;
	}

	switch(nmg->nType)
	{
	case NET_MSG_LOGIN : // �α��� ����Ÿ�϶�...		
		MsgLogIn(pMsg);		
		break;
	case NET_MSG_VERSION_INFO : // Ŭ���̾�Ʈ ��������
		MsgVersion(pMsg);
		break;
	case NET_MSG_REQ_USER_INFO : // ����� ���� ��û
		MsgSndUserInfo(pMsg);
		break;		
	case NET_MSG_CHA_NEW : // ���ο� ĳ���� ����
		MsgCreateCharacter(pMsg);
		break;	
	case NET_MSG_REQ_CHA_BAINFO : // Ŭ���̾�Ʈ->���Ӽ��� : ����� (���÷��̿�)  ĳ���� �������� ��û
        MsgSndChaBasicBAInfo(pMsg);
		break;
	case NET_MSG_REQ_CHA_BINFO : // Ŭ���̾�Ʈ->���Ӽ��� : Ư��ĳ���� �������� ��û (���÷��̿�)
		MsgSndChaBasicInfo(pMsg);
		break;
	case NET_MSG_CHA_DEL : // Ŭ���̾�Ʈ->���Ӽ��� : ĳ���� ����
		MsgSndChaDelInfo(pMsg);
		break;
	case NET_MSG_LOBBY_GAME_JOIN : // Ŭ���̾�Ʈ->���Ӽ��� : ĳ���� ������ ����
        MsgGameJoin(pMsg);
		break;
	//case NET_MSG_CHAT : // ä�� �޽���
	//	MsgChatProcess(pMsg);
	//	break;		
	case NET_MSG_PING_REQUEST : // �ο�û
		MsgSndPingAnswer(pMsg);
		break;

	default:
		{
			if ( nmg->nType > NET_MSG_GCTRL )
			{
				GLGaeaServer::GetInstance().MsgProcess ( (NET_MSG_GENERIC*)pMsg->Buffer, pMsg->dwClient, m_pClientManager->GetGaeaID(pMsg->dwClient) );
			}

			//CloseClient(dwClient);
		}
		break;
	}
	return 0;
}

//void CGameServer::MsgChatProcess(MSG_LIST* pMsg)
//{
//    DWORD dwClient;
//	dwClient = pMsg->dwClient;
//	NET_CHAT* pNc = (NET_CHAT*) pMsg->Buffer;
//	switch (pNc->emType)
//	{
//	case NET_MSG_CHAT_SUB_NORMAL : // �Ϲݸ޽���
//		MsgSndChatNormal(pMsg->dwClient, pNc->szName, pNc->szChatMsg);
//		break;
//
//	case NET_MSG_CHAT_SUB_PARTY : // ��Ƽ�޽���
//		break;
//	case NET_MSG_CHAT_SUB_PRIVATE : // ���θ޽���
//		break;
//	case NET_MSG_CHAT_SUB_GUILD : // ���޽���
//		break;
//	default :
//		break;
//	}
//}

void CGameServer::MsgSndCryptKey(DWORD dwClient)
{
	CRYPT_KEY ck = m_pClientManager->GetNewCryptKey(); // Get crypt key
			
	NET_CRYPT_KEY nck;
	nck.nmg.dwSize	= sizeof(NET_CRYPT_KEY);
	nck.nmg.nType   = NET_MSG_SND_CRYT_KEY;
	nck.ck			= ck;
	SendClient(dwClient, &nck);

	m_pClientManager->SetCryptKey(dwClient, ck); // Set crypt key
}

// ���Ӽ���->Ŭ���̾�Ʈ
// �Ϲ��� ä�� �޽��� ����
void CGameServer::MsgSndChatNormal(DWORD dwClient, const char* szName, const char* szMsg)
{
    NET_CHAT nc;
	nc.nmg.nType = NET_MSG_CHAT;
	nc.emType = CHAT_TYPE_NORMAL;
	StringCchCopy(nc.szName, CHR_ID_LENGTH+1, szName);
	StringCchCopy(nc.szChatMsg, CHAT_MSG_SIZE+1, szMsg);

	SendClient(dwClient, (NET_MSG_GENERIC*) &nc);
}

// Ŭ���̾�Ʈ->���Ӽ��� : ĳ���� ������ ����
void CGameServer::MsgGameJoin(MSG_LIST* pMsg)
{
	NET_GAME_JOIN* pNgj = (NET_GAME_JOIN*) pMsg->Buffer;
	DWORD dwClient = pMsg->dwClient;
	int nChaNum = pNgj->nChaNum;
	int nRetCode = 0;
	DWORD dwItemNum = 0;

	SCHARDATA2 CharData2;

	CharData2.m_dwUserID	= (DWORD) m_pClientManager->GetUserNum(dwClient);// ������ȣ (DB)
	CharData2.m_dwServerID	= (DWORD) m_nServerGroup; // �����׷�
	CharData2.m_dwCharID    = (DWORD) nChaNum; // ĳ���� ��ȣ (DB)

	// ĳ���� ��������
	nRetCode = COdbcManager::GetInstance()->GetCharacterInfo(CharData2.m_dwUserID, nChaNum, &CharData2);
	if (nRetCode == DB_ERROR)
	{
		CConsoleMessage::GetInstance()->Write ( C_MSG_FILE_CONSOLE, "ERROR - ĳ���� DB �б⿡ �����Ͽ����ϴ�." );

		// �б� ����
		return;
	}

	// ĳ���� ����
	PGLCHAR pGLChar = GLGaeaServer::GetInstance().CreatePC ( &CharData2, dwClient, 0 );
	if ( !pGLChar )
	{
		CConsoleMessage::GetInstance()->Write ( C_MSG_FILE_CONSOLE, "ERROR - ĳ���� �ν��Ͻ��� �ʱ�ȭ�� �����Ͽ����ϴ�." );

		//	ĳ���� �������� : ����ó�� �߰�...
		return ;
	}

	// ���̾Ƽ����� ���� ����
	m_pClientManager->SetGaeaID ( dwClient, pGLChar->m_dwGaeaID );

	// Ŭ���̾�Ʈ�� ���� : ĳ�������� + ��ų + ������
	pGLChar->MsgGameJoin();

	CConsoleMessage::GetInstance()->Write("ĳ���� ������ ��� �����Ͽ����ϴ�");

	///////////////////////////////////////////////////////////////////////////
	// ���Ǽ����� ĳ�������� ����
	NET_GAME_JOIN_OK ngjo;
	ngjo.nmg.dwSize	= sizeof(NET_GAME_JOIN_OK);
	ngjo.nmg.nType	= NET_MSG_GAME_JOIN_OK;
	ngjo.nUsrNum	= m_pClientManager->GetUserNum(dwClient);; // ����� ������ȣ(DB)
	ngjo.nChaNum	= nChaNum; // ĳ���͹�ȣ(DB)
	ngjo.dwGaeaID	= pGLChar->m_dwGaeaID; // ���̾� ID
	StringCchCopyN(ngjo.szUserID,	USR_ID_LENGTH, m_pClientManager->GetUserID(dwClient),	USR_ID_LENGTH); // ����� ID	
	StringCchCopyN(ngjo.szChaName,	CHR_ID_LENGTH, CharData2.m_szName,						CHR_ID_LENGTH); // ĳ���͸�

	// ���Ǽ����� ����
	SendSession((LPVOID) &ngjo);

	CConsoleMessage::GetInstance()->Write("���Ǽ����� �������� ������ �����Ͽ����ϴ�");
}

// Ŭ���̾�Ʈ->���Ӽ��� : �ɸ��� ����
void CGameServer::MsgSndChaDelInfo(MSG_LIST* pMsg)
{
	NET_CHA_DEL_FB ncbi;
	DWORD dwSndBytes = 0;
	DWORD dwClient = pMsg->dwClient;
	
	NET_CHA_DEL* ncd = (NET_CHA_DEL*) pMsg->Buffer;

	dwSndBytes		= sizeof(NET_CHA_BBA_INFO);
	ncbi.nmg.dwSize	= dwSndBytes;
	ncbi.nChaNum	= ncd->nChaNum;

    if (ncd->nChaNum > 0)
	{
		if (COdbcManager::GetInstance()->DelCharacter(m_pClientManager->GetUserNum(dwClient), ncd->nChaNum, ncd->szPass2) == DB_OK) // ��������
			ncbi.nmg.nType = NET_MSG_CHA_DEL_FB_OK;    
		else // ���� ����
			ncbi.nmg.nType = NET_MSG_CHA_DEL_FB_ERROR;
	}	
	SendClient(dwClient, &ncbi);
}

// Ŭ���̾�Ʈ�� ĳ���� ����ȭ�鿡�� ���÷��� �ؾ���
// ĳ������ ������ȣ�� �����Ѵ�.
void CGameServer::MsgSndChaBasicBAInfo(MSG_LIST* pMsg)
{
	DWORD dwSndBytes = 0;
	DWORD dwClient = pMsg->dwClient;
	INT nUsrNum = 0;
	INT nSvrGrp = 0;

	NET_CHA_BBA_INFO ncbi;
	
	dwSndBytes = sizeof(NET_CHA_BBA_INFO);
	ncbi.nmg.dwSize = dwSndBytes;
	ncbi.nmg.nType = NET_MSG_CHA_BAINFO;
	
	nUsrNum = m_pClientManager->GetUserNum(dwClient);
	if (nUsrNum < 1) // ġ���� ����
		return;
	nSvrGrp = m_nServerGroup;
	COdbcManager::GetInstance()->GetChaBAInfo(nUsrNum, nSvrGrp, &ncbi);

	SendClient(dwClient, &ncbi);
}

void CGameServer::MsgSndChaBasicInfo(DWORD dwClient, int nChaNum)
{
	DWORD dwSndBytes = 0;	
	GLMSG::SNETLOBBY_CHARINFO snci;
	COdbcManager::GetInstance()->GetChaBInfo(m_pClientManager->GetUserNum(dwClient),
		                                     nChaNum, 
											 &snci.Data);
	
	SendClient(dwClient, &snci);
}

void CGameServer::MsgSndChaBasicInfo(MSG_LIST* pMsg)
{
	DWORD dwSndBytes = 0;
	DWORD dwClient = pMsg->dwClient;
	INT nChaNum = 0;
	INT nUsrNum = 0;
	INT nSvrGrp = 0;

	NET_CHA_BA_INFO* ncbi;
	ncbi = (NET_CHA_BA_INFO*) pMsg->Buffer;
	nChaNum = ncbi->nChaNum;

	int nUserNum = m_pClientManager->GetUserNum(dwClient);

	GLMSG::SNETLOBBY_CHARINFO snci;
	if ( COdbcManager::GetInstance()->GetChaBInfo(nUserNum, nChaNum, &snci.Data) == DB_ERROR )
	{		
		return;
	}
	
	SendClient(dwClient, &snci);
}

void CGameServer::MsgSndChatGlobal(char* szChatMsg)
{
	DWORD dwSndBytes = 0;
	CConsoleMessage::GetInstance()->Write(szChatMsg);
	NET_CHAT_FB ncf;
	ncf.emType = CHAT_TYPE_GLOBAL;
	StringCchCopyN(ncf.szChatMsg, CHAT_MSG_SIZE, szChatMsg, CHAT_MSG_SIZE);
	SendAllClient(&ncf);
}

void CGameServer::MsgLogIn(MSG_LIST* pMsg)
{	
	NET_LOGIN_DATA* nml;
	DWORD dwFlags = 0;
	DWORD dwSndBytes = 0;
	DWORD dwClient;
	
	nml = (NET_LOGIN_DATA *) pMsg->Buffer;	
	dwClient = pMsg->dwClient;
	
	// ����� id ����
	m_pClientManager->SetUserID(dwClient, nml->szUserid);
	
	// ���Ӽ��� -> ���Ǽ����� id / pwd ����
	SessionSndLoginInfo(nml->szUserid, nml->szPassword, dwClient);
	
	/*
	int nResult;
	nResult = m_pDB->UserCheck(nml->szUserid, 
		nml->szPassword, 
		m_pClientManager->GetClientIP(dwClient), 
		m_nServerGroup,
		m_nServerNum);

	// ������ ����� ������ �����Ѵ�.		
	NET_LOGIN_FEEDBACK_DATA nlfd;
	nlfd.nmg.nType = NET_MSG_LOGIN_FB;
	nlfd.nmg.dwSize = sizeof(NET_LOGIN_FEEDBACK_DATA);
	switch ( nResult )
	{	
	case 4 : // ID/PWD �� ��ġ�ϰ�, IP ������ ���� (�Ϲ������� �α��� ����)
	case 2 : // ID/PWD �� ��ġ�ϰ�, IP �� ��밡��	
			nlfd.nResult = NET_MSG_LOGIN_FB_SUB_OK;		
			m_pClientManager->SetUserNum(dwClient, m_pDB->GetUserNum(nml->szUserid));
			m_pClientManager->SetLoginTime(dwClient);
			break;

	case 0 : // ID / PWD ����ġ (���ο� ������ ���� �Ǵ� ���Է¿䱸)	
		nlfd.nResult = NET_MSG_LOGIN_FB_SUB_INCORRECT;
		break;

	case DB_ERROR : // �ý��ۿ����� �α��� ���� (-1)
		nlfd.nResult = NET_MSG_LOGIN_FB_SUB_SYSTEM;
		break;

	case 3 : // ID/PWD �� ��ġ������ IP �� ���Ұ��� ����(���� ���ܵ� IP)
		nlfd.nResult = NET_MSG_LOGIN_FB_SUB_IP_BAN;
		break;

	default : // �ٸ� ����� �α��� ����
		nlfd.nResult = NET_MSG_LOGIN_FB_SUB_FAIL;
		break;
	}
	SendClient(dwClient, (LPVOID) &nlfd);
	*/
}

void CGameServer::MsgVersion(MSG_LIST* pMsg)
{
	DWORD					dwSndBytes = 0;
	NET_CLIENT_VERSION		ncv;

	DWORD dwClient			= pMsg->dwClient;	
	ncv.nmg.nType			= NET_MSG_VERSION_INFO;
	ncv.nGameProgramVer		= m_nVersion;
	ncv.nPatchProgramVer	= m_nPatchVersion;
	
	SendClient(dwClient, &ncv);
}

// ���ο� ĳ���͸� �����Ѵ�.
void CGameServer::MsgCreateCharacter(MSG_LIST* pMsg)
{
	DWORD			dwClient;
	NET_NEW_CHA*	nnc;
	NET_NEW_CHA_FB	nncfd;	
	DWORD			dwSndBytes = 0;
	int				nUsrNum;
	int				nIndex;	
	int				nSvrGrp;
	int				nChaNum;

	dwClient = pMsg->dwClient;	
	nnc = (NET_NEW_CHA*) pMsg->Buffer;

	// ĳ���� �ε���
	nIndex = nnc->nIndex;
	if (nIndex < 0 || nIndex >= GLCI_NUM )	return;

	// ����ڹ�ȣ, ĳ���� ��, ���� �׷�
	nUsrNum = m_pClientManager->GetUserNum(dwClient);	 	
	nSvrGrp = m_nServerGroup;

	// ĳ���͸�
	CString strChaName(nnc->szChaName);
	strChaName.Trim(_T(" ")); // �յ� ��������

	if ((strChaName.FindOneOf(" ") != -1) || (strChaName.GetLength() < 2)) // �����̽� ã��, ĳ�����̸� 2���� ���� ����
	{	 	
		nChaNum = -1; // ĳ�����̸� ����
	}
	else
	{
		// ĳ���� �ʱ� ��ġ ����.
		GLCHARLOGIC NewCharLogic;
		NewCharLogic.INIT_NEW_CHAR ( (EMCHARINDEX)nIndex, (DWORD)nUsrNum, (DWORD)nSvrGrp, strChaName );

		// ĳ���� ����
		nChaNum = COdbcManager::GetInstance()->CreateNewCharacter(&NewCharLogic);
		dwSndBytes = sizeof (NET_NEW_CHA_FB);		
		nncfd.nmg.dwSize = (USHORT) dwSndBytes;	
		nncfd.nmg.nType = NET_MSG_CHA_NEW_FB;
		nncfd.nChaNum = 0;
	}
	
	if (nChaNum > 0) // ĳ���� ���� ����
	{
		nncfd.nChaNum = nChaNum;
		nncfd.nResult = EM_CHA_NEW_FB_SUB_OK;
	}	
	else if (nChaNum == DB_ERROR) // ĳ���� ����(DB) �����߻�...
	{
		nncfd.nResult  = EM_CHA_NEW_FB_SUB_ERROR;
	}
	else if (nChaNum == DB_CHA_MAX) // ����� �ִ� ĳ���� ���� �ʰ�
	{
		nncfd.nResult  = EM_CHA_NEW_FB_SUB_MAX;
	}
	else if (nChaNum == DB_CHA_DUF) // ���� �̸��� ���� ĳ���� ����
	{
		nncfd.nResult  = EM_CHA_NEW_FB_SUB_DUP;
	}
	else // �˼� ���� ����
	{
		nncfd.nResult  = EM_CHA_NEW_FB_SUB_ERROR;
	}

	SendClient(dwClient, &nncfd);
	
	if ( nChaNum > 0 )
	{
		MsgSndChaBasicInfo(dwClient, nChaNum);
	}
}

// ����� ������ DB ���� �о�ͼ� g_Client�� �����Ѵ�.
void CGameServer::SetUserInfo(DWORD dwClient, const char* strUserId, const char* strPassword)
{

}

// ����� ������ Ŭ���̾�Ʈ�� �����Ѵ�.
void CGameServer::MsgSndUserInfo(MSG_LIST* pMsg)
{
	//LPPER_IO_OPERATION_DATA pIoWrite;
	//DWORD Flags = 0;
	//DWORD dwSndBytes = 0;
	//NET_USER_INFO* nui;
	//
	//int nClient = pMsg->nClient;

	//pIoWrite = (LPPER_IO_OPERATION_DATA) m_pIOCP->GetFreeOverIO(NET_SEND_POSTED);		

	//nui = (NET_USER_INFO*) pIoWrite->Buffer;
	//pIoWrite->DataBuf.buf = pIoWrite->Buffer;
	//
	//dwSndBytes = sizeof(NET_USER_INFO);
	//nui->nmg.dwSize = (USHORT) dwSndBytes;
	//nui->nmg.nType = NET_MSG_USER_INFO;

	//EnterCriticalSection(&m_CriticalSection);
	//memcpy(&nui->uid, &m_pClientManager->m_pClient[nClient].uid, sizeof(NET_USER_INFO));
	//LeaveCriticalSection(&m_CriticalSection);
	//pIoWrite->dwTotalBytes = dwSndBytes;
	//pIoWrite->dwSndBytes = 0;
	//Send(nClient, pIoWrite, dwSndBytes);
}

void CGameServer::MsgSndPingAnswer(MSG_LIST* pMsg)
{	
	NET_MSG_PING*	nmpr;	
	NET_MSG_PING	nmpa;	
	DWORD			dwSndBytes = 0;
	DWORD			dwClient = pMsg->dwClient;

	nmpr = (NET_MSG_PING*) pMsg->Buffer;
	
	nmpa.nmg.dwSize	= sizeof(NET_MSG_PING);	
	nmpa.nmg.nType	= NET_MSG_PING_ANSWER;
	nmpa.stime		= nmpr->stime;
	SendClient(dwClient, &nmpa);
}

///////////////////////////////////////////////////////////////////////////////
// Session Server Communication Member Function
// If not use Session Server, this functions are unnecessary
///////////////////////////////////////////////////////////////////////////////
int	CGameServer::SessionConnect(const char* cServerIP, int nPort)
{
	hostent* hst;	
	char strRet[MAX_IP_LENGTH+1] = {0};
	unsigned long ulIP;		
	struct in_addr inetAddr;

	hst = gethostbyname(cServerIP);		
	if ( hst == NULL ) // ERROR
	{		
		return NET_ERROR;
	}	
	ulIP = *(DWORD*)(*hst->h_addr_list);
	inetAddr.s_addr = ulIP;
	StringCchCopyN(strRet, MAX_IP_LENGTH, inet_ntoa(inetAddr), MAX_IP_LENGTH);
	return SessionConnect(inet_addr(strRet), nPort);
}

int CGameServer::SessionConnect(unsigned long cServerIP, int dwPort)
{
	int nRetCode;

	m_sSession = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if ( m_sSession == INVALID_SOCKET )
	{
		return NET_ERROR;
	}
	
	// ////////////////////////////////////////////////////////////////////////////
	// Set the socket option
    // Disable send buffering on the socket.  Setting SO_SNDBUF 
    // to 0 causes winsock to stop bufferring sends and perform 
    // sends directly from our buffers, thereby reducing CPU usage. 
	// Winsock �� ���۸��� ���̰� cpu ��뷮�� ���̷��� �Ʒ� �ɼ��� Ȱ���� ��Ű�� ��.
	// Set SO_SNDBUF to 0
	// Set SO_RCVBUF to 0
	// ///////////////////////////////////////////////////////////////////////////
	
	// ///////////////////////////////////////////////////////////////////////////
    /*
	int nZero = 0; 
	::setsockopt(m_sSession, SOL_SOCKET, SO_SNDBUF, (char *) &nZero, sizeof(nZero)); 
	::setsockopt(m_sSession, SOL_SOCKET, SO_RCVBUF, (char *) &nZero, sizeof(nZero)); 
	*/
	// ///////////////////////////////////////////////////////////////////////////
	
	ZeroMemory(&m_Addr, sizeof(SOCKADDR_IN));
	m_Addr.sin_family = AF_INET;
	m_Addr.sin_port = htons(dwPort);
	m_Addr.sin_addr.s_addr = cServerIP;

	// Connect to server
	nRetCode = connect(m_sSession, (sockaddr *)&m_Addr, sizeof(SOCKADDR_IN));
	if ( nRetCode == SOCKET_ERROR )
	{
		closesocket(m_sSession);
		m_sSession = INVALID_SOCKET;
		return NET_ERROR;
	}
	
	m_pClientManager->SetAcceptedClient(NET_RESERVED_SESSION, m_sSession);

	// Associate the accepted socket with the completion port
	
	LPPER_IO_OPERATION_DATA IOData;
	DWORD dwRcvBytes;
	DWORD Flags = 0;
	HANDLE hRetCode = NULL;	

	hRetCode = CreateIoCompletionPort((HANDLE)m_sSession, 
									m_hIOServer, 
									(DWORD) NET_RESERVED_SESSION,
									0);

	if ( hRetCode == NULL ) 
	{
		CConsoleMessage::GetInstance()->Write(C_MSG_FILE_CONSOLE, "CreateIoCompletionPort Error");
		closesocket(m_sSession);
		return NET_ERROR;
	}

	// Start processing I/O on ther accepted socket
	// First WSARecv, TCP/IP Send 8 bytes (ignored byte)
	// Client ���� ������ �޴´�.
	dwRcvBytes = sizeof(NET_MSG_GENERIC);

	IOData = (LPPER_IO_OPERATION_DATA) GetFreeOverIO(NET_RECV_POSTED);

	if (IOData == NULL) return NET_ERROR;

	// IOData->DataBuf.buf = IOData->Buffer;
	IOData->dwRcvBytes		= 0;
	IOData->dwTotalBytes	= dwRcvBytes;
	// Head receive mode
	m_pClientManager->SetNetMode(NET_RESERVED_SESSION, NET_PACKET_HEAD);

	WSARecv(m_sSession,
		&(IOData->DataBuf), 
		1,
		&dwRcvBytes,
		&Flags ,
		&(IOData->Overlapped),
		NULL);
	return NET_OK;	
}

int CGameServer::SessionCloseConnect(void)
{
	int nRetCode;
	if ( m_sSession != INVALID_SOCKET )
	{
		nRetCode = closesocket(m_sSession);
		m_sSession = INVALID_SOCKET;
		if ( nRetCode == SOCKET_ERROR )
			return NET_ERROR;		
	}
	return NET_OK;
}

void CGameServer::ServerMsgProcess(MSG_LIST* pMsg)
{
	DWORD dwClient;
	dwClient = pMsg->dwClient;
	switch ( dwClient )
	{
	case NET_RESERVED_SESSION :
		SessionMsgProcess(pMsg);
		break;
	default :
		break;
	}
}

// Session Server message process
void CGameServer::SessionProcess(LPARAM lParam)
{
	//switch (LOWORD(lParam))
	//{
	//case FD_CONNECT: // Connection established
	//	CConsoleMessage::GetInstance()->Write(C_MSG_CONSOLE, "Session Server Connection Succeeded");
	//	break;
	//case FD_CLOSE: // Connection close
	//	SessionCloseConnect();
	//	CConsoleMessage::GetInstance()->Write(C_MSG_CONSOLE, "Session Server Connection Closed");
	//	break;
	//case FD_WRITE: // Send Data
	//	break;
	//case FD_READ: // Receive Data
	//	SessionMsgProcess();	
	//	break;
	//}
}

// Session Server message process
void CGameServer::SessionMsgProcess(MSG_LIST* pMsg)
{	
	NET_MSG_GENERIC* nmg;

	nmg = (NET_MSG_GENERIC*) pMsg->Buffer;
	switch ( nmg->nType )
	{
	// �θ޽���
	case NET_MSG_PING_ANSWER :	
		SessionMsgPingAnswer(nmg);
		break;
	case NET_MSG_REQ_FULL_SVR_INFO :	
		SessionSndSvrInfo();
		break;
	case NET_MSG_REQ_CUR_STATE :
		SessionSndSvrCurState();
		break;		
	case NET_MSG_LOGIN_FB :
		SessionMsgLoginFeedBack(nmg);
		break;
	case NET_MSG_CHAT :
		SessionMsgChatProcess(nmg);		
		break;
	default:
		break;
	}
}

void CGameServer::SendSession(LPVOID pBuffer)
{
	m_pClientManager->SendClient2(NET_RESERVED_SESSION, pBuffer);
	// SendClient(NET_RESERVED_SESSION, pBuffer);
}

// ���Ǽ������� ���� ä�ø޽��� ó��
void CGameServer::SessionMsgChatProcess(NET_MSG_GENERIC* nmg)
{
	NET_CHAT* nc;
	nc = (NET_CHAT*) nmg;

	switch (nc->emType) 
	{
	case CHAT_TYPE_GLOBAL: // ��ü ���� �޽���
		CConsoleMessage::GetInstance()->Write("��ü���� �޽����� �����ϴ�");
		MsgSndChatGlobal(nc->szChatMsg);
		break;
	case CHAT_TYPE_NORMAL:
		break;
	case CHAT_TYPE_PARTY:
		break;
	case CHAT_TYPE_PRIVATE:
		break;
	case CHAT_TYPE_GUILD:
		break;
	case CHAT_TYPE_CTRL_GLOBAL:
		break;
	default:
		break;
	}	
}

// ���Ǽ��� -> ���Ӽ��� : �α��ΰ�� ����Ÿ
void CGameServer::SessionMsgLoginFeedBack(NET_MSG_GENERIC* nmg)
{	
	NET_LOGIN_FEEDBACK_DATA		nlfd;
	NET_LOGIN_FEEDBACK_DATA2*	nlfd2;
	
	DWORD	dwClient;	
	DWORD	dwSndBytes = 0;
	CHAR	szUserid[USR_ID_LENGTH+1] = {0};
	INT		nUserNum = 0;

	// �������� �Ѿ�� �α��� ��� �޽���
	nlfd2 = (NET_LOGIN_FEEDBACK_DATA2*) nmg;
	dwClient = (DWORD) nlfd2->nClient;
	
	// �α��� ��û�� Ŭ���̾�Ʈ�� �������� �˻��Ѵ�.
	dwSndBytes = sizeof(NET_LOGIN_FEEDBACK_DATA2);
	nlfd.nmg.dwSize = dwSndBytes;
	nlfd.nmg.nType = NET_MSG_LOGIN_FB;
	
	// �¶����̰�, ip, id �� �����Ҷ�...
	if ((m_pClientManager->IsOnline(dwClient) == true) &&
		(strcmp(m_pClientManager->GetClientIP(dwClient), nlfd2->szIp) == 0) &&
		(strcmp(m_pClientManager->GetUserID(dwClient), nlfd2->szUserid) == 0))
	{
		nlfd.nResult = nlfd2->nResult;
		// �α����� ���������� �Ǿ����� ����� ��ȣ�� �����Ѵ�.
		if (nlfd2->nResult == EM_LOGIN_FB_SUB_OK)
		{
			StringCchCopyN(szUserid, USR_ID_LENGTH, m_pClientManager->GetUserID(dwClient), USR_ID_LENGTH);  			
			if (nlfd2->nUserNum < 1)
			{
				CConsoleMessage::GetInstance()->Write(C_MSG_CONSOLE, "SessionMsgLoginFeedBack GetUserNum Failed");
				return ;
			}
			m_pClientManager->SetUserNum(dwClient, nlfd2->nUserNum);
			m_pClientManager->SetLoginTime(dwClient);			
		}
	}
	else
	{
		nlfd.nResult = EM_LOGIN_FB_SUB_FAIL;
	}
	// �α��� ��� ����	
	SendClient(dwClient, &nlfd);
}

void CGameServer::SessionMsgPingAnswer(NET_MSG_GENERIC* nmg)
{
	NET_MSG_PING* nmp;
	nmp = (NET_MSG_PING*)nmg;
	int etime;
	etime = (int)((timeGetTime() - nmp->stime));
	CConsoleMessage::GetInstance()->Write(C_MSG_CONSOLE, "Ping Time %d", etime);
}

//CHAR* CGameServer::SessionRecv(void)
//{
//	memset(m_pRecvBuffer, '\0', NET_DATA_BUFSIZE);
//	recv(m_sSession, m_pRecvBuffer, NET_DATA_BUFSIZE, 0);
//	return m_pRecvBuffer;
//}

// Session Server �� ping ��û�� �Ѵ�.
void CGameServer::SessionSndPing(void)
{
	// int nSize;
	NET_MSG_PING nmp;	
	// nSize = sizeof(NET_MSG_PING);
	// memset(&nmp, 0, nSize);
	// nmp.nmg.dwSize	= nSize;
	nmp.nmg.nType	= NET_MSG_PING_REQUEST;
	nmp.stime		= timeGetTime();
	SendSession(&nmp);
}

// Game Server -> Session Server 
// Send server info to session server
void CGameServer::SessionSndSvrInfo(void)
{
	int nSize;
	NET_SERVER_INFO nsi;
	
	nSize = sizeof(NET_SERVER_INFO);
	::memset(&nsi, 0, nSize);
	
	nsi.nmg.dwSize					= nSize;
	nsi.nmg.nType					= NET_MSG_SND_FULL_SVR_INFO;
	nsi.gsi.nControlPort			= CCfg::GetInstance()->GetControlPort();
	nsi.gsi.nServerCurrentClient	= m_pClientManager->GetCurrentClientNumber();
	nsi.gsi.nServerGroup			= CCfg::GetInstance()->GetServerGroup();
	nsi.gsi.nServerMaxClient		= CCfg::GetInstance()->GetServerMaxClient();
	nsi.gsi.nServerNumber			= CCfg::GetInstance()->GetServerNumber();
	nsi.gsi.nServerChannel          = CCfg::GetInstance()->GetServerChannel();
	nsi.gsi.nServerType				= CCfg::GetInstance()->GetServerType();
	nsi.gsi.nServicePort			= CCfg::GetInstance()->GetServicePort();	
	// ::memcpy(nsi.gsi.szServerIP, m_szAddress, MAX_IP_LENGTH);
	StringCbCopy(nsi.gsi.szServerIP, MAX_IP_LENGTH+1, m_szAddress);
	
	SendSession(&nsi);

	CConsoleMessage::GetInstance()->Write(C_MSG_CONSOLE, "CGameServer::SessionSndSvrInfo %s", m_szAddress);
}

// Game Server -> Session Server
void CGameServer::SessionSndSvrCurState()
{
	int nSize;
	NET_SERVER_CUR_INFO nsci;

	nSize = sizeof(NET_SERVER_CUR_INFO);
	
	// �α��μ���, ���Ӽ��� -> ���Ǽ��� : ���� ��������(������)�� �����Ѵ�.
	// ����������, �ִ�������
	nsci.nmg.dwSize					= nSize;
	nsci.nmg.nType					= NET_MSG_SND_CUR_STATE;	
	nsci.gsci.nServerCurrentClient	= m_pClientManager->GetCurrentClientNumber();
	nsci.gsci.nServerMaxClient		= CCfg::GetInstance()->GetServerMaxClient();
	
	SendSession(&nsci);
}

// Game Server -> Session Server : ����� ���� ����
// ����� ������ ��ٸ���.
int CGameServer::SessionSndLoginInfo(const char* szUsrID, const char* szUsrPass, int nClient)
{
	NET_LOGIN_DATA2 nld2;
	int nSize;
	nSize = sizeof(NET_LOGIN_DATA2);
	memset(&nld2, 0, nSize);
	nld2.nmg.dwSize	  = nSize;
	nld2.nmg.nType	  = NET_MSG_LOGIN;
	nld2.nClient	  = nClient;
	StringCchCopyN(nld2.szUserid,	USR_ID_LENGTH,	 szUsrID,							     USR_ID_LENGTH);
	StringCchCopyN(nld2.szPassword, USR_PASS_LENGTH, szUsrPass,							     USR_PASS_LENGTH);
	StringCchCopyN(nld2.szIp,		MAX_IP_LENGTH,	 m_pClientManager->GetClientIP(nClient), MAX_IP_LENGTH);
	SendSession(&nld2);

	// ����� ���� ����
	m_pClientManager->SetUserID(nClient, nld2.szUserid);	

	CConsoleMessage::GetInstance()->Write(C_MSG_CONSOLE, 
				"SessionSndLoginInfo %s(%s)",
				nld2.szUserid,
				nld2.szIp);


	return 0;
}

void CGameServer::SessionSndLogOutInfoSimple(const char* szUsrID)
{
	int nSize;
	NET_LOGOUT_DATA_SIMPLE    nlods;
	nSize			= sizeof(NET_LOGOUT_DATA_SIMPLE);	
	nlods.nmg.dwSize	= nSize;
	nlods.nmg.nType	= NET_MSG_LOGOUT_G_S_S;	
	StringCchCopyN(nlods.szUserID, USR_ID_LENGTH, szUsrID, USR_ID_LENGTH);

	SendSession(&nlods);
}

void CGameServer::SessionSndLogOutInfo(const char* szUsrID, int nUserNum, int nGameTime)
{	
	int nSize;
	NET_LOGOUT_DATA    nlod;
	nSize			= sizeof(NET_LOGOUT_DATA);	
	nlod.nmg.dwSize	= nSize;
	nlod.nmg.nType	= NET_MSG_LOGOUT_G_S;	
	nlod.nUserNum   = nUserNum;
	nlod.nGameTime  = nGameTime;
	StringCchCopyN(nlod.szUserID, USR_ID_LENGTH, szUsrID, USR_ID_LENGTH);

	SendSession(&nlod);
}

int CGameServer::ExecuteCommand(char* strCmd)
{
	// ��ū���� �и��� �и���.
	// space
	char seps[]   = " ";
	char *token;
//	char strTemp[100];

	// Establish string and get the first token
	token = strtok(strCmd, seps);
	while (token != NULL)
	{
		// Request Ping Info
		// command : ping session
		if (strcmp(token, "ping") == 0) 
		{
			// Ping ��û
			token = strtok(NULL, seps);
			if (token) 
			{
				// ���Ǽ����� �ο�û
				if (strcmp(token, "session") == 0) 
				{
					SessionSndPing();
				}			
				return 0;
			}
			else
			{
				return 0;
			}
		}
		// ���� ä�� �޽���, global
		if (strcmp(token, "chat") == 0)
		{
			token = strtok(NULL, seps);
			if (token)
			{
				MsgSndChatGlobal(token);
				return 0;
			}
			else
			{
				return 0;
			}
		}

		// Get next token
		token = strtok(NULL, seps);
	}	
	return 0;
}

//                        ,     
//                   ,   /^\     ___
//                  /^\_/   `...'  /`
//               ,__\    ,'     ~ (
//            ,___\ ,,    .,       \
//             \___ \\\ .'.'   .-.  )
//               .'.-\\\`.`.  '.-. (
//              / (==== ."".  ( o ) \
//            ,/u  `~~~'|  /   `-'   )
//           "")^u ^u^|~| `""".  ~_ /
//             /^u ^u ^\~\     ".  \\
//     _      /u^  u ^u  ~\      ". \\
//    ( \     )^ ^U ^U ^U\~\      ". \\
//   (_ (\   /^U ^ ^U ^U  ~|       ". `\
//  (_  _ \  )U ^ U^ ^U ^|~|        ". `\.
// (_  = _(\ \^ U ^U ^ U^ ~|          ".`.;
//(_ -(    _\_)U ^ ^ U^ ^|~|            ""
//(_    =   ( ^ U^ U^ ^ U ~|
//(_ -  ( ~  = ^ U ^U U ^|~/
// (_  =     (_^U^ ^ U^ U /
//  (_-   ~_(/ \^ U^ ^U^,"
//   (_ =  _/   |^ u^u."  
//    (_  (/    |u^ u.(   
//     (__/     )^u^ u/ 
//             /u^ u^(  
//            |^ u^ u/   
//            |u^ u^(       ____   
//            |^u^ u(    .-'    `-,
//             \^u ^ \  / ' .---.  \
//              \^ u^u\ |  '  `  ;  |
//               \u^u^u:` . `-'  ;  |
//                `-.^ u`._   _.'^'./
//                   "-.^.-```_=~._/
//                      `"------"'
// jgkim