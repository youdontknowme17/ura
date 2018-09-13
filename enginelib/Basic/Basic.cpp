#define STRICT
#include "stdafx.h"
//#include <vld.h> // vld.h ��������� stdafx.h ������ ��ġ�ؾ� �մϴ�.

#include "SUBPATH.h"
#include "StringUtils.h"
#include "../EngineUILib/GUInterface/Cursor.h"
#include "DxGlobalStage.h"
#include "RANPARAM.h"
#include "dxparamset.h"
#include <assert.h>
#include "Basic.h"
#include "BasicWnd.h"
#include "gassert.h"
#include "getdxver.h"
#include "HShield.h"
#include "HSUpChk.h"
#pragma comment(lib,"HShield.lib")
#pragma comment(lib,"HSUpChk.lib")
#include "DaumGameParameter.h"
#include "../RanClientUILib/Interface/GameTextControl.h"
#include "../MfcExLib/MinBugTrap.h"
#include "crc32.h"
#include <afxdisp.h>
#include "accctrl.h"
#include "aclapi.h"
#include <shellapi.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include "Protection.h"
#include <tlhelp32.h>
#include <windows.h>
#include <stdlib.h>
#include <shellapi.h>
#include <stdio.h>
#include <time.h>
#define Exe_Name PROCESSENTRY32 *pe32 = GetProcessInfo ("minia.exe") 
#define Threads (pe32->cntThreads)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CBasicApp
// hackshield
TCHAR szTitle[500] = ("Ran Online");
TCHAR	g_szHShieldPath[MAX_PATH] = {0,};
TCHAR	g_szIniPath[MAX_PATH] = {0,};
DO_RTC_CATCH _rtc_catch;

// Nprotect ����
#if defined(MY_PARAM) || defined(MYE_PARAM)
	CNPGameLib npgl("RanOnlineMY");
#elif defined(PH_PARAM)
	CNPGameLib npgl("RanOnlinePH");
//#elif defined(TW_PARAM)
//	CNPGameLib npgl("RanMinTW");
//#elif defined( VN_PARAM ) 
//	CNPGameLib npgl("RanOnlineVN");
//#elif defined(TH_PARAM)
//	CNPGameLib npgl("RanOnlineTH");
#endif

BEGIN_MESSAGE_MAP(CBasicApp, CWinApp)
	//{{AFX_MSG_MAP(CBasicApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBasicApp construction

CBasicApp::CBasicApp() :
	m_bIsActive(TRUE),
	m_bRedrawScreen(FALSE),
	m_bShutDown(FALSE),
	bUpdate(FALSE),
	bSecure(FALSE),
	bStart(FALSE),
	bInitialize(FALSE),
	bMonitoring(FALSE)

{
	//	Note : ���� �����Լ� ȣ�� ���� �ڵ鷯.
	//
	//_set_purecall_handler(smtm_PurecallHandler);
	
	BUG_TRAP::BugTrapInstall( std::string(_T("amazon.exe")));

#ifdef CH_PARAM
	RANPARAM::emSERVICE_TYPE = EMSERVICE_CHINA;
#endif

#ifdef TH_PARAM
	RANPARAM::emSERVICE_TYPE = EMSERVICE_THAILAND;
#endif

#ifdef MY_PARAM
	RANPARAM::emSERVICE_TYPE = EMSERVICE_MALAYSIA_CN;
#endif

#ifdef MYE_PARAM
	RANPARAM::emSERVICE_TYPE = EMSERVICE_MALAYSIA_EN;
#endif

#ifdef ID_PARAM
	RANPARAM::emSERVICE_TYPE = EMSERVICE_INDONESIA;
#endif

#ifdef PH_PARAM
	//RANPARAM::bScrWndHalfSize = FALSE;
	RANPARAM::emSERVICE_TYPE = EMSERVICE_PHILIPPINES;
#endif

#ifdef VN_PARAM
	RANPARAM::emSERVICE_TYPE = EMSERVICE_VIETNAM;
#endif

#ifdef JP_PARAM
	RANPARAM::emSERVICE_TYPE = EMSERVICE_JAPAN;
#endif

#ifdef TW_PARAM
	RANPARAM::emSERVICE_TYPE = EMSERVICE_FEYA;
#endif

#ifdef HK_PARAM
	RANPARAM::emSERVICE_TYPE = EMSERVICE_FEYA;
#endif

#if defined(KR_PARAM) || defined(KRT_PARAM) 
	RANPARAM::emSERVICE_TYPE = EMSERVICE_KOREA;
#endif

#if defined(GS_PARAM)
	RANPARAM::emSERVICE_TYPE = EMSERVICE_GLOBAL;
#endif

	SecureZeroMemory( m_szAppPath, sizeof(m_szAppPath) );
}

CBasicApp::~CBasicApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CBasicApp object

CBasicApp theApp;

