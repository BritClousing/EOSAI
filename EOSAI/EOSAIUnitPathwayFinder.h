
#pragma once

#ifdef CREATE_EOSAI_DLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

//
// This is a base-class for the CAIAirUnitPathway, CAIGroundUnitPathway, and CAISeaUnitPathway
//   It provides a generic interface for objects to calculate/get information
//
// Why do I use this class instead of just creating an AIUnit (real or hypothetical) to find paths?
//   Because it contains specialized code for the different types of units (Ground/Sea/Air)
//   Air Units (especially) need specialized code for solving pathways.
//   The Transport code also needs special coding.
//

#include "EOSAILocation.h"
#include "EOSAIEnumForeignRelations.h"
#include "EOSAIBCDumbArray1D.h"
#include "EOSAIIntSet.h"
#include "EOSAIRegionPathfinder.h"
class CGlobalForeignRelations;
//class EOSAI::UnitPathwayResult;
class CEOSAIUnitTemplate;
class CEOSAIPoiObject;
class CEOSAIUnit;
class CEOSAICity;
class CEOSAIBrain;
class CEOSAIRegion2;
class CEOSAIUnitPathwayPredefinedStep;
class CEOSAIUnitPathwayPredefinedStepSeq;
namespace EOSAI
{
	class UnitPathwayResult;
	class UnitPathwayResultStep;
}
class CEOSAIUnitPathwayPredefinedStepPathStep;


class DLLIMPEXP CEOSAIUnitPathwayFinder
{
	public:
		static CEOSAIUnitPathwayFinder* CreatePathwayFinder( CEOSAIUnit* pAIUnitActor );
		static CEOSAIUnitPathwayFinder* CreatePathwayFinder( CEOSAICity* pAICityActor, CEOSAIUnitTemplate* pAIUnitTemplate, float fBuildTime = 0.0f );

		// Given a WaterAIRegion and a LandAIRegion, return Water and Land Locations for Pickup/Dropoff
		static bool FindPickupDropoffLocations( 
						long iWaterAIRegion, long iLandAIRegion,
						CEOSAILocation* pWaterLocation, CEOSAILocation* pLandLocation );

	public:
		CEOSAIUnitPathwayFinder( CEOSAIUnit* pAIUnitActor );
		CEOSAIUnitPathwayFinder( CEOSAICity* pAICityActor, CEOSAIUnitTemplate* pAIUnitTemplate, float fBuildTime = 0.0f );
		virtual ~CEOSAIUnitPathwayFinder();

		//
			//CEOSAIBrain*      GetAIBrain(){ return m_pAIBrain; }
		//
			CEOSAIUnit*         GetAIUnitActor(){ return m_pAIUnitActor; }
			CEOSAIPoiObject*    GetAIPoiObjectActor();
			CEOSAIUnitTemplate* GetAIUnitTemplate(){ return m_pAIUnitTemplate; }
			long             GetUnitOwner(){ return m_iUnitOwner; }

		//
			void  ClearPredefinedAndResultPaths();
		//
		// Initialization
		//
			void       SetStartLocation( CEOSAILocation Location ){ m_StartLocation = Location; } // Optional
			CEOSAILocation  GetStartLocation(){ return m_StartLocation; }
			void       SetStartTime( float f ){ m_fStartTime = f; }
			float      GetStartTime(){ return m_fStartTime; }

			//float GetEndTime();

			// Foreign Relations
			void  SetNeutralsPathwayStategy( CEOSAIRegionPathfinder::EnumNeutralsPathwayType ePathwayType ){ m_eNeutralsPathwayStategy = ePathwayType; }
			CEOSAIRegionPathfinder::EnumNeutralsPathwayType  GetNeutralsPathwayStategy(){ return m_eNeutralsPathwayStategy; }
			void  SetEnemiesPathwayStategy( CEOSAIRegionPathfinder::EnumEnemiesPathwayType ePathwayType ){ m_eEnemiesPathwayStategy = ePathwayType; }
			CEOSAIRegionPathfinder::EnumEnemiesPathwayType   GetEnemiesPathwayStategy(){ return m_eEnemiesPathwayStategy; }
			void  ResetForeignRelations();
			void  TreatAsEnemy( long iPlayer ){ m_OwnerForeignRelations[iPlayer] = EOSAIEnumForeignRelations::enum_War; } // important for sneak-attacks
			CEOSAIBCDumbArray1D< EOSAIEnumForeignRelations >*  GetForeignRelations(){ return &m_OwnerForeignRelations; } // important for sneak-attacks

			void  AvoidDangerousPathways( bool b ){ m_bAvoidDangerousPathways = b; }

			// Transport Settings - used by GroundUnits
			bool  NeedsTransport();

