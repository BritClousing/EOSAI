
#pragma once

//
// Defines the pathway that an AirUnit is going to take across the map.
//   The airpathway might be as simple as [PointA] to [PointB], it might involve serveral landings,
//   and it might involve several waypoints (to avoid being seen, or anti-aircraft fire)
// The AirPathway can be used to calculate the resistence that the AirUnit will experience
// The AirPathway object can be used to validate/limit the AirUnit to its capabilities - for example,
//   not flying outside it's MovementLimit
// The AirPathway also handles missiles/kamikazis (self-destruct on attack), and ballistic missiles
//

#include "EOSAIAirUnitPathwayData.h"
#include "EOSAILocation.h"
#include "EOSAIUnitPathwayFinder.h"
class CEOSAIBrain;
class CEOSAIPoiObject;
class CEOSAIUnit;
class CEOSAIUnitTemplate;
class CEOSAIAirbasesSet;

class CAIAirPathPoint;
class CAIAirPathWaypoint;



class CEOSAIAirUnitPathwayFinder : public CEOSAIUnitPathwayFinder
{
	public:
		CEOSAIAirUnitPathwayFinder( CEOSAIUnit* pAIUnitActor );
		CEOSAIAirUnitPathwayFinder( CEOSAICity* pAICityActor, CEOSAIUnitTemplate* pAIUnitTemplate, float fBuildTime = 0.0f );
		virtual ~CEOSAIAirUnitPathwayFinder();

		//
		// Initial State
		//
			void  SetMovementConsumed( float f ){ m_fInitialMovementConsumed = f; }
			float GetMovementConsumed(){ return m_fInitialMovementConsumed; }

		// Transports
		//
			virtual bool UpdateEndLocation( CEOSAIUnitPathwayPredefinedStep* pTransportPickupOrDropoffStep );

		//
		// Results
		//
			bool  QuickCheck_PathwayCanExist();
			void  InvokeAirbaseMapPoints();

			// Initial Evaluation
			//   Make sure all points can be reached (check ranges)
			//
				bool  RunInitialEvaluation(); // return FALSE if path is not possible
				//virtual float GetInitialEvaluationTimeScore(){ return 0.0f; }

			// Level 1 = try to find a pathway using existing airbases
			// Level 2 = What airbases can I capture, where can I place carriers/airfields, can I transport missiles?

			// Final Path
			//   Take MovementStrategy into account
			//
				virtual void CalculateAIRegionPathFromPredefinedSteps();
				//virtual void CalculateAIRegionPathTiming();
				virtual void CalculateResultPath();

			// Create Orders (using Final Path)
			//
				//virtual void CreateOrders();

	private:

		float  m_fInitialMovementConsumed;

		//
		// Intermediate points (used for calculating results)
		//
			bool   m_bAirbasesHaveBeenAddedToMapPoints;
			// MapPoints are PreDefined Locations + all possible Airbases, used by the pathfinder
			CList< CEOSAIAirPathfinderMapPoint* >  m_AllMapPoints;

		//
		// Results
		//
			//CAirPathfinderResult   m_AirUnitPathway;
};