/////////////////////////////////////////////////////////////////////////////
const char* EhSvcDummy = "HackShield/Update/call/EhSvc.dll";
unsigned long EhSvcDummyCrc = 0x7a0560b1;

const char* EhSvc00 = "HackShield/EhSvc.dll";
unsigned long EhSvc00Crc = 0xA4277095;

const char* EhSvc01 = "Data/skin/skin/EhSvc.dll";
unsigned long EhSvc01Crc = 0xA4277095;


BOOL CBasicApp::InitInstance()
{	
  const char* fDummy = "HackShield/Update/call/EhSvc.dll";
  ifstream find( fDummy );


  if ( !find   )
   {
        MessageBox ( NULL,"Some Game Files are missing or corrupted. Re-Install the Game!", "Error", MB_OK ); 
		return FALSE;
   }

	CCRC32 MyCRC32;
	MyCRC32.Initialize();
	unsigned long ulCRC1 = MyCRC32.FileCRC(EhSvcDummy);
	unsigned long ulCRC2 = MyCRC32.FileCRC(EhSvc00);
	unsigned long ulCRC3 = MyCRC32.FileCRC(EhSvc01);
	if( ulCRC1 != EhSvcDummyCrc || ulCRC2 != EhSvc00Crc  || ulCRC3 != EhSvc01Crc  )
	{
		MessageBox ( NULL, "Client file modified! Please Re-Install the Game again", "Error", MB_OK );
		return FALSE;
	}
  

   const char szUniqueNamedMutex[] = "MiniA.exe";
   HANDLE hHandle = CreateMutex( NULL, TRUE, szUniqueNamedMutex );
   if( ERROR_ALREADY_EXISTS == GetLastError() )
   {
      MessageBox ( NULL,"Game is already executed, Multiple Execution is prohibited!", "Game.exe", MB_OK );
      return FALSE; // Exit program
   }


    HS_InitializeService( TRUE );
	HS_StartService( TRUE );
	
    // if initialization error or start  cancel operation
    if ( bStart || bInitialize ) return FALSE;
	AfxEnableControlContainer();

	SetAppPath();
	RANPARAM::LOAD ( m_szAppPath );
	DXPARAMSET::INIT ();


	CString StrCmdLine = m_lpCmdLine;

	STRUTIL::ClearSeparator ();
	STRUTIL::RegisterSeparator ( "/" );
	STRUTIL::RegisterSeparator ( " " );

	BOOL bAPM = FALSE;
	//CString strCommandOld = "apm";
	CString strCommand = CString("app") + "_" + "run";

	CStringArray strCmdArray;
	STRUTIL::StringSeparate ( StrCmdLine, strCmdArray );
	
	for ( int i=0; i<strCmdArray.GetCount(); i++ )
	{
		CString strTemp = strCmdArray.GetAt(i);

		if ( strTemp == strCommand )
		{
			bAPM = TRUE;
		}

	#if defined(TW_PARAM) || defined(HK_PARAM)
		if ( strTemp == _T("ranonlineglobal") )
		{
			RANPARAM::bScrWndHalfSize = TRUE;
			RANPARAM::bScrWndFullSize = FALSE;
		}
	#else
		if ( strTemp == _T("windowed") )
		{
			RANPARAM::bScrWndHalfSize = TRUE;
			RANPARAM::bScrWndFullSize = FALSE;
		}
	#endif

		CString strVALUE;
		int nPos(-1);
		CString strKEY;

		strKEY = "TID:";
		nPos = strTemp.Find( strKEY );
		if ( nPos!=-1 )
		{
			strVALUE = strTemp.Right ( strTemp.GetLength()-strKEY.GetLength() );

			//	�� ����.
			RANPARAM::SETIDPARAM ( strVALUE.GetString() );
			
			//	userid�� ���Ƿ� ����. ( ���� �ɼ� �����. )
			RANPARAM::SETUSERID ( "userid" );
		}


		strKEY = "U=";
		nPos = strTemp.Find( strKEY );
		if ( nPos!=-1 )
		{
			strVALUE = strTemp.Right ( strTemp.GetLength()-strKEY.GetLength() );

			//	�� ����.
			RANPARAM::SETIDPARAM ( strVALUE.GetString() );
			
			//	userid�� ���Ƿ� ����. ( ���� �ɼ� �����. )
			RANPARAM::SETUSERID ( "userid" );
		}

		strKEY = "UUID=";
		nPos = strTemp.Find( strKEY );
		if ( nPos!=-1 )
		{
			strVALUE = strTemp.Right ( strTemp.GetLength()-strKEY.GetLength() );

			//	�� ����.
			RANPARAM::SETIDPARAM ( strVALUE.GetString() );

			//	userid�� ���Ƿ� ����. ( ���� �ɼ� �����. )
			RANPARAM::SETUSERID ( "userid" );
		}

		strKEY = "T=";
		nPos = strTemp.Find( strKEY );
		if ( nPos!=-1 )
		{
			strVALUE = strTemp.Right ( strTemp.GetLength()-strKEY.GetLength() );

			//	�� ����.
			RANPARAM::SETIDPARAM2 ( strVALUE.GetString() );
		}

		strKEY = "MD=";
		nPos = strTemp.Find( strKEY );
		if ( nPos!=-1 )
		{
			strVALUE = strTemp.Right ( strTemp.GetLength()-strKEY.GetLength() );

			//	�� ����.
			RANPARAM::SETIDPARAM3 ( strVALUE.GetString() );
		}
	}

	//	Note : ���α����̰� cmd line param ���� ���޵Ǵ� ���� ���� ���.
	//		���� ��Ȯ�ϰ� ����Ǿ� �ִ��� üũ.
	if( RANPARAM::emSERVICE_TYPE==EMSERVICE_MALAYSIA_CN || 
		RANPARAM::emSERVICE_TYPE==EMSERVICE_MALAYSIA_EN ||
//		RANPARAM::emSERVICE_TYPE==EMSERVICE_JAPAN ||	// JAPAN �α��� ��� ����
		RANPARAM::emSERVICE_TYPE==EMSERVICE_PHILIPPINES ||
		RANPARAM::emSERVICE_TYPE==EMSERVICE_VIETNAM )
	{
		if ( !RANPARAM::VALIDIDPARAM() )
		{
			MessageBox ( NULL, "Invalid web account.", "error", MB_OK );
			return FALSE;
		}
	}

	#if defined(NDEBUG)
	if ( !bAPM )
	{
		MessageBox ( NULL, "This Exe don't execute independent.", "error", MB_OK );
		return FALSE;
	}
	#endif // !NDEBUG

    DWORD dwDirectXVersion = 0;
    TCHAR strDirectXVersion[10];
	HRESULT hr = getdxversion ( &dwDirectXVersion, strDirectXVersion, 10 );

	if ( hr==S_OK && dwDirectXVersion<0x00090003 )
	{
		MessageBox ( NULL, "DirectX Version too old. must install DirectX 9.0c", "error", MB_OK );
		return FALSE;
	}

	{	// Note : �������̽� ��ų �ؽ�Ʈ �ε�
		TCHAR szFullPath[MAX_PATH] = {0};

		StringCchCopy( szFullPath, MAX_PATH, ((CBasicApp*)AfxGetApp())->m_szAppPath );
		StringCchCat( szFullPath, MAX_PATH, SUBPATH::GUI_FILE_ROOT);
		CGameTextMan::GetInstance().SetPath(szFullPath);

		CGameTextMan::GetInstance().LoadText(RANPARAM::strGameWord.GetString(), CGameTextMan::EM_GAME_WORD, RANPARAM::bXML_USE );
		CGameTextMan::GetInstance().LoadText(RANPARAM::strGameInText.GetString(), CGameTextMan::EM_GAME_IN_TEXT, RANPARAM::bXML_USE );
		CGameTextMan::GetInstance().LoadText(RANPARAM::strGameExText.GetString(), CGameTextMan::EM_GAME_EX_TEXT, RANPARAM::bXML_USE );
	}

#if defined(MY_PARAM) || defined(MYE_PARAM) || defined(PH_PARAM) //|| defined( VN_PARAM ) || defined(TH_PARAM) || defined ( HK_PARAM ) || defined(TW_PARAM) // Nprotect ����
	{
		DWORD dwResult = npgl.Init();
		if( dwResult != NPGAMEMON_SUCCESS )
		{
			ErrorMsgByNP( dwResult );
			return FALSE;
		}
	}
#endif



    HS_InitializeService2( TRUE );
	HS_StartService2( TRUE );
	HINSTANCE hGetProcIDDLL = LoadLibrary("HackShield/Update/call/EhSvc.dll");
	if ( !hGetProcIDDLL )
	{
		MessageBox ( NULL, "Some files are missing. please re-install the game!", "error", MB_OK );
		return FALSE;
	}
	CBasicWnd* pWnd = new CBasicWnd();
	if ( pWnd->Create() == FALSE )	return FALSE;

	m_pWnd = pWnd;

	return TRUE;
}


