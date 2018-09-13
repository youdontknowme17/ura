// ItemProperty1.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ItemEdit.h"
#include "ItemParent.h"
#include "GLOGIC.h"

#include "DxEffcharData.h"
#include "DxSkinPieceContainer.h"
#include "DxSkinPieceRootData.h"//add abl
#include "DxSimpleMeshMan.h"
#include "EtcFunction.h"

#include "SheetWithTab.h"

#include "GLStringTable.h"
#include ".\itemparent.h"

#include "GLPet.h"
#include "GLVEHICLE.h"
#include "GLSummon.h"

// CItemParent ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CItemParent, CPropertyPage)
CItemParent::CItemParent( LOGFONT logfont )
	: CPropertyPage(CItemParent::IDD),
	m_pFont ( NULL )
{
	m_bDlgInit = FALSE;
	m_pItem = NULL;
	m_pDummyItem = new SITEM;

	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CItemParent::~CItemParent()
{
	SAFE_DELETE ( m_pDummyItem );
	SAFE_DELETE ( m_pFont );
}

void CItemParent::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);	
}


BEGIN_MESSAGE_MAP(CItemParent, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnBnClickedButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnBnClickedButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnBnClickedButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_INVENFILE, OnBnClickedButtonInvenfile)
	ON_BN_CLICKED(IDC_BUTTON_FIELDFILE, OnBnClickedButtonFieldfile)
	
	ON_BN_CLICKED(IDC_BUTTON_WEARINGFILE_C1, OnBnClickedButtonWearingfileC1)
	ON_BN_CLICKED(IDC_BUTTON_WEARINGFILE_C2, OnBnClickedButtonWearingfileC2)
	ON_BN_CLICKED(IDC_BUTTON_WEARINGFILE_C3, OnBnClickedButtonWearingfileC3)
	ON_BN_CLICKED(IDC_BUTTON_WEARINGFILE_C4, OnBnClickedButtonWearingfileC4)
	ON_BN_CLICKED(IDC_BUTTON_WEARINGFILE_C5, OnBnClickedButtonWearingfileC5)
	ON_BN_CLICKED(IDC_BUTTON_WEARINGFILE_C6, OnBnClickedButtonWearingfileC6)
	ON_BN_CLICKED(IDC_BUTTON_WEARINGFILE_C7, OnBnClickedButtonWearingfileC7)
	ON_BN_CLICKED(IDC_BUTTON_WEARINGFILE_C8, OnBnClickedButtonWearingfileC8)
	ON_BN_CLICKED(IDC_BUTTON_WEARINGFILE_C9, OnBnClickedButtonWearingfileC9)
	ON_BN_CLICKED(IDC_BUTTON_WEARINGFILE_C80, OnBnClickedButtonWearingfileC10)
	ON_BN_CLICKED(IDC_BUTTON_WEARINGFILE_C12, OnBnClickedButtonWearingfileC12)
	ON_BN_CLICKED(IDC_BUTTON_WEARINGFILE_C81, OnBnClickedButtonWearingfileC13)
	ON_BN_CLICKED(IDC_BUTTON_WEARINGFILE_C13, OnBnClickedButtonWearingfileC14)
	ON_BN_CLICKED(IDC_BUTTON_WEARINGFILE_C82, OnBnClickedButtonWearingfileC15)
	ON_BN_CLICKED(IDC_BUTTON_WEARINGFILE_C14, OnBnClickedButtonWearingfileC16)
	ON_BN_CLICKED(IDC_BUTTON_WEARINGFILE_C83, OnBnClickedButtonWearingfileC17)
	ON_BN_CLICKED(IDC_BUTTON_WEARINGFILE_C11, OnBnClickedButtonWearingfileC11)

	ON_BN_CLICKED(IDC_BUTTON_SKIN_EFFECTFILE, OnBnClickedButtonSkinEffectfile)
	ON_BN_CLICKED(IDC_BUTTON_EFFECTFILE, OnBnClickedButtonEffectfile)
	ON_BN_CLICKED(IDC_BUTTON_SKIN_TAR_EFFECTFILE, OnBnClickedButtonSkinTarEffectfile)
	ON_EN_CHANGE(IDC_EDIT_ITEMNAME, OnEnChangeEditItemname)
	ON_EN_CHANGE(IDC_EDIT_ITEMDESC, OnEnChangeEditItemdesc)

	ON_CBN_SELCHANGE(IDC_COMBO_ITEM_TYPE, OnCbnSelchangeComboItemType)
	ON_BN_CLICKED(IDC_RADIO_M, OnBnClickedRadioM)
	ON_BN_CLICKED(IDC_RADIO_W, OnBnClickedRadioW)
	ON_BN_CLICKED(IDC_RADIO_ALL, OnBnClickedRadioAll)
	ON_BN_CLICKED(IDC_BUTTON_RANDOM_OPT, OnBnClickedButtonRandomOpt)
	ON_BN_CLICKED(IDC_BUTTON_BR, OnBnClickedButtonBr)
	ON_BN_CLICKED(IDC_BUTTON_SW, OnBnClickedButtonSw)
	ON_BN_CLICKED(IDC_BUTTON_AR, OnBnClickedButtonAr)
	ON_BN_CLICKED(IDC_BUTTON_SH, OnBnClickedButtonSh)
	ON_BN_CLICKED(IDC_BUTTON_EX, OnBnClickedButtonEx)
	ON_BN_CLICKED(IDC_BUTTON_GU, OnBnClickedButtonGu)
	ON_BN_CLICKED(IDC_BUTTON_AS, OnBnClickedButtonAs)
	ON_BN_CLICKED(IDC_BUTTON_TE, OnBnClickedButtonTe)
	ON_BN_CLICKED(IDC_BUTTON_X, OnBnClickedButtonX)
	
	
	ON_BN_CLICKED(IDC_BUTTON_ABL_M_L, OnBnClickedButtonAblML)
	ON_BN_CLICKED(IDC_BUTTON_ABL_M_R, OnBnClickedButtonAblMR)
	ON_BN_CLICKED(IDC_BUTTON_ABL_W_L, OnBnClickedButtonAblWL)
	ON_BN_CLICKED(IDC_BUTTON_ABL_W_R, OnBnClickedButtonAblWR)
	ON_BN_CLICKED(IDC_BUTTON_VEHICLEPART, OnBnClickedButtonVehiclepart)


	ON_BN_CLICKED(IDC_BUTTON_WEARINGFILE2_1, OnBnClickedButtonWearingfile21)
	ON_BN_CLICKED(IDC_BUTTON_WEARINGFILE2_2, OnBnClickedButtonWearingfile22)
	ON_BN_CLICKED(IDC_BUTTON_WEARINGFILE2_3, OnBnClickedButtonWearingfile23)
	ON_BN_CLICKED(IDC_BUTTON_WEARINGFILE2_4, OnBnClickedButtonWearingfile24)
	ON_BN_CLICKED(IDC_BUTTON_WEARINGFILE2_5, OnBnClickedButtonWearingfile25)
	ON_BN_CLICKED(IDC_BUTTON_WEARINGFILE2_6, OnBnClickedButtonWearingfile26)
	ON_BN_CLICKED(IDC_BUTTON_WEARINGFILE2_7, OnBnClickedButtonWearingfile27)
	ON_BN_CLICKED(IDC_BUTTON_WEARINGFILE2_8, OnBnClickedButtonWearingfile28)
	ON_BN_CLICKED(IDC_BUTTON_WEARINGFILE2_9, OnBnClickedButtonWearingfile29)
	ON_BN_CLICKED(IDC_BUTTON_WEARINGFILE2_10, OnBnClickedButtonWearingfile210)
	ON_BN_CLICKED(IDC_BUTTON_WEARINGFILE2_11, OnBnClickedButtonWearingfile211)
	ON_BN_CLICKED(IDC_BUTTON_WEARINGFILE2_12, OnBnClickedButtonWearingfile212)
	ON_BN_CLICKED(IDC_BUTTON_WEARINGFILE2_13, OnBnClickedButtonWearingfile213)
	ON_BN_CLICKED(IDC_BUTTON_WEARINGFILE2_14, OnBnClickedButtonWearingfile214)
	ON_BN_CLICKED(IDC_BUTTON_WEARINGFILE2_15, OnBnClickedButtonWearingfile215)
	ON_BN_CLICKED(IDC_BUTTON_WEARINGFILE2_16, OnBnClickedButtonWearingfile216)
END_MESSAGE_MAP()


// CItemParent �޽��� ó�����Դϴ�.

BOOL CItemParent::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_pSheetTab->ChangeDialogFont( this, m_pFont, CDF_TOPLEFT );
	
	m_bDlgInit = TRUE;
	InitDefaultCtrls ();
	UpdateItems ();

	SetReadMode();


	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CItemParent::OnBnClickedButtonCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_pItem = NULL;
	m_pSheetTab->ActiveItemSuitTreePage ();
}

void CItemParent::OnBnClickedButtonOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if ( !InverseUpdateItems() )
	{		
		return;
	}

	CItemNode sItemNode;
	sItemNode.m_sItem = *m_pDummyItem;

	GLItemMan::GetInstance().InsertItem ( m_pDummyItem->sBasicOp.sNativeID, &sItemNode );

	GLStringTable::GetInstance().InsertString ( strKeyName, strName, GLStringTable::ITEM );
	GLStringTable::GetInstance().InsertString ( strKeyDesc, strDesc, GLStringTable::ITEM );

	m_pItem = NULL;
	m_pSheetTab->ActiveItemSuitTreePage ();
}

void CItemParent::InitAllItem ()
{
	m_pItem = NULL;
}

BOOL CItemParent::SetItem ( SITEM* pItem )
{
	if ( m_pItem )
	{
		return FALSE;
	}

	m_pItem = pItem;

	//	���̷� ��������
	*m_pDummyItem = *m_pItem;

	if ( m_bDlgInit ) 
	{
		InitDefaultCtrls ();
		UpdateItems ();		
	}

	return TRUE;
}

