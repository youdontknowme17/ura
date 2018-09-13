#pragma	once

#include "../UIWindowEx.h"
class	CBasicTextButton;
class	CBasicLineBoxEx;
class	CBasicTextBox;
class	CBasicTextButton;

class	CPvpWindowDisplay : public CUIWindowEx
{
protected:
	enum
	{
		CTF_REWARD_BUFF_TEXT00 = ET_CONTROL_NEXT,
		CTF_REWARD_BUFF_TEXT01,
		CTF_REWARD_BUFF_TEXT02,
		CTF_REWARD_BUFF_TEXT03,
		INFO_CTF_DISPLAY_WINNER_ICON_SG,
		INFO_CTF_DISPLAY_WINNER_ICON_MP,
		INFO_CTF_DISPLAY_WINNER_ICON_PHX,
		INFO_CTF_DISPLAY_WINNER_ICON_NONE,
		SCHOOLWAR_BACKGROUND,
		SCHOOLWAR_PAGE,
		SCHOOLWAR_BUTTON_PARTICIPATE,
		SCHOOLWAR_BUTTON_PAGE,
		SCHOOLWAR_BUTTON_CLOSE,
		HISTORY_HELP,

	};
	enum
	{
		nTEXT		= 8,
	};


public:
	CPvpWindowDisplay ();
	virtual	~CPvpWindowDisplay ();

public:
	CBasicTextButton*	CreateTextButton23 ( const char* szButton, UIGUID ControlID, const char* szText );
	CBasicTextButton*	CreateTextButton ( char* szButton, UIGUID ControlID , char* szText );
	virtual void Update ( int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl );

	void CreateSubControl ();

private:
	int								m_nActivePage;
	CUIControl*						m_pMapImg;
	CBasicTextButton*				m_pButtonSchoolWarPage;

	CBasicLineBoxEx*				m_pLineBoxTop;
	CBasicLineBoxEx*				m_pLineBoxMapImg;
	CBasicLineBoxEx*				m_pLineBoxUpper;
	CBasicLineBoxEx*				m_pLineBoxBottom;
	CBasicLineBoxEx*				m_pLineBoxTopWhite;
	CBasicLineBoxEx*				m_pLineBoxBottomEx;
	CBasicLineBoxEx*				m_pLineBoxBattle;
	//
	CBasicLineBoxEx*				m_pLineBoxBuff00;
	CBasicLineBoxEx*				m_pLineBoxBuff01;
	CBasicLineBoxEx*				m_pLineBoxBuff02;
	CBasicLineBoxEx*				m_pLineBoxBuff03;

	CBasicLineBoxEx*				m_pLineBoxPrevious;
	CBasicLineBoxEx*				m_pLineBoxBottomWhite;
	CBasicTextBox*                  m_pText[8];
	CBasicTextBox*					m_pTimeTextNext;
	CBasicTextBox*					m_pTextProgress;
	CBasicTextBox*					m_pTextRegister;
	CBasicTextBox*					m_pTopCharName;
	CBasicTextBox*					m_pInfo;
	CBasicTextBox*					m_pTitle;
	CBasicTextBox*					m_pReward;
	CBasicTextButton*				m_pButtonClose;
	CBasicTextButton*				m_pButtonParticipate;

public:
	virtual	void TranslateUIMessage ( UIGUID ControlID, DWORD dwMsg );
	virtual	void SetVisibleSingle ( BOOL bVisible );
	void CheckMouseState ();

public:
	void	SetProgressTime(float fElapsedTime);
	void    SetTopWinner ( int nRank, std::string strNAME );
	void    SetBattleRecord( DWORD dwPreviousTime, float fLastBattleTime );
	void	SetNextBattleTime(DWORD dwNextBattleStartTime , DWORD dwNextBattleEndTime );
	void	RefreshRanking();
	void    Refresh();
	void    GetTopOne();
	void    RefreshTopOne();
	BOOL    m_bQueue;
	BOOL    bToMarket;
	void	OpenPage( int nPage );
    float   fRegTime;
	CBasicTextBox*				m_pStart;
	CBasicTextBox*				m_pEnd;

	CBasicButton*				m_pHelp;
	CUIControl*             	m_pWinnerIconNone;
	CUIControl*             	m_pWinnerIcon[3];
	CUIControl*			        m_pRewardBuffBack[4];
	CUIControl*                 m_pSchoolImg[3];
	CUIControl*                 m_pRewardImg[4];

	bool m_bCHECK_MOUSE_STATE;
	BOOL m_bFirstGap;
	D3DXVECTOR2	m_vGap;
	int			m_PosX;
	int			m_PosY;

	CBasicTextBox*	CreateStaticControl ( char* szControlKeyword, CD3DFontPar* pFont, D3DCOLOR dwColor, int nAlign );

};