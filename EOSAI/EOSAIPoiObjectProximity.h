
#pragma once

class CEOSAIAirfield;
class CEOSAIUnit;
class CEOSAICity;
class CEOSAIRegion2;
class CEOSAIPoiObject;
//class CEOSAIMultiRegion2;
class CEOSAITaskPrerequisite;
class CEOSAIUnitTemplateAndProximity;
class CEOSAIPoiObjectProximityDescription_TransportRoute;
class CAIPoiObjectsOrderedByFutureProximityToLocation;
class CAIThought_WhatUnitsCanIGetToMultiRegion;
//class CEOSAIMovementPathwaysFromMRXtoMRY;
//class CEOSAIMovementPathwaysFromMRABCtoMRY;
//#include "EOSAIMovementPathwaysFromMRABCtoMRY.h"
//class C9MovementPathwaysFromEverywhereToMRX;

/*
class CEOSAIPoiObjectProximity
{
	public:
		CEOSAIPoiObjectProximity( 
			CAIPoiObjectsOrderedByFutureProximityToLocation* pOrderedByProximity,
			CEOSAIPoiObject*  pAIPoiObject,
			float fCrowsFlightDistance,
			float fCrowsFlightTime )
		{
			m_pAIPoiObject = pAIPoiObject;
			m_pOrderedByProximity = pOrderedByProximity;
			m_iDepthOfThought = 0;
			m_fCrowsFlightDistance = fCrowsFlightDistance;
			m_fCrowsFlightTime = fCrowsFlightTime;
		}

		// For air-units, cities, airfields, aircraft carriers, based on the 
		//   unit or unit-template range.
		virtual bool IsWithinAirRange();

		// Can be a city, airfield, actual unit
		CEOSAIPoiObject*  m_pAIPoiObject;

		// Used to access higher-level information, like closest air-range
		CAIPoiObjectsOrderedByFutureProximityToLocation*  m_pOrderedByProximity;

		long   m_iDepthOfThought;

		//
		// DepthOfThought 1
		//
			// CrowsFlight distance from the unit's current movement path
			float  m_fCrowsFlightDistance;
			float  m_fCrowsFlightTime; // distance / unit's-best-movement-rate
			//float  m_fDistanceFromMyClosestAirbaseToLocation; // used by air units only
};
*/
//
/*
class CEOSAIPoiObjectProximity_City
{
	public:
		CEOSAIPoiObjectProximity_City(
			C9MovementPathwaysFromEverywhereToMRX* pParent_PathwaysFromEverywhereToMRX,
			CEOSAICity*  pAICity );

		//bool  IsWithinAirRangeOfLocation(){ return m_bLocationIsWithinAirRangeOfAValidAirUnit; }
		//float GetAirDistanceToEdgeOfTargetMultiRegion(){ return m_fAirDistanceToTarget; }

		// Used to access higher-level information, like closest air-range
		//CAIPoiObjectsOrderedByFutureProximityToLocation*  m_pOrderedByProximity;

		//
		CEOSAICity*  m_pAICity;

		//long  m_iDepthOfThought; // 1 = CrowsFlight, 2 = Land/Sea Route

		//
		//float m_fAirDistanceToTarget;

		//CAIMovementPathwaySummaryFromMultiRegion* m_pPathwaySummaryToTargetMultiRegion;
		CEOSAIMovementPathwaysFromMRXtoMRY* m_pPathwaysToTargetMultiRegion;

		//float m_fCrowsTimeBasedOnFastestValidUnit;
		//bool  m_bCityToTargetDistanceIsWithinAirRangeOfAValidAirUnit;

		//bool  m_bLandRouteExists;
		//float m_fLandRouteDistanceToLocation;
		//float m_fLandRouteTimeToLocation;

		//bool  m_bSeaRouteExists;
		//float m_fSeaRouteDistanceToLocation;
		//float m_fSeaRouteTimeToLocation;
};
*/
// Airfield, city, or carrier
/*
class CEOSAIPoiObjectProximity_Airbase
{
	public:
		CEOSAIPoiObjectProximity_Airbase(
			C9MovementPathwaysFromEverywhereToMRX* pParent_PathwaysFromEverywhereToMRX,
			CEOSAIPoiObject*  pAIAirbase );

		bool  m_bAirbaseIsMobile;
		CEOSAIPoiObject*  m_pAIAirbase;

		//
		float m_fCurrentAirDistanceToTarget;

		CEOSAIMovementPathwaysFromMRABCtoMRY  m_PathwaysToTargetMultiRegion;
};
*/
/*
class CEOSAIPoiObjectProximity_Unit
{
	public:
		CEOSAIPoiObjectProximity_Unit(
			C9MovementPathwaysFromEverywhereToMRX* pParent_PathwaysFromEverywhereToMRX,
			CEOSAIUnit*  pAIUnit );

		CEOSAIUnit* m_pAIUnit;

		// MovementPathways are important for land/sea distances, but they
		//   are also important for air units danger information.
		// INCOMPLETE: the danger value depends on the type of unit
		//   e.g. GroundUnits attack GroundUnits, AA guns attack Aircraft, AntiSub attacks Subs
		CEOSAIMovementPathwaysFromMRABCtoMRY  m_PathwaysToTargetMultiRegion;
};
*/
/*
class CEOSAIPoiObjectProximity_GroundUnit : public CEOSAIPoiObjectProximity_Unit
{
	public:
		CEOSAIPoiObjectProximity_GroundUnit(
			C9MovementPathwaysFromEverywhereToMRX* pParent_PathwaysFromEverywhereToMRX,
			CEOSAIUnit*  pAIUnit ) : CEOSAIPoiObjectProximity_Unit( pParent_PathwaysFromEverywhereToMRX, pAIUnit )
		{
			//m_bLandRouteExists = false;
			//m_fLandRouteDistanceToLocation = 0.0f;
			//m_fLandRouteTimeToLocation = 0.0f;
		}

		// Land Route (based on a unit's total movement pathway)
		//bool  m_bLandRouteExists;
		//float m_fLandRouteDistanceToLocation;
		//float m_fLandRouteTimeToLocation;

		//CList< CEOSAIPoiObjectProximityDescription_TransportRoute* > m_TransportRouteList;
};
*/
/*
class CEOSAIPoiObjectProximity_AirUnit : public CEOSAIPoiObjectProximity_Unit
{
	public:
		CEOSAIPoiObjectProximity_AirUnit(
			C9MovementPathwaysFromEverywhereToMRX* pParent_PathwaysFromEverywhereToMRX,
			CEOSAIUnit*  pAIUnit ) : CEOSAIPoiObjectProximity_Unit( pParent_PathwaysFromEverywhereToMRX, pAIUnit ){}
			//CAIPoiObjectsOrderedByFutureProximityToLocation* pOrderedByProximity,
			//CEOSAIUnit*  pAIUnit ) : CEOSAIPoiObjectProximity_Unit( pOrderedByProximity, pAIUnit ){}
};
*/
/*
class CEOSAIPoiObjectProximity_SeaUnit : public CEOSAIPoiObjectProximity_Unit
{
	public:
		CEOSAIPoiObjectProximity_SeaUnit(
			C9MovementPathwaysFromEverywhereToMRX* pParent_PathwaysFromEverywhereToMRX,
			CEOSAIUnit*  pAIUnit ) : CEOSAIPoiObjectProximity_Unit( pParent_PathwaysFromEverywhereToMRX, pAIUnit )
			//CAIPoiObjectsOrderedByFutureProximityToLocation* pOrderedByProximity,
			//CEOSAIUnit*  pAIUnit ) : CEOSAIPoiObjectProximity_Unit( pOrderedByProximity, pAIUnit )
		{
			//m_bSeaRouteExists = false;
			//m_fSeaRouteDistanceToLocation = 0.0f;
			//m_fSeaRouteTimeToLocation = 0.0f;
		}

		// Sea Route (based on a unit's total movement pathway)
		//bool  m_bSeaRouteExists;
		//float m_fSeaRouteDistanceToLocation;
		//float m_fSeaRouteTimeToLocation;
		// Sea Route
		//bool       m_bDirectRouteExists; // no transports needed
		//float      m_fClosestDistance;
		//CEOSAIRegion2* m_pClosestRegion;
};
*/
/*
class CEOSAIPoiObjectProximity_SeaTransportRoute
{
	public:
		CEOSAIPoiObjectProximity_SeaTransportRoute()
		{
			m_iGroundGeo1 = 0; // pickup
			m_iWaterGeo = 0;
			m_iGroundGeo2 = 0; // dropoff
		}

		long   m_iGroundGeo1; // pickup
		long   m_iWaterGeo;
		long   m_iGroundGeo2; // dropoff

		CList< CEOSAIUnit* >  m_PotentialTransportsList;
};

class CEOSAIPoiObjectProximity_AirTransportRoute
{
	public:
		CEOSAIPoiObjectProximity_AirTransportRoute()
		{
			m_iGroundGeo1 = 0; // pickup
			m_iGroundGeo2 = 0; // dropoff
		}

		long   m_iGroundGeo1; // pickup
		long   m_iGroundGeo2; // dropoff

		CList< CEOSAIUnit* >  m_PotentialTransportsList;
};
*/
// Describes an AIPoiObject's proximity to something