void CItemParent::UpdateItems ()
{
	int temp = 100;


	//	���� ID
	SetWin_Num_int ( this, IDC_EDIT_MID, m_pDummyItem->sBasicOp.sNativeID.wMainID );
	SetWin_Num_int ( this, IDC_EDIT_SID, m_pDummyItem->sBasicOp.sNativeID.wSubID );
//	SetWin_Check ( this, IDC_CHECK13, m_pDummyItem->sBasicOp.dwFlags&ITEM_PERFECT_GRINDER );;

	//	������ �̸�
	SetWin_Text ( this, IDC_EDIT_ITEMNAME, m_pDummyItem->sBasicOp.strName.c_str() );
	//SetWin_Text ( this, IDC_EDIT_NAME, GLStringTable::GetInstance().GetName( m_pDummyItem->GetName() ) );

	//	������ ����
	//SetWin_Combo_Sel ( this, IDC_COMBO_LEVEL, COMMENT::ITEMLEVEL[m_pDummyItem->sBasicOp.emLevel] );
	SetWin_Combo_Sel ( this, IDC_COMBO_LEVEL, (int)m_pDummyItem->sBasicOp.emLevel );

	//	Trade
	SetWin_Check ( this, IDC_CHECK_SELLENABLE, m_pDummyItem->sBasicOp.dwFlags&TRADE_SALE );
	SetWin_Check ( this, IDC_CHECK_BUYABLE , m_pDummyItem->sBasicOp.dwFlags&TRADE_EXCHANGE );
	SetWin_Check ( this, IDC_CHECK_THROW, m_pDummyItem->sBasicOp.dwFlags&TRADE_THROW );
	SetWin_Check ( this, IDC_CHECK_GARBAGE, m_pDummyItem->sBasicOp.dwFlags&TRADE_GARBAGE );

	//add locker
	SetWin_Check ( this, IDC_CHECK_LOCKER, m_pDummyItem->sBasicOp.dwFlags&TRADE_LOCKER );
	//add ownership
	SetWin_Check ( this, IDC_CHECK_OWNERSHIP, m_pDummyItem->sBasicOp.dwFlags&ITEM_OWNERSHIP );
	//add point sell
	SetWin_Check ( this, IDC_CHECK_POINTSELL, m_pDummyItem->sBasicOp.dwFlags&ITEM_POINT_RECOVER );
	//add jitem
	SetWin_Check ( this, IDC_CHECK_COMBINE, m_pDummyItem->sBasicOp.dwFlags&ITEM_COSTUME_COMBINE );

	SetWin_Check ( this, IDC_CHECK_COLORING, m_pDummyItem->sBasicOp.dwFlags&ITEM_COLORING );//add itemcolor

	SetWin_Check ( this, IDC_CHECK_EVENT_SGL, m_pDummyItem->sBasicOp.dwFlags&TRADE_EVENT_SGL );

	SetWin_Check ( this, IDC_CHECK_DISGUISE, m_pDummyItem->sBasicOp.dwFlags&ITEM_DISGUISE );
	SetWin_Check ( this, IDC_CHECK_TIMELMT, m_pDummyItem->sBasicOp.dwFlags&ITEM_TIMELMT );
	
	//	���԰���
	SetWin_Num_int ( this, IDC_EDIT_PRICE, m_pDummyItem->sBasicOp.dwBuyPrice );
	SetWin_Num_int ( this, IDC_EDIT_PRICE2, m_pDummyItem->sBasicOp.dwSellPrice );

/*	SetWin_Num_int ( this, IDC_EDIT_DWDUMMY1, m_pDummyItem->sBasicOp.dwDummy1 );
	SetWin_Num_int ( this, IDC_EDIT_DWDUMMY2, m_pDummyItem->sBasicOp.dwDummy2 );
	SetWin_Num_int ( this, IDC_EDIT_DWDUMMY3, m_pDummyItem->sBasicOp.dwDummy3 );
	SetWin_Num_int ( this, IDC_EDIT_DWDUMMY4, m_pDummyItem->sBasicOp.dwDummy4 );*/

	//	������ ���� ��Ģ
	SetWin_Num_int ( this, IDC_EDIT_SPECID, m_pDummyItem->sGenerateOp.dwSpecID );
	SetWin_Num_int ( this, IDC_EDIT_TIME_SET, m_pDummyItem->sGenerateOp.dwLimitTime );
	SetWin_Num_int ( this, IDC_EDIT_TIME_LIMIT, m_pDummyItem->sGenerateOp.dwLimitTimeGen );

	//	�κ� ũ��.
	SetWin_Num_int ( this, IDC_EDIT_INVENSIZEX, m_pDummyItem->sBasicOp.wInvenSizeX );
	SetWin_Num_int ( this, IDC_EDIT_INVENSIZEY, m_pDummyItem->sBasicOp.wInvenSizeY );

	//	������ �ε���.
	SetWin_Num_int ( this, IDC_EDIT_INVENICON_X, m_pDummyItem->sBasicOp.sICONID.wMainID );
	SetWin_Num_int ( this, IDC_EDIT_INVENICON_Y, m_pDummyItem->sBasicOp.sICONID.wSubID );

	//	���� ����.
	SetWin_Text ( this, IDC_EDIT_FIELDFILE, m_pDummyItem->GetFieldFile() );
	SetWin_Text ( this, IDC_EDIT_INVENFILE, m_pDummyItem->GetInventoryFile() );

	SetWin_Text ( this, IDC_EDIT_WEARINGFILE_C1, m_pDummyItem->GetWearingFile((EMCHARINDEX)0) );
	SetWin_Text ( this, IDC_EDIT_WEARINGFILE_C2, m_pDummyItem->GetWearingFile((EMCHARINDEX)6) );
	SetWin_Text ( this, IDC_EDIT_WEARINGFILE_C3, m_pDummyItem->GetWearingFile((EMCHARINDEX)1) );
	SetWin_Text ( this, IDC_EDIT_WEARINGFILE_C4, m_pDummyItem->GetWearingFile((EMCHARINDEX)7) );
	SetWin_Text ( this, IDC_EDIT_WEARINGFILE_C5, m_pDummyItem->GetWearingFile((EMCHARINDEX)8) );
	SetWin_Text ( this, IDC_EDIT_WEARINGFILE_C6, m_pDummyItem->GetWearingFile((EMCHARINDEX)2) );
	SetWin_Text ( this, IDC_EDIT_WEARINGFILE_C7, m_pDummyItem->GetWearingFile((EMCHARINDEX)9) );
	SetWin_Text ( this, IDC_EDIT_WEARINGFILE_C8, m_pDummyItem->GetWearingFile((EMCHARINDEX)3) );
	SetWin_Text ( this, IDC_EDIT_WEARINGFILE_C9, m_pDummyItem->GetWearingFile((EMCHARINDEX)4) );
	SetWin_Text ( this, IDC_EDIT_WEARINGFILE_C10, m_pDummyItem->GetWearingFile((EMCHARINDEX)5) );
	SetWin_Text ( this, IDC_EDIT_WEARINGFILE_C12, m_pDummyItem->GetWearingFile((EMCHARINDEX)10) );
	SetWin_Text ( this, IDC_EDIT_WEARINGFILE_C13, m_pDummyItem->GetWearingFile((EMCHARINDEX)11) );
	SetWin_Text ( this, IDC_EDIT_WEARINGFILE_C14, m_pDummyItem->GetWearingFile((EMCHARINDEX)12) );
	SetWin_Text ( this, IDC_EDIT_WEARINGFILE_C15, m_pDummyItem->GetWearingFile((EMCHARINDEX)13) );
	SetWin_Text ( this, IDC_EDIT_WEARINGFILE_C16, m_pDummyItem->GetWearingFile((EMCHARINDEX)14) );
	SetWin_Text ( this, IDC_EDIT_WEARINGFILE_C17, m_pDummyItem->GetWearingFile((EMCHARINDEX)15) );
	SetWin_Text ( this, IDC_EDIT_WEARINGFILE_C18, m_pDummyItem->GetWearingFile((EMCHARINDEX)16) );
	SetWin_Text ( this, IDC_EDIT_WEARINGFILE_C20, m_pDummyItem->GetWearingFile((EMCHARINDEX)17) );
	/////
	SetWin_Text ( this, IDC_EDIT_STRDUMMY2_1, m_pDummyItem->GetWearingFile2((EMCHARINDEX)0) );
	SetWin_Text ( this, IDC_EDIT_STRDUMMY2_9, m_pDummyItem->GetWearingFile2((EMCHARINDEX)6) );
	SetWin_Text ( this, IDC_EDIT_STRDUMMY2_2, m_pDummyItem->GetWearingFile2((EMCHARINDEX)1) );
	SetWin_Text ( this, IDC_EDIT_STRDUMMY2_10, m_pDummyItem->GetWearingFile2((EMCHARINDEX)7) );
	SetWin_Text ( this, IDC_EDIT_STRDUMMY2_3, m_pDummyItem->GetWearingFile2((EMCHARINDEX)8) );
	SetWin_Text ( this, IDC_EDIT_STRDUMMY2_11, m_pDummyItem->GetWearingFile2((EMCHARINDEX)2) );
	SetWin_Text ( this, IDC_EDIT_STRDUMMY2_4, m_pDummyItem->GetWearingFile2((EMCHARINDEX)9) );
	SetWin_Text ( this, IDC_EDIT_STRDUMMY2_12, m_pDummyItem->GetWearingFile2((EMCHARINDEX)3) );
	SetWin_Text ( this, IDC_EDIT_STRDUMMY2_5, m_pDummyItem->GetWearingFile2((EMCHARINDEX)4) );
	SetWin_Text ( this, IDC_EDIT_STRDUMMY2_13, m_pDummyItem->GetWearingFile2((EMCHARINDEX)5) );
	SetWin_Text ( this, IDC_EDIT_STRDUMMY2_6, m_pDummyItem->GetWearingFile2((EMCHARINDEX)10) );
	SetWin_Text ( this, IDC_EDIT_STRDUMMY2_14, m_pDummyItem->GetWearingFile2((EMCHARINDEX)11) );
	SetWin_Text ( this, IDC_EDIT_STRDUMMY2_7, m_pDummyItem->GetWearingFile2((EMCHARINDEX)12) );
	SetWin_Text ( this, IDC_EDIT_STRDUMMY2_15, m_pDummyItem->GetWearingFile2((EMCHARINDEX)13) );
	SetWin_Text ( this, IDC_EDIT_STRDUMMY2_8, m_pDummyItem->GetWearingFile2((EMCHARINDEX)14) );
	SetWin_Text ( this, IDC_EDIT_STRDUMMY2_16, m_pDummyItem->GetWearingFile2((EMCHARINDEX)15) );

	/*SetWin_Text ( this, IDC_EDIT_STRDUMMY2_1, m_pDummyItem->GetWearingFile2((EMCHARINDEX)0) );
	SetWin_Text ( this, IDC_EDIT_STRDUMMY2_9, m_pDummyItem->GetWearingFile2((EMCHARINDEX)1) );
	SetWin_Text ( this, IDC_EDIT_STRDUMMY2_2, m_pDummyItem->GetWearingFile2((EMCHARINDEX)2) );
	SetWin_Text ( this, IDC_EDIT_STRDUMMY2_10, m_pDummyItem->GetWearingFile2((EMCHARINDEX)3) );
	SetWin_Text ( this, IDC_EDIT_STRDUMMY2_3, m_pDummyItem->GetWearingFile2((EMCHARINDEX)4) );
	SetWin_Text ( this, IDC_EDIT_STRDUMMY2_11, m_pDummyItem->GetWearingFile2((EMCHARINDEX)5) );
	SetWin_Text ( this, IDC_EDIT_STRDUMMY2_4, m_pDummyItem->GetWearingFile2((EMCHARINDEX)8) );
	SetWin_Text ( this, IDC_EDIT_STRDUMMY2_12, m_pDummyItem->GetWearingFile2((EMCHARINDEX)9) );
	SetWin_Text ( this, IDC_EDIT_STRDUMMY2_5, m_pDummyItem->GetWearingFile2((EMCHARINDEX)6) );
	SetWin_Text ( this, IDC_EDIT_STRDUMMY2_13, m_pDummyItem->GetWearingFile2((EMCHARINDEX)7) );
	SetWin_Text ( this, IDC_EDIT_STRDUMMY2_6, m_pDummyItem->GetWearingFile2((EMCHARINDEX)12) );
	SetWin_Text ( this, IDC_EDIT_STRDUMMY2_14, m_pDummyItem->GetWearingFile2((EMCHARINDEX)13) );
	SetWin_Text ( this, IDC_EDIT_STRDUMMY2_7, m_pDummyItem->GetWearingFile2((EMCHARINDEX)14) );
	SetWin_Text ( this, IDC_EDIT_STRDUMMY2_15, m_pDummyItem->GetWearingFile2((EMCHARINDEX)15) );
	SetWin_Text ( this, IDC_EDIT_STRDUMMY2_8, m_pDummyItem->GetWearingFile2((EMCHARINDEX)10) );
	SetWin_Text ( this, IDC_EDIT_STRDUMMY2_16, m_pDummyItem->GetWearingFile2((EMCHARINDEX)11) );*/



	SetWin_Text ( this, IDC_EDIT_WEARINGFILE_C11, m_pDummyItem->GetPetWearingFile () );
	SetWin_Text ( this, IDC_EDIT_ITEMDESC, m_pDummyItem->sBasicOp.strComment.c_str() );
	
	//	��, ��� �Ӽ�.	
	SetWin_Check ( this, IDC_RADIO_BRIGHT_LIGHT, m_pDummyItem->sBasicOp.emReqBright==BRIGHT_LIGHT);
	SetWin_Check ( this, IDC_RADIO_BRIGHT_DARK,  m_pDummyItem->sBasicOp.emReqBright==BRIGHT_DARK );
	SetWin_Check ( this, IDC_RADIO_BRIGHT_BOTH,  m_pDummyItem->sBasicOp.emReqBright==BRIGHT_BOTH );

	//	��밡�� ĳ����.
	SetWin_Check ( this, IDC_CHECK_FIGHTER_M, m_pDummyItem->sBasicOp.dwReqCharClass&GLCC_FIGHTER_M );
	SetWin_Check ( this, IDC_CHECK_ARMS_M, m_pDummyItem->sBasicOp.dwReqCharClass&GLCC_ARMS_M );
	SetWin_Check ( this, IDC_CHECK_ARCHER_W, m_pDummyItem->sBasicOp.dwReqCharClass&GLCC_ARCHER_W );
	SetWin_Check ( this, IDC_CHECK_SPIRIT_W, m_pDummyItem->sBasicOp.dwReqCharClass&GLCC_SPIRIT_W );

	SetWin_Check ( this, IDC_CHECK_FIGHTER_W, m_pDummyItem->sBasicOp.dwReqCharClass&GLCC_FIGHTER_W );
	SetWin_Check ( this, IDC_CHECK_ARMS_W, m_pDummyItem->sBasicOp.dwReqCharClass&GLCC_ARMS_W );
	SetWin_Check ( this, IDC_CHECK_ARCHER_M, m_pDummyItem->sBasicOp.dwReqCharClass&GLCC_ARCHER_M );
	SetWin_Check ( this, IDC_CHECK_SPIRIT_M, m_pDummyItem->sBasicOp.dwReqCharClass&GLCC_SPIRIT_M );

	SetWin_Check ( this, IDC_CHECK_EXTREME_M, m_pDummyItem->sBasicOp.dwReqCharClass&GLCC_EXTREME_M );
	SetWin_Check ( this, IDC_CHECK_EXTREME_W, m_pDummyItem->sBasicOp.dwReqCharClass&GLCC_EXTREME_W );

	//add class
	SetWin_Check ( this, IDC_CHECK_SCIENCE_M, m_pDummyItem->sBasicOp.dwReqCharClass&GLCC_SCIENCE_M );
	SetWin_Check ( this, IDC_CHECK_SCIENCE_W, m_pDummyItem->sBasicOp.dwReqCharClass&GLCC_SCIENCE_W );
	SetWin_Check ( this, IDC_CHECK_ASSASIN_M, m_pDummyItem->sBasicOp.dwReqCharClass&GLCC_ASSASIN_M );
	SetWin_Check ( this, IDC_CHECK_ASSASIN_W, m_pDummyItem->sBasicOp.dwReqCharClass&GLCC_ASSASIN_W );

	SetWin_Check ( this, IDC_CHECK_TESTING_M, m_pDummyItem->sBasicOp.dwReqCharClass&GLCC_TESTING_M );
	SetWin_Check ( this, IDC_CHECK_TESTING_W, m_pDummyItem->sBasicOp.dwReqCharClass&GLCC_TESTING_W );

	SetWin_Check ( this, IDC_CHECK_TESTING_M2, m_pDummyItem->sBasicOp.dwReqCharClass&GLCC_SHAPER_M );
	SetWin_Check ( this, IDC_CHECK_TESTING_W2, m_pDummyItem->sBasicOp.dwReqCharClass&GLCC_SHAPER_W );

	//add juver
	//SetWin_Check ( this, IDC_RADIO_M, m_pDummyItem->sBasicOp.dwReqCharClass&GLCC_MAN );
	//SetWin_Check ( this, IDC_RADIO_W, m_pDummyItem->sBasicOp.dwReqCharClass&GLCC_WOMAN );
	//SetWin_Check ( this, IDC_RADIO_ALL, m_pDummyItem->sBasicOp.dwReqCharClass&GLCC_ALL_2012 );


	//	��밡�� �п�.
	SetWin_Check ( this, IDC_CHECK_SCHOOL1, m_pDummyItem->sBasicOp.dwReqSchool&GLSCHOOL_00 );
	SetWin_Check ( this, IDC_CHECK_SCHOOL2, m_pDummyItem->sBasicOp.dwReqSchool&GLSCHOOL_01 );
	SetWin_Check ( this, IDC_CHECK_SCHOOL3, m_pDummyItem->sBasicOp.dwReqSchool&GLSCHOOL_02 );

	//	�䱸 ��ġ.
	SetWin_Num_int ( this, IDC_EDIT_NEEDLEVELDW, m_pDummyItem->sBasicOp.wReqLevelDW );
	SetWin_Num_int ( this, IDC_EDIT_NEEDLEVELUP, m_pDummyItem->sBasicOp.wReqLevelUP );

	//
	SetWin_Num_int ( this, IDC_EDIT_NEEDLEVELUP2, m_pDummyItem->sBasicOp.dwRebornReq );

	SetWin_Num_int ( this, IDC_EDIT_POWER, m_pDummyItem->sBasicOp.sReqStats.dwPow );
	SetWin_Num_int ( this, IDC_EDIT_STRENGTH, m_pDummyItem->sBasicOp.sReqStats.dwStr );
	SetWin_Num_int ( this, IDC_EDIT_SPIRIT, m_pDummyItem->sBasicOp.sReqStats.dwSpi );
	SetWin_Num_int ( this, IDC_EDIT_DEXTERITY, m_pDummyItem->sBasicOp.sReqStats.dwDex );
	SetWin_Num_int ( this, IDC_EDIT_INTELLIGENT, m_pDummyItem->sBasicOp.sReqStats.dwInt );
	SetWin_Num_int ( this, IDC_EDIT_STAMINA, m_pDummyItem->sBasicOp.sReqStats.dwSta );


	SetWin_Num_int ( this, IDC_EDIT_REQ_PA, m_pDummyItem->sBasicOp.wReqPA );
	SetWin_Num_int ( this, IDC_EDIT_REQ_SA, m_pDummyItem->sBasicOp.wReqSA );

	//	������ ����.
	SetWin_Combo_Sel ( this, IDC_COMBO_ITEM_TYPE, COMMENT::ITEMTYPE[m_pDummyItem->sBasicOp.emItemType] );
	SetWin_Combo_Sel ( this, IDC_COMBO_ITEM_TYPE, (int)m_pDummyItem->sBasicOp.emItemType );

	// PET
	SetWin_Combo_Sel ( this, IDC_COMBO_PET_TYPE, (int)m_pDummyItem->sPet.emPetType );
	SetWin_Num_int ( this, IDC_EDIT_PET_MID, m_pDummyItem->sPet.sPetID.wMainID );
	SetWin_Num_int ( this, IDC_EDIT_PET_SID, m_pDummyItem->sPet.sPetID.wSubID );

	// Ż��
	SetWin_Combo_Sel ( this, IDC_COMBO_VEHICLE_TYPE, (int)m_pDummyItem->sVehicle.emVehicleType );

	//cooldown
	SetWin_Combo_Sel ( this, IDC_COMBO_COOLDOWN, (int)m_pDummyItem->sBasicOp.emCoolType );

	//	���� ���� Ƚ��.
	SetWin_Num_int ( this, IDC_EDIT_REMODEL_NUM, m_pDummyItem->sSuitOp.wReModelNum );
	
	//	�Ҹ���.
	SetWin_Check ( this, IDC_CHECK_INSTANCE, m_pDummyItem->sDrugOp.bInstance );

	CTimeSpan cSPAN(m_pDummyItem->sDrugOp.tTIME_LMT);
	SetWin_Num_int ( this, IDC_EDIT_LMT_D, (int)cSPAN.GetDays() );
	SetWin_Num_int ( this, IDC_EDIT_LMT_H, cSPAN.GetHours() );
	SetWin_Num_int ( this, IDC_EDIT_LMT_M, cSPAN.GetMinutes() );

	SetWin_Text ( this, IDC_EDIT_SKIN_EFFECTFILE, m_pDummyItem->GetSelfBodyEffect() );
	SetWin_Text ( this, IDC_EDIT_TAR_EFFECTFILE, m_pDummyItem->GetTargBodyEffect() );
	SetWin_Text ( this, IDC_EDIT_EFFECTFILE, m_pDummyItem->GetTargetEffect() );

	SetWin_Text ( this, IDC_EDIT_RANDOM_OPT, m_pDummyItem->sRandomOpt.szNAME );

	SetWin_Num_int ( this, IDC_EDIT_MAP_MID, m_pDummyItem->sBasicOp.sSubID.wMainID );
	SetWin_Num_int ( this, IDC_EDIT_MAP_SID, m_pDummyItem->sBasicOp.sSubID.wSubID );
	SetWin_Num_int ( this, IDC_EDIT_MAP_POSX, m_pDummyItem->sBasicOp.wPosX );
	SetWin_Num_int ( this, IDC_EDIT_MAP_POSY, m_pDummyItem->sBasicOp.wPosY );





	//cooldown
	SetWin_Num_int ( this, IDC_EDIT_COOLDOWN, m_pDummyItem->sBasicOp.dwCoolTime );

	SetWin_Num_int ( this, IDC_EDIT_SUM_MID, m_pDummyItem->sBasicOp.sSumID.wMainID );
	SetWin_Num_int ( this, IDC_EDIT_SUM_SID, m_pDummyItem->sBasicOp.sSumID.wSubID );
	SetWin_Combo_Sel ( this, IDC_COMBO_SUM_TYPE, (int)m_pDummyItem->sBasicOp.m_emSumType );

	SetWin_Num_int ( this, IDC_EDIT_SLOTNUM, m_pDummyItem->sBasicOp.dwSlotNum );

	//add abl
	SetWin_Text ( this, IDC_EDIT_ABL_M_L, m_pDummyItem->GetCpsFile((EMCPSINDEX)0) ); // ������ 	
	SetWin_Text ( this, IDC_EDIT_ABL_M_R, m_pDummyItem->GetCpsFile((EMCPSINDEX)1) ); // ������
	SetWin_Text ( this, IDC_EDIT_ABL_W_L, m_pDummyItem->GetCpsFile((EMCPSINDEX)2) ); // �˵���
	SetWin_Text ( this, IDC_EDIT_ABL_W_R, m_pDummyItem->GetCpsFile((EMCPSINDEX)3) ); // �˵���

	SetWin_Text ( this, IDC_EDIT_VEHICLEPART, m_pDummyItem->sBasicOp.strVPSFile.c_str()) ; // �˵���

}

