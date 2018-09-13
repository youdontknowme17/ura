#include "stdafx.h"
#include "Emulator.h"

#include "GLOGIC.h"
#include "DxEffectMan.h"
#include "RANPARAM.h"
#include "DxResponseMan.h"
#include "DxViewPort.h"
#include "DxLightMan.h"
#include "DxSurfaceTex.h"
#include "DxEnvironment.h"
#include "DxGlowMan.h"
#include "DxShadowMap.h"
#include "DxPostProcess.h"
#include "DxInputDevice.h"
#include "profile.h"
#include "dxparamset.h"
#include "EDITMESHS.h"

#include "SUBPATH.h"
#include "GLPeriod.h"
#include "GLChar.h"
#include "GLGaeaServer.h"
#include "DxServerInstance.h"
#include "DxGlobalStage.h"
#include "GLGaeaClient.h"

#include "EmulatorDoc.h"
#include "EmulatorView.h"

#include "CommonWeb.h"
#include "DXInputString.h"
#include "D3DFont.h"
#include "dxincommand.h"

#include "../EngineUILib/GUInterface/Cursor.h"
#include "../EngineLib/DxCommon/DxFontMan.h"
#include "../RanClientUILib/Interface/GameTextControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BOOL AUTO_PLAY = FALSE;	// ���� �ڵ� �ε�

class DxDirectMsgServer : public DxMsgServer
{
public:
	virtual int SendClient ( DWORD dwClient, LPVOID pBuffer )
	{
		if ( GETMYCLIENTID()==dwClient )
			DxGlobalStage::GetInstance().MsgProcess ( reinterpret_cast<NET_MSG_GENERIC*> ( pBuffer ) );
	
		return 0;
	}

	virtual int SendField ( DWORD dwClient, LPVOID pBuffer )
	{
		GASSERT ( 0 && "SendField()" );
		return 0;
	}

	virtual int SendFieldSvr ( int nSvrNum, LPVOID pBuffer, int nServerChannel=0 )
	{
		GASSERT ( 0 && "SendFieldSvr()" );
		return 0;
	}

	virtual int SendAgent ( DWORD dwClient, LPVOID pBuffer )
	{
		NET_MSG_GENERIC *nmg = (NET_MSG_GENERIC *)pBuffer;

		switch ( nmg->nType )
		{
		default:
			if ( GETMYCLIENTID()==dwClient )
			{
				DxGlobalStage::GetInstance().MsgProcess ( reinterpret_cast<NET_MSG_GENERIC*> ( pBuffer ) );
			}
			break;
		};

		return 0;
	}

	virtual int SendAgent ( LPVOID pBuffer )
	{
		NET_MSG_GENERIC *nmg = (NET_MSG_GENERIC *)pBuffer;

		switch ( nmg->nType )
		{
		case NET_MSG_GCTRL_REQ_GATEOUT:
			GLGaeaServer::GetInstance().MsgProcess ( nmg, 0, GAEAID_NULL );
			break;

		default:
			break;
		};

		return 0;
	}

	virtual void SendAllField ( LPVOID pBuffer )
	{
		GASSERT(0&&"SendAllField()");
	}

	virtual void SendAllField	( LPVOID pBuffer, int nServerChannel=0 )
	{
		GASSERT(0&&"SendAllField()");
	}

	virtual void SendChannel    ( LPVOID pBuffer, int nServerChannel=0 )
	{
		GASSERT(0&&"SendAllField()");
	}

	virtual void SendAllChannel ( LPVOID pBuffer )
	{
		GASSERT(0&&"SendAllField()");
	}

public:
	static DxDirectMsgServer& GetInstance();
};

DxDirectMsgServer& DxDirectMsgServer::GetInstance()
{
	static DxDirectMsgServer Instance;
	return Instance;
}

class DxSetConsoleMsg : public DxConsoleMsg
{
public:
	virtual void Write ( int nType, const char* szFormat, ... )
	{
		va_list argList;
		char szBuffer[1024];

		va_start(argList, szFormat);
		StringCbVPrintf(szBuffer, 1024, szFormat, argList);
		va_end(argList);

		CDebugSet::ToListView ( szBuffer );
	}

	virtual void Write( const TCHAR* msg, ... )
	{
		if (msg == NULL) return;

		TCHAR szBuf[C_BUFFER_SIZE+1] = {0};	

		va_list ap;
		va_start(ap, msg);
		StringCchVPrintf(szBuf, C_BUFFER_SIZE+1, msg, ap);
		va_end(ap);	

		CDebugSet::ToListView ( szBuf );
	}

