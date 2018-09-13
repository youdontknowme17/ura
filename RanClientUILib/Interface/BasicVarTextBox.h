#pragma	once

#include "../EngineUIlib/GUInterface/UIGroup.h"
#include "../EngineUIlib/GUInterface/BasicTextBox.h"
#include "GLItem.h"
class	CBasicTextBox;
class	CBasicTextBoxEx;
class	CBasicLineBoxEx;
class	CItemImage;
class	CD3DFontPar;
class	GLItem;

struct	SNATIVEID;

class	CBasicVarTextBox : public CUIGroup
{

private:
static	const	float	fMOUSEPOINT_GAP;

public:
	CBasicVarTextBox ();
	virtual	~CBasicVarTextBox ();

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

private:
	bool	m_bBLOCK_MOUSETRACKING;

	CBasicTextBoxEx*	m_pTextBox; 

	CBasicLineBoxEx*	m_pLineBox;
	CBasicLineBoxEx*	m_pLineBoxGray;

	D3DXVECTOR2			m_vMousePointGap;

	CUIControl*			m_pSelfDummy;

	CD3DFontPar*		m_pFont;
	CD3DFontPar*		m_pFont12;


public:
	void SetItemRender ( SNATIVEID sICONINDEX, const char* szTexture, SITEMCUSTOM sCustom );
	void ResetItemRender ();
	void SetItemInRandomBox ( SNATIVEID sICONINDEX, const char* szTexture, int nIndex );
	void ResetItemInRandomBox ();
	void SetItemBoxRandomRender ( SNATIVEID sICONINDEX, const char* szTexture, int nIndex );
	void ResetItemBoxRandomRender ();
	void SetItemInBox ( SNATIVEID sICONINDEX, const char* szTexture, int nIndex );
	void ResetItemInBox ();

	int	 AddTextNoSplitBig ( CString strText, D3DCOLOR dwColor );

private:
	CItemImage*	CreateItemImage ( const char* szControl );
	CItemImage*	m_pBoxImg[ITEM::SBOX::ITEM_SIZE];
	CItemImage*	m_pRandBoxImg[30];
	CItemImage*			m_pItemImage; 
	CBasicTextBox*		m_pTextBoxName; 

	BOOL m_bLONG;
	BOOL IsLong()	{ return m_bLONG; }
	void SetLong(BOOL bLONG) { m_bLONG = bLONG; }
	
public:
	int		AddMultiText ( CBasicTextBox::MULTITEXT mlText ); //Add MultiText by NaJ
	int		AddMultiTextLongestLineSplit ( CBasicTextBox::MULTITEXT mlText ); //Add MultiTextLongestLine by NaJ

};