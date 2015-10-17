
#pragma once

#ifdef _USRDLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

#include "EOSAIPoiMobile.h"
#include "EOSAIUnitTemplate.h"
#include "EOSAIStringAndFloatSet.h"
#include "EOSAIRegionPathfinder.h"
#include "EOSAIRegionPathwayMap.h"
#include "EOSAIUnit2CombatCapability.h"
class CEOSAITransportAssistedPath;
namespace EOSAI
{
	class UnitTask;
}
class CEOSAICity;
class CAITacticalProject;
class CEOSAIUnit2ActionIdea;
class CEOSAITaskForce3;
class CEOSAIUnit2PathwayFinder;
class CEOSAIAirUnitPathwayFinder;
class CEOSAISeaUnitPathwayFinder;
class CAIGroundUnitPathwayFinder;
class CEOSAIUnit2PathwayPredefinedStep;
class CEOSAITacticalProject2;

// Used for hypothetical (BestCase) allocations
//   This data structure exists because a BestCase TaskForce is allowed to allocate transports
//   It can allocate space in each transport (but not overfill it), and it ignores allocations by 
//   other TaskForces.  This class allows me to track allocations for each task force separately.
class CEOSAITransportSpaceAllocation 
{
	public:
		CEOSAITransportSpaceAllocation()
		{
			m_pAIUnit = NULL;
			m_pAITaskForce = NULL;
			m_bPreAllocatedSpaceHasBeenCalculated = false;
			m_iPreAllocatedSpace = 0;
			m_iAllocatedSpace = 0;
		}
		CEOSAITransportSpaceAllocation( CEOSAIUnit2* pAIUnit, CEOSAITaskForce3* p )
		{
			m_pAIUnit = pAIUnit;
			m_pAITaskForce = p;
			m_bPreAllocatedSpaceHasBeenCalculated = false;
			m_iPreAllocatedSpace = 0;
			m_iAllocatedSpace = 0;
			SetAIUnit( pAIUnit );
		}

		void  SetAIUnit( CEOSAIUnit2* pAIUnit );
		void  Clear();

		void  CalculatePreAllocatedSpace();

		bool  CanInsert( CEOSAIUnit2ActionIdea* pUnitActionIdea );
		bool  HasAllocatedSpace( CEOSAIUnit2ActionIdea* pUnitActionIdea );
		void  AllocateTransportSpace( CEOSAIUnit2ActionIdea* pUnitActionIdea );
		void  DeAllocateTransportSpace( CEOSAIUnit2ActionIdea* pUnitActionIdea );

		long  GetAllocatedAndPreAllocatedSpace(){ return m_iPreAllocatedSpace + m_iAllocatedSpace; }

	private:
		CEOSAIUnit2*        m_pAIUnit;
	public:
		CEOSAITaskForce3*  m_pAITaskForce; // if this is NULL, this is the "Real" list (not the hypothetical)
		bool            m_bPreAllocatedSpaceHasBeenCalculated;
		long            m_iPreAllocatedSpace;
		long            m_iAllocatedSpace;
		CList< CEOSAIUnit2ActionIdea* >  m_AllocatedUnitActionIdeaSpace;
};

class DLLIMPEXP CEOSAIUnit2 : public EOSAI::PoiMobile
{
	public:
		CEOSAIUnit2();
		virtual ~CEOSAIUnit2();

		//virtual void  SetServerPoiObject( CPoiObject* pPoiObject );
		void  CreateHypotheticalUnit( CEOSAIUnitTemplate* pUnitTemplate, long iOwner, CEOSAILocation Location, float fTime );
		void  CreateHypotheticalUnit( CEOSAICity* pBuiltByCity, CEOSAIUnitTemplate* pUnitTemplate, float fTimeToBuild );

		//CUnit*              GetServerUnit(){ return m_pServerUnit; }
		//virtual CPoiObject* GetServerPoiObject();
		//virtual long        GetObjectId();

		CEOSAICity*  GetBuiltByCity(){ return m_pBuiltByCity; } // sometimes valid
		CString   GetCombatUnitTypeString();
		long      GetCombatUnitTypeLong(){ return m_pAIUnitTemplate->GetCombatUnitTypeLong(); }
		long      GetCombatUnitTypeWhenLandedOrDocked(){ return m_pAIUnitTemplate->GetCombatUnitTypeWhenLandedOrDocked(); }

