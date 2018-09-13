#include "StdAfx.h"
#include "CreateCharacterNew.h"
#include "BasicTextButton.h"
#include "GameTextControl.h"
#include "Outerinterface.h"
#include "ModalWindow.h"
#include "CharacterEnum.h"
#include "InnerInterface.h"
#include "DxLobyStage.h"
#include "DxGlobalStage.h"
#include "DxInputString.h"
#include "RanFilter.h"
#include "UITextControl.h"
#include "../EngineUIlib/GUInterface/UIEditBox.h"
#include "../enginelib/Common/StringUtils.h"
#include "../EngineLib/DxCommon/d3dfont.h"
#include "../EngineUILib/GUInterface/BasicButton.h"
#include "../EngineUILib/GUInterface/BasicTextBox.h"
#include "../Interface/UITextControl.h"
#include "./BasicVarTextBox.h"
#include "./BasicLineBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int CCreateCharacterNew::nLIMITCHAR = 16;

CCreateCharacterNew::CCreateCharacterNew()
	: m_pButtonPrev ( NULL ) 
	, m_pButtonNext ( NULL ) 
	, m_pButtonNew ( NULL )
	, m_pButtonCancel ( NULL )
	, m_uCharRemain ( USHRT_MAX )
	, m_pButtonLeft ( NULL ) 
	, m_pButtonRight ( NULL ) 
	, m_pBackImage ( NULL )
	, m_pSchoolNameStatic ( NULL )
	, m_pSchoolDec ( NULL )
	, m_pSexNameStatic ( NULL )
	, m_pClassNameStatic ( NULL ) 
	, m_pClassDec ( NULL )
	, m_pImage_Back_Set ( NULL )
	, m_pImage_Back_School ( NULL ) 
	, m_pImage_Back_Sex ( NULL ) 
	, m_pImage_Back_info ( NULL ) 
	, m_pImage_Back_Class ( NULL ) 
	, m_pImage_Dec_School ( NULL )
	, m_pImage_Dec_Sex ( NULL )
	, m_pImage_Dec_Class ( NULL )
	, m_pImage_Logo ( NULL )
	, m_pDummyImage ( NULL ) 
	, m_nSchool ( -1 )
	, m_nSex ( -1 ) 
	, m_nClass ( -1 ) 
	, m_nHair ( -1 )
	, m_nFace ( -1 )
	, mapSchool( -1 )
	, m_bUseCancel ( TRUE )
	, m_hCheckStrDLL( NULL )
	, m_pCheckString( NULL )
	, m_pButtonLeft_face( NULL )
	, m_pButtonRight_face( NULL )
	, m_pButtonLeft_hair( NULL )
	, m_pButtonRight_hair( NULL )
	, m_pEditBox ( NULL )
	, pTextBoxSchool( NULL )
	, pTextBoxSex( NULL )
	, pTextBoxClass( NULL )
	, pTextBoxInfo( NULL )
	, m_pInfo( NULL )
	, pBasicLineBoxSchool( NULL )
	, pBasicLineBoxSex( NULL )
	, pBasicLineBoxClass( NULL )
	, m_pBasicLineBox( NULL )
	, m_pBasicLineBoxServer( NULL )
{
	memset( m_pSelectSchoolImg, 0, sizeof( CUIControl* ) * MAX_SELSCHOOL );
	memset( m_pSelectSchoolSetImg, 0, sizeof( CUIControl* ) * MAX_SELSCHOOL );
	memset( m_pSelectSexImg, 0 , sizeof( CUIControl* ) * MAX_SEX );
	memset( m_pSelectSexSetImg, 0 , sizeof( CUIControl* ) * MAX_SEX );
	memset ( m_pSelectClassImg_men, 0, sizeof( CUIControl* ) * MAX_CLASS );
	memset ( m_pSelectClassSetImg_men, 0, sizeof( CUIControl* ) * MAX_CLASS );
	memset ( m_pSelectClassImg_women, 0, sizeof( CUIControl* ) * MAX_CLASS );
	memset ( m_pSelectClassSetImg_women, 0, sizeof( CUIControl* ) * MAX_CLASS );

	memset( m_pSchoolImg, 0, sizeof( CUIControl* ) * MAX_SELSCHOOL );
	memset( m_pSexImg, 0 , sizeof( CUIControl* ) * MAX_SEX );
	memset ( m_pClassImg_men, 0, sizeof( CUIControl* ) * MAX_CLASS );
	memset ( m_pClassImg_women, 0, sizeof( CUIControl* ) * MAX_CLASS );
}


CCreateCharacterNew::~CCreateCharacterNew()
{
	if ( m_hCheckStrDLL ) FreeLibrary( m_hCheckStrDLL );
}


