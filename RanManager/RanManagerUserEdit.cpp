// RanManagerUserEdit.cpp : implementation file
//

#include "stdafx.h"
#include "RanManager.h"
#include "RanManagerUserEdit.h"
#include "RanManagerUserTab.h"

#include "COdbc.h"
#include "CCfg.h"
#include "s_NetGlobal.h"
#include "china_md5.h"
#include "Calendar.h"
#include "EtcFunction.h"
#include "LogScript.h"
#include "RanManagerComments.h"
#include ".\ranmanageruseredit.h"
// CRanManagerUserEdit dialog

IMPLEMENT_DYNAMIC(CRanManagerUserEdit, CPropertyPage)
CRanManagerUserEdit::CRanManagerUserEdit(LOGFONT logfont)
	: CPropertyPage(CRanManagerUserEdit::IDD)
	,m_pFont(NULL)
{
	m_bDlgInit = FALSE;
	m_pFont = new CFont();
	m_pFont->CreateFontIndirect(&logfont);
}

CRanManagerUserEdit::~CRanManagerUserEdit()
{
}

void CRanManagerUserEdit::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_USERTYPE, m_cUserType );
}


BEGIN_MESSAGE_MAP(CRanManagerUserEdit, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_MD5_1, OnBnClickedButtonMd51)
	ON_BN_CLICKED(IDC_BUTTON_MD5_2, OnBnClickedButtonMd52)
	ON_BN_CLICKED(IDC_BTN_EDIT_CREATEDATE, OnBnClickedBtnEditCreatedate)
	ON_BN_CLICKED(IDC_BTN_EDIT_LASTLOGIN, OnBnClickedBtnEditLastlogin)
	ON_BN_CLICKED(IDC_BTN_EDIT_USERBLOCK, OnBnClickedBtnEditUserblock)
	ON_BN_CLICKED(IDC_BTN_EDIT_PREMIUMDATE, OnBnClickedBtnEditPremiumdate)
	ON_BN_CLICKED(IDC_BTN_EDIT_CHATBLOCK, OnBnClickedBtnEditChatblock)
	ON_BN_CLICKED(IDC_BTN_EDIT_SAVE, OnBnClickedBtnEditSave)
	ON_BN_CLICKED(IDC_BTN_EDIT_CREATEDATE2, OnBnClickedBtnEditCreatedate2)
	ON_BN_CLICKED(IDC_BTN_EDIT_LASTLOGIN2, OnBnClickedBtnEditLastlogin2)
	ON_BN_CLICKED(IDC_BTN_EDIT_USERBLOCK2, OnBnClickedBtnEditUserblock2)
	ON_BN_CLICKED(IDC_BTN_EDIT_PREMIUMDATE2, OnBnClickedBtnEditPremiumdate2)
	ON_BN_CLICKED(IDC_BTN_EDIT_CHATBLOCK2, OnBnClickedBtnEditChatblock2)
	ON_BN_CLICKED(IDC_BTN_EDIT_CANCEL, OnBnClickedBtnEditCancel)
END_MESSAGE_MAP()


// CRanManagerUserEdit message handlers
BOOL CRanManagerUserEdit::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	m_bDlgInit = TRUE;

	for ( int i = 0; i < USER_TYPE_SIZE; ++i )
	{
		m_cUserType.InsertString( i, RMComments::m_UserType[i].c_str() );
	}
	m_cUserType.SetCurSel( 0 );

	return TRUE;  // return TRUE unless you set the focus to a control	
}

void CRanManagerUserEdit::SetUser(int nUser)
{
	LoadUser(nUser);
}

