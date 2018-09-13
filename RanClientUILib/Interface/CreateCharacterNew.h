#pragma	once

#include "../EngineUILib/GUInterface/UIGroup.h"
#include "CharacterEnum.h"

const DWORD UIMSG_BASICINFOVIEW_LBDUP = UIMSG_USER1;

class CBasicTextButton;
class CBasicButton;
class CBasicTextBox;
class CUIEditBox;
class CBasicVarTextBox;
class CBasicLineBox;


class	CCreateCharacterNew : public CUIGroup
{
private:
	static	const int nLIMITCHAR;
	enum
	{
		SELECT_PREV_BUTTON = NO_ID + 1,
		SELECT_NEXT_BUTTON,
		SELECT_LEFT_BUTTON,
		SELECT_RIGHT_BUTTON,
		SELECT_SCHOOL_NAME,
		SELECT_SCHOOL_DEC,
		SELECT_SEX_NAME,
		SELECT_CLASS_NAME,
		SELECT_CLASS_DEC,
		SELECT_SCHOOL_IMAGE_SG,
		SELECT_SCHOOL_IMAGE_MP,
		SELECT_SCHOOL_IMAGE_PH,
		SELECT_SCHOOL_SET_IMAGE_SG,
		SELECT_SCHOOL_SET_IMAGE_MP,
		SELECT_SCHOOL_SET_IMAGE_PH,
		SELECT_SEX_IMAGE0,
		SELECT_SEX_IMAGE1,
		SELECT_SEX_SET_IMAGE0,
		SELECT_SEX_SET_IMAGE1,
		SELECT_CLASS_MEN_IMAGE0,
		SELECT_CLASS_MEN_IMAGE1,
		SELECT_CLASS_MEN_IMAGE2,
		SELECT_CLASS_MEN_IMAGE3,
		SELECT_CLASS_MEN_IMAGE4,
		SELECT_CLASS_MEN_IMAGE5,
		SELECT_CLASS_MEN_IMAGE6,
		SELECT_CLASS_MEN_IMAGE7,
		SELECT_CLASS_MEN_IMAGE8,
		SELECT_CLASS_SET_MEN_IMAGE0,
		SELECT_CLASS_SET_MEN_IMAGE1,
		SELECT_CLASS_SET_MEN_IMAGE2,
		SELECT_CLASS_SET_MEN_IMAGE3,
		SELECT_CLASS_SET_MEN_IMAGE4,
		SELECT_CLASS_SET_MEN_IMAGE5,
		SELECT_CLASS_SET_MEN_IMAGE6,
		SELECT_CLASS_SET_MEN_IMAGE7,
		SELECT_CLASS_SET_MEN_IMAGE8,
		SELECT_CLASS_WOMEN_IMAGE0,
		SELECT_CLASS_WOMEN_IMAGE1,
		SELECT_CLASS_WOMEN_IMAGE2,
		SELECT_CLASS_WOMEN_IMAGE3,
		SELECT_CLASS_WOMEN_IMAGE4,
		SELECT_CLASS_WOMEN_IMAGE5,
		SELECT_CLASS_WOMEN_IMAGE6,
		SELECT_CLASS_WOMEN_IMAGE7,
		SELECT_CLASS_WOMEN_IMAGE8,
		SELECT_CLASS_SET_WOMEN_IMAGE0,
		SELECT_CLASS_SET_WOMEN_IMAGE1,
		SELECT_CLASS_SET_WOMEN_IMAGE2,
		SELECT_CLASS_SET_WOMEN_IMAGE3,
		SELECT_CLASS_SET_WOMEN_IMAGE4,
		SELECT_CLASS_SET_WOMEN_IMAGE5,
		SELECT_CLASS_SET_WOMEN_IMAGE6,
		SELECT_CLASS_SET_WOMEN_IMAGE7,
		SELECT_CLASS_SET_WOMEN_IMAGE8,
		ICON_SCHOOL_IMAGE_SG,
		ICON_SCHOOL_IMAGE_MP,
		ICON_SCHOOL_IMAGE_PH,
		ICON_SEX_IMAGE_MEN,
		ICON_SEX_IMAGE_WOMEN,
		ICON_CLASS_MEN_IMAGE0,
		ICON_CLASS_MEN_IMAGE1,
		ICON_CLASS_MEN_IMAGE2,
		ICON_CLASS_MEN_IMAGE3,
		ICON_CLASS_MEN_IMAGE4,
		ICON_CLASS_MEN_IMAGE5,
		ICON_CLASS_MEN_IMAGE6,
		ICON_CLASS_MEN_IMAGE7,
		ICON_CLASS_MEN_IMAGE8,
		ICON_CLASS_WOMEN_IMAGE0,
		ICON_CLASS_WOMEN_IMAGE1,
		ICON_CLASS_WOMEN_IMAGE2,
		ICON_CLASS_WOMEN_IMAGE3,
		ICON_CLASS_WOMEN_IMAGE4,
		ICON_CLASS_WOMEN_IMAGE5,
		ICON_CLASS_WOMEN_IMAGE6,
		ICON_CLASS_WOMEN_IMAGE7,
		ICON_CLASS_WOMEN_IMAGE8,
		SELECT_FACE_LEFT,
		SELECT_FACE_RIGHT,
		SELECT_HAIR_LEFT,
		SELECT_HAIR_RIGHT,
		NEWCHAR_NAME_EDIT,
		NEW_CHARACTER_NEW,
		NEW_CHARACTER_CANCEL,
	
	};

private:
	CBasicTextButton*	m_pButtonPrev;
	CBasicTextButton*	m_pButtonNext;
	CBasicTextButton*	m_pButtonNew;
	CBasicTextButton*	m_pButtonCancel;
	CBasicButton*		m_pButtonLeft;
	CBasicButton*		m_pButtonRight;
	CBasicTextBox*		m_pSchoolNameStatic;	
	CBasicTextBox*		m_pSchoolDec;
	CBasicTextBox*		m_pSexNameStatic;	
	CBasicTextBox*		m_pClassNameStatic;	
	CBasicTextBox*		m_pClassDec;
	CBasicTextBox*		pTextBoxFace;
	CBasicTextBox*		pTextBoxHair;
	CBasicTextBox*		pTextBoxFace_dec;
	CBasicTextBox*		pTextBoxHair_dec;
	CBasicButton*		m_pButtonLeft_face;
	CBasicButton*		m_pButtonRight_face;
	CBasicButton*		m_pButtonLeft_hair;
	CBasicButton*		m_pButtonRight_hair;	
	CBasicVarTextBox*	m_pInfo;
	CBasicLineBox*		pBasicLineBoxSchool;
	CBasicLineBox*		pBasicLineBoxSex;
	CBasicLineBox*		pBasicLineBoxClass;

