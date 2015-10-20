
#pragma once

#include "EOSAIBrain.h"
#include "EOSAIUnitSummary.h"
#include "EOSAICitResSummary.h"
#include "EOSAIListOwned.h"
//#include "EnumGeoStrategicSituation.h"
#include "EOSAIBCDumbArray1D.h"
#include "EOSAICitResUnitSummary.h"
#include "EOSAIIntSet.h"
#include "EOSAIGeoView.h"
//#include "AIGeoState.h"

//#include "Geo.h"
//#include <math.h>

//#define EOSAI_MAX_NUMBER_OF_PLAYERS 10

//class CGeo;
namespace EOSAI
{
	class CAIPlayer2;
};
class CEOSAIBrain;
class CEOSAIPoiObject;

#ifdef CREATE_EOSAI_DLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

class DLLIMPEXP CEOSAIGeo
{
	public:
		enum GeoType
		{
			Unknown,
			Land,
			Water,
			Impassible,
			Road
		};

		//CEOSAIGeo( EOSAI::AIPlayer* pAIPlayer, long iGeoId );//, CGeo* pGeo );
		CEOSAIGeo( long iGeoId );//, CGeo* pGeo );

		//
		//
			long     GetId(){ return m_iId; }
			void     SetGeoType( GeoType e ){ m_eGeoType = e; }
			GeoType  GetGeoType(){ return m_eGeoType; }
			void     SetSize( long iSize ){ m_iSize = iSize; }
			long     GetSize(){ return m_iSize; }
			CEOSAIIntSet* GetConnectedSet(){ return &m_ConnectedSet; }

		// Views
		//
			CEOSAIGeoView*  GetView( long iAIPlayer ){ return m_AIGeoView[iAIPlayer]; }
			CEOSAIGeoView*  CreateView( long iAIPlayer );

		// AIPoiObjects
		//
			CList< CEOSAIPoiObject* >*  GetAIPoiObjectList(){ return &m_AIPoiObjectList; } // PoiObjects on this geo

		// Multiregions
		//
			void  AddMultiRegion( CEOSAIMultiRegion2* pMultiRegion );
			CList< CEOSAIMultiRegion2* >* GetMultiRegions(){ return &m_MultiRegions; }

		// CitRes
		//
			CEOSAICitResSummary* GetTotalCitResSummary(){ return &m_TotalCitResSummary; }
			float  GetTotalCitResValue(){ return m_TotalCitResSummary.GetCitResValue(); } // Combined production + resource values
			long   GetTotalCitResCount(){ return m_TotalCitResSummary.m_iCitResCount; }
			long   GetUnownedCitResCount(){ return m_PlayerCitResUnitSummary[ 0 ].m_CitRes.m_iCitResCount; }

		//
		// Player Cities/Units
		//
			CEOSAICitResUnitSummary* GetPlayerCitResUnitSummary( long iPlayer ){ return &( m_PlayerCitResUnitSummary[iPlayer] ); }
			float  GetPlayerCitResControl01( long iPlayer ){ return m_PlayerCitResUnitSummary[iPlayer].m_CitRes.GetCitResValue() / m_TotalCitResSummary.GetCitResValue(); }
			CEOSAIUnitSummary*  GetUnitSummary( long iPlayer );

	// Situation
	//
		//float GetPlayerPresence01( long iPlayer );
		//float GetMine01();          // I have a presence.  1.0 = I own all CitResUnits on the geo
		//float GetTeammateOrAlly01();
		//float GetUnowned01(){ return GetPlayerPresence01( 0 ); }
		//float GetNeutralPlayer01();
		//float GetEnemyPlayer01();

	protected:

		long     m_iId;
		GeoType  m_eGeoType;
		CEOSAIIntSet  m_ConnectedSet;
		long     m_iSize; // Size of the geo (in terms of grid points)

		// Views
		//
			CEOSAIBCDumbArray1D< CEOSAIGeoView* >  m_AIGeoView; // NULL if it's a human player

		//
		// PoiObjects
		//
			CList< CEOSAIPoiObject* >  m_AIPoiObjectList;  // PoiObjects on this geo

		// CitRes Summary Data
		//
			float m_fTotalCitResValue; // Combined production + resource values
			CEOSAICitResSummary  m_TotalCitResSummary;
			CEOSAIBCDumbArray1D< CEOSAICitResUnitSummary >  m_PlayerCitResUnitSummary; // per player (includes AI and human players)

		// Multiregions
		//
			CList< CEOSAIMultiRegion2* >  m_MultiRegions; // unowned
};

