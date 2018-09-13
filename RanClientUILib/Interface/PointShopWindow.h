#pragma	once

#include "UIWindowEx.h"
#include "GLItem.h"
#include "GlogicData.h"

class CItemImage;
class CBasicTextButton;
class CBasicAnimationBox;
class CBasicTextBox;
class CBasicButtonText;
class CBasicTextButton;
class CBasicTextBoxEx;
class CBasicScrollBarEx;
class CPointShopWindowMenu;
class CPointShopTicket;
class CBasicProgressBar;
//--------------------------------------------------------------------
// CPointShopWindow : //
//--------------------------------------------------------------------
class CPointShopWindow : public CUIWindowEx	
{
protected:
	enum
	{
		CHECK_CHECKLIST = ET_CONTROL_NEXT,
		CNDEV_POINTSHOP_ITEM_LIST_TEXTBOX,
		CNDEV_POINTSHOP_ITEM_LIST_SCROLLBAR,
		CNDEV_POINTSHOP_PPOINTS_BUTTON,
		CNDEV_POINTSHOP_PURCHASE_BUTTON,
		CNDEV_POINTSHOP_TOPUP_BUTTON,
		TICKET_IMAGE00,
		PRODUCT_IMAGE,

		TICKET_SLOT = 1
	};

protected:
	CPointShopTicket*	m_pTicketItemImage[TICKET_SLOT];
	CUIControl*     	m_pTicketItemImageSlot[TICKET_SLOT];
	
	CBasicAnimationBox*	m_pAniBox;
	CBasicTextButton*	m_pOkButton;
	CUIControl*			m_pBackImage;
	CUIControl*     	m_pBodyLeft;
	CUIControl*	        m_pBodyRight;
	CUIControl*			m_pSuccessImage;
	CUIControl*			m_pFailImage;

	CBasicTextBox*		m_pTextBox;

	bool	m_bOK;
	bool    m_bIsOK;
	bool	m_bResult;
	float	m_fTime;
	CBasicProgressBar*	m_pResult;
	CString	m_strResultMsg;
	CString m_strBasicMsg;
private:
	CBasicButtonText*	                CreateButtonBlue ( char* szButton, char* szTextBox, CD3DFontPar* pFont, int nAlign, UIGUID ControlID, CString strText );
	CPointShopTicket*	                CreateItemImageTicket( const char* szControl, UIGUID ControlID );
	CPointShopWindowMenu*				m_pMenu;
	CBasicTextBox*						m_pTypeText;
	CBasicTextBox*						m_pPremiumListText;
	CBasicTextBox*						m_pTitleText;
	CBasicTextBox*						m_pRateText;
	CBasicTextBox*						m_pCostText;
	CBasicTextBox*						m_pAmountText;
	CBasicTextBox*						m_pPremiumPointstText;
	CBasicTextBox*						m_pResourceText;
	CBasicTextBox*						m_pRecipeNumText[5];
	CBasicTextBox*						m_pRecipeNumText2[5];
	CBasicTextBox*						CreateStaticControl ( char* szControlKeyword, CD3DFontPar* pFont, int nAlign );

	CBasicTextBoxEx*					m_pListText;
	CBasicScrollBarEx*					m_pListScrollBar;

	
	CBasicTextBox*						m_pNotes;

	void								LoadItemList( WORD wType );
	void								HideButton();
	void								ShowButton();

	WORD								m_wType;

	int									m_nSelectIndex;

	bool								m_bRefresh;

	CItemImage*							m_pItemResultImage;
	
	CBasicTextButton*	CreateTextButton ( char* szButton, UIGUID ControlID, char* szText );
	CBasicTextButton*	m_pGenerateButton;
	CBasicTextButton*	m_pTopUpButton;
	CBasicTextButton*	m_pGenerateBtn;
	
protected:
	SNATIVEID							m_sNativeID;
	SNATIVEID							m_sTicketNativeID[5];

public:
	DWORD	m_dwNpcID;

public:
	CPointShopWindow();
	virtual	~CPointShopWindow();

public:
	VOID CreateSubControl();
	VOID CreateSubControlEx();
	VOID DoPurchaseItem();
	void Reset();
	void SetAnimation();
	bool SetBuyItem( CString strMsg, bool bSuccess = false, bool bFail = false );

protected:
	CBasicTextBox*	CreateTextBox ( const char* szTextBox );


public:
	virtual	VOID Update( INT x, INT y, BYTE LB, BYTE MB, BYTE RB, INT nScroll, FLOAT fElapsedTime, BOOL bFirstControl );
	virtual	VOID TranslateUIMessage( UIGUID ControlID, DWORD dwMsg );
	virtual void SetVisibleSingle( BOOL bVisible );
};