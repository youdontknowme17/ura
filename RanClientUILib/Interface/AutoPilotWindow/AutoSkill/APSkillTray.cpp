// Name		: APSkillTray.cpp
// Project	: Lib-RanClientUI


#include "../../../StdAfx.h"
#include "APSkillTray.h"
#include "APSkillSlotEx.h"

#include "../../BasicVarTextBox.h"

#include "../../GameTextControl.h"
#include "../../UITextControl.h"

#include "../../InnerInterface.h"
#include "../../SkillWindowToTray.h"

#include "../../../../EngineLib/DxCommon/DxFontMan.h"
#include "../../../../EngineLib/DxCommon/D3DFont.h"

#include "../../../../RanClientLib/G-Logic/GLGaeaClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int CAPSkillTray::nLBUPSKIP = 1;

CAPSkillTray::CAPSkillTray()
	: m_nLBUPSKIP(0)
	, m_pSkillInfo(NULL)
{
	memset(m_pSlotEx, 0, sizeof(m_pSlotEx));
}

CAPSkillTray::~CAPSkillTray()
{
}

void CAPSkillTray::CreateSubControl(INT nTabIndex)
{
	GASSERT((0 <= nTabIndex) && (nTabIndex < AP_MAX_TAB_INDEX));

	CD3DFontPar * m_pFont8 = DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 8, D3DFONT_SHADOW | D3DFONT_ASCII);

	// AutoPilotCfg: AP_QUICK_SKILL_TRAY_SLOT_0
	// AutoPilotCfg: AP_QUICK_SKILL_TRAY_SLOT_1
	// AutoPilotCfg: AP_QUICK_SKILL_TRAY_SLOT_2
	// AutoPilotCfg: AP_QUICK_SKILL_TRAY_SLOT_3
	// AutoPilotCfg: AP_QUICK_SKILL_TRAY_SLOT_4
	// AutoPilotCfg: AP_QUICK_SKILL_TRAY_SLOT_5
	// AutoPilotCfg: AP_QUICK_SKILL_TRAY_SLOT_6
	// AutoPilotCfg: AP_QUICK_SKILL_TRAY_SLOT_7
	// AutoPilotCfg: AP_QUICK_SKILL_TRAY_SLOT_8
	// AutoPilotCfg: AP_QUICK_SKILL_TRAY_SLOT_9
	for (int nNumber = 0; nNumber < AP_QUICK_SKILL_SLOT_MAX; ++nNumber)
	{
		CString strKeyword;
		strKeyword.Format("AP_QUICK_SKILL_TRAY_SLOT_%d", nNumber);

		CAPSkillSlotEx*	pSlot = new CAPSkillSlotEx;
		pSlot->CreateSub(this, strKeyword.GetString(), UI_FLAG_DEFAULT, AP_QUICK_SKILL_SLOT_1 + nNumber);
		pSlot->CreateSubControl(nTabIndex);
		pSlot->CreateNumberText(m_pFont8, nNumber);
		RegisterControl(pSlot);
		m_pSlotEx[nNumber] = pSlot;
	}

	CBasicVarTextBox* pSkillInfo = new CBasicVarTextBox;
	pSkillInfo->CreateSub(this, "BASIC_VAR_TEXT", UI_FLAG_XSIZE | UI_FLAG_YSIZE);
	pSkillInfo->CreateSubControl();
	pSkillInfo->SetVisibleSingle(FALSE);
	RegisterControl(pSkillInfo);
	m_pSkillInfo = pSkillInfo;
}

void CAPSkillTray::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
	if (!IsVisible()) return;

	m_pSkillInfo->SetVisibleSingle(FALSE);

	CUIGroup::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);

	DWORD dwMsg = GetMessageEx();
	if (dwMsg & UIMSG_LB_UP)
	{
		CSkillWindowToTray* pSkillWindowToTray = CInnerInterface::GetInstance().GetSkillWindowToTray();
		if (!pSkillWindowToTray)	return;

		BOOL bExist = (pSkillWindowToTray->GetSkill() != NATIVEID_NULL());
		if (bExist)
		{
			if (nLBUPSKIP <= m_nLBUPSKIP)
			{
				pSkillWindowToTray->ResetSkill();
				m_nLBUPSKIP = 0;
			}
			else
			{
				m_nLBUPSKIP++;
			}
		}
	}
}

