#include "StdAfx.h"
#include "BlockProgramAlarm.h"
#include "BasicLineBox.h"
#include "../EngineUIlib/GUInterface/BasicTextBox.h"
#include "BasicTextButton.h"
#include "../EngineLib/DxCommon/DxFontMan.h"
#include "GameTextControl.h"
#include "GLCharacter.h"
#include "GLGaeaClient.h"
#include "DxGlobalStage.h"
#include "InnerInterface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CBlockProgramAlarm::CBlockProgramAlarm () :
	m_pTextBox ( NULL ),
    m_pAlarmOKButton ( NULL )
{
}

CBlockProgramAlarm::~CBlockProgramAlarm ()
{
}

void CBlockProgramAlarm::CreateSubControl ()
{
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9, _DEFAULT_FONT_FLAG );

	CBasicLineBox* pLineBox = new CBasicLineBox;
	pLineBox->CreateSub ( this, "BASIC_LINE_BOX_MAPMOVE", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
	pLineBox->CreateBaseBoxMapMove ( "BLOCK_PROGRAM_ALARM_LINE_BOX" );
	RegisterControl ( pLineBox );	

	CBasicTextBox* pMapMoveTextBox = new CBasicTextBox;
	pMapMoveTextBox->CreateSub ( this, "BLOCK_PROGRAM_ALARM_TEXTBOX", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
    pMapMoveTextBox->SetFont ( pFont9 );
	pMapMoveTextBox->SetTextAlign ( TEXT_ALIGN_CENTER_X );
	pMapMoveTextBox->AddText ( ID2GAMEINTEXT("BLOCK_PROGRAM_ALARM") );
	RegisterControl ( pMapMoveTextBox );
	m_pTextBox = pMapMoveTextBox;

	m_pAlarmOKButton = CreateTextButton ( "BLOCK_PROGRAM_ALARM_OK", ALARM_OKBUTTON, (char*)ID2GAMEWORD ( "MAPMOVE_OKBUTTON" ) );
//	m_pAlarmOKButton->SetShortcutKey ( DIK_SPACE );
}

CBasicTextButton* CBlockProgramAlarm::CreateTextButton ( char* szButton, UIGUID ControlID , char* szText )
{
	CBasicTextButton* pTextButton = new CBasicTextButton;
	pTextButton->CreateSub ( this, "BASIC_TEXT_BUTTON24", UI_FLAG_XSIZE, ControlID );
	pTextButton->CreateBaseButton ( szButton, 
									CBasicTextButton::SIZE24, 
									CBasicButton::CLICK_FLIP, 
									szText,
									_DEFAULT_FONT_SHADOW_EX_FLAG );
	pTextButton->SetFlip ( TRUE );
	RegisterControl ( pTextButton );

	return pTextButton;
}

void CBlockProgramAlarm::DoALARM ()
{
	GLGaeaClient::GetInstance().GetCharacter()->ResetPASSIVITY();
}

void CBlockProgramAlarm::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	switch ( ControlID )
	{
	case ALARM_OKBUTTON:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				AddMessageEx ( UIMSG_MOUSEIN_ALARMBUTTON );

				if ( UIMSG_LB_UP & dwMsg )
				{
					//	Note : ���丶�콺�� �ƴ϶�� �ǻ�ǥ��
					//
					DoALARM ();

					CInnerInterface::GetInstance().SetBlockProgramAlarm ( false );
				}
			}
		}
        break;
	}
}