void CCreateCharacterNew::CreateSubControl()
{
	const int nBUTTONSIZE = CBasicTextButton::SIZE40;
	float fontSize = 1.0f;

	LONG lResolution = COuterInterface::GetInstance().GetResolution ();
	WORD X_RES = HIWORD( lResolution );

	//if ( X_RES <= 800 ) fontSize = fFontRate;

	CD3DFontPar* pFont20 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 20 * fontSize, _DEFAULT_FONT_FLAG );
	CD3DFontPar* pFont12 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 12 * fontSize, _DEFAULT_FONT_FLAG );
	CD3DFontPar* pFont9 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 9 * fontSize, _DEFAULT_FONT_FLAG );
	CD3DFontPar* pFont10 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 10 * fontSize, _DEFAULT_FONT_FLAG );
	CD3DFontPar* pFont16 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 16 * fontSize, _DEFAULT_FONT_FLAG );
	CD3DFontPar* pFont8 = DxFontMan::GetInstance().LoadDxFont ( _DEFAULT_FONT, 8 * fontSize, _DEFAULT_FONT_FLAG );

	//�����ѧ
	{
		m_pBasicLineBox = new CBasicLineBox;
		m_pBasicLineBox->CreateSub ( this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pBasicLineBox->CreateBaseBoxOuter ( "CHARACTER_PAGE_DOWNBACK" );
		RegisterControl ( m_pBasicLineBox );
	}
	//�����ѧ Server
	{
		m_pBasicLineBoxServer = new CBasicLineBox;
		m_pBasicLineBoxServer->CreateSub ( this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pBasicLineBoxServer->CreateBaseBoxOuter ( "CHARACTER_PAGE_DOWNBACK_SERVER" );
		RegisterControl ( m_pBasicLineBoxServer );
	}
	
	{
		CBasicTextBox* m_pServerStatic = new CBasicTextBox;
		m_pServerStatic->CreateSub ( this, "CHARACTER_PAGE_SERVER_CHANEL", UI_FLAG_DEFAULT );
		m_pServerStatic->SetFont ( pFont9 );
		m_pServerStatic->AddText( ID2GAMEEXTEXT("SELECT_CHARACTER_PAGE_SERVER_CHANEL"), NS_UITEXTCOLOR::WHITE );
		m_pServerStatic->SetTextAlign ( TEXT_ALIGN_LEFT );		
		RegisterControl ( m_pServerStatic );
	}
	//���˹ѧ������ �ç���¹
	{
		pBasicLineBoxSchool = new CBasicLineBox;
		pBasicLineBoxSchool->CreateSub ( this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBoxSchool->CreateBaseBoxOuter("NEW_CHAR_BACK_SCHOOL_IMG");
		pBasicLineBoxSchool->SetVisibleSingle(FALSE);
		RegisterControl ( pBasicLineBoxSchool );
	}
	//���˹ѧ������ �?
	{
		pBasicLineBoxSex = new CBasicLineBox;
		pBasicLineBoxSex->CreateSub ( this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBoxSex->CreateBaseBoxOuter("NEW_CHAR_BACK_SEX_IMG");
		pBasicLineBoxSex->SetVisibleSingle(FALSE);
		RegisterControl ( pBasicLineBoxSex );
	}
	//���˹ѧ������ �Ҫ�?
	{
		pBasicLineBoxClass = new CBasicLineBox;
		pBasicLineBoxClass->CreateSub ( this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		pBasicLineBoxClass->CreateBaseBoxOuter("NEW_CHAR_BACK_CLASS_IMG");
		pBasicLineBoxClass->SetVisibleSingle(FALSE);
		RegisterControl ( pBasicLineBoxClass );
	}
	//�����ç���¹
	{
		m_pSchoolNameStatic = new CBasicTextBox;
		m_pSchoolNameStatic->CreateSub ( this, "2012_NEW_CHAR_SELECT_SCHOOL_NAME", UI_FLAG_DEFAULT, SELECT_SCHOOL_NAME );
		m_pSchoolNameStatic->SetFont ( pFont10 );
		m_pSchoolNameStatic->SetTextAlign ( TEXT_ALIGN_LEFT );		
		RegisterControl ( m_pSchoolNameStatic );
	}
	//�������ç���¹
	{
		m_pSchoolDec = new CBasicTextBox;
		m_pSchoolDec->CreateSub ( this, "2012_NEW_CHAR_SELECT_SCHOOL_DEC", UI_FLAG_DEFAULT, SELECT_SCHOOL_DEC );
		m_pSchoolDec->SetFont ( pFont8 );
		m_pSchoolDec->SetTextAlign ( TEXT_ALIGN_LEFT );		
		RegisterControl ( m_pSchoolDec );
	}
	//�?
	{
		m_pSexNameStatic = new CBasicTextBox;
		m_pSexNameStatic->CreateSub ( this, "2012_NEW_CHAR_SELECT_SEX_NAME", UI_FLAG_DEFAULT, SELECT_SEX_NAME );
		m_pSexNameStatic->SetFont ( pFont10 );
		m_pSexNameStatic->SetTextAlign ( TEXT_ALIGN_LEFT );		
		RegisterControl ( m_pSexNameStatic );
	}
	//�����Ҫ�?
	{
		m_pClassNameStatic = new CBasicTextBox;
		m_pClassNameStatic->CreateSub ( this, "2012_NEW_CHAR_SELECT_CLASS_NAME", UI_FLAG_DEFAULT, SELECT_CLASS_NAME );
		m_pClassNameStatic->SetFont ( pFont10 );
		m_pClassNameStatic->SetTextAlign ( TEXT_ALIGN_LEFT );		
		RegisterControl ( m_pClassNameStatic );
	}
	//�������Ҫ�?
	{
		m_pClassDec = new CBasicTextBox;
		m_pClassDec->CreateSub ( this, "2012_NEW_CHAR_SELECT_CLASS_DEC", UI_FLAG_DEFAULT, SELECT_CLASS_DEC );
		m_pClassDec->SetFont ( pFont8 );
		m_pClassDec->SetTextAlign ( TEXT_ALIGN_LEFT );		
		RegisterControl ( m_pClassDec );
	}
	//�����ѧ ������
	{
		m_pImage_Back_Set =  new CBasicLineBox;
		m_pImage_Back_Set->CreateSub ( this, "BASIC_LINE_BOX_OUTER", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pImage_Back_Set->CreateBaseBoxOuter ( "2012_NEW_CHAR_IMAGE_SET" );
		m_pImage_Back_Set->SetVisibleSingle( TRUE );
		RegisterControl ( m_pImage_Back_Set );
	}

	{
		m_pBackImage =  new CUIControl;
		m_pBackImage->CreateSub ( this, "NEW_CHAR_SELECT_BACK_IMAGE", UI_FLAG_DEFAULT );
		m_pBackImage->SetVisibleSingle( TRUE );
		RegisterControl ( m_pBackImage );
	}

	//�����ѧ���ǹ�ͧ��Ǣ�?���͡�ç����?
	{
		m_pImage_Back_School =  new CUIControl;
		m_pImage_Back_School->CreateSub ( this, "2012_NEW_CHAR_IMAGE_BACK_SCHOOL", UI_FLAG_DEFAULT );
		m_pImage_Back_School->SetVisibleSingle( TRUE );
		RegisterControl ( m_pImage_Back_School );
	}
	//��Ǣ�����͡�ç����?
	{
		pTextBoxSchool = new CBasicTextBox;
		pTextBoxSchool->CreateSub ( this, "2012_NEW_CHAR_TEXT_SCHOOL", UI_FLAG_DEFAULT );
		pTextBoxSchool->SetFont ( pFont9 );
		pTextBoxSchool->AddText ( ID2GAMEEXTEXT("2012_NEW_CHAR_TEXT_SCHOOL" ), NS_UITEXTCOLOR::PALEGREEN );
		pTextBoxSchool->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );		
		RegisterControl ( pTextBoxSchool );
	}
	//�����ѧ���ǹ�ͧ��Ǣ�?���͡��
	{
		m_pImage_Back_Sex =  new CUIControl;
		m_pImage_Back_Sex->CreateSub ( this, "2012_NEW_CHAR_IMAGE_BACK_SEX", UI_FLAG_DEFAULT );	
		m_pImage_Back_Sex->SetVisibleSingle( TRUE );
		RegisterControl ( m_pImage_Back_Sex );
	}
	//��Ǣ�����͡��
	{
		pTextBoxSex = new CBasicTextBox;
		pTextBoxSex->CreateSub ( this, "2012_NEW_CHAR_TEXT_SEX", UI_FLAG_DEFAULT );
		pTextBoxSex->SetFont ( pFont9 );
		pTextBoxSex->AddText ( ID2GAMEEXTEXT("2012_NEW_CHAR_TEXT_SEX" ), NS_UITEXTCOLOR::WHITE );
		pTextBoxSex->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );		
		RegisterControl ( pTextBoxSex );
	}
	//�����ѧ���ǹ�ͧ��Ǣ�?���͡�Ҫվ
	{
		m_pImage_Back_Class =  new CUIControl;
		m_pImage_Back_Class->CreateSub ( this, "2012_NEW_CHAR_IMAGE_BACK_CLASS", UI_FLAG_DEFAULT );
		m_pImage_Back_Class->SetVisibleSingle( TRUE );
		RegisterControl ( m_pImage_Back_Class );
	}
	//��Ǣ�����͡�Ҫվ
	{
		pTextBoxClass = new CBasicTextBox;
		pTextBoxClass->CreateSub ( this, "2012_NEW_CHAR_TEXT_CLASS", UI_FLAG_DEFAULT );
		pTextBoxClass->SetFont ( pFont9 );
		pTextBoxClass->AddText ( ID2GAMEEXTEXT("2012_NEW_CHAR_TEXT_CLASS" ), NS_UITEXTCOLOR::WHITE );
		pTextBoxClass->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );		
		RegisterControl ( pTextBoxClass );
	}
	//�����ѧ���ǹ�ͧ��Ǣ�?���͡����
	{
		m_pImage_Back_info =  new CUIControl;
		m_pImage_Back_info->CreateSub ( this, "2012_NEW_CHAR_IMAGE_BACK_INFO", UI_FLAG_DEFAULT );
		m_pImage_Back_info->SetVisibleSingle( TRUE );
		RegisterControl ( m_pImage_Back_info );
	}
	//��Ǣ�����͡����
	{
		pTextBoxInfo = new CBasicTextBox;
		pTextBoxInfo->CreateSub ( this, "2012_NEW_CHAR_TEXT_INFO", UI_FLAG_DEFAULT );
		pTextBoxInfo->SetFont ( pFont9 );
		pTextBoxInfo->AddText ( ID2GAMEEXTEXT("2012_NEW_CHAR_TEXT_INFO" ), NS_UITEXTCOLOR::WHITE );
		pTextBoxInfo->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );		
		RegisterControl ( pTextBoxInfo );
	}
	//��Ǣ���˹��
	{
		pTextBoxFace = new CBasicTextBox;
		pTextBoxFace->CreateSub ( this, "2012_NEW_CHAR_TEXT_FACE", UI_FLAG_DEFAULT );
		pTextBoxFace->SetFont ( pFont9 );
		pTextBoxFace->SetTextAlign ( TEXT_ALIGN_LEFT );		
		pTextBoxFace->AddText ( ID2GAMEEXTEXT("2012_NEW_CHAR_TEXT_FACE" ), NS_UITEXTCOLOR::WHITE );
		RegisterControl ( pTextBoxFace );
	}
	//�˹��
	{
		pTextBoxFace_dec = new CBasicTextBox;
		pTextBoxFace_dec->CreateSub ( this, "2012_NEW_CHAR_TEXT_FACE_DEC", UI_FLAG_DEFAULT );
		pTextBoxFace_dec->SetFont ( pFont9 );
		pTextBoxFace_dec->SetTextAlign ( TEXT_ALIGN_LEFT );
		RegisterControl ( pTextBoxFace_dec );
	}
	//��Ǣ�ͷç�?
	{
		pTextBoxHair = new CBasicTextBox;
		pTextBoxHair->CreateSub ( this, "2012_NEW_CHAR_TEXT_HAIR", UI_FLAG_DEFAULT );
		pTextBoxHair->SetFont ( pFont9 );
		pTextBoxHair->SetTextAlign ( TEXT_ALIGN_LEFT);		
		pTextBoxHair->AddText ( ID2GAMEEXTEXT("2012_NEW_CHAR_TEXT_HAIR" ), NS_UITEXTCOLOR::WHITE );
		RegisterControl ( pTextBoxHair );
	}
	//�ç�?
	{
		pTextBoxHair_dec = new CBasicTextBox;
		pTextBoxHair_dec->CreateSub ( this, "2012_NEW_CHAR_TEXT_HAIR_DEC", UI_FLAG_DEFAULT );
		pTextBoxHair_dec->SetFont ( pFont9 );
		pTextBoxHair_dec->SetTextAlign ( TEXT_ALIGN_LEFT);		
		RegisterControl ( pTextBoxHair_dec );
	}
	//���������͡�ç����?SG �ѧ�����?
	{		
		m_pSelectSchoolImg[0] = new CUIControl;
		m_pSelectSchoolImg[0]->CreateSub ( this, "2012_NEW_CHAR_SELECT_SCHOOL_IMAGE_SG", UI_FLAG_DEFAULT, SELECT_SCHOOL_IMAGE_SG );	
		m_pSelectSchoolImg[0]->SetVisibleSingle ( TRUE );
		m_pSelectSchoolImg[0]->SetTransparentOption( TRUE );
		RegisterControl ( m_pSelectSchoolImg[0] );
	}
	//���������͡�ç����?SG �͹��衴
	{
		m_pSelectSchoolSetImg[0] = new CUIControl;
		m_pSelectSchoolSetImg[0]->CreateSub ( this, "2012_NEW_CHAR_SELECT_SCHOOL_SET_IMAGE_SG", UI_FLAG_DEFAULT, SELECT_SCHOOL_SET_IMAGE_SG );	
		m_pSelectSchoolSetImg[0]->SetVisibleSingle ( FALSE );
		m_pSelectSchoolSetImg[0]->SetTransparentOption( TRUE );
		RegisterControl ( m_pSelectSchoolSetImg[0] );
	}
	//���������͡�ç����?MP �ѧ�����?
	{	
		m_pSelectSchoolImg[1] = new CUIControl;
		m_pSelectSchoolImg[1]->CreateSub ( this, "2012_NEW_CHAR_SELECT_SCHOOL_IMAGE_MP", UI_FLAG_DEFAULT, SELECT_SCHOOL_IMAGE_MP );	
		m_pSelectSchoolImg[1]->SetVisibleSingle ( TRUE );
		m_pSelectSchoolImg[1]->SetTransparentOption( TRUE );
		RegisterControl ( m_pSelectSchoolImg[1] );
	}
	//���������͡�ç����?MP �͹��衴
	{
		m_pSelectSchoolSetImg[1] = new CUIControl;
		m_pSelectSchoolSetImg[1]->CreateSub ( this, "2012_NEW_CHAR_SELECT_SCHOOL_SET_IMAGE_MP", UI_FLAG_DEFAULT, SELECT_SCHOOL_SET_IMAGE_MP );	
		m_pSelectSchoolSetImg[1]->SetVisibleSingle ( FALSE );
		m_pSelectSchoolSetImg[1]->SetTransparentOption( TRUE );
		RegisterControl ( m_pSelectSchoolSetImg[1] );
	}
	//���������͡�ç����?PH �ѧ�����?
	{
		m_pSelectSchoolImg[2] = new CUIControl;
		m_pSelectSchoolImg[2]->CreateSub ( this, "2012_NEW_CHAR_SELECT_SCHOOL_IMAGE_PH", UI_FLAG_DEFAULT, SELECT_SCHOOL_IMAGE_PH );	
		m_pSelectSchoolImg[2]->SetVisibleSingle ( TRUE );
		m_pSelectSchoolImg[2]->SetTransparentOption( TRUE );
		RegisterControl ( m_pSelectSchoolImg[2] );
	}
	//���������͡�ç����?PH �͹��衴
	{
		m_pSelectSchoolSetImg[2] = new CUIControl;
		m_pSelectSchoolSetImg[2]->CreateSub ( this, "2012_NEW_CHAR_SELECT_SCHOOL_SET_IMAGE_PH", UI_FLAG_DEFAULT, SELECT_SCHOOL_SET_IMAGE_PH );	
		m_pSelectSchoolSetImg[2]->SetVisibleSingle ( FALSE );
		m_pSelectSchoolSetImg[2]->SetTransparentOption( TRUE );
		RegisterControl ( m_pSelectSchoolSetImg[2] );
	}
	//�ͤ͹�ç����?SG
	{
		m_pSchoolImg[0] = new CUIControl;
		m_pSchoolImg[0]->CreateSub ( this, "NEW_CHAR_ICON_SCHOOL_IMAGE_SG", UI_FLAG_DEFAULT, ICON_SCHOOL_IMAGE_SG );	
		m_pSchoolImg[0]->SetVisibleSingle ( FALSE );
		m_pSchoolImg[0]->SetTransparentOption( TRUE );
		RegisterControl ( m_pSchoolImg[0] );
	}
	//�ͤ͹�ç����?MP
	{
		m_pSchoolImg[1] = new CUIControl;
		m_pSchoolImg[1]->CreateSub ( this, "NEW_CHAR_ICON_SCHOOL_IMAGE_MP", UI_FLAG_DEFAULT, ICON_SCHOOL_IMAGE_MP );	
		m_pSchoolImg[1]->SetVisibleSingle ( FALSE );
		m_pSchoolImg[1]->SetTransparentOption( TRUE );
		RegisterControl ( m_pSchoolImg[1] );
	}
	//�ͤ͹�ç����?PH
	{
		m_pSchoolImg[2] = new CUIControl;
		m_pSchoolImg[2]->CreateSub ( this, "NEW_CHAR_ICON_SCHOOL_IMAGE_PH", UI_FLAG_DEFAULT, ICON_SCHOOL_IMAGE_PH );	
		m_pSchoolImg[2]->SetVisibleSingle ( FALSE );
		m_pSchoolImg[2]->SetTransparentOption( TRUE );
		RegisterControl ( m_pSchoolImg[2] );
	}
	//�ͤ͹�� ��?
	{
		m_pSexImg[0] = new CUIControl;
		m_pSexImg[0]->CreateSub ( this, "NEW_CHAR_ICON_SEX_IMAGE_MEN", UI_FLAG_DEFAULT, ICON_SEX_IMAGE_MEN );	
		m_pSexImg[0]->SetVisibleSingle ( FALSE );
		m_pSexImg[0]->SetTransparentOption( TRUE );
		RegisterControl ( m_pSexImg[0] );
	}
	//�ͤ͹�� ˭ԧ
	{
		m_pSexImg[1] = new CUIControl;
		m_pSexImg[1]->CreateSub ( this, "NEW_CHAR_ICON_SEX_IMAGE_WOMEN", UI_FLAG_DEFAULT, ICON_SEX_IMAGE_WOMEN );	
		m_pSexImg[1]->SetVisibleSingle ( FALSE );
		m_pSexImg[1]->SetTransparentOption( TRUE );
		RegisterControl ( m_pSexImg[1] );
	}
	{
		CString strTemp;
		for ( int i = 0 ; i < MAX_SEX; ++i ) 
		{
			{
				strTemp.Format ( "2012_NEW_CHAR_SELECT_SEX_IMAGE%d",i); 
				m_pSelectSexImg[i] = new CUIControl;
				m_pSelectSexImg[i]->CreateSub ( this, strTemp, UI_FLAG_DEFAULT, SELECT_SEX_IMAGE0 + i );	
				m_pSelectSexImg[i]->SetVisibleSingle ( TRUE );
				m_pSelectSexImg[i]->SetTransparentOption( TRUE );
				RegisterControl ( m_pSelectSexImg[i] );
			}
			{
				strTemp.Format ( "2012_NEW_CHAR_SELECT_SEX_SET_IMAGE%d",i); 
				m_pSelectSexSetImg[i] = new CUIControl;
				m_pSelectSexSetImg[i]->CreateSub ( this, strTemp, UI_FLAG_DEFAULT, SELECT_SEX_SET_IMAGE0 + i );	
				m_pSelectSexSetImg[i]->SetVisibleSingle ( FALSE );
				m_pSelectSexSetImg[i]->SetTransparentOption( TRUE );
				RegisterControl ( m_pSelectSexSetImg[i] );
			}
		}
	}
	CString strTemp;
	//���������͡�Ҫվ ˭ԧ
	for ( int i = 0 ; i < MAX_CLASS; ++i ) 
	{
		{	
			strTemp.Format ( "2012_NEW_CHAR_SELECT_CLASS_WOMEN_IMAGE%d",i); 	
			m_pSelectClassImg_women[i] = new CUIControl;
			m_pSelectClassImg_women[i]->CreateSub ( this, strTemp, UI_FLAG_DEFAULT, SELECT_CLASS_WOMEN_IMAGE0 + i );	
			m_pSelectClassImg_women[i]->SetVisibleSingle ( FALSE );
			m_pSelectClassImg_women[i]->SetTransparentOption( TRUE );
			RegisterControl ( m_pSelectClassImg_women[i] );

			//�ͤ͹�Ҫվ
			strTemp.Format ( "NEW_CHAR_ICON_CLASS_WOMEN_IMAGE%d",i); 	
			m_pClassImg_women[i] = new CUIControl;
			m_pClassImg_women[i]->CreateSub ( this, strTemp, UI_FLAG_DEFAULT, ICON_CLASS_WOMEN_IMAGE0 + i );	
			m_pClassImg_women[i]->SetVisibleSingle ( FALSE );
			m_pClassImg_women[i]->SetTransparentOption( TRUE );
			RegisterControl ( m_pClassImg_women[i] );
		}
		{
			strTemp.Format ( "2012_NEW_CHAR_SELECT_CLASS_SET_WOMEN_IMAGE%d",i); 
			m_pSelectClassSetImg_women[i] = new CUIControl;
			m_pSelectClassSetImg_women[i]->CreateSub ( this, strTemp, UI_FLAG_DEFAULT, SELECT_CLASS_SET_WOMEN_IMAGE0 + i );	
			m_pSelectClassSetImg_women[i]->SetVisibleSingle ( FALSE );
			m_pSelectClassSetImg_women[i]->SetTransparentOption( TRUE );
			RegisterControl ( m_pSelectClassSetImg_women[i] );
		}
	}
	//���������͡�Ҫվ ��?
	for ( int i = 0 ; i < MAX_CLASS; ++i ) 
	{
		{	
			strTemp.Format ( "2012_NEW_CHAR_SELECT_CLASS_MEN_IMAGE%d",i); 	
			m_pSelectClassImg_men[i] = new CUIControl;
			m_pSelectClassImg_men[i]->CreateSub ( this, strTemp, UI_FLAG_DEFAULT, SELECT_CLASS_MEN_IMAGE0 + i );	
			m_pSelectClassImg_men[i]->SetVisibleSingle ( TRUE );
			m_pSelectClassImg_men[i]->SetTransparentOption( TRUE );
			RegisterControl ( m_pSelectClassImg_men[i] );
			
			//�ͤ͹�Ҫվ
			strTemp.Format ( "NEW_CHAR_ICON_CLASS_MEN_IMAGE%d",i); 	
			m_pClassImg_men[i] = new CUIControl;
			m_pClassImg_men[i]->CreateSub ( this, strTemp, UI_FLAG_DEFAULT, ICON_CLASS_MEN_IMAGE0 + i );	
			m_pClassImg_men[i]->SetVisibleSingle ( FALSE );
			m_pClassImg_men[i]->SetTransparentOption( TRUE );
			RegisterControl ( m_pClassImg_men[i] );
		}
		{
			strTemp.Format ( "2012_NEW_CHAR_SELECT_CLASS_SET_MEN_IMAGE%d",i); 
			m_pSelectClassSetImg_men[i] = new CUIControl;
			m_pSelectClassSetImg_men[i]->CreateSub ( this, strTemp, UI_FLAG_DEFAULT, SELECT_CLASS_SET_MEN_IMAGE0 + i );	
			m_pSelectClassSetImg_men[i]->SetVisibleSingle ( FALSE );
			m_pSelectClassSetImg_men[i]->SetTransparentOption( TRUE );
			RegisterControl ( m_pSelectClassSetImg_men[i] );
		}
	}
	//COPYRIGHT
	{
		CBasicTextBox* pTextBox = new CBasicTextBox;
		pTextBox->CreateSub ( this, "2012_NEW_CHAR_SELECT_COPYRIGHT", UI_FLAG_DEFAULT );
		pTextBox->SetFont ( pFont10 );
		pTextBox->SetTextAlign ( TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y );		
		pTextBox->AddText ( ID2GAMEWORD("COPYRIGHT_COMPANY" ), NS_UITEXTCOLOR::WHITE );
		RegisterControl ( pTextBox );
	}
	//��Ǣ�������?
	{
		CBasicTextBox* pTextBoxName = new CBasicTextBox;
		pTextBoxName->CreateSub ( this, "2012_NEW_CHAR_TEXT_NAME", UI_FLAG_DEFAULT );
		pTextBoxName->SetFont ( pFont10 );
		pTextBoxName->SetTextAlign ( TEXT_ALIGN_LEFT);		
		pTextBoxName->AddText ( ID2GAMEEXTEXT("2012_NEW_CHAR_TEXT_NAME" ), NS_UITEXTCOLOR::WHITE );
		RegisterControl ( pTextBoxName );
	}

	{
		m_pButtonPrev = new CBasicTextButton;
		m_pButtonPrev->CreateSub ( this, "BASIC_TEXT_BUTTON40", UI_FLAG_XSIZE, SELECT_PREV_BUTTON );
		m_pButtonPrev->CreateBaseButton ( "2012_NEW_CHAR_SELECT_PREV_BUTTON", nBUTTONSIZE, CBasicButton::CLICK_FLIP, (char*)ID2GAMEEXTEXT ( "2012_NEW_CHAR_SELECT_PREV_BUTTON", 0 ) );
		m_pButtonPrev->SetShortcutKey ( DIK_ESCAPE );
		RegisterControl ( m_pButtonPrev );
	}
	{
		m_pButtonNext = new CBasicTextButton;
		m_pButtonNext->CreateSub ( this, "BASIC_TEXT_BUTTON40", UI_FLAG_XSIZE, SELECT_NEXT_BUTTON );
		m_pButtonNext->CreateBaseButton ( "2012_NEW_CHAR_SELECT_NEXT_BUTTON", nBUTTONSIZE, CBasicButton::CLICK_FLIP, (char*)ID2GAMEEXTEXT( "2012_NEW_CHAR_SELECT_PREV_BUTTON", 1 ) );
		RegisterControl ( m_pButtonNext );
	}

	{
		m_pButtonNew = new CBasicTextButton;
		m_pButtonNew->CreateSub ( this, "BASIC_TEXT_BUTTON40", UI_FLAG_XSIZE, NEW_CHARACTER_NEW );
		m_pButtonNew->CreateBaseButton ( "NEW_CHARACTER_NEW", nBUTTONSIZE, CBasicButton::CLICK_FLIP, (char*)ID2GAMEEXTEXT ( "2012_NEW_CHAR_SELECT_PREV_BUTTON", 2 ) );
		RegisterControl ( m_pButtonNew );
	}
	{
		m_pButtonCancel = new CBasicTextButton;
		m_pButtonCancel->CreateSub ( this, "BASIC_TEXT_BUTTON40", UI_FLAG_XSIZE, NEW_CHARACTER_CANCEL );
		m_pButtonCancel->CreateBaseButton ( "NEW_CHARACTER_CANCEL", nBUTTONSIZE, CBasicButton::CLICK_FLIP, (char*)ID2GAMEEXTEXT( "2012_NEW_CHAR_SELECT_PREV_BUTTON", 3 ) );
		RegisterControl ( m_pButtonCancel );
	}

	//������ع����Фë��?
	{
		m_pButtonLeft = new CBasicButton;
		m_pButtonLeft->CreateSub( this, "2012_NEW_CHAR_SELECT_LEFT_BUTTON", UI_FLAG_XSIZE | UI_FLAG_YSIZE, SELECT_LEFT_BUTTON );
		m_pButtonLeft->CreateFlip( "2012_NEW_CHAR_SELECT_LEFT_BUTTON_CLICK", CBasicButton::CLICK_FLIP );
		m_pButtonLeft->SetVisibleSingle( FALSE );
		RegisterControl( m_pButtonLeft );
	}
	//������ع����Фâ��
	{
		m_pButtonRight = new CBasicButton;
		m_pButtonRight->CreateSub( this, "2012_NEW_CHAR_SELECT_RIGHT_BUTTON", UI_FLAG_RIGHT, SELECT_RIGHT_BUTTON );
		m_pButtonRight->CreateFlip( "2012_NEW_CHAR_SELECT_RIGHT_BUTTON_CLICK", CBasicButton::CLICK_FLIP );
		m_pButtonRight->SetAlignFlag ( UI_FLAG_RIGHT );
		m_pButtonRight->SetVisibleSingle( FALSE );
		RegisterControl( m_pButtonRight );
	}
	{
		m_pButtonLeft_face = new CBasicButton;
		m_pButtonLeft_face->CreateSub( this, "2012_NEW_CHAR_SELECT_STYLE_PAGE_FACE_L", UI_FLAG_XSIZE | UI_FLAG_YSIZE, SELECT_FACE_LEFT );
		m_pButtonLeft_face->CreateFlip( "2012_NEW_CHAR_SELECT_STYLE_PAGE_FACE_L_F", CBasicButton::CLICK_FLIP );
		m_pButtonLeft_face->SetVisibleSingle( TRUE );
		RegisterControl( m_pButtonLeft_face );
	}
	{
		m_pButtonRight_face = new CBasicButton;
		m_pButtonRight_face->CreateSub( this, "2012_NEW_CHAR_SELECT_STYLE_PAGE_FACE_R", UI_FLAG_RIGHT, SELECT_FACE_RIGHT );
		m_pButtonRight_face->CreateFlip( "2012_NEW_CHAR_SELECT_STYLE_PAGE_FACE_R_F", CBasicButton::CLICK_FLIP );
		m_pButtonRight_face->SetAlignFlag ( UI_FLAG_RIGHT );
		m_pButtonRight_face->SetVisibleSingle( TRUE );
		RegisterControl( m_pButtonRight_face );
	}
	{
		m_pButtonLeft_hair = new CBasicButton;
		m_pButtonLeft_hair->CreateSub( this, "2012_NEW_CHAR_SELECT_STYLE_PAGE_HAIR_L", UI_FLAG_XSIZE | UI_FLAG_YSIZE, SELECT_HAIR_LEFT );
		m_pButtonLeft_hair->CreateFlip( "2012_NEW_CHAR_SELECT_STYLE_PAGE_HAIR_L_F", CBasicButton::CLICK_FLIP );
		m_pButtonLeft_hair->SetVisibleSingle( TRUE );
		RegisterControl( m_pButtonLeft_hair );
	}
	{
		m_pButtonRight_hair = new CBasicButton;
		m_pButtonRight_hair->CreateSub( this, "2012_NEW_CHAR_SELECT_STYLE_PAGE_HAIR_R", UI_FLAG_RIGHT, SELECT_HAIR_RIGHT );
		m_pButtonRight_hair->CreateFlip( "2012_NEW_CHAR_SELECT_STYLE_PAGE_HAIR_R_F", CBasicButton::CLICK_FLIP );
		m_pButtonRight_hair->SetAlignFlag ( UI_FLAG_RIGHT );
		m_pButtonRight_hair->SetVisibleSingle( TRUE );
		RegisterControl( m_pButtonRight_hair );
	}
	{
		m_pEditBox = new CUIEditBox;
		m_pEditBox->CreateSub ( this, "2012_NEW_CREATE_CHARACTER_NAMEEDIT_BOX", UI_FLAG_DEFAULT, NEWCHAR_NAME_EDIT );
		m_pEditBox->CreateCarrat ( "2012_NEW_CREATE_CHARACTER_NAME_CARRAT", TRUE, UINT_MAX );
		m_pEditBox->SetLimitInput ( nLIMITCHAR );
		m_pEditBox->SetFont ( pFont10 );
		RegisterControl ( m_pEditBox );
	}
	{
		m_pInfo = new CBasicVarTextBox;
		m_pInfo->CreateSub ( this, "BASIC_VAR_TEXT", UI_FLAG_XSIZE | UI_FLAG_YSIZE );
		m_pInfo->CreateSubControl ();
		m_pInfo->SetMousePointGap ( D3DXVECTOR2(30.0f,30.0f) );
		m_pInfo->SetVisibleSingle ( FALSE );
		RegisterControl ( m_pInfo );
	}

//#ifdef TH_PARAM	
	m_hCheckStrDLL = LoadLibrary("ThaiCheck.dll");

	if ( m_hCheckStrDLL )
	{
		m_pCheckString = ( BOOL (_stdcall*)(CString)) GetProcAddress(m_hCheckStrDLL, "IsCompleteThaiChar");
	}

//#endif
}

CBasicButton* CCreateCharacterNew::CreateFlipButton ( char* szButton, char* szButtonFlip, UIGUID ControlID )
{
	CBasicButton* pButton = new CBasicButton;
	pButton->CreateSub ( this, szButton, UI_FLAG_DEFAULT, ControlID );
	pButton->CreateFlip ( szButtonFlip, CBasicButton::CLICK_FLIP );
	RegisterControl ( pButton );

	return pButton;
}
void CCreateCharacterNew::Update( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl )
{
	if ( !IsVisible () ) return ;

	m_pInfo->SetVisibleSingle( FALSE );
	CUIGroup::Update ( x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl );

	const USHORT uCharRemain = COuterInterface::GetInstance().GetCharRemain ();

	if ( m_uCharRemain != uCharRemain )
	{
		CString strTemp;
		strTemp.Format ( "%s (%u)", (char*)ID2GAMEWORD ( "SELECT_CHARACTER_PAGE_BUTTON", 0 ), uCharRemain );
		m_pButtonNew->SetOneLineText ( strTemp );

		m_uCharRemain = uCharRemain;
	}
}

void CCreateCharacterNew::TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg )
{
	CUIGroup::TranslateUIMessage ( ControlID, dwMsg );

	DxLobyStage *pLobyStage = DxGlobalStage::GetInstance().GetLobyStage();

	switch ( ControlID )
	{
	case SELECT_NEXT_BUTTON:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				NEXT_STEP( );
			}
		}
		break;
	case SELECT_PREV_BUTTON:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				PREV_STEP( );
			}
		}
		break;
	case NEW_CHARACTER_CANCEL:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				DoModalOuter ( ID2GAMEEXTEXT ("CHARACTERSTAGE_6"), MODAL_INFOMATION, OKCANCEL, OUTER_MODAL_LOGOUT );
				Reset();
			}
		}
		break;
	case SELECT_SCHOOL_IMAGE_SG:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				m_pInfo->SetTextNoSplit ( ID2GAMEEXTEXT("SCHOOL_INFO_NAME0" ), NS_UITEXTCOLOR::WHITE);
				m_pInfo->SetVisibleSingle ( TRUE );

				if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
				{
					int select = ControlID - SELECT_SCHOOL_IMAGE_SG;
					if ( select == m_nSchool ) return;				

					SELECT_SCHOOL_PROCESS( select );	
				}
			}
		}
		break;
	case SELECT_SCHOOL_IMAGE_MP:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				m_pInfo->SetTextNoSplit ( ID2GAMEEXTEXT("SCHOOL_INFO_NAME1" ), NS_UITEXTCOLOR::WHITE);
				m_pInfo->SetVisibleSingle ( TRUE );
				if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
				{
					int select = ControlID - SELECT_SCHOOL_IMAGE_SG;
					if ( select == m_nSchool ) return;				

					SELECT_SCHOOL_PROCESS( select );	
				}
			}
		}
		break;
	case SELECT_SCHOOL_IMAGE_PH:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				m_pInfo->SetTextNoSplit ( ID2GAMEEXTEXT("SCHOOL_INFO_NAME2" ), NS_UITEXTCOLOR::WHITE );
				m_pInfo->SetVisibleSingle ( TRUE );
				if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
				{
					int select = ControlID - SELECT_SCHOOL_IMAGE_SG;
					if ( select == m_nSchool ) return;				

					SELECT_SCHOOL_PROCESS( select );	
				}
			}
		}
		break;
	case SELECT_SEX_IMAGE0:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				m_pInfo->SetTextNoSplit ( ID2GAMEEXTEXT("SEX_INFO_NAME0" ), NS_UITEXTCOLOR::WHITE );
				m_pInfo->SetVisibleSingle ( TRUE );
				if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
				{
					int select = ControlID - SELECT_SEX_IMAGE0;
					if ( select == m_nSex ) return;				

					SELECT_SEX_PROCESS( select );
				}
			}
		}
		break;
	case SELECT_SEX_IMAGE1:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				m_pInfo->SetTextNoSplit ( ID2GAMEEXTEXT("SEX_INFO_NAME1" ), NS_UITEXTCOLOR::WHITE);
				m_pInfo->SetVisibleSingle ( TRUE );
				if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
				{
					int select = ControlID - SELECT_SEX_IMAGE0;
					if ( select == m_nSex ) return;				

					SELECT_SEX_PROCESS( select );
				}
			}
		}
		break;
	case SELECT_CLASS_MEN_IMAGE0:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				m_pInfo->SetTextNoSplit ( ID2GAMEEXTEXT("CLASS_INFO_NAME0" ), NS_UITEXTCOLOR::WHITE );
				m_pInfo->SetVisibleSingle ( TRUE );
				if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
				{
					int select = ControlID - SELECT_CLASS_MEN_IMAGE0;
					if ( select == m_nClass ) return;	
				
				if ( ClassEnableCheck( select ) )	SELECT_CLASS_PROCESS( select );
				else	DoModalOuter( ID2GAMEEXTEXT ("2012_NEW_CHAR_SELECT_ENABLE_ERROR"), MODAL_ERROR, OK );	
				}
			}
		}
		break;
	case SELECT_CLASS_MEN_IMAGE1:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				m_pInfo->SetTextNoSplit ( ID2GAMEEXTEXT("CLASS_INFO_NAME1" ), NS_UITEXTCOLOR::WHITE );
				m_pInfo->SetVisibleSingle ( TRUE );
				if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
				{
					int select = ControlID - SELECT_CLASS_MEN_IMAGE0;
					if ( select == m_nClass ) return;	
				
				if ( ClassEnableCheck( select ) )	SELECT_CLASS_PROCESS( select );
				else	DoModalOuter( ID2GAMEEXTEXT ("2012_NEW_CHAR_SELECT_ENABLE_ERROR"), MODAL_ERROR, OK );	
				}
			}
		}
		break;
	case SELECT_CLASS_MEN_IMAGE2:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				m_pInfo->SetTextNoSplit ( ID2GAMEEXTEXT("CLASS_INFO_NAME2" ), NS_UITEXTCOLOR::WHITE );
				m_pInfo->SetVisibleSingle ( TRUE );
				if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
				{
					int select = ControlID - SELECT_CLASS_MEN_IMAGE0;
					if ( select == m_nClass ) return;	
				
				if ( ClassEnableCheck( select ) )	SELECT_CLASS_PROCESS( select );
				else	DoModalOuter( ID2GAMEEXTEXT ("2012_NEW_CHAR_SELECT_ENABLE_ERROR"), MODAL_ERROR, OK );	
				}
			}
		}
		break;
	case SELECT_CLASS_MEN_IMAGE3:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				m_pInfo->SetTextNoSplit ( ID2GAMEEXTEXT("CLASS_INFO_NAME3" ), NS_UITEXTCOLOR::WHITE);
				m_pInfo->SetVisibleSingle ( TRUE );
				if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
				{
					int select = ControlID - SELECT_CLASS_MEN_IMAGE0;
					if ( select == m_nClass ) return;	
				
				if ( ClassEnableCheck( select ) )	SELECT_CLASS_PROCESS( select );
				else	DoModalOuter( ID2GAMEEXTEXT ("2012_NEW_CHAR_SELECT_ENABLE_ERROR"), MODAL_ERROR, OK );	
				}
			}
		}
		break;
	case SELECT_CLASS_MEN_IMAGE4:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				m_pInfo->SetTextNoSplit ( ID2GAMEEXTEXT("CLASS_INFO_NAME4" ), NS_UITEXTCOLOR::WHITE );
				m_pInfo->SetVisibleSingle ( TRUE );
				if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
				{
					int select = ControlID - SELECT_CLASS_MEN_IMAGE0;
					if ( select == m_nClass ) return;	
				
				if ( ClassEnableCheck( select ) )	SELECT_CLASS_PROCESS( select );
				else	DoModalOuter( ID2GAMEEXTEXT ("2012_NEW_CHAR_SELECT_ENABLE_ERROR"), MODAL_ERROR, OK );	
				}
			}
		}
		break;
	case SELECT_CLASS_MEN_IMAGE5:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				m_pInfo->SetTextNoSplit ( ID2GAMEEXTEXT("CLASS_INFO_NAME5" ), NS_UITEXTCOLOR::WHITE );
				m_pInfo->SetVisibleSingle ( TRUE );
				if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
				{
					int select = ControlID - SELECT_CLASS_MEN_IMAGE0;
					if ( select == m_nClass ) return;	
				
				if ( ClassEnableCheck( select ) )	SELECT_CLASS_PROCESS( select );
				else	DoModalOuter( ID2GAMEEXTEXT ("2012_NEW_CHAR_SELECT_ENABLE_ERROR"), MODAL_ERROR, OK );	
				}
			}
		}
		break;
	case SELECT_CLASS_MEN_IMAGE6:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				m_pInfo->SetTextNoSplit ( ID2GAMEEXTEXT("CLASS_INFO_NAME6" ), NS_UITEXTCOLOR::WHITE );
				m_pInfo->SetVisibleSingle ( TRUE );
				if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
				{
					int select = ControlID - SELECT_CLASS_MEN_IMAGE0;
					if ( select == m_nClass ) return;	
				
				if ( ClassEnableCheck( select ) )	SELECT_CLASS_PROCESS( select );
				else	DoModalOuter( ID2GAMEEXTEXT ("2012_NEW_CHAR_SELECT_ENABLE_ERROR"), MODAL_ERROR, OK );	
				}
			}
		}
		break;
	case SELECT_CLASS_MEN_IMAGE7:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				m_pInfo->SetTextNoSplit ( ID2GAMEEXTEXT("CLASS_INFO_NAME7" ), NS_UITEXTCOLOR::WHITE );
				m_pInfo->SetVisibleSingle ( TRUE );
				if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
				{
					int select = ControlID - SELECT_CLASS_MEN_IMAGE0;
					if ( select == m_nClass ) return;	
				
				if ( ClassEnableCheck( select ) )	SELECT_CLASS_PROCESS( select );
				else	DoModalOuter( ID2GAMEEXTEXT ("2012_NEW_CHAR_SELECT_ENABLE_ERROR"), MODAL_ERROR, OK );	
				}
			}
		}
		break;

	case SELECT_CLASS_MEN_IMAGE8:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				m_pInfo->SetTextNoSplit ( ID2GAMEEXTEXT("CLASS_INFO_NAME8" ), NS_UITEXTCOLOR::WHITE );
				m_pInfo->SetVisibleSingle ( TRUE );
				if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
				{
					int select = ControlID - SELECT_CLASS_MEN_IMAGE0;
					if ( select == m_nClass ) return;	
				
				if ( ClassEnableCheck( select ) )	SELECT_CLASS_PROCESS( select );
				else	DoModalOuter( ID2GAMEEXTEXT ("2012_NEW_CHAR_SELECT_ENABLE_ERROR"), MODAL_ERROR, OK );	
				}
			}
		}
		break;

	case SELECT_CLASS_WOMEN_IMAGE0:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				m_pInfo->SetTextNoSplit ( ID2GAMEEXTEXT("CLASS_INFO_NAME0" ), NS_UITEXTCOLOR::WHITE );
				m_pInfo->SetVisibleSingle ( TRUE );
				if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
				{
					int select = ControlID - SELECT_CLASS_WOMEN_IMAGE0;
					if ( select == m_nClass ) return;	
				
				if ( ClassEnableCheck( select ) )	SELECT_CLASS_PROCESS( select );
				else	DoModalOuter( ID2GAMEEXTEXT ("2012_NEW_CHAR_SELECT_ENABLE_ERROR"), MODAL_ERROR, OK );	
				}
			}
		}
		break;
	case SELECT_CLASS_WOMEN_IMAGE1:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				m_pInfo->SetTextNoSplit ( ID2GAMEEXTEXT("CLASS_INFO_NAME1" ), NS_UITEXTCOLOR::WHITE );
				m_pInfo->SetVisibleSingle ( TRUE );
				if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
				{
					int select = ControlID - SELECT_CLASS_WOMEN_IMAGE0;
					if ( select == m_nClass ) return;	
				
				if ( ClassEnableCheck( select ) )	SELECT_CLASS_PROCESS( select );
				else	DoModalOuter( ID2GAMEEXTEXT ("2012_NEW_CHAR_SELECT_ENABLE_ERROR"), MODAL_ERROR, OK );	
				}
			}
		}
		break;
	case SELECT_CLASS_WOMEN_IMAGE2:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				m_pInfo->SetTextNoSplit ( ID2GAMEEXTEXT("CLASS_INFO_NAME2" ), NS_UITEXTCOLOR::WHITE );
				m_pInfo->SetVisibleSingle ( TRUE );
				if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
				{
					int select = ControlID - SELECT_CLASS_WOMEN_IMAGE0;
					if ( select == m_nClass ) return;	
				
				if ( ClassEnableCheck( select ) )	SELECT_CLASS_PROCESS( select );
				else	DoModalOuter( ID2GAMEEXTEXT ("2012_NEW_CHAR_SELECT_ENABLE_ERROR"), MODAL_ERROR, OK );	
				}
			}
		}
		break;
	case SELECT_CLASS_WOMEN_IMAGE3:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				m_pInfo->SetTextNoSplit ( ID2GAMEEXTEXT("CLASS_INFO_NAME3" ), NS_UITEXTCOLOR::WHITE);
				m_pInfo->SetVisibleSingle ( TRUE );
				if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
				{
					int select = ControlID - SELECT_CLASS_WOMEN_IMAGE0;
					if ( select == m_nClass ) return;	
				
				if ( ClassEnableCheck( select ) )	SELECT_CLASS_PROCESS( select );
				else	DoModalOuter( ID2GAMEEXTEXT ("2012_NEW_CHAR_SELECT_ENABLE_ERROR"), MODAL_ERROR, OK );	
				}
			}
		}
		break;
	case SELECT_CLASS_WOMEN_IMAGE4:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				m_pInfo->SetTextNoSplit ( ID2GAMEEXTEXT("CLASS_INFO_NAME4" ), NS_UITEXTCOLOR::WHITE );
				m_pInfo->SetVisibleSingle ( TRUE );
				if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
				{
					int select = ControlID - SELECT_CLASS_WOMEN_IMAGE0;
					if ( select == m_nClass ) return;	
				
				if ( ClassEnableCheck( select ) )	SELECT_CLASS_PROCESS( select );
				else	DoModalOuter( ID2GAMEEXTEXT ("2012_NEW_CHAR_SELECT_ENABLE_ERROR"), MODAL_ERROR, OK );	
				}
			}
		}
		break;
	case SELECT_CLASS_WOMEN_IMAGE5:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				m_pInfo->SetTextNoSplit ( ID2GAMEEXTEXT("CLASS_INFO_NAME5" ), NS_UITEXTCOLOR::WHITE );
				m_pInfo->SetVisibleSingle ( TRUE );
				if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
				{
					int select = ControlID - SELECT_CLASS_WOMEN_IMAGE0;
					if ( select == m_nClass ) return;	
				
				if ( ClassEnableCheck( select ) )	SELECT_CLASS_PROCESS( select );
				else	DoModalOuter( ID2GAMEEXTEXT ("2012_NEW_CHAR_SELECT_ENABLE_ERROR"), MODAL_ERROR, OK );	
				}
			}
		}
		break;
	case SELECT_CLASS_WOMEN_IMAGE6:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				m_pInfo->SetTextNoSplit ( ID2GAMEEXTEXT("CLASS_INFO_NAME6" ), NS_UITEXTCOLOR::WHITE );
				m_pInfo->SetVisibleSingle ( TRUE );
				if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
				{
					int select = ControlID - SELECT_CLASS_WOMEN_IMAGE0;
					if ( select == m_nClass ) return;	
				
				if ( ClassEnableCheck( select ) )	SELECT_CLASS_PROCESS( select );
				else	DoModalOuter( ID2GAMEEXTEXT ("2012_NEW_CHAR_SELECT_ENABLE_ERROR"), MODAL_ERROR, OK );	
				}
			}
		}
		break;

	case SELECT_CLASS_WOMEN_IMAGE7:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				m_pInfo->SetTextNoSplit ( ID2GAMEEXTEXT("CLASS_INFO_NAME7" ), NS_UITEXTCOLOR::WHITE );
				m_pInfo->SetVisibleSingle ( TRUE );
				if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
				{
					int select = ControlID - SELECT_CLASS_WOMEN_IMAGE0;
					if ( select == m_nClass ) return;	
				
				if ( ClassEnableCheck( select ) )	SELECT_CLASS_PROCESS( select );
				else	DoModalOuter( ID2GAMEEXTEXT ("2012_NEW_CHAR_SELECT_ENABLE_ERROR"), MODAL_ERROR, OK );	
				}
			}
		}
		break;

	case SELECT_CLASS_WOMEN_IMAGE8:
		{
			if ( CHECK_MOUSE_IN ( dwMsg ) )
			{
				m_pInfo->SetTextNoSplit ( ID2GAMEEXTEXT("CLASS_INFO_NAME8" ), NS_UITEXTCOLOR::WHITE );
				m_pInfo->SetVisibleSingle ( TRUE );
				if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
				{
					int select = ControlID - SELECT_CLASS_WOMEN_IMAGE0;
					if ( select == m_nClass ) return;	
				
				if ( ClassEnableCheck( select ) )	SELECT_CLASS_PROCESS( select );
				else	DoModalOuter( ID2GAMEEXTEXT ("2012_NEW_CHAR_SELECT_ENABLE_ERROR"), MODAL_ERROR, OK );	
				}
			}
		}
		break;

	case SELECT_LEFT_BUTTON:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSE_IN_LBDOWNLIKE ( dwMsg ) )
			{
				LEFT_BUTTON_PROCESS();
			}
			else
			{
				if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
				{
					LEFT_BUTTON_PROCESS();
				}
			}
		}
		break;
	case SELECT_RIGHT_BUTTON:
		{
			if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSE_IN_LBDOWNLIKE ( dwMsg ) )
			{
				RIGHT_BUTTON_PROCESS();
			}
			else
			{
				if ( CHECK_KEYFOCUSED ( dwMsg ) || CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
				{
					RIGHT_BUTTON_PROCESS();
				}
			}
		}
		break;
	case SELECT_FACE_LEFT:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				pLobyStage->ShiftCharFace(DxLobyStage::EM_PREV);
				STYLE_PROCESS();
			}
		}
		break;

	case SELECT_FACE_RIGHT:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				pLobyStage->ShiftCharFace(DxLobyStage::EM_NEXT);
				STYLE_PROCESS();
			}
		}
		break;

	case SELECT_HAIR_LEFT:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				pLobyStage->ShiftCharHair(DxLobyStage::EM_PREV);
				STYLE_PROCESS();
			}
		}
		break;

	case SELECT_HAIR_RIGHT:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				pLobyStage->ShiftCharHair(DxLobyStage::EM_NEXT);
				STYLE_PROCESS();
			}
		}
		break;
	case NEWCHAR_NAME_EDIT:
		{
			if ( CHECK_MOUSEIN_LBUPLIKE ( dwMsg ) )
			{
				ResetAll ();
			}
		}
		break;
	}
}