	static DxSetConsoleMsg& GetInstance();
};

DxSetConsoleMsg& DxSetConsoleMsg::GetInstance()
{
	static DxSetConsoleMsg Instance;
	return Instance;
}

void CEmulatorView::OnInitialUpdate()
{
	__super::OnInitialUpdate();

	m_strCharSetFile = "class1.charset";
	m_strMapList = "mapslist.ini";

	if( AUTO_PLAY )
		OnAppPlay();
}

void CEmulatorView::PostNcDestroy()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	CD3DApplication::Cleanup3DEnvironment();

	::SetCursor ( NULL );
	::DestroyCursor ( m_hCursorDefault );

	__super::PostNcDestroy();
}

BOOL CEmulatorView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	HCURSOR hCursl = CCursor::GetInstance().GetCurCursor();
	if ( !hCursl )
	{
		if ( !m_hCursorDefault )		m_hCursorDefault = LoadCursor ( NULL, IDC_ARROW );
		hCursl = m_hCursorDefault;
	}

	if ( hCursl )	::SetCursor ( hCursl );

	return TRUE;
}

HRESULT CEmulatorView::ConfirmDevice ( D3DCAPSQ* pCaps, DWORD dwBehavior, D3DFORMAT Format )
{
	if ( dwBehavior & D3DCREATE_HARDWARE_VERTEXPROCESSING )
		return E_FAIL;

	return S_OK;
}

HRESULT CEmulatorView::OneTimeSceneInit()
{
	HRESULT hr;

	CEmulatorApp *pApp = (CEmulatorApp *) AfxGetApp();

	//	Note : ���� ��ġ��.
	//
	DxResponseMan::GetInstance().OneTimeSceneInit ( pApp->m_szAppPath, this,
		RANPARAM::strFontType, RANPARAM::dwLangSet, RANPARAM::strGDIFont );

	//	Note : ���̾� ���� ����.
	//
	RANPARAM::SETUSERID ( "userid" );

	hr = DxFieldInstance::Create ( pApp->m_szAppPath, &DxDirectMsgServer::GetInstance(), &DxSetConsoleMsg::GetInstance(), NULL, SP_MINCOMS, m_strMapList.GetString(),
								   FIELDSERVER_MAX , 1000, true, 0, FALSE, FALSE, 3);	
	if ( FAILED(hr) )	return E_FAIL;

	//	Note : ���� ��������.
	//
	DxGlobalStage::GetInstance().SetD3DApp(this);
	DxGlobalStage::GetInstance().OneTimeSceneInit ( pApp->m_szAppPath, m_hWndApp, TRUE,
		static_cast<WORD>(RANPARAM::dwScrWidth), static_cast<WORD>(RANPARAM::dwScrHeight),
		m_strMapList.GetString() );

	// �� �Լ� �ҷ��ָ� HelpWindow�� WebBrowser ��ġ �غ��� ( ���� )
	if( m_bEnableGUI )
		CCommonWeb::Get()->Create( (CWnd*)this, &m_bVisibleGUI, &m_rtBoundGUI );

	return S_OK;
}

HRESULT CEmulatorView::CreateObjects()
{
	{ // Note : �ε��� �ؽ�Ʈ�� ����ϱ� ���ؼ� ��Ʈ�� �ؽ�Ʈ �κ��� ���� �ʱ�ȭ �ߴ�.

		//	Note : ��Ʈ �޴��� �ʱ�ȭ.
		DxFontMan::GetInstance().InitDeviceObjects ( m_pd3dDevice );
		CD3DFontPar* pD3dFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );
		CD3DFontPar* pD3dFont10 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 10, _DEFAULT_FONT_FLAG );
		CD3DFontPar* pD3dFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8, D3DFONT_SHADOW|D3DFONT_ASCII );

		//	Note	:	����׼� ��� ���� �� �ʱ�ȭ
		CDebugSet::InitDeviceObjects( pD3dFont10 );

		// Note : �������̽� ��ų �ؽ�Ʈ �ε�
		TCHAR szFullPath[MAX_PATH] = {0};

		StringCchCopy( szFullPath, MAX_PATH, ((CEmulatorApp*)AfxGetApp())->m_szAppPath );
		StringCchCat( szFullPath, MAX_PATH, SUBPATH::GUI_FILE_ROOT);
		CGameTextMan::GetInstance().SetPath(szFullPath);

		CGameTextMan::GetInstance().LoadText(RANPARAM::strGameWord.GetString(), CGameTextMan::EM_GAME_WORD, RANPARAM::bXML_USE );
		CGameTextMan::GetInstance().LoadText(RANPARAM::strGameInText.GetString(), CGameTextMan::EM_GAME_IN_TEXT, RANPARAM::bXML_USE );
		CGameTextMan::GetInstance().LoadText(RANPARAM::strGameExText.GetString(), CGameTextMan::EM_GAME_EX_TEXT, RANPARAM::bXML_USE );
	}

	HRESULT hr = S_OK;

	hr = InitDeviceObjects();
    if( FAILED(hr) )	return hr;

	hr = RestoreDeviceObjects();	
    if( FAILED(hr) )	return hr;

	return hr;
}