BOOL CItemParent::InverseUpdateItems ()
{
	//	���� ID
	m_pDummyItem->sBasicOp.sNativeID.wMainID = GetWin_Num_int ( this, IDC_EDIT_MID );
	m_pDummyItem->sBasicOp.sNativeID.wSubID = GetWin_Num_int ( this, IDC_EDIT_SID );

	//	������ �̸�
	m_pDummyItem->sBasicOp.strName = GetWin_Text ( this, IDC_EDIT_ITEMNAME ).GetString();
	strKeyName = GetWin_Text ( this, IDC_EDIT_ITEMNAME ).GetString();
	strName	= GetWin_Text ( this, IDC_EDIT_NAME ).GetString();

	//	������ ����
	m_pDummyItem->sBasicOp.emLevel = (EMITEMLEVEL) GetWin_Combo_Sel ( this, IDC_COMBO_LEVEL );

	//	Trade
	SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_SELLENABLE ), m_pDummyItem->sBasicOp.dwFlags, TRADE_SALE );
	SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_BUYABLE ), m_pDummyItem->sBasicOp.dwFlags, TRADE_EXCHANGE );
	SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_THROW ), m_pDummyItem->sBasicOp.dwFlags, TRADE_THROW );
	SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_GARBAGE ), m_pDummyItem->sBasicOp.dwFlags, TRADE_GARBAGE );
	SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_EVENT_SGL ), m_pDummyItem->sBasicOp.dwFlags, TRADE_EVENT_SGL );
