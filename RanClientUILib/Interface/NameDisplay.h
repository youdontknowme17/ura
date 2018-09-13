#pragma	once

#include "../EngineUIlib/GUInterface/UIGroup.h"
#include "glcrowrenlist.h"

class CBasicTextBox;
class CBasicLineBoxEx;
class CBasicProgressBar;

class CNameDisplay : public CUIGroup
{
public:
	enum
	{
		nSCHOOLMARK		= 3,
		nPARTYMARK		= 5,
		nRANKMARK		= 20, //add pkrank
	};

public:
	CNameDisplay ();
	virtual	~CNameDisplay ();

public:
	void CreateSubControl ();

public:
	void SetRankNameColor ( const D3DCOLOR& dwColor );
//	void SetClubNameColor ( const D3DCOLOR& dwColor );

public:
	void SetName ( const CString& strName, const DWORD m_dwPkWin, const BOOL bChar, const BOOL bRankName, const BOOL bRankMark, const D3DCOLOR& dwColor, WORD wSchoolMark = NODATA, DISP_PARTY emPartyMark = NODATA );
	void SetClubName ( const CString& strName, const D3DCOLOR& dwColor );
	void SetPetOwnerName( const CString& strName, const D3DCOLOR& dwColor );
	void SetRankName ( const CString& strName, const D3DCOLOR& dwColor ); //add pkrank

	bool IsMemPool()					{ return m_bUsedMemPool; }
	void SetMemPool()					{ m_bUsedMemPool = true; }

	void INITIALIZE();

protected:
    CUIControl* CreateControl ( const char* szControl, WORD wAlignFlag );

public:
	virtual HRESULT Render ( LPDIRECT3DDEVICEQ pd3dDevice );

private:
	CBasicTextBox*		m_pNameBox;
	CBasicLineBoxEx*	m_pNameLineBox;
	CBasicLineBoxEx*	m_pNameLineBoxCD;
	UIRECT				m_rcNameBox; // MEMO : static ������...?

private:
	CUIControl*			m_pSchoolMark[nSCHOOLMARK];
	CUIControl*			m_pPartyMark[nPARTYMARK];
	CUIControl*			m_pClubMark;
	CUIControl*			m_pRankMark[nRANKMARK]; //add pkrank

	UIRECT				m_rcSchoolMark[nSCHOOLMARK];
	UIRECT				m_rcPartyMark[nPARTYMARK];
	UIRECT				m_rcRankMark[nRANKMARK]; //add pkrank
	
private:
	bool				m_bDISP;
	CROWREN				m_sDISP_INFO;
	bool				m_bUsedMemPool; // MEMO : �޸� Ǯ�� ����ϱ� ���� �÷���]
	bool				m_bClub;

public:
	bool DIFFERENT ( const CROWREN &sINFO );
	void UPDATE ( DWORD _dwCOUNT, D3DXVECTOR3 vPOS );
	void SET_INFO ( CROWREN &sINFO )					{ m_sDISP_INFO = sINFO; }
	void SetItemLineBox();
	void SET_DISP ( bool bDISP )						{ m_bDISP = bDISP; }
	int CalculateVal ( DWORD dwCalc );

public:
	DWORD GETCOUNT ()			{ return m_sDISP_INFO.m_dwCOUNT; }
	bool GET_DISP ()			{ return m_bDISP; }
	EMCROW GETCROW ()			{ return m_sDISP_INFO.m_emCROW; }
	DWORD GETCTRLID()			{ return m_sDISP_INFO.m_dwID; }
	const CROWREN* GET_INFO ()	{ return &m_sDISP_INFO; }
	
public:
	BOOL IsRELAY ()				{ return NULL != (m_sDISP_INFO.m_dwTYPE&(TYPE_TARGET|TYPE_OURS)); }
	BOOL IsTARGET ()			{ return NULL != (m_sDISP_INFO.m_dwTYPE&(TYPE_TARGET)); }
	BOOL IsClubTARGET ()		{ return NULL != (m_sDISP_INFO.m_dwTYPE&(TYPE_CLUBBATTLE)); }
	BOOL IsPMARKET ()			{ return m_sDISP_INFO.m_bPMARKET; }
	BOOL IsCDCERTIFY ()			{ return NULL != (m_sDISP_INFO.m_dwTYPE&(TYPE_CLUBCD)); }
private:
	CBasicProgressBar*			m_pHP;
	bool						m_bFocus;
public:
	void SetHP(float fPercent, BOOL bFocus);
	void SetFocus(bool bFocus) { m_bFocus = bFocus; }
	bool IsFocus() { return m_bFocus; }
};