HRESULT CEmulatorView::InitDeviceObjects()
{
	{
		SCHARDATA2 CharData2;
		CharData2.Assign ( GLCONST_CHAR::GET_CHAR_DATA2(1,GLCI_ARMS_M) );

		if ( !m_strCharSetFile.IsEmpty() )
		{
			CharData2.LOADFILE ( m_strCharSetFile.GetString() );
		}

		CharData2.m_cInventory.SetItemGenTime();

		if( !AUTO_PLAY )
		{
			if (bGmRights)
			{
				CharData2.m_dwUserLvl = USER_MASTER;
			}
		}

		strcpy ( CharData2.m_szName, "MAC [DEVELOPER]" );

		PGLCHAR pGLChar = GLGaeaServer::GetInstance().CreatePC ( &CharData2, 0, 0, FALSE );
		if ( pGLChar )
		{
			// Ŭ���̾�Ʈ�� ���� : ĳ�������� + ��ų + ������
			pGLChar->MsgGameJoin ();

			DxGlobalStage::GetInstance().MsgProcessFrame ();
		}
		else
		{
			MessageBox ( "Server Activate Charactor Create fail.", "ERROR", MB_OK|MB_ICONEXCLAMATION );
		}
	}

	//	Note : ����Ʈ.
	//
	DxViewPort::GetInstance().InitDeviceObjects ( m_pd3dDevice, m_hWnd );

	//	Note : ���� ��ġ��.
	//
	DxResponseMan::GetInstance().InitDeviceObjects ( m_pd3dDevice );

	DXLIGHT sDirectional;
	sDirectional.SetDefault();
	sDirectional.m_Light.Diffuse = D3DXCOLOR(0,0,0,0);
	sDirectional.m_Light.Ambient = D3DXCOLOR(0,0,0,0);
	DxLightMan::SetDefDirect ( sDirectional );

	// Texture Type Debug ���� ����
	TextureManager::EnableTexTypeDEBUG();

	//	Note : ���� ��������.
	//
	DxGlobalStage::GetInstance().InitDeviceObjects ( m_pd3dDevice );

	//	Note : Ű���� ���콺.
	//
	CCursor::GetInstance().InitDeviceObjects ();

	return S_OK;
}

HRESULT CEmulatorView::RestoreDeviceObjects()
{
	HRESULT hr=S_OK;

	//	Note : ���� ��ġ��.
	//
	DxResponseMan::GetInstance().RestoreDeviceObjects ();

	//	Note : ���� ��������.
	//
	DxGlobalStage::GetInstance().RestoreDeviceObjects ();
	DXPARAMSET::INIT ();

	//	Note : �۷ο� ó���� �ϱ����� ��� �Ѵ�.
	DxGlowMan::GetInstance().SetProjectActiveON();
	DxPostProcess::GetInstance().SetProjectActiveON();

	//	DEBUG : �������ϸ� �ʱ�ȭ.
	PROFILE_INIT();

	return S_OK;
}

HRESULT CEmulatorView::FrameMove3DEnvironment ()
{
	HRESULT hr=S_OK;

	PROFILE_BLOCKSTART();

	if ( m_pd3dDevice )
	{
		hr = CD3DApplication::FrameMove3DEnvironment ();
		if ( FAILED(hr) )	return hr;
	}

	PROFILE_BLOCKEND();
	PROFILE_DUMPOUTPUT();

	return S_OK;
}