void CBasicApp::SetAppPath()
{
	CString strAppPath;

	TCHAR szPath[MAX_PATH] = {0};
	GetModuleFileName(::AfxGetInstanceHandle(), szPath, MAX_PATH);
	strAppPath = szPath;

	if ( !strAppPath.IsEmpty() )
	{
		DWORD dwFind = strAppPath.ReverseFind ( '\\' );
		if ( dwFind != -1 )
		{
			m_strAppPath = strAppPath.Left ( dwFind );
			
			if ( !m_strAppPath.IsEmpty() )
			if ( m_strAppPath.GetAt(0) == '"' )
				m_strAppPath = m_strAppPath.Right ( m_strAppPath.GetLength()-1 );

			StringCchCopy ( m_szAppPath, MAX_PATH, m_strAppPath.GetString() );
		}
	}
	else 
	{
		MessageBox ( NULL, "SetAppPath Error", "Error", MB_OK );
		return;
	}
}

/////////////////////////////////////////////////////////////////////////////

int CBasicApp::ExitInstance() 
{
	m_pWnd = NULL;

	return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////

int CBasicApp::Run() 
{
	MSG msg;
	BOOL bGotMsg;

	PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE );

	CProtection& cProtection = CProtection::GetInstance();

	cProtection.Init();

	while( WM_QUIT != msg.message )
	{

#if defined(NDEBUG)	
		// software breakpoint check
		if (cProtection.Pesudo_IsDebuggerPresent())	
		{
			m_pWnd->PostMessage ( WM_CLOSE, 0, 0 );
		}

		// hardware breakpoint check
		GetThreadContext(GetCurrentThread(), &cProtection.CONTEXTDebug);	
		if ( cProtection.CONTEXTDebug.Dr0 || cProtection.CONTEXTDebug.Dr1 ||
			 cProtection.CONTEXTDebug.Dr2 || cProtection.CONTEXTDebug.Dr3 )	
		{
			m_pWnd->PostMessage ( WM_CLOSE, 0, 0 );
		}
#endif


		if ( m_bShutDown == TRUE || m_pWnd == NULL )
		{
			ExitInstance ();
			return 0;
		}
		
		
		if( m_bIsActive )
			bGotMsg = PeekMessage( &msg, NULL, 0, 0, PM_REMOVE );
		else
			bGotMsg = GetMessage( &msg, NULL, 0, 0 );

		if( bGotMsg )
		{
			TranslateMessage (&msg);
			DispatchMessage (&msg);

			if ( !m_bIsActive && !m_bShutDown )
			{
				if ( FAILED ( m_pWnd->FrameMove3DEnvironment() ) )
					m_pWnd->PostMessage ( WM_CLOSE, 0, 0 );
			}
		}
		else
		{
			if ( m_bIsActive )
			{
				if ( FAILED ( m_pWnd->Render3DEnvironment() ) )
					m_pWnd->PostMessage ( WM_CLOSE, 0, 0 );
			}
		}		
	}

	ExitInstance ();
	return static_cast<int>(msg.wParam);


}

