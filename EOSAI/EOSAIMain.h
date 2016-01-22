
#pragma once

#ifdef CREATE_EOSAI_DLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

#include "EOSAIGameRules.h"
#include "EOSAICommonData.h"
#include "EOSAIPlayerManager.h"

// Contains the data pointers.

namespace EOSAI
{
	class DLLIMPEXP Main
	{
		public:
			static Main* p(){ return s_pMain; }

			Main();
			~Main();

			// Interproc
			//
				void InitializeInterprocessCommunication();
				void ShutdownInterprocessCommunication();

			// Data Structures
			//
				EOSAI::CCommonData*   GetAICommonData() { return &m_AICommonData; }
				EOSAI::CGameRules*    GetAIGameRules() { return &m_AIGameRules; }
				CEOSAIRegionManager2* GetAIRegionManager();
				CEOSAIPlayerManager*  GetAIPlayerManager() { return &m_AIPlayerManager; }
				//
				CEOSAIPoiObject*      GetAIPoiObject(long iAIPoiObject);
				CList< CEOSAIPoiObject* >*  GetAIPoiObjectList() { return m_AICommonData.GetAIPoiObjects(); }
			
			// Thread control
			//
				void SetGameShouldPauseAtTheEndOfTurn(int i){ m_iGameShouldPauseAtTheEndOfTurn = i; }
				int  GetGameShouldPauseAtTheEndOfTurn(){ return m_iGameShouldPauseAtTheEndOfTurn; }

		private:
			CEOSAIPlayerManager m_AIPlayerManager;
			EOSAI::CGameRules   m_AIGameRules;
			EOSAI::CCommonData  m_AICommonData;

			static Main* s_pMain;

			int m_iGameShouldPauseAtTheEndOfTurn;
	};
}

extern EOSAI::Main* g_pEOSAIMain;