	int mapSchool;
	int	m_nOverMenu;

	CUIControl*			m_pDummyImage;
	CBasicTextBox*		m_pStageTitle;
	CBasicTextBox*		m_pStageDec;

	CUIEditBox*			m_pEditBox;

	int					m_nSchool;
	int					m_nSex;
	int					m_nClass;
	int					m_nHair;
	int					m_nFace;
	int					m_nStart;
	BOOL				m_bCreateExtream;
	BOOL				m_bUseCancel;

	LPDIRECT3DDEVICEQ	m_pd3dDevice;

	CString m_strClassHair;
	CString m_strClassFace;

	USHORT m_uCharRemain;

public:
	CBasicLineBox*		m_pImage_Back_Set;

	CUIControl*			m_pBackImage;

	CUIControl*			m_pImage_Logo;
	CUIControl*			m_pImage_Back_School;
	CUIControl*			m_pImage_Back_Sex;
	CUIControl*			m_pImage_Back_Class;
	CUIControl*			m_pImage_Back_info;
	CUIControl*			m_pImage_Dec_School;
	CUIControl*			m_pImage_Dec_Sex;
	CUIControl*			m_pImage_Dec_Class;

	CBasicTextBox*		pTextBoxSchool;
	CBasicTextBox*		pTextBoxSex;
	CBasicTextBox*		pTextBoxClass;
	CBasicTextBox*		pTextBoxInfo;

