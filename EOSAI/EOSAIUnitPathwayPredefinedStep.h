
#pragma once

#ifdef CREATE_EOSAI_DLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

//#include "AIUnitPathway.h"
#include "EOSAILocation.h"
//#include "EnumPathwayResultStep.h"
#include "EOSAIRegionPathfinder.h"
//class EOSAI::UnitPathwayResultStep;
class CEOSAIUnitPathwayFinder;
class CEOSAIPoiObject;
class CEOSAICity;
class CEOSAIUnit;
class CEOSAIUnitPathwayPredefinedStep;
namespace EOSAI
{
	class UnitPathwayResultStep;
}
class CEOSAIRegion2;
class CEOSAIUnitActionIdea;

// Describes the movement of a Transport (UnitTemplate) from pickup to dropoff
/*
class CAITransportTemplatePathway
{
	public:
		CAITransportTemplatePathway()
		{
			m_pTransportUnitTemplate = NULL;
			m_bSneakAttackPathway = false;
		}
		CUnitTemplate*     m_pTransportUnitTemplate;
		bool               m_bSneakAttackPathway;
		CEOSAIRegionPathwayMap  m_SeaRoute_DropoffToPickup;
};
*/

// Remembers the time/danger of each Transport/CityBuild
/*
class CAITransportPossibility
{
	public:
		CAITransportPossibility()
		{
			m_pAITransportUnitTemplatePathway = NULL;
			m_pAIPoiObject = NULL;
			m_pTransportUnitTemplate = NULL;
			m_fTotalTime = 0.0f;
			m_fBuildTime = 0.0f;
			m_iGroundUnitPickupAIRegion = 0;
			m_iTransportPickupAIRegion = 0;
		}
		CAITransportTemplatePathway*  m_pAITransportUnitTemplatePathway;
		//CPoiObject*    m_pPoiObject;    // CUnit or CCity
		CEOSAIPoiObject*  m_pAIPoiObject;    // CUnit or CCity
		CUnitTemplate* m_pTransportUnitTemplate;
		//
		float m_fTotalTime;
		float m_fBuildTime; // I include the build time because units can be reused for other tasks (so the time is not quite as costly as normal movement time)
		long  m_iGroundUnitPickupAIRegion;
		long  m_iTransportPickupAIRegion;
};
*/
/*
class CEOSAIUnitPathwayPredefinedStepSeq
{
	public:
		CEOSAIUnitPathwayPredefinedStepSeq( CEOSAIUnitPathwayPredefinedStep* pStep, long iSubStep )
		{
			m_pPredefinedStep = pStep;
			m_iSubStep = iSubStep;
		}
		CEOSAILocation  GetEndLocation();

		CEOSAIUnitPathwayPredefinedStep*  m_pPredefinedStep;
		long  m_iSubStep; // if this is a Transport Step, SubStep1 = Pickup, SubStep2 = Dropoff
};
*/

// Used to track a Predefined Step's pathway through different PathfinderPoints
class CEOSAIUnitPathwayPredefinedStepPathStep
{
	public:
		CEOSAIUnitPathwayPredefinedStepPathStep( CEOSAIPathfinderPoint* pPathfinderPoint, float fOffsetTime )
		{
			m_pPathfinderPoint = pPathfinderPoint;
			m_fOffsetTime = fOffsetTime;
		}
		CEOSAIPathfinderPoint* m_pPathfinderPoint;
		float m_fOffsetTime;
};

class DLLIMPEXP CEOSAIUnitPathwayPredefinedStep
{
	public:
		enum EnumEndType
		{
			enum_EndOfStep,
			enum_MyArrivalAtPickupLocation,
		};

		enum EnumTask
		{
			enum_Undefined,
			//
			enum_Waypoint,
			enum_LandAtAirbase,
			//
			enum_BuildAirfield,
			enum_Repair,
			enum_Upgrade,
			//
			enum_AttackUnit,
			enum_CaptureCitRes,
			enum_DegradeCityOrAirfield,
			//
			//enum_Transport,
			enum_PickupTarget,
			enum_DropoffTarget,
		};