void CRanManagerUserEdit::LoadUser(int nNum )
{
	SQLRETURN	sReturn = 0; // SQL return value (short)	
	SQLHSTMT	hStmt; // Handle of statement (void*)

	sReturn = ::SQLAllocHandle(SQL_HANDLE_STMT, COdbc::GetInstance()->ConnectUser(), &hStmt);
	if (sReturn != SQL_SUCCESS && sReturn != SQL_SUCCESS_WITH_INFO)	return;

	TCHAR szTemp[1024] = {0};
	
	_snprintf( szTemp, 1024, 
		"SELECT UserNum,UserID,UserPass,UserPass2,UserType,UserLoginState,UserAvailable,CreateDate,LastLoginDate,SGNum,SvrNum,"
		"ChaName,UserBlock,UserBlockDate,ChaRemain,ChaTestRemain,PremiumDate,ChatBlockDate,UserEmail,UserPoint,OfflineTime,GameTime,UserVPoints,VegaPoints"
		" FROM %s.dbo.UserInfo WITH (NOLOCK) WHERE UserNum =%d "
	,CToolCfg::GetInstance()->szUserDb,nNum);

	sReturn = ::SQLExecDirect(hStmt,(SQLCHAR*) szTemp, SQL_NTS);
	std::string strTIME = CDebugSet::GetCurTime ();
	LOGSCRIPT::LogToFile("UserEdit.txt","%s--%s",strTIME.c_str(),szTemp);
	if ((sReturn != SQL_SUCCESS) && (sReturn != SQL_SUCCESS_WITH_INFO))	
	{
		::SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		return;
	}

	SQLINTEGER nUserNum = 0,						cbUserNum = SQL_NTS; 
	SQLCHAR    szCharId[CHR_ID_LENGTH+1] = {0};		SQLINTEGER cbUserId = SQL_NTS;
	SQLCHAR    szPass[CHR_ID_LENGTH+1] = {0};		SQLINTEGER cbPass = SQL_NTS;
	SQLCHAR    szPass2[CHR_ID_LENGTH+1] = {0};		SQLINTEGER cbPass2 = SQL_NTS;
	SQLINTEGER nUserType = 0,						cbUserType = SQL_NTS; 
	SQLINTEGER nUserLoginState = 0,					cbUserLoginState = SQL_NTS; 
	SQLINTEGER nUserAvailable = 0,					cbUserAvailable = SQL_NTS; 
	TIMESTAMP_STRUCT sCreateDate;					SQLINTEGER cbCreateDate   = SQL_NTS;
	TIMESTAMP_STRUCT sLastLoginDate;				SQLINTEGER cbLastLoginDate = SQL_NTS;
	SQLINTEGER nSGNum = 0,							cbSGNum = SQL_NTS; 
	SQLINTEGER nSvrNum = 0,							cbSvrNum = SQL_NTS; 
	SQLCHAR    szChaName[CHR_ID_LENGTH+1] = {0};	SQLINTEGER cbChaName = SQL_NTS;
	SQLINTEGER nUserBlock = 0,						cbUserBlock = SQL_NTS; 
	TIMESTAMP_STRUCT sUserBlockDate;				SQLINTEGER cbUserBlockDate = SQL_NTS;
	SQLINTEGER nChaRemain = 0,						cbChaRemain = SQL_NTS; 
	SQLINTEGER nChaTestRemain = 0,					cbChaTestRemain = SQL_NTS; 
	TIMESTAMP_STRUCT sPremiumDate;					SQLINTEGER cbPremiumDate = SQL_NTS;
	TIMESTAMP_STRUCT sChatBlockDate;				SQLINTEGER cbChatBlockDate = SQL_NTS;
	SQLCHAR    szUserEmail[CHR_ID_LENGTH+1] = {0};	SQLINTEGER cbUserEmail = SQL_NTS;
	SQLINTEGER nUserPoint = 0,						cbUserPoint = SQL_NTS; 
	SQLINTEGER nOfflineTime = 0,					cbOfflineTime = SQL_NTS; 
	SQLINTEGER nGameTime = 0,						cbGameTime = SQL_NTS; 
	SQLINTEGER nUserVPoints = 0,					cbUserVPoints = SQL_NTS; 
	SQLINTEGER nVegaPoints = 0,						cbVegaPoints = SQL_NTS; 


	while (true)
	{
		sReturn = ::SQLFetch(hStmt);
		if (sReturn == SQL_ERROR || sReturn == SQL_SUCCESS_WITH_INFO)	return;
		if (sReturn == SQL_SUCCESS || sReturn == SQL_SUCCESS_WITH_INFO)
		{
			::SQLGetData(hStmt, 1, SQL_C_LONG,		&nUserNum,			0,		&cbUserNum);	
			::SQLGetData(hStmt, 2, SQL_C_CHAR,		szCharId,			CHR_ID_LENGTH,	&cbUserId);
			::SQLGetData(hStmt, 3, SQL_C_CHAR,		szPass,				CHR_ID_LENGTH,	&cbPass);
			::SQLGetData(hStmt, 4, SQL_C_CHAR,		szPass2,			CHR_ID_LENGTH,  &cbPass2);//
			::SQLGetData(hStmt, 5, SQL_C_LONG,		&nUserType,			0,		&cbUserType);	//
			::SQLGetData(hStmt, 6, SQL_C_LONG,		&nUserLoginState,	0,		&cbUserLoginState);	//
			::SQLGetData(hStmt, 7, SQL_C_LONG,		&nUserAvailable,	0,		&cbUserAvailable);	//
			::SQLGetData(hStmt, 8, SQL_C_TYPE_TIMESTAMP,	&sCreateDate,		0,		&cbCreateDate);//
			::SQLGetData(hStmt, 9, SQL_C_TYPE_TIMESTAMP,	&sLastLoginDate,	0,		&cbLastLoginDate);//
			::SQLGetData(hStmt, 10, SQL_C_LONG,		&nSGNum,			0,		&cbSGNum);	//
			::SQLGetData(hStmt, 11, SQL_C_LONG,		&nSvrNum,			0,		&cbSvrNum);	//
			::SQLGetData(hStmt, 12, SQL_C_CHAR,		szChaName,			CHR_ID_LENGTH,  &cbChaName);//
			::SQLGetData(hStmt, 13, SQL_C_LONG,		&nUserBlock,		0,		&cbUserBlock);	//
			::SQLGetData(hStmt, 14, SQL_C_TYPE_TIMESTAMP,	&sUserBlockDate,	0,		&cbUserBlockDate);//
			::SQLGetData(hStmt, 15, SQL_C_LONG,		&nChaRemain,		0,		&cbChaRemain);	//
			::SQLGetData(hStmt, 16, SQL_C_LONG,		&nChaTestRemain,	0,		&cbChaTestRemain);	//
			::SQLGetData(hStmt, 17, SQL_C_TYPE_TIMESTAMP,	&sPremiumDate,	0,		&cbPremiumDate);//
			::SQLGetData(hStmt, 18, SQL_C_TYPE_TIMESTAMP,	&sChatBlockDate,	0,		&cbChatBlockDate);//
			::SQLGetData(hStmt, 19, SQL_C_CHAR,		szUserEmail,		CHR_ID_LENGTH,  &cbUserEmail);//
			::SQLGetData(hStmt, 20, SQL_C_LONG,		&nUserPoint,		0,		&cbUserPoint);	
			::SQLGetData(hStmt, 21, SQL_C_LONG,		&nOfflineTime,		0,		&cbOfflineTime);
			::SQLGetData(hStmt, 22, SQL_C_LONG,		&nGameTime,			0,		&cbGameTime);

			::SQLGetData(hStmt, 23, SQL_C_LONG,		&nUserVPoints,			0,		&cbUserVPoints);
			::SQLGetData(hStmt, 24, SQL_C_LONG,		&nVegaPoints,			0,		&cbVegaPoints);
		}
		else	break;
		Sleep( 0 );
	}

	SetWin_Num_int ( this, IDC_EDIT_USERNUM,nUserNum );
	SetWin_Text ( this, IDC_EDIT_USERID, (const TCHAR*)szCharId );
	SetWin_Text ( this, IDC_EDIT_USERPASS, (const TCHAR*)szPass );
	SetWin_Text ( this, IDC_EDIT_USERPASS2, (const TCHAR*)szPass2 );

	if (nUserType < 10 )	m_cUserType.SetCurSel(0);
	else if (nUserType == 10 )	m_cUserType.SetCurSel(1);
	else if (nUserType > 10 )	m_cUserType.SetCurSel(2);
	
	if ( nUserLoginState == 1 )	SetWin_Check ( this, IDC_CHECK_ONLINE, TRUE);
	else	SetWin_Check ( this, IDC_CHECK_ONLINE, FALSE);

	if ( nUserAvailable == 1 )	SetWin_Check ( this, IDC_CHECK_AVAILABLE, TRUE);
	else	SetWin_Check ( this, IDC_CHECK_AVAILABLE, FALSE);

	SetWin_Num_int ( this, IDC_EDIT_DATE1_1,sCreateDate.day);
	SetWin_Num_int ( this, IDC_EDIT_DATE1_2,sCreateDate.month);
	SetWin_Num_int ( this, IDC_EDIT_DATE1_3,sCreateDate.year);
	SetWin_Num_int ( this, IDC_EDIT_DATE1_4,sCreateDate.hour);
	SetWin_Num_int ( this, IDC_EDIT_DATE1_5,sCreateDate.minute);
	SetWin_Num_int ( this, IDC_EDIT_DATE1_6,sCreateDate.second);

	SetWin_Num_int ( this, IDC_EDIT_DATE2_1,sLastLoginDate.day);
	SetWin_Num_int ( this, IDC_EDIT_DATE2_2,sLastLoginDate.month);
	SetWin_Num_int ( this, IDC_EDIT_DATE2_3,sLastLoginDate.year);
	SetWin_Num_int ( this, IDC_EDIT_DATE2_4,sLastLoginDate.hour);
	SetWin_Num_int ( this, IDC_EDIT_DATE2_5,sLastLoginDate.minute);
	SetWin_Num_int ( this, IDC_EDIT_DATE2_6,sLastLoginDate.second);

	SetWin_Num_int ( this, IDC_EDIT_SGNUM,nSGNum );
	SetWin_Num_int ( this, IDC_EDIT_SVRNUM,nSvrNum );

	SetWin_Text ( this, IDC_EDIT_CHANAME, (const TCHAR*)szChaName );

	if ( nUserBlock == 1 )	SetWin_Check ( this, IDC_CHECK_USERBLOCK, TRUE);
	else	SetWin_Check ( this, IDC_CHECK_USERBLOCK, FALSE);

	SetWin_Num_int ( this, IDC_EDIT_DATE3_1,sUserBlockDate.day);
	SetWin_Num_int ( this, IDC_EDIT_DATE3_2,sUserBlockDate.month);
	SetWin_Num_int ( this, IDC_EDIT_DATE3_3,sUserBlockDate.year);
	SetWin_Num_int ( this, IDC_EDIT_DATE3_4,sUserBlockDate.hour);
	SetWin_Num_int ( this, IDC_EDIT_DATE3_5,sUserBlockDate.minute);
	SetWin_Num_int ( this, IDC_EDIT_DATE3_6,sUserBlockDate.second);

	SetWin_Num_int ( this, IDC_EDIT_CHAREMAIN,nChaRemain );
	SetWin_Num_int ( this, IDC_EDIT_CHATREMAIN,nChaTestRemain );

	SetWin_Num_int ( this, IDC_EDIT_DATE4_1,sPremiumDate.day);
	SetWin_Num_int ( this, IDC_EDIT_DATE4_2,sPremiumDate.month);
	SetWin_Num_int ( this, IDC_EDIT_DATE4_3,sPremiumDate.year);
	SetWin_Num_int ( this, IDC_EDIT_DATE4_4,sPremiumDate.hour);
	SetWin_Num_int ( this, IDC_EDIT_DATE4_5,sPremiumDate.minute);
	SetWin_Num_int ( this, IDC_EDIT_DATE4_6,sPremiumDate.second);

	SetWin_Num_int ( this, IDC_EDIT_DATE5_1,sChatBlockDate.day);
	SetWin_Num_int ( this, IDC_EDIT_DATE5_2,sChatBlockDate.month);
	SetWin_Num_int ( this, IDC_EDIT_DATE5_3,sChatBlockDate.year);
	SetWin_Num_int ( this, IDC_EDIT_DATE5_4,sChatBlockDate.hour);
	SetWin_Num_int ( this, IDC_EDIT_DATE5_5,sChatBlockDate.minute);
	SetWin_Num_int ( this, IDC_EDIT_DATE5_6,sChatBlockDate.second);

	SetWin_Text ( this, IDC_EDIT_USEREMAIL, (const TCHAR*)szUserEmail );
	SetWin_Num_int ( this, IDC_EDIT_USERP,nUserPoint);
	
	SetWin_Num_int ( this, IDC_EDIT_USERVP,nUserVPoints);
	SetWin_Num_int ( this, IDC_EDIT_USERVGP,nVegaPoints);

	::SQLFreeHandle(SQL_HANDLE_STMT, hStmt);	
}

