// Name		: GLContrlAutoPilot.h
// Project	: Lib-RanClient


#pragma once

#include "GLContrlBaseMsg.h"

namespace GLMSG
{
#pragma pack(1)
	
	struct SNETPC_REQ_USE_AUTO_SKILL
	{
		NET_MSG_GENERIC		nmg;
		BOOL				m_bAutoSkill;

		SNETPC_REQ_USE_AUTO_SKILL()
			: m_bAutoSkill(FALSE)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_REQ_USE_AUTO_SKILL;
		}
	};
	struct SNETPC_UPDATE_USE_AUTO_SKILL
	{
		NET_MSG_GENERIC		nmg;
		BOOL				m_bAutoSkill;

		SNETPC_UPDATE_USE_AUTO_SKILL()
			: m_bAutoSkill(FALSE)
		{
			nmg.dwSize = sizeof(*this);
			nmg.nType = NET_MSG_GCTRL_UPDATE_USE_AUTO_SKILL;
		}
	};

#pragma pack()
}