		CEOSAIUnit2CombatCapability*  GetAIUnitCombatCapability(){ ASSERT( false ); return NULL; }// &m_AIUnitCombatCapability; }

		// Unit-level Desires
		//
			void  CreatePersonalDesires(); // Repair, Upgrade, Combat
			bool  CanBeRepairedHere();
			bool  CanBeRepairedHere( CEOSAIPoiObject* p );
			float GetNeedForRepair01(){ return m_fNeedForRepair01; } // 0.0 if the unit is not damaged, 1.0 if severely damaged
			float GetNeedForRepair01BasedOnDistanceAndDamage(){ return m_fNeedForRepair01BasedOnDistanceAndDamage; } // 0.0 if the unit is not damaged, 1.0 if severely damaged

		// Upgrade
		//
			bool  GetNeedsToBeUpgraded();//{ ASSERT( false ); return false; }
			bool  CanBeUpgradedHere(){ ASSERT( false ); return false; }
			bool  CanBeUpgradedHere( CEOSAIPoiObject* p ){ ASSERT( false ); return false; }
			float GetDesireForUpgrade01(){ return m_fDesireForUpgrade01; } // 0.0 if no upgrade needed
			float GetUseInCombat01(){ return m_fUseInCombat01; } // 0.0 for transports, 0.0 for severely damaged combat units

		// Potential Targets (used by the AI to assign targets to aircraft; tactical bombers)
		//
			void  ClearPotentialTargets();
			void  AddPotentialTarget( CEOSAIUnit2* pAIUnit );
			bool  HasPotentialTargets();
			CEOSAIUnit2* GetRandomPotentialTarget();

		//
			//float GetCurrentHP01(){ ASSERT( false ); return 1.0f; }
			float GetCurrentHP01();

			//CEOSAIPoiObject* GetContainerAIUnit(){ return m_InitialState.GetContainer(); }

		// Information
		virtual bool  IsGroundUnit();
		bool          IsCityHunter();
		virtual bool  IsAirUnit();
		virtual bool  IsSeaUnit();
		bool          IsSubmarine();
		bool          IsSeaResHunter();
		bool          IsTransport();
		bool          IsAirUnitCarrier();

		//
		//void             SetUnitTemplate( CEOSAIUnitTemplate* pAIUnitTemplate ){ m_pAIUnitTemplate = pAIUnitTemplate; }
		//CEOSAIUnitTemplate* GetUnitTemplate(){ return m_pAIUnitTemplate; }

		//GetAIUnitTemplate
		void                 SetAIUnitTemplate( CEOSAIUnitTemplate* pAIUnitTemplate ){ m_pAIUnitTemplate = pAIUnitTemplate; }
		void                 SetAIUnitTemplateByInternalName( CString strInternalName );
		CEOSAIUnitTemplate*  GetAIUnitTemplate(){ return m_pAIUnitTemplate; }

		float  GetProductionCost(){ return (float) m_pAIUnitTemplate->GetProductionCost(); }

		float  GetAICombatSignificance(){ return m_pAIUnitTemplate->GetAICombatSignificance(); }

		//
		//void   SetCurrentHP( int iHP ){ m_iCurrentHP = iHP; GetInitialState()->SetHP( (float)iHP ); }
		//long   GetCurrentHP() { return m_iCurrentHP; }
		long   GetCurrentHP() { return (long) GetInitialState()->GetHP(); }

		void   SetCurrentTerrain( long i ){ m_iCurrentTerrain = i; }
		void   IsEntrenched( bool b ){ m_bIsEntrenched = b; }
		void   IsInsideFortification( bool b ){ m_bIsInsideFortification = b; }

		long   GetNumberOfAvailableUpgrades(); // Number of upgrades available which the unit hasn't taken advantage of

		// Resources
		//
			virtual void GetResourcesProducedPerTurn(std::map<CString, float>& ResourcesProducedAssumingNoOrders, std::map<CString, float>& ResourcesProducedByCurrentOrders);
			virtual void GetResourcesConsumedPerTurn(std::map<CString, float>& ResourcesConsumedAssumingNoOrders, std::map<CString, float>& ResourcesConsumedByCurrentOrders);