void CCreateCharacterNew::CREATE_CHAR_PROCESS()
{
	if ( m_nSchool < 0 ) 
	{
		DoModalOuter( ID2GAMEEXTEXT ("2012_NEW_CHAR_SELECT_SCHOOL_ERROR"), MODAL_ERROR, OK );
		return;
	}
	if ( m_nSex < 0 ) 
	{
		DoModalOuter( ID2GAMEEXTEXT ("2012_NEW_CHAR_SELECT_SEX_ERROR"), MODAL_ERROR, OK );
		return;
	}
	if ( m_nClass < 0 ) 
	{
		DoModalOuter( ID2GAMEEXTEXT ("2012_NEW_CHAR_SELECT_CLASS_ERROR"), MODAL_ERROR, OK );
		return;
	}
	if ( !m_pEditBox )											return;
	if ( !m_pEditBox->GetEditLength() )							return;
	CString strTemp = m_pEditBox->GetEditString ();				
	m_pEditBox->EndEdit();
	
	if ( !CheckStringName ( strTemp ) )
	{
		m_pEditBox->ClearEdit();
		return;
	}

	if ( CRanFilter::GetInstance().NameFilter ( strTemp.GetString () ) )
	{
		DoModalOuter ( ID2GAMEEXTEXT("CHARACTER_BADNAME") );
		//Reset();
		return ;
	}

	DxLobyStage *pLobyStage = DxGlobalStage::GetInstance().GetLobyStage();
	const GLCHAR_NEWINFO& sNEWINFO = pLobyStage->GetNewCharInfo ();

	if ( sNEWINFO.CHECKVALID () )
	{
		CNetClient* pNetClient = DxGlobalStage::GetInstance().GetNetClient ();
		pNetClient->SndCreateChaInfo
		(
			sNEWINFO.m_emIndex,
			sNEWINFO.m_wSchool,
			sNEWINFO.m_wFace,
			sNEWINFO.m_wHair,
			sNEWINFO.m_wHairColor,
			sNEWINFO.m_wSex,		
			strTemp.GetString ()
		);	
		DxLobyStage *pLobyStage = DxGlobalStage::GetInstance().GetLobyStage();
		pLobyStage->SetStyleStep( false );
		m_pButtonLeft->SetVisibleSingle( FALSE );
		m_pButtonRight->SetVisibleSingle( FALSE );
		m_pEditBox->EndEdit();
		m_pEditBox->ClearEdit();
		COuterInterface::GetInstance().ToSelectCharacterPage ( GetWndID () );
	}

	m_bUseCancel = FALSE;
	Reset();
}