	CBasicLineBox*	   m_pBasicLineBox;
	CBasicLineBox*	   m_pBasicLineBoxServer;

	CUIControl*			m_pSelectSchoolImg[MAX_SELSCHOOL];
	CUIControl*			m_pSelectSchoolSetImg[MAX_SELSCHOOL];
	CUIControl*			m_pSelectSexImg[MAX_SEX];
	CUIControl*			m_pSelectSexSetImg[MAX_SEX];
	CUIControl*			m_pSelectClassImg_men[MAX_CLASS];
	CUIControl*			m_pSelectClassSetImg_men[MAX_CLASS];
	CUIControl*			m_pSelectClassImg_women[MAX_CLASS];
	CUIControl*			m_pSelectClassSetImg_women[MAX_CLASS];

	CUIControl*			m_pSchoolImg[MAX_SELSCHOOL];
	CUIControl*			m_pSexImg[MAX_SEX];
	CUIControl*			m_pClassImg_men[MAX_CLASS];
	CUIControl*			m_pClassImg_women[MAX_CLASS];

	CCreateCharacterNew ();
	virtual	~CCreateCharacterNew ();

	CBasicLineBox*	GetBasicLineSchool()	{ return pBasicLineBoxSchool; }
	CBasicLineBox*	GetBasicLineSex()		{ return pBasicLineBoxSex; }
	CUIControl*		GetIconSex( int i )		{ return m_pSexImg[i]; }
	CUIControl*		GetIconSchool( int i )	{ return m_pSchoolImg[i]; }
	CBasicTextBox*	GetSchoolName()			{ return m_pSchoolNameStatic; }
	CBasicTextBox*  GetSchoolInfo()			{ return m_pSchoolDec; }

	void	InitData();
	
	BOOL	ClassEnableCheck( int nCheck ); //enable checking if creation is allowed

	void	CREATE_CHAR_PROCESS();

    void	LEFT_BUTTON_PROCESS( );
	void	RIGHT_BUTTON_PROCESS( );
	void	PREV_STEP( );
	void	NEXT_STEP( );

	void	SELECT_SCHOOL_PROCESS( int nSelect );

	void	STYLE_PROCESS();
	
	int		GetSelectSchool() { return m_nSchool; }
	void	SetSelectSchool( int nSchool) { m_nSchool = nSchool; }

	void	SELECT_SEX_PROCESS( int nSelect );
	
	int		GetSelectSex() { return m_nSex; }
	void	SetSelectSex(int nSex) { m_nSex = nSex; }


	void	SELECT_CLASS_PROCESS( int nSelect );
	
	int		GetSelectClass() { return m_nClass; }
	void	SetSelectClass( int nClass) { m_nClass = nClass; }

	int		GetCharIndex();

	CUIEditBox*		GetEditBox() { return m_pEditBox; }

	void	SELECT_STYLE_PROCESS( int nSelect );
	
	BOOL	CheckStringName( CString strTemp );
	
	void UseCancelButton()				{ m_bUseCancel = TRUE; }

	void ResetAll();
	void Reset();

private:
	HMODULE m_hCheckStrDLL;
	BOOL (_stdcall *m_pCheckString)(CString);


public:
	void	CreateSubControl ();
	CBasicButton*	CreateFlipButton ( char* szButton, char* szButtonFlip, UIGUID ControlID );
public:
	virtual void Update( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void SetVisibleSingle ( BOOL bVisible );
	virtual HRESULT InitDeviceObjects( LPDIRECT3DDEVICEQ pd3dDevice );
protected:
	
};
