
#pragma once

#include "EOSAIUnitPathwayFinder.h"
#include "EOSAILocation.h"
class CEOSAIPoiObject;
class CEOSAIUnit;
class CEOSAICity;
class CAISeaPathPoint;
class EOSAI::UnitPathwayResult;

//
// This class is used for finding a pathway for sea units
//

class CEOSAISeaUnitPathwayFinder : public CEOSAIUnitPathwayFinder
{
	public:
		//CEOSAISeaUnitPathwayFinder( CEOSAIBrain* pAIBrain, long iPlayer, CEOSAIPoiObject* pActor, CUnitTemplate* pUnitTemplate );
		CEOSAISeaUnitPathwayFinder( CEOSAIUnit* pAIUnitActor );
		CEOSAISeaUnitPathwayFinder( CEOSAICity* pAICityActor, CEOSAIUnitTemplate* pAIUnitTemplate, float fBuildTime = 0.0f );
		virtual ~CEOSAISeaUnitPathwayFinder();

		//
		// Cached pathway data
		//   (Transfered from CEOSAIUnit)
		//
			//CEOSAIRegionPathwayMap*  InvokeDirectAIRegionMapToEverywhere();

		// [this] unit is a transport. calculate Time, Pathway, and Danger for transporting another unit
		//    This method calculates simple pathway data (assume no other units get transported)
		//
			//void  CalculateTransportPath( 
			//		CEOSAIUnit* pTransportee, CEOSAILocation TransporteeStartLocation, CEOSAILocation TransporteeTargetLocation,
			//		EOSAI::UnitPathwayResult* pAIUnitPathwayResult );
					//CAITransportPath* pTransportPath );

			virtual bool UpdateEndLocation( CEOSAIUnitPathwayPredefinedStep* pTransportPickupOrDropoffStep );
			//virtual void UpdatePickupPredefinedStep( CEOSAIUnitPathwayPredefinedStep* pGroundUnitPredefinedStep );
			//virtual bool UpdatePredefinedStepWithTransportAssistedPath( CEOSAIUnitPathwayPredefinedStep* pGroundUnitPredefinedStep );
			//virtual bool UpdateResultStepWithTransportAssistedPath( EOSAI::UnitPathwayResultStep* pGroundUnitResultStep );

		//
		// Results
		//
			//virtual void ClearResults();

			// Initial Evaluation
			//
				//virtual void  CalculateInitialEvaluation();
				//EOSAI::UnitPathwayResult*  GetInitialEvaluationRoute(){ return &m_SimpleRoute; }

			//
		//	virtual long  GetHighestPathwayLevel(){ return 2; }
		//	virtual void  CalculatePathway( long iLevel ){ m_iCurrentPathfindingLevel = iLevel; RecalculateCurrentPathway(); }
			// iLevel 1 = Calculate MultiRegion pathway
			// iLevel 2 = Calculate AIRegion pathway
		//	void          RecalculateCurrentPathway();

			// Final Evaluation
			//
				void          PreprocessPath();
				virtual void  CalculateAIRegionPathFromPredefinedSteps();
				//virtual float GetTransportArrivalTimeAtDropoffLocation( CEOSAIUnitPathwayPredefinedStep* pGroundUnitStep );
				//virtual float GetTransporteeArrivalTimeAtPickupLocation( CEOSAIUnitPathwayPredefinedStep* pGroundUnitsStep );
				//virtual void CalculateAIRegionPathTiming_UptoDropoffTask( CEOSAIUnitPathwayPredefinedStep* pGroundUnitsStep );
				virtual void  CalculateResultPath();

		//
		// Create orders
		//
			//virtual void CreateOrders();
			//void         CreateOrders( EOSAI::UnitPathwayResult* pPath ); // limit pathway to the AIRegions

	private:

		void SolvePathway();

		//EOSAI::UnitPathwayResult  m_SimpleRoute;

		//CEOSAIRegionPathwayMap  m_AIRegionMapToEverywhere;
};