class CEOSAIPoiObjectProximityDescription
{
	public:
		CEOSAIPoiObjectProximityDescription()
		{
			m_iDepthOfThought = 0;
			m_pAIPoiObject = NULL;
			//
			m_fCrowsFlightDistance = 0.0f;
			m_fCrowsFlightTime = 0.0f; // distance / unit-movement-rate (for cities, airfields, use the movement rate of the best unit in Unitset)
			m_fDistanceFromMyClosestAirbaseToLocation = 0.0f;
/*
			m_fCrowsFlightDistance = 0.0f;
			m_fCrowsFlightTime = 0.0f;
			//m_bLocationIsOutsideAnyCurrentAirbaseRange = false; // assuming I want aircraft to return
			m_bIsWithinMovementRangeOfSomeAirbase = false; // assuming I want aircraft to return

			m_bAirPathCalculated = false;
			m_bThereIsAPathThroughSomeAirbase = true;
			m_fShortestAirTimeFromPathway = 0.0f;
			//m_pClosestAirRegion = NULL;
			//m_bIsWithinMovementRangeOfCurrentOrFutureAirbase = false; // Can this aircraft reach the location from this airbase?

			m_bGroundDistanceCalculated = false;
			m_bGroundRouteExists = false; // no transports needed
			m_fClosestGroundDistance = 0.0f;
			m_pClosestGroundRegion = NULL;

			m_bSeaDistanceCalculated = false;
			m_bSeaRouteExists = false;
			m_fClosestSeaDistance = 0.0f;
			m_pClosestSeaRegion = NULL;
			*/
		}

