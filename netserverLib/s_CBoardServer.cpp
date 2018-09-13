///////////////////////////////////////////////////////////////////////////////
// s_CBoardServer.cpp
//
// class CBoardServer
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
#include "DxServerInstance.h"
#include "s_CBoardServer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBoardServer::CBoardServer(HWND hWnd, HWND hEditBox, HWND hEditBoxInfo) 
		: CServer(hWnd, hEditBox, hEditBoxInfo),
		m_pClientManager (NULL)
{
}

CBoardServer::~CBoardServer()
{
	SAFE_DELETE(m_pClientManager);
}

// Ŭ���̾�Ʈ �������� Ŭ���� ����
int CBoardServer::StartClientManager()
{	
	SAFE_DELETE(m_pClientManager);
	m_pClientManager = new CClientBoard(m_nMaxClient, m_pSendIOCP, m_pRecvIOCP, m_hIOServer);
	if (!m_pClientManager) 
	{
		m_bIsRunning = false;
		return NET_ERROR;
	}
	return NET_OK;
}

int CBoardServer::Start()
{
	int nRetCode;
	m_bIsRunning = true;
	
	if (StartCfg()			== NET_ERROR) return NET_ERROR;	
	if (StartIOCPList()		== NET_ERROR) return NET_ERROR;    	
	if (StartMsgManager()	== NET_ERROR) return NET_ERROR;
	if (StartIOCP()			== NET_ERROR) return NET_ERROR;
	if (StartClientManager()== NET_ERROR) return NET_ERROR;	
	// if (StartDbManager()    == NET_ERROR) return NET_ERROR;
	if (StartWorkThread()	== NET_ERROR) return NET_ERROR;
	if (StartUpdateThread() == NET_ERROR) return NET_ERROR;
	if (StartListenThread() == NET_ERROR) return NET_ERROR;	
	
	//////////////////////////////////////////////////////////////////////////////
	// Connect Session Server
	nRetCode = SessionConnect(CCfg::GetInstance()->GetSessionServerIP(), CCfg::GetInstance()->GetSessionServerPort());
	if (nRetCode != NET_OK)
	{
		CConsoleMessage::GetInstance()->Write(C_MSG_CONSOLE, "ERROR:Session Server Connection Failed");	
		CConsoleMessage::GetInstance()->Write(C_MSG_CONSOLE, "Check Session server or IP address");	
		m_bIsRunning = false;
		return NET_ERROR;
	}
	CConsoleMessage::GetInstance()->Write(C_MSG_CONSOLE, "Session Server Connection OK");

	// Send Server info to Session Server
	SessionSndSvrInfo();	

	CConsoleMessage::GetInstance()->Write(C_MSG_CONSOLE, "=======================================================");	
	CConsoleMessage::GetInstance()->Write(C_MSG_CONSOLE, "Server Start OK");	
	CConsoleMessage::GetInstance()->Write(C_MSG_CONSOLE, "=======================================================");

	m_nStatus = S_SERVER_RUNING;

	return NET_OK;
}

int CBoardServer::Stop()
{
	// Stop All Thread and exit
	DWORD dwExitCode = 0;

	CConsoleMessage::GetInstance()->Write(C_MSG_CONSOLE, "=========== Please wait until server stop =============");
	CConsoleMessage::GetInstance()->Write(C_MSG_CONSOLE, "=========== UpdateThread ���Ḧ ��ٸ��� �ֽ��ϴ�.");

	LockOn();
	m_bIsRunning = false;
	LockOff();

	while ( m_hUpdateThread )	{	::Sleep(0); }
	CConsoleMessage::GetInstance()->Write(C_MSG_CONSOLE, "=========== UpdateThread�� ����Ǿ����ϴ�.");

	StopListenThread();
	StopIOCPList();
	StopWorkThread();
	StopIOCP();

	CloseAllClient();

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
	// SAFE_DELETE(m_pDB);
	// CConsoleMessage::GetInstance()->Write(C_MSG_CONSOLE, "Database Manager closed OK");
	// CFG class ����
	CCfg::GetInstance()->ReleaseInstance();

	// Put message to console
	CConsoleMessage::GetInstance()->Write(C_MSG_CONSOLE, "=======================================================");	
	CConsoleMessage::GetInstance()->Write(C_MSG_CONSOLE, "Server Stop OK");	
	CConsoleMessage::GetInstance()->Write(C_MSG_CONSOLE, "=======================================================");

	m_nStatus = S_SERVER_STOP;

	return NET_OK;
}