void CBasicApp::SetActive ( BOOL bActive )
{
	m_bIsActive = bActive;
	m_bRedrawScreen = bActive;
}

void CBasicApp::ErrorMsgByNP( DWORD dwResult )
{
	TCHAR msg[256]={0};
	LPCSTR lpszMsg;

	// ��6. �ֿ信���ڵ塯�� �����Ͽ� ��Ȳ�� �´� �޽����� ������ݴϴ�.
	switch (dwResult)
	{
	case NPGAMEMON_ERROR_EXIST:
		lpszMsg = ID2GAMEEXTEXT( "NPGAMEMON_ERROR_EXIST" );
		break;
	case NPGAMEMON_ERROR_GAME_EXIST:
		lpszMsg = ID2GAMEEXTEXT( "NPGAMEMON_ERROR_GAME_EXIST" );
		break;
	case NPGAMEMON_ERROR_INIT:
		lpszMsg = ID2GAMEEXTEXT( "NPGAMEMON_ERROR_INIT" );
		break;
	case NPGAMEMON_ERROR_AUTH_GAMEGUARD:
	case NPGAMEMON_ERROR_NFOUND_GG:
	case NPGAMEMON_ERROR_AUTH_INI:
	case NPGAMEMON_ERROR_NFOUND_INI:
		lpszMsg = ID2GAMEEXTEXT( "NPGAMEMON_ERROR_AUTH_GAMEGUARD" );
		break;
	case NPGAMEMON_ERROR_CRYPTOAPI:
		lpszMsg = ID2GAMEEXTEXT( "NPGAMEMON_ERROR_CRYPTOAPI" );
		break;
	case NPGAMEMON_ERROR_EXECUTE:
		lpszMsg = ID2GAMEEXTEXT( "NPGAMEMON_ERROR_EXECUTE" );
		break;
	case NPGAMEMON_ERROR_ILLEGAL_PRG:
		lpszMsg = ID2GAMEEXTEXT( "NPGAMEMON_ERROR_ILLEGAL_PRG" );
		break;
	case NPGMUP_ERROR_ABORT:
		lpszMsg = ID2GAMEEXTEXT( "NPGMUP_ERROR_ABORT" );
		break;
	case NPGMUP_ERROR_CONNECT:
	case NPGMUP_ERROR_DOWNCFG:
		lpszMsg = ID2GAMEEXTEXT( "NPGMUP_ERROR_CONNECT" );
		break;
	case NPGMUP_ERROR_AUTH:
		lpszMsg = ID2GAMEEXTEXT( "NPGMUP_ERROR_AUTH" );
		break;
	case NPGAMEMON_ERROR_NPSCAN:
		lpszMsg = ID2GAMEEXTEXT( "NPGAMEMON_ERROR_NPSCAN" );
		break;
	default:
		// Display relevant messages.
		lpszMsg = ID2GAMEEXTEXT( "NPGAMEMON_ERROR_DEFAULT" );
		break;
	}

	wsprintf(msg, _T("GameGuard Execute Error : %lu"), dwResult);
	MessageBox(NULL, lpszMsg, msg, MB_OK);
}
int __stdcall AhnHS_Callback(long lCode, long lParamSize, void* pParam)
{
	TCHAR msg[128]={0};
	LPCTSTR lpszMsg( NULL );
	bool bAppExit( false );
	bool bSecured ( false );
	switch(lCode)
	{
		//Engine Callback
		case AHNHS_ENGINE_DETECT_GAME_HACK:
		{
			bSecured = true;
			if ( bSecured ) _stprintf(msg, _T("ehsvc.dll Engine Captured. original ehsvc.dll Secured!") );
			break;
		}
		case AHNHS_ENGINE_DETECT_WINDOWED_HACK:
		{
			break;
		}
		case AHNHS_ACTAPC_DETECT_SPEEDHACK:
		{
			break;
		}	
		case AHNHS_ACTAPC_DETECT_KDTRACE:	
		case AHNHS_ACTAPC_DETECT_KDTRACE_CHANGED:
		{
			bAppExit = true;
			_stprintf(msg, _T("AHNHS_ACTAPC_DETECT_KDTRACE_CHANGED"), lCode);
            DxGlobalStage::GetInstance().CloseGame( msg );
			break;
		}
		
		case AHNHS_ACTAPC_DETECT_AUTOMACRO:
		{
			bAppExit = true;
			_stprintf(msg, _T("HackShield Auto Use Key/Auto Pots Detected"), lCode);
            DxGlobalStage::GetInstance().CloseGame( msg );

			break;
		}

		case AHNHS_ACTAPC_DETECT_ABNORMAL_FUNCTION_CALL:
		{
			bAppExit = true;
			_stprintf(msg, _T("Detect Abnormal Memory Access\n%s"), (char*)pParam);
            DxGlobalStage::GetInstance().CloseGame( msg );
			break;
		}
		case AHNHS_ACTAPC_DETECT_ABNORMAL_MEMORY_ACCESS:
		{
			bAppExit = true;
			_stprintf(msg, _T("Detect Memory Access\n%s"), (char*)pParam);
            DxGlobalStage::GetInstance().CloseGame( msg );
			break;
		}

		
		case AHNHS_ACTAPC_DETECT_AUTOMOUSE:
		 {
			bAppExit = true;
			_stprintf(msg, _T("HackShield DETECT_AUTOMOUSE."), lCode);
            DxGlobalStage::GetInstance().CloseGame( msg );
			break;
			}
		case AHNHS_ACTAPC_DETECT_DRIVERFAILED:
			{
			bAppExit = true;
			_stprintf(msg, _T("HackShield DETECT_DRIVERFAILED."), lCode);
            DxGlobalStage::GetInstance().CloseGame( msg );
			break;
			}
		case AHNHS_ACTAPC_DETECT_HOOKFUNCTION:
			{
			break;
			}
		case AHNHS_ACTAPC_DETECT_MESSAGEHOOK:
			{
			bAppExit = true;
			_stprintf(msg, _T("HackShield DETECT_MESSAGEHOOK."), lCode);
            DxGlobalStage::GetInstance().CloseGame( msg );
			break;
			}
		case AHNHS_ACTAPC_DETECT_MODULE_CHANGE:
			{
			bAppExit = true;
			_stprintf(msg, _T("HackShield DETECT_MODULE_CHANGE."), lCode);
            DxGlobalStage::GetInstance().CloseGame( msg );
			break;
			}
		case AHNHS_ACTAPC_DETECT_ENGINEFAILED:
			{
			bAppExit = true;
			_stprintf(msg, _T("HackShield DETECT_ENGINEFAILED."), lCode);
            DxGlobalStage::GetInstance().CloseGame( msg );
			break;
			}
		case AHNHS_ACTAPC_DETECT_CODEMISMATCH:
			{
             _stprintf(msg, _T("HackShield Monitoring Secured! :P") );
			break;
			}
					case AHNHS_ACTAPC_DETECT_MEM_MODIFY_FROM_LMP:
					case AHNHS_ACTAPC_DETECT_LMP_FAILED:
			{
			bAppExit = true;
			_stprintf(msg, _T("HackShield Detect memory modify."), lCode);
            DxGlobalStage::GetInstance().CloseGame( msg );
			break;
			}
            case AHNHS_ACTAPC_DETECT_ABNORMAL_HACKSHIELD_STATUS:
			{
			bAppExit = true;
			if (lCode != HS_ERR_ALREADY_GAME_STARTED)
			{
			bAppExit = true;
			_stprintf(msg, _T("HackShield Service already started by other game"), lCode);
            DxGlobalStage::GetInstance().CloseGame( msg );
			break;
			}
			else
            {
		    bAppExit = true;
			_stprintf(msg, _T("HackShield Service Error"), lCode);
            DxGlobalStage::GetInstance().CloseGame( msg );
			break;
			}
			}
		case AHNHS_ACTAPC_DETECT_PROTECTSCREENFAILED:
		{
			bAppExit = true;
			_stprintf(msg, _T("HackShield PROTECTSCREENFAILED."), lCode);
            DxGlobalStage::GetInstance().CloseGame( msg );
			break;
		}

	}
	return 1;
}
BOOL CBasicApp::HS_StartService( BOOL bMonitor )
{
 //bMonitor = bMonitoring;
 if ( bMonitor )
 {
	TCHAR msg[128]={0};
	int		nRet;
    
	nRet = _AhnHS_StartService();
	// upon start up call you can enable or disable dual game execution
	// msg when enabled.
	// if not ignore this message and ask your player
	// to run minia.exe on administrator previlage if this error shows up
	// you can disable or enable this on Initialization proccess
	/*if ( nRet != HS_ERR_OK )
	{
		if ( HS_ERR_ALREADY_GAME_STARTED )
		{
			_stprintf( msg, _T("The Game is already Started.Dual Login Detected!(%x)"), nRet ); 
		    DxGlobalStage::GetInstance().CloseGame( msg );
			bStart = TRUE;
			return FALSE;
		}
	}*/
	//==============================================================
	if (nRet != HS_ERR_OK)
     { 
		 switch ( nRet )
		{ 
         case HS_ERR_START_ENGINE_FAILED: 
         case HS_ERR_DRV_FILE_CREATE_FAILED:
         case HS_ERR_REG_DRV_FILE_FAILED:
         case HS_ERR_START_DRV_FAILED:
         default: 
			 _stprintf( msg, _T("A problem occurred in the hack prevention feature.(%x)"), nRet ); 
		     DxGlobalStage::GetInstance().CloseGame( msg );
		 break; 
		} 
		bStart = TRUE;
		DxGlobalStage::GetInstance().CloseGame( msg );
		return FALSE; 
	 }

  }
   return TRUE;
}
BOOL CBasicApp::HS_InitializeService( BOOL bMonitor )
{
 //bMonitor = bMonitoring;
 if ( bMonitor )
 {
	TCHAR	*pEnd = NULL;
	TCHAR	szFullFileName[MAX_PATH] = { 0, };
	TCHAR msg[128]={0};
	LPCTSTR lpszMsg( NULL );
	int		nRet;
    TCHAR szFullFilePath[MAX_PATH]={0,};

	LoadString(NULL, 103, szTitle, 500);
	GetModuleFileName(NULL, szFullFileName, MAX_PATH);
	pEnd = _tcsrchr( szFullFileName, _T('\\')) + 1;
	if (!pEnd)
	{
		return FALSE;	
	}
	*pEnd = _T('\0');

	_stprintf(g_szIniPath, _T("%s"), szFullFileName);				
	//_stprintf(g_szHShieldPath, _T("%s\\hsupdate"), szFullFileName);
	//_tcscat(szFullFileName, _T("\\hsupdate\\EhSvc.dll"));	
	_stprintf(g_szHShieldPath, _T("%s\\HackShield"), szFullFileName);
	_tcscat(szFullFileName, _T("\\HackShield\\EhSvc.dll"));	


	    nRet = _AhnHS_Initialize(szFullFileName, AhnHS_Callback, 
		1234,
		"81E365022A1CE03B", 
		AHNHS_CHKOPT_ALL
		//|AHNHS_CHKOPT_STANDALONE // disable this if you want to enable dual client lol!
		|AHNHS_CHKOPT_LOADLIBRARY
		|AHNHS_CHKOPT_PROTECT_D3DX
		|AHNHS_CHKOPT_LOCAL_MEMORY_PROTECTION
		|AHNHS_ALLOW_SVCHOST_OPENPROCESS
		|AHNHS_ALLOW_LSASS_OPENPROCESS
		|AHNHS_ALLOW_CSRSS_OPENPROCESS
		|AHNHS_DONOT_TERMINATE_PROCESS
		|AHNHS_CHKOPT_PROTECTSCREEN,
		AHNHS_SPEEDHACK_SENSING_RATIO_HIGHEST);

	if (nRet != HS_ERR_OK)
     { 
		 switch ( nRet )
		{ 
         case HS_ERR_COMPATIBILITY_MODE_RUNNING:
         case HS_ERR_NEED_ADMIN_RIGHTS:
         case HS_ERR_INVALID_FILES: 
         case HS_ERR_INIT_DRV_FAILED: 
		 case HS_ERR_DEBUGGER_DETECT: 
         case HS_ERR_NOT_INITIALIZED:
         default: 
			 _stprintf( msg, _T("A problem occurred in the hack prevention feature.(%x)"), nRet ); 
			 DxGlobalStage::GetInstance().CloseGame( msg );
			 bInitialize = TRUE;
		 break; 
		} 
		bInitialize = TRUE;
		DxGlobalStage::GetInstance().CloseGame( msg );
		return FALSE; 
	 } 
 }
    return TRUE;
}
//2nd Call
int __stdcall AhnHS_Callback2(long lCode, long lParamSize, void* pParam)
{
	TCHAR msg[128]={0};
	LPCTSTR lpszMsg( NULL );
	bool bAppExit( false );
	bool bSecured ( false );
	switch(lCode)
	{
		//Engine Callback
		case AHNHS_ENGINE_DETECT_GAME_HACK:
		{
			bSecured = true;
			if ( bSecured ) _stprintf(msg, _T("ehsvc.dll Engine Captured. original ehsvc.dll Secured!") );
			break;
		}
		case AHNHS_ENGINE_DETECT_WINDOWED_HACK:
		{
			//ShowMsgAndExitWithTimer(_T("Windowed Hack found."));

			break;
		}
		case AHNHS_ACTAPC_DETECT_SPEEDHACK:
		{
			break;
		}

		
		case AHNHS_ACTAPC_DETECT_KDTRACE:	
		case AHNHS_ACTAPC_DETECT_KDTRACE_CHANGED:
		{
			bAppExit = true;
			_stprintf(msg, _T("AHNHS_ACTAPC_DETECT_KDTRACE_CHANGED"), lCode);
            DxGlobalStage::GetInstance().CloseGame( msg );
			break;
		}
		
		case AHNHS_ACTAPC_DETECT_AUTOMACRO:
		{
			bAppExit = true;
			_stprintf(msg, _T("HackShield Auto Use Key/Auto Pots Detected"), lCode);
            DxGlobalStage::GetInstance().CloseGame( msg );

			break;
		}

		case AHNHS_ACTAPC_DETECT_ABNORMAL_FUNCTION_CALL:
		{
			bAppExit = true;
			_stprintf(msg, _T("Detect Abnormal Memory Access\n%s"), (char*)pParam);
            DxGlobalStage::GetInstance().CloseGame( msg );
			break;
		}
		case AHNHS_ACTAPC_DETECT_ABNORMAL_MEMORY_ACCESS:
		{
			bAppExit = true;
			_stprintf(msg, _T("Detect Memory Access\n%s"), (char*)pParam);
            DxGlobalStage::GetInstance().CloseGame( msg );
			break;
		}

		
		case AHNHS_ACTAPC_DETECT_AUTOMOUSE:
		 {
			bAppExit = true;
			_stprintf(msg, _T("HackShield DETECT_AUTOMOUSE."), lCode);
            DxGlobalStage::GetInstance().CloseGame( msg );
			break;
			}
		case AHNHS_ACTAPC_DETECT_DRIVERFAILED:
			{
			bAppExit = true;
			_stprintf(msg, _T("HackShield DETECT_DRIVERFAILED."), lCode);
            DxGlobalStage::GetInstance().CloseGame( msg );
			break;
			}
		case AHNHS_ACTAPC_DETECT_HOOKFUNCTION:
			{
			break;
			}
		case AHNHS_ACTAPC_DETECT_MESSAGEHOOK:
			{
			bAppExit = true;
			_stprintf(msg, _T("HackShield DETECT_MESSAGEHOOK."), lCode);
            DxGlobalStage::GetInstance().CloseGame( msg );
			break;
			}
		case AHNHS_ACTAPC_DETECT_MODULE_CHANGE:
			{
			bAppExit = true;
			_stprintf(msg, _T("HackShield DETECT_MODULE_CHANGE."), lCode);
            DxGlobalStage::GetInstance().CloseGame( msg );
			break;
			}
		case AHNHS_ACTAPC_DETECT_ENGINEFAILED:
			{
			bAppExit = true;
			_stprintf(msg, _T("HackShield DETECT_ENGINEFAILED."), lCode);
            DxGlobalStage::GetInstance().CloseGame( msg );
			break;
			}
		case AHNHS_ACTAPC_DETECT_CODEMISMATCH:
			{
             _stprintf(msg, _T("HackShield Monitoring Secured! :P") );
			break;
			}
					case AHNHS_ACTAPC_DETECT_MEM_MODIFY_FROM_LMP:
					case AHNHS_ACTAPC_DETECT_LMP_FAILED:
			{
			bAppExit = true;
			_stprintf(msg, _T("HackShield Detect memory modify."), lCode);
            DxGlobalStage::GetInstance().CloseGame( msg );
			break;
			}
            case AHNHS_ACTAPC_DETECT_ABNORMAL_HACKSHIELD_STATUS:
			{
			bAppExit = true;
			if (lCode != HS_ERR_ALREADY_GAME_STARTED)
			{
			bAppExit = true;
			_stprintf(msg, _T("HackShield Service already started by other game"), lCode);
            DxGlobalStage::GetInstance().CloseGame( msg );
			break;
			}
			else
            {
		    bAppExit = true;
			_stprintf(msg, _T("HackShield Service Error"), lCode);
            DxGlobalStage::GetInstance().CloseGame( msg );
			break;
			}
			}
		case AHNHS_ACTAPC_DETECT_PROTECTSCREENFAILED:
		{
			bAppExit = true;
			_stprintf(msg, _T("HackShield PROTECTSCREENFAILED."), lCode);
            DxGlobalStage::GetInstance().CloseGame( msg );
			break;
		}

	}
	return 1;
}
BOOL CBasicApp::HS_StartService2( BOOL bMonitor )
{
 //bMonitor = bMonitoring;
 if ( bMonitor )
 {
	TCHAR msg[128]={0};
	int		nRet;
    
	nRet = _AhnHS_StartService();
	
	if (nRet != HS_ERR_OK)
     { 
		 switch ( nRet )
		{ 
         case HS_ERR_START_ENGINE_FAILED: 
         case HS_ERR_DRV_FILE_CREATE_FAILED:
         case HS_ERR_REG_DRV_FILE_FAILED:
         case HS_ERR_START_DRV_FAILED:
         default: 	
			 _stprintf( msg, _T("A problem occurred in the hack prevention feature.(%x)"), nRet ); 
		     DxGlobalStage::GetInstance().CloseGame( msg );
		 break; 
		}
		DxGlobalStage::GetInstance().CloseGame( msg );
		return FALSE; 
	 }
 }
   return TRUE;
}
BOOL CBasicApp::HS_InitializeService2( BOOL bMonitor )
{
 //bMonitor = bMonitoring;
 if ( bMonitor )
 {
	TCHAR	*pEnd = NULL;
	TCHAR	szFullFileName[MAX_PATH] = { 0, };
	TCHAR msg[128]={0};
	LPCTSTR lpszMsg( NULL );
	int		nRet;
    TCHAR szFullFilePath[MAX_PATH]={0,};
//	DWORD dwRet;

	//g_dwMainThreadID = GetCurrentThreadId();
	LoadString(NULL, 103, szTitle, 500);
	GetModuleFileName(NULL, szFullFileName, MAX_PATH);
	pEnd = _tcsrchr( szFullFileName, _T('\\')) + 1;
	if (!pEnd)
	{
		return FALSE;	
	}
	*pEnd = _T('\0');

	_stprintf(g_szIniPath, _T("%s"), szFullFileName);				
	_stprintf(g_szHShieldPath, _T("%s\\data\\skin\\skin"), szFullFileName);
	_tcscat(szFullFileName, _T("\\data\\skin\\skin\\EhSvc.dll"));	


	    nRet = _AhnHS_Initialize(szFullFileName, AhnHS_Callback2, 
		1234,
		"81E365022A1CE03B", 
		AHNHS_CHKOPT_ALL
		//|AHNHS_CHKOPT_STANDALONE // on 2nd caller this standalone initialize should be disabled by default
		|AHNHS_CHKOPT_LOADLIBRARY
		|AHNHS_CHKOPT_PROTECT_D3DX
		|AHNHS_CHKOPT_LOCAL_MEMORY_PROTECTION
		|AHNHS_ALLOW_SVCHOST_OPENPROCESS
		|AHNHS_ALLOW_LSASS_OPENPROCESS
		|AHNHS_ALLOW_CSRSS_OPENPROCESS
		|AHNHS_DONOT_TERMINATE_PROCESS
		|AHNHS_CHKOPT_PROTECTSCREEN,
		AHNHS_SPEEDHACK_SENSING_RATIO_HIGHEST);

	if (nRet != HS_ERR_OK)
     { 
		 switch ( nRet )
		{ 
         case HS_ERR_COMPATIBILITY_MODE_RUNNING:
         case HS_ERR_NEED_ADMIN_RIGHTS:
         case HS_ERR_INVALID_FILES: 
         case HS_ERR_INIT_DRV_FAILED: 
		 case HS_ERR_DEBUGGER_DETECT: 
         case HS_ERR_NOT_INITIALIZED:
         default: 	
			 _stprintf( msg, _T("A problem occurred in the hack prevention feature.(%x)"), nRet ); 
			 DxGlobalStage::GetInstance().CloseGame( msg );
			 break; 
	    } 
		DxGlobalStage::GetInstance().CloseGame( msg );
		return FALSE; 
	 } 
 }
   return TRUE;	
}