		CEOSAIUnitPathwayPredefinedStep( CEOSAIUnitPathwayFinder* pAIUnitPathwayFinder );
		~CEOSAIUnitPathwayPredefinedStep();

		void  SetAIUnitActionIdea( CEOSAIUnitActionIdea* pIdea ){ m_pAIUnitActionIdea = pIdea; }
		CEOSAIUnitActionIdea*  GetAIUnitActionIdea(){ return m_pAIUnitActionIdea; }

		CEOSAIUnitPathwayFinder*      GetAIUnitPathwayFinder(){ return m_pAIUnitPathwayFinder; }
		EOSAI::UnitPathwayResultStep*  GetResultStep();

		//
			CEOSAIUnitPathwayPredefinedStep*  GetPrevStep();
			CEOSAIUnitPathwayPredefinedStep*  GetNextStep();

		// Setup
		//
			void      SetStartLocation( CEOSAILocation StartLocation );
			CEOSAILocation GetStartLocation(){ return m_StartLocation; }
			void      SetEndLocation( CEOSAILocation EndLocation ); // update next start
			CEOSAILocation GetEndLocation(){ return m_EndLocation; }

			void  UpdateStartToEndDistancesAndTimes();
			float GetCrowsFlightEstimatedMovementTime(){ return m_fCrowsFlightEstimatedMovementTime; }
			//float GetEndTime(){ return m_fStepEndTime; }
			//float GetStepTime(){ return m_fStepTime; }
			float GetDirectMovementStepTime(){ return m_fDirectMovementStepTime; }
			//float GetEstimatedEndTime();
			//float GetEstimatedPickupTime();
			//float GetEndTime(); // EstimatedEndTime, or the ResultStep's EndTime
		//	float GetTransportTimeToPickupLocation();
		//	float GetGroundUnitTimeToPickupLocation();
		//	float GetGroundUnitWaitTimeAtPickupLocation(); // negative value = transport arrives first
			void  InvokeImaginaryTransportPath();

			// Time
			float GetStartTime();

			// 
			bool  EndLocationIsVariable(){ return m_eTask == enum_PickupTarget || m_eTask == enum_DropoffTarget; }

			// Pathway settings
			void  AvoidDangerousPathways( bool b );

		// What task?
		//
			void  GotoWaypoint( CEOSAILocation WaypointLocation );
			//
			void  LandAtAirbase( CEOSAIPoiObject* pAIAirbase );
			void  LandAtAnyAirbase();
			void  BuildAirfield( CEOSAILocation BuildLocation );
			//
			void  Pickup( CEOSAIUnitPathwayPredefinedStep* pTransporteeStep );
			void  Dropoff( CEOSAIUnitPathwayPredefinedStep* pTransporteeStep );
			void  UpgradeAt( CEOSAIPoiObject* pAIPoiObject );
			//
			void  RepairAt( CEOSAIPoiObject* pAIPoiObject );
			void  RepairAnywhere(); // Look for a place to get repaired (very important for damaged units)
			//
			void  CaptureCitRes( CEOSAIPoiObject* pAITarget );
			void  DegradeTarget( CEOSAIPoiObject* pAITarget );
			void  AttackUnit( CEOSAIUnit* pAITarget );
			//
			EnumTask  GetTask(){ return m_eTask; }
			bool      IsAWaypoint(){ return m_eTask == enum_Waypoint; }
			bool      IsAnUpgrade(){ return m_eTask == enum_Upgrade; }
			bool      IsAnAttack(){ return m_eTask == enum_AttackUnit || m_eTask == enum_CaptureCitRes || m_eTask == enum_DegradeCityOrAirfield; }
			bool      IsALandAtAirbase(){ return m_eTask == enum_LandAtAirbase; }
			bool      IsAPickup(){ return m_eTask == enum_PickupTarget; }
			bool      IsADropoff(){ return m_eTask == enum_DropoffTarget; }

		// Transport-Support Functions (either [this] needs a transport, or [this] is the transport-task)
		//
			// Calculated by Preprocess
			bool  RequiresTransport(){ ASSERT( m_fCrowsFlightDistance<999999.0f ); return (m_fCrowsFlightDistance*3.0f)+100.0f < m_fDirectPathDistance; }
			bool  PrefersTransport(){ ASSERT( m_fCrowsFlightDistance<999999.0f ); return (m_fCrowsFlightDistance*2.0f)+50.0f < m_fDirectPathDistance; }

