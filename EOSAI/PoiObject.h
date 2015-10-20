
#pragma once

#ifdef CREATE_EOSAI_DLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

namespace EOSAI
{
	class CAIPlayer2;
	class UnitTask;
}
class CEOSAIBrain;
//class CWorldDesc;
//class CCity;
//class CResourceSource;
class CAIProductionSymbol;
class CEOSAIBuildOption;
class CEOSAIUnit2;
//class CAISymbolProcessor;
class CEOSAIUnitTemplate;
class CAIThoughtTask;
class CEOSAIAttackVs2;
//#include "EOSAIObject.h"
#include "EOSAILocation.h"
#include "EOSAIListUnowned.h"
#include "PoiObjectState.h"
#include "EOSAIRegionPathway.h"
class CEOSAIUnit2ActionIdea;
#include "EOSAIListSort.h"
#include "EOSAIEnumForeignRelations.h"
#include "EOSAIEnumPoiObjectType.h"
#include "EOSAIObjectActionProgression.h"
class CAITacticalProject;
class CAIPoiMobile;
class CEOSAIGeo;
class CEOSAIUnitSummary;
class CEOSAIAction;
class CEOSAITask;
class CEOSAIDesireSpatial;
enum EOSAIEnumAIDesireType;
//class CEOSAIUnit2ActionIdea2;

enum EOSAIEnumBorderViolation
{
	EOSAIEnumBorderViolation_None,
	EOSAIEnumBorderViolation_JustEnteredBorders,
	EOSAIEnumBorderViolation_IsLeaving,
	EOSAIEnumBorderViolation_Stationary,
	EOSAIEnumBorderViolation_IsGoingDeeper,
};

class DLLIMPEXP CEOSAIPoiObject //: public CAIObject
{
	public:
		CEOSAIPoiObject();// CEOSAIBrain* pAIBrain );
		virtual ~CEOSAIPoiObject();

		// Initialization
		//
			void  SetObjectId( long iPoiObjectId ){ m_iPoiObjectId = iPoiObjectId; }
			long  GetObjectId(){ return m_iPoiObjectId; }

			//virtual void  SetServerPoiObject( CPoiObject* pPoiObject );
			//EOSAI::AIPlayer*   GetAIPlayer(){ return m_pAIPlayer; }
			//CEOSAIBrain*     GetAIBrain(){ return m_pAIBrain; }

			void                SetAIPoiObjectType( EnumAIPoiObjectType e ){ m_ePoiObjectType = e; }
			EnumAIPoiObjectType GetAIPoiObjectType(){ return m_ePoiObjectType; }
			bool  IsCityResourceAirfieldOrUnit(){ return m_ePoiObjectType == EnumAIPoiObjectType::enum_City || m_ePoiObjectType == EnumAIPoiObjectType::enum_Resource || m_ePoiObjectType == EnumAIPoiObjectType::enum_Airfield || m_ePoiObjectType == EnumAIPoiObjectType::enum_Unit; }
			bool  IsCityResourceAirfieldFortOrUnit(){ return m_ePoiObjectType == EnumAIPoiObjectType::enum_City || m_ePoiObjectType == EnumAIPoiObjectType::enum_Resource || m_ePoiObjectType == EnumAIPoiObjectType::enum_Airfield || m_ePoiObjectType == EnumAIPoiObjectType::enum_Fortification || m_ePoiObjectType == EnumAIPoiObjectType::enum_Unit; }

			void  IsHypotheticalPoiObject( bool b ){ m_bHypotheticalPoiObject = b; }
			bool  IsHypotheticalPoiObject(){ return m_bHypotheticalPoiObject; }
			bool  IsDead(){ return false; }

			//long       GetGeoId(){ ASSERT( false ); return 0; }
			long       GetGeoId();
			CEOSAILocation  GetLocation(){ return this->GetInitialState()->GetLocation(); }
			//CEOSAILocation  GetLocation(){ return m_Location; }
			void            CopyCurrentLocationToLastTurnLocation() { m_LastTurnLocation = GetLocation(); }
			CEOSAILocation  GetLastTurnLocation() { return m_LastTurnLocation; }

			void     SetName( CString str ){ m_strName = str; }
			CString  GetName(){ return m_strName; }
			bool     IsAlive(){ return m_bIsAlive; }

			virtual bool  IsGroundUnit(){ return false; }
			virtual bool  IsAirUnit(){ return false; }
			virtual bool  IsSeaUnit(){ return false; }

			CEOSAIPoiObject*  GetAirBase(){ return m_InitialState.GetAirbase(); }
			CEOSAIPoiObject*  GetContainer(){ return m_InitialState.GetContainer(); }

		//CEOSAIObjectActionProgression2*  GetAIObjectActionProgression(){ return &m_AIObjectActionProgression; }