HRESULT CEmulatorView::Render3DEnvironment ()
{
	HRESULT hr=S_OK;

	PROFILE_BLOCKSTART();

	if ( m_pd3dDevice )
	{
		hr = CD3DApplication::Render3DEnvironment ();
		if ( FAILED(hr) )	return hr;
	}

	PROFILE_BLOCKEND();
	PROFILE_DUMPOUTPUT();

	return S_OK;
}

HRESULT CEmulatorView::FrameMove()
{
	PROFILE_BEGIN("FrameMove");

	//	float ��Ȯ�� ���
	CheckControlfp();

	GLPeriod::GetInstance().UpdateWeather ( m_fElapsedTime );
	GLGaeaServer::GetInstance().FrameMove ( m_fTime, m_fElapsedTime );

	//	Note : ���� ��ġ��.
	//
	DxResponseMan::GetInstance().FrameMove( m_fTime, m_fElapsedTime, m_bDefWin );

	//	Note : ���� ��������.
	//
	DxGlobalStage::GetInstance().FrameMove ( m_fTime, m_fElapsedTime );

	//	Note : ����Ʈ
	//
	DxViewPort::GetInstance().FrameMove ( m_fTime, m_fElapsedTime );

	PROFILE_END("FrameMove");

	return S_OK;
}

HRESULT CEmulatorView::Render()
{
	PROFILE_BEGIN("Render");

	//	Fog ����
	D3DCOLOR colorClear = D3DCOLOR_XRGB(89,135,179);
	DxFogMan::GetInstance().RenderFogSB ( m_pd3dDevice );
	colorClear = DxFogMan::GetInstance().GetFogColor();

	// Clear the viewport
	HRESULT hr = m_pd3dDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
						colorClear, 1.0f, 0L );

	//// Begin the scene
	if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
	{
			//	Note : Glow 
		DxGlowMan::GetInstance().RenderTex ( m_pd3dDevice );

		//	Note : Light ����.
		//
		DxLightMan::GetInstance()->Render ( m_pd3dDevice );

		//	Note : ���ؽ� ���̴� �ܽ�źƮ ����.
		//
		DXLIGHT &Light = *DxLightMan::GetInstance()->GetDirectLight ();
		m_pd3dDevice->SetVertexShaderConstantF ( VSC_LIGHTDIRECT, (float*)&Light.m_Light.Direction, 1 );
		m_pd3dDevice->SetVertexShaderConstantF ( VSC_LIGHTDIFFUSE, (float*)&Light.m_Light.Diffuse, 1 );
		m_pd3dDevice->SetVertexShaderConstantF ( VSC_LIGHTAMBIENT, (float*)&Light.m_Light.Ambient, 1 );

		D3DXVECTOR3 &vFromPt = DxViewPort::GetInstance().GetFromPt ();
		m_pd3dDevice->SetVertexShaderConstantF ( VSC_CAMERAPOSITION, (float*)&vFromPt, 1 );
		
		//	Note : ����Ʈ ����Ʈ VertexShaderConstant
		//
		D3DLIGHTQ	pLight;
		D3DXVECTOR4	vPointPos;
		D3DXVECTOR3	vPointDiff;
		for ( int i=0; i<7; i++ )
		{
			if ( DxLightMan::GetInstance()->GetClosedLight(i+1) )
			{
				pLight = DxLightMan::GetInstance()->GetClosedLight(i+1)->m_Light;
				vPointDiff = D3DXVECTOR3 ( pLight.Diffuse.r, pLight.Diffuse.g, pLight.Diffuse.b );
				vPointPos.x = pLight.Position.x;
				vPointPos.y = pLight.Position.y;
				vPointPos.z = pLight.Position.z;
				vPointPos.w = pLight.Range;
			}
			else
			{
				vPointPos = D3DXVECTOR4 ( 0.f, 0.f, 0.f, 0.1f );
				vPointDiff = D3DXVECTOR3 ( 0.f, 0.f, 0.f );
			}
			m_pd3dDevice->SetVertexShaderConstantF (i*2+VSC_PLIGHTPOS01, (float*)&vPointPos, 1);
			m_pd3dDevice->SetVertexShaderConstantF (i*2+VSC_PLIGHTDIFF01, (float*)&vPointDiff, 1);
		}

		D3DXMATRIX matView = DxViewPort::GetInstance().GetMatView();
		D3DXMATRIX matProj = DxViewPort::GetInstance().GetMatProj();

		m_pd3dDevice->SetVertexShaderConstantF ( VSC_SKIN_DEFAULT, (float*)&D3DXVECTOR4 (1.f, 0.5f, 0.f, 765.01f), 1 );

		D3DXVECTOR3	vLightVector = DxLightMan::GetInstance()->GetDirectLight()->m_Light.Direction;
		D3DXVec3TransformNormal ( &vLightVector, &vLightVector, &matView );
		D3DXVec3Normalize ( &vLightVector, &vLightVector);
		vLightVector = -vLightVector;
		m_pd3dDevice->SetVertexShaderConstantF ( VSC_LIGHTDIRECT_VIEW, (float*)&vLightVector, 1 );

		D3DXMatrixTranspose( &matView, &matView );
		D3DXMatrixTranspose( &matProj, &matProj );

		D3DXMATRIX matIdentity;
		D3DXMatrixIdentity( &matIdentity );
		m_pd3dDevice->SetTransform( D3DTS_WORLD,  &matIdentity );

		m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,	D3DTOP_MODULATE );		// ���� ����

		//	Note : ���� ��������.
		//
		DxGlobalStage::GetInstance().Render ();

		RenderText();

		//	Note : Ŀ�� �׸���.
		//
		CCursor::GetInstance().Render ( m_pd3dDevice, DxInputDevice::GetInstance().GetMouseLocateX(), DxInputDevice::GetInstance().GetMouseLocateY() );

		// End the scene.
		m_pd3dDevice->EndScene();
	}

	PROFILE_END("Render");
	return S_OK;
}