void CRanManagerUserEdit::OnBnClickedButtonMd51()
{
	if ( MessageBox("Are you sure you want to convert UserPass to MD5?","Question",MB_YESNO) == IDYES )
	ConvertMD5( GetWin_Text( this, IDC_EDIT_USERPASS ).GetString(),1);
}

void CRanManagerUserEdit::OnBnClickedButtonMd52()
{
	if ( MessageBox("Are you sure you want to convert UserPass2 to MD5?","Question",MB_YESNO) == IDYES )
	ConvertMD5( GetWin_Text( this, IDC_EDIT_USERPASS2 ).GetString(),2);
}

void CRanManagerUserEdit::ConvertMD5(const TCHAR* szText,int nType)
{
	CHINA_MD5::MD5 md5; 
	size_t nLength = 0;
	StringCchLength( szText, USR_PASS_LENGTH, &nLength );
	md5.update( reinterpret_cast<const unsigned char*> (szText), nLength );	
	std::string strOut = md5.asString();

	TCHAR	szTemp[25];
	StringCchCopy( szTemp, USR_PASS_LENGTH, strOut.c_str() );
	m_Tea.encrypt( szTemp, USR_PASS_LENGTH );

	switch (nType)
	{
		case 1:	SetWin_Text ( this, IDC_EDIT_USERPASS, szTemp );	break;
		case 2:	SetWin_Text ( this, IDC_EDIT_USERPASS2, szTemp );	break;
	};
}
void CRanManagerUserEdit::OnBnClickedBtnEditCreatedate()
{
	CCalendar sCalender;
	SYSTEMTIME sNewTime;
	sNewTime.wDay = GetWin_Num_int ( this, IDC_EDIT_DATE1_1 );
	sNewTime.wMonth = GetWin_Num_int ( this, IDC_EDIT_DATE1_2 );
	sNewTime.wYear = GetWin_Num_int ( this, IDC_EDIT_DATE1_3 );
	sNewTime.wHour = GetWin_Num_int ( this, IDC_EDIT_DATE1_4 );
	sNewTime.wMinute = GetWin_Num_int ( this, IDC_EDIT_DATE1_5 );
	sNewTime.wSecond = GetWin_Num_int ( this, IDC_EDIT_DATE1_6 );
	sCalender.SetTime(sNewTime);

	if ( IDOK == sCalender.DoModal () )
	{
		SetWin_Num_int ( this, IDC_EDIT_DATE1_1,sCalender.GetTime().wDay);
		SetWin_Num_int ( this, IDC_EDIT_DATE1_2,sCalender.GetTime().wMonth);
		SetWin_Num_int ( this, IDC_EDIT_DATE1_3,sCalender.GetTime().wYear);
		SetWin_Num_int ( this, IDC_EDIT_DATE1_4,sCalender.GetTime().wHour);
		SetWin_Num_int ( this, IDC_EDIT_DATE1_5,sCalender.GetTime().wMinute);
		SetWin_Num_int ( this, IDC_EDIT_DATE1_6,sCalender.GetTime().wSecond);
	}
}

