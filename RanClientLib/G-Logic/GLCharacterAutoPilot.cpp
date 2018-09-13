#include "stdafx.h"
#include "shlobj.h"
#include "SUBPATH.h"

#include "../EngineUILib/GUInterface/Cursor.h"
//#include "DxCursor.h"
#include "DxInputDevice.h"
#include "editmeshs.h"
#include "DxMethods.h"
#include "DxViewPort.h"
#include "DxEffectMan.h"
#include "DxEnvironment.h"
#include "DxShadowMap.h"
#include "EditMeshs.h"
#include "GLogicData.h"
#include "GLItemMan.h"
#include "DxEffcharData.h"
#include "DxEffProj.h"

#include "tlhelp32.h"
#include "winbase.h"

#include "Psapi.h"
#pragma comment( lib, "Psapi.lib" )

#include "../../RanClientUILib/Interface/GameTextControl.h"
#include "../../RanClientUILib/Interface/InnerInterface.h"
#include "../../RanClientUILib/Interface/UITextControl.h"
#include "DxGlobalStage.h"
#include "GLGaeaClient.h"
#include "stl_Func.h"
#include "DxEffGroupPlayer.h"
#include "GLStrikeM.h"
#include "GLCrowData.h"
#include "GLPartyClient.h"
#include "GLQUEST.h"
#include "GLQUESTMAN.h"
#include "GLFriendClient.h"
#include "GLFactEffect.h"

#include "RANPARAM.h"
#include "DxRenderStates.h"
#include "GLCharacter.h"
#include "DXInputString.h"
#include "../EngineSoundLib/DxSound/BgmSound.h"
#include "../EngineSoundLib/DxSound/DxSoundLib.h"
#include "GLTaxiStation.h"
#include "GLItemMixMan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace
{
	class CFINDER
	{
	public:
		enum { BUFFER = 36, };
	protected:
		std::vector<STARGETID*>		m_vecTAR;

	public:
		CFINDER()
		{
			m_vecTAR.reserve(BUFFER);
		}

	public:
		void CLEAR()
		{
			if (!m_vecTAR.empty())
			{
				m_vecTAR.erase(m_vecTAR.begin(), m_vecTAR.end());
			}
		}

	public:
		STARGETID & FindClosedCrow(const DETECTMAP_RANGE &mapRange, const D3DXVECTOR3 vFromPt)
		{
			GASSERT(mapRange.first != mapRange.second);

			CLEAR();
			std_afunc::CCompareTargetDist sCOMP(vFromPt);

			for (DETECTMAP_ITER pos = mapRange.first; pos != mapRange.second; ++pos)
			{
				m_vecTAR.push_back(&(*pos));
			}

			std::vector<STARGETID*>::iterator found = std::min_element(m_vecTAR.begin(), m_vecTAR.end(), sCOMP);

			return *(*found);
		}

		static CFINDER& GetInstance()
		{
			static CFINDER Instance;
			return Instance;
		}
	};

	STARGETID* FindCrow(const DETECTMAP_RANGE &mapRange, const STARGETID &_starid)
	{
		static STARGETID sTARID;

		GASSERT(mapRange.first != mapRange.second);

		DETECTMAP_ITER iter = mapRange.first;
		for (; iter != mapRange.second; iter++)
		{
			sTARID = (*iter);

			if (sTARID.emCrow == _starid.emCrow && sTARID.dwID == _starid.dwID)	return &sTARID;
		}

		return NULL;
	}

	STARGETID* FindCrow(const DETECTMAP_RANGE &mapRange, SCONFTING_CLT::CONFT_MEM &setconftMember)
	{
		static STARGETID sTARID;

		GASSERT(mapRange.first != mapRange.second);

		DETECTMAP_ITER iter = mapRange.first;
		for (; iter != mapRange.second; iter++)
		{
			sTARID = (*iter);

			if (sTARID.emCrow == CROW_PC)
			{
				SCONFTING_CLT::CONFT_MEM_ITER iter = setconftMember.find(sTARID.dwID);
				if (iter != setconftMember.end())	return &sTARID;
			}
		}

		return NULL;
	}

	STARGETID* FindCrowCID(const DETECTMAP_RANGE &mapRange, SCONFTING_CLT::CONFT_MEM &setconftMember)
	{
		static STARGETID sTARID;

		GASSERT(mapRange.first != mapRange.second);

		DETECTMAP_ITER iter = mapRange.first;
		for (; iter != mapRange.second; iter++)
		{
			sTARID = (*iter);

			if (sTARID.emCrow == CROW_PC)
			{
				PGLCHARCLIENT pCHAR = GLGaeaClient::GetInstance().GetChar(sTARID.dwID);

				if (pCHAR)
				{
					SCONFTING_CLT::CONFT_MEM_ITER iter = setconftMember.find(pCHAR->GetCharData().dwCharID);
					if (iter != setconftMember.end())	return &sTARID;
				}
			}
		}

		return NULL;
	}

};



