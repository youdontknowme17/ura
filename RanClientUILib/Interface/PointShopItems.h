#pragma	once

#include "UIWindowEx.h"
#include "GLItem.h"
#include "../RanClientLib/G-Logic/GLPointShopMan.h"
#include "GLCharData.h"
#include "GLCharacter.h"
class	CItemImage;
class	CBasicButton;
class	CBasicTextBoxEx;
class	CBasicScrollBarEx;
class	CBasicTextButton;
class	CBasicLineBox;
class	CUIEditBox;
class	GLCharacter;

class	CPointShopItems : public CUIWindowEx , public GLCHARLOGIC
{
private:
static	const int nLIMIT_CHAR;

private:
	enum
	{
		POINTSHOP_LIST_TEXTBOX = NO_ID + 1,
		POINT_SHOP_ITEMS_SLOT0,
		POINT_SHOP_ITEMS_SLOT1,
		POINT_SHOP_ITEMS_SLOT2,
		POINT_SHOP_ITEMS_SLOT3,
		POINT_SHOP_ITEMS_SLOT4,
		POINT_SHOP_ITEMS_SLOTR,
		//ITEMMIX_LIST_SCROLLBAR,
		POINTSHOP_CATEGORY_BUTTON0,
		POINTSHOP_CATEGORY_BUTTON1,
		POINTSHOP_CATEGORY_BUTTON2,
		POINTSHOP_CATEGORY_BUTTON3,
		POINTSHOP_CATEGORY_BUTTON4,
		POINTSHOP_CATEGORY_BUTTON5,
		POINTSHOP_CATEGORY_BUTTON6,
		POINTSHOP_CATEGORY_BUTTON7,
		POINTSHOP_CATEGORY_BUTTON8,
		POINTSHOP_CATEGORY_BUTTON9,
		POINTSHOP_CATEGORY_BUTTON10,
		POINTSHOP_CATEGORY_BUTTON11,
		POINTSHOP_CATEGORY_BUTTON12,
		//ITEMMIX_MONEY_BUTTON,
	};


public:
	CPointShopItems ();
	virtual ~CPointShopItems ();

public:
	void	CreateSubControl ();
	CBasicTextButton*	CreateTextButton14 ( char* szButton, UIGUID ControlID , char* szText );	

public:
	
	DWORD	GetItemID() { return m_nItemID; }
	BOOL	IsSelectedIndex();
	void	LoadPointShopList ();

public:
	int		m_nIndex;
	void	SetItemIndex ( int nIndex )				{ m_nIndex = nIndex; }

private:
	BOOL	GetPointShop ( const DWORD nItemID );

	SSHOPDATA sShopData;
public:
	virtual	void SetVisibleSingle ( BOOL bVisible );

public:
	virtual HRESULT InitDeviceObjects ( LPDIRECT3DDEVICEQ pd3dDevice );
	virtual	void	Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void	TranslateUIMessage ( UIGUID cID, DWORD dwMsg );
	void	FlipBot ( int nCategory );
private:
	void	LoadPointShopItems ( const POINT_SHOP& sPointShop );

private:
	int		m_nSelectIndex;
	CString	m_strSelectName;

private:
	CBasicTextBoxEx*	m_pListText;
	CBasicScrollBarEx*	m_pListScrollBar;
	//CUIControl*			m_pBackImage;
	DWORD				m_nItemID;
	DWORD				m_nCategory;
	POINT_SHOP			pPointShop;
	//CBasicTextButton*	m_pNote;

	BOOL				m_bINIT;

	enum
	{
		ITEM = 0
	};
	
protected:
	CBasicTextBox*	CreateTextBox ( const char* szTextBox );
	CBasicButton*		m_pCategoryButton[POINTSHOP_CATEGORYNUM];
	CBasicTextBox*		m_pMoneyTextBox;
	CBasicTextBox*		m_pRateTextBox;
	CItemImage*			m_pTicketItemImage[POINTSHOP_ITEMNUM];
	CBasicTextBox*		m_pTicketItemNum[POINTSHOP_ITEMNUM];
	CItemImage*			m_pProductItemImage;
	CBasicLineBox*		m_pTicketItemNumBackImage[POINTSHOP_ITEMNUM];

public:
	//SFRIEND & GetFriendInfo() { return m_sFriend; }
	
	void	SetMoney ( LONGLONG Money );
	void	SetRate ( DWORD Rate );
};
