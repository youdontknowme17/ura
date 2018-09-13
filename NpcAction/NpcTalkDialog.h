#pragma once

// CNpcTalkDialog ��ȭ �����Դϴ�.
#include "NpcTalk.h"

class	CNpcTalkControl;
class	CNpcDialogueSet;

class CNpcTalkDialog : public CDialog
{
	DECLARE_DYNAMIC(CNpcTalkDialog)

public:
	CNpcTalkDialog(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CNpcTalkDialog();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_NPCTALKDIALOG };

private:
	BOOL		m_bModify;
	SNpcTalk	m_Talk;

public:
	void	SetTalkData ( CNpcDialogueSet* pNpcDialogueSet, CNpcTalkControl* pTalkControl );
	void	SetModifyData ( BOOL bModify, SNpcTalk* pTalk );

private:
	CNpcTalkControl*	m_pTalkControl;
	CNpcDialogueSet*	m_pNpcDialogueSet;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnBnClickedButtonOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboActionType();
	afx_msg void OnCbnSelchangeComboActionData();
};
