// Name		: APSkillSlot.h
// Project	: Lib-RanClientUI


#pragma once

#include "../../../../EngineLib/G-Logic/GLDefine.h"
#include "../../../../EngineUILib/GUInterface/UIGroup.h"

class CSkillImage;
class CAPSkillSlot : public CUIGroup
{
protected:
	enum
	{
		QUICK_SKILL_MOUSE_OVER = NO_ID + 1,
	};

public:
	CAPSkillSlot();
	virtual	~CAPSkillSlot();

public:
	virtual	void CreateSubControl();

protected:
	void CreateSkillImage();
	void CreateMouseOver();

private:
	CSkillImage*	m_pSkillImage;

protected:
	CUIControl*		m_pSkillMouseOver;

public:
	virtual void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);

protected:
	void UpdateSlot(SNATIVEID sNativeID);

private:
	LPDIRECT3DDEVICEQ	m_pd3dDevice;

public:
	UIRECT GetAbsPosSkillImage();
	void SetUseSkillImageTwinkle(bool bUse);
};