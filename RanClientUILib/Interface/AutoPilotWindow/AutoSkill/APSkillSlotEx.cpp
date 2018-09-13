// Name		: APSkillSlotEx.cpp
// Project	: Lib-RanClientUI


#include "../../../StdAfx.h"
#include "APSkillSlotEx.h"

#include "../../GameTextControl.h"
#include "../../UITextControl.h"

#include "../../../../EngineUILib/GUInterface/BasicTextBox.h"

#include "../../../../RanClientLib/G-Logic/GLGaeaClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int CAPSkillSlotEx::nNOT_INIT = -1;

CAPSkillSlotEx::CAPSkillSlotEx()
	: m_nNumber(nNOT_INIT)
	, m_pNumberText(NULL)
	, m_nTabIndex(0)
{
}

CAPSkillSlotEx::~CAPSkillSlotEx()
{
}

void CAPSkillSlotEx::CreateSubControl(INT nTabIndex)
{
	GASSERT(0 <= nTabIndex && nTabIndex < AP_MAX_TAB_INDEX);
	m_nTabIndex = nTabIndex;

	CAPSkillSlot::CreateSubControl();
	if (m_pSkillMouseOver) m_pSkillMouseOver->SetVisibleSingle(FALSE);
}

void CAPSkillSlotEx::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
	CUIGroup::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);

	int nNumber = m_nNumber;
	GASSERT(0 <= nNumber && nNumber < EMSKILLQUICK_SIZE && "범위를 벗어납니다.");

	INT nG_Index = m_nTabIndex * AP_QUICK_SKILL_SLOT_MAX;
	nG_Index += nNumber;

	SNATIVEID sNativeID = GLGaeaClient::GetInstance().GetCharacter()->m_sSKILLQUICK[nG_Index];
	UpdateSlot(sNativeID);

	BOOL bSlotEmpty = (sNativeID == NATIVEID_NULL());
	DWORD dwMsg = GetMessageEx();
	if (m_pSkillMouseOver)
	{
		if (!bSlotEmpty && CHECK_MOUSE_IN(dwMsg) && bFirstControl)
		{
			m_pSkillMouseOver->SetVisibleSingle(TRUE);
		}
		else
		{
			m_pSkillMouseOver->SetVisibleSingle(FALSE);
		}
	}
}

void CAPSkillSlotEx::CreateNumberText(CD3DFontPar* pFont8, int nNumber)
{
	m_nNumber = nNumber;

	int nDisplayNumber = nNumber + 1;
	if (nDisplayNumber == AP_QUICK_SKILL_SLOT_MAX)
	{
		nDisplayNumber = 0;
	}

	// AutoPilotCfg : AP_QUICK_SKILL_SLOT_NUMBER
	{
		CString strNumber;
		strNumber.Format("%d", nDisplayNumber);

		CBasicTextBox* pNumberText = new CBasicTextBox;
		pNumberText->CreateSub(this, "AP_QUICK_SKILL_SLOT_NUMBER");
		pNumberText->SetFont(pFont8);
		pNumberText->SetTextAlign(TEXT_ALIGN_LEFT);
		pNumberText->SetOneLineText(strNumber, NS_UITEXTCOLOR::WHITE);
		pNumberText->SetControlNameEx("스킬 넘버 텍스트박스");
		RegisterControl(pNumberText);
		m_pNumberText = pNumberText;
	}
}