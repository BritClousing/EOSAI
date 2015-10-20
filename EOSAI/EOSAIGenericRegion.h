
#pragma once

#ifdef CREATE_EOSAI_DLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

#include "EOSAILocation.h"
#include "EOSAICitResUnitSummary.h"
#include "EOSAIBCDumbArray1D.h"
#include "EOSAICitResSummary.h"
//#include "City.h"
#include "EOSAIPathfinderPoint.h"
class CEOSAIGeo;
class CEOSAIRoad2;
class CEOSAICity;
//class CGeo;
//class CEOSAIMultiRegion2;

class CEOSAIGenericRegionManager;
class CEOSAIGenericRegion;


#define TERRAIN_WATER 1
#define TERRAIN_GRASS 5
#define TERRAIN_CITY  8

class CEOSAIPoiObjectProximity
{
	public:
		CEOSAIPoiObjectProximity(){ m_pAIPoiObject = NULL; m_fProximity01 = 0.0f; m_fDistanceFromRegionEdge = 1000000.0f; }
		CEOSAIPoiObject* m_pAIPoiObject;
		float          m_fProximity01; // 1.0 = in the Region, anything above 0.0 = within two turn attack radius
		float          m_fDistanceFromRegionEdge;
};

class CEOSAIAdjacentRegionDesc
{
	public:
		CEOSAIAdjacentRegionDesc( CEOSAIGenericRegion* pRegion, CEOSAILocation JumpOffLocation )
		{
			m_pRegion = pRegion;
			m_JumpOffLocation = JumpOffLocation;
		}
		CEOSAILocation  m_JumpOffLocation; // jumping-off point to the adjacent region (i.e. next to adj region)
		CEOSAIGenericRegion* m_pRegion;
};

class DLLIMPEXP CEOSAIGenericRegion : public CEOSAIPathfinderPoint
{
	public:
		CEOSAIGenericRegion( CEOSAIGenericRegionManager* pRegionManager, long iId );
		virtual ~CEOSAIGenericRegion();
		//
		void  Clear();
		//virtual void ClearPoiData();
		void ClearPoiData();

		// Initialization
		//
			long        GetGeoId(){ return m_iGeo; }
			CEOSAIGeo*  GetGeo();

			void   SetCityObjectId( long iObjectId );
			long   GetCityObjectId(){ return m_iCityObjectId; }
			CEOSAICity*  GetEOSAICity();
			//long    GetGeoId(){ return m_iGeo; }
			//CGeo*   GetGeo();
			//
			bool    IsAdjacentToRegion( CEOSAIGenericRegion* pGenericRegion );
			void    AddAdjacentRegion( CEOSAIGenericRegion* pGenericRegion, CEOSAILocation JumpOffLocation );
			CList< CEOSAIAdjacentRegionDesc* >*  GetAdjacentRegions(){ return &m_AdjacentRegions; }

			bool    GetAdjacentRegionJumpOffLocation( long iAdjRegion, CEOSAILocation* pLocation );

			void    AddPoiObjectToSummary( CEOSAIPoiObject* pPoiObject );
			CList< CEOSAIPoiObject* >* GetPoiObjectsInsideRegion(){ return &m_AIPoiObjectsInsideRegion; }
			CList< CEOSAIPoiObjectProximity* >* GetPoiObjectProximities(){ return &m_AIPoiObjectProximity; } // owned. 1.0 = within region radius, values above 0.0 = within 2-turn attack radius

		// Information
		//
			CEOSAIGenericRegionManager*  GetGenericRegionManager(){ return m_pRegionManager; }

		//
		// Road
		//
			void  AddAIRoad( CEOSAIRoad2* pAIRoad );
			void  SetupAIRoadConnections();
			void  SetupAIRoadConnections( CEOSAIRoad2* pAIRoad );

		//
			float GetUnownedCitResValue(){ return m_PlayerCitResUnitSummaries[0].m_CitRes.GetCitResValue(); }

			// Power, Presence, Sighting
			void   ClearSightingAndPowerValueForEachPlayer();
			//void   ClearSightingPresenceForEachPlayer();
			//void   ClearPowerValueForEachPlayer();
			//void   CalculateSightingPresenceForEachPlayer();
			//

