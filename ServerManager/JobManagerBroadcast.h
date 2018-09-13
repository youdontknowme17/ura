#pragma once
#include "afxwin.h"


// CJobManagerBroadcast ��ȭ �����Դϴ�.

class CJobManagerBroadcast : public CDialog
{
	DECLARE_DYNAMIC(CJobManagerBroadcast)

public:
	CJobManagerBroadcast(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CJobManagerBroadcast();

	void InitControl();
	void SetInitData( std::vector<int> vecGroupList, CString strMsg, int nType, std::vector<int> vecGroupNum, 
					  int nHour=0, int nMin=0, int nDayOfWeek=0, int nMonth=0, int nDay=0, int nYear=0 );

	int m_nHour;
	int m_nMin;
	int m_nType;
	int	m_nDayOfWeek; 
	int	m_nYear;	  
	int	m_nMonth;	  
	int	m_nDay;		  
	std::vector<int> m_vecGroupNum;
	std::vector<int> m_vecGroupList;
 	CString m_strBroadcastMsg;


// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_JOB_BROADCAST_MESSAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_ComboHour;
	CComboBox m_ComboMin;
	CComboBox m_ComboType;
	CString m_strMsg;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedSendType2();
	afx_msg void OnBnClickedSendType1();
	afx_msg void OnCbnSelchangeComboType();
	afx_msg void OnMcnSelchangeMonthcalendar1(NMHDR *pNMHDR, LRESULT *pResult);

	void SetGroupList( std::vector<int> vecGroupList ) { m_vecGroupList = vecGroupList; }
};