void CCreateCharacterNew::PREV_STEP()
{
	COuterInterface::GetInstance().SetSex = FALSE;

	UIRECT Pos,newPos,newPos2,Pos1,Pos2,Pos3,Pos4,Pos5,Pos6;
	Pos = pBasicLineBoxSchool->GetGlobalPos();
	newPos = pBasicLineBoxSex->GetGlobalPos();
	pBasicLineBoxSchool->SetGlobalPos(newPos);

	newPos2 = m_pSexImg[0]->GetGlobalPos();
	// ���˹�����ͧ ���� ??
	Pos2 = m_pSchoolNameStatic->GetGlobalPos();
	// ���˹�����ͧ ������ ??
	Pos3 = m_pSchoolDec->GetGlobalPos();
	// ��˹Ѵ���������������ҡѺ���˹����
	Pos5 = Pos2; // ����
	Pos6 = Pos3; // ������
	Pos5.left = newPos.left + 5.0f;
	Pos6.left = newPos.left + 5.0f;

	for (int i = 0 ; i < 3; i++)
	{
		m_pSchoolImg[i]->SetGlobalPos(newPos2);
	}
	m_pSchoolNameStatic->SetGlobalPos(Pos5);
	m_pSchoolDec->SetGlobalPos(Pos6);

	COuterInterface::GetInstance().ToSelectCharacterPage ( GetWndID () );
	DxLobyStage *pLobyStage = DxGlobalStage::GetInstance().GetLobyStage();
	pLobyStage->SetStyleStep( false );

	m_pButtonLeft->SetVisibleSingle( FALSE );
	m_pButtonRight->SetVisibleSingle( FALSE );

	m_pEditBox->EndEdit();
	m_pEditBox->ClearEdit();

	m_pSchoolNameStatic->ClearText();
	m_pSchoolDec->ClearText();
	m_pSexNameStatic->ClearText();
	m_pClassNameStatic->ClearText();
	m_pClassDec->ClearText();

	pTextBoxFace_dec->ClearText();
	pTextBoxHair_dec->ClearText();
	pTextBoxSchool->ClearText();
	pTextBoxSex->ClearText();
	pTextBoxClass->ClearText();
	pTextBoxInfo->ClearText();

	pTextBoxSchool->SetText ( ID2GAMEEXTEXT("2012_NEW_CHAR_TEXT_SCHOOL" ), NS_UITEXTCOLOR::PALEGREEN );
	pTextBoxSex->SetText ( ID2GAMEEXTEXT("2012_NEW_CHAR_TEXT_SEX" ), NS_UITEXTCOLOR::WHITE );
	pTextBoxClass->SetText ( ID2GAMEEXTEXT("2012_NEW_CHAR_TEXT_CLASS" ), NS_UITEXTCOLOR::WHITE );
	pTextBoxInfo->SetText ( ID2GAMEEXTEXT("2012_NEW_CHAR_TEXT_INFO" ), NS_UITEXTCOLOR::WHITE );

	pBasicLineBoxSchool->SetVisibleSingle(FALSE);
	pBasicLineBoxSex->SetVisibleSingle(FALSE);
	pBasicLineBoxClass->SetVisibleSingle(FALSE);

	if(m_nSchool > -1)
	{
		m_pSelectSchoolImg[m_nSchool]->SetFadeTime( 0.01f );
		m_pSelectSchoolImg[m_nSchool]->SetFadeIn();
		m_pSelectSchoolSetImg[m_nSchool]->SetFadeTime( 0.01f );
		m_pSelectSchoolSetImg[m_nSchool]->SetFadeOut();
		m_pSchoolImg[m_nSchool]->SetVisibleSingle(FALSE);
		m_nSchool = -1;
	}

	if (m_nClass > -1)
	{	
		if (m_nSex == GENDER_MAN)
		{
			m_pSelectClassImg_men[m_nClass]->SetFadeTime( 0.01f );
			m_pSelectClassImg_men[m_nClass]->SetFadeIn();
			m_pSelectClassSetImg_men[m_nClass]->SetFadeTime( 0.01f );
			m_pSelectClassSetImg_men[m_nClass]->SetFadeOut();
			//�ͤ�?
			m_pClassImg_men[m_nClass]->SetFadeTime( 0.01f );
			m_pClassImg_men[m_nClass]->SetFadeOut();
		}
		else
		{
			m_pSelectClassImg_women[m_nClass]->SetFadeTime( 0.01f );
			m_pSelectClassImg_women[m_nClass]->SetFadeIn();
			m_pSelectClassSetImg_women[m_nClass]->SetFadeTime( 0.01f );
			m_pSelectClassSetImg_women[m_nClass]->SetFadeOut();
			//�ͤ�?
			m_pClassImg_women[m_nClass]->SetFadeTime( 0.01f );
			m_pClassImg_women[m_nClass]->SetFadeOut();
		}
		m_nClass = -1;
	}

	if (m_nSex > -1)
	{		
		m_pSelectSexImg[m_nSex]->SetFadeTime( 0.01f );
		m_pSelectSexImg[m_nSex]->SetFadeIn();
		m_pSelectSexSetImg[m_nSex]->SetFadeTime( 0.01f );
		m_pSelectSexSetImg[m_nSex]->SetFadeOut();
		m_pSexImg[m_nSex]->SetVisibleSingle(FALSE);
		m_nSex = -1;
	}
}
void CCreateCharacterNew::NEXT_STEP( )
{
	CREATE_CHAR_PROCESS();
}
void CCreateCharacterNew::LEFT_BUTTON_PROCESS( )
{
	DxLobyStage *pLobyStage = DxGlobalStage::GetInstance().GetLobyStage();
	pLobyStage->RotateChar( true );
}