			float GetStartToPickupDistance(); // Using Real or Imaginary Transport

			// Imaginary Transport
			//
				void  SetImaginaryTransport_PickupGroundAIRegion( long i ){ m_iImaginaryTransport_PickupGroundAIRegion = i; }
				void  SetImaginaryTransport_PickupWaterAIRegion( long i ){ m_iImaginaryTransport_PickupWaterAIRegion = i; }
				void  SetImaginaryTransport_DropoffGroundAIRegion( long i ){ m_iImaginaryTransport_DropoffGroundAIRegion = i; }
				void  SetImaginaryTransport_DropoffWaterAIRegion( long i ){ m_iImaginaryTransport_DropoffWaterAIRegion = i; }
				CEOSAILocation  GetImaginaryTransportPickupLocation();
				CEOSAILocation  GetImaginaryTransportDropoffLocation();
				//float GetImaginaryTransport_StartToPickupDistance();
				//float GetImaginaryTransport_DropoffToTargetDistance();

			// Real Transport
			//
				//void  UpdateEndLocation(); // Pickup or Dropoff Locations can change

				// If this step get deleted, then notify the transport to remove support for this step
				CEOSAIUnit*  GetTransportToUse(){ return m_pTransportToUse; }
				void  UseTransportAndCreatePickupDropoffSteps( CEOSAIUnit* pTransportToUse );
				void  SetRealTransport_PickupGroundAIRegion( long i ){ m_iRealTransport_PickupGroundAIRegion = i; }
				long  GetRealTransport_PickupGroundAIRegion(){ return m_iRealTransport_PickupGroundAIRegion; }
				void  SetRealTransport_PickupWaterAIRegion( long i ){ m_iRealTransport_PickupWaterAIRegion = i; }
				long  GetRealTransport_PickupWaterAIRegion(){ return m_iRealTransport_PickupWaterAIRegion; }

				void  SetRealTransport_DropoffGroundAIRegion( long i ){ m_iRealTransport_DropoffGroundAIRegion = i; }
				long  GetRealTransport_DropoffGroundAIRegion(){ return m_iRealTransport_DropoffGroundAIRegion; }
				void  SetRealTransport_DropoffWaterAIRegion( long i ){ m_iRealTransport_DropoffWaterAIRegion = i; }
				long  GetRealTransport_DropoffWaterAIRegion(){ return m_iRealTransport_DropoffWaterAIRegion; }

				CEOSAILocation  GetRealTransportPickupLocation();

			CEOSAIUnitPathwayPredefinedStep*  GetTransporteeStep(){ return m_pTransporteeStep; }
			CEOSAIUnitPathwayPredefinedStep*  GetTransportPickupStep(){ return m_pTransportPickupStep; }
			CEOSAIUnitPathwayPredefinedStep*  GetTransportDropoffStep(){ return m_pTransportDropoffStep; }

		//
			void  SetDirectPathDistance( float f ){ m_fDirectPathDistance = f; }
			float GetDirectPathDistance(){ return m_fDirectPathDistance; }
			void  SetCrowsFlightDistance( float f ){ m_fCrowsFlightDistance = f; }
			float GetCrowsFlightDistance(){ return m_fCrowsFlightDistance; }
			CEOSAIPoiObject*  GetAITarget(){ return m_pAITarget; }

		// MovementPath
		//
			void  ClearDirectMovementPath();
			void  AddToDirectMovementPathTail( CEOSAIPathfinderPoint* p );
			void  ClearPreTransportMovementPath();
			void  AddToPreTransportMovementPathHead( CEOSAIPathfinderPoint* p );
			void  ClearPostTransportMovementPath();
			void  AddToPostTransportMovementPathTail( CEOSAIPathfinderPoint* p );

			// Used for timing, danger calculations
			CList< CEOSAIPathfinderPoint* >*  GetDirectMovementPath(){ return &m_DirectMovementPath; }
			CList< CEOSAIPathfinderPoint* >*  GetPreTransportMovementPath(){ return &m_PreTransportMovementPath; } // If using a transport
			CList< CEOSAIPathfinderPoint* >*  GetPostTransportMovementPath(){ return &m_PostTransportMovementPath; }

