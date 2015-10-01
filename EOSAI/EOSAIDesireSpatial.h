
#pragma once

#include "EOSAIDesire2.h"
#include "EOSAILocation.h"
#include "EOSAIMultiRegionManager2.h"
#include "EOSAIEnumDesireType.h"
class CEOSAIDesireConnection;
class CEOSAIUnit2;
class CEOSAIPoiObject;
class CEOSAIUnit2ActionIdea;

//
// This is an abstract class
// Spatial desires have a location associated with them.
//   Example: Capture a city, protect a multiregion, bomb a target, defend isthmus, etc.
// (As opposed to more general desires like "use strategic bombing against the enemy")
//

class CEOSAIDesireSpatial : public CEOSAIDesire2
{
	public:

		CEOSAIDesireSpatial();
		virtual ~CEOSAIDesireSpatial();

		virtual CString Dump();

		//void   InvokeProcessingLevel( EnumDesireProcessingLevel eNewProcessingLevel );

		// Accessibility is based on proximity and the movement rates of units required for the task
		//   Capturing a city 500 pixels away might be infeasible, but
		//   bombing a city 500 pixels is feasible if I have aircraft that can reach it
		void   CalculateAccessibility01();
		float  GetAccessibility01(){ ASSERT( m_fAccessibility01 > 0.0f ); return m_fAccessibility01; }

		// Override
		//
			virtual bool GetInvolvesAnAttack(){ return false; }
			virtual EOSAIEnumAIDesireType  GetType(){ return EOSAIEnumAIDesireType_Undefined; } // Type Information (Capture, Degrade, etc)

		// Desire Fulfillment
		//
			//void  IsBeingFulfilled( CAITacticalProject* pTacticalProjectFulfillingThisDesire );
			//bool  IsBeingFulfilled(){ return m_pTacticalProjectFulfillingThisDesire != NULL; }

		// AttachedUnits = Units within 30 pixels of this target
		//
			void   GetDestroyUnitDesiresInTheImmediateArea( CList< CEOSAIDesireSpatial* >* pDesires );
			void   GetUnitsInTheImmediateArea( CList< CEOSAIUnit2* >* pEnemyUnits );

		//
			virtual bool InvolvesGroundTargets(){ return false; }
			bool   RequiresLandUnits();
			bool   RequiresTransport(){ return RequiresLandUnits() && NationwideLandRouteExists() == false; }
			//float  GetTransportNeed01(); // 0 = no need, 1 = must have transport, 0.5 = transport useful
			bool   RequiresSeaUnits();
			bool   NationwideLandRouteExists(){ return m_bNationwideLandRouteExists; }
			bool   NationwideSeaRouteExists(){ return m_bNationwideSeaRouteExists; }

		// Simple Interest
		//
			virtual float GetSimpleInterest(){ return 0.0f; }
			virtual float GetAreaSimpleInterest(); // based on my RawScore + RawScores of connected

		// Effort
		//
			void                CalculateExpectedPathwayResistance();
			CList< CEOSAIUnit2* >*  GetPathwayResistance(){ return &m_PathwayResistance; }

			float  GetNationwideEstimatedTimeToTarget(){ return m_fNationwideEstimatedTimeToTarget; }

			// Look over all my units, figure out how quickly the required units can get there
			//virtual void  CreateUnitActionIdeas();
			//float        GetTimeToReachTargetViaPathwaysAndMyUnits();
			//CEOSAIRegionPathwayMap*  GetPathwayResult(){ return &m_PathwayResult; }

		// Location
		//
			virtual CEOSAILocation GetLocation(){ return CEOSAILocation( CEOSAILocation::Pixel,0.0f,0.0f ); }
			virtual float     GetRadius(){ return 0.0f; }
			float             GetDistance( CEOSAILocation Location );
			CEOSAIMultiRegion2*    GetMultiRegion();
			bool              IsOnLand();//{ ASSERT( false ); return false; }

		// PoiObject
		//
			virtual long            GetTargetObjectId(){ return 0; }
			//virtual CEOSAIPoiObject*  GetAIPoiObjectTarget(){ return NULL; }
			virtual CEOSAIPoiObject*  GetAIPoiObjectTarget(){ return NULL; }

		// Air-Distance
		//
			float   GetDistanceFromMyClosestAirbase(); // includes allies' airbases, but not carriers
			float   GetDistanceFromMyClosestCity();
			float   GetDistanceFromMyClosestCitRes();

			bool    IsOnTheSameGeoAsMyCitRes();

		// Connected Desire
		//
			virtual void  CreateConnectedDesire( long iPlayer, CEOSAIDesireSpatial* pSpatialDesire );
			//CList< CEOSAIDesireConnection* >*  GetConnectedDesires(){ return &m_ConnectedDesires; }
			CList< CEOSAIDesireConnection* >*  GetNearbyDesires(){ return &m_ConnectedDesires; }

			float GetEnemyCombatPowerWithinRange( float fRange ); // Checks the connected desires

		// UnitActionIdea Allocations
		//
			void  AddAllocation( CEOSAIUnit2ActionIdea* pAIUnitActionIdea ){ m_UnitActionIdeaAllocations.AddTail( pAIUnitActionIdea ); CalculateAllocatedFulfillment(); }
			void  CalculateAllocatedFulfillment();
			float GetAllocatedFulfillment(); // 0.0 = no fulfillment, 1.0 = completely fulfilled

		//
			CEOSAIRegionPathwayMap*  InvokePathToEverywhere();
			//void CreatePathwayFromEverywhereToHere();

	protected:

		// Processing Level
		//EnumDesireProcessingLevel  m_eProcessingLevel;

		//
			//CAITacticalProject*  m_pTacticalProjectFulfillingThisDesire;

		// m_eProcessingLevel = EnumInitialCalculation
		//    At this level, I know the accessibility of the desire from my entire nation,
		//    what other desires are in the area, and pathway resistence (based on nationwide pathway)
		//
			bool   m_bNationwideLandRouteExists;
			bool   m_bNationwideSeaRouteExists;
			
			float  m_fAccessibility01;
			float  m_fNationwideEstimatedTimeToTarget;
			float  m_fNationwidePathwayDistance; // just for debugging
			CList< CEOSAIDesireConnection* >  m_ConnectedDesires; // owned

			bool   m_bPotentialResistanceHasBeenCalculated;
			CList< CEOSAIUnit2* >  m_PathwayResistance; // pathway

		// m_eProcessingLevel = EnumCustomPathwayCalculation
		//    At this level, I know the pathways from everywhere (i.e. from every unit) to [here]
		//
			//float  m_fTimeToReachTargetViaPathwaysAndMyUnits;
			//CEOSAIRegionPathwayMap  m_PathwayToEverywhereResult; // will contain pathways from everywhere to [here]

		//
			float  m_fFulfillment;
			CList< CEOSAIUnit2ActionIdea* >  m_UnitActionIdeaAllocations;//m_Allocations;
};