//    SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK13 ), m_pDummyItem->sBasicOp.dwFlags, ITEM_PERFECT_GRINDER ) ;

	SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_LOCKER ), m_pDummyItem->sBasicOp.dwFlags, TRADE_LOCKER );//add locker
	SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_OWNERSHIP ), m_pDummyItem->sBasicOp.dwFlags, ITEM_OWNERSHIP );	//add ownership
	SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_POINTSELL ), m_pDummyItem->sBasicOp.dwFlags, ITEM_POINT_RECOVER );	//add point sell
	SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_COMBINE ), m_pDummyItem->sBasicOp.dwFlags, ITEM_COSTUME_COMBINE );	//add jitem
	SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_COLORING ), m_pDummyItem->sBasicOp.dwFlags, ITEM_COLORING );//add itemcolor

	SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_DISGUISE ), m_pDummyItem->sBasicOp.dwFlags, ITEM_DISGUISE );
	SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_TIMELMT ), m_pDummyItem->sBasicOp.dwFlags, ITEM_TIMELMT );

	//	���԰���
	m_pDummyItem->sBasicOp.dwBuyPrice  = GetWin_Num_int ( this, IDC_EDIT_PRICE );
	//  �ǸŰ���
	m_pDummyItem->sBasicOp.dwSellPrice = GetWin_Num_int ( this, IDC_EDIT_PRICE2 );

	//	������ ���� ��Ģ
	m_pDummyItem->sGenerateOp.dwSpecID = GetWin_Num_int ( this, IDC_EDIT_SPECID );
	m_pDummyItem->sGenerateOp.dwLimitTime = GetWin_Num_int ( this, IDC_EDIT_TIME_SET );
	m_pDummyItem->sGenerateOp.dwLimitTimeGen = GetWin_Num_int ( this, IDC_EDIT_TIME_LIMIT );

    //	�κ� ũ��.
	m_pDummyItem->sBasicOp.wInvenSizeX = GetWin_Num_int ( this, IDC_EDIT_INVENSIZEX );
	m_pDummyItem->sBasicOp.wInvenSizeY = GetWin_Num_int ( this, IDC_EDIT_INVENSIZEY );

	//	������ �ε���.
	m_pDummyItem->sBasicOp.sICONID.wMainID = GetWin_Num_int ( this, IDC_EDIT_INVENICON_X );
	m_pDummyItem->sBasicOp.sICONID.wSubID = GetWin_Num_int ( this, IDC_EDIT_INVENICON_Y );

	//	���� ����.
	m_pDummyItem->sBasicOp.strFieldFile = GetWin_Text ( this, IDC_EDIT_FIELDFILE ).GetString();
	m_pDummyItem->sBasicOp.strInventoryFile = GetWin_Text ( this, IDC_EDIT_INVENFILE ).GetString();

	m_pDummyItem->sBasicOp.strWearingFile[GLCI_FIGHTER_M] = GetWin_Text ( this, IDC_EDIT_WEARINGFILE_C1 ).GetString();
	m_pDummyItem->sBasicOp.strWearingFile[GLCI_FIGHTER_W] = GetWin_Text ( this, IDC_EDIT_WEARINGFILE_C2 ).GetString();
	m_pDummyItem->sBasicOp.strWearingFile[GLCI_ARMS_M] = GetWin_Text ( this, IDC_EDIT_WEARINGFILE_C3 ).GetString();
	m_pDummyItem->sBasicOp.strWearingFile[GLCI_ARMS_W] = GetWin_Text ( this, IDC_EDIT_WEARINGFILE_C4 ).GetString();
	m_pDummyItem->sBasicOp.strWearingFile[GLCI_ARCHER_M] = GetWin_Text ( this, IDC_EDIT_WEARINGFILE_C5 ).GetString();
	m_pDummyItem->sBasicOp.strWearingFile[GLCI_ARCHER_W] = GetWin_Text ( this, IDC_EDIT_WEARINGFILE_C6 ).GetString();
	m_pDummyItem->sBasicOp.strWearingFile[GLCI_SPIRIT_M] = GetWin_Text ( this, IDC_EDIT_WEARINGFILE_C7 ).GetString();	
	m_pDummyItem->sBasicOp.strWearingFile[GLCI_SPIRIT_W] = GetWin_Text ( this, IDC_EDIT_WEARINGFILE_C8 ).GetString();	
	m_pDummyItem->sBasicOp.strWearingFile[GLCI_EXTREME_M] = GetWin_Text ( this, IDC_EDIT_WEARINGFILE_C9 ).GetString();
	m_pDummyItem->sBasicOp.strWearingFile[GLCI_EXTREME_W] = GetWin_Text ( this, IDC_EDIT_WEARINGFILE_C10 ).GetString();
	m_pDummyItem->sBasicOp.strWearingFile[GLCI_SCIENCE_M] = GetWin_Text ( this, IDC_EDIT_WEARINGFILE_C12 ).GetString();
	m_pDummyItem->sBasicOp.strWearingFile[GLCI_SCIENCE_W] = GetWin_Text ( this, IDC_EDIT_WEARINGFILE_C13 ).GetString();
	m_pDummyItem->sBasicOp.strWearingFile[GLCI_ASSASIN_M] = GetWin_Text ( this, IDC_EDIT_WEARINGFILE_C14 ).GetString();
	m_pDummyItem->sBasicOp.strWearingFile[GLCI_ASSASIN_W] = GetWin_Text ( this, IDC_EDIT_WEARINGFILE_C15 ).GetString();
	m_pDummyItem->sBasicOp.strWearingFile[GLCI_TESTING_M] = GetWin_Text ( this, IDC_EDIT_WEARINGFILE_C16 ).GetString();
	m_pDummyItem->sBasicOp.strWearingFile[GLCI_TESTING_W] = GetWin_Text ( this, IDC_EDIT_WEARINGFILE_C17 ).GetString();
	m_pDummyItem->sBasicOp.strWearingFile[GLCI_SHAPER_M] = GetWin_Text ( this, IDC_EDIT_WEARINGFILE_C18 ).GetString();
	m_pDummyItem->sBasicOp.strWearingFile[GLCI_SHAPER_W] = GetWin_Text ( this, IDC_EDIT_WEARINGFILE_C20 ).GetString();
	//////

	m_pDummyItem->sBasicOp.strWearingFile2[GLCI_FIGHTER_M] = GetWin_Text ( this, IDC_EDIT_STRDUMMY2_1 ).GetString();
	m_pDummyItem->sBasicOp.strWearingFile2[GLCI_FIGHTER_W] = GetWin_Text ( this, IDC_EDIT_STRDUMMY2_9 ).GetString();
	m_pDummyItem->sBasicOp.strWearingFile2[GLCI_ARMS_M] = GetWin_Text ( this, IDC_EDIT_STRDUMMY2_2 ).GetString();
	m_pDummyItem->sBasicOp.strWearingFile2[GLCI_ARMS_W] = GetWin_Text ( this, IDC_EDIT_STRDUMMY2_10 ).GetString();
	m_pDummyItem->sBasicOp.strWearingFile2[GLCI_ARCHER_M] = GetWin_Text ( this, IDC_EDIT_STRDUMMY2_3 ).GetString();
	m_pDummyItem->sBasicOp.strWearingFile2[GLCI_ARCHER_W] = GetWin_Text ( this, IDC_EDIT_STRDUMMY2_11 ).GetString();
	m_pDummyItem->sBasicOp.strWearingFile2[GLCI_SPIRIT_M] = GetWin_Text ( this, IDC_EDIT_STRDUMMY2_4 ).GetString();	
	m_pDummyItem->sBasicOp.strWearingFile2[GLCI_SPIRIT_W] = GetWin_Text ( this, IDC_EDIT_STRDUMMY2_12 ).GetString();	
	m_pDummyItem->sBasicOp.strWearingFile2[GLCI_EXTREME_M] = GetWin_Text ( this, IDC_EDIT_STRDUMMY2_5 ).GetString();
	m_pDummyItem->sBasicOp.strWearingFile2[GLCI_EXTREME_W] = GetWin_Text ( this, IDC_EDIT_STRDUMMY2_13 ).GetString();
	m_pDummyItem->sBasicOp.strWearingFile2[GLCI_SCIENCE_M] = GetWin_Text ( this, IDC_EDIT_STRDUMMY2_6 ).GetString();
	m_pDummyItem->sBasicOp.strWearingFile2[GLCI_SCIENCE_W] = GetWin_Text ( this, IDC_EDIT_STRDUMMY2_14 ).GetString();
	m_pDummyItem->sBasicOp.strWearingFile2[GLCI_ASSASIN_M] = GetWin_Text ( this, IDC_EDIT_STRDUMMY2_7 ).GetString();
	m_pDummyItem->sBasicOp.strWearingFile2[GLCI_ASSASIN_W] = GetWin_Text ( this, IDC_EDIT_STRDUMMY2_15 ).GetString();
	m_pDummyItem->sBasicOp.strWearingFile2[GLCI_TESTING_M] = GetWin_Text ( this, IDC_EDIT_STRDUMMY2_8 ).GetString();
	m_pDummyItem->sBasicOp.strWearingFile2[GLCI_TESTING_W] = GetWin_Text ( this, IDC_EDIT_STRDUMMY2_16 ).GetString();


	
	m_pDummyItem->sBasicOp.strPetWearingFile = GetWin_Text ( this, IDC_EDIT_WEARINGFILE_C11 ).GetString();


	strKeyDesc = GetWin_Text ( this, IDC_EDIT_ITEMDESC ).GetString();
	CString strComment = GetWin_Text ( this, IDC_EDIT_COMMENT );
	//if ( strComment.GetLength()+1 < ITEM_SZCOMMENT )	m_pDummyItem->sBasicOp.strComment = strComment.GetString();
	if ( strComment.GetLength()+1 < ITEM_SZCOMMENT ) strDesc = strComment.GetString();

	//	��, ��� �Ӽ�.
	if ( GetWin_Check ( this, IDC_RADIO_BRIGHT_LIGHT) )	m_pDummyItem->sBasicOp.emReqBright = BRIGHT_LIGHT;
	if ( GetWin_Check ( this, IDC_RADIO_BRIGHT_DARK ) )	m_pDummyItem->sBasicOp.emReqBright = BRIGHT_DARK;
	if ( GetWin_Check ( this, IDC_RADIO_BRIGHT_BOTH ) )	m_pDummyItem->sBasicOp.emReqBright = BRIGHT_BOTH;

	//	��밡�� ĳ����.
	SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_FIGHTER_M ),m_pDummyItem->sBasicOp.dwReqCharClass, GLCC_FIGHTER_M );
	SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_ARMS_M ),   m_pDummyItem->sBasicOp.dwReqCharClass, GLCC_ARMS_M );
	SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_ARCHER_W ), m_pDummyItem->sBasicOp.dwReqCharClass, GLCC_ARCHER_W );
	SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_SPIRIT_W ), m_pDummyItem->sBasicOp.dwReqCharClass, GLCC_SPIRIT_W );

	SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_FIGHTER_W ),m_pDummyItem->sBasicOp.dwReqCharClass, GLCC_FIGHTER_W );
	SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_ARMS_W ),   m_pDummyItem->sBasicOp.dwReqCharClass, GLCC_ARMS_W );
	SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_ARCHER_M ), m_pDummyItem->sBasicOp.dwReqCharClass, GLCC_ARCHER_M );
	SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_SPIRIT_M ), m_pDummyItem->sBasicOp.dwReqCharClass, GLCC_SPIRIT_M );

	SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_EXTREME_M ), m_pDummyItem->sBasicOp.dwReqCharClass, GLCC_EXTREME_M );
	SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_EXTREME_W ), m_pDummyItem->sBasicOp.dwReqCharClass, GLCC_EXTREME_W );

	//add class
	SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_SCIENCE_M ), m_pDummyItem->sBasicOp.dwReqCharClass, GLCC_SCIENCE_M ); 
	SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_SCIENCE_W ), m_pDummyItem->sBasicOp.dwReqCharClass, GLCC_SCIENCE_W ); 
	SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_ASSASIN_M ), m_pDummyItem->sBasicOp.dwReqCharClass, GLCC_ASSASIN_M ); 
	SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_ASSASIN_W ), m_pDummyItem->sBasicOp.dwReqCharClass, GLCC_ASSASIN_W ); 

	SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_TESTING_M ), m_pDummyItem->sBasicOp.dwReqCharClass, GLCC_TESTING_M ); 
	SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_TESTING_W ), m_pDummyItem->sBasicOp.dwReqCharClass, GLCC_TESTING_W ); 

	SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_TESTING_M2 ), m_pDummyItem->sBasicOp.dwReqCharClass, GLCC_SHAPER_M ); 
	SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_TESTING_W2 ), m_pDummyItem->sBasicOp.dwReqCharClass, GLCC_SHAPER_W ); 


	//	��밡�� �п�.
	SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_SCHOOL1 ), m_pDummyItem->sBasicOp.dwReqSchool, GLSCHOOL_00 );
	SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_SCHOOL2 ), m_pDummyItem->sBasicOp.dwReqSchool, GLSCHOOL_01 );
	SetCheck_Flags ( GetWin_Check ( this, IDC_CHECK_SCHOOL3 ), m_pDummyItem->sBasicOp.dwReqSchool, GLSCHOOL_02 );

	if ( m_pDummyItem->sBasicOp.dwReqCharClass == GLCC_NONE )
	{
		MessageBox ( "An usable class is not selected.", "Caution", MB_OK | MB_ICONEXCLAMATION );
		return FALSE;
	}

	//	�䱸 ��ġ.
	m_pDummyItem->sBasicOp.wReqLevelDW = GetWin_Num_int ( this, IDC_EDIT_NEEDLEVELDW );
	m_pDummyItem->sBasicOp.wReqLevelUP = GetWin_Num_int ( this, IDC_EDIT_NEEDLEVELUP );

	//
	m_pDummyItem->sBasicOp.dwRebornReq = GetWin_Num_int ( this, IDC_EDIT_NEEDLEVELUP2 );

	m_pDummyItem->sBasicOp.sReqStats.dwPow = GetWin_Num_int ( this, IDC_EDIT_POWER );
	m_pDummyItem->sBasicOp.sReqStats.dwStr = GetWin_Num_int ( this, IDC_EDIT_STRENGTH );
	m_pDummyItem->sBasicOp.sReqStats.dwSpi = GetWin_Num_int ( this, IDC_EDIT_SPIRIT );
	m_pDummyItem->sBasicOp.sReqStats.dwDex = GetWin_Num_int ( this, IDC_EDIT_DEXTERITY );
	m_pDummyItem->sBasicOp.sReqStats.dwInt = GetWin_Num_int ( this, IDC_EDIT_INTELLIGENT );
	m_pDummyItem->sBasicOp.sReqStats.dwSta = GetWin_Num_int ( this, IDC_EDIT_STAMINA );


	m_pDummyItem->sBasicOp.wReqPA = GetWin_Num_int ( this, IDC_EDIT_REQ_PA );
	m_pDummyItem->sBasicOp.wReqSA = GetWin_Num_int ( this, IDC_EDIT_REQ_SA );

	//	������ ����.
	m_pDummyItem->sBasicOp.emItemType = (EMITEM_TYPE) GetWin_Combo_Sel ( this, IDC_COMBO_ITEM_TYPE );

	// PET
	m_pDummyItem->sPet.sPetID.wMainID = GetWin_Num_int ( this, IDC_EDIT_PET_MID );
	m_pDummyItem->sPet.sPetID.wSubID = GetWin_Num_int ( this, IDC_EDIT_PET_SID );
	m_pDummyItem->sPet.emPetType = (PETTYPE) GetWin_Combo_Sel ( this, IDC_COMBO_PET_TYPE );

	// Ż�� Ÿ��
	m_pDummyItem->sVehicle.emVehicleType = (VEHICLE_TYPE) GetWin_Combo_Sel ( this, IDC_COMBO_VEHICLE_TYPE );

	//cooldown
	m_pDummyItem->sBasicOp.emCoolType = (EMCOOL_TYPE) GetWin_Combo_Sel ( this, IDC_COMBO_COOLDOWN );

	//	���� ���� Ƚ��.
	m_pDummyItem->sSuitOp.wReModelNum = GetWin_Num_int ( this, IDC_EDIT_REMODEL_NUM );
	if ( m_pDummyItem->sSuitOp.wReModelNum > 0xff )	m_pDummyItem->sSuitOp.wReModelNum = 0xff;
	
	//	�Ҹ���.
	m_pDummyItem->sDrugOp.bInstance = GetWin_Check ( this, IDC_CHECK_INSTANCE );

	LONG lDays = GetWin_Num_int ( this, IDC_EDIT_LMT_D );
	int nHours = GetWin_Num_int ( this, IDC_EDIT_LMT_H );
	int nMins = GetWin_Num_int ( this, IDC_EDIT_LMT_M );
	
	CTimeSpan cSPAN(lDays,nHours,nMins,0);
	m_pDummyItem->sDrugOp.tTIME_LMT = cSPAN.GetTimeSpan();

	m_pDummyItem->sBasicOp.strSelfBodyEffect = GetWin_Text(this,IDC_EDIT_SKIN_EFFECTFILE).GetString();
	m_pDummyItem->sBasicOp.strTargBodyEffect = GetWin_Text(this,IDC_EDIT_TAR_EFFECTFILE).GetString();
	m_pDummyItem->sBasicOp.strTargetEffect = GetWin_Text(this,IDC_EDIT_EFFECTFILE).GetString();


	StringCchCopy ( m_pDummyItem->sRandomOpt.szNAME, SRANDOM_DATA::NAME_LEN, GetWin_Text(this,IDC_EDIT_RANDOM_OPT).GetString() );

	m_pDummyItem->sBasicOp.sSubID.wMainID = GetWin_Num_int ( this, IDC_EDIT_MAP_MID );
	m_pDummyItem->sBasicOp.sSubID.wSubID = GetWin_Num_int ( this, IDC_EDIT_MAP_SID );
	m_pDummyItem->sBasicOp.wPosX = GetWin_Num_int ( this, IDC_EDIT_MAP_POSX );
	m_pDummyItem->sBasicOp.wPosY = GetWin_Num_int ( this, IDC_EDIT_MAP_POSY );


	//cooldown
	m_pDummyItem->sBasicOp.dwCoolTime = GetWin_Num_int ( this, IDC_EDIT_COOLDOWN );

	m_pDummyItem->sBasicOp.sSumID.wMainID = GetWin_Num_int ( this, IDC_EDIT_SUM_MID );
	m_pDummyItem->sBasicOp.sSumID.wSubID = GetWin_Num_int ( this, IDC_EDIT_SUM_SID );
	m_pDummyItem->sBasicOp.m_emSumType = (SUMMON_TYPE) GetWin_Combo_Sel ( this, IDC_COMBO_SUM_TYPE );

	m_pDummyItem->sBasicOp.dwSlotNum = GetWin_Num_int ( this, IDC_EDIT_SLOTNUM );

	//add abl
	m_pDummyItem->sBasicOp.strABLFile[CPSIND_M_LEFT] = GetWin_Text ( this, IDC_EDIT_ABL_M_L ).GetString();
	m_pDummyItem->sBasicOp.strABLFile[CPSIND_M_RIGHT] = GetWin_Text ( this, IDC_EDIT_ABL_M_R ).GetString();
	m_pDummyItem->sBasicOp.strABLFile[CPSIND_W_LEFT] = GetWin_Text ( this, IDC_EDIT_ABL_W_L ).GetString();
	m_pDummyItem->sBasicOp.strABLFile[CPSIND_W_RIGHT] = GetWin_Text ( this, IDC_EDIT_ABL_W_R ).GetString();

	m_pDummyItem->sBasicOp.strVPSFile = GetWin_Text ( this, IDC_EDIT_VEHICLEPART ).GetString();

	return TRUE;
}