void CRanManagerUserEdit::OnBnClickedBtnEditLastlogin()
{
	CCalendar sCalender;
	SYSTEMTIME sNewTime;
	sNewTime.wDay = GetWin_Num_int ( this, IDC_EDIT_DATE2_1 );
	sNewTime.wMonth = GetWin_Num_int ( this, IDC_EDIT_DATE2_2 );
	sNewTime.wYear = GetWin_Num_int ( this, IDC_EDIT_DATE2_3 );
	sNewTime.wHour = GetWin_Num_int ( this, IDC_EDIT_DATE2_4 );
	sNewTime.wMinute = GetWin_Num_int ( this, IDC_EDIT_DATE2_5 );
	sNewTime.wSecond = GetWin_Num_int ( this, IDC_EDIT_DATE2_6 );
	sCalender.SetTime(sNewTime);

	if ( IDOK == sCalender.DoModal () )
	{
		SetWin_Num_int ( this, IDC_EDIT_DATE2_1,sCalender.GetTime().wDay);
		SetWin_Num_int ( this, IDC_EDIT_DATE2_2,sCalender.GetTime().wMonth);
		SetWin_Num_int ( this, IDC_EDIT_DATE2_3,sCalender.GetTime().wYear);
		SetWin_Num_int ( this, IDC_EDIT_DATE2_4,sCalender.GetTime().wHour);
		SetWin_Num_int ( this, IDC_EDIT_DATE2_5,sCalender.GetTime().wMinute);
		SetWin_Num_int ( this, IDC_EDIT_DATE2_6,sCalender.GetTime().wSecond);
	}
}