void CCreateCharacterNew::RIGHT_BUTTON_PROCESS( )
{
	DxLobyStage *pLobyStage = DxGlobalStage::GetInstance().GetLobyStage();
	pLobyStage->RotateChar( false );
}

void CCreateCharacterNew::STYLE_PROCESS( )
{
	if ( m_nSchool > -1 && m_nSex > -1 && m_nClass > -1 )
	{
		DxLobyStage *pLobyStage = DxGlobalStage::GetInstance().GetLobyStage();

		const GLCHAR_NEWINFO& sNEWINFO = pLobyStage->GetNewCharInfo ();	
		const EMCHARINDEX& emCharIndex = sNEWINFO.m_emIndex;

		m_strClassHair = _T("NEW_HAIR_NAME");
		m_strClassFace = _T("NEW_FACE_NAME");

		const CString& strHairName = ID2GAMEWORD (m_strClassHair.GetString(), sNEWINFO.m_wHair );
		const CString& strFaceName = ID2GAMEWORD (m_strClassFace.GetString(), sNEWINFO.m_wFace );

		pTextBoxFace_dec->ClearText();
		pTextBoxFace_dec->SetText ( strFaceName ,NS_UITEXTCOLOR::WHITE);
		pTextBoxHair_dec->ClearText();
		pTextBoxHair_dec->SetText ( strHairName ,NS_UITEXTCOLOR::WHITE);
	}

}