int	CBoardServer::Pause()
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

int CBoardServer::Resume()
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

int	CBoardServer::ReStart()
{
	Stop();
	Start();
	return NET_OK;
}

int CBoardServer::ListenProc()
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
		Accept = ::WSAAccept(m_sServer, NULL, NULL, NULL, 0);		
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
			::closesocket(Accept);
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
		hRetCode = ::CreateIoCompletionPort((HANDLE) Accept, 
										m_hIOServer, 
										(DWORD) nClientNumber, 
										0);
		if (hRetCode == NULL) 
		{
			CConsoleMessage::GetInstance()->Write(C_MSG_CONSOLE, "CreateIoCompletionPort Error");
			::closesocket(Accept);
			continue;
		} 

		// ���Ǽ����� ���� �����ڼ� ����
		SessionSndSvrCurState();
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
		
		// �ʵ弭�������� ��ȣȭ ����
		// MsgSndCryptKey(nClientNumber); // Send crypt key
				
		::WSARecv(Accept,
			&(IOData->DataBuf), 
			1,
			&dwRecvNumBytes,
			&Flags ,
			&(IOData->Overlapped),
			NULL);
		::Sleep(0);
	}
	// ServerAcceptThread Running End
	CConsoleMessage::GetInstance()->Write(C_MSG_CONSOLE, "ListenProc End");
	return 0;	
}

