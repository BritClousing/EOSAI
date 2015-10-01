
#pragma once

//
// Defines the pathway that an GroundUnit is going to take across the map.
// It can be used to calculate the resistence that the GroundUnit will experience.
// The GroundUnitPathway can calculate land-only routes versus transport routes.
//    It can also calculate AirTransport routes and SeaTransport routes.
//

#include "EOSAIGroundUnitPathwayFinder.h"
#include "EOSAIUnitPathwayFinder.h"
#include "EOSAILocation.h"
class CEOSAIBrain;
class CEOSAIPoiObject;
class CEOSAIUnit2;
class CEOSAIAirbasesSet;
class CEOSAIRegionPathwayMap;
//class CAIGroundUnitPathPoint;
class CAIGroundPathfinderResult;
class CGroundUnitAndTransportPathway;
class CEOSAIPathfinderPoint;

// Records a combined (GroundUnit / Transport) pathway 
//   In most cases, this will be used to check feasibility and then be discarded
class CGroundUnitAndTransportPathway
{
	public:
		CGroundUnitAndTransportPathway()
		{
			m_fStepTime = 0.0f;
			m_fPathwayValue = 0.0f;
		}
		virtual ~CGroundUnitAndTransportPathway();

		void  Clear()
		{
			m_fStepTime = 0.0f;
			m_fPathwayValue = 0.0f;
			m_GroundUnitToPickupAIRegionPath.RemoveAll();
			m_TransportToPickupAIRegionPath.RemoveAll();
			m_PickupToDropoffAIRegionPath.RemoveAll();
			m_DropoffToTargetAIRegionPath.RemoveAll();
		}
		void   Copy( CGroundUnitAndTransportPathway* pOther )
		{
			Clear();

			m_fStepTime = pOther->m_fStepTime;
			m_fPathwayValue = pOther->m_fPathwayValue;

			POSITION pos;
			pos = pOther->m_GroundUnitToPickupAIRegionPath.GetHeadPosition();
			while( pos ){ m_GroundUnitToPickupAIRegionPath.AddTail( pOther->m_GroundUnitToPickupAIRegionPath.GetNext( pos ) ); }
			pos = pOther->m_TransportToPickupAIRegionPath.GetHeadPosition();
			while( pos ){ m_TransportToPickupAIRegionPath.AddTail( pOther->m_TransportToPickupAIRegionPath.GetNext( pos ) ); }
			pos = pOther->m_PickupToDropoffAIRegionPath.GetHeadPosition();
			while( pos ){ m_PickupToDropoffAIRegionPath.AddTail( pOther->m_PickupToDropoffAIRegionPath.GetNext( pos ) ); }
			pos = pOther->m_DropoffToTargetAIRegionPath.GetHeadPosition();
			while( pos ){ m_DropoffToTargetAIRegionPath.AddTail( pOther->m_DropoffToTargetAIRegionPath.GetNext( pos ) ); }
		}

		float  m_fStepTime;
		float  m_fPathwayValue; // want to minimize the pathway value (it includes distance+danger)

		// None of the AIRegions are owned
		// Step 1
		CList< CEOSAIPathfinderPoint* >  m_GroundUnitToPickupAIRegionPath;
		CList< CEOSAIPathfinderPoint* >  m_TransportToPickupAIRegionPath;
		// Step 2
		CList< CEOSAIPathfinderPoint* >  m_PickupToDropoffAIRegionPath;
		// Step 3
		CList< CEOSAIPathfinderPoint* >  m_DropoffToTargetAIRegionPath;
};

class CAIGroundUnitPathwayFinder : public CEOSAIUnit2PathwayFinder
{
	public:
		CAIGroundUnitPathwayFinder( CEOSAIUnit2* pAIUnitActor );
		CAIGroundUnitPathwayFinder( CEOSAICity* pAICityActor, CEOSAIUnitTemplate* pAIUnitTemplate, float fBuildTime = 0.0f );

		//
		// 1) Calculate AIRegion pathway (ignore danger, sighting)
		//      GroundOnly Path, Imaginary (Best Case) Transport Path
		// 2) Calculate AIRegion pathway + existing transports
		// 3) Calculate AIRegion pathway + existing and hypothetical transports
		//


		//
		// Cached pathway data
		//   (Appears in CEOSAIUnit2)
		//
			//CEOSAIRegionPathwayMap*  InvokeDirectAIRegionMapToEverywhere();
			//CEOSAIRegionPathwayMap*  InvokeDirectAIRegionMapToCoasts();
			//CEOSAIRegionPathwayMap*  InvokeLandAndImaginarySeaTransportAIRegionMapToEverywhere();

		//
		// Results
		//
			// CalculatePath
			//   Calculates a number of things:
			//   1) A land-path only
			//   2) If the Initial-Transport-Path is useful or necessary, then
			//      Create transport-assisted path possibilities:
			//        - Consider using each existing transport (sneak-attack or straight-path)
			//        - Consider having each city create a transport (sneak-attack or straight-path)
			//   The FinalPath does not decide which transport to use (that still has to be worked out)
			void          PreprocessPath(); // used to figure out if I need a transport, align Start/End Locations
			virtual void  CalculateAIRegionPathFromPredefinedSteps();
			//virtual void  CalculateAIRegionPathTiming();
			//virtual float GetTransporteeArrivalTimeAtPickupLocation( CEOSAIUnit2PathwayPredefinedStep* pTransportPickupStep );
			virtual void  CalculateResultPath();

	private:

		void  CalculatePathwayStepUsingTransport2( CEOSAIUnit2* pAITransport, 
												   CEOSAIRegionPathwayMap& GroundUnitToPickupMap, CEOSAIRegionPathwayMap& GroundUnitToPickupMapWithCoasts,
												   CEOSAIRegionPathwayMap& GroundUnitToDropoffMap, CEOSAIRegionPathwayMap& GroundUnitToDropoffMapWithCoasts,
												   CGroundUnitAndTransportPathway* pPathway );

		//void  InvokeCoastalPathwayMap( CEOSAILocation Location, CEOSAIRegionPathwayMap& EnlargedGroundPathwayMap );
		//void  CalculatePickupToDropoffPathway( CEOSAIUnit2* pMovingUnit, CEOSAIRegionPathwayMap* pStartingMap, CEOSAIRegionPathwayMap* pEndingMap );

		//CEOSAIRegionPathwayMap  m_AIRegionMapToEverywhere;
		//CEOSAIRegionPathwayMap  m_AIRegionMapToCoasts;
		//CEOSAIRegionPathwayMap  m_LandAndImaginarySeaTransportAIRegionMapToEverywhere;
};