void CItemParent::OnBnClickedButtonNext()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if ( !InverseUpdateItems() )
	{
		return;
	}

	CItemNode sItemNode;
	sItemNode.m_sItem = *m_pDummyItem;

	GLItemMan::GetInstance().InsertItem ( m_pDummyItem->sBasicOp.sNativeID, &sItemNode );

	GLStringTable::GetInstance().InsertString ( strKeyName, strName, GLStringTable::ITEM );
	GLStringTable::GetInstance().InsertString ( strKeyDesc, strDesc, GLStringTable::ITEM );

	m_pSheetTab->ActiveItemSuitPage ( m_CallPage, m_pItem );
	m_pItem = NULL;
}

void CItemParent::InitDefaultCtrls ()
{
	GetDlgItem ( IDC_BUTTON_PREV )->EnableWindow ( FALSE );

	SetWin_Combo_Init ( this, IDC_COMBO_LEVEL, COMMENT::ITEMLEVEL, LEVEL_NSIZE );
	SetWin_Combo_Init ( this, IDC_COMBO_ITEM_TYPE, COMMENT::ITEMTYPE, ITEM_NSIZE );

	// PET
	SetWin_Combo_Init ( this, IDC_COMBO_PET_TYPE, COMMENT::PET_TYPE, PETTYPE_NEWSIZE );

	SetWin_Combo_Init ( this, IDC_COMBO_SUM_TYPE, COMMENT::SUMMONTYPE, SUMMON_TYPE_SIZE );

	// VEHICLE
	SetWin_Combo_Init ( this, IDC_COMBO_VEHICLE_TYPE, COMMENT::VEHICLE_TYPE, VEHICLE_TYPE_SIZE );

	//cooldown
	SetWin_Combo_Init ( this, IDC_COMBO_COOLDOWN, COMMENT::COOLTYPE, EMCOOL_SIZE );

}