int CBoardServer::WorkProc()
{
	LPPER_IO_OPERATION_DATA PerIoData;
	DWORD dwSndBytes;
	DWORD dwRcvBytes;
	DWORD dwByteTrans;
	DWORD Flags;
	DWORD dwClient = -1;
	int	  nRetCode=0;
	PerIoData = NULL;
	DWORD dwMaxClient = (DWORD) m_pClientManager->GetMaxClient();
	NET_MSG_GENERIC* pNmg = NULL;

	while (m_bIsRunning)
	{
		/////////////////////////////////////////////////////////////////////////////
		// Wait for I/O to complete on any socket
		nRetCode = ::GetQueuedCompletionStatus(m_hIOServer,
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
					} else {
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
				
				nRetCode = ::WSARecv(m_pClientManager->GetSocket(dwClient),
									&(PerIoData->DataBuf), 
									1, 
									&dwRcvBytes,
									&Flags,
									&(PerIoData->Overlapped),
									NULL);
				if ((nRetCode == SOCKET_ERROR) && (WSAGetLastError() != WSA_IO_PENDING))
				{
					ReleaseOperationData(PerIoData);
					CloseClient(dwClient);
					// ġ���� ����, �α׿� ��ϳ���
					CConsoleMessage::GetInstance()->Write(C_MSG_FILE_CONSOLE, "ERROR:CFieldServer::WorkProc WSARecv %d ERROR", nRetCode);
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
				nRetCode = m_pClientManager->SendClient2(dwClient, PerIoData, dwSndBytes);
				if (nRetCode == NET_ERROR)
				{
					CloseClient(dwClient);
				}
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

int	CBoardServer::UpdateProc()
{
	// ������ ������ ���� ������
	DWORD nTimeS;
	DWORD dwLastMsgTime; 
	float fFrame			= 0;
	float fCount			= 0;	
	float fUpdateSec		= 30000; // FPS ǥ�� ���Žð� 30��
	DWORD dwUsrChkTime		= 1800000; // 30 �и��� �ѹ��� ����� ���Ӳ��� üũ
	DWORD dwUsrChkStart		= timeGetTime(); // �ƹ��͵� ���� �ʴ� ����� ���Ӳ��⸦ ���� �ð�
	DWORD dwUsrSaveTime		= 60000 * 30; // ����� ����ð� 30��
	DWORD dwUsrSaveStart	= timeGetTime(); // ����� ����ð� üũ�� ���� �ð�����

	nTimeS				= timeGetTime();
	DWORD dwN; // ���޻���� ���Ӳ��⸦ ���� ���� ����
	DWORD dwTimeDiff; // ���޻���� �ð�üũ�� ���� ����
	DWORD dwMaxClient;

	MSG_LIST* pMsg = NULL;
	while (m_bIsRunning)
	{
		//	Note : ���� Ŭ���̾�Ʈ ID ����.
		//
		m_pClientManager->ResetPreSleepCID ();

		// ����� ��Žð� ������Ʈ�� ���ؼ� �ð�������
		dwLastMsgTime = ::timeGetTime();
		// Flip Messge Queue
		m_pRecvMsgManager->MsgQueueFlip();	
		// Get Messages and Processing message
		pMsg = m_pRecvMsgManager->GetMsg();		
		while (pMsg != NULL)
		{	
			// �޽����� ���� Ŭ���̾�Ʈ�� �ð� ������Ʈ
			m_pClientManager->SetLastMsgTime(pMsg->dwClient, dwLastMsgTime);
			// �޽��� ó��
			MsgProcess(pMsg);
			pMsg = m_pRecvMsgManager->GetMsg(); // �����޽��� ��������
		}
		
		/////////////////////////////////////////////////////////////////////
		// �ƹ��͵� ���� �ʴ� ����� ���Ӳ���
		if ((dwLastMsgTime - dwUsrChkStart) > dwUsrChkTime)
		{
			CConsoleMessage::GetInstance()->Write("Info : ���޻������ üũ ����");
			dwMaxClient = (DWORD) m_nMaxClient;
			for (dwN = NET_RESERVED_SLOT; dwN < dwMaxClient; dwN++)
			{					
				// �¶��� �����ΰ��
				if (m_pClientManager->IsOnline(dwN))
				{
					dwTimeDiff = dwLastMsgTime - m_pClientManager->GetLastMsgTime(dwN); // ���� �ð� - ���� �޽������� �ð�
					if (dwTimeDiff > dwUsrChkTime) // ����ڰ� ChkTime �̻� �ƹ��� �޽����� ������ �ʾ��� -> ���������Ŵ
					{
						CloseClient(dwN);
					}
				}
			}
			dwUsrChkStart = dwLastMsgTime; // ���� ���Ӳ��� �ð� ������Ʈ
			CConsoleMessage::GetInstance()->Write("Info : ���޻������ üũ �Ϸ�");
		}	

		///////////////////////////////////////////////////////////////////////
		// ������ ����
		if ( (timeGetTime() - nTimeS) >= fUpdateSec )
		{			
			fFrame = ( (float)((fCount * fUpdateSec) /(float)(timeGetTime() - nTimeS)));
			//nFrame = Count; 
			nTimeS = timeGetTime();
			fCount = 0;
			
			/*
			CConsoleMessage::GetInstance()->WriteInfo ( "UpdateProc %d FPS, avg %d msec , Player Char : %d ", 
							(int)(fFrame/((fUpdateSec)/1000)), 
							(int)(fUpdateSec/fFrame), m_pClientManager->GetCurrentClientNumber());

			CConsoleMessage::GetInstance()->WriteInfo ( "Current User %d/%d",							
							(int) m_pClientManager->GetCurrentClientNumber(),
							(int) m_pClientManager->GetMaxClient());

			// ��Ŷ ó���� ���÷���
			CConsoleMessage::GetInstance()->WriteInfo ( "INFO:Packet Inbound Count (%d/sec) Size (%d(bytes)/sec)",
							(int) (m_nPacketInCount/(fUpdateSec/1000)),
							(int) (m_nPacketInSize/(fUpdateSec/1000)));
			*/
			m_pClientManager->resetPacketCount();
		} 
		else
		{
			fCount++;
		}
		::Sleep(0);
	}
	TRACE ( "[UpdateProc End]\n" );
	//	������Ʈ ������ �����.
	m_hUpdateThread = NULL;
	return 0;
}

void CBoardServer::Send(DWORD nClient, LPPER_IO_OPERATION_DATA PerIoData, DWORD dwSize)
{
}

int CBoardServer::SendClient(DWORD dwClient, LPVOID pBuffer)
{
	return m_pClientManager->SendClient(dwClient, pBuffer);
}

void CBoardServer::SendAllClient(LPVOID pBuffer)
{
	DWORD dwMaxClient;
	DWORD dwStartClient;

	dwStartClient = NET_RESERVED_SLOT;
	dwMaxClient = (DWORD) m_nMaxClient;

	CConsoleMessage::GetInstance()->Write("��ü �������� �÷��̾ �޽����� �����ϴ�");

	for (dwStartClient; dwStartClient < dwMaxClient; dwStartClient++ )
	{
		// �������̸� �޽����� ������
		if (m_pClientManager->IsOnline(dwStartClient))
			SendClient(dwStartClient, pBuffer);
	}
}

void CBoardServer::CloseClient(DWORD dwClient)
{
	if ((m_pClientManager->IsOnline(dwClient) == false) || (dwClient < 0))
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
		CConsoleMessage::GetInstance()->Write("����ڰ� ������ �����߽��ϴ�");
		// ���� �������� ����
		SessionSndSvrCurState();
		m_pClientManager->CloseClient(dwClient); // lock
	}
}

void CBoardServer::CloseAllClient()
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

int CBoardServer::MsgProcess(MSG_LIST* pMsg)
{
	if (pMsg == NULL) return NET_ERROR;
	
	DWORD dwClient = 0;
	NET_MSG_GENERIC* nmg = (NET_MSG_GENERIC*) pMsg->Buffer;
	dwClient = pMsg->dwClient;
	
	// ������ ��� �޽���
	if (dwClient >= 0 && dwClient < NET_RESERVED_SLOT)
	{
		ServerMsgProcess(pMsg);
		return 0;
	}

	switch(nmg->nType)
	{
	case NET_MSG_PING_REQUEST : // �ο�û
		MsgSndPingAnswer(pMsg);
		break;

	default:
		break;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// Session Server Communication Member Function
// If not use Session Server, this functions are unnecessary
///////////////////////////////////////////////////////////////////////////////
int	CBoardServer::SessionConnect(const char* cServerIP, int nPort)
{
	hostent* hst;	
	char strRet[MAX_IP_LENGTH+1] = {0};
	unsigned long ulIP;		
	struct in_addr inetAddr;

	hst = ::gethostbyname(cServerIP);		
	if ( hst == NULL ) // ERROR
	{		
		return NET_ERROR;
	}	
	ulIP = *(DWORD*)(*hst->h_addr_list);
	inetAddr.s_addr = ulIP;
	::StringCchCopy(strRet, MAX_IP_LENGTH+1, inet_ntoa(inetAddr));
	return SessionConnect(inet_addr(strRet), nPort);
}

int CBoardServer::SessionConnect(unsigned long cServerIP, int dwPort)
{
	int nRetCode;

	m_sSession = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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
	
	::ZeroMemory(&m_Addr, sizeof(SOCKADDR_IN));
	m_Addr.sin_family = AF_INET;
	m_Addr.sin_port = ::htons((u_short) dwPort);
	m_Addr.sin_addr.s_addr = cServerIP;

	// Connect to server
	nRetCode = ::connect(m_sSession, (sockaddr *)&m_Addr, sizeof(SOCKADDR_IN));
	if ( nRetCode == SOCKET_ERROR )
	{
		::closesocket(m_sSession);
		m_sSession = INVALID_SOCKET;
		return NET_ERROR;
	}
	
	// If type is session, all slots are itself.
	m_pClientManager->SetAcceptedClient (NET_RESERVED_SESSION, m_sSession);
	m_pClientManager->SetSlotType       (NET_RESERVED_SESSION, NET_SLOT_SESSION);
	m_pClientManager->SetSlotFieldAgent (NET_RESERVED_SESSION, NET_RESERVED_SESSION);
	m_pClientManager->SetSlotFieldClient(NET_RESERVED_SESSION, NET_RESERVED_SESSION);

	// Associate the accepted socket with the completion port
	
	LPPER_IO_OPERATION_DATA IOData;
	DWORD dwRcvBytes;
	DWORD Flags = 0;
	HANDLE hRetCode = NULL;	

	hRetCode = ::CreateIoCompletionPort((HANDLE)m_sSession, 
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
	
	IOData->dwRcvBytes		= 0;
	IOData->dwTotalBytes	= dwRcvBytes;
	// Head receive mode
	m_pClientManager->SetNetMode(NET_RESERVED_SESSION, NET_PACKET_HEAD);

	::WSARecv(m_sSession,
		&(IOData->DataBuf), 
		1,
		&dwRcvBytes,
		&Flags ,
		&(IOData->Overlapped),
		NULL);
	return NET_OK;	
}

int CBoardServer::SessionCloseConnect(void)
{
	int nRetCode;
	if ( m_sSession != INVALID_SOCKET )
	{
		nRetCode = ::closesocket(m_sSession);
		m_sSession = INVALID_SOCKET;
		if ( nRetCode == SOCKET_ERROR )
			return NET_ERROR;		
	}
	return NET_OK;
}

void CBoardServer::ServerMsgProcess(MSG_LIST* pMsg)
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
void CBoardServer::SessionMsgProcess(MSG_LIST* pMsg)
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
	case NET_MSG_SVR_CMD:
		SessionMsgSvrCmd(nmg);
		break;
	default:
		break;
	}
}

void CBoardServer::SessionMsgSvrCmd(NET_MSG_GENERIC* nmg)
{
	NET_SVR_CMD_CHANGE_STATE* pTemp = reinterpret_cast <NET_SVR_CMD_CHANGE_STATE*> (nmg);

	CConsoleMessage::GetInstance()->Write("INFO:Received Server Command From Session Server");

	switch (pTemp->nCmdType)
	{
	case NET_MSG_SVR_PAUSE: // Server pause message
		CConsoleMessage::GetInstance()->Write("INFO:Server Will Pause");
		Pause();
		break;
	case NET_MSG_SVR_RESUME: // Server resume message
		CConsoleMessage::GetInstance()->Write("INFO:Server Will Resume");
		Resume();
		break;
	case NET_MSG_SVR_RESTART: // Server restart message
		CConsoleMessage::GetInstance()->Write("INFO:Server Will Restart");
		Stop();
		Start();
		break;
	default:
		break;
	}
}

void CBoardServer::SendSession(LPVOID pBuffer)
{
	m_pClientManager->SendClient2(NET_RESERVED_SESSION, pBuffer);
	// SendClient(NET_RESERVED_SESSION, pBuffer);
}

void CBoardServer::SessionMsgPingAnswer(NET_MSG_GENERIC* nmg)
{
	NET_MSG_PING* nmp;
	nmp = (NET_MSG_PING*)nmg;
	int etime;
	etime = (int)(timeGetTime() - nmp->stime);
	CConsoleMessage::GetInstance()->Write(C_MSG_CONSOLE, "Ping Time %d", etime);
}

// Session Server �� ping ��û�� �Ѵ�.
void CBoardServer::SessionSndPing(void)
{
	// int nSize;
	NET_MSG_PING nmp;	
	nmp.nmg.nType	= NET_MSG_PING_REQUEST;
	nmp.stime		= timeGetTime();
	SendSession(&nmp);
}

// Game Server -> Session Server 
// Send server info to session server
void CBoardServer::SessionSndSvrInfo(void)
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
	nsi.gsi.nServerType				= CCfg::GetInstance()->GetServerType();
	nsi.gsi.nServerChannel          = CCfg::GetInstance()->GetServerChannel();
	nsi.gsi.nServicePort			= CCfg::GetInstance()->GetServicePort();	
	// ::memcpy(nsi.gsi.szServerIP, m_szAddress, MAX_IP_LENGTH);
	::CopyMemory(nsi.gsi.szServerIP, m_szAddress, MAX_IP_LENGTH);
	
	SendSession(&nsi);

	CConsoleMessage::GetInstance()->Write(C_MSG_CONSOLE, "CFieldServer::SessionSndSvrInfo %s", m_szAddress);
}

// Game Server -> Session Server
void CBoardServer::SessionSndSvrCurState()
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

void CBoardServer::MsgSndPingAnswer(MSG_LIST* pMsg)
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

int CBoardServer::ExecuteCommand(char* strCmd)
{
	// ��ū���� �и��� �и���.
	// space
	char seps[]   = " ";
	char *token;

	// Establish string and get the first token
	token = ::strtok(strCmd, seps);
	while (token != NULL)
	{
		// Request Ping Info
		// command : ping session
		if (::strcmp(token, "ping") == 0) 
		{
			// Ping ��û
			token = ::strtok(NULL, seps);
			if (token) 
			{
				// ���Ǽ����� �ο�û
				if (::strcmp(token, "session") == 0) 
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
		// Get next token
		token = ::strtok(NULL, seps);
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