		EOSAI::PoiObjectState*  GetInitialState(){ return &m_InitialState; }
		EOSAI::PoiObjectState*  GetFinalState(){ return &m_InitialState; } // THINGS_TO_COMPILE_EVENTUALLY (this should be the final state). I'm doing a hack right now
		//EOSAI::PoiObjectState*  GetFinalState(){ return m_AIObjectActionProgression.GetFinalState(); }

		// Containment
		//
			virtual bool CanContain_IgnoreForeignRelations( CEOSAIPoiObject* pAIPoiObject ){ ASSERT( false ); return false; }
			virtual bool CanContain_IgnoreForeignRelations( CEOSAIUnitTemplate* pAIUnitTemplate ){ ASSERT( false ); return false; }

			//EOSAIEnumForeignRelations  GetForeignRelationsTo( long iOtherPlayer ){ ASSERT( false ); return EOSAIEnumForeignRelations::enum_Undefined; }
			EOSAIEnumForeignRelations  GetForeignRelationsTo( long iOtherPlayer );//{ ASSERT( false ); return EOSAIEnumForeignRelations::enum_Undefined; }

			void SetInitialContainer( CEOSAIPoiObject* pNewContainer );
			void sSetInitialContainee( EOSAI::PoiMobile* pNewContainee );

			void IsLandingPadForAirUnits( bool b ){ m_bIsLandingPadForAirUnits = b; }
			bool IsLandingPadForAirUnits(){ return m_bIsLandingPadForAirUnits; }
			void IsLaunchingPadForAirUnits( bool b ){ m_bIsLaunchingPadForAirUnits = b; }
			bool IsLaunchingPadForAirUnits(){ return m_bIsLaunchingPadForAirUnits; }

		// Resources
		//
			virtual void GetResourcesProducedPerTurn(std::map<CString, float>& ResourcesProducedAssumingNoOrders, std::map<CString, float>& ResourcesProducedByCurrentOrders){}
			virtual void GetResourcesConsumedPerTurn(std::map<CString, float>& ResourcesConsumedAssumingNoOrders, std::map<CString, float>& ResourcesConsumedByCurrentOrders){}

		// Repair
		//
			void  SetGroundUnitRepairRate( float f ){ m_fGroundUnitRepairRate = f; }
			float GetGroundUnitRepairRate(){ return m_fGroundUnitRepairRate; }
			void  SetAirUnitRepairRate( float f ){ m_fAirUnitRepairRate = f; }
			float GetAirUnitRepairRate(){ return m_fAirUnitRepairRate; }
			void  SetSeaUnitRepairRate( float f ){ m_fSeaUnitRepairRate = f; }
			float GetSeaUnitRepairRate(){ return m_fSeaUnitRepairRate; }

		//bool IsInsideATransport();
		//bool IAmContainedInside( CEOSAIPoiObject* pAIPoiObject );

		//void SetLocationOwner( char iOwner, char iOwnershipLevel ){ m_iLocationOwner = iOwner; m_iLocationOwnershipLevel = iOwnershipLevel; }
		//void SetLastLocationOwner( char iOwner, char iOwnershipLevel ){ m_iLastLocationOwner = iOwner; m_iLastLocationOwnershipLevel = iOwnershipLevel; }
		void SetBorderViolationState( EOSAIEnumBorderViolation e ){ m_eBorderViolation = e; }
		EOSAIEnumBorderViolation GetBorderViolationState(){ return m_eBorderViolation; }

		// This unit plus any units inside it
		void AddMeAndChildrenToList( CList< CEOSAIPoiObject* >* pAIPoiObjects );
		void AddMeAndChildrenToUnitSummary( CEOSAIUnitSummary* pUnitSummary );
		//void AddMeToUnitSummary( CEOSAIUnitSummary* pUnitSummary );

		virtual void  AddToCombatGroup( CList< CEOSAIPoiObject* >* pCombatGroupList );

		// Container/Containment
		//
			virtual long GetMySize(){ return 0; }

			//CList< CEOSAIUnit2* >*  GetUnitsInsideMe(){ return &m_UnitsInsideMe; }
			virtual long  GetContainerSizeAlreadyUsed(){ return 0; }
			CEOSAIPoiObject* GetContainerAIPoiObject(){ return m_InitialState.GetContainer(); }

			bool  CanContainGroundUnits(){ ASSERT( false ); return false; }

		// Desires
		//   (These are desires involving this AIPoiObject, like "capture" or "destroy")
			void               AddAIDesire( CEOSAIDesireSpatial* pDesire ){ m_AIDesires.AddTail( pDesire ); }
			void               ClearAIDesires(){ m_AIDesires.RemoveAll(); }
			CList< CEOSAIDesireSpatial* >* GetAIDesires(){ return &m_AIDesires; }
			CEOSAIDesireSpatial*  GetAIDesire( EOSAIEnumAIDesireType eType );