		// Container
		//
			CEOSAIUnit2* GetCurrentAIUnitContainer();

			bool         IsInsideTransport();
			CEOSAIUnit2* GetCurrentAITransport();

			virtual bool CanContainUnits();

			float        GetAIGroundTransportCapability();
			virtual long GetNumberOfGroundUnitsThatICanContain();
			// Carrier
			virtual long GetNumberOfAirUnitsThatICanContain();
			virtual bool CanLaunchAirUnits(){ return m_pAIUnitTemplate->CanLaunchAirUnits(); }

			virtual long GetMySize();

			long  GetContainerSize();
			bool  HasAllocatedSpace( CEOSAITaskForce3* pAITaskForce, CEOSAIUnit2ActionIdea* pUnitActionIdea );
			void  AllocateTransportSpace( CEOSAITaskForce3* pAITaskForce, CEOSAIUnit2ActionIdea* pIdea );
			void  DeAllocateTransportSpace( CEOSAITaskForce3* pAITaskForce, CEOSAIUnit2ActionIdea* pIdea );
			bool  CanInsertTransportIdea( CEOSAITaskForce3* pAITaskForce, CEOSAIUnit2ActionIdea* pIdea );

		// Containment
		//
			virtual long GetContainerSizeAlreadyUsed();
			//bool  IsInsideAUnitContainer(){ ASSERT( false ); return false; }
			bool  IsInsideAUnitContainer();//{ ASSERT( false ); return false; }

			//virtual bool CanContain_IgnoreForeignRelations( CEOSAIPoiObject* pAIPoiObject );
			virtual bool CanContain_IgnoreForeignRelations( CEOSAIUnitTemplate* pUnitTemplate );
			//virtual long GetNumberOfGroundUnitsThatICanContain();
			//virtual long GetNumberOfAirUnitsThatICanContain();

		//
		virtual bool CanCaptureCities();
		virtual bool CanCaptureResources();

		// Movement
		//
			virtual float             GetMovementRate();
			virtual float             GetMovementLimit();
			virtual float             GetMovementRangeForAttack();
			virtual EOSAIEnumMovementType3 GetMovementType();
			virtual void              SetMovementConsumed( float f ){ m_fMovementConsumed = f; }
			virtual float             GetMovementConsumed(){ return m_fMovementConsumed; }
			virtual bool              HasARange();

		// Attack Ranges
		//
			virtual float GetMaxAttackRange();//{ return m_pAIUnitTemplate->GetMaxAttackRange(); } // How far away the unit can attack without moving
			//virtual float GetMovementRangeForAttack(){ return m_pAIUnitTemplate->GetMovementRangeForAttack(); } // Flight distance for air units (aircraft attack range is how far it can fly and return home; missiles is the one-way distance)

		// View Ranges
		//
			virtual float GetDefaultViewRange(){ return m_pAIUnitTemplate->GetDefaultViewRange(); } // How far this unit can see

		// Combat
		//
			virtual float GetAttackRange( CEOSAIPoiObject* pTarget, bool bIsLandedOrDocked );
			virtual CEOSAIAttackVs2* GetAttackVs( CEOSAIPoiObject* pPoiObject, bool bIsLandedOrDocked = false ){ return CEOSAIPoiObject::GetAttackVs( pPoiObject, bIsLandedOrDocked ); }
			//virtual CEOSAIAttackVs2* GetAttackVs( CPoi* pPoi ){ return m_pAIUnitTemplate->GetAttackVs( pPoi ); }
			virtual CEOSAIAttackVs2* GetAttackVs( long iCombatUnitType ){ return m_pAIUnitTemplate->GetAttackVsCombatUnitType( iCombatUnitType ); }
			//virtual float      GetAttackRangeOnCitResFort(){ return m_pAIUnitTemplate->GetAttackRangeOnCitResFort(); }
			//virtual CEOSAIAttackVs2* GetAttackVsCitResFort(){ return m_pAIUnitTemplate->GetAttackVsCitResFort(); }

			float GetHPDefenseValue();
			EOSAI::StringAndFloatSet  GetCombatPowerVsUnitTypes();

			virtual void  AddToCombatGroup( CList< CEOSAIPoiObject* >* pCombatGroupList );

