// Name		: GlCharAutoPilotMsg.h
// Project	: Lib-RanClient


#include "StdAfx.h"
#include "GLChar.h"
#include "GLGaeaServer.h"
#include "GLItemLMT.h"
#include "GLItemMan.h"
#include "../../EngineLib/Common/StringUtils.h"
#include "../../MfcExLib/RanFilter.h"
#include "GLItemMixMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

////////////////////////////////////////////////////////////////////////////////
// AutoSkill
HRESULT GLChar::MsgReqUseAutoSkill(NET_MSG_GENERIC* nmg)
{
	GLMSG::SNETPC_REQ_USE_AUTO_SKILL *pNetMsg = (GLMSG::SNETPC_REQ_USE_AUTO_SKILL *)nmg;

	switch (pNetMsg->m_bAutoSkill)
	{
	case TRUE:
	{
		if (!IsUsingAutoSkill())
		{
			SetUsingAutoSkillOn();

			GLMSG::SNETPC_UPDATE_USE_AUTO_SKILL NetMsg;
			NetMsg.m_bAutoSkill = TRUE;

			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &NetMsg);
		}
	}
	break;

	case FALSE:
	{
		if (IsUsingAutoSkill())
		{
			SetUsingAutoSkillOff();

			GLMSG::SNETPC_UPDATE_USE_AUTO_SKILL NetMsg;
			NetMsg.m_bAutoSkill = FALSE;

			GLGaeaServer::GetInstance().SENDTOCLIENT(m_dwClientID, &NetMsg);
		}
	}
	break;
	}



	return S_OK;
}
////////////////////////////////////////////////////////////////////////////////