		void SetAIPoiObject( CEOSAIPoiObject* pAIPoiObject );
		CEOSAIPoiObject* GetAIPoiObject(){ return m_pAIPoiObject; }

		void SetCrowsFlightValues( 
				float fDistanceToLocation,
				float fTimeToLocation,
				float fDistanceFromMyClosestAirbaseToLocation )
				//float fDistanceToLocation, float fTimeToLocation, bool bIsWithinMovementRangeOfSomeAirbase )
		{
			m_iDepthOfThought = 1;
			m_fCrowsFlightDistance = fDistanceToLocation;
			m_fCrowsFlightTime = fTimeToLocation;
			//m_bIsWithinMovementRangeOfSomeAirbase = bIsWithinMovementRangeOfSomeAirbase;
			m_fDistanceFromMyClosestAirbaseToLocation = fDistanceFromMyClosestAirbaseToLocation;
		}

		// Needs to rebase, needs a transport, etc
		void  GetPrerequisites( CList< CEOSAITaskPrerequisite* >* pPrereqList );

		// returns CrowsFlight or Spatial distance depending on the iDepthOfThought
		float GetTimeToLocation();

		float GetCrowsFlightDistance(){ return m_fCrowsFlightDistance; }
		float GetCrowsFlightTime(){ return m_fCrowsFlightTime; }