void CItemParent::OnBnClickedButtonInvenfile()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString szFilter = "Texture Image (*.bmp,*.dds,*.tga,*.jpg)|*.bmp;*.dds;*.tga;*.jpg|";
	
	//	Note : ���� ���� ���̾˷α׸� ����.
	CFileDialog dlg(TRUE,".x",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter,
		(CItemParent*)this);

	dlg.m_ofn.lpstrInitialDir = TextureManager::GetTexturePath(); 
		//DxSimpleMeshMan::GetInstance().GetPath ();
	if ( dlg.DoModal() == IDOK )
	{
		SetWin_Text ( this, IDC_EDIT_INVENFILE, dlg.GetFileName().GetString() );
	}
}

void CItemParent::OnBnClickedButtonFieldfile()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString szFilter = "Item File (*.x)|*.x|";
	
	//	Note : ���� ���� ���̾˷α׸� ����.
	CFileDialog dlg(TRUE,".x",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter,
		(CItemParent*)this);

	dlg.m_ofn.lpstrInitialDir = DxSimpleMeshMan::GetInstance().GetPath ();
	if ( dlg.DoModal() == IDOK )
	{
		SetWin_Text ( this, IDC_EDIT_FIELDFILE, dlg.GetFileName().GetString() );
	}
}



void CItemParent::OnBnClickedButtonSkinEffectfile()
{
	CString szFilter = "Effect File (*.effskin_a)|*.effskin_a|";
	
	//	Note : ���� ���� ���̾˷α׸� ����.
	CFileDialog dlg(TRUE,".effskin_a",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this);

	dlg.m_ofn.lpstrInitialDir = DxEffcharDataMan::GetInstance().GetPath ();
	if ( dlg.DoModal() == IDOK )
	{
		GetDlgItem ( IDC_EDIT_SKIN_EFFECTFILE )->SetWindowText( dlg.GetFileName().GetString() );		
	}
}

void CItemParent::OnBnClickedButtonEffectfile()
{
	CString szFilter = "Effect File (*.egp)|*.egp|";
	
	//	Note : ���� ���� ���̾˷α׸� ����.
	CFileDialog dlg(TRUE,".egp",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this);

	dlg.m_ofn.lpstrInitialDir = DxEffSinglePropGMan::GetInstance().GetPath ();
	if ( dlg.DoModal() == IDOK )
	{
		GetDlgItem ( IDC_EDIT_EFFECTFILE )->SetWindowText( dlg.GetFileName().GetString() );		
	}
}

void CItemParent::OnBnClickedButtonSkinTarEffectfile()
{
	CString szFilter = "Effect File (*.effskin_a)|*.effskin_a|";
	
	//	Note : ���� ���� ���̾˷α׸� ����.
	CFileDialog dlg(TRUE,".effskin_a",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this);

	dlg.m_ofn.lpstrInitialDir = DxEffcharDataMan::GetInstance().GetPath ();
	if ( dlg.DoModal() == IDOK )
	{
		GetDlgItem ( IDC_EDIT_TAR_EFFECTFILE )->SetWindowText( dlg.GetFileName().GetString() );		
	}
}

void CItemParent::OnEnChangeEditItemname()
{
	// TODO:  RICHEDIT ��Ʈ���� ��� �� ��Ʈ����
	// CPropertyPage::����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ�
	// CRichEditCtrl().SetEventMask()�� ȣ���ϵ��� OnInitDialog() �Լ��� ���������� ������
	// �� �˸��� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	const char * szTemp =  GLStringTable::GetInstance().GetString( GetWin_Text( this, IDC_EDIT_ITEMNAME ).GetString(),
																	GLStringTable::ITEM );
	if( szTemp )
	{
		SetWin_Text ( this, IDC_EDIT_NAME, szTemp );	
	}
	else
	{
		SetWin_Text ( this, IDC_EDIT_NAME, "" );
	}
}

void CItemParent::OnEnChangeEditItemdesc()
{
	// TODO:  RICHEDIT ��Ʈ���� ��� �� ��Ʈ����
	// CPropertyPage::����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ�
	// CRichEditCtrl().SetEventMask()�� ȣ���ϵ��� OnInitDialog() �Լ��� ���������� ������
	// �� �˸��� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	const char * szTemp = GLStringTable::GetInstance().GetString( GetWin_Text( this, IDC_EDIT_ITEMDESC ).GetString(), 
																	GLStringTable::ITEM );
	if( szTemp )
	{
		SetWin_Text ( this, IDC_EDIT_COMMENT, szTemp );	
	}
	else
	{
		SetWin_Text ( this, IDC_EDIT_COMMENT, "" );
	}
}


void CItemParent::OnCbnSelchangeComboItemType()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//m_pDummyItem->sBasicOp.emItemType = (EMITEM_TYPE) GetWin_Combo_Sel ( this, IDC_COMBO_ITEM_TYPE );

	//if ( m_pDummyItem->sBasicOp.emItemType != ITEM_PET_CARD ) 
	//{
	//	GetDlgItem ( IDC_COMBO_PET_TYPE )->EnableWindow ( FALSE );
	//	GetDlgItem ( IDC_EDIT_PET_MID )->EnableWindow ( FALSE );
	//	GetDlgItem ( IDC_EDIT_PET_SID )->EnableWindow ( FALSE );
	//}
	//else
	//{
	//	//GetDlgItem ( IDC_COMBO_PET_TYPE )->EnableWindow ();
	//	GetDlgItem ( IDC_EDIT_PET_MID )->EnableWindow ();
	//	GetDlgItem ( IDC_EDIT_PET_SID )->EnableWindow ();
	//}
}

void CItemParent::LoadWearingfile( int nID )
{
	CString szFilter = "Wearing File (*.cps,*.abl,*.vcf)|*.cps;*.abl;*.vcf|";
	
	//	Note : ���� ���� ���̾˷α׸� ����.
	CFileDialog dlg(TRUE,".cps",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter,
		(CItemParent*)this);

	dlg.m_ofn.lpstrInitialDir = DxSkinPieceContainer::GetInstance().GetPath ();
	if ( dlg.DoModal() == IDOK )
	{
		SetWin_Text ( this, nID, dlg.GetFileName().GetString() );
	}
}

void CItemParent::LoadVPSFile( int nID )
{
	CString szFilter = "Vehicle Piece (*.vps)|*.vps|";
	
	//	Note : ���� ���� ���̾˷α׸� ����.
	CFileDialog dlg(TRUE,".vps",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter,
		(CItemParent*)this);

	dlg.m_ofn.lpstrInitialDir = DxSkinPieceContainer::GetInstance().GetPath ();
	if ( dlg.DoModal() == IDOK )
	{
		SetWin_Text ( this, nID, dlg.GetFileName().GetString() );
	}
}
void CItemParent::LoadBIN( int nID )
{
	CString szFilter = "Item Random Option File (*.bin)|*.BIN|";
	
	CFileDialog dlg(TRUE,".bin",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter, this);

	dlg.m_ofn.lpstrInitialDir = GLOGIC::GetServerPath ();

	if ( dlg.DoModal() == IDOK )
	{
		SetWin_Text ( this, nID, dlg.GetFileName().GetString() );
	}
}