		// Pathfinding
		//
			virtual void        InvokeMultiRegionPathwaysToEverywhere();
			// "Direct" means it uses no transports (land units moving only on land)
			CEOSAIRegionPathwayMap*  InvokeDirectAIRegionMapToEverywhere();
			CEOSAIRegionPathwayMap*  InvokeDirectAIRegionMapToCoasts();

			CEOSAIRegionPathwayMap*  InvokeDirectMultiRegionMapToEverywhere();
			CEOSAIRegionPathwayMap*  InvokeDirectMultiRegionMapToCoasts();
			CEOSAIRegionPathwayMap*  InvokeLandAndImaginarySeaTransportMultiRegionMapToEverywhere();

			// return Time-To-Target
			float GetTransportCombinedAssistedMovementTime( CEOSAILocation TargetLocation );
			//
			float GetTransportAssistedAIRegionMovementPath( CEOSAIUnit2ActionIdea* pAITransportActionIdea, CEOSAILocation TargetLocation, CEOSAITransportAssistedPath* pPathResults = NULL );
			float GetTransportAssistedAIRegionMovementPath( CEOSAIUnit2* pTransport, CEOSAILocation TargetLocation, CEOSAITransportAssistedPath* pPathResults = NULL );
			float GetTransportAssistedAIRegionMovementPath( CEOSAICity* pCity, CEOSAIUnitTemplate* pTransportUnitTemplate, float fTransportBuildTime, CEOSAILocation TargetLocation, CEOSAITransportAssistedPath* pPathResults = NULL );
			float GetTransportAssistedAIRegionMovementPath( CEOSAIRegionPathwayMap* pTransportAIRegionMovementMap, CEOSAIUnitTemplate* pTransportUnitTemplate, float fTransportDelayTime, CEOSAILocation TargetLocation, CEOSAITransportAssistedPath* pPathResults = NULL );

			float GetTransportAssistedMultiRegionMovementPath( CEOSAIUnit2ActionIdea* pAITransportActionIdea, CEOSAILocation TargetLocation, CEOSAITransportAssistedPath* pPathResults = NULL );
			float GetTransportAssistedMultiRegionMovementPath( CEOSAIUnit2* pTransport, CEOSAILocation TargetLocation, CEOSAITransportAssistedPath* pPathResults = NULL );
			float GetTransportAssistedMultiRegionMovementPath( CEOSAICity* pCity, CEOSAIUnitTemplate* pTransportUnitTemplate, float fTransportBuildTime, CEOSAILocation TargetLocation, CEOSAITransportAssistedPath* pPathResults = NULL );
			float GetTransportAssistedMultiRegionMovementPath( CEOSAIRegionPathwayMap* pTransportMultiRegionMovementMap, CEOSAIUnitTemplate* pTransportUnitTemplate, float fTransportDelayTime, CEOSAILocation TargetLocation, CEOSAITransportAssistedPath* pPathResults = NULL );

		// Path
 		//
			//void  CalculateTimingDataFromPredefinedSteps();
			bool  HasPredefinedPath();
			void  CalculateAIRegionPathFromPredefinedSteps();
			void  UpdateRedPath();
			void  ConstructFullRedPath();
			float CalculateRedPathArrivalTimeAtGeo( long iTargetGeo );
			float     GetAttackTimeOfTacticalProject( CEOSAITacticalProject2* pTacticalProject );
			CEOSAIUnit2*  GetTransportUsedInTacticalProject( CEOSAITacticalProject2* pTacticalProject );
			void  SetInitialWaitTime( float fWaitTime ){ m_fInitialWaitTime = fWaitTime; }
			float GetInitialWaitTime(){ return m_fInitialWaitTime; }
			//CEOSAIUnit2PathwayPredefinedStep*  GetPickupStep( CEOSAIUnit2PathwayPredefinedStep* pGroundUnitStep );
			//CEOSAIUnit2PathwayPredefinedStep*  GetDropoffStep( CEOSAIUnit2PathwayPredefinedStep* pGroundUnitStep );

		//
		//
			void CalculateUnitPathway();