	private:

		// Can be a city, airfield, actual unit
		CEOSAIPoiObject*  m_pAIPoiObject;

		long   m_iDepthOfThought;

		//
		// DepthOfThought 1
		//
			// CrowsFlight distance from the unit's current movement path
			float  m_fCrowsFlightDistance;
			float  m_fCrowsFlightTime; // distance / unit's-best-movement-rate
			float  m_fDistanceFromMyClosestAirbaseToLocation; // used by air units only

		//
		// DepthOfThought 2
		//
			//float  m_fDistanceFromMyClosestAirbaseTo

	public:
/*
		// City Proximity
		float  m_fAirDistanceFromThisCity;
		float  m_fAirTimeFromThisCity;
		bool   m_bSomeAirUnitCanReachTheLocationFromThisCity;
		bool   m_bSomeAirUnitCanReachTheLocationFromSomeAirbase;
		bool   m_bGroundRouteExists;
		bool   m_bSeaRouteExists;
		// UnitTemplates + time to build, time to reach Location
		CList< CEOSAIUnitTemplateAndProximity* >  m_UnitsThatCanBeBuilt;

		// Airfield Proximity
		float  m_fAirDistanceFromThisAirfield;
		bool   m_bSomeAirUnitCanReachTheLocationFromThisCity;

		// AirUnit Proximity
		float  m_fAirDistanceFromMyAirbase;
		bool   m_bAirUnitCanReachTheLocationFromThisCity;
		bool   m_bAirUnitCanReachTheLocationFromSomeAirbase;
		CList< CEOSAITaskPrerequisite* >  m_Prerequisites; // airbase needed, etc
/-*
I can dramatically simplify the ground-unit movement calculations
	if I change all the terrain to a particular cost multiplier rather
	than having every unit determine its movement rate through each terrain.
I might want to have two different costs for SoftUnits as opposed to Armored.
	(Although, trucks are considered SoftUnits - but might have the same
	movement capabilies of tanks.)
*-/
		// GroundUnit Proximity
		bool   m_bCanReachLocationViaLandRoute; // if false, needs a transport
		float  m_fGroundDistanceViaSpatialMap;
		float  m_fGroundTimeViaSpatialMap;
		CList< CEOSAITaskPrerequisite* >  m_Prerequisites; // transport needed, etc

		// GroundUnit Proximity
		bool   m_bCanReachLocationViaSeaRoute;
		float  m_fSeaDistanceViaSpatialMap;
		CList< CEOSAITaskPrerequisite* >  m_Prerequisites;
*/

		// If air unit or a city (which can build air units)
		//   If the Location is not immediately within range, come up with
		//   an estimate of how long it will take to get there (jumping from
		//   one airbase to another).
		bool   m_bAirPathCalculated;
		bool   m_bThereIsAPathThroughSomeAirbase; // the Location is too far out
		float  m_fShortestAirTimeFromPathway;
		//bool   m_bIsWithinMovementRangeOfCurrentOrFutureAirbase; // I still might be able to rebase and reach the target

		// If ground unit or a city (which can build ground units)
		// Look over the unit's pathway, figure out if it can get there without
		//    transports.  If not, add a prerequisite, and come up with an
		//    estimate of how long it will take to get there.
		bool       m_bGroundPathCalculated;
		bool       m_bDirectGroundRouteExists; // no transports needed
		float      m_fClosestGroundDistance;
		CEOSAIRegion2* m_pClosestGroundRegion;

		// If a sea-unit or a city (if this is a coastal city which can build sea units)
		// Look over the unit's pathways, figure out if I can get there and add
		//    any prerequisites (e.g. capture a city).
		// Note: there are sometimes multiple possible pathways (e.g. through
		//    the panama canal, around the strait of Magellan).
		bool       m_bSeaPathCalculated;
		bool       m_bDirectSeaRouteExists; // don't need to move through city
		float      m_fClosestSeaDistance;
		CEOSAIRegion2* m_pClosestSeaRegion;
};