void CAPSkillTray::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
	switch (ControlID)
	{
	case AP_QUICK_SKILL_SLOT_1:
	case AP_QUICK_SKILL_SLOT_2:
	case AP_QUICK_SKILL_SLOT_3:
	case AP_QUICK_SKILL_SLOT_4:
	case AP_QUICK_SKILL_SLOT_5:
	case AP_QUICK_SKILL_SLOT_6:
	case AP_QUICK_SKILL_SLOT_7:
	case AP_QUICK_SKILL_SLOT_8:
	case AP_QUICK_SKILL_SLOT_9:
	case AP_QUICK_SKILL_SLOT_0:
	{
		if (CHECK_MOUSE_IN(dwMsg))
		{
			int nG_Index = m_pSlotEx[0]->GetTabIndex() * AP_QUICK_SKILL_SLOT_MAX;
			int nIndex = ControlID - AP_QUICK_SKILL_SLOT_1;
			nG_Index += nIndex;

			SNATIVEID sOverSkill = GLGaeaClient::GetInstance().GetCharacter()->GetCharData().m_sSKILLQUICK[nG_Index];
			if (sOverSkill != NATIVEID_NULL())
			{
				UpdateSkillInfo(sOverSkill);
				m_pSkillInfo->SetVisibleSingle(TRUE);
			}

			CSkillWindowToTray* pSkillWindowToTray = CInnerInterface::GetInstance().GetSkillWindowToTray();
			if (!pSkillWindowToTray)
			{
				GASSERT(0 && "CSkillWindowToTray가 널입니다.");
				return;
			}

			if (pSkillWindowToTray->GetSkill() != NATIVEID_NULL())
			{
				const UIRECT& rcSlotPos = m_pSlotEx[nIndex]->GetAbsPosSkillImage();
				pSkillWindowToTray->SetGlobalPos(rcSlotPos);

				AddMessageEx(UIMSG_MOUSEIN_SKILLSLOT);
			}

			if (dwMsg & UIMSG_LB_UP)
			{
				SNATIVEID sNativeID = pSkillWindowToTray->GetSkill();

				if (sNativeID != NATIVEID_NULL())
				{
					GLGaeaClient::GetInstance().GetCharacter()->ReqSkillQuickSet(sNativeID, nG_Index);
					pSkillWindowToTray->ResetSkill();
					m_nLBUPSKIP = 0;
				}
				else
				{
					GLGaeaClient::GetInstance().GetCharacter()->ReqSkillRunSet(nG_Index);
				}
			}

			if (dwMsg & UIMSG_RB_UP)
			{
				GLGaeaClient::GetInstance().GetCharacter()->ReqSkillQuickReSet(nG_Index);
			}
		}
	}
	break;
	}
}

void CAPSkillTray::UpdateSkillInfo(SNATIVEID sNativeID)
{
	if (sNativeID != NATIVEID_NULL())
	{
		static SNATIVEID sNativeIDBack = NATIVEID_NULL();

		BOOL bUPDATE = FALSE;
		if (sNativeID != sNativeIDBack)
		{
			bUPDATE = TRUE;
		}

		if (bUPDATE)
		{
			PGLSKILL pSkill = GLSkillMan::GetInstance().GetData(sNativeID.wMainID, sNativeID.wSubID);
			if (pSkill)
			{
				SCHARSKILL* pCharSkill = GLGaeaClient::GetInstance().GetCharacter()->GETLEARNED_SKILL(sNativeID);
				LoadSkillInfo(pSkill, pCharSkill);
			}

			sNativeIDBack = sNativeID;
		}
	}
}

void CAPSkillTray::LoadSkillInfo(PGLSKILL const pSkill, SCHARSKILL* const pCharSkill)
{
	m_pSkillInfo->SetTextNoSplit(pSkill->GetName(), NS_UITEXTCOLOR::PALEGREEN);
}