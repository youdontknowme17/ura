#pragma once
#include <string>

// CItemParent ��ȭ �����Դϴ�.
class	CsheetWithTab;
struct	SITEM;

class CItemParent : public CPropertyPage
{
	DECLARE_DYNAMIC(CItemParent)

public:
	CItemParent( LOGFONT logfont );
	virtual ~CItemParent();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ITEMPARENT_DIALOG };

protected:
	CsheetWithTab*	m_pSheetTab;
	int				m_CallPage;

	SITEM*			m_pDummyItem;
	SITEM*			m_pItem;

	BOOL			m_bDlgInit;

	std::string		strKeyName;
	std::string		strName;
	std::string		strKeyDesc;
	std::string		strDesc;
	CFont*			m_pFont;



public:
	void	SetCallPage ( int CallPage ) { m_CallPage = CallPage; }
	void	SetSheetTab ( CsheetWithTab* pSheetTab ) { m_pSheetTab = pSheetTab; }
	BOOL	SetItem ( SITEM* pItem );

protected:
	void	UpdateItems ();
	BOOL	InverseUpdateItems ();
	void	InitDefaultCtrls ();
	void	LoadWearingfile( int nID );
	void	LoadBIN( int nID );
	void	LoadABLFile( int nID );
	void	LoadVPSFile( int nID );

	//////////////////////////////////////////////////////////////////////////////
	//	�����丵 �������
public:
	void	InitAllItem ();
	void	SetReadMode ();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnBnClickedButtonOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonNext();
	afx_msg void OnBnClickedButtonInvenfile();
	afx_msg void OnBnClickedButtonFieldfile();
	
	afx_msg void OnBnClickedButtonWearingfileC1();
	afx_msg void OnBnClickedButtonWearingfileC2();
	afx_msg void OnBnClickedButtonWearingfileC3();
	afx_msg void OnBnClickedButtonWearingfileC4();
	afx_msg void OnBnClickedButtonWearingfileC5();
	afx_msg void OnBnClickedButtonWearingfileC6();
	afx_msg void OnBnClickedButtonWearingfileC7();
	afx_msg void OnBnClickedButtonWearingfileC8();
	afx_msg void OnBnClickedButtonWearingfileC9();
	afx_msg void OnBnClickedButtonWearingfileC10();
	afx_msg void OnBnClickedButtonWearingfileC11();
	//add class
	afx_msg void OnBnClickedButtonWearingfileC12();
	afx_msg void OnBnClickedButtonWearingfileC13();
	afx_msg void OnBnClickedButtonWearingfileC14();
	afx_msg void OnBnClickedButtonWearingfileC15();
	afx_msg void OnBnClickedButtonWearingfileC16();
	afx_msg void OnBnClickedButtonWearingfileC17();
	afx_msg void OnBnClickedButtonWearingfileC18();
	afx_msg void OnBnClickedButtonWearingfileC20();
	//add juver
	//
	afx_msg void OnBnClickedButtonWearingfileC1B();
	afx_msg void OnBnClickedButtonWearingfileC2B();
	afx_msg void OnBnClickedButtonWearingfileC3B();
	afx_msg void OnBnClickedButtonWearingfileC4B();
	afx_msg void OnBnClickedButtonWearingfileC5B();
	afx_msg void OnBnClickedButtonWearingfileC6B();
	afx_msg void OnBnClickedButtonWearingfileC7B();
	afx_msg void OnBnClickedButtonWearingfileC8B();
	afx_msg void OnBnClickedButtonWearingfileC9B();
	afx_msg void OnBnClickedButtonWearingfileC10B();
	afx_msg void OnBnClickedButtonWearingfileC11B();
	//add class
	afx_msg void OnBnClickedButtonWearingfileC12B();
	afx_msg void OnBnClickedButtonWearingfileC13B();
	afx_msg void OnBnClickedButtonWearingfileC14B();
	afx_msg void OnBnClickedButtonWearingfileC15B();
	afx_msg void OnBnClickedButtonWearingfileC16B();
	afx_msg void OnBnClickedButtonWearingfileC17B();

	afx_msg void OnBnClickedButtonSkinEffectfile();
	afx_msg void OnBnClickedButtonEffectfile();
	afx_msg void OnBnClickedButtonSkinTarEffectfile();
	afx_msg void OnEnChangeEditItemname();
	afx_msg void OnEnChangeEditItemdesc();

	afx_msg void OnCbnSelchangeComboItemType();
	afx_msg void OnBnClickedRadioM();
	afx_msg void OnBnClickedRadioW();
	afx_msg void OnBnClickedRadioAll();
	afx_msg void OnBnClickedButtonRandomOpt();
	afx_msg void OnBnClickedButtonBr();
	afx_msg void OnBnClickedButtonSw();
	afx_msg void OnBnClickedButtonAr();
	afx_msg void OnBnClickedButtonSh();
	afx_msg void OnBnClickedButtonEx();
	afx_msg void OnBnClickedButtonGu();
	afx_msg void OnBnClickedButtonAs();
	afx_msg void OnBnClickedButtonTe();
	afx_msg void OnBnClickedButtonTe2();
	afx_msg void OnBnClickedButtonX();
	
	
	afx_msg void OnBnClickedButtonAblML();
	afx_msg void OnBnClickedButtonAblMR();
	afx_msg void OnBnClickedButtonAblWL();
	afx_msg void OnBnClickedButtonAblWR();
	afx_msg void OnBnClickedButtonVehiclepart();

	afx_msg void OnEnChangeEditFieldfile();
	afx_msg void OnEnChangeEditDwdummy1();
	afx_msg void OnBnClickedButtonWearingfile21();
	afx_msg void OnBnClickedButtonWearingfile22();
	afx_msg void OnBnClickedButtonWearingfile23();
	afx_msg void OnBnClickedButtonWearingfile24();
	afx_msg void OnBnClickedButtonWearingfile25();
	afx_msg void OnBnClickedButtonWearingfile26();
	afx_msg void OnBnClickedButtonWearingfile27();
	afx_msg void OnBnClickedButtonWearingfile28();
	afx_msg void OnBnClickedButtonWearingfile29();
	afx_msg void OnBnClickedButtonWearingfile210();
	afx_msg void OnBnClickedButtonWearingfile211();
	afx_msg void OnBnClickedButtonWearingfile212();
	afx_msg void OnBnClickedButtonWearingfile213();
	afx_msg void OnBnClickedButtonWearingfile214();
	afx_msg void OnBnClickedButtonWearingfile215();
	afx_msg void OnBnClickedButtonWearingfile216();
};