HRESULT CEmulatorView::RenderText()
{
	if ( !dxincommand::bDISP_CONSOLE && !dxincommand::bDISP_FPS )	return S_OK;

	D3DCOLOR fontColor        = D3DCOLOR_ARGB(255,255,255,255);
	D3DCOLOR fontWarningColor = D3DCOLOR_ARGB(255,0,255,255);
	TCHAR szMsg[MAX_PATH] = TEXT("");

	// Output display stats
	CD3DFontPar* pD3dFont = DxFontMan::GetInstance().FindFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );
	if ( !pD3dFont )	return S_OK;

	if ( dxincommand::bDISP_FPS )
	{
		// Output display stats
		FLOAT fNextLine = 40.0f; 

		lstrcpy( szMsg, m_strDeviceStats );
		fNextLine -= 20.0f;
		pD3dFont->DrawText( 2, fNextLine, fontColor, szMsg );

		lstrcpy( szMsg, m_strFrameStats );
		fNextLine -= 20.0f;
		pD3dFont->DrawText( 2, fNextLine, fontColor, szMsg );
	}

	if ( dxincommand::bDISP_CONSOLE )
	{
		CDebugSet::Render ();
	}

    return S_OK;
}

HRESULT CEmulatorView::InvalidateDeviceObjects()
{
	//	Note : ���� ��ġ��.
	//
	DxResponseMan::GetInstance().InvalidateDeviceObjects ();

	//	Note : ���� ��������.
	//
	DxGlobalStage::GetInstance().InvalidateDeviceObjects ();

	return S_OK;
}

HRESULT CEmulatorView::DeleteDeviceObjects()
{
#ifdef _DEBUG
	_CrtCheckMemory();
#endif //_DEBUG

	//	Note : ���� ��ġ��.
	//
	DxResponseMan::GetInstance().DeleteDeviceObjects ();

	//	Note : ���� ��������.
	//
	DxGlobalStage::GetInstance().DeleteDeviceObjects ();

	//	Note : ���콺 Ŀ��.
	//
	CCursor::GetInstance().DeleteDeviceObjects ();

	return S_OK;
}

HRESULT CEmulatorView::FinalCleanup()
{
	DxFieldInstance::CleanUp ();

	//	Note : ���� ��������.
	//
	DxGlobalStage::GetInstance().FinalCleanup ();

	//	Note : ���� ��ġ��.
	//
	DxResponseMan::GetInstance().FinalCleanup ();

	return S_OK;
}

void CEmulatorView::SetActive ( BOOL bActive )
{
	DxInputDevice::GetInstance().OnActivate ( bActive );
}

void CEmulatorView::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if ( m_bCreated )
	{
		ReSizeWindow(cx,cy);

		if ( m_pd3dDevice )
		{
			DxGlobalStage::GetInstance().ReSizeWindow ( cx, cy );
		}
	}
}