void CCreateCharacterNew::SELECT_SCHOOL_PROCESS( int nSelect )
{
	if ( m_nSchool > -1 )
	{
		m_pSelectSchoolImg[m_nSchool]->SetFadeTime( 0.01f );
		m_pSelectSchoolImg[m_nSchool]->SetFadeIn();
		m_pSelectSchoolSetImg[m_nSchool]->SetFadeTime( 0.01f );
		m_pSelectSchoolSetImg[m_nSchool]->SetFadeOut();
		//�ͤ�?
		m_pSchoolImg[m_nSchool]->SetFadeTime( 0.01f );
		m_pSchoolImg[m_nSchool]->SetFadeOut();
	}
	m_nSchool = nSelect;

	m_pSelectSchoolImg[m_nSchool]->SetFadeTime( 0.01f );
	m_pSelectSchoolImg[m_nSchool]->SetFadeOut();
	m_pSelectSchoolSetImg[m_nSchool]->SetFadeTime( 0.01f );
	m_pSelectSchoolSetImg[m_nSchool]->SetFadeIn();
	//�ͤ�?
	m_pSchoolImg[m_nSchool]->SetFadeTime( 0.01f );
	m_pSchoolImg[m_nSchool]->SetFadeIn();

	pBasicLineBoxSchool->SetVisibleSingle(TRUE);

	pTextBoxSex->ClearText();
	pTextBoxSex->SetText ( ID2GAMEEXTEXT("2012_NEW_CHAR_TEXT_SEX" ), NS_UITEXTCOLOR::PALEGREEN );
	m_pSchoolNameStatic->SetText( ID2GAMEEXTEXT("2012_NEW_CHAR_SELECT_SCHOOL_NAME",m_nSchool ), NS_UITEXTCOLOR::WHITE );
	CString strTemp;
	strTemp.Format ( "2012_NEW_CHAR_SELECT_SCHOOL_DEC%d", m_nSchool );
	m_pSchoolDec->ClearText();
	m_pSchoolDec->SetText ( ID2GAMEEXTEXT(strTemp), NS_UITEXTCOLOR::WHITE );

	if ( m_nSchool > -1 && m_nSex > -1 && m_nClass > -1 )
	{
		DxLobyStage *pLobyStage = DxGlobalStage::GetInstance().GetLobyStage();
		pLobyStage->SelCharSchool( m_nSchool );
		pLobyStage->SelCharClass( GetCharIndex() );
		pLobyStage->SetStyleStep( true );
		m_pButtonLeft->SetVisibleSingle( TRUE );
		m_pButtonRight->SetVisibleSingle( TRUE );
		m_pSchoolNameStatic->SetText( ID2GAMEEXTEXT("2012_NEW_CHAR_SELECT_SCHOOL_NAME",m_nSchool ), NS_UITEXTCOLOR::WHITE );
		CString strTemp;
		strTemp.Format ( "2012_NEW_CHAR_SELECT_SCHOOL_DEC%d", m_nSchool );
		m_pSchoolDec->ClearText();
		m_pSchoolDec->SetText ( ID2GAMEEXTEXT(strTemp), NS_UITEXTCOLOR::WHITE );
	}
}
void CCreateCharacterNew::SELECT_SEX_PROCESS( int nSelect )
{
	if ( m_nSchool < 0 ) 
	{
		DoModalOuter( ID2GAMEEXTEXT ("2012_NEW_CHAR_SELECT_SCHOOL_ERROR"), MODAL_ERROR, OK );
		return;
	}
	if ( m_nSex > -1 )
	{		
		m_pSelectSexImg[m_nSex]->SetFadeTime( 0.01f );
		m_pSelectSexImg[m_nSex]->SetFadeIn();
		m_pSelectSexSetImg[m_nSex]->SetFadeTime( 0.01f );
		m_pSelectSexSetImg[m_nSex]->SetFadeOut();
		//�ͤ�?
		m_pSexImg[m_nSex]->SetFadeTime( 0.01f );
		m_pSexImg[m_nSex]->SetFadeOut();
	}

	m_nSex = nSelect;

	m_pSelectSexImg[m_nSex]->SetFadeTime( 0.01f );
	m_pSelectSexImg[m_nSex]->SetFadeOut();
	m_pSelectSexSetImg[m_nSex]->SetFadeTime( 0.01f );
	m_pSelectSexSetImg[m_nSex]->SetFadeIn();
	//�ͤ�?
	m_pSexImg[m_nSex]->SetFadeTime( 0.01f );
	m_pSexImg[m_nSex]->SetFadeIn();

	pBasicLineBoxSex->SetVisibleSingle(TRUE);

	// �����ѧ ?? ����¹���˹�
	/*UIRECT Pos,newPos,newPos2;
	//���˹�����ͧ �����ѧ ??
	Pos = pBasicLineBoxSchool->GetGlobalPos();
	//���˹�����ͧ �����ѧ �?
	newPos = pBasicLineBoxSex->GetGlobalPos();

	// �����??? ���� ᡹��硫?- ���¤�������?��?�����ѧ �?
	newPos2.left = newPos.left - newPos.sizeX;
	// ���˹�?��?���� ��ҡ�Ѻ ���˹���� �ͧ������?�?
	newPos2.top = newPos.top;
	// ��駵�?��� ��?����?��?��?
	newPos2.sizeX = Pos.sizeX;
	newPos2.sizeY = Pos.sizeY;
	
	// �?���˹����?��?�����ѧ ??
	pBasicLineBoxSchool->SetGlobalPos(newPos2);

	UIRECT Pos1,Pos2,Pos3,newPos3,newPos4,newPos5;
	// ���˹�?�����?�ͤ�???
	Pos1 = m_pSchoolImg[m_nSchool]->GetGlobalPos();
	// ���˹�����ͧ ���� ??
	Pos2 = m_pSchoolNameStatic->GetGlobalPos();
	// ���˹�����ͧ ������ ??
	Pos3 = m_pSchoolDec->GetGlobalPos();
	// ��˹Ѵ���������������ҡѺ���˹����
	newPos3 = Pos1; // �ͤ�?
	newPos4 = Pos2; // ����
	newPos5 = Pos3; // ������

	newPos3.left = newPos.left - newPos3.sizeX - 5.0f;
	newPos4.left = newPos2.left + 5.0f;
	newPos5.left = newPos2.left + 5.0f;
	for (int i = 0; i < 3; i++)
	{
		m_pSchoolImg[i]->SetGlobalPos(newPos3);
	}
	m_pSchoolNameStatic->SetGlobalPos(newPos4);
	m_pSchoolDec->SetGlobalPos(newPos5);*/

	COuterInterface::GetInstance().SetSex = TRUE;

	pTextBoxClass->ClearText();
	pTextBoxClass->AddText ( ID2GAMEEXTEXT("2012_NEW_CHAR_TEXT_CLASS" ), NS_UITEXTCOLOR::PALEGREEN );

	if (m_nClass == -1)
	{
		if ( m_nSex == GENDER_MAN )
		{
			for ( int i = 0; i < MAX_CLASS; ++i ) 
			{
				m_pSelectClassImg_men[i]->SetFadeTime( 0.01f );
				m_pSelectClassImg_men[i]->SetFadeIn();
				m_pSelectClassImg_women[i]->SetFadeTime( 0.01f );
				m_pSelectClassImg_women[i]->SetFadeOut();
			}
		}
		else
		{
			for ( int i = 0; i < MAX_CLASS; ++i ) 
			{
				m_pSelectClassImg_women[i]->SetFadeTime( 0.01f );
				m_pSelectClassImg_women[i]->SetFadeIn();
				m_pSelectClassImg_men[i]->SetFadeTime( 0.01f );
				m_pSelectClassImg_men[i]->SetFadeOut();
			}
		}
	}
	if ( m_nSex > -1 && m_nSchool > -1 && m_nClass > -1 )
	{
		DxLobyStage *pLobyStage = DxGlobalStage::GetInstance().GetLobyStage();
		pLobyStage->SelCharSchool( m_nSchool );
		pLobyStage->SelCharClass( GetCharIndex() );
		pLobyStage->SetStyleStep( false );
		m_pButtonLeft->SetVisibleSingle( FALSE );
		m_pButtonRight->SetVisibleSingle( FALSE );

		m_pSexNameStatic->SetText( ID2GAMEEXTEXT("2012_NEW_CHAR_SELECT_SEX_NAME",m_nSex ), NS_UITEXTCOLOR::WHITE );

		if ( m_nSex == GENDER_MAN )
		{
			m_pSelectClassSetImg_women[m_nClass]->SetFadeTime( 0.01f );
			m_pSelectClassSetImg_women[m_nClass]->SetFadeOut();
			//�ͤ�?
			m_pClassImg_women[m_nClass]->SetFadeTime( 0.01f );
			m_pClassImg_women[m_nClass]->SetFadeOut();
			for ( int i = 0; i < MAX_CLASS; ++i ) 
			{
				m_pSelectClassImg_men[i]->SetFadeTime( 0.01f );
				m_pSelectClassImg_men[i]->SetFadeIn();
				m_pSelectClassImg_women[i]->SetFadeTime( 0.01f );
				m_pSelectClassImg_women[i]->SetFadeOut();
			}
			pBasicLineBoxClass->SetVisibleSingle(FALSE);
			pTextBoxInfo->ClearText();
			m_pClassDec->ClearText();
			m_pClassNameStatic->ClearText();
			pTextBoxFace_dec->ClearText();
			pTextBoxHair_dec->ClearText();
			pTextBoxInfo->SetText ( ID2GAMEEXTEXT("2012_NEW_CHAR_TEXT_INFO" ), NS_UITEXTCOLOR::WHITE );
			m_nClass = -1;
		}
		else
		{		
			m_pSelectClassSetImg_men[m_nClass]->SetFadeTime( 0.01f );
			m_pSelectClassSetImg_men[m_nClass]->SetFadeOut();
			//�ͤ�?
			m_pClassImg_men[m_nClass]->SetFadeTime( 0.01f );
			m_pClassImg_men[m_nClass]->SetFadeOut();
			for ( int i = 0; i < MAX_CLASS; ++i ) 
			{
				m_pSelectClassImg_women[i]->SetFadeTime( 0.01f );
				m_pSelectClassImg_women[i]->SetFadeIn();
				m_pSelectClassImg_men[i]->SetFadeTime( 0.01f );
				m_pSelectClassImg_men[i]->SetFadeOut();
			}
			pBasicLineBoxClass->SetVisibleSingle(FALSE);
			pTextBoxInfo->ClearText();
			m_pClassDec->ClearText();
			m_pClassNameStatic->ClearText();
			pTextBoxFace_dec->ClearText();
			pTextBoxHair_dec->ClearText();
			pTextBoxInfo->SetText ( ID2GAMEEXTEXT("2012_NEW_CHAR_TEXT_INFO" ), NS_UITEXTCOLOR::WHITE );
			m_nClass = -1;
		}
	}
	m_pSexNameStatic->SetText( ID2GAMEEXTEXT("2012_NEW_CHAR_SELECT_SEX_NAME",m_nSex ), NS_UITEXTCOLOR::WHITE );
}
void CCreateCharacterNew::SELECT_CLASS_PROCESS( int nSelect )
{
	if ( m_nSex < 0 ) 
	{
		DoModalOuter( ID2GAMEEXTEXT ("2012_NEW_CHAR_SELECT_SEX_ERROR"), MODAL_ERROR, OK );
		return;
	}
	if ( m_nClass > -1 )
	{
		
		if ( m_nSex == GENDER_MAN )
		{
			m_pSelectClassImg_men[m_nClass]->SetFadeTime( 0.01f );
			m_pSelectClassImg_men[m_nClass]->SetFadeIn();
			m_pSelectClassSetImg_men[m_nClass]->SetFadeTime( 0.01f );
			m_pSelectClassSetImg_men[m_nClass]->SetFadeOut();
			//�ͤ�?
			m_pClassImg_men[m_nClass]->SetFadeTime( 0.01f );
			m_pClassImg_men[m_nClass]->SetFadeOut();
		}
		else
		{
			m_pSelectClassImg_women[m_nClass]->SetFadeTime( 0.01f );
			m_pSelectClassImg_women[m_nClass]->SetFadeIn();
			m_pSelectClassSetImg_women[m_nClass]->SetFadeTime( 0.01f );
			m_pSelectClassSetImg_women[m_nClass]->SetFadeOut();
			//�ͤ�?
			m_pClassImg_women[m_nClass]->SetFadeTime( 0.01f );
			m_pClassImg_women[m_nClass]->SetFadeOut();
		}
	}

	m_nClass = nSelect;


	DxLobyStage *pLobyStage = DxGlobalStage::GetInstance().GetLobyStage();
	pLobyStage->SelCharSchool( m_nSchool );
	pLobyStage->SelCharClass( GetCharIndex() );
	pLobyStage->SetStyleStep( true );

	m_pButtonLeft->SetVisibleSingle( TRUE );
	m_pButtonRight->SetVisibleSingle( TRUE );
	
	if ( m_nSex == GENDER_MAN )
	{
		m_pSelectClassImg_men[m_nClass]->SetFadeTime( 0.01f );
		m_pSelectClassImg_men[m_nClass]->SetFadeOut();
		m_pSelectClassSetImg_men[m_nClass]->SetFadeTime( 0.01f );
		m_pSelectClassSetImg_men[m_nClass]->SetFadeIn();
		//�ͤ�?
		m_pClassImg_men[m_nClass]->SetFadeTime( 0.01f );
		m_pClassImg_men[m_nClass]->SetFadeIn();
	}
	else
	{
		m_pSelectClassImg_women[m_nClass]->SetFadeTime( 0.01f );
		m_pSelectClassImg_women[m_nClass]->SetFadeOut();
		m_pSelectClassSetImg_women[m_nClass]->SetFadeTime( 0.01f );
		m_pSelectClassSetImg_women[m_nClass]->SetFadeIn();
		//�ͤ�?
		m_pClassImg_women[m_nClass]->SetFadeTime( 0.01f );
		m_pClassImg_women[m_nClass]->SetFadeIn();
	}

	pBasicLineBoxClass->SetVisibleSingle(TRUE);
	pTextBoxFace_dec->ClearText();
	pTextBoxHair_dec->ClearText();
	pTextBoxInfo->ClearText();
	pTextBoxInfo->SetText ( ID2GAMEEXTEXT("2012_NEW_CHAR_TEXT_INFO" ), NS_UITEXTCOLOR::PALEGREEN );
	m_pClassNameStatic->SetText( ID2GAMEEXTEXT("2012_NEW_CHAR_SELECT_CLASS_NAME",m_nClass ), NS_UITEXTCOLOR::WHITE );	

	CString strTemp;
	strTemp.Format ( "2012_NEW_CHAR_SELECT_CLASS_DEC%d", m_nClass );
	m_pClassDec->ClearText();
	m_pClassDec->SetText ( ID2GAMEEXTEXT(strTemp), NS_UITEXTCOLOR::WHITE );
}
void CCreateCharacterNew::InitData()
{
	m_pButtonLeft->SetVisibleSingle( FALSE );
	m_pButtonRight->SetVisibleSingle( FALSE );
	m_pImage_Dec_School->SetVisibleSingle( FALSE );
	m_pImage_Dec_Sex->SetVisibleSingle( FALSE );
	m_pImage_Dec_Class->SetVisibleSingle( FALSE );

	pBasicLineBoxSchool->SetVisibleSingle(FALSE);
	pBasicLineBoxSex->SetVisibleSingle(FALSE);
	pBasicLineBoxClass->SetVisibleSingle(FALSE);

	pTextBoxSchool->SetText ( ID2GAMEEXTEXT("2012_NEW_CHAR_TEXT_SCHOOL" ), NS_UITEXTCOLOR::WHITE );
	pTextBoxSex->SetText ( ID2GAMEEXTEXT("2012_NEW_CHAR_TEXT_SEX" ), NS_UITEXTCOLOR::WHITE );
	pTextBoxClass->SetText ( ID2GAMEEXTEXT("2012_NEW_CHAR_TEXT_CLASS" ), NS_UITEXTCOLOR::WHITE );
	pTextBoxInfo->SetText ( ID2GAMEEXTEXT("2012_NEW_CHAR_TEXT_INFO" ), NS_UITEXTCOLOR::WHITE );

	DxLobyStage *pLobyStage = DxGlobalStage::GetInstance().GetLobyStage();	
	pLobyStage->SetStyleStep( false );


}
int CCreateCharacterNew::GetCharIndex()
{
	if ( !m_nSex )
	{
		switch ( m_nClass ) 
		{
		case EXTREME_CLASS:	return GLCI_EXTREME_M;
		case FIGHTER_CLASS:	return GLCI_FIGHTER_M;
		case ARMS_CLASS:	return GLCI_ARMS_M;
		case ARCHER_CLASS:	return GLCI_ARCHER_M;
		case SPIRIT_CLASS:	return GLCI_SPIRIT_M;
		case SCIENCE_CLASS:	return GLCI_SCIENCE_M;
		case ASSASIN_CLASS:	return GLCI_ASSASIN_M;
		case TESTING_CLASS:	return GLCI_TESTING_M;
		case SHAPER_CLASS:	return GLCI_SHAPER_M;
		}
	}
	else
	{
		switch ( m_nClass ) 
		{
		case EXTREME_CLASS:	return GLCI_EXTREME_W;
		case FIGHTER_CLASS:	return GLCI_FIGHTER_W;
		case ARMS_CLASS:	return GLCI_ARMS_W;
		case ARCHER_CLASS:	return GLCI_ARCHER_W;
		case SPIRIT_CLASS:	return GLCI_SPIRIT_W;
		case SCIENCE_CLASS:	return GLCI_SCIENCE_W;
		case ASSASIN_CLASS:	return GLCI_ASSASIN_W;
		case TESTING_CLASS:	return GLCI_TESTING_W;
		case SHAPER_CLASS:	return GLCI_SHAPER_W;
		}
		return GLCI_FIGHTER_W;
	}
	return GLCI_FIGHTER_M;
}