void CRanManagerUserEdit::OnBnClickedBtnEditUserblock()
{
	CCalendar sCalender;
	SYSTEMTIME sNewTime;
	sNewTime.wDay = GetWin_Num_int ( this, IDC_EDIT_DATE3_1 );
	sNewTime.wMonth = GetWin_Num_int ( this, IDC_EDIT_DATE3_2 );
	sNewTime.wYear = GetWin_Num_int ( this, IDC_EDIT_DATE3_3 );
	sNewTime.wHour = GetWin_Num_int ( this, IDC_EDIT_DATE3_4 );
	sNewTime.wMinute = GetWin_Num_int ( this, IDC_EDIT_DATE3_5 );
	sNewTime.wSecond = GetWin_Num_int ( this, IDC_EDIT_DATE3_6 );
	sCalender.SetTime(sNewTime);

	if ( IDOK == sCalender.DoModal () )
	{
		SetWin_Num_int ( this, IDC_EDIT_DATE3_1,sCalender.GetTime().wDay);
		SetWin_Num_int ( this, IDC_EDIT_DATE3_2,sCalender.GetTime().wMonth);
		SetWin_Num_int ( this, IDC_EDIT_DATE3_3,sCalender.GetTime().wYear);
		SetWin_Num_int ( this, IDC_EDIT_DATE3_4,sCalender.GetTime().wHour);
		SetWin_Num_int ( this, IDC_EDIT_DATE3_5,sCalender.GetTime().wMinute);
		SetWin_Num_int ( this, IDC_EDIT_DATE3_6,sCalender.GetTime().wSecond);
	}
}