			CEOSAIUnit2PathwayFinder*        GetAIUnitPathwayFinder(){ return m_pAIUnitPathway; }
			CEOSAIAirUnitPathwayFinder*     GetAIAirUnitPathwayFinder();
			CEOSAISeaUnitPathwayFinder*     GetAISeaUnitPathwayFinder();
			CAIGroundUnitPathwayFinder*  GetAIGroundUnitPathwayFinder();

			virtual void  AddOnTheWayAndWaitTasksAsPredefinedSteps(); // over-ridden from AIPoiObject

			// If [this] is a Transport
			//void AddTransportAction( CEOSAIUnit2PathwayPredefinedStep* pTransporteeStep ); // Support this step with a transport action
			//void AddTransportAction( CEOSAIUnit2PathwayPredefinedStep* pTransporteeStep ); // Support this step with a transport action

		//
			//void AllocateMinorTaskToThisIdleUnit();
			void AllocateRepairNearbyTaskOrWarzoneToThisIdleUnit();

		void DeleteTransportSpaceAllocations();
		CEOSAITransportSpaceAllocation*  GetTransportSpaceAllocationObj( CEOSAITaskForce3* pAITaskForce );
		CEOSAITransportSpaceAllocation*  GetRealTransportSpaceAllocations(){ return &m_Real_TransportSpaceAllocations; }

		// Resource Consumption
		//
			float GetProductionAndConsumptionPerTurnAndMove( CString strResource );
			float GetProductionAndConsumptionPerTurn( CString strResource );
			float GetProductionAndConsumptionPerMove( CString strResource );

		// Combat
		//
			virtual float GetAIPowerValue(){ return GetAICombatSignificance(); } // used in pathfinding (to avoid enemy power)

			//float GetCurrentDefense()
			virtual float GetCurrentDefense();
			virtual float GetDefenseValueForNormalTerrain();//{ ASSERT( false ); return 0.0f; }

		//
			//virtual bool IsLandedOrDocked(){ return m_bIsLandedOrDocked; }
			virtual bool IsLandedOrDocked();

	private:

		CEOSAITransportSpaceAllocation*  InvokeTransportSpaceAllocationObj( CEOSAITaskForce3* pAITaskForce );

		CEOSAICity*        m_pBuiltByCity;  // sometimes valid
		//CUnit*          m_pServerUnit;   // sometimes valid
		CEOSAIUnitTemplate*  m_pAIUnitTemplate; // always valid

		CList< CEOSAIUnit2* >  m_PotentialTargets; // not owned

		CEOSAIUnit2CombatCapability  m_AIUnitCombatCapability;

		// Unit-level Desires
		//
			// Repairs
			float m_fNeedForRepair01;
			float m_fNeedForRepair01BasedOnDistanceAndDamage; // Based on damage and distance from repair
			// Upgrades
			//long  m_iAvailableUpgradeCount;
			float m_fDesireForUpgrade01; // 0.0 if no upgrade needed
			float m_fDesireForUpgrade01BasedOnDistanceAndDamage;
			// Combat
			float m_fUseInCombat01; // 0.0 for transports, 0.0 for severely damaged combat units

		//
		//long   m_iCurrentHP;
		float  m_fMovementConsumed;

		long   m_iCurrentTerrain;
		bool   m_bIsEntrenched;
		bool   m_bIsInsideFortification;
		//bool   m_bIsLandedOrDocked;

		// Used if [this] unit is a transport?
		CList< CEOSAITransportSpaceAllocation* >  m_BestCase_TransportSpaceAllocations;
		CEOSAITransportSpaceAllocation            m_Real_TransportSpaceAllocations;

	//	CEOSAIRegionPathwayMap  m_AIRegionMapDirectToEverywhere;
		CEOSAIRegionPathwayMap  m_AIRegionMapDirectToCoasts;
		CEOSAIRegionPathwayMap  m_LandAndImaginarySeaTransportAIRegionMapToEverywhere;

		CEOSAIRegionPathwayMap  m_MultiRegionMapDirectToEverywhere;
		CEOSAIRegionPathwayMap  m_MultiRegionMapDirectToCoasts;
		CEOSAIRegionPathwayMap  m_LandAndImaginarySeaTransportMultiRegionMapToEverywhere;

		CEOSAIUnit2PathwayFinder*  m_pAIUnitPathway;

		float m_fInitialWaitTime;
};
