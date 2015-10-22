
#pragma once

#include "MessageFromAI.h"
#include "EOSAILongSet.h"

#ifdef CREATE_EOSAI_DLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

namespace EOSAI
{
	class DLLIMPEXP MessageFromAI_BorderViolationComplaint : public MessageFromAI
	{
		public:
			void SetSender(int iAIPlayer) { m_iAIPlayer = iAIPlayer; }
			void SetViolator(int iPlayer) { m_iViolatorPlayer = iPlayer; }
			void SetViolatingPoiObjects(CEOSAILongSet& PoiObjects) { m_ViolatingPoiObjects = PoiObjects; }
			void SetTitle(CString strTitle) { m_strMessageTitle = strTitle; }
			void SetMessage(CString strMessage) { m_strMessage = strMessage; }

			int m_iAIPlayer;
			int m_iViolatorPlayer;
			CEOSAILongSet m_ViolatingPoiObjects;
			CString m_strMessageTitle;
			CString m_strMessage;
	};
}