		//	void   SetTimedMovementPath_TransportPickupTime( float f ){ m_fTimedMovementPath_TransportPickupTime = f; } // Used by transport and transportee
		//	float  GetTimedMovementPath_TransportPickupTime(){ return m_fTimedMovementPath_TransportPickupTime; } // Used by transport and transportee
		//	void   SetTimedMovementPath_TransportDropoffTime( float f ){ m_fTimedMovementPath_TransportDropoffTime = f; } // Used by transport and transportee
		//	float  GetTimedMovementPath_TransportDropoffTime(){ return m_fTimedMovementPath_TransportDropoffTime; } // Used by transport and transportee

			//
			void   SetRedMovementPath_ArrivalOffsetTimeAtPickupLocation( float f ){ m_fRedMovementPath_ArrivalOffsetTimeAtPickupLocation = f; } 
			float  GetRedMovementPath_ArrivalOffsetTimeAtPickupLocation();
			void   SetRedMovementPath_ArrivalOffsetTimeAtDropoffLocation( float f ){ m_fRedMovementPath_ArrivalOffsetTimeAtDropoffLocation = f; } 
			float  GetRedMovementPath_ArrivalOffsetTimeAtDropoffLocation();

			//float  GetGroundUnitArrivalTimeAtPickupLocation();
			//float  GetTransportArrivalTimeAtPickupLocation();
			//float  GetTransportArrivalTimeAtDropoffLocation();

			void   DeleteDirectTimedMovementPath();
			void   DeletePreTransportTimedMovementPath();
			void   DeletePostTransportTimedMovementPath();

			CList< CEOSAIUnitPathwayPredefinedStepPathStep* >*  GetDirectRedMovementPath(){ return &m_DirectRedMovementPath; }
			CList< CEOSAIUnitPathwayPredefinedStepPathStep* >*  GetPreTransportRedMovementPath(){ return &m_PreTransportRedMovementPath; }
			CList< CEOSAIUnitPathwayPredefinedStepPathStep* >*  GetPostTransportRedMovementPath(){ return &m_PostTransportRedMovementPath; }

			//void UpdateRedMovementPathTimingIfNecessary(); // Update the timing

			//void UpdateRedMovementPathIfNecessary(); // Update the path
			void UpdateRedMovementPath(); // Update the path
			void   UpdateDirectRedMovementPath();
			void   UpdatePreTransportRedMovementPath();
			void   UpdatePostTransportRedMovementPath();
			//void CalculateDirectRedPathTiming();
			//void CalculatePreTransportRedPathTiming();
			//void CalculatePostTransportRedPathTiming();
			//
			float GetRedPathEndTime( EnumEndType eEndType );
			void  GetFullRedMovementPath( float fStartTime, CList< CEOSAIUnitPathwayPredefinedStepPathStep* >* pTimedMovementPath );

		// Allowed MovementPath
		//
			CList< CEOSAIRegion2* >*  GetAIRegionAllowedMovementPath(){ return &m_AIRegionAllowedMovementPath; }

		// INCOMPLETE:
		// Usually the same as m_EndLocation - but might be different if this is a pickup/dropoff task
		//   In the case of pickup/dropoff, this is the target location for the ground unit
		//
			CEOSAILocation  GetPredefinedEndLocation(){ return m_EndLocation; }

		// PathwayMap
		//
			CEOSAIRegionPathwayMap*  InvokeEndLocationToEverywhereDirectMap();
			CEOSAIRegionPathwayMap*  InvokeEndLocationToEverywhereDirectMapPlusCoasts();
			CEOSAIRegionPathwayMap*  InvokeEndLocationToEverywhereGroundUnitAndSeaTransportMap();
			CEOSAIRegionPathwayMap*  InvokeEndLocationToEverywhereByAirMap();

	private:

			CEOSAIUnitPathwayFinder*  m_pAIUnitPathwayFinder;
			CEOSAIUnitActionIdea*     m_pAIUnitActionIdea;  // used for debugging
			EnumTask   m_eTask;

