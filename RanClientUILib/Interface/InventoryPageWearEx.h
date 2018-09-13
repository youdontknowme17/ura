#pragma	once

#include "../EngineUILib/GUInterface/UIGroup.h"
#include "GLItem.h"

////////////////////////////////////////////////////////////////////
//	����� �޽��� ����
const DWORD UIMSG_MOUSEIN_WEARSLOTEX = UIMSG_USER1;
////////////////////////////////////////////////////////////////////

class CItemImage;
class CBasicTextButton;
class	CBasicTextBox;
class	CBasicVarTextBox;
class	CBasicButton;

class	CInventoryPageWearEx : public CUIGroup
{
private:
static	const	int	nOUTOFRANGE;

private:
	enum
	{
		ITEM_IMAGE0 = NO_ID + 1,
		ITEM_IMAGE1,
		ITEM_IMAGE2,
		ITEM_IMAGE3,
		ITEM_IMAGE4,
		ITEM_IMAGE5,
		ITEM_IMAGE6,
		ITEM_IMAGE7,
		ITEM_IMAGE8,
		ITEM_IMAGE9,
		ITEM_IMAGE10,
		ITEM_IMAGE11,
		ITEM_IMAGE12,
		ITEM_IMAGE13,

		ITEM_IMAGE14,

		ITEM_IMAGE15,

		ITEM_IMAGE16,
		ITEM_IMAGE17,
		ITEM_IMAGE18,
		ITEM_IMAGE19,

		ITEM_IMAGE_SIZE = 20,


		TEXT_A_BUTTON,
		TEXT_B_BUTTON,

		SWITCH_BUTTONA,
		SWITCH_BUTTONB
	};

public:
	CInventoryPageWearEx ();
	virtual	~CInventoryPageWearEx ();

public:
	void	CreateSubControl ( int nClassType );

private:
	CUIControl*	CreateControl ( const char* szControl );
	CItemImage*	CreateItemImage ( const char* szControl, UIGUID ControlID );

	void	CreateNumberBox ();



public:
	void	SetNumber ( int nIndex, int nNumber, int nMaxNumber );
	void	SetNumber2 ( int nIndex, int nNumber );
	void	SetNumber3 ( int nIndex, int nNumber );
	void	ResetNumber ( int nIndex );

public:
	void	SetItemIndex ( int nIndex )				{ m_nIndex = nIndex; }
	int		GetItemIndex ()							{ return m_nIndex; }

	EMSLOT	GetItemSlot ();
	BOOL	m_bRingSwap;

public:
	void	LoadItem ( int nIndex, SITEMCUSTOM& ref_sItemCustom );
	SITEMCUSTOM&	GetItem ( int nIndex );
	void	UnLoadItem ( int nIndex );

	void SetTabButton( BOOL bMain ); // ���� ����

public:
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );

public:
	EMSLOT	IMAGE2EMSLOT ( int nIndex );

private:
	CUIControl*		m_pMouseOver;

	CBasicTextButton* m_pAButton;
	CBasicTextButton* m_pBButton;

	CUIControl*		m_pARHandSlot;
	CUIControl*		m_pALHandSlot;
	CUIControl*		m_pBRHandSlot;
	CUIControl*		m_pBLHandSlot;

//	BOOL	m_bRingSwap;
	BOOL	m_bEarringSwap;

	CUIControl*	m_Button[2];
	CUIControl*	m_ButtonSet[2];

	void	SWITCH1();
	void	SWITCH2();

private:
	CUIControl*	m_pSlotDisplay[ITEM_IMAGE_SIZE];
	CItemImage*	m_pItemImage[ITEM_IMAGE_SIZE];
	SITEMCUSTOM	m_ItemCustomArray[ITEM_IMAGE_SIZE];
	CBasicTextBox* m_pNumberBoxArray[ITEM_IMAGE_SIZE];

private:
	int		m_nIndex;	

private:
	CBasicTextBox*	CreateNumberBox ( const char* szControl );

};