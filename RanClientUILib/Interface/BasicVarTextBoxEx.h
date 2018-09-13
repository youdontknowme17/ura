#pragma	once

#include "../EngineUIlib/GUInterface/UIGroup.h"
#include "GLItem.h"
#include "../EngineUIlib/GUInterface/BasicTextBox.h"

class	CBasicButton;
class	CBasicTextBox;
class	CBasicTextBoxEx;
class	CBasicLineBoxEx;
class	CItemImage;
class	CD3DFontPar;
class	GLItem;

struct	SNATIVEID;

const DWORD UIMSG_UIVARTEXT_BUTTON_CLOSE_CLICK = UIMSG_USER1;
const DWORD UIMSG_UIVARTEXT_BUTTON_CLOSE_MOUSEIN = UIMSG_USER2;

class	CBasicVarTextBoxEx : public CUIGroup
{
protected:
	enum
	{
		BUTTON_CLOSE = NO_ID + 1,
	};

private:
static	const	float	fMOUSEPOINT_GAP;

public:
	CBasicVarTextBoxEx ();
	virtual	~CBasicVarTextBoxEx ();

public:
	void	CreateSubControl ();

	int		AddTextNoSplit ( CString strText, D3DCOLOR dwColor );
	int		SetTextNoSplit ( CString strText, D3DCOLOR dwColor );
	int		AddText ( CString strText, D3DCOLOR dwColor );
	int		GetCount ();

	float	GetLongestLine ();

	void	ClearText ();
	void	SetText( CString strText, D3DCOLOR	dwColor );
	void	AddString ( int nIndex, const CString& strText, const D3DCOLOR& dwColor );
	void	AddTextLongestLineSplit ( CString strText, D3DCOLOR dwColor );
	void	SetUseOverColor ( bool bUseColor );
	void	SetOverColor ( const D3DCOLOR& dwColor );
	void	SetLineInterval ( const float fLineInterval );
	void	RePosControl ( int x, int y );
	void	SetBlockMouseTracking ( bool bBlock );
	void	SetMousePointGap ( D3DXVECTOR2 vGap );
	void	SetTextAlign ( int nALIGN );

	virtual	void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID cID, DWORD dwMsg );

private:
	bool	m_bBLOCK_MOUSETRACKING;

	CBasicTextBoxEx*	m_pTextBox;

	CBasicLineBoxEx*	m_pLineBox;
	CBasicLineBoxEx*	m_pLineBoxGray;

	D3DXVECTOR2			m_vMousePointGap;

	CUIControl*			m_pSelfDummy;

	CD3DFontPar*		m_pFont;
	CD3DFontPar*		m_pFont12;
	
	CBasicButton*		m_pCloseButton;
	CUIControl*			m_pItemWrap;//add wrap ejsayaaa

public:


	void SetItemRender ( SNATIVEID sICONINDEX, const char* szTexture, SITEMCUSTOM sCustom );
	void ResetItemRender ();
	void SetItemInRandomBox ( SNATIVEID sICONINDEX, const char* szTexture, int nIndex );
	void ResetItemInRandomBox ();
	void SetItemInBox ( SNATIVEID sICONINDEX, const char* szTexture, int nIndex );
	void ResetItemInBox ();
	void ShowCloseButton( bool bSHOW );

	int	 AddTextNoSplitBig ( CString strText, D3DCOLOR dwColor );

private:
	CItemImage*	CreateItemImage ( const char* szControl );
	CItemImage*	m_pBoxImg[ITEM::SBOX::ITEM_SIZE];
	CItemImage*	m_pRandBoxImg[30];
	CItemImage*			m_pItemImage; 
	CBasicTextBox*		m_pTextBoxName; 

public:
	BOOL m_bCLOSE;
	void SetClose(BOOL bCLOSE) { m_bCLOSE = bCLOSE; }
	BOOL IsClose() { return m_bCLOSE; }
	
public:
	int		AddMultiText ( CBasicTextBox::MULTITEXT mlText ); //Add MultiText by NaJ
	int		AddMultiTextLongestLineSplit ( CBasicTextBox::MULTITEXT mlText ); //Add MultiTextLongestLine by NaJ

};