void CEmulatorView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	CEmulatorApp *pApp = (CEmulatorApp*) AfxGetApp();
	pApp->SetActive ( bActivate );
	CEmulatorView::SetActive ( bActivate );

	__super::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CEmulatorView::OnAppPlay()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CEmulatorApp *pApp = (CEmulatorApp *) AfxGetApp();

	char szFullPath[MAX_PATH] = {0};
	strcpy ( szFullPath, pApp->m_szAppPath );
	strcat ( szFullPath, SUBPATH::GLOGIC_FILE );
	GLOGIC::SetPath ( szFullPath );

	if( !AUTO_PLAY )
	{
		{
			//	Note : ���� ���͸� �����Ѵ�.
			CString szFilter = "charset file  |*.charset|";

			//	Note : ���� ���� ���̾˷α׸� ����.
			CFileDialog dlg ( TRUE,".charset",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );

			dlg.m_ofn.lpstrInitialDir = GLOGIC::GetPath ();
			if ( dlg.DoModal() == IDOK )
			{
				m_strCharSetFile = dlg.GetFileName();
				m_strMapList = "mapslist.ini";
				bGmRights = FALSE;
			}
			else return;
		}

		{
			////	Note : ���� ���͸� �����Ѵ�.
			//CString szFilter = "map list file  (*.list,*.ini)|*.list;*ini|";

			////	Note : ���� ���� ���̾˷α׸� ����.
			//CFileDialog dlg ( TRUE,".list",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );

			//dlg.m_ofn.lpstrInitialDir = GLOGIC::GetPath ();
			//if ( dlg.DoModal() == IDOK )
			//{
			//	m_strMapList = dlg.GetFileName();
			//}
			//else return;
		}
	}

	RANPARAM::LOAD ( pApp->m_szAppPath );
	DXPARAMSET::INIT ();

	CD3DApplication::SetScreen ( RANPARAM::dwScrWidth, RANPARAM::dwScrHeight, RANPARAM::emScrFormat, 
		RANPARAM::uScrRefreshHz, TRUE );

	if ( FAILED( CD3DApplication::Create ( m_hWnd, m_hWnd, AfxGetInstanceHandle() ) ) )		return;
	
	m_bCreated = TRUE;

	
}

void CEmulatorView::OnAppPlayGm()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CEmulatorApp *pApp = (CEmulatorApp *) AfxGetApp();

	char szFullPath[MAX_PATH] = {0};
	strcpy ( szFullPath, pApp->m_szAppPath );
	strcat ( szFullPath, SUBPATH::GLOGIC_FILE );
	GLOGIC::SetPath ( szFullPath );

	if( !AUTO_PLAY )
	{
		{
			//	Note : ���� ���͸� �����Ѵ�.
			CString szFilter = "charset file  |*.charset|";

			//	Note : ���� ���� ���̾˷α׸� ����.
			CFileDialog dlg ( TRUE,".charset",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );

			dlg.m_ofn.lpstrInitialDir = GLOGIC::GetPath ();
			if ( dlg.DoModal() == IDOK )
			{
				m_strCharSetFile = dlg.GetFileName();
				m_strMapList = "mapslist.ini";
				bGmRights = TRUE;
			}
			else return;
		}

		{
			//	Note : ���� ���͸� �����Ѵ�.
			//CString szFilter = "map list file  (*.list,*.ini)|*.list;*ini|";

			//	Note : ���� ���� ���̾˷α׸� ����.
			//CFileDialog dlg ( TRUE,".list",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this );

			//dlg.m_ofn.lpstrInitialDir = GLOGIC::GetPath ();
			//if ( dlg.DoModal() == IDOK )
			//{
				//m_strMapList = dlg.GetFileName();
			//}
			//else return;
		}
	}

	RANPARAM::LOAD ( pApp->m_szAppPath );
	DXPARAMSET::INIT ();

	CD3DApplication::SetScreen ( RANPARAM::dwScrWidth, RANPARAM::dwScrHeight, RANPARAM::emScrFormat, 
		RANPARAM::uScrRefreshHz, TRUE );

	if ( FAILED( CD3DApplication::Create ( m_hWnd, m_hWnd, AfxGetInstanceHandle() ) ) )		return;
	
	m_bCreated = TRUE;

	
	//MessageBox ("awawawaw",0,0);
}