void CCreateCharacterNew::SetVisibleSingle ( BOOL bVisible )
{
	CUIGroup::SetVisibleSingle( bVisible );
}

HRESULT CCreateCharacterNew::InitDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice )
{
	GASSERT( pd3dDevice );
	m_pd3dDevice = pd3dDevice;

	HRESULT hr = S_OK;
	hr = CUIGroup::InitDeviceObjects ( pd3dDevice );
	if ( FAILED ( hr ) ) return hr;

	return S_OK;
}

BOOL CCreateCharacterNew::CheckStringName( CString strTemp )
{
	strTemp = strTemp.Trim();


#ifdef TH_PARAM	

	if ( !m_pCheckString ) return FALSE;

	if ( !m_pCheckString(strTemp) )
	{
		DoModalOuter ( ID2GAMEEXTEXT ( "CREATESTAGE_9" ), MODAL_INFOMATION, OK, OUTER_MODAL_THAI_CHECK_STRING_ERROR );
		return FALSE;
	}

#endif

#ifdef VN_PARAM

	if( STRUTIL::CheckVietnamString( strTemp ) )
	{
		DoModalOuter ( ID2GAMEEXTEXT ( "CREATESTAGE_10" ), MODAL_INFOMATION, OK, OUTER_MODAL_THAI_CHECK_STRING_ERROR );
		return FALSE;
	}

#endif 

	if( STRUTIL::CheckString( strTemp ) )
	{
		DoModalOuter ( ID2GAMEEXTEXT ( "CREATE_CHARACTER_NAME_ERROR" ), MODAL_INFOMATION, OK, OUTER_MODAL_NAME_ERROR );
		return FALSE;
	}

    return TRUE;
}