void CItemParent::OnBnClickedButtonWearingfileC1()
{
	LoadWearingfile( IDC_EDIT_WEARINGFILE_C1 );
}

void CItemParent::OnBnClickedButtonWearingfileC2()
{
	LoadWearingfile( IDC_EDIT_WEARINGFILE_C2 );
}

void CItemParent::OnBnClickedButtonWearingfileC3()
{
	LoadWearingfile( IDC_EDIT_WEARINGFILE_C3 );
}

void CItemParent::OnBnClickedButtonWearingfileC4()
{
	LoadWearingfile( IDC_EDIT_WEARINGFILE_C4 );
}

void CItemParent::OnBnClickedButtonWearingfileC5()
{
	LoadWearingfile( IDC_EDIT_WEARINGFILE_C5 );
}

void CItemParent::OnBnClickedButtonWearingfileC6()
{
	LoadWearingfile( IDC_EDIT_WEARINGFILE_C6 );
}

void CItemParent::OnBnClickedButtonWearingfileC7()
{
	LoadWearingfile( IDC_EDIT_WEARINGFILE_C7 );
}

void CItemParent::OnBnClickedButtonWearingfileC8()
{
	LoadWearingfile( IDC_EDIT_WEARINGFILE_C8 );
}


void CItemParent::OnBnClickedButtonWearingfileC9()
{
	LoadWearingfile( IDC_EDIT_WEARINGFILE_C9 );
}


void CItemParent::OnBnClickedButtonWearingfileC10()
{
	LoadWearingfile( IDC_EDIT_WEARINGFILE_C10 );
}

void CItemParent::OnBnClickedButtonWearingfileC11()
{
	LoadWearingfile( IDC_EDIT_WEARINGFILE_C11 );
}
//add class
void CItemParent::OnBnClickedButtonWearingfileC12()
{
	LoadWearingfile( IDC_EDIT_WEARINGFILE_C12 );
}
void CItemParent::OnBnClickedButtonWearingfileC13()
{
	LoadWearingfile( IDC_EDIT_WEARINGFILE_C13 );
}
void CItemParent::OnBnClickedButtonWearingfileC14()
{
	LoadWearingfile( IDC_EDIT_WEARINGFILE_C14 );
}
void CItemParent::OnBnClickedButtonWearingfileC15()
{
	LoadWearingfile( IDC_EDIT_WEARINGFILE_C15 );
}
void CItemParent::OnBnClickedButtonWearingfileC16()
{
	LoadWearingfile( IDC_EDIT_WEARINGFILE_C16 );
}
void CItemParent::OnBnClickedButtonWearingfileC17()
{
	LoadWearingfile( IDC_EDIT_WEARINGFILE_C17 );
}
void CItemParent::OnBnClickedButtonWearingfileC18()
{
	LoadWearingfile( IDC_EDIT_WEARINGFILE_C18 );
}
void CItemParent::OnBnClickedButtonWearingfileC20()
{
	LoadWearingfile( IDC_EDIT_WEARINGFILE_C20 );
}
//
////////////////

//////////////
void CItemParent::SetReadMode ()
{
//  �б����� ����϶��� �����Ѵ�. 
//  ���ҽ� �߰��� ���� ��� 
#ifdef READTOOL_PARAM

	const int nSkipNum = 6;
	const int nSkipID[nSkipNum] = { IDC_EDIT_NAME, IDC_EDIT_COMMENT, IDC_BUTTON_PREV,
									IDC_BUTTON_NEXT, IDC_BUTTON_CANCEL, IDC_BUTTON_OK };

	int nID = 0;
	bool bOK = false;

	CWnd* pChildWnd = GetWindow(GW_CHILD);

	while (pChildWnd)
	{
		bOK = false;
		nID = pChildWnd->GetDlgCtrlID();	

		for ( int i = 0; i < nSkipNum; ++i )
		{
			if ( nID == nSkipID[i] )
			{
				bOK = true;	
				break;
			}
		}

		if ( !bOK )	pChildWnd->EnableWindow( FALSE );

		pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT);
	}

#endif
}
void CItemParent::OnBnClickedRadioM()
{
	BOOL bCheck = GetWin_Check ( this, IDC_RADIO_M );

	SetWin_Check ( this, IDC_CHECK_ARCHER_W, false );
	SetWin_Check ( this, IDC_CHECK_SPIRIT_W, false );	
	SetWin_Check ( this, IDC_CHECK_FIGHTER_W, false );
	SetWin_Check ( this, IDC_CHECK_ARMS_W, false );	
	SetWin_Check ( this, IDC_CHECK_EXTREME_W, false );
	SetWin_Check ( this, IDC_CHECK_SCIENCE_W, false );
	SetWin_Check ( this, IDC_CHECK_ASSASIN_W, false );
	SetWin_Check ( this, IDC_CHECK_TESTING_W, false );
	SetWin_Check ( this, IDC_CHECK_TESTING_W2, false );

	SetWin_Check ( this, IDC_CHECK_FIGHTER_M, bCheck );
	SetWin_Check ( this, IDC_CHECK_ARMS_M, bCheck );	
	SetWin_Check ( this, IDC_CHECK_ARCHER_M, bCheck );
	SetWin_Check ( this, IDC_CHECK_SPIRIT_M, bCheck );	
	SetWin_Check ( this, IDC_CHECK_EXTREME_M, bCheck );
	SetWin_Check ( this, IDC_CHECK_SCIENCE_M, bCheck );
	SetWin_Check ( this, IDC_CHECK_ASSASIN_M, bCheck );
	SetWin_Check ( this, IDC_CHECK_TESTING_M, bCheck );
	SetWin_Check ( this, IDC_CHECK_TESTING_M2, bCheck );
}

void CItemParent::OnBnClickedRadioW()
{
	BOOL bCheck = GetWin_Check ( this, IDC_RADIO_W );

	SetWin_Check ( this, IDC_CHECK_ARCHER_W, bCheck );
	SetWin_Check ( this, IDC_CHECK_SPIRIT_W, bCheck );	
	SetWin_Check ( this, IDC_CHECK_FIGHTER_W, bCheck );
	SetWin_Check ( this, IDC_CHECK_ARMS_W, bCheck );
	SetWin_Check ( this, IDC_CHECK_EXTREME_W, bCheck );
	SetWin_Check ( this, IDC_CHECK_SCIENCE_W, bCheck );
	SetWin_Check ( this, IDC_CHECK_ASSASIN_W, bCheck );
	SetWin_Check ( this, IDC_CHECK_TESTING_W, bCheck );
	SetWin_Check ( this, IDC_CHECK_TESTING_W2, bCheck );

	SetWin_Check ( this, IDC_CHECK_FIGHTER_M, false );
	SetWin_Check ( this, IDC_CHECK_ARMS_M, false );	
	SetWin_Check ( this, IDC_CHECK_ARCHER_M, false );
	SetWin_Check ( this, IDC_CHECK_SPIRIT_M, false );	
	SetWin_Check ( this, IDC_CHECK_EXTREME_M, false );
	SetWin_Check ( this, IDC_CHECK_SCIENCE_M, false );
	SetWin_Check ( this, IDC_CHECK_ASSASIN_M, false );
	SetWin_Check ( this, IDC_CHECK_TESTING_M, false );
	SetWin_Check ( this, IDC_CHECK_TESTING_M2, false );
}

void CItemParent::OnBnClickedRadioAll()
{
	BOOL bCheck = GetWin_Check ( this, IDC_RADIO_ALL );

	SetWin_Check ( this, IDC_CHECK_FIGHTER_M, bCheck );
	SetWin_Check ( this, IDC_CHECK_ARMS_M, bCheck );
	SetWin_Check ( this, IDC_CHECK_ARCHER_W, bCheck );
	SetWin_Check ( this, IDC_CHECK_SPIRIT_W, bCheck );
	
	SetWin_Check ( this, IDC_CHECK_FIGHTER_W, bCheck );
	SetWin_Check ( this, IDC_CHECK_ARMS_W, bCheck );
	SetWin_Check ( this, IDC_CHECK_ARCHER_M, bCheck );
	SetWin_Check ( this, IDC_CHECK_SPIRIT_M, bCheck );
	
	SetWin_Check ( this, IDC_CHECK_EXTREME_M, bCheck );
	SetWin_Check ( this, IDC_CHECK_EXTREME_W, bCheck );
	SetWin_Check ( this, IDC_CHECK_SCIENCE_M, bCheck );
	SetWin_Check ( this, IDC_CHECK_SCIENCE_W, bCheck );
	SetWin_Check ( this, IDC_CHECK_ASSASIN_M, bCheck );
	SetWin_Check ( this, IDC_CHECK_ASSASIN_W, bCheck );
	SetWin_Check ( this, IDC_CHECK_TESTING_M, bCheck );
	SetWin_Check ( this, IDC_CHECK_TESTING_W, bCheck );
	SetWin_Check ( this, IDC_CHECK_TESTING_M2, bCheck );
	SetWin_Check ( this, IDC_CHECK_TESTING_W2, bCheck );
}

void CItemParent::OnBnClickedButtonRandomOpt()
{
	LoadBIN( IDC_EDIT_RANDOM_OPT );
}

void CItemParent::OnBnClickedButtonBr()
{
	BOOL bCheck1 = GetWin_Check ( this, IDC_CHECK_FIGHTER_M );
	BOOL bCheck2 = GetWin_Check ( this, IDC_CHECK_FIGHTER_W );

	if (bCheck1)
	{
		SetWin_Check ( this, IDC_CHECK_FIGHTER_M, 0 );
	}
	else
	{
		SetWin_Check ( this, IDC_CHECK_FIGHTER_M, 1 );
	}
	if (bCheck2)
	{
		SetWin_Check ( this, IDC_CHECK_FIGHTER_W, 0 );
	}
	else
	{
		SetWin_Check ( this, IDC_CHECK_FIGHTER_W, 1 );
	}
}

void CItemParent::OnBnClickedButtonSw()
{
	BOOL bCheck1 = GetWin_Check ( this, IDC_CHECK_ARMS_M );
	BOOL bCheck2 = GetWin_Check ( this, IDC_CHECK_ARMS_W );

	if (bCheck1)
	{
		SetWin_Check ( this, IDC_CHECK_ARMS_M, 0 );
	}
	else
	{
		SetWin_Check ( this, IDC_CHECK_ARMS_M, 1 );
	}
	if (bCheck2)
	{
		SetWin_Check ( this, IDC_CHECK_ARMS_W, 0 );
	}
	else
	{
		SetWin_Check ( this, IDC_CHECK_ARMS_W, 1 );
	}
}