		// Allocated TacticalProject/UnitActionIdea
		//
			void                 SetAllocatedUnitActionIdea( CEOSAIUnit2ActionIdea* pUnitActionIdea );
			void                 ReleaseAllocatedUnitActionIdea( CEOSAIUnit2ActionIdea* pUnitActionIdea );
			void                 ReleaseAllocatedUnitActionIdea();
			CEOSAIUnit2ActionIdea*   GetAllocatedUnitActionIdea();//{ return m_pAllocatedUnitActionIdea; }
			bool                 IsAllocated();//{ return m_pAllocatedUnitActionIdea != NULL; }

		// Transports
		//
			float GetBestTransportArrivalTime_CrowsFlight( long iPlayer, CEOSAIUnitTemplate* pGroundUnit );

		// AIUnitActionIdeas
		//
			// SimpleScores
			//float GetBestUnitActionValueSimpleScore();
			// RelativeScores
			void  CreateRelativeActionScoresAccordingToAIPoiObject();

		//
			float GetHPRepairPerTurn( CEOSAIUnitTemplate* pUnitTemplate );

		// Predefined Path, Result Path, Orders
		//
			virtual void  AddOnTheWayAndWaitTasksAsPredefinedSteps(); // over-ridden by AIUnit

			//void  TurnAllocatedUnitActionIdeaIntoPredefinedStep();
			void  TurnPredefinedPathIntoResultPath();
			void  TurnResultPathIntoOrders();

		// Actions, Tasks, Orders
		//
			//void  TurnActionsIntoTasks();
			//bool  TurnTasksIntoOrders();

		// Pathfinding
		//
			virtual void InvokeMultiRegionPathwaysToEverywhere(){}

		// Attack Ranges
		//
			virtual float GetMaxAttackRange(){ return 0.0f; } // How far away the unit can attack without moving
			virtual float GetMovementRangeForAttack(){ return 0.0f; } // Flight distance for air units

		// View Ranges
		//
			virtual float GetDefaultViewRange(){ return 0.0f; } // How far this unit can see

		//
		//EOSAIEnumForeignRelations  GetForeignRelations();
		bool IsUnowned();
		bool IsTeammateTo( long iPlayer );
		bool IsAnAllyTo( long iPlayer );
		//bool IsMine();
		bool IsNeutralPlayerTo( long iPlayer );
		bool IsEnemyTo( long iPlayer );
		//bool WillBeMine(){ return m_bIPlanOnCapturingThis || IsMine(); }
		long GetOwner(){ return m_InitialState.GetOwner(); }

		bool IsOwnedByOrAlliedTo( long iPlayer );

		virtual bool IsMobile(){ return false; }
		virtual bool IsLandedOrDocked(){ return false; }

		// Combat
		//
			virtual float  GetAIPowerValue(){ return 0.0f; } // used in pathfinding (to avoid enemy power)

			virtual float GetAttackRange( CEOSAIPoiObject* pTarget, bool bIsLandedOrDocked ){ return NULL; }
			virtual CEOSAIAttackVs2* GetAttackVs( CEOSAIPoiObject* pPoiObject, bool bIsLandedOrDocked = false ){ return NULL; }
			virtual CEOSAIAttackVs2* GetAttackVs( long iCombatUnitType ){ return NULL; }
			//virtual float      GetAttackRangeOnCitResFort(){ return m_pAIUnitTemplate->GetAttackRangeOnCitResFort(); }
			//virtual CEOSAIAttackVs2* GetAttackVsCitResFort(){ return m_pAIUnitTemplate->GetAttackVsCitResFort(); }


	protected:

		//CPoiObject* m_pServerPoiObject;
		//EOSAI::AIPlayer* m_pAIPlayer;
		//CEOSAIBrain*   m_pAIBrain;

		bool        m_bHypotheticalPoiObject;
		bool        m_bIsAlive;

		long        m_iPoiObjectId;
		CString     m_strName;
		//CEOSAILocation   m_Location;
		//CEOSAILocation   m_LastTurnLocation;

		//bool        m_bIPlanOnCapturingThis;

		EnumAIPoiObjectType    m_ePoiObjectType;
		EOSAIEnumBorderViolation  m_eBorderViolation;

		bool m_bIsLandingPadForAirUnits;
		bool m_bIsLaunchingPadForAirUnits;

		float m_fGroundUnitRepairRate;
		float m_fAirUnitRepairRate;
		float m_fSeaUnitRepairRate;

	// AIAction demand level
	//
		// Used by AIUnits and AICities (when there is a hypothetical AIUnit)
		CEOSAIUnit2ActionIdea*  m_pAllocatedUnitActionIdea; // What am I going to do?

		//CList< CEOSAIUnit2* >   m_UnitsInsideMe;

		// AI support functions
		//   These are desires associated with this Poi (e.g. capture it, destroy it, ...)
		CList< CEOSAIDesireSpatial* >  m_AIDesires; // not owned (just cached for speed)

		CEOSAILocation                  m_LastTurnLocation;
		EOSAI::PoiObjectState           m_InitialState;
		//CEOSAIObjectActionProgression2  m_AIObjectActionProgression;
};