void CCreateCharacterNew::ResetAll()
{
	m_pEditBox->EndEdit();
	m_pEditBox->ClearEdit();		
	if( !m_pEditBox->IsBegin() )
	{
		m_pEditBox->BeginEdit();
	}
}
void CCreateCharacterNew::Reset()
{
	COuterInterface::GetInstance().SetSex = FALSE; 

	UIRECT Pos,newPos,newPos2,Pos1,Pos2,Pos3,Pos4,Pos5,Pos6;
	Pos = pBasicLineBoxSchool->GetGlobalPos();
	newPos = pBasicLineBoxSex->GetGlobalPos();
	pBasicLineBoxSchool->SetGlobalPos(newPos);

	newPos2 = m_pSexImg[0]->GetGlobalPos();

	// ���˹�����ͧ ���� ??
	Pos2 = m_pSchoolNameStatic->GetGlobalPos();
	// ���˹�����ͧ ������ ??
	Pos3 = m_pSchoolDec->GetGlobalPos();
	// ��˹Ѵ���������������ҡѺ���˹����
	Pos5 = Pos2; // ����
	Pos6 = Pos3; // ������
	Pos5.left = newPos.left + 5.0f;
	Pos6.left = newPos.left + 5.0f;

	for (int i = 0 ; i < 3; i++)
	{
		m_pSchoolImg[i]->SetGlobalPos(newPos2);
	}
	m_pSchoolNameStatic->SetGlobalPos(Pos5);
	m_pSchoolDec->SetGlobalPos(Pos6);

	DxLobyStage *pLobyStage = DxGlobalStage::GetInstance().GetLobyStage();
	pLobyStage->SetStyleStep( false );

	m_pButtonLeft->SetVisibleSingle( FALSE );
	m_pButtonRight->SetVisibleSingle( FALSE );

	m_pEditBox->EndEdit();
	m_pEditBox->ClearEdit();

	m_pSchoolNameStatic->ClearText();
	m_pSchoolDec->ClearText();
	m_pSexNameStatic->ClearText();
	m_pClassNameStatic->ClearText();
	m_pClassDec->ClearText();

	pTextBoxFace_dec->ClearText();
	pTextBoxHair_dec->ClearText();
	pTextBoxSchool->ClearText();
	pTextBoxSex->ClearText();
	pTextBoxClass->ClearText();
	pTextBoxInfo->ClearText();

	pTextBoxSchool->SetText ( ID2GAMEEXTEXT("2012_NEW_CHAR_TEXT_SCHOOL" ), NS_UITEXTCOLOR::PALEGREEN );
	pTextBoxSex->SetText ( ID2GAMEEXTEXT("2012_NEW_CHAR_TEXT_SEX" ), NS_UITEXTCOLOR::WHITE );
	pTextBoxClass->SetText ( ID2GAMEEXTEXT("2012_NEW_CHAR_TEXT_CLASS" ), NS_UITEXTCOLOR::WHITE );
	pTextBoxInfo->SetText ( ID2GAMEEXTEXT("2012_NEW_CHAR_TEXT_INFO" ), NS_UITEXTCOLOR::WHITE );

	pBasicLineBoxSchool->SetVisibleSingle(FALSE);
	pBasicLineBoxSex->SetVisibleSingle(FALSE);
	pBasicLineBoxClass->SetVisibleSingle(FALSE);

	if( m_nSchool > -1 )
	{
		m_pSelectSchoolImg[m_nSchool]->SetFadeTime( 0.01f );
		m_pSelectSchoolImg[m_nSchool]->SetFadeIn();
		m_pSelectSchoolSetImg[m_nSchool]->SetFadeTime( 0.01f );
		m_pSelectSchoolSetImg[m_nSchool]->SetFadeOut();
		m_pSchoolImg[m_nSchool]->SetVisibleSingle(FALSE);
		m_nSchool = -1;
	}

	if ( m_nClass > -1 )
	{
		
		if ( m_nSex == GENDER_MAN )
		{
			m_pSelectClassImg_men[m_nClass]->SetFadeTime( 0.01f );
			m_pSelectClassImg_men[m_nClass]->SetFadeIn();
			m_pSelectClassSetImg_men[m_nClass]->SetFadeTime( 0.01f );
			m_pSelectClassSetImg_men[m_nClass]->SetFadeOut();
			//�ͤ�?
			m_pClassImg_men[m_nClass]->SetFadeTime( 0.01f );
			m_pClassImg_men[m_nClass]->SetFadeOut();
		}
		else
		{
			m_pSelectClassImg_women[m_nClass]->SetFadeTime( 0.01f );
			m_pSelectClassImg_women[m_nClass]->SetFadeIn();
			m_pSelectClassSetImg_women[m_nClass]->SetFadeTime( 0.01f );
			m_pSelectClassSetImg_women[m_nClass]->SetFadeOut();
			//�ͤ�?
			m_pClassImg_women[m_nClass]->SetFadeTime( 0.01f );
			m_pClassImg_women[m_nClass]->SetFadeOut();
		}
		m_nClass = -1;
	}
	if ( m_nSex > -1 )
	{		
		m_pSelectSexImg[m_nSex]->SetFadeTime( 0.01f );
		m_pSelectSexImg[m_nSex]->SetFadeIn();
		m_pSelectSexSetImg[m_nSex]->SetFadeTime( 0.01f );
		m_pSelectSexSetImg[m_nSex]->SetFadeOut();
		m_pSexImg[m_nSex]->SetVisibleSingle(FALSE);
		m_nSex = -1;
	}
}
BOOL CCreateCharacterNew::ClassEnableCheck( int nCheck )
{
	switch ( nCheck )
	{
	case 0:
		{
			if (GLCONST_CHAR::bCreateExtreme )	return TRUE;
		}break;
	case 1:
		{
			if (GLCONST_CHAR::bCreateBrawler )	return TRUE;
		}break;
	case 2:
		{
			if (GLCONST_CHAR::bCreateSwordsman )	return TRUE;
		}break;
	case 3:
		{
			if (GLCONST_CHAR::bCreateArcher )	return TRUE;
		}break;
	case 4:
		{
			if (GLCONST_CHAR::bCreateShamman )	return TRUE;
		}break;
	case 5:
		{
			if (GLCONST_CHAR::bCreateGunner )	return TRUE;
		}break;
	case 6:
		{
			if (GLCONST_CHAR::bCreateAssasin )	return TRUE;
		}break;
	case 7:
		{
			if (GLCONST_CHAR::bCreateTESTING ) return TRUE;
		}break;
	case 8:
		{
			if (GLCONST_CHAR::bCreateShaper ) return TRUE;
		}break;
	default:	return FALSE;
	};
	return FALSE;
}