void CItemParent::OnBnClickedButtonAr()
{
	BOOL bCheck1 = GetWin_Check ( this, IDC_CHECK_ARCHER_W );
	BOOL bCheck2 = GetWin_Check ( this, IDC_CHECK_ARCHER_M );

	if (bCheck1)
	{
		SetWin_Check ( this, IDC_CHECK_ARCHER_W, 0 );
	}
	else
	{
		SetWin_Check ( this, IDC_CHECK_ARCHER_W, 1 );
	}
	if (bCheck2)
	{
		SetWin_Check ( this, IDC_CHECK_ARCHER_M, 0 );
	}
	else
	{
		SetWin_Check ( this, IDC_CHECK_ARCHER_M, 1 );
	}
}

void CItemParent::OnBnClickedButtonSh()
{
	BOOL bCheck1 = GetWin_Check ( this, IDC_CHECK_SPIRIT_W );
	BOOL bCheck2 = GetWin_Check ( this, IDC_CHECK_SPIRIT_M );

	if (bCheck1)
	{
		SetWin_Check ( this, IDC_CHECK_SPIRIT_W, 0 );
	}
	else
	{
		SetWin_Check ( this, IDC_CHECK_SPIRIT_W, 1 );
	}
	if (bCheck2)
	{
		SetWin_Check ( this, IDC_CHECK_SPIRIT_M, 0 );
	}
	else
	{
		SetWin_Check ( this, IDC_CHECK_SPIRIT_M, 1 );
	}
}

void CItemParent::OnBnClickedButtonEx()
{
	BOOL bCheck1 = GetWin_Check ( this, IDC_CHECK_EXTREME_M );
	BOOL bCheck2 = GetWin_Check ( this, IDC_CHECK_EXTREME_W );

	if (bCheck1)
	{
		SetWin_Check ( this, IDC_CHECK_EXTREME_M, 0 );
	}
	else
	{
		SetWin_Check ( this, IDC_CHECK_EXTREME_M, 1 );
	}
	if (bCheck2)
	{
		SetWin_Check ( this, IDC_CHECK_EXTREME_W, 0 );
	}
	else
	{
		SetWin_Check ( this, IDC_CHECK_EXTREME_W, 1 );
	}
}

void CItemParent::OnBnClickedButtonGu()
{
	BOOL bCheck1 = GetWin_Check ( this, IDC_CHECK_SCIENCE_M );
	BOOL bCheck2 = GetWin_Check ( this, IDC_CHECK_SCIENCE_W );

	if (bCheck1)
	{
		SetWin_Check ( this, IDC_CHECK_SCIENCE_M, 0 );
	}
	else
	{
		SetWin_Check ( this, IDC_CHECK_SCIENCE_M, 1 );
	}
	if (bCheck2)
	{
		SetWin_Check ( this, IDC_CHECK_SCIENCE_W, 0 );
	}
	else
	{
		SetWin_Check ( this, IDC_CHECK_SCIENCE_W, 1 );
	}
}

void CItemParent::OnBnClickedButtonAs()
{
	BOOL bCheck1 = GetWin_Check ( this, IDC_CHECK_ASSASIN_M );
	BOOL bCheck2 = GetWin_Check ( this, IDC_CHECK_ASSASIN_W );

	if (bCheck1)
	{
		SetWin_Check ( this, IDC_CHECK_ASSASIN_M, 0 );
	}
	else
	{
		SetWin_Check ( this, IDC_CHECK_ASSASIN_M, 1 );
	}
	if (bCheck2)
	{
		SetWin_Check ( this, IDC_CHECK_ASSASIN_W, 0 );
	}
	else
	{
		SetWin_Check ( this, IDC_CHECK_ASSASIN_W, 1 );
	}
}
void CItemParent::OnBnClickedButtonTe2()
{
	BOOL bCheck1 = GetWin_Check ( this, IDC_CHECK_TESTING_M2 );
	BOOL bCheck2 = GetWin_Check ( this, IDC_CHECK_TESTING_W2 );

	if (bCheck1)
	{
		SetWin_Check ( this, IDC_CHECK_TESTING_M2, 0 );
	}
	else
	{
		SetWin_Check ( this, IDC_CHECK_TESTING_M2, 1 );
	}
	if (bCheck2)
	{
		SetWin_Check ( this, IDC_CHECK_TESTING_W2, 0 );
	}
	else
	{
		SetWin_Check ( this, IDC_CHECK_TESTING_W2, 1 );
	}
}
void CItemParent::OnBnClickedButtonTe()
{
	BOOL bCheck1 = GetWin_Check ( this, IDC_CHECK_TESTING_M );
	BOOL bCheck2 = GetWin_Check ( this, IDC_CHECK_TESTING_W );

	if (bCheck1)
	{
		SetWin_Check ( this, IDC_CHECK_TESTING_M, 0 );
	}
	else
	{
		SetWin_Check ( this, IDC_CHECK_TESTING_M, 1 );
	}
	if (bCheck2)
	{
		SetWin_Check ( this, IDC_CHECK_TESTING_W, 0 );
	}
	else
	{
		SetWin_Check ( this, IDC_CHECK_TESTING_W, 1 );
	}
}

void CItemParent::OnBnClickedButtonX()
{
	SetWin_Check ( this, IDC_CHECK_FIGHTER_M, 0 );
	SetWin_Check ( this, IDC_CHECK_ARMS_M, 0 );
	SetWin_Check ( this, IDC_CHECK_ARCHER_W, 0 );
	SetWin_Check ( this, IDC_CHECK_SPIRIT_W, 0 );
	
	SetWin_Check ( this, IDC_CHECK_FIGHTER_W, 0 );
	SetWin_Check ( this, IDC_CHECK_ARMS_W, 0 );
	SetWin_Check ( this, IDC_CHECK_ARCHER_M, 0 );
	SetWin_Check ( this, IDC_CHECK_SPIRIT_M, 0 );
	
	SetWin_Check ( this, IDC_CHECK_EXTREME_M, 0 );
	SetWin_Check ( this, IDC_CHECK_EXTREME_W, 0 );
	SetWin_Check ( this, IDC_CHECK_SCIENCE_M, 0 );
	SetWin_Check ( this, IDC_CHECK_SCIENCE_W, 0 );
	SetWin_Check ( this, IDC_CHECK_ASSASIN_M, 0 );
	SetWin_Check ( this, IDC_CHECK_ASSASIN_W, 0 );
	SetWin_Check ( this, IDC_CHECK_TESTING_M, 0 );
	SetWin_Check ( this, IDC_CHECK_TESTING_W, 0 );
	SetWin_Check ( this, IDC_CHECK_TESTING_M2, 0 );
	SetWin_Check ( this, IDC_CHECK_TESTING_W2, 0 );
}

void CItemParent::LoadABLFile( int nID )
{
	CString szFilter = "ABL File (*.abl)|*.abl|";
	
	//	Note : ���� ���� ���̾˷α׸� ����.
	CFileDialog dlg(TRUE,".abl",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter,
		(CItemParent*)this);

	dlg.m_ofn.lpstrInitialDir = DxSkinPieceRootDataContainer::GetInstance().GetPath ();
	if ( dlg.DoModal() == IDOK )
	{
		SetWin_Text ( this, nID, dlg.GetFileName().GetString() );
	}
}


void CItemParent::OnBnClickedButtonAblML()
{
	LoadABLFile ( IDC_EDIT_ABL_M_L );
}

void CItemParent::OnBnClickedButtonAblMR()
{
	LoadABLFile ( IDC_EDIT_ABL_M_R );
}

void CItemParent::OnBnClickedButtonAblWL()
{
	LoadABLFile ( IDC_EDIT_ABL_W_L );
}

void CItemParent::OnBnClickedButtonAblWR()
{
	LoadABLFile ( IDC_EDIT_ABL_W_R );
}



void CItemParent::OnBnClickedButtonVehiclepart()
{
	LoadVPSFile( IDC_EDIT_VEHICLEPART );
}




void CItemParent::OnEnChangeEditDwdummy1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CItemParent::OnBnClickedButtonWearingfile21()
{
	// TODO: Add your control notification handler code here
		LoadWearingfile( IDC_EDIT_STRDUMMY2_1 );
}

void CItemParent::OnBnClickedButtonWearingfile22()
{
		LoadWearingfile( IDC_EDIT_STRDUMMY2_2 );
}

void CItemParent::OnBnClickedButtonWearingfile23()
{
		LoadWearingfile( IDC_EDIT_STRDUMMY2_3 );
}

void CItemParent::OnBnClickedButtonWearingfile24()
{
		LoadWearingfile( IDC_EDIT_STRDUMMY2_4 );
}

void CItemParent::OnBnClickedButtonWearingfile25()
{
		LoadWearingfile( IDC_EDIT_STRDUMMY2_5 );
}

void CItemParent::OnBnClickedButtonWearingfile26()
{
		LoadWearingfile( IDC_EDIT_STRDUMMY2_6 );
}

void CItemParent::OnBnClickedButtonWearingfile27()
{
		LoadWearingfile( IDC_EDIT_STRDUMMY2_7 );}

void CItemParent::OnBnClickedButtonWearingfile28()
{
		LoadWearingfile( IDC_EDIT_STRDUMMY2_8 );}

void CItemParent::OnBnClickedButtonWearingfile29()
{
		LoadWearingfile( IDC_EDIT_STRDUMMY2_9 );}

void CItemParent::OnBnClickedButtonWearingfile210()
{
		LoadWearingfile( IDC_EDIT_STRDUMMY2_10 );}

void CItemParent::OnBnClickedButtonWearingfile211()
{
		LoadWearingfile( IDC_EDIT_STRDUMMY2_11 );}

void CItemParent::OnBnClickedButtonWearingfile212()
{
		LoadWearingfile( IDC_EDIT_STRDUMMY2_12 );}

void CItemParent::OnBnClickedButtonWearingfile213()
{
		LoadWearingfile( IDC_EDIT_STRDUMMY2_13 );}

void CItemParent::OnBnClickedButtonWearingfile214()
{
		LoadWearingfile( IDC_EDIT_STRDUMMY2_14 );}

void CItemParent::OnBnClickedButtonWearingfile215()
{
		LoadWearingfile( IDC_EDIT_STRDUMMY2_15 );}

void CItemParent::OnBnClickedButtonWearingfile216()
{
		LoadWearingfile( IDC_EDIT_STRDUMMY2_16 );}