void CRanManagerUserEdit::OnBnClickedBtnEditPremiumdate()
{
	CCalendar sCalender;
	SYSTEMTIME sNewTime;
	sNewTime.wDay = GetWin_Num_int ( this, IDC_EDIT_DATE4_1 );
	sNewTime.wMonth = GetWin_Num_int ( this, IDC_EDIT_DATE4_2 );
	sNewTime.wYear = GetWin_Num_int ( this, IDC_EDIT_DATE4_3 );
	sNewTime.wHour = GetWin_Num_int ( this, IDC_EDIT_DATE4_4 );
	sNewTime.wMinute = GetWin_Num_int ( this, IDC_EDIT_DATE4_5 );
	sNewTime.wSecond = GetWin_Num_int ( this, IDC_EDIT_DATE4_6 );
	sCalender.SetTime(sNewTime);

	if ( IDOK == sCalender.DoModal () )
	{
		SetWin_Num_int ( this, IDC_EDIT_DATE4_1,sCalender.GetTime().wDay);
		SetWin_Num_int ( this, IDC_EDIT_DATE4_2,sCalender.GetTime().wMonth);
		SetWin_Num_int ( this, IDC_EDIT_DATE4_3,sCalender.GetTime().wYear);
		SetWin_Num_int ( this, IDC_EDIT_DATE4_4,sCalender.GetTime().wHour);
		SetWin_Num_int ( this, IDC_EDIT_DATE4_5,sCalender.GetTime().wMinute);
		SetWin_Num_int ( this, IDC_EDIT_DATE4_6,sCalender.GetTime().wSecond);
	}
}

void CRanManagerUserEdit::OnBnClickedBtnEditChatblock()
{
	CCalendar sCalender;
	SYSTEMTIME sNewTime;
	sNewTime.wDay = GetWin_Num_int ( this, IDC_EDIT_DATE5_1 );
	sNewTime.wMonth = GetWin_Num_int ( this, IDC_EDIT_DATE5_2 );
	sNewTime.wYear = GetWin_Num_int ( this, IDC_EDIT_DATE5_3 );
	sNewTime.wHour = GetWin_Num_int ( this, IDC_EDIT_DATE5_4 );
	sNewTime.wMinute = GetWin_Num_int ( this, IDC_EDIT_DATE5_5 );
	sNewTime.wSecond = GetWin_Num_int ( this, IDC_EDIT_DATE5_6 );
	sCalender.SetTime(sNewTime);

	if ( IDOK == sCalender.DoModal () )
	{
		SetWin_Num_int ( this, IDC_EDIT_DATE5_1,sCalender.GetTime().wDay);
		SetWin_Num_int ( this, IDC_EDIT_DATE5_2,sCalender.GetTime().wMonth);
		SetWin_Num_int ( this, IDC_EDIT_DATE5_3,sCalender.GetTime().wYear);
		SetWin_Num_int ( this, IDC_EDIT_DATE5_4,sCalender.GetTime().wHour);
		SetWin_Num_int ( this, IDC_EDIT_DATE5_5,sCalender.GetTime().wMinute);
		SetWin_Num_int ( this, IDC_EDIT_DATE5_6,sCalender.GetTime().wSecond);
	}
}