		// Pathway setup
		//
			// Waypoint
			void  AddWaypoint( CEOSAILocation Location );
			// Pickup/Dropoff - Transports
		//	void  AddPickupTarget( CEOSAIUnit* pAIUnitToPickup, CEOSAILocation OtherUnitLocation );//( CEOSAIUnit* pAIUnitToPickup );
		//	void  AddDropoffTarget( CEOSAIUnit* pAIUnitToDropoff, CEOSAILocation OtherUnitTargetLocation ); // Dropoff near this location
		//	CEOSAIUnitPathwayPredefinedStep*  CreateTransportAction( CEOSAIUnit* pAITransportee, CEOSAILocation TransporteeStartLocation, CEOSAILocation TransporteeEndLocation );
			//void  AddTransportAction( CEOSAIUnit* pAITransportee, CEOSAILocation TransporteeStartLocation, CEOSAILocation TransporteeEndLocation );
			//CEOSAIUnitPathwayPredefinedStep*  CreateTransportAction( CEOSAIUnitPathwayPredefinedStep* pTransporteeStep );

			// Used by Transportees (GroundUnits)
			//
				CEOSAIUnit* GetCurrentTransport();

			//
			// Used by transports
			//
				void  AppendTransportSteps( CEOSAIUnitPathwayPredefinedStep* pTransporteeStep );
				void  InsertTransportSteps( CEOSAIUnitPathwayPredefinedStep* pTransporteeStep );
				void  DeleteTransportSteps( CEOSAIUnitPathwayPredefinedStep* pTransporteeStep );

				CEOSAIUnitPathwayPredefinedStep* GetTransportPickupStep( CEOSAIUnitPathwayPredefinedStep* pTransporteeStep );
				CEOSAIUnitPathwayPredefinedStep* GetTransportDropoffStep( CEOSAIUnitPathwayPredefinedStep* pTransporteeStep );

				//virtual bool  UpdateTransportResultStep( EOSAI::UnitPathwayResultStep* pResultStep ){ return false; }
				virtual bool UpdateEndLocation( CEOSAIUnitPathwayPredefinedStep* pTransportPickupOrDropoffStep ){ return false; }
				virtual bool UpdateTransportPickupDropoffPath( CEOSAIUnitPathwayPredefinedStep* pGroundUnitStep );
				// Calculate a real pathway using [this] unit as the transport
				//virtual bool UpdateResultStepWithTransportAssistedPath( EOSAI::UnitPathwayResultStep* pGroundUnitResultStep ){ return false; }

			//
			// Used if [this] unit needs a transport
			//
				void  SetTransportToUse( CEOSAIUnit* pAITransportToUse );

			// Airfield (ground unit only)
			void  AddBuildAirfield( CEOSAILocation Location );

			// Airbase Landing (air units)
			bool  PathwayShouldAppendALanding();
			void  LandAtAirbase( CEOSAIPoiObject* pAirbase ); // (used for sighting/just-in-case)
			void  LandAtAnyAirbase(); // city,airbase,carrier
			void  AppendALandingIfPossible(){ if( PathwayShouldAppendALanding() ) LandAtAnyAirbase(); }
			//virtual void  AppendALanding( bool b ){ m_bAppendLandingToTheEnd = b; }

			// Repair
			void  AddRepairTask( CEOSAIPoiObject* pAIPoiObject );
			//void  AddRepairTaskAnywhere();
			// Attack
			CEOSAIUnitPathwayPredefinedStep*  CreateCaptureTarget( CEOSAIPoiObject* pTarget );
			CEOSAIUnitPathwayPredefinedStep*  CreateDegradeTarget( CEOSAIPoiObject* pTarget );
			CEOSAIUnitPathwayPredefinedStep*  CreateAttackUnit( CEOSAIPoiObject* pTarget );

		//
			void  InsertPickupOrDropoffStep( CEOSAIUnitPathwayPredefinedStep* pNewStep ); // Find best place
			void  InsertAsFirstStep( CEOSAIUnitPathwayPredefinedStep* pStep );
			void  AppendStep( CEOSAIUnitPathwayPredefinedStep* pNewStep ); // Add to the end
			void  AddToPredefinedSteps_InsertBefore( CEOSAIUnitPathwayPredefinedStep* pNewStep, CEOSAIUnitPathwayPredefinedStep* pInsertBeforeStep );
			//void  InvokeImaginaryTransport( CEOSAIUnitPathwayPredefinedStep* pPredefinedStep );
		//
			// zero-based index
			CEOSAIUnitPathwayPredefinedStep* GetPredefinedPathStep( long i );
			CList< CEOSAIUnitPathwayPredefinedStep* >*  GetPreDefinedPath(){ return &m_PreDefinedPath; } // This is somewhat ordered

			bool IncludesAnAttack();

		//
			void ValidatePredefinedStepsStartEndLocations();