			CEOSAICitResSummary*      GetTotalCitResSummary(){ return &m_TotalCitResSummary; }
			CEOSAICitResUnitSummary*  GetCitResUnitSummary( long iPlayer ){ return &m_PlayerCitResUnitSummaries[iPlayer]; }
			float  GetPlayerAndSATPresence( long iPlayer );

		//
		// INCOMPLETE:
		//   Right now, the pathfinder calculates danger and presence based ONLY on units
		//   inside that specific GenericRegion.  When using AIRegions, this is a ridiculously small area.
		//   I think this is a hold-over from when I was using MultiRegions.
		//   I need to precalculate this stuff.  
		//   Maybe a "territory" value (which presses up against other players territory).
		//   Territory can also be used by the AI to warn players to back off, and help AI avoid stepping
		//   on other players territory.
		//
		//   Right now, "PlayerPower" and PlayerTerritory" values aren't being calculated
		//   I'm putting this on hold for now because other AI stuff needs work.
		//

			float  GetUnitPower( long iPlayer );
			long   GetCitResUnitCount( long iPlayer );
			long   GetCitGroundUnitCount( long iPlayer );
			long   GetCitResCount( long iPlayer );

			// The PlayerPower is based on unit power (not normalized)
			//   The AdjustedPowerValue needs to be normalized by the moving unit
			//   (transports fear more than battleships do)
			float  GetPlayerPower( long iPlayer ){ return GetUnitPower( iPlayer ); }
			// A small area of control around Cities, Resources, and Units
			// Aircraft have no "presence" value
			float  GetPlayerPresence01( long iPlayer );
			// See description below
			float  GetPlayerVisibility01( long iPlayer ){ return m_PlayerVisibility01[iPlayer]; }
			// Based off of GroundUnits, Cities, Resources. Expands inside islands, and includes coasts.
			//   Perhaps it includes internal waterways.
			//float  GetPlayerTerritory01( long iPlayer ){ return 0.0f; }

			// Highest values where PlayerControl values overlap
			float  GetWarzoneValue01( long iPlayer1, long iPlayer2 );

			void   AddPoiObjectProximity( CEOSAIPoiObject* pPoiObject, float fProximity01, float fActualDistance );
			//void   AddToPlayerCitResControlValue01( CPoiObject* pPoiObject );
		//

		long       m_iGeo;
		float      m_fRegionRadius;

		long           m_iCityObjectId;
		CEOSAICity*   m_pAICity;

		CList< CEOSAIAdjacentRegionDesc* > m_AdjacentRegions;
		
		CList< CEOSAIRoad2* >             m_RoadsOnTheRegionEdge;

		CEOSAIBCDumbArray1D< CEOSAICitResUnitSummary >  m_PlayerCitResUnitSummaries;
		CEOSAICitResSummary                        m_TotalCitResSummary; // Combined summary of all PoiObjects

		// Value ranges from 0-1, based on Player Cities, Resources, and Units
		//   Value of 1.0 means "currently visible", 
		//   Values between 0.01-0.99 are potentially visible (if units move)
		//   Aircraft produce the largest visibility areas because they move quickly
		//   Stationary Poi produce an area of 1.0 visibility surrounded by 0.0 visibility
		CEOSAIBCDumbArray1D< float >  m_PlayerVisibility01;
		CEOSAIBCDumbArray1D< float >  m_PlayerCityUnitAIPower; // 0-infinite. Based on City, Unit Proximity and Unit Power
		CEOSAIBCDumbArray1D< float >  m_PlayerCitResControlValue01; // based on Cities, Resources

		// Borders - players tend to assert 'ownership' over areas of land.
		//   This is based on m_PlayerControlValue
		long  m_iSingleOwner;
		float m_fSingleOwnerControl; // 0.0 - 1.0

		float m_fTempWarzoneValue01;

		CList< CEOSAIPoiObject* >          m_AIPoiObjectsInsideRegion; // PoiObjects in the Region
	private:
		CList< CEOSAIPoiObjectProximity* >  m_AIPoiObjectProximity; // owned. 1.0 = within region radius, values above 0.0 = within 2-turn attack radius

		CList< CEOSAIGenericRegion* >  m_CachedListOfRegionsWithin400px;
};