		// Start Location
		//
			CEOSAILocation  m_StartLocation;  // based on previous PredefinedStep (important for detecting changes)
			//float      m_fStartTime;

		// End Target/Location
		//
			CEOSAIPoiObject*  m_pAITarget;
			// If this is a pickup/dropoff, it isn't the transport's final location
			//   This might even be completely invalid if this is a pickup/dropoff - the time/location
			//   depends heavily on the GroundUnit.  (If the GroundUnit has to wait around, it can be
			//   at any coastal location.)
			CEOSAILocation      m_EndLocation;

			//
			float m_fDirectPathDistance;
			float m_fCrowsFlightDistance;
			float m_fCrowsFlightEstimatedMovementTime;

			bool  m_bAvoidDangerousPathways;

			// Used only if [this] USES a Transport
			long  m_iImaginaryTransport_PickupGroundAIRegion;
			long  m_iImaginaryTransport_PickupWaterAIRegion;
			long  m_iImaginaryTransport_DropoffGroundAIRegion;
			long  m_iImaginaryTransport_DropoffWaterAIRegion;
			CEOSAIUnit*   m_pTransportToUse;

			// Used only if [this] is a Transport Action
			CEOSAIUnitPathwayPredefinedStep*  m_pTransporteeStep;
			long  m_iRealTransport_PickupGroundAIRegion;
			long  m_iRealTransport_PickupWaterAIRegion;
			//float m_fRealTransport_PickupTime;
			long  m_iRealTransport_DropoffGroundAIRegion;
			long  m_iRealTransport_DropoffWaterAIRegion;
			//float m_fRealTransport_DropoffTime;

			CEOSAIUnitPathwayPredefinedStep*  m_pTransportPickupStep;
			CEOSAIUnitPathwayPredefinedStep*  m_pTransportDropoffStep;

		// MovementPath
		//
			CList< CEOSAIPathfinderPoint* >  m_DirectMovementPath;  // If using a direct path
			CList< CEOSAIPathfinderPoint* >  m_PreTransportMovementPath; // If using a transport
			CList< CEOSAIPathfinderPoint* >  m_PostTransportMovementPath;

			bool   m_bDirectRedMovementPathIsValid;
			bool   m_bPreTransportRedMovementPathIsValid;
			bool   m_bPostTransportRedMovementPathIsValid;

			float  m_fDirectMovementStepTime;
			float  m_fPreTransportMovementStepTime;
			//float  m_fPreTransportMovementStepEndTime;
			float  m_fPostTransportMovementStepTime;
			//float  m_fPostTransportMovementStepEndTime;

			//float  GetStepStartTime();
			//float  m_fStepTime;
			//float  m_fStepEndTime;

			// Used for timing, danger calculations.  It includes the transport's path, too
			float  m_fRedMovementPath_ArrivalOffsetTimeAtPickupLocation;  // Used by transport and transportee
			float  m_fRedMovementPath_ArrivalOffsetTimeAtDropoffLocation; // Used by transport and transportee
			CList< CEOSAIUnitPathwayPredefinedStepPathStep* >  m_DirectRedMovementPath;
			CList< CEOSAIUnitPathwayPredefinedStepPathStep* >  m_PreTransportRedMovementPath;
			CList< CEOSAIUnitPathwayPredefinedStepPathStep* >  m_PostTransportRedMovementPath;

			CList< CEOSAIRegion2* >  m_AIRegionAllowedMovementPath; // Used for allowed pathway

		// Pathway data (this could go into the ResultStep, but it's used by several ResultSteps)
		//
			void  ClearCachedMapData();

			CEOSAIRegionPathwayMap  m_EndLocationToEverywhereDirectMap;
			CEOSAIRegionPathwayMap  m_EndLocationToEverywhereDirectMapPlusCoasts;
			CEOSAIRegionPathwayMap  m_EndLocationToEverywhereGroundUnitAndSeaTransportMap;
			CEOSAIRegionPathwayMap  m_EndLocationToEverywhereByAirMap;

			// AirTransport uses the DirectMap along with some AIAirUnitPathwayFinder information

};

