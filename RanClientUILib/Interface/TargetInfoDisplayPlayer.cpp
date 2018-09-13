#include "StdAfx.h"
#include "./TargetInfoDisplayPlayer.h"

#include "../EngineUILib/GUInterface/BasicProgressBar.h"
#include "../EngineUILib/GUInterface/BasicTextBox.h"
#include "BasicLineBoxEx.h"
#include "../EngineLib/DxCommon/DxFontMan.h"
#include "../EngineLib/DxCommon/d3dfontx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CTargetInfoDisplayPlayer::CTargetInfoDisplayPlayer () :
	m_pNameBoxDummy ( NULL )
{
	//memset ( m_pSchoolMark, 0, sizeof ( CUIControl* ) * nSCHOOLMARK );
	//memset ( m_pPartyMark, 0, sizeof ( CUIControl* ) * nPARTYMARK );
}

CTargetInfoDisplayPlayer::~CTargetInfoDisplayPlayer ()
{
}

void CTargetInfoDisplayPlayer::CreateSubControl ()
{
	m_pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_SHADOW_FLAG );

	//CBasicLineBoxEx* pLineBox = new CBasicLineBoxEx;
	//pLineBox->CreateSub ( this, "BASIC_LINE_BOX_EX_TARGETINFO", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	//pLineBox->CreateBaseBoxTargetInfo ( "BASIC_TARGETINFO_LINE_BOX" );
	//RegisterControl ( pLineBox );

	CBasicProgressBar* pHP = new CBasicProgressBar;
	pHP->CreateSub ( this, "TARGETINFO_HP" );
	pHP->CreateOverImage ( "TARGETINFO_HP_OVERIMAGE_PLAYER" );
	pHP->SetControlNameEx ( "HP 煎斜楚蝶夥" );
	pHP->SetAlignFlag ( UI_FLAG_XSIZE );
	RegisterControl ( pHP );
	m_pHP = pHP;



	CBasicTextBox* pNameBox = new CBasicTextBox;
	pNameBox->CreateSub ( this, "TARGETINFO_NAMEBOX" );
    pNameBox->SetFont ( m_pFont8 );
	pNameBox->SetTextAlign ( TEXT_ALIGN_CENTER_X );
	pNameBox->SetControlNameEx ( "HP" );	
	pNameBox->SetAlignFlag ( UI_FLAG_XSIZE );
	RegisterControl ( pNameBox );
	m_pNameBox = pNameBox;

	m_pNameBoxDummy = CreateControl ( "TARGETINFO_NAMEBOX", UI_FLAG_DEFAULT );


}

CUIControl*	CTargetInfoDisplayPlayer::CreateControl ( const char* szControl, WORD wAlignFlag )
{
	CUIControl* pControl = new CUIControl;
	pControl->CreateSub ( this, szControl, wAlignFlag );
	RegisterControl ( pControl );
	return pControl;
}

void CTargetInfoDisplayPlayer::SetTargetInfoPlayer ( float fPercent, CString strName, D3DCOLOR dwColor ) /*, int nSchoolMark, int nPartyMark )*/
{
	m_pHP->SetPercent ( fPercent );
	m_pNameBox->ClearText ();

	SIZE Size;
	m_pFont8->GetTextExtent ( strName.GetString (), Size );
	
	const UIRECT& rcNameBoxDummy = m_pNameBoxDummy->GetLocalPos();
	float fNameWidth = 0.0f;
	fNameWidth = (float)Size.cx;


	{
	
		SetAlignFlag ( UI_FLAG_XSIZE );

		if ( fNameWidth < rcNameBoxDummy.sizeX )
		{
			fNameWidth = rcNameBoxDummy.sizeX;
		}

		//	臢蝶 夢蝶 觼啪 葬餌檜癒
		const UIRECT& rcNameLocalPos = m_pNameBox->GetLocalPos ();
		const UIRECT& rcNameBoxNew = UIRECT ( rcNameLocalPos.left, rcNameLocalPos.top,
			fNameWidth, rcNameLocalPos.sizeY );
		AlignSubControl ( rcNameLocalPos, rcNameBoxNew );

		const UIRECT& rcLocalPosOld = GetLocalPos ();
		UIRECT rcLocalPosNew = UIRECT ( rcLocalPosOld.left, rcLocalPosOld.top,
			rcLocalPosOld.sizeX + (fNameWidth - rcNameLocalPos.sizeX), rcLocalPosOld.sizeY );

		SetLocalPos ( rcLocalPosNew );
		SetGlobalPos ( rcLocalPosNew );

		//	臢蝶 厥晦 ( 憲嬴憮 澀葡 )				
		m_pNameBox->AddTextNoSplit ( strName, dwColor );	
		m_strName = strName;
	}
}

