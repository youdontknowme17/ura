#pragma	once

#include "../EngineUILib/GUInterface/UIGroup.h"

class	CBasicTextBox;
class	CBasicButton;
class	CD3DFontPar;
class	CBasicTextButton;
class	CBasicButtonText;

class	CBasicUIOption : public CUIGroup
{
protected:
	enum
	{
		HWOPTION_UI_SIMPLEHP_BUTTON = NO_ID + 1,
		HWOPTION_UI_MOVABLE_ON_CHAT_BUTTON,
		HWOPTION_UI_VIEW_DAMAGE_BUTTON,
		HWOPTION_UI_VIEW_NAME_DAMAGE_BUTTON,
		HWOPTION_UI_DEFAULTPOS_BUTTON
	};

public:
	CBasicUIOption ();
	virtual	~CBasicUIOption ();

public:
	void	CreateSubControl ();

private:
	CBasicButton*	CreateFlipButton ( char* szButton, char* szButtonFlip, UIGUID ControlID );
	CBasicTextBox*	CreateStaticControl ( char* szControlKeyword, CD3DFontPar* pFont, D3DCOLOR D3DCOLOR, int nAlign );
	CBasicTextButton*	CreateTextButton24 ( const char* szButton, UIGUID ControlID, const char* szText );

public:
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void SetVisibleSingle ( BOOL bVisible );

public:
	void	LoadCurrentOption();

private:
	CBasicButton*		m_pSimpleHPButton;
	CBasicButton*		m_pMovableOnChatButton;
	CBasicButton*		m_pUserDamageButton;
	CBasicButton*		m_pNameDamageButton;

	CBasicTextButton*	m_pDefaultPosButton;

private:
	CD3DFontPar*	m_pFont;	

private:
	void	LoadSimpleHP ();
	void	LoadMovableOnChat ();

public:
	BOOL m_bSimpleHP;
	BOOL m_bMovableOnChat;
};