void CRanManagerUserEdit::OnBnClickedBtnEditSave()
{
	SQLRETURN	sReturn = 0; // SQL return value (short)	
	SQLHSTMT	hStmt; // Handle of statement (void*)

	sReturn = ::SQLAllocHandle(SQL_HANDLE_STMT, COdbc::GetInstance()->ConnectUser(), &hStmt);
	if (sReturn != SQL_SUCCESS && sReturn != SQL_SUCCESS_WITH_INFO)	return;

	TCHAR szTemp[1024] = {0};
	
	int nUserTypeTemp,nUserType;
	nUserTypeTemp = m_cUserType.GetCurSel();
	switch (nUserTypeTemp)
	{
	case 0:	nUserType = 1; break;
	case 1:	nUserType = 10; break;
	case 2:	nUserType = 30; break;
	};
	
	BOOL bUserLogin = GetWin_Check ( this, IDC_CHECK_ONLINE );
	int nUserLogin;
	if ( bUserLogin )	nUserLogin = 1;
	else nUserLogin = 0;

	BOOL bUserAvailable = GetWin_Check ( this, IDC_CHECK_AVAILABLE );
	int nUserAvailable;
	if ( bUserAvailable )	nUserAvailable = 1;
	else nUserAvailable = 0;

	BOOL bUserBlock = GetWin_Check ( this, IDC_CHECK_USERBLOCK );
	int nUserBlock;
	if ( bUserBlock )	nUserBlock = 1;
	else nUserBlock = 0;

	CString strCreateDate;
	strCreateDate.Format("%d/%d/%d %d:%d:%d"
		,GetWin_Num_int ( this, IDC_EDIT_DATE1_2 )
		,GetWin_Num_int ( this, IDC_EDIT_DATE1_1 )
		,GetWin_Num_int ( this, IDC_EDIT_DATE1_3 )
		,GetWin_Num_int ( this, IDC_EDIT_DATE1_4 )
		,GetWin_Num_int ( this, IDC_EDIT_DATE1_5 )
		,GetWin_Num_int ( this, IDC_EDIT_DATE1_6 )
		);

	CString strLastLoginDate;
	strLastLoginDate.Format("%d/%d/%d %d:%d:%d"
		,GetWin_Num_int ( this, IDC_EDIT_DATE2_2 )
		,GetWin_Num_int ( this, IDC_EDIT_DATE2_1 )
		,GetWin_Num_int ( this, IDC_EDIT_DATE2_3 )
		,GetWin_Num_int ( this, IDC_EDIT_DATE2_4 )
		,GetWin_Num_int ( this, IDC_EDIT_DATE2_5 )
		,GetWin_Num_int ( this, IDC_EDIT_DATE2_6 )
		);

	CString strUserBlock;
	strUserBlock.Format("%d/%d/%d %d:%d:%d"
		,GetWin_Num_int ( this, IDC_EDIT_DATE3_2 )
		,GetWin_Num_int ( this, IDC_EDIT_DATE3_1 )
		,GetWin_Num_int ( this, IDC_EDIT_DATE3_3 )
		,GetWin_Num_int ( this, IDC_EDIT_DATE3_4 )
		,GetWin_Num_int ( this, IDC_EDIT_DATE3_5 )
		,GetWin_Num_int ( this, IDC_EDIT_DATE3_6 )
		);

	CString strPremiumDate;
	strPremiumDate.Format("%d/%d/%d %d:%d:%d"
		,GetWin_Num_int ( this, IDC_EDIT_DATE4_2 )
		,GetWin_Num_int ( this, IDC_EDIT_DATE4_1 )
		,GetWin_Num_int ( this, IDC_EDIT_DATE4_3 )
		,GetWin_Num_int ( this, IDC_EDIT_DATE4_4 )
		,GetWin_Num_int ( this, IDC_EDIT_DATE4_5 )
		,GetWin_Num_int ( this, IDC_EDIT_DATE4_6 )
		);

	CString strChatBlockDate;
	strChatBlockDate.Format("%d/%d/%d %d:%d:%d"
		,GetWin_Num_int ( this, IDC_EDIT_DATE5_2 )
		,GetWin_Num_int ( this, IDC_EDIT_DATE5_1 )
		,GetWin_Num_int ( this, IDC_EDIT_DATE5_3 )
		,GetWin_Num_int ( this, IDC_EDIT_DATE5_4 )
		,GetWin_Num_int ( this, IDC_EDIT_DATE5_5 )
		,GetWin_Num_int ( this, IDC_EDIT_DATE5_6 )
		);

	_snprintf( szTemp, 1024, 
		"Update %s.dbo.UserInfo Set "
		"UserName='%s',UserID='%s',UserPass='%s',UserPass2='%s',UserType=%d,UserLoginState=%d,UserAvailable=%d,CreateDate='%s',LastLoginDate='%s',SGNum=%d,SvrNum=%d,"
		"ChaName='%s',UserBlock=%d,UserBlockDate='%s',ChaRemain=%d,ChaTestRemain=%d,PremiumDate='%s',ChatBlockDate='%s',UserEmail='%s',UserPoint=%d,UserVPoints=%d,VegaPoints=%d"
		"  WHERE UserNum =%d "
	,CToolCfg::GetInstance()->szUserDb
	,(const TCHAR*)GetWin_Text( this, IDC_EDIT_USERID ).GetString()
	,(const TCHAR*)GetWin_Text( this, IDC_EDIT_USERID ).GetString()
	,(const TCHAR*)GetWin_Text( this, IDC_EDIT_USERPASS ).GetString()
	,(const TCHAR*)GetWin_Text( this, IDC_EDIT_USERPASS2 ).GetString()
	,nUserType
	,nUserLogin
	,nUserAvailable
	,strCreateDate.GetString()
	,strLastLoginDate.GetString()
	,GetWin_Num_int ( this, IDC_EDIT_SGNUM )
	,GetWin_Num_int ( this, IDC_EDIT_SVRNUM )
	,(const TCHAR*)GetWin_Text( this, IDC_EDIT_CHANAME ).GetString()
	,nUserBlock
	,strUserBlock.GetString()
	,GetWin_Num_int ( this, IDC_EDIT_CHAREMAIN )
	,GetWin_Num_int ( this, IDC_EDIT_CHATREMAIN )
	,strPremiumDate.GetString()
	,strChatBlockDate.GetString()
	,(const TCHAR*)GetWin_Text( this, IDC_EDIT_USEREMAIL ).GetString()
	,GetWin_Num_int ( this, IDC_EDIT_USERP )
	,GetWin_Num_int ( this, IDC_EDIT_USERVP )
	,GetWin_Num_int ( this, IDC_EDIT_USERVGP )
	,GetWin_Num_int ( this, IDC_EDIT_USERNUM )
	);

	sReturn = ::SQLExecDirect(hStmt,(SQLCHAR*) szTemp, SQL_NTS);
	std::string strTIME = CDebugSet::GetCurTime ();
	LOGSCRIPT::LogToFile("UserEdit.txt","%s--%s",strTIME.c_str(),szTemp);

	if (sReturn == SQL_ERROR )	MessageBox("Error! Saving User Data","Ran Manager",MB_OK);
	if (sReturn == SQL_SUCCESS || sReturn == SQL_SUCCESS_WITH_INFO)	MessageBox("Success! Saving User Data","Ran Manager",MB_OK);

	::SQLFreeHandle(SQL_HANDLE_STMT, hStmt);	
}