		//
		// Pathfinding Results
		//
			// Path
			//   Take MovementStrategy into account
			//
				virtual void  CalculateAIRegionPathFromPredefinedSteps(){}
				void          UpdateRedPath();
				float         CalculateRedPathArrivalTimeAtGeo( long iTargetGeo );
				//void          SetCacheRedPathGeoArrivalTime( float fTime ){ m_fCacheGeoArrivalTime = fTime; }
				//float         GetCacheRedPathGeoArrivalTime(){ return m_fCacheGeoArrivalTime; }
				//virtual float GetTransportArrivalTimeAtDropoffLocation( CEOSAIUnitPathwayPredefinedStep* pGroundUnitStep ){ ASSERT( false ); return 0.0f; }
				//virtual float GetTransporteeArrivalTimeAtPickupLocation( CEOSAIUnitPathwayPredefinedStep* pTransportPickupStep ){ ASSERT( false ); return 0.0f; }
				virtual void  CalculateResultPath(){}
				bool          ResultPathIsValid();
				EOSAI::UnitPathwayResult* GetResultPath(){ return m_pResultPath; }

				void  GetFullRedMovementPath( CList< CEOSAIUnitPathwayPredefinedStepPathStep* >* pTimedMovementPath );
				void  ConstructFullRedPath();

				void  AddPickupToDropoffToTimedMovementPath( 
						CEOSAIUnitPathwayPredefinedStep* pTransporteeStep,
						float* fTransporteeTime, // Starts as the Tranportee Pickup Time
						CList< CEOSAIUnitPathwayPredefinedStepPathStep* >* pTimedPath );
				//void  AddPickupToDropoffToTimedMovementPath( 
				//		CEOSAIUnitPathwayPredefinedStep* pTransporteeStep,
				//		float* fTransporteeTime );

		// Create orders
		//
			void  CreateOrders(); // Create a Pathway Result (if it doesn't exist) + Create Orders

	protected:

		//CEOSAIBrain*  m_pAIBrain;

		long              m_iUnitOwner;   // Important for airbase calculations
		CEOSAIUnit*          m_pAIUnitActor; // optional
		CEOSAICity*          m_pAICityActor; // optional
		CEOSAIUnitTemplate*  m_pAIUnitTemplate;

		// Initial State
		//
			CEOSAILocation  m_StartLocation;
			float      m_fStartTime;


		// Pathfinding strategy
		//
			bool  m_bAvoidDangerousPathways;
			CEOSAIRegionPathfinder::EnumNeutralsPathwayType  m_eNeutralsPathwayStategy;
			CEOSAIRegionPathfinder::EnumEnemiesPathwayType   m_eEnemiesPathwayStategy;
			CEOSAIBCDumbArray1D< EOSAIEnumForeignRelations >  m_OwnerForeignRelations; // Team, Alliance, Neutral, War, etc

		// Path Steps
		//
			//void  InsertPredefinedStep( CEOSAIUnitPathwayPredefinedStep* pStep );
			//void  InsertPredefinedStepAsFirstStep( CEOSAIUnitPathwayPredefinedStep* pStep );
			void  DeletePredefinedStep( CEOSAIUnitPathwayPredefinedStep* pStep );
			CList< CEOSAIUnitPathwayPredefinedStep* >  m_PreDefinedPath; // This is somewhat ordered

		// Full RedPath (this can be invalid if any units change their path or Predefined Steps)
		//
			CList< CEOSAIUnitPathwayPredefinedStepPathStep* >  m_FullRedMovementPath;
			//float  m_fCacheGeoArrivalTime;

		//
		// Results
		//
			//bool  m_bPathIsValid;
			//EOSAI::UnitPathwayResult*  m_pPathwayResult; // owned, get cleared on each calculation, but can be stolen 
			EOSAI::UnitPathwayResult*  m_pResultPath; // owned, get cleared on each calculation, but can be stolen 
			//bool  m_bOrdersCreated;
};



/*
class EOSAI::UnitPathwayResult
{
	public:
		EOSAI::UnitPathwayResult()
		{
			m_fTime = 0.0f;
			m_fDanger = 0.0f;
			m_iTransportsNeeded = 0;
		}
		~EOSAI::UnitPathwayResult(){ Clear(); }
		//
		void  Clear();//{ m_fTime = 0.0f; m_fDanger = 0.0f; while( m_PathwayNodes.IsEmpty() == FALSE ){ delete m_PathwayNodes.RemoveHead(); } }
		//bool  PathwayWasCalculated(); // Calculated (but not necessarily valid)
		bool  PathwayIsValid();
		void  AddResultStep( EOSAI::UnitPathwayResultStep* pResultStep );

		// Full result
		float  m_fTime;
		float  m_fDanger;
		long   m_iTransportsNeeded;
		//CList< EOSAI::UnitPathwayResultStep* >  m_PathwayNodes;
		CList< EOSAI::UnitPathwayResultStep* >  m_PathwaySteps;

		//
		CList< EOSAI::UnitPathwayResult* >  m_SupportUnits; // Transports, Carriers used in movement
};
*/