void CRanManagerUserEdit::OnBnClickedBtnEditCreatedate2()
{
	SetWin_Num_int ( this, IDC_EDIT_DATE1_1,1);
	SetWin_Num_int ( this, IDC_EDIT_DATE1_2,2);
	SetWin_Num_int ( this, IDC_EDIT_DATE1_3,1970);
	SetWin_Num_int ( this, IDC_EDIT_DATE1_4,1);
	SetWin_Num_int ( this, IDC_EDIT_DATE1_5,1);
	SetWin_Num_int ( this, IDC_EDIT_DATE1_6,1);
}

void CRanManagerUserEdit::OnBnClickedBtnEditLastlogin2()
{
	SetWin_Num_int ( this, IDC_EDIT_DATE2_1,1);
	SetWin_Num_int ( this, IDC_EDIT_DATE2_2,2);
	SetWin_Num_int ( this, IDC_EDIT_DATE2_3,1970);
	SetWin_Num_int ( this, IDC_EDIT_DATE2_4,1);
	SetWin_Num_int ( this, IDC_EDIT_DATE2_5,1);
	SetWin_Num_int ( this, IDC_EDIT_DATE2_6,1);
}

void CRanManagerUserEdit::OnBnClickedBtnEditUserblock2()
{
	SetWin_Num_int ( this, IDC_EDIT_DATE3_1,1);
	SetWin_Num_int ( this, IDC_EDIT_DATE3_2,2);
	SetWin_Num_int ( this, IDC_EDIT_DATE3_3,1970);
	SetWin_Num_int ( this, IDC_EDIT_DATE3_4,1);
	SetWin_Num_int ( this, IDC_EDIT_DATE3_5,1);
	SetWin_Num_int ( this, IDC_EDIT_DATE3_6,1);
}

void CRanManagerUserEdit::OnBnClickedBtnEditPremiumdate2()
{
	SetWin_Num_int ( this, IDC_EDIT_DATE4_1,1);
	SetWin_Num_int ( this, IDC_EDIT_DATE4_2,2);
	SetWin_Num_int ( this, IDC_EDIT_DATE4_3,1970);
	SetWin_Num_int ( this, IDC_EDIT_DATE4_4,1);
	SetWin_Num_int ( this, IDC_EDIT_DATE4_5,1);
	SetWin_Num_int ( this, IDC_EDIT_DATE4_6,1);
}

void CRanManagerUserEdit::OnBnClickedBtnEditChatblock2()
{
	SetWin_Num_int ( this, IDC_EDIT_DATE5_1,1);
	SetWin_Num_int ( this, IDC_EDIT_DATE5_2,2);
	SetWin_Num_int ( this, IDC_EDIT_DATE5_3,1970);
	SetWin_Num_int ( this, IDC_EDIT_DATE5_4,1);
	SetWin_Num_int ( this, IDC_EDIT_DATE5_5,1);
	SetWin_Num_int ( this, IDC_EDIT_DATE5_6,1);
}

void CRanManagerUserEdit::OnBnClickedBtnEditCancel()
{
	m_pToolTab->ActivePage(USERPAGE_MAIN,0);
}
