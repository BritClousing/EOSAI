
#include "stdafx.h"
#include "EOSAIUnit2.h"
//#include "Unit.h"
#include "AIPlayer.h"
//#include "AISpatialMap.h"
//#include "AILogicalObject.h"
//#include "AILogicalAction.h"
#include "EOSAICommonData.h"
#include "EOSAIBrain.h"
#include "EOSAIUnitTask.h"
#include "City.h"
#include "PoiObject.h"
#include "EOSAIThoughtDatabase.h"
#include "EOSAIUnitTemplate.h"
#include "EOSAITacticalProject2.h"
//#include "EOSAIUnitAction.h"
#include "EOSAIUnitActionIdea.h"
#include "EOSAITransportAssistedPath.h"
#include "EOSAIUnitPathwayFinder.h"
#include "EOSAIAirUnitPathwayFinder.h"
#include "EOSAISeaUnitPathwayFinder.h"
#include "EOSAIGroundUnitPathwayFinder.h"
#include "EOSAIUnitPathwayResult.h"

#include "EOSAIDesireSpatial.h"
#include "EOSAIUnitPathwayPredefinedStep.h"
#include "EOSAITransportComboMap.h"
//#include "WorldDescServer.h"
#include "EOSAICommonData.h"
#include "EOSAIWarzoneLocation.h"
//#include "CommonState.h"
#include "EOSAIMultiRegionAttackAccess.h"
#include "EOSAIMultiRegionNationwidePathways.h"
#include "EOSAIPlayerManager.h"
//#include "GeoLand.h"
#include "EOSAISettings.h"
#include <math.h>

#include "EOSAILimitedList.h"

#include "EOSAIMain.h"
#include "EOSAIInterface.h"
extern EOSAI::CInterface* g_pEOSAIInterface;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void  CEOSAITransportSpaceAllocation::SetAIUnit( CEOSAIUnit* pAIUnit )
{
	m_pAIUnit = pAIUnit;
	CalculatePreAllocatedSpace();
}

void  CEOSAITransportSpaceAllocation::Clear()
{
	m_bPreAllocatedSpaceHasBeenCalculated = false;
	m_iPreAllocatedSpace = 0;
	m_iAllocatedSpace = 0;
	m_AllocatedUnitActionIdeaSpace.RemoveAll();

	CalculatePreAllocatedSpace();
}

void  CEOSAITransportSpaceAllocation::CalculatePreAllocatedSpace()
{
	if( m_bPreAllocatedSpaceHasBeenCalculated == false )
	{
		//ASSERT( m_pAIUnit );
		//if( m_pAIUnit->GetServerUnit() )
		if( m_pAIUnit )
		{
			m_bPreAllocatedSpaceHasBeenCalculated = true;
			m_iPreAllocatedSpace = m_pAIUnit->GetContainerSizeAlreadyUsed();
		}
	}
}

bool  CEOSAITransportSpaceAllocation::CanInsert( CEOSAIUnitActionIdea* pUnitActionIdea )
{
	//CalculatePreAllocatedSpace();
	//
	// Am I already inside the transport?
	if( pUnitActionIdea->GetAIUnitActor() &&
		pUnitActionIdea->GetAIUnitActor()->GetCurrentAIUnitContainer() == this->m_pAIUnit )
	{
		// Yes, I am already inside this container
		//   Note: this will allow multiple UnitActionIdeas from a single AIUnit
		return true;
	}
	else
	{
		long iNewUnitSize = pUnitActionIdea->GetAIUnitTemplate()->GetMySizeInContainer();
		long iContainerSize = m_pAIUnit->GetContainerSize();

		// No, I am not inside this container
		//long i1 = pMovementDesc->m_pAITransport->GetContainerSize();
		//long i2 = pMovementDesc->m_pAITransport->GetAllocatedContainerSpace( pAITaskForce );
		//long i3 = pMovementDesc->m_pAITransport->GetAvailableContainerSpace( pAITaskForce );
		if( m_iAllocatedSpace + m_iPreAllocatedSpace + iNewUnitSize <= iContainerSize )
		{
			return true;
		}
	}
	return false;
}

bool  CEOSAITransportSpaceAllocation::HasAllocatedSpace( CEOSAIUnitActionIdea* pUnitActionIdea )
{
	POSITION pos = m_AllocatedUnitActionIdeaSpace.GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitActionIdea* pUnitActionIdeaInList = m_AllocatedUnitActionIdeaSpace.GetNext( pos );
		if( pUnitActionIdea == pUnitActionIdeaInList ) return true;
	}
	return false;
}

void  CEOSAITransportSpaceAllocation::AllocateTransportSpace( CEOSAIUnitActionIdea* pUnitActionIdea )
{
	//CalculatePreAllocatedSpace();
	//
	ASSERT( HasAllocatedSpace( pUnitActionIdea ) == false );
	if( HasAllocatedSpace( pUnitActionIdea ) == false )
	{
		m_AllocatedUnitActionIdeaSpace.AddTail( pUnitActionIdea );

		// Am I already inside the transport?
		if( pUnitActionIdea->GetAIUnitActor() &&
			pUnitActionIdea->GetAIUnitActor()->GetCurrentAIUnitContainer() == this->m_pAIUnit )
		{
			// Yes, I am already inside this container
		}
		else
		{
			// No, I am not inside this container
			long iUnitSize = pUnitActionIdea->GetAIUnitTemplate()->GetMySizeInContainer();
			m_iAllocatedSpace += iUnitSize;
			ASSERT( m_iPreAllocatedSpace + m_iAllocatedSpace <= m_pAIUnit->GetContainerSize() );//pUnitActionIdea->GetUnitTemplate()->GetNumberOfUnitsThatICanContain() );
			ASSERT( m_iPreAllocatedSpace + m_iAllocatedSpace >= 0 );
		}
	}
	ASSERT( m_iPreAllocatedSpace + m_iAllocatedSpace <= m_pAIUnit->GetContainerSize() );//pUnitActionIdea->GetUnitTemplate()->GetNumberOfUnitsThatICanContain() );
	ASSERT( m_iPreAllocatedSpace + m_iAllocatedSpace >= 0 );
}

void  CEOSAITransportSpaceAllocation::DeAllocateTransportSpace( CEOSAIUnitActionIdea* pUnitActionIdea )
{
	//CalculatePreAllocatedSpace();
	//
	//bool bHasAllocatedSpace = HasAllocatedSpace( pUnitActionIdea );
	ASSERT( HasAllocatedSpace( pUnitActionIdea ) );
	if( HasAllocatedSpace( pUnitActionIdea ) )
	{
		POSITION pos = m_AllocatedUnitActionIdeaSpace.GetHeadPosition();
		while( pos )
		{
			POSITION PrevPos = pos;
			CEOSAIUnitActionIdea* pUnitActionIdeaInList = m_AllocatedUnitActionIdeaSpace.GetNext( pos );
			if( pUnitActionIdea == pUnitActionIdeaInList )
			{
				m_AllocatedUnitActionIdeaSpace.RemoveAt( PrevPos );
			}
		}

		if( pUnitActionIdea->GetAIUnitActor() &&
			pUnitActionIdea->GetAIUnitActor()->GetCurrentAIUnitContainer() == this->m_pAIUnit )
		{
			// Yes, I am already inside this container
		}
		else
		{
			// No, I am not inside this container
			long iUnitSize = pUnitActionIdea->GetAIUnitTemplate()->GetMySizeInContainer();
			m_iAllocatedSpace -= iUnitSize;
		}
	}

	ASSERT( m_iPreAllocatedSpace + m_iAllocatedSpace <= m_pAIUnit->GetContainerSize() );//pUnitActionIdea->GetUnitTemplate()->GetNumberOfUnitsThatICanContain() );
	ASSERT( m_iPreAllocatedSpace + m_iAllocatedSpace >= 0 );
}

///

CEOSAIUnit::CEOSAIUnit() : EOSAI::PoiMobile()
{
	m_pBuiltByCity = NULL;
	//m_pServerUnit = NULL;
	m_pAIUnitTemplate = NULL;

	// Desires
	//
		// Repair
		m_fNeedForRepair01 = 0.0f;  // 0.0 if the unit is not damaged, 1.0 if severely damaged
		m_fNeedForRepair01BasedOnDistanceAndDamage = 0.0f;
		// Upgrade
		m_fDesireForUpgrade01 = 0.0f; // 0.0 if no upgrade needed
		m_fDesireForUpgrade01BasedOnDistanceAndDamage = 0.0f;
		// Combat
		m_fUseInCombat01 = 1.0f;

	//m_iCurrentHP = 0;
	m_fMovementConsumed = 0.0f;
	m_iCurrentTerrain = -1;
	m_bIsEntrenched = false;
	m_bIsInsideFortification = false;
	//m_bIsLandedOrDocked = false;

	m_pAIUnitPathway = NULL;
	m_fInitialWaitTime = 0.0f;
}

CEOSAIUnit::~CEOSAIUnit()
{
	while( m_BestCase_TransportSpaceAllocations.IsEmpty() == FALSE )
	{
		delete m_BestCase_TransportSpaceAllocations.RemoveHead();
	}
	delete m_pAIUnitPathway; m_pAIUnitPathway = NULL;
}

void CEOSAIUnit::ResetAIPlayerData()
{
	CEOSAIPoiObject::ResetAIPlayerData();
	ResetAIPlayerData_AIUnit();
}

void CEOSAIUnit::ResetAIPlayerData_AIUnit()
{
	// We shouldn't be reseting hypothetical units, they should be deleted
	ASSERT(this->IsHypotheticalPoiObject() == false);

	this->ClearPotentialTargets();

	if (this->GetAIUnitPathwayFinder())
	{
		this->GetAIUnitPathwayFinder()->ClearPredefinedAndResultPaths();
		ASSERT(this->GetAIUnitPathwayFinder()->GetPreDefinedPath()->IsEmpty());
		ASSERT(this->GetAIUnitPathwayFinder()->GetResultPath() == NULL ||
			   this->GetAIUnitPathwayFinder()->GetResultPath()->m_Steps.IsEmpty());
	}
	this->DeleteTransportSpaceAllocations();

	if (this->GetAIUnitTemplate()) // Sometimes this function gets called 
	{
		this->CalculateUnitPathway();
	}
}

/*
// have to create m_pAIUnitPathway
void  CEOSAIUnit::SetServerPoiObject( CPoiObject* pPoiObject )
//void  CEOSAIUnit::SetStateCalculatePathways()
{
	ASSERT( m_pServerUnit == NULL && m_bHypotheticalPoiObject == false );
	ASSERT( pPoiObject->IsDead() == false );

	ASSERT( dynamic_cast< CUnit* >( pPoiObject ) );
	m_pServerUnit = dynamic_cast< CUnit* >( pPoiObject );
	EOSAI::PoiMobile::SetServerPoiObject( pPoiObject );

	//m_pUnit = pUnit;
	m_bHypotheticalPoiObject = false;

	CUnitTemplate* pUnitTemplate = m_pServerUnit->GetUnitTemplate();
	ASSERT( pUnitTemplate );
	SetUnitTemplate( pUnitTemplate );
	m_InitialState.SetHP( m_pServerUnit->GetCurrentHP() );
	m_InitialState.SetGroundUnit( pUnitTemplate->IsGroundUnit() );

	// HP01 = 1.0 -> m_fDesireForRepair01 = 0.0
	// HP01 = 0.8 -> m_fDesireForRepair01 = 0.25
	// HP01 = 0.6 -> m_fDesireForRepair01 = 0.5
	// HP01 = 0.4 -> m_fDesireForRepair01 = 0.75
	// HP01 = 0.2 -> m_fDesireForRepair01 = 1.0
	//m_fDesireForRepair01 = 1.25f * ( 1.0f - m_pServerUnit->GetCurrentHP01() );
	//if( m_fDesireForRepair01 > 1.0f ){ m_fDesireForRepair01 = 1.0f; }
	//CalculateDesires();

	//SetPoiMobile( m_pUnit );
	m_Real_TransportSpaceAllocations.SetAIUnit( this );
	m_pAIUnitPathway = CEOSAIUnitPathwayFinder::CreatePathwayFinder( this );
}
*/
void CEOSAIUnit::CreateHypotheticalUnit( CEOSAIUnitTemplate* pAIUnitTemplate, long iOwner, CEOSAILocation Location, float fTime )
{
	//ASSERT( m_pServerUnit == NULL && m_bHypotheticalPoiObject == false );
	ASSERT( false ); // hypothetical units exist only in the minds of the AI - I need an AIBrain pointer
	ASSERT( m_bHypotheticalPoiObject == false );

	//m_pServerUnit = NULL;
	m_bHypotheticalPoiObject = true;

	SetAIUnitTemplate( pAIUnitTemplate );
	m_InitialState.SetOwner( iOwner );
	m_InitialState.SetGroundUnit( pAIUnitTemplate->IsGroundUnit() );
	m_InitialState.SetLocationAndRegions( Location );
	m_InitialState.SetTime( fTime );
	m_InitialState.SetContainer( NULL );
	m_InitialState.SetHP( (float)pAIUnitTemplate->GetMaxHP() );
	//CAIObject::m_InitialState.SetOwner( iOwner );
	//CAIObject::m_InitialState.SetLocationAndGeo( Location );
	//CAIObject::m_InitialState.SetTime( fTime );
	//CAIObject::m_InitialState.SetContainer( NULL );
	m_pAIUnitPathway = CEOSAIUnitPathwayFinder::CreatePathwayFinder( this );
}

void CEOSAIUnit::CreateHypotheticalUnit( CEOSAICity* pBuiltByCity, CEOSAIUnitTemplate* pAIUnitTemplate, float fTimeToBuild )
{
	//ASSERT( false ); // hypothetical units exist only in the minds of the AI - I need an AIBrain pointer
	ASSERT( m_bHypotheticalPoiObject == false );

	m_pBuiltByCity = pBuiltByCity;
	//m_pServerUnit = NULL;
	m_bHypotheticalPoiObject = true;

	SetAIUnitTemplate( pAIUnitTemplate );
	m_InitialState.SetOwner( pBuiltByCity->GetOwner() );
	m_InitialState.SetGroundUnit( pAIUnitTemplate->IsGroundUnit() );
	m_InitialState.SetLocationAndRegions( pBuiltByCity->GetLocation() );
	m_InitialState.SetTime( fTimeToBuild );
	m_InitialState.SetContainer( NULL );
	m_InitialState.SetHP( (float)pAIUnitTemplate->GetMaxHP() );
	m_pAIUnitPathway = CEOSAIUnitPathwayFinder::CreatePathwayFinder( this );
}

//

CString  CEOSAIUnit::GetCombatUnitTypeString()
{
	return m_pAIUnitTemplate->GetCombatUnitTypeString();
}

//
void CEOSAIUnit::CreatePersonalDesires()
{
	// Calculate Desire for Repair
	//   This requires that all CAIPoiObjects are already created
	{
		m_fNeedForRepair01 = 0.0f;
		m_fNeedForRepair01BasedOnDistanceAndDamage = 0.0f;

		float fCurrentHP01 = this->GetCurrentHP01();
		CEOSAIMathFunction HPToRepairFunc;
		HPToRepairFunc.SetInputOutput( 0.0f,1.0f );
		HPToRepairFunc.SetInputOutput( 0.1f,1.0f );
		HPToRepairFunc.SetInputOutput( 0.9f,0.0f );
		HPToRepairFunc.SetInputOutput( 1.0f,0.0f );
		m_fNeedForRepair01 = HPToRepairFunc.GetOutput( fCurrentHP01 );

		// Find distance to a repair location
		if( m_fNeedForRepair01 > 0.0f )
		{
			CEOSAIPoiObject* pClosestRepairLocation = NULL;
			float fClosestRepairLocationDistance = 0.0f;
			//m_pAIBrain->GetAIThoughtDatabase()->GetClosestRepairLocation( this, &pClosestRepairLocation, &fClosestRepairLocationDistance );
			long iUnitOwner = GetOwner();
			if (iUnitOwner > 0)
			{
				g_pEOSAICommonData->GetAIThoughtDatabase(iUnitOwner)->GetClosestRepairLocation(this, &pClosestRepairLocation, &fClosestRepairLocationDistance);
				if (pClosestRepairLocation)
				{
					float fTimeToCity = fClosestRepairLocationDistance / this->GetAIUnitTemplate()->GetMovementRate();
					CEOSAIMathFunction TimeToCityFunc;
					TimeToCityFunc.SetInputOutput(0.0f, 4.0f);
					TimeToCityFunc.SetInputOutput(0.5f, 2.0f);
					TimeToCityFunc.SetInputOutput(2.0f, 1.0f);
					TimeToCityFunc.SetInputOutput(10.0f, 0.2f);
					float fTimeToCityMult = TimeToCityFunc.GetOutput(fTimeToCity);

					CEOSAIMathFunction CurrentHP01ToRepairDesire;
					CurrentHP01ToRepairDesire.SetInputOutput(0.0f, 5.0f);
					CurrentHP01ToRepairDesire.SetInputOutput(0.5f, 1.0f);
					CurrentHP01ToRepairDesire.SetInputOutput(0.7f, 0.4f);
					CurrentHP01ToRepairDesire.SetInputOutput(1.0f, 0.0f);
					float fCurrentHP01ToRepair = CurrentHP01ToRepairDesire.GetOutput(fCurrentHP01);

					m_fNeedForRepair01BasedOnDistanceAndDamage = min(1.0f, fCurrentHP01ToRepair * fTimeToCityMult);
				}
			}
		}
	}
	// Calculate desire for upgrade
	{
		m_fDesireForUpgrade01 = 0.0f;
		m_fDesireForUpgrade01BasedOnDistanceAndDamage = 0.0f;

		long iAvailableUpgradeCount = this->GetNumberOfAvailableUpgrades();
		if( iAvailableUpgradeCount == 0 ){ m_fDesireForUpgrade01 = 0.0f; }
		if( iAvailableUpgradeCount == 1 ){ m_fDesireForUpgrade01 = 0.7f; }
		if( iAvailableUpgradeCount == 2 ){ m_fDesireForUpgrade01 = 0.9f; }
		if( iAvailableUpgradeCount >= 3 ){ m_fDesireForUpgrade01 = 1.0f; }

		// Find distance to a upgrade location
		if( m_fDesireForUpgrade01 > 0.0f )
		{
			CEOSAIPoiObject* pClosestUpgradeLocation = NULL;
			float fClosestUpgradeLocationDistance = 0.0f;
			g_pEOSAICommonData->GetAIThoughtDatabase( GetOwner() )->GetClosestUpgradeLocation( this, &pClosestUpgradeLocation, &fClosestUpgradeLocationDistance );
			if( pClosestUpgradeLocation )
			{
				float fTimeToCity = fClosestUpgradeLocationDistance / GetAIUnitTemplate()->GetMovementRate();
				CEOSAIMathFunction TimeToCityFunc;
				TimeToCityFunc.SetInputOutput(  0.0f,2.0f );
				TimeToCityFunc.SetInputOutput(  0.5f,2.0f );
				TimeToCityFunc.SetInputOutput(  2.0f,1.0f );
				TimeToCityFunc.SetInputOutput( 10.0f,0.2f );
				float fTimeToCityMult = TimeToCityFunc.GetOutput( fTimeToCity );

				m_fDesireForUpgrade01BasedOnDistanceAndDamage = min( 1.0f, m_fDesireForUpgrade01 * fTimeToCityMult );
			}
		}
	}
	// Desire for combat
	{
		m_fUseInCombat01 = 1.0f;
		if( m_pAIUnitTemplate->IsACombatUnit() )
		{
			m_fUseInCombat01 = 1.0f - m_fNeedForRepair01 - 0.7f*m_fDesireForUpgrade01;
			ASSERT( m_fUseInCombat01 <= 1.0f );
			if( m_fUseInCombat01 > 1.0f ){ m_fUseInCombat01 = 1.0f; }
			if( m_fUseInCombat01 < 0.0f ){ m_fUseInCombat01 = 0.0f; }
		}
		else
		{
			m_fUseInCombat01 = 0.0f;
		}
	}
}

bool CEOSAIUnit::CanBeRepairedHere()
{
	CEOSAIPoiObject* pCurrentContainer = GetContainer();
	while( pCurrentContainer )
	{
		if( ( this->IsGroundUnit() && pCurrentContainer->GetGroundUnitRepairRate() > 0.0f ) ||
			( this->IsAirUnit()    && pCurrentContainer->GetAirUnitRepairRate() > 0.0f ) ||
			( this->IsSeaUnit()    && pCurrentContainer->GetSeaUnitRepairRate() > 0.0f ) )
		{
			return true;
		}
		pCurrentContainer = pCurrentContainer->GetContainer();
	}
	return false;
}

bool CEOSAIUnit::CanBeRepairedHere( CEOSAIPoiObject* pContainer )
{
	CEOSAIPoiObject* pCurrentContainer = pContainer;
	while( pCurrentContainer )
	{
		if( ( this->IsGroundUnit() && pCurrentContainer->GetGroundUnitRepairRate() > 0.0f ) ||
			( this->IsAirUnit()    && pCurrentContainer->GetAirUnitRepairRate() > 0.0f ) ||
			( this->IsSeaUnit()    && pCurrentContainer->GetSeaUnitRepairRate() > 0.0f ) )
		{
			return true;
		}
		pCurrentContainer = pCurrentContainer->GetContainer();
	}
	return false;
}

bool CEOSAIUnit::GetNeedsToBeUpgraded()
{
	EOSAI::CGamePlayer* pGamePlayer = g_pEOSAIInterface->GetGamePlayer( this->GetOwner() );
	if( pGamePlayer == NULL ) return false; // can happen if the owner is 0
	CString strUnitsubset = pGamePlayer->GetUnitsubset();

	// I need to know the end state of the unit (not the 'current' replay-state value)
	CEOSAIUnitTemplate* pCurrentUnitTemplate = this->GetAIUnitTemplate();
	/*
	CWorldDesc* pWorldDesc = dynamic_cast< CWorldDesc* >( this->GetWorldDesc() );
	if( pWorldDesc && pWorldDesc->GetReplayLastTurn() )
	{
		CPoiObjectState* pEndState = this->GetEndState();
		pCurrentUnitTemplate = pEndState->GetUnitTemplate();
	}
	*/
	if( pCurrentUnitTemplate )
	{
		CString strUpgrade = pCurrentUnitTemplate->GetUnitsubsetUpgradeTo( strUnitsubset );
		if( strUpgrade != _T("") )
		{
			//CUnitTemplate* pUpgradeUnitTemplate = GetCommonState()->GetActiveUnitset()->GetUnitTemplate( strUpgrade );
			CEOSAIUnitTemplate* pUpgradeUnitTemplate = g_pEOSAIMain->GetAIGameRules()->GetAIUnitTemplate( strUpgrade );
			if( pUpgradeUnitTemplate )
			{
				//long iPlayer = GetCommonState()->GetPlayer( this->GetOwner() )->GetPlayerNumber();
				//if( GetCommonState()->GetActiveUnitset()->CanBuildUnit( iPlayer, strUpgrade ) )
				if( g_pEOSAIMain->GetAIGameRules()->CanBuildUnit( GetOwner(), strUpgrade ) )
				{
					return true;
				}
			}
		}
	}
	return false;
}

// Potential Targets (used by the AI to assign targets to aircraft; tactical bombers)
//
void  CEOSAIUnit::ClearPotentialTargets()
{
	m_PotentialTargets.RemoveAll();
}

void  CEOSAIUnit::AddPotentialTarget( CEOSAIUnit* pAIUnit )
{
	m_PotentialTargets.AddTail( pAIUnit );
}

bool  CEOSAIUnit::HasPotentialTargets()
{
	if( m_PotentialTargets.IsEmpty() ) return false;
	return true;
}

CEOSAIUnit* CEOSAIUnit::GetRandomPotentialTarget()
{
	// Give preference to nearby targets
	//CWorldBuildDesc* pWorldBuildDesc = m_pAIBrain->GetWorldDescServer()->GetWorldBuildDesc();
	CEOSAILocation MyLocation = this->GetInitialState()->GetLocation();
	float fAttackRange = GetAIUnitTemplate()->GetMovementRangeForAttack();

	for( long iSearchLimit=1; iSearchLimit<=4; iSearchLimit++ )
	{
		CList< CEOSAIUnit* >  LimitedTargets;
		POSITION pos = m_PotentialTargets.GetHeadPosition();
		while( pos )
		{
			CEOSAIUnit* pAIEnemyUnit = m_PotentialTargets.GetNext( pos );
			float fDistance = g_pWorldDistanceTool->GetDistance( MyLocation, pAIEnemyUnit->GetInitialState()->GetLocation() );
			if( fDistance < iSearchLimit*fAttackRange )
			{
				LimitedTargets.AddTail( pAIEnemyUnit );
			}
		}
		if( LimitedTargets.IsEmpty() == FALSE )
		{
			long iCount = LimitedTargets.GetCount();
			POSITION RandPos = LimitedTargets.FindIndex( rand() % iCount );
			return LimitedTargets.GetAt( RandPos );
		}
	}
	return NULL;
}

float CEOSAIUnit::GetCurrentHP01()
{
	return (float)GetCurrentHP() / (float)m_pAIUnitTemplate->GetMaxHP();
	//return (float)m_iCurrentHP / (float)m_pAIUnitTemplate->GetMaxHP();
}

// Ground Units
bool  CEOSAIUnit::IsGroundUnit(){ return m_pAIUnitTemplate->IsGroundUnit(); }
//bool  CEOSAIUnit::IsSoftGroundUnit(){ return m_pAIUnitTemplate->GetUnitTemplate()->IsSoftGroundUnit(); }
//bool  CEOSAIUnit::IsArmor(){ return m_pAIUnitTemplate->GetUnitTemplate()->IsArmor(); }
bool  CEOSAIUnit::IsCityHunter(){ return m_pAIUnitTemplate->IsCityHunter(); }
// Air Units
bool  CEOSAIUnit::IsAirUnit(){ return m_pAIUnitTemplate->IsAirUnit(); }
//bool  CEOSAIUnit::IsAircraft(){ return m_pAIUnitTemplate->GetUnitTemplate()->IsAircraft(); }
//bool  CEOSAIUnit::IsMissile(){ return m_pAIUnitTemplate->GetUnitTemplate()->IsMissile(); }
// Sea Units
bool  CEOSAIUnit::IsSeaUnit(){ return m_pAIUnitTemplate->IsSeaUnit(); }
bool  CEOSAIUnit::IsSubmarine(){ return m_pAIUnitTemplate->IsSubmarine(); }
//bool  CEOSAIUnit::IsShip(){ return m_pAIUnitTemplate->GetUnitTemplate()->IsShip(); }
bool  CEOSAIUnit::IsSeaResHunter(){ return m_pAIUnitTemplate->IsSeaResHunter(); }
bool  CEOSAIUnit::IsTransport(){ return m_pAIUnitTemplate->IsTransport(); }
bool  CEOSAIUnit::IsAirUnitCarrier(){ return m_pAIUnitTemplate->IsAirUnitCarrier(); }
//bool  CEOSAIUnit::IsSubmarine(){ return m_pAIUnitTemplate->GetUnitTemplate()->IsSubmarine(); }

void  CEOSAIUnit::SetAIUnitTemplateByInternalName( CString strInternalName )
{
	CEOSAIUnitTemplate* pEOSAIUnitTemplate = g_pEOSAIMain->GetAIGameRules()->GetAIUnitTemplate( strInternalName );
	ASSERT( pEOSAIUnitTemplate );
	SetAIUnitTemplate( pEOSAIUnitTemplate );
}

/*
CUnit* CEOSAIUnit::GetPlayerUnit()
{
	return dynamic_cast< CUnit* >( m_pPlayerPoiObject );
}
*/
/*
CPoiObject* CEOSAIUnit::GetServerPoiObject()
{
	if( m_pServerUnit )
	{
		return m_pServerUnit;
	}
	if( m_pBuiltByCity )
	{
		return m_pBuiltByCity->GetServerCity();
	}
	ASSERT( false );
	return NULL;
}

long  CEOSAIUnit::GetObjectId()
{
	ASSERT( m_pServerUnit );
	if( m_pServerUnit )
	{
		return m_pServerUnit->ObjectId();
	}
	return 0;
}
*/
/*
CUnitTemplate* CEOSAIUnit::GetUnitTemplate()
{
	return m_pAIUnitTemplate->GetUnitTemplate();
}
*/
/*
void CEOSAIUnit::SetContainer( CAIPlan* pAIPlan, long iContainer )
{
	CAIObject* pAIObject = pAIPlan->GetAIObject( iContainer );
	if( pAIObject )
	{
		CEOSAIPoiObject* pPoiObject = dynamic_cast< CEOSAIPoiObject* >( pAIObject );
		if( pPoiObject )
		{
			m_pInitialContainer = pPoiObject;
			pPoiObject->GetInitialUnitsInsideMe()->AddTail( this );
		}
	}
}
*/
/*
void CEOSAIUnit::CreateTypeInformation()
{
	// Create AILogicObject types
	ASSERT( m_pUnitTemplate );
	if( m_pUnitTemplate )
	{
		if( m_pUnitTemplate->IsGroundUnit() )
		{
			//this->AddLogicalObjectType( "GroundUnit" );
		}
		if( m_pUnitTemplate->CanCaptureCities() )
		{
			//this->AddLogicalObjectType( "CityHunter" );
		}
		if( m_pUnitTemplate->GetMovementRate() > 0.0f )
		{
			//this->AddLogicalObjectType( "Explorer" );
		}
		if( m_pUnitTemplate->CanContainUnits() )
		{
			//if( m_pUnitTemplate->GetNumberOfGroundUnitsThatICanContain() > 0 )
			//{
			//	this->AddLogicalObjectType( "GroundTransport" );
			//}
		}
	}
}
*/
// Information
/*
bool CEOSAIUnit::IAmOfType( CString strLogicObjectName )
{
	POSITION pos = m_IAmOfType.GetHeadPosition();
	while( pos )
	{
		CString strType = m_IAmOfType.GetNext( pos );
		if( strType == strLogicObjectName )
		{
			return true;
		}
	}
	return false;
}
*/

long CEOSAIUnit::GetNumberOfGroundUnitsThatICanContain()
{
	return m_pAIUnitTemplate->GetNumberOfGroundUnitsThatICanContain();
}
/*
long CEOSAIUnit::GetAvailableGroundUnitContainerSpace()
{
	fdssd
}
*/
long CEOSAIUnit::GetNumberOfAirUnitsThatICanContain()
{
	return m_pAIUnitTemplate->GetNumberOfAirUnitsThatICanContain();
}

long CEOSAIUnit::GetMySize()
{
	return m_pAIUnitTemplate->GetMySizeInContainer();
}

long CEOSAIUnit::GetContainerSize()
{
	return m_pAIUnitTemplate->GetNumberOfUnitsThatICanContain();
}

void CEOSAIUnit::DeleteTransportSpaceAllocations()
{
	while( m_BestCase_TransportSpaceAllocations.IsEmpty() == FALSE )
	{
		delete m_BestCase_TransportSpaceAllocations.RemoveHead();
	}
	m_Real_TransportSpaceAllocations.Clear();
}

CEOSAITransportSpaceAllocation*  CEOSAIUnit::GetTransportSpaceAllocationObj( CEOSAITaskForce3* pAITaskForce )
{
	if( pAITaskForce->GetBestCaseOrRealTaskForce() == CEOSAITaskForce3::BestCaseTaskForce )
	{
		// Lookup the TaskForce
		POSITION pos = m_BestCase_TransportSpaceAllocations.GetHeadPosition();
		while( pos )
		{
			CEOSAITransportSpaceAllocation* pTransportAlloc = m_BestCase_TransportSpaceAllocations.GetNext( pos );
			if( pTransportAlloc->m_pAITaskForce == pAITaskForce )
			{
				return pTransportAlloc;//->m_AllocatedTransportSpace;
			}
		}
	}
	if( pAITaskForce->GetBestCaseOrRealTaskForce() == CEOSAITaskForce3::RealTaskForce )
	{
		return &m_Real_TransportSpaceAllocations;
	}
	return NULL;
}

//CList< CEOSAIUnitActionIdea* >*  CEOSAIUnit::InvokeTransportSpaceAllocationList( CEOSAITaskForce3* pAITaskForce )
CEOSAITransportSpaceAllocation*  CEOSAIUnit::InvokeTransportSpaceAllocationObj( CEOSAITaskForce3* pAITaskForce )
{
	CEOSAITransportSpaceAllocation* pAllocationObj = GetTransportSpaceAllocationObj( pAITaskForce );
	if( pAllocationObj == NULL )
	{
		ASSERT( pAITaskForce->GetBestCaseOrRealTaskForce() == CEOSAITaskForce3::BestCaseTaskForce );
		pAllocationObj = new CEOSAITransportSpaceAllocation( this, pAITaskForce );
		m_BestCase_TransportSpaceAllocations.AddTail( pAllocationObj );
	}
	return pAllocationObj;
}

bool CEOSAIUnit::HasAllocatedSpace( CEOSAITaskForce3* pAITaskForce, CEOSAIUnitActionIdea* pUnitActionIdea )
{
	CEOSAITransportSpaceAllocation* pAllocationObj = GetTransportSpaceAllocationObj( pAITaskForce );
	if( pAllocationObj )
	{
		return pAllocationObj->HasAllocatedSpace( pUnitActionIdea );
	}
	return false;
}

void CEOSAIUnit::AllocateTransportSpace( CEOSAITaskForce3* pAITaskForce, CEOSAIUnitActionIdea* pIdea )
{
	CEOSAITransportSpaceAllocation* pAllocationObj = InvokeTransportSpaceAllocationObj( pAITaskForce );
	ASSERT( pAllocationObj );
	pAllocationObj->AllocateTransportSpace( pIdea );

	if( pAITaskForce->GetBestCaseOrRealTaskForce() == CEOSAITaskForce3::RealTaskForce && 
		GetBuiltByCity() != NULL )
	{
		// Allocate the city to build
		CEOSAICity* pAICity = GetBuiltByCity();
		if( pAICity->GetAllocatedUnitActionIdea() == NULL )
		{
			pAICity->SetAllocatedUnitActionIdea( pIdea );
		}
		else
		{
			ASSERT( pAICity->GetAllocatedUnitActionIdea()->GetTacticalProject2() == pIdea->GetTacticalProject2() );
		}
	}
}

void CEOSAIUnit::DeAllocateTransportSpace( CEOSAITaskForce3* pAITaskForce, CEOSAIUnitActionIdea* pIdea )
{
	CEOSAITransportSpaceAllocation* pAllocationObj = GetTransportSpaceAllocationObj( pAITaskForce );
	ASSERT( pAllocationObj );
	if( pAllocationObj )
	{
		pAllocationObj->DeAllocateTransportSpace( pIdea );
	}
	if( pAITaskForce->GetBestCaseOrRealTaskForce() == CEOSAITaskForce3::RealTaskForce && 
		GetBuiltByCity() != NULL )
	{
		// Allocate the city to build
		CEOSAICity* pAICity = GetBuiltByCity();
		pAICity->ReleaseAllocatedUnitActionIdea( pIdea );
	}
}

bool  CEOSAIUnit::CanInsertTransportIdea( CEOSAITaskForce3* pAITaskForce, CEOSAIUnitActionIdea* pUnitActionIdea )
{
	CEOSAITransportSpaceAllocation* pAllocationObj = InvokeTransportSpaceAllocationObj( pAITaskForce );
	return pAllocationObj->CanInsert( pUnitActionIdea );
}

/*
long  CEOSAIUnit::GetAllocatedContainerSpace( CEOSAITaskForce3* pAITaskForce )
{
	CEOSAITransportSpaceAllocation* pAllocationObj = GetTransportSpaceAllocationObj( pAITaskForce );
	if( pAllocationObj )
	{
		long iAllocatedSpace = 0;
		if( this->m_pUnit->GetNumberOfDescendents() ){ iAllocatedSpace += this->m_pUnit->GetNumberOfDescendents(); }
		return iAllocatedSpace + pAllocationObj->m_iAllocatedSpace;
	}
	return 0;
}
*/

bool CEOSAIUnit::CanCaptureCities(){ return m_pAIUnitTemplate->CanCaptureCities(); }

bool CEOSAIUnit::CanCaptureResources(){ return m_pAIUnitTemplate->CanCaptureResources(); }

//
float  CEOSAIUnit::GetMovementRate()
{
	return m_pAIUnitTemplate->GetMovementRate();
}

float  CEOSAIUnit::GetMovementLimit()
{
	return m_pAIUnitTemplate->GetMovementLimit();
}

float  CEOSAIUnit::GetMovementRangeForAttack()
{
	return m_pAIUnitTemplate->GetMovementRangeForAttack();
}

EOSAIEnumMovementType3 CEOSAIUnit::GetMovementType()
{
	return m_pAIUnitTemplate->GetMovementType();
}
/*
float CEOSAIUnit::GetMovementConsumed()
{
	/-*
	if( m_pServerUnit )
	{
		return m_pServerUnit->GetMovementConsumed();
	}
	*-/
	return m_fMovementConsumed;
}
*/
bool CEOSAIUnit::HasARange()
{
	return m_pAIUnitTemplate->HasARange();
}


float CEOSAIUnit::GetMaxAttackRange()
{
	float fMaxRange = 0.0f; // count the largest value twice (specialized units can sometimes be extra-dangerous)
	POSITION pos = g_pEOSAIMain->GetAIGameRules()->GetCombatUnitTypes()->GetHeadPosition();
	while( pos )
	{
		CEOSAICombatUnitType* pCombatUnitType = g_pEOSAIMain->GetAIGameRules()->GetCombatUnitTypes()->GetNext( pos );
		//CEOSAIAttackVs2* pAttackVs = GetAttackVs( pCombatUnitType->m_strName );
		CEOSAIAttackVs2* pAttackVs = GetAttackVs( pCombatUnitType->m_iCombatUnitType );

		//ASSERT( pAttackVs );
		if( pAttackVs )
		{
			fMaxRange = max( fMaxRange, pAttackVs->GetAttackRange() );
		}
	}
	return fMaxRange;
}


float CEOSAIUnit::GetAttackRange( CEOSAIPoiObject* pTarget, bool bIsLandedOrDocked )
{
	if( dynamic_cast< CEOSAIUnit* >( pTarget ) )
	{
		CEOSAIUnit* pUnit = dynamic_cast< CEOSAIUnit* >( pTarget );
		long iTargetCombatUnitType = pUnit->GetCombatUnitTypeLong();
		if( bIsLandedOrDocked )
		{
			iTargetCombatUnitType = pUnit->GetCombatUnitTypeWhenLandedOrDocked();
		}
		CEOSAIAttackVs2* pAttackVs = GetAttackVs( iTargetCombatUnitType );
		ASSERT( pAttackVs );
		if( pAttackVs )
		{
			return pAttackVs->GetFullAttackRange();
		}
		return 0.0f;
	}
	/*
	if( dynamic_cast< CEOSAIUnitGroup* >( pTarget ) )
	{
		float fMaxAttackRange = 0.0f;
		POSITION pos = pTarget->GetUnitsInsideMe()->GetHeadPosition();
		while( pos )
		{
			CPoiObject* pPoiObjectInside = pTarget->GetUnitsInsideMe()->GetNext( pos )->GetPtr();
			float fAttackRange = GetAttackRange( pPoiObjectInside, bIsLandedOrDocked );
			fMaxAttackRange = max( fMaxAttackRange, fAttackRange );
		}
		return fMaxAttackRange;
	}
	*/
	return m_pAIUnitTemplate->GetAttackVsCitResFort()->GetFullAttackRange();
}

//

long CEOSAIUnit::GetNumberOfAvailableUpgrades()
{
	EOSAI::CGamePlayer* pGamePlayer = g_pEOSAIInterface->GetGamePlayer( this->GetOwner() );
	//CPlayer* pPlayer = GetCommonState()->GetPlayer( this->GetOwner() );
	if( pGamePlayer == NULL ) return false; // can happen if the owner is 0
	CString strUnitsubset = pGamePlayer->GetUnitsubset();

	// I need to know the end state of the unit (not the 'current' replay-state value)
	CEOSAIUnitTemplate* pCurrentUnitTemplate = this->GetAIUnitTemplate();
	/*
	CWorldDesc* pWorldDesc = dynamic_cast< CWorldDesc* >( this->GetWorldDesc() );
	if( pWorldDesc && pWorldDesc->GetReplayLastTurn() )
	{
		CPoiObjectState* pEndState = this->GetEndState();
		pCurrentUnitTemplate = pEndState->GetUnitTemplate();
	}
	*/
	long iAvailableUpgrades = 0;
	if( pCurrentUnitTemplate )
	{
		CString strUpgrade = pCurrentUnitTemplate->GetUnitsubsetUpgradeTo( strUnitsubset );
		if( strUpgrade != _T("") )
		{
			CEOSAIUnitTemplate* pUpgradeUnitTemplate = g_pEOSAIMain->GetAIGameRules()->GetAIUnitTemplate( strUpgrade );
			if( pUpgradeUnitTemplate )
			{
				//long iPlayer = GetCommonState()->GetPlayer( this->GetOwner() )->GetPlayerNumber();
				if( g_pEOSAIMain->GetAIGameRules()->CanBuildUnit( this->GetOwner(), strUpgrade ) )
				{
					pCurrentUnitTemplate = pUpgradeUnitTemplate;
					iAvailableUpgrades++;
				}
			}
		}
	}
	return iAvailableUpgrades;
}

// Resources
//
void CEOSAIUnit::GetResourcesProducedPerTurn(std::map<CString, float>& ResourcesProducedAssumingNoOrders, std::map<CString, float>& ResourcesProducedByCurrentOrders)
{
	// TODO: This is very game specific. It needs to be changed.
	POSITION pos = this->GetAIUnitTemplate()->GetProductionAndConsumptionPerTurnList()->GetHeadPosition();
	while (pos)
	{
		EOSAI::StringAndFloat* p = this->GetAIUnitTemplate()->GetProductionAndConsumptionPerTurnList()->GetNext(pos);
		if (p->m_fValue > 0.0f)
		{
			ResourcesProducedAssumingNoOrders[p->m_strValue] += p->m_fValue;
		}
	}
	pos = this->GetAIUnitTemplate()->GetProductionAndConsumptionPerMoveList()->GetHeadPosition();
	while (pos)
	{
		EOSAI::StringAndFloat* p = this->GetAIUnitTemplate()->GetProductionAndConsumptionPerMoveList()->GetNext(pos);
		//ResourcesConsumedByCurrentOrders[_T("Oil")] += this->Has;
		if (p->m_fValue > 0.0f)
		{
			ResourcesProducedByCurrentOrders[p->m_strValue] += p->m_fValue;
		}
	}
}

void CEOSAIUnit::GetResourcesConsumedPerTurn(std::map<CString, float>& ResourcesConsumedAssumingNoOrders, std::map<CString, float>& ResourcesConsumedByCurrentOrders)
{
	// TODO: This is very game specific. It needs to be changed.

	// Consume oil
	POSITION pos = this->GetAIUnitTemplate()->GetProductionAndConsumptionPerTurnList()->GetHeadPosition();
	while (pos)
	{
		EOSAI::StringAndFloat* p = this->GetAIUnitTemplate()->GetProductionAndConsumptionPerTurnList()->GetNext(pos);
		if (p->m_fValue < 0.0f)
		{
			ResourcesConsumedAssumingNoOrders[p->m_strValue] += -p->m_fValue;
		}
	}
	pos = this->GetAIUnitTemplate()->GetProductionAndConsumptionPerMoveList()->GetHeadPosition();
	while (pos)
	{
		EOSAI::StringAndFloat* p = this->GetAIUnitTemplate()->GetProductionAndConsumptionPerMoveList()->GetNext(pos);
		//ResourcesConsumedByCurrentOrders[_T("Oil")] += this->Has;
		if (p->m_fValue < 0.0f)
		{
			ResourcesConsumedByCurrentOrders[p->m_strValue] += -p->m_fValue;
		}
	}
}

//
CEOSAIUnit* CEOSAIUnit::GetCurrentAIUnitContainer()
{
	//if( m_pServerUnit )
	{
		CEOSAIUnit* pContainer = dynamic_cast< CEOSAIUnit* >( this->GetContainerAIPoiObject() );
		if( pContainer )
		{
			return g_pEOSAICommonData->GetAIUnit( pContainer->GetObjectId() );
			//return m_pAIPlayer->GetAIBrain()->GetAIUnit( pContainer->GetObjectId() );
		}
	}
	return NULL;
}

bool CEOSAIUnit::IsInsideTransport()
{
	ASSERT( IsGroundUnit() );
	if( this->IsGroundUnit() == false ) return false;

	//if( m_pServerUnit )
	{
		CEOSAIPoiObject* pContainer = this->GetContainerAIPoiObject();
		while( pContainer )
		{
			if( pContainer->IsSeaUnit() || pContainer->IsAirUnit() )
			{
				return true;
			}
			pContainer = pContainer->GetContainerAIPoiObject();
		}
	}
	return NULL;
}

CEOSAIUnit* CEOSAIUnit::GetCurrentAITransport()
{
	ASSERT( IsGroundUnit() );
	if( IsGroundUnit() == false ) return NULL;

	{
		CEOSAIPoiObject* pContainer = this->GetContainerAIPoiObject();
		while( pContainer )
		{
			if( pContainer->IsSeaUnit() || pContainer->IsAirUnit() )
			{
				//return m_pAIPlayer->GetAIBrain()->GetAIUnit( pContainer->GetObjectId() );
				return g_pEOSAICommonData->GetAIUnit( pContainer->GetObjectId() );
			}
			pContainer = pContainer->GetContainerAIPoiObject();
		}
	}
	return NULL;
}

bool CEOSAIUnit::CanContainUnits()
{
	return m_pAIUnitTemplate->CanContainUnits();
}

float CEOSAIUnit::GetAIGroundTransportCapability()
{
	//if( m_pServerUnit )
	//{
	//	return m_pServerUnit->GetAIGroundTransportCapability();
	//}
	if( IsAirUnit() || IsSeaUnit() )
	{
		return m_pAIUnitTemplate->GetNumberOfGroundUnitsThatICanContain() * sqrt( m_pAIUnitTemplate->GetMovementRate()/10.0f );
	}
	return 0.0f;
}

// Combat
//

float CEOSAIUnit::GetHPDefenseValue()
{
	//if( m_pServerUnit )
	{
		return this->GetCurrentHP() * m_pAIUnitTemplate->GetDefenseValueForNormalTerrain();
	}
	//return m_pAIUnitTemplate->GetMaxHP() * m_pAIUnitTemplate->GetDefenseValueForNormalTerrain();
}

EOSAI::StringAndFloatSet  CEOSAIUnit::GetCombatPowerVsUnitTypes()
{
	/*
	if( m_pServerUnit )
	{
		return m_pServerUnit->GetCombatPowerVsUnitTypes();
	}
	*/
	return m_pAIUnitTemplate->GetCombatPowerVsUnitTypes();
}

void  CEOSAIUnit::AddToCombatGroup( CList< CEOSAIPoiObject* >* pCombatGroupList )
{
	pCombatGroupList->AddTail( this );
}

/*
float CEOSAIUnit::GetAICombatVsCity() // How well can this unit attack my Soft units?
{
	/-*
	if( m_pUnit )
	{
		return m_pUnit->GetCurrentHP() * m_pUnit->GetAttackVsCityToHit();
	}
	CEOSAIAttackVs2* pAttackVs = 
		m_pAIUnitTemplate->GetUnitTemplate()->GetAttackVs( EnumPoiObjectType::enum_Soft );
	ASSERT( pAttackVs );
	return m_pAIUnitTemplate->GetUnitTemplate()->GetMaxHP() * pAttackVs->GetDamage() * pAttackVs->GetToHit();
	*-/
	// INCOMPLETE
	return 1.0f;
}
*/
void CEOSAIUnit::InvokeMultiRegionPathwaysToEverywhere()
{
	InvokeDirectAIRegionMapToEverywhere(); // 4000x3200 = 320K
	//if( m_pUnitTemplate->IsGroundUnit() )
	//{
		//InvokeDirectAIRegionMapToCoasts(); // 4000x3200 = 48K
		//InvokeLandAndImaginarySeaTransportAIRegionMapToEverywhere(); // 4000x3200 = 1MB, 1.5MB, 2.0MB
	//}
	/*
	InvokeDirectMultiRegionMapToEverywhere(); // 4000x3200 = 320K
	if( m_pUnitTemplate->IsGroundUnit() )
	{
		InvokeDirectMultiRegionMapToCoasts(); // 4000x3200 = 48K
		InvokeLandAndImaginarySeaTransportMultiRegionMapToEverywhere(); // 4000x3200 = 1MB, 1.5MB, 2.0MB
	}
	*/
}

// Cache the data into an AIRegionMapToEverywhere structure
CEOSAIRegionPathwayMap* CEOSAIUnit::InvokeDirectAIRegionMapToEverywhere()
{
	//return m_pAIBrain->GetAIRegionMapToEverywhere()->InvokeDirectAIRegionMapToEverywhere( 
	return g_pEOSAICommonData->GetAIRegionMapToEverywhere()->InvokeDirectAIRegionMapToEverywhere( 
				m_InitialState.GetPathfinderPoint()->GetId(), GetAIUnitTemplate() );
				//m_InitialState.GetAIRegion()->GetId(), GetUnitTemplate() );
/*
	if( m_AIRegionMapDirectToEverywhere.GetArraySize() == 0 )
	{
		long iOwner = m_InitialState.GetOwner();
		CEOSAILocation StartLocation = m_InitialState.GetLocation();//GetPoiObject()->GetLocation();

		CEOSAIRegionPathfinder& RegionPathfinder = m_pAIBrain->GetAIRegionManager()->GetPathfinder();
		RegionPathfinder.ClearPathfindingSettings();
		RegionPathfinder.SetPlayer( iOwner );
		RegionPathfinder.SetMovementRate( GetUnitTemplate() );
		RegionPathfinder.SetDistanceMultiplier( 1.0f );
		//RegionPathfinder.SetGeoSwitchMultiplier( 300.0f );
		RegionPathfinder.SetToIgnoreOtherPlayersPathway( iOwner );
		RegionPathfinder.AddStartLocation( StartLocation );
		//RegionPathfinder.AddEndLocation( pPoint->m_Location );//pMultiRegion );
		//RegionPathfinder.Solve2( iOwner, true );
		RegionPathfinder.Solve3();
		RegionPathfinder.GetResult( true, &m_AIRegionMapDirectToEverywhere );

		// Enlarge the Pathway map - find coastal locations
	}
	//
	return &m_AIRegionMapDirectToEverywhere;
	*/
}

CEOSAIRegionPathwayMap* CEOSAIUnit::InvokeDirectAIRegionMapToCoasts()
{
	//return m_pAIBrain->GetAIRegionMapToEverywhere()->InvokeDirectAIRegionMapToCoasts( 
	return g_pEOSAICommonData->GetAIRegionMapToEverywhere()->InvokeDirectAIRegionMapToCoasts( 
				m_InitialState.GetAIRegion()->GetId(), GetAIUnitTemplate() );
/*
	ASSERT( m_pUnitTemplate->IsGroundUnit() );
	if( m_pUnitTemplate->IsGroundUnit() == false ){ return NULL; }

	InvokeDirectAIRegionMapToEverywhere();
	if( m_AIRegionMapDirectToCoasts.GetArraySize() == 0 )
	{
		float fMovementRate = m_pUnitTemplate->GetMovementRate();
		CEOSAIRegionPathwayMap* pPathwayMap = InvokeDirectAIRegionMapToEverywhere();
		CEOSAIRegionPathfinder::AddBorderRegions( pPathwayMap, &m_AIRegionMapDirectToCoasts, fMovementRate );
		//CEOSAIRegionPathfinder::AddBorderRegions( &m_AIRegionMapDirectToEverywhere, &m_AIRegionMapDirectToCoasts, fMovementRate );
	}
	return &m_AIRegionMapDirectToCoasts;
*/
}

/*
CEOSAIRegionPathwayMap* CEOSAIUnit::InvokeLandAndImaginarySeaTransportAIRegionMapToEverywhere()
{
	ASSERT( m_pUnitTemplate->IsGroundUnit() );
	if( m_pUnitTemplate->IsGroundUnit() == false ){ return NULL; }
	if( m_LandAndImaginarySeaTransportAIRegionMapToEverywhere.GetArraySize() == 0 )
	{
		long iOwner = this->GetOwner();
		CEOSAILocation StartLocation = GetServerPoiObject()->GetLocation();

		CEOSAIUnitTemplateSet* pUnitTemplateSet = GetAIBrain()->GetAIThoughtDatabase()->GetUnitsICanBuildOrHave();
		float fFastestSeaTransport = pUnitTemplateSet->GetFastestSeaTransportMovementRate( m_pUnitTemplate );

		CTerrainBasedValue MovementDesc;
		MovementDesc.Set( m_pUnitTemplate );
		MovementDesc.m_fTerrainValue[TERRAIN_COAST] = fFastestSeaTransport;
		MovementDesc.m_fTerrainValue[TERRAIN_WATER] = fFastestSeaTransport;

		CEOSAIRegionPathfinder& RegionPathfinder = m_pAIBrain->GetAIRegionManager()->GetPathfinder();
		RegionPathfinder.ClearPathfindingSettings();
		RegionPathfinder.SetPlayer( iOwner );
		RegionPathfinder.SetGeoSwitches_TreatInitialCitiesAsLand( true );
		RegionPathfinder.SetGeoSwitchMultiplier( 100.0f ); // we want to avoid excessive geo-switching
		//RegionPathfinder.SetLandDistanceMultiplier( GetUnitTemplate() );
		RegionPathfinder.SetMovementRate( &MovementDesc );
		//RegionPathfinder.SetSeaDistanceMultiplier( 1.0f );
		//RegionPathfinder.SetMovementRate( &LandMovementDesc );
		RegionPathfinder.SetToIgnoreOtherPlayersPathway( iOwner );
		RegionPathfinder.AddStartLocation( StartLocation );
		RegionPathfinder.Solve3();
		RegionPathfinder.GetResult( true, &m_LandAndImaginarySeaTransportAIRegionMapToEverywhere );
	}
	return &m_LandAndImaginarySeaTransportAIRegionMapToEverywhere;
}
*/
CEOSAIRegionPathwayMap* CEOSAIUnit::InvokeDirectMultiRegionMapToEverywhere()
{
	if( m_MultiRegionMapDirectToEverywhere.GetArraySize() == 0 )
	{
		long iOwner = m_InitialState.GetOwner();
		CEOSAILocation StartLocation = m_InitialState.GetLocation();//GetPoiObject()->GetLocation();

		//CEOSAIRegionManager2* pAIRegionManager = m_pAIBrain->GetAIRegionManager();
		//CEOSAIRegionPathwayMap  PathwayResult;
		CEOSAIRegionPathfinder& RegionPathfinder = g_pEOSAICommonData->GetMultiRegionManager()->GetPathfinder();
		RegionPathfinder.ClearPathfindingSettings();
		RegionPathfinder.SetPlayer( iOwner );
		RegionPathfinder.SetMovementRate( GetAIUnitTemplate() );
		RegionPathfinder.SetDistanceMultiplier( 1.0f );
		//RegionPathfinder.SetGeoSwitchMultiplier( 300.0f );
		RegionPathfinder.SetToIgnoreOtherPlayersPathway( iOwner );
		RegionPathfinder.AddStartLocation( StartLocation );
		//RegionPathfinder.AddEndLocation( pPoint->m_Location );//pMultiRegion );
		//RegionPathfinder.Solve2( iOwner, true );
		RegionPathfinder.Solve3();
		RegionPathfinder.GetResult( true, &m_MultiRegionMapDirectToEverywhere );

		// Enlarge the Pathway map - find coastal locations
	}
	//
	return &m_MultiRegionMapDirectToEverywhere;
}

CEOSAIRegionPathwayMap* CEOSAIUnit::InvokeDirectMultiRegionMapToCoasts()
{
	ASSERT( m_pAIUnitTemplate->IsGroundUnit() );
	if( m_pAIUnitTemplate->IsGroundUnit() == false ){ return NULL; }

	InvokeDirectMultiRegionMapToEverywhere();
	if( m_MultiRegionMapDirectToCoasts.GetArraySize() == 0 )
	{
		float fMovementRate = m_pAIUnitTemplate->GetMovementRate();
		CEOSAIRegionPathfinder::AddBorderRegions( &m_MultiRegionMapDirectToEverywhere, &m_MultiRegionMapDirectToCoasts, fMovementRate );
	}
	return &m_MultiRegionMapDirectToCoasts;
}

CEOSAIRegionPathwayMap* CEOSAIUnit::InvokeLandAndImaginarySeaTransportMultiRegionMapToEverywhere()
{
	ASSERT( m_pAIUnitTemplate->IsGroundUnit() );
	if( m_pAIUnitTemplate->IsGroundUnit() == false ){ return NULL; }
	if( m_LandAndImaginarySeaTransportMultiRegionMapToEverywhere.GetArraySize() == 0 )
	{
		long iOwner = this->GetOwner();
		CEOSAILocation StartLocation = this->GetLocation();

		CEOSAIUnitTemplateSet* pAIUnitTemplateSet = g_pEOSAICommonData->GetAIThoughtDatabase( GetOwner() )->GetUnitsICanBuildOrHave();
		float fFastestSeaTransport = pAIUnitTemplateSet->GetFastestSeaTransportMovementRate( m_pAIUnitTemplate );

		CEOSAITerrainBasedValue MovementDesc;
		MovementDesc = *m_pAIUnitTemplate->GetTerrainBasedMovementDesc();// );
		MovementDesc.m_fTerrainValue[TERRAIN_COAST] = fFastestSeaTransport;
		MovementDesc.m_fTerrainValue[TERRAIN_WATER] = fFastestSeaTransport;

		CEOSAIRegionPathfinder& RegionPathfinder = g_pEOSAICommonData->GetAIRegionManager()->GetPathfinder();
		RegionPathfinder.ClearPathfindingSettings();
		RegionPathfinder.SetPlayer( iOwner );
		RegionPathfinder.SetGeoSwitches_TreatInitialCitiesAsLand( true );
		RegionPathfinder.SetGeoSwitchMultiplier( 100.0f ); // we want to avoid excessive geo-switching
		//RegionPathfinder.SetLandDistanceMultiplier( GetUnitTemplate() );
		RegionPathfinder.SetMovementRate( &MovementDesc );
		//RegionPathfinder.SetSeaDistanceMultiplier( 1.0f );
		//RegionPathfinder.SetMovementRate( &LandMovementDesc );
		RegionPathfinder.SetToIgnoreOtherPlayersPathway( iOwner );
		RegionPathfinder.AddStartLocation( StartLocation );
		RegionPathfinder.Solve3();
		RegionPathfinder.GetResult( true, &m_LandAndImaginarySeaTransportMultiRegionMapToEverywhere );
	}
	return &m_LandAndImaginarySeaTransportMultiRegionMapToEverywhere;
}

// return Time-To-Target
float CEOSAIUnit::GetTransportCombinedAssistedMovementTime( CEOSAILocation TargetLocation )
{
	float fBestTime = 1000000.0f;
	//CList< CEOSAITransportCombinedMap* >* pMaps = g_pEOSAICommonData->GetAIThoughtDatabase( GetOwner() )->GetTransportCombinedMultiRegionMaps();
	EOSAI::AIPlayer* pAIPlayer = g_pEOSAIInterface->GetAIPlayer( this->GetOwner() );
	ASSERT( pAIPlayer != NULL );
	if( pAIPlayer == NULL ) return 100000.0f;

	CList< CEOSAITransportCombinedMap* >* pMaps = pAIPlayer->GetAIBrain()->GetTransportCombinedMultiRegionMaps();
	POSITION pos = pMaps->GetHeadPosition();
	while( pos )
	{
		CEOSAITransportCombinedMap* pAITransportCombinedMap = pMaps->GetNext( pos );
		//CEOSAITransportCombinedMap* pAITransportCombinedMap = g_pEOSAICommonData->GetTransportCombinedMultiRegionMaps()->GetNext( pos );
		float fTime = GetTransportAssistedMultiRegionMovementPath( &pAITransportCombinedMap->m_PathwayMap, pAITransportCombinedMap->m_pAITransport, 0.0f, TargetLocation );
		fBestTime = min( fBestTime, fTime );
	}
	return fBestTime;
}

float CEOSAIUnit::GetTransportAssistedAIRegionMovementPath( CEOSAIUnitActionIdea* pAITransportActionIdea, CEOSAILocation TargetLocation, CEOSAITransportAssistedPath* pPathResults )
{
	if( pAITransportActionIdea->GetAIUnitActor() )
	{
		CEOSAIUnit* pAITransport = pAITransportActionIdea->GetAIUnitActor();
		//pAITransport->InvokeMultiRegionPathwaysToEverywhere();
		//CEOSAIRegionPathwayMap* pTransportMovementMap = pTransport->InvokeDirectAIRegionMapToEverywhere();
		return GetTransportAssistedAIRegionMovementPath( pAITransport, TargetLocation, pPathResults );
	}
	if( pAITransportActionIdea->GetAICityActor() )
	{
		CEOSAICity* pAICity = pAITransportActionIdea->GetAICityActor();
		//pAICity->InvokeMultiRegionPathwaysToEverywhere();
		//CEOSAIRegionPathwayMap* pTransportMovementMap = pAICity->InvokeWaterAIRegionMapToEverywhere();
		return GetTransportAssistedAIRegionMovementPath( pAICity, pAITransportActionIdea->GetAIUnitTemplate(), pAITransportActionIdea->GetBuildTime(), TargetLocation, pPathResults );
	}
	ASSERT( false );
	return 0.0f;
}

float CEOSAIUnit::GetTransportAssistedAIRegionMovementPath( CEOSAIUnit* pTransport, CEOSAILocation TargetLocation, CEOSAITransportAssistedPath* pPathResults )
{
	CEOSAIRegionPathwayMap* pTransportMovementMap = pTransport->InvokeDirectAIRegionMapToEverywhere();
	return GetTransportAssistedAIRegionMovementPath( pTransportMovementMap, pTransport->GetAIUnitTemplate(), pTransport->GetInitialState()->GetTime(), TargetLocation, pPathResults );
}

float CEOSAIUnit::GetTransportAssistedAIRegionMovementPath( CEOSAICity* pCity, CEOSAIUnitTemplate* pTransportUnitTemplate, float fTransportBuildTime, CEOSAILocation TargetLocation, CEOSAITransportAssistedPath* pPathResults )
{
	CEOSAIRegionPathwayMap* pTransportMovementMap = pCity->InvokeWaterAIRegionMapToEverywhere();
	return GetTransportAssistedAIRegionMovementPath( pTransportMovementMap, pTransportUnitTemplate, fTransportBuildTime, TargetLocation, pPathResults );
}

float CEOSAIUnit::GetTransportAssistedAIRegionMovementPath( CEOSAIRegionPathwayMap* pTransportMovementAIRegionMap, CEOSAIUnitTemplate* pTransportUnitTemplate, float fTransportDelayTime, CEOSAILocation TargetLocation, CEOSAITransportAssistedPath* pPathResults )
{
	ASSERT( false );
	ASSERT( pTransportMovementAIRegionMap->GetRegionManager() == g_pEOSAICommonData->GetAIRegionManager() );

	ASSERT( m_pAIUnitTemplate->IsGroundUnit() );
	if( m_pAIUnitTemplate->IsGroundUnit() == false ){ return 1000000.0f; }

	CEOSAIBrain* pAIBrain = g_pAIPlayerManager->GetAIPlayer( GetOwner() )->GetAIBrain();

	// If the [target] is on water (like a ship or sea-resource), then I don't need a transport, do I?
	CEOSAIRegion2* pTargetAIRegion = g_pEOSAICommonData->GetAIRegionManager()->GetAIRegion( TargetLocation );
	//CMultiRegion* pTargetMultiRegion = m_pAIBrain->GetMultiRegionManager()->GetMultiRegion( TargetLocation );
	ASSERT( pTargetAIRegion->IsLand() || pTargetAIRegion->IsCity() );

	if( pPathResults )
	{
		pPathResults->ClearAIRegionData();
	}

	// Calculate time to coastal pickups, transport move-time to dropoff, unit dropoff-to-target time
	//CEOSAIRegionPathwayMap* pTransportMovementMap = pTransport->InvokeDirectAIRegionMapToEverywhere();
	//

	// I have to do a back-calculation from the target back to the pickup points.
	//   Why?  Because forward calculations have a flaw: they weigh too heavily on the existing low-pickup times

	// Algorithm:
	//   Calculate TargetLocation -> Everywhere, Maximum of one geo switch
	//   Calculate the best pickup by looking at the coastal pickup locations (mixing both data together)

	//m_pAIBrain->m_GetTransportAssistedMovementPath.Continue();
	//CEOSAIRegionPathwayMap* pTargetToEverywhereMap = pTargetAIRegion->InvokeGroundUnitAndSeaTransportPathToEverywhere( m_pAIBrain );
	CEOSAIRegionPathwayMap* pTargetToEverywhereAIRegionMap = pTargetAIRegion->InvokeGroundUnitAndSeaTransportPathToEverywhere( pAIBrain );
	//m_pAIBrain->m_GetTransportAssistedMovementPath.Stop();

	float fTransportMovementRate = pTransportUnitTemplate->GetMovementRate();
/*
	float fTransportMovementRate = pTransportUnitTemplate->GetMovementRate();
	CTerrainBasedValue MovementDesc;
	MovementDesc.Set( m_pUnitTemplate );
	MovementDesc.m_fTerrainValue[TERRAIN_COAST] = fTransportMovementRate;
	MovementDesc.m_fTerrainValue[TERRAIN_WATER] = fTransportMovementRate;

	CEOSAIRegionPathwayMap  TargetToEverywhereMap;
	CEOSAIRegionPathfinder& RegionPathfinder = m_pAIBrain->GetAIRegionManager()->GetPathfinder();
	RegionPathfinder.ClearPathfindingSettings();
	RegionPathfinder.SetPlayer( GetOwner() );
	RegionPathfinder.SetGeoSwitches_TreatInitialCitiesAsLand( true );
	RegionPathfinder.SetMaximumGeoSwitches( 1 );
	RegionPathfinder.SetMovementRate( &MovementDesc );
	RegionPathfinder.SetToIgnoreOtherPlayersPathway( GetOwner() );
	RegionPathfinder.AddStartLocation( TargetLocation );
	RegionPathfinder.Solve3();
	RegionPathfinder.GetResult( true, &TargetToEverywhereMap );
*/
	// Two possibilities:
	//  (1) A transport needs to pickup the GroundUnit 
	//        Check GroundTarget->everywhere (1 geo switch)
	//        Check GroundUnit->CoastalLocations (0 geo switches)
	//        Find the overlap, and calculate the time
	//  (2) The GroundUnit is already travelling inside a sea-transport
	//        Check GroundTarget->everywhere (1 geo switch)
	//        Calculate the time

	//CEOSAIRegion2* pCurrentGroundUnitAIRegion = m_pAIBrain->GetAIRegionManager()->GetAIRegion( GetInitialState()->GetLocation() );
	//ASSERT( pCurrentGroundUnitAIRegion );
	float fBestTime = 1000000.0f;
	//if( pTargetToEverywhereMap->GetRegionResult( GetInitialState()->GetLocation() ) )
	if( pTargetToEverywhereAIRegionMap->GetRegionResult( GetInitialState()->GetLocation() ) )
	{
		CEOSAIRegion2* pStartAIRegion = g_pEOSAICommonData->GetAIRegionManager()->GetAIRegion( GetInitialState()->GetLocation() );
		long iStartMultiRegion = pStartAIRegion->m_iId;
		CEOSAIRegionPathwayItem* pTargetToPickupRegion = pTargetToEverywhereAIRegionMap->GetRegionResult( iStartMultiRegion );
		fBestTime = pTargetToPickupRegion->m_fTime;

		if( pPathResults )
		{
			// Drop the pathway information into a path and then walk it backwards
			CList< long > Path;
			CEOSAIRegionPathwayItem* pCurrentPathwayItem = pTargetToPickupRegion;
			while( pCurrentPathwayItem )
			{
				Path.AddHead( pCurrentPathwayItem->m_pPathfinderPoint->GetId() );
				pCurrentPathwayItem = pTargetToEverywhereAIRegionMap->GetRegionResult( pCurrentPathwayItem->m_iPreviousPathwayItem );
			}
			bool bMovementOnLand = true;
			POSITION pos = Path.GetHeadPosition();
			while( pos )
			{
				long iRegion = Path.GetNext( pos );
				CEOSAIRegionPathwayItem* pPathItem = pTargetToEverywhereAIRegionMap->GetRegionResult( iRegion );

				if( pPathItem->m_pPathfinderPoint->IsWater() ){ bMovementOnLand = false; }
				if( bMovementOnLand )
				{
					pPathResults->m_iLandDropoffAIRegion = pPathItem->m_pPathfinderPoint->GetId();
					ASSERT( pPathItem->m_pPathfinderPoint->IsLand() || pPathItem->m_pPathfinderPoint->IsCity() );
					pPathResults->m_GroundUnitFromDropoffToTargetAIRegionPath.AddTail( iRegion );
				}
				else
				{
					if( pPathResults->m_iWaterDropoffAIRegion == 0 )
					{
						pPathResults->m_iWaterDropoffAIRegion = pPathItem->m_pPathfinderPoint->GetId();
					}
					ASSERT( pPathItem->m_pPathfinderPoint->IsWater() || pPathItem->m_pPathfinderPoint->IsCity() );
					pPathResults->m_TransportPickupToDropoffAIRegionPath.AddTail( iRegion );
				}
			}
		}
	}
	else // The GroundUnit needs to be picked-up
	{
		long iGroundUnitGeo = this->GetInitialState()->GetGeo();
		CEOSAIGeo* pGeo = g_pEOSAICommonData->GetAIGeo( iGroundUnitGeo );
		//ASSERT( dynamic_cast< CGeoLand* >( pGeo ) );
		ASSERT( pGeo->GetGeoType() == CEOSAIGeo::Land );

		long iBestLandPickupRegion = 0;
		long iBestWaterPickupRegion = 0;

		CEOSAIRegionPathwayMap* pGroundUnitDirectAIRegionMap = InvokeDirectAIRegionMapToEverywhere();
		CEOSAIRegionManager2* pAIRegionManager = g_pEOSAICommonData->GetAIRegionManager();
		CGeoCoastalData2* pGeoCoastalData = pAIRegionManager->GetGeoCoastalData( iGroundUnitGeo );
		if( pGeoCoastalData )
		{
			long iGroundUnitToPickupRegionCount = 0;
			long iTransportToPickupRegionCount = 0;
			long iTargetToPickupRegionCount = 0;
			POSITION pos = pGeoCoastalData->m_GeoSwitchList.GetHeadPosition();
			while( pos )
			{
				CGeoSwitchItem2* pGeoSwitchItem = pGeoCoastalData->m_GeoSwitchList.GetNext( pos );
				long iLandAIRegion = pGeoSwitchItem->m_iLandAIRegion;
				long iWaterAIRegion = pGeoSwitchItem->m_iWaterAIRegion;
				//
				CEOSAIRegionPathwayItem* pGroundUnitToPickupRegion = pGroundUnitDirectAIRegionMap->GetRegionResult( iLandAIRegion );
				CEOSAIRegionPathwayItem* pTransportToPickupRegion = pTransportMovementAIRegionMap->GetRegionResult( iWaterAIRegion );
				CEOSAIRegionPathwayItem* pTargetToPickupRegion = pTargetToEverywhereAIRegionMap->GetRegionResult( iWaterAIRegion );

				//
				if( pGroundUnitToPickupRegion ) iGroundUnitToPickupRegionCount++;
				if( pTransportToPickupRegion ) iTransportToPickupRegionCount++;
				if( pTargetToPickupRegion ) iTargetToPickupRegionCount++;

				if( pGroundUnitToPickupRegion && pTransportToPickupRegion && pTargetToPickupRegion )
				{
					float fGroundUnitToPickupTime = pGroundUnitToPickupRegion->m_fTime;
					float fTransportToPickupTime = fTransportDelayTime + (pTransportToPickupRegion->m_fDistance/fTransportMovementRate);
					float fTargetToPickupTime = pTargetToPickupRegion->m_fTime;
					float fTotalTime = max( fGroundUnitToPickupTime, fTransportToPickupTime ) + fTargetToPickupTime;
					if( fGroundUnitToPickupTime < 0.6f && fTransportToPickupTime < 0.5f )
					{
						// If GroundUnit and Transport are both close to a pickup location, use it
						// tweak the time to favor this option
						fTotalTime -= 1.0f; 
					}
					if( fTotalTime < fBestTime )
					{
						fBestTime = fTotalTime;
						iBestLandPickupRegion = iLandAIRegion;
						iBestWaterPickupRegion = iWaterAIRegion;
					}
					int g=0;
				}
			}
			int sdsad=0;
		}
		if( pPathResults && iBestLandPickupRegion > 0 && iBestWaterPickupRegion > 0 )
		{
			CEOSAIRegionPathwayItem* pGroundUnitToPickupRegion = pGroundUnitDirectAIRegionMap->GetRegionResult( iBestLandPickupRegion );
			CEOSAIRegionPathwayItem* pTransportToPickupRegion = pTransportMovementAIRegionMap->GetRegionResult( iBestWaterPickupRegion );
			CEOSAIRegionPathwayItem* pTargetToPickupRegion = pTargetToEverywhereAIRegionMap->GetRegionResult( iBestWaterPickupRegion );

			pPathResults->m_iWaterPickupAIRegion = iBestWaterPickupRegion;
			//
			CEOSAIRegionPathwayItem* pCurrentPathwayItem = pGroundUnitToPickupRegion;
			while( pCurrentPathwayItem )
			{
				//iStep++;
				/*
				if( pPathResults->m_iWaterPickupAIRegion == 0 )//iStep == 1 )
				{
					pPathResults->m_iWaterPickupAIRegion = pCurrentPathwayItem->m_pPathfinderPoint->GetId();
				}
				*/
				if( pPathResults->m_iLandPickupAIRegion == 0 )
				{
					//if( pCurrentPathwayItem->m_pPathfinderPoint->IsCity() )
					//{
					//	pPathResults->m_iWaterPickupRegion = pCurrentPathwayItem->m_pPathfinderPoint->GetId();
					//}
					if( pCurrentPathwayItem->m_pPathfinderPoint->IsCity() ||
						pCurrentPathwayItem->m_pPathfinderPoint->IsLand() )
					{
						pPathResults->m_iLandPickupAIRegion = pCurrentPathwayItem->m_pPathfinderPoint->GetId();
					}
				}
				pPathResults->m_GroundUnitToPickupAIRegionPath.AddHead( pCurrentPathwayItem->m_pPathfinderPoint->GetId() );
				//pCurrentPathwayItem = pGroundUnitToCoastsAIRegionMap->GetRegionResult( pCurrentPathwayItem->m_iPreviousPathwayItem );
				pCurrentPathwayItem = pGroundUnitDirectAIRegionMap->GetRegionResult( pCurrentPathwayItem->m_iPreviousPathwayItem );
			}
			pCurrentPathwayItem = pTransportToPickupRegion;
			while( pCurrentPathwayItem )
			{
				pPathResults->m_TransportToPickupAIRegionPath.AddHead( pCurrentPathwayItem->m_pPathfinderPoint->GetId() );
				pCurrentPathwayItem = pTransportMovementAIRegionMap->GetRegionResult( pCurrentPathwayItem->m_iPreviousPathwayItem );
			}

			// Drop the pathway information into a path and then walk it backwards
			CList< long > Path;
			pCurrentPathwayItem = pTargetToPickupRegion;
			while( pCurrentPathwayItem )
			{
				Path.AddHead( pCurrentPathwayItem->m_pPathfinderPoint->GetId() );
				pCurrentPathwayItem = pTargetToEverywhereAIRegionMap->GetRegionResult( pCurrentPathwayItem->m_iPreviousPathwayItem );
			}
			bool bMovementOnLand = true;
			POSITION pos = Path.GetHeadPosition();
			while( pos )
			{
				long iRegion = Path.GetNext( pos );
				CEOSAIRegionPathwayItem* pPathItem = pTargetToEverywhereAIRegionMap->GetRegionResult( iRegion );

				if( pPathItem->m_pPathfinderPoint->IsWater() ){ bMovementOnLand = false; }
				if( bMovementOnLand )
				{
					pPathResults->m_iLandDropoffAIRegion = pPathItem->m_pPathfinderPoint->GetId();
					ASSERT( pPathItem->m_pPathfinderPoint->IsLand() || pPathItem->m_pPathfinderPoint->IsCity() );
					pPathResults->m_GroundUnitFromDropoffToTargetAIRegionPath.AddTail( iRegion );
				}
				else
				{
					if( pPathResults->m_iWaterDropoffAIRegion == 0 )
					{
						pPathResults->m_iWaterDropoffAIRegion = pPathItem->m_pPathfinderPoint->GetId();
					}
					ASSERT( pPathItem->m_pPathfinderPoint->IsWater() || pPathItem->m_pPathfinderPoint->IsCity() );
					pPathResults->m_TransportPickupToDropoffAIRegionPath.AddTail( iRegion );
				}
				/*
				if( bMovementOnLand )
				{
					ASSERT( pCurrentPathwayItem->m_pPathfinderPoint->IsLand() || pCurrentPathwayItem->m_pPathfinderPoint->IsCity() );
					pPathResults->m_TransportPickupToDropoffPath.AddTail( pCurrentPathwayItem->m_pPathfinderPoint->GetId() );
				}
				else
				{
					ASSERT( pCurrentPathwayItem->m_pPathfinderPoint->IsWater() || pCurrentPathwayItem->m_pPathfinderPoint->IsCity() );
					pPathResults->m_GroundUnitFromDropoffToTargetPath.AddTail( pCurrentPathwayItem->m_pPathfinderPoint->GetId() );
				}
				pCurrentPathwayItem = pTargetToEverywhereMap->GetRegionResult( pCurrentPathwayItem->m_iPreviousPathwayItem );
				*/
			}

			int g=0;
		}
	}

	// Iterate over all the overlapping GroundUnit-Coastal + Transport-Coastal regions (Pickup)
	//   Then setup the Movement Description based on GroundUnit + Transport movement

	return fBestTime;
}


float CEOSAIUnit::GetTransportAssistedMultiRegionMovementPath( CEOSAIUnitActionIdea* pAITransportActionIdea, CEOSAILocation TargetLocation, CEOSAITransportAssistedPath* pPathResults )
{
	if( pAITransportActionIdea->GetAIUnitActor() )
	{
		CEOSAIUnit* pAITransport = pAITransportActionIdea->GetAIUnitActor();
		//pAITransport->InvokeMultiRegionPathwaysToEverywhere();
		//CEOSAIRegionPathwayMap* pTransportMovementMap = pTransport->InvokeDirectAIRegionMapToEverywhere();
		return GetTransportAssistedMultiRegionMovementPath( pAITransport, TargetLocation, pPathResults );
	}
	if( pAITransportActionIdea->GetAICityActor() )
	{
		CEOSAICity* pAICity = pAITransportActionIdea->GetAICityActor();
		//pAICity->InvokeMultiRegionPathwaysToEverywhere();
		//CEOSAIRegionPathwayMap* pTransportMovementMap = pAICity->InvokeWaterAIRegionMapToEverywhere();
		return GetTransportAssistedMultiRegionMovementPath( pAICity, pAITransportActionIdea->GetAIUnitTemplate(), pAITransportActionIdea->GetBuildTime(), TargetLocation, pPathResults );
	}
	ASSERT( false );
	return 0.0f;
}

float CEOSAIUnit::GetTransportAssistedMultiRegionMovementPath( CEOSAIUnit* pTransport, CEOSAILocation TargetLocation, CEOSAITransportAssistedPath* pPathResults )
{
	CEOSAIRegionPathwayMap* pTransportMovementMap = pTransport->InvokeDirectMultiRegionMapToEverywhere();
	return GetTransportAssistedMultiRegionMovementPath( pTransportMovementMap, pTransport->GetAIUnitTemplate(), pTransport->GetInitialState()->GetTime(), TargetLocation, pPathResults );
}

float CEOSAIUnit::GetTransportAssistedMultiRegionMovementPath( CEOSAICity* pCity, CEOSAIUnitTemplate* pTransportUnitTemplate, float fTransportBuildTime, CEOSAILocation TargetLocation, CEOSAITransportAssistedPath* pPathResults )
{
	CEOSAIRegionPathwayMap* pTransportMovementMap = pCity->InvokeWaterMultiRegionMapToEverywhere();
	return GetTransportAssistedMultiRegionMovementPath( pTransportMovementMap, pTransportUnitTemplate, fTransportBuildTime, TargetLocation, pPathResults );
}

float CEOSAIUnit::GetTransportAssistedMultiRegionMovementPath( CEOSAIRegionPathwayMap* pTransportMovementMultiRegionMap, CEOSAIUnitTemplate* pTransportUnitTemplate, float fTransportDelayTime, CEOSAILocation TargetLocation, CEOSAITransportAssistedPath* pPathResults )
{
	ASSERT( pTransportMovementMultiRegionMap->GetRegionManager() == g_pEOSAICommonData->GetMultiRegionManager() );

	ASSERT( m_pAIUnitTemplate->IsGroundUnit() );
	if( m_pAIUnitTemplate->IsGroundUnit() == false ){ return 1000000.0f; }

	CEOSAIBrain* pAIBrain = g_pAIPlayerManager->GetAIPlayer( GetOwner() )->GetAIBrain();

	// If the [target] is on water (like a ship or sea-resource), then I don't need a transport, do I?
	//CEOSAIRegion2* pTargetAIRegion = m_pAIBrain->GetAIRegionManager()->GetAIRegion( TargetLocation );
	CEOSAIMultiRegion2* pTargetMultiRegion = g_pEOSAICommonData->GetMultiRegionManager()->GetMultiRegion( TargetLocation );
	ASSERT( pTargetMultiRegion->IsLand() || pTargetMultiRegion->IsCity() );

	if( pPathResults )
	{
		pPathResults->ClearMultiRegionData();
		pPathResults->ClearAIRegionData();
	}

	// Calculate time to coastal pickups, transport move-time to dropoff, unit dropoff-to-target time
	//CEOSAIRegionPathwayMap* pTransportMovementMap = pTransport->InvokeDirectAIRegionMapToEverywhere();
	//

	// I have to do a back-calculation from the target back to the pickup points.
	//   Why?  Because forward calculations have a flaw: they weigh too heavily on the existing low-pickup times

	// Algorithm:
	//   Calculate TargetLocation -> Everywhere, Maximum of one geo switch
	//   Calculate the best pickup by looking at the coastal pickup locations (mixing both data together)

//	m_pAIBrain->m_GetTransportAssistedMovementPath.Continue();
	//CEOSAIRegionPathwayMap* pTargetToEverywhereMap = pTargetAIRegion->InvokeGroundUnitAndSeaTransportPathToEverywhere( m_pAIBrain );
	CEOSAIRegionPathwayMap* pTargetToEverywhereMultiRegionMap = pTargetMultiRegion->InvokeGroundUnitAndSeaTransportPathToEverywhere( pAIBrain );
//	m_pAIBrain->m_GetTransportAssistedMovementPath.Stop();

	float fTransportMovementRate = pTransportUnitTemplate->GetMovementRate();
/*
	float fTransportMovementRate = pTransportUnitTemplate->GetMovementRate();
	CTerrainBasedValue MovementDesc;
	MovementDesc.Set( m_pUnitTemplate );
	MovementDesc.m_fTerrainValue[TERRAIN_COAST] = fTransportMovementRate;
	MovementDesc.m_fTerrainValue[TERRAIN_WATER] = fTransportMovementRate;

	CEOSAIRegionPathwayMap  TargetToEverywhereMap;
	CEOSAIRegionPathfinder& RegionPathfinder = m_pAIBrain->GetAIRegionManager()->GetPathfinder();
	RegionPathfinder.ClearPathfindingSettings();
	RegionPathfinder.SetPlayer( GetOwner() );
	RegionPathfinder.SetGeoSwitches_TreatInitialCitiesAsLand( true );
	RegionPathfinder.SetMaximumGeoSwitches( 1 );
	RegionPathfinder.SetMovementRate( &MovementDesc );
	RegionPathfinder.SetToIgnoreOtherPlayersPathway( GetOwner() );
	RegionPathfinder.AddStartLocation( TargetLocation );
	RegionPathfinder.Solve3();
	RegionPathfinder.GetResult( true, &TargetToEverywhereMap );
*/
	// Two possibilities:
	//  (1) A transport needs to pickup the GroundUnit 
	//        Check GroundTarget->everywhere (1 geo switch)
	//        Check GroundUnit->CoastalLocations (0 geo switches)
	//        Find the overlap, and calculate the time
	//  (2) The GroundUnit is already travelling inside a sea-transport
	//        Check GroundTarget->everywhere (1 geo switch)
	//        Calculate the time

	//CEOSAIRegion2* pCurrentGroundUnitAIRegion = m_pAIBrain->GetAIRegionManager()->GetAIRegion( GetInitialState()->GetLocation() );
	//ASSERT( pCurrentGroundUnitAIRegion );
	float fBestTotalTime = 1000000.0f;
	//if( pTargetToEverywhereMap->GetRegionResult( GetInitialState()->GetLocation() ) )
	if( pTargetToEverywhereMultiRegionMap->GetRegionResult( GetInitialState()->GetLocation() ) )
	{
		// The TargetToEverywhereMultiRegionMap maps from land->water, but not land->water->land
		// If the GroundUnit's Location is in the list, it means it's in the water or on the
		//    same geo as the target

		//CEOSAIRegion2* pStartAIRegion = m_pAIBrain->GetAIRegionManager()->GetAIRegion( GetInitialState()->GetLocation() );
		CEOSAIMultiRegion2* pStartMultiRegion = g_pEOSAICommonData->GetMultiRegionManager()->GetMultiRegion( GetInitialState()->GetLocation() );
		long iStartMultiRegion = pStartMultiRegion->m_iId;
		CEOSAIRegionPathwayItem* pTargetToPickupRegion = pTargetToEverywhereMultiRegionMap->GetRegionResult( iStartMultiRegion );
		fBestTotalTime = pTargetToPickupRegion->m_fTime;

		if( pPathResults )
		{
			// Drop the pathway information into a path and then walk it backwards
			CList< long > Path;
			CEOSAIRegionPathwayItem* pCurrentPathwayItem = pTargetToPickupRegion;
			while( pCurrentPathwayItem )
			{
				Path.AddHead( pCurrentPathwayItem->m_pPathfinderPoint->GetId() );
				pCurrentPathwayItem = pTargetToEverywhereMultiRegionMap->GetRegionResult( pCurrentPathwayItem->m_iPreviousPathwayItem );
			}
			bool bMovementOnLand = true;
			POSITION pos = Path.GetHeadPosition();
			while( pos )
			{
				long iRegion = Path.GetNext( pos );
				CEOSAIRegionPathwayItem* pPathItem = pTargetToEverywhereMultiRegionMap->GetRegionResult( iRegion );

				if( pPathItem->m_pPathfinderPoint->IsWater() ){ bMovementOnLand = false; }
				if( bMovementOnLand )
				{
					pPathResults->m_iLandDropoffMultiRegion = pPathItem->m_pPathfinderPoint->GetId();
					ASSERT( pPathItem->m_pPathfinderPoint->IsLand() || pPathItem->m_pPathfinderPoint->IsCity() );
					pPathResults->m_GroundUnitFromDropoffToTargetMultiRegionPath.AddTail( iRegion );
				}
				else
				{
					if( pPathResults->m_iWaterDropoffMultiRegion == 0 )
					{
						pPathResults->m_iWaterDropoffMultiRegion = pPathItem->m_pPathfinderPoint->GetId();
					}
					if( EOSAISettings::m_bAssertWhenAIPathwayFails )
					{
						ASSERT( pPathItem->m_pPathfinderPoint->IsWater() || pPathItem->m_pPathfinderPoint->IsCity() );
					}
					pPathResults->m_TransportPickupToDropoffMultiRegionPath.AddTail( iRegion );
				}
			}
		}
	}
	else // The GroundUnit needs to be picked-up
	{
		// Now iterate over the coastal locations, find the results
		long  iBestPickupRegion = 0;
		CEOSAIRegionPathwayMap* pGroundUnitToCoastsMultiRegionMap = InvokeDirectMultiRegionMapToCoasts();
		long iNumberOfRegions = pGroundUnitToCoastsMultiRegionMap->GetRegionManager()->GetNumberOfGenericRegions();

		long iGroundUnitToPickupRegionCount = 0;
		long iTransportToPickupRegionCount = 0;
		long iTargetToPickupRegionCount = 0;
		for( long iRegion=0; iRegion<=iNumberOfRegions; iRegion++ )
		{
			CEOSAIRegionPathwayItem* pGroundUnitToPickupRegion = pGroundUnitToCoastsMultiRegionMap->GetRegionResult( iRegion );
			CEOSAIRegionPathwayItem* pTransportToPickupRegion = pTransportMovementMultiRegionMap->GetRegionResult( iRegion );
			CEOSAIRegionPathwayItem* pTargetToPickupRegion = pTargetToEverywhereMultiRegionMap->GetRegionResult( iRegion );

			if( pGroundUnitToPickupRegion ) iGroundUnitToPickupRegionCount++;
			if( pTransportToPickupRegion ) iTransportToPickupRegionCount++;
			if( pTargetToPickupRegion ) iTargetToPickupRegionCount++;
			/*
			if( iRegion == 8 || iRegion == 19 || iRegion == 21 || iRegion == 41 )
			{
				int h=0;
			}
			if( pTransportToPickupRegion )
			{
				int h=0;
			}
			*/
			if( pGroundUnitToPickupRegion && pTransportToPickupRegion && pTargetToPickupRegion )
			{
				float fGroundUnitToPickupTime = pGroundUnitToPickupRegion->m_fTime;
				float fTransportToPickupTime = fTransportDelayTime + (pTransportToPickupRegion->m_fDistance/fTransportMovementRate);
				float fTargetToPickupTime = pTargetToPickupRegion->m_fTime;
				float fTotalTime = max( fGroundUnitToPickupTime, fTransportToPickupTime ) + fTargetToPickupTime;
				if( fGroundUnitToPickupTime < 0.6f && fTransportToPickupTime < 0.5f )
				{
					// If GroundUnit and Transport are both close to a pickup location, use it
					// tweak the time to favor this option
					fTotalTime -= 1.0f; 
				}
				if( fTotalTime < fBestTotalTime )
				{
					fBestTotalTime = fTotalTime;
					iBestPickupRegion = iRegion;
				}
				int g=0;
			}
		}
		if( pPathResults && iBestPickupRegion > 0 )
		{
			CEOSAIRegionPathwayItem* pGroundUnitToPickupRegion = pGroundUnitToCoastsMultiRegionMap->GetRegionResult( iBestPickupRegion );
			CEOSAIRegionPathwayItem* pTransportToPickupRegion = pTransportMovementMultiRegionMap->GetRegionResult( iBestPickupRegion );
			CEOSAIRegionPathwayItem* pTargetToPickupRegion = pTargetToEverywhereMultiRegionMap->GetRegionResult( iBestPickupRegion );

			//
			//long iStep = 0;
			CEOSAIRegionPathwayItem* pCurrentPathwayItem = pGroundUnitToPickupRegion;
			while( pCurrentPathwayItem )
			{
				//iStep++;
				if( pPathResults->m_iWaterPickupMultiRegion == 0 )//iStep == 1 )
				{
					pPathResults->m_iWaterPickupMultiRegion = pCurrentPathwayItem->m_pPathfinderPoint->GetId();
				}
				if( pPathResults->m_iLandPickupMultiRegion == 0 )
				{
					//if( pCurrentPathwayItem->m_pPathfinderPoint->IsCity() )
					//{
					//	pPathResults->m_iWaterPickupRegion = pCurrentPathwayItem->m_pPathfinderPoint->GetId();
					//}
					if( pCurrentPathwayItem->m_pPathfinderPoint->IsCity() ||
						pCurrentPathwayItem->m_pPathfinderPoint->IsLand() )
					{
						pPathResults->m_iLandPickupMultiRegion = pCurrentPathwayItem->m_pPathfinderPoint->GetId();
					}
				}
				pPathResults->m_GroundUnitToPickupMultiRegionPath.AddHead( pCurrentPathwayItem->m_pPathfinderPoint->GetId() );
				pCurrentPathwayItem = pGroundUnitToCoastsMultiRegionMap->GetRegionResult( pCurrentPathwayItem->m_iPreviousPathwayItem );
			}
			pCurrentPathwayItem = pTransportToPickupRegion;
			while( pCurrentPathwayItem )
			{
				pPathResults->m_TransportToPickupMultiRegionPath.AddHead( pCurrentPathwayItem->m_pPathfinderPoint->GetId() );
				pCurrentPathwayItem = pTransportMovementMultiRegionMap->GetRegionResult( pCurrentPathwayItem->m_iPreviousPathwayItem );
			}

			// Drop the pathway information into a path and then walk it backwards
			CList< long > Path;
			pCurrentPathwayItem = pTargetToPickupRegion;
			while( pCurrentPathwayItem )
			{
				Path.AddHead( pCurrentPathwayItem->m_pPathfinderPoint->GetId() );
				pCurrentPathwayItem = pTargetToEverywhereMultiRegionMap->GetRegionResult( pCurrentPathwayItem->m_iPreviousPathwayItem );
			}
			bool bMovementOnLand = true;
			POSITION pos = Path.GetHeadPosition();
			while( pos )
			{
				long iRegion = Path.GetNext( pos );
				CEOSAIRegionPathwayItem* pPathItem = pTargetToEverywhereMultiRegionMap->GetRegionResult( iRegion );

				if( pPathItem->m_pPathfinderPoint->IsWater() ){ bMovementOnLand = false; }
				if( bMovementOnLand )
				{
					pPathResults->m_iLandDropoffMultiRegion = pPathItem->m_pPathfinderPoint->GetId();
					ASSERT( pPathItem->m_pPathfinderPoint->IsLand() || pPathItem->m_pPathfinderPoint->IsCity() );
					pPathResults->m_GroundUnitFromDropoffToTargetMultiRegionPath.AddTail( iRegion );
				}
				else
				{
					if( pPathResults->m_iWaterDropoffMultiRegion == 0 )
					{
						pPathResults->m_iWaterDropoffMultiRegion = pPathItem->m_pPathfinderPoint->GetId();
					}
					ASSERT( pPathItem->m_pPathfinderPoint->IsWater() || pPathItem->m_pPathfinderPoint->IsCity() );
					pPathResults->m_TransportPickupToDropoffMultiRegionPath.AddTail( iRegion );
				}
				/*
				if( bMovementOnLand )
				{
					ASSERT( pCurrentPathwayItem->m_pPathfinderPoint->IsLand() || pCurrentPathwayItem->m_pPathfinderPoint->IsCity() );
					pPathResults->m_TransportPickupToDropoffPath.AddTail( pCurrentPathwayItem->m_pPathfinderPoint->GetId() );
				}
				else
				{
					ASSERT( pCurrentPathwayItem->m_pPathfinderPoint->IsWater() || pCurrentPathwayItem->m_pPathfinderPoint->IsCity() );
					pPathResults->m_GroundUnitFromDropoffToTargetPath.AddTail( pCurrentPathwayItem->m_pPathfinderPoint->GetId() );
				}
				pCurrentPathwayItem = pTargetToEverywhereMap->GetRegionResult( pCurrentPathwayItem->m_iPreviousPathwayItem );
				*/
			}

			int g=0;
			/*
			CEOSAIRegionPathwayItem* pCurrentPathwayItem = pGroundUnitToPickupRegion;
			while( pCurrentPathwayItem )
			{
				//CEOSAIPathfinderPoint* pPathfinderPoint = pGroundUnitToPickupRegion->m_pPathfinderPoint;
				pPathResults->m_GroundUnitToPickupPath.AddTail( pCurrentPathwayItem->m_pPathfinderPoint );
				pCurrentPathwayItem = pCurrentPathwayItem->m_iPreviousPathwayItem;
			}
			*/
		}
	}

	/*
	// TargetLocation to Everywhere (Pickup Coastal Locations)
	CEOSAIRegionPathwayMap  TargetToEverywhereMap;
	RegionPathfinder.ClearPathfindingSettings();
	//RegionPathfinder.SetPlayer( m_iUnitOwner );
	//RegionPathfinder.SetForeignRelations( &m_OwnerForeignRelations );
	RegionPathfinder.SetMovementRate( &GroundUnitMovementRate );
	RegionPathfinder.ApplyStrategy( m_eNeutralsPathwayStategy, m_eEnemiesPathwayStategy, 1.0f );
	RegionPathfinder.AddStartLocation( pPredefinedStep->m_EndLocation );
	//RegionPathfinder.AddEndLocation( pPredefinedStep->m_EndLocation );
	RegionPathfinder.Solve3();
	RegionPathfinder.GetResult( true, &TargetToEverywhereMap );
	*/

	// Iterate over all the overlapping GroundUnit-Coastal + Transport-Coastal regions (Pickup)
	//   Then setup the Movement Description based on GroundUnit + Transport movement

	return fBestTotalTime;
}

bool  CEOSAIUnit::HasPredefinedPath()
{
	if( GetAIUnitPathwayFinder()->GetPreDefinedPath()->IsEmpty() )
	{
		return false;
	}
	return false;
}

void  CEOSAIUnit::CalculateAIRegionPathFromPredefinedSteps()
{
	CEOSAIUnitPathwayFinder* pPathwayFinder = GetAIUnitPathwayFinder();
	ASSERT( pPathwayFinder );
	//GetAIUnitPathwayFinder();
	//GetAIUnitPathwayFinder()->CalculateAIRegionPathFromPredefinedSteps();
	pPathwayFinder->CalculateAIRegionPathFromPredefinedSteps();
}

void  CEOSAIUnit::UpdateRedPath()
{
	GetAIUnitPathwayFinder()->UpdateRedPath();
}

void  CEOSAIUnit::ConstructFullRedPath()
{
	GetAIUnitPathwayFinder()->ConstructFullRedPath();
}

float CEOSAIUnit::CalculateRedPathArrivalTimeAtGeo( long iTargetGeo )
{
	return GetAIUnitPathwayFinder()->CalculateRedPathArrivalTimeAtGeo( iTargetGeo );
}

float CEOSAIUnit::GetAttackTimeOfTacticalProject( CEOSAITacticalProject2* pTacticalProject2 )
{
	POSITION pos = GetAIUnitPathwayFinder()->GetPreDefinedPath()->GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitPathwayPredefinedStep* pStep = GetAIUnitPathwayFinder()->GetPreDefinedPath()->GetNext( pos );
		if( pStep->IsAnAttack() &&
			pStep->GetAIUnitActionIdea()->GetTacticalProject2() == pTacticalProject2 )
		{
			return pStep->GetRedPathEndTime( CEOSAIUnitPathwayPredefinedStep::enum_EndOfStep );
		}
	}
	return 0.0f;
}

CEOSAIUnit* CEOSAIUnit::GetTransportUsedInTacticalProject( CEOSAITacticalProject2* pTacticalProject2 )
{
	POSITION pos = GetAIUnitPathwayFinder()->GetPreDefinedPath()->GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitPathwayPredefinedStep* pStep = GetAIUnitPathwayFinder()->GetPreDefinedPath()->GetNext( pos );
		if( pStep->GetAIUnitActionIdea() &&
			pStep->GetAIUnitActionIdea()->GetTacticalProject2() == pTacticalProject2 )
		{
			return pStep->GetTransportToUse();
		}
	}
	return NULL;
}

/*
CEOSAIUnitPathwayPredefinedStep* CEOSAIUnit::GetPickupStep( CEOSAIUnitPathwayPredefinedStep* pGroundUnitStep )
{
	ASSERT( pGroundUnitStep->GetAIUnitPathwayFinder()->GetUnitTemplate()->IsGroundUnit() );
	POSITION pos = GetAIUnitPathwayFinder()->GetPreDefinedPath()->GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitPathwayPredefinedStep* pStep = GetAIUnitPathwayFinder()->GetPreDefinedPath()->GetNext( pos );
		if( pStep->IsAPickup() && 
			pStep->GetTransporteeStep() == pGroundUnitStep )
		{
			return pStep;
		}
	}
	return NULL;
}

CEOSAIUnitPathwayPredefinedStep* CEOSAIUnit::GetDropoffStep( CEOSAIUnitPathwayPredefinedStep* pGroundUnitStep )
{
	ASSERT( pGroundUnitStep->GetAIUnitPathwayFinder()->GetUnitTemplate()->IsGroundUnit() );
	POSITION pos = GetAIUnitPathwayFinder()->GetPreDefinedPath()->GetHeadPosition();
	while( pos )
	{
		CEOSAIUnitPathwayPredefinedStep* pStep = GetAIUnitPathwayFinder()->GetPreDefinedPath()->GetNext( pos );
		if( pStep->IsADropoff() && 
			pStep->GetTransporteeStep() == pGroundUnitStep )
		{
			return pStep;
		}
	}
	return NULL;
}
*/
long CEOSAIUnit::GetContainerSizeAlreadyUsed()
{
	long iUsed = 0;
	POSITION pos = m_InitialState.GetContaineesList()->GetHeadPosition();
	//POSITION pos = m_UnitsInsideMe.GetHeadPosition();
	while( pos )
	{
		//CPoiObject* pPoiObject = m_UnitsInsideMe.GetNext( pos )->GetPtr();
		EOSAI::PoiMobile* pPoiMobile = m_InitialState.GetContaineesList()->GetNext( pos );
		iUsed += pPoiMobile->GetMySize();
	}
	return iUsed;
}

bool  CEOSAIUnit::IsInsideAUnitContainer()
{
	CEOSAIPoiObject* pContainer = GetContainer();
	while( pContainer )
	{
		//if( dynamic_cast< CUnit* >( pContainer ) ){ return true; }
		//if( pContainer->GetPoiObjectType2() == EnumPoiObjectType::enum_Unit ){ return true; }
		if( pContainer->GetAIPoiObjectType() == EnumAIPoiObjectType::enum_Unit ){ return true; }
		pContainer = pContainer->GetContainer();
	}
	return false;
}

//bool CEOSAIUnit::CanContain_IgnoreForeignRelations( CEOSAIPoiObject* pAIPoiObject );
bool CEOSAIUnit::CanContain_IgnoreForeignRelations( CEOSAIUnitTemplate* pUnitTemplate )
{
	return m_pAIUnitTemplate->CanContain( pUnitTemplate );
}

void CEOSAIUnit::CalculateUnitPathway()
{
	if (m_pAIUnitPathway)
	{
		delete m_pAIUnitPathway;
		m_pAIUnitPathway = NULL;
	}
	GetRealTransportSpaceAllocations()->SetAIUnit(this);
	m_pAIUnitPathway = CEOSAIUnitPathwayFinder::CreatePathwayFinder(this);
}

CEOSAIAirUnitPathwayFinder*  CEOSAIUnit::GetAIAirUnitPathwayFinder()
{
	return dynamic_cast< CEOSAIAirUnitPathwayFinder*>( m_pAIUnitPathway );
}

CEOSAISeaUnitPathwayFinder*  CEOSAIUnit::GetAISeaUnitPathwayFinder()
{
	return dynamic_cast< CEOSAISeaUnitPathwayFinder*>( m_pAIUnitPathway );
}

CAIGroundUnitPathwayFinder*  CEOSAIUnit::GetAIGroundUnitPathwayFinder()
{
	return dynamic_cast< CAIGroundUnitPathwayFinder*>( m_pAIUnitPathway );
}

void  CEOSAIUnit::AddOnTheWayAndWaitTasksAsPredefinedSteps() // over-ridden from AIPoiObject
{
	//CWorldBuildDesc* pWorldBuildDesc = GetCommonState()->GetWorldBuildDesc();
	if( IsAirUnit() ) return; // skip this step for air units (they can only do one bombing)
	//if( GetPoiObject() == NULL ) return; // skip this step for air units (they can only do one bombing)

	CList< CEOSAIDesireSpatial* >  PotentialOnTheWayDesires;

	// Look for nearby desires
	// g_pAIWorldDistanceTool->GetDistance
	CList< CEOSAIDesireSpatial* >  NearbyDesires;
	g_pEOSAICommonData->GetAIThoughtDatabase( GetOwner() )->GetAllDesiresWithinRadius( GetInitialState()->GetLocation(), 150.0f, &NearbyDesires );
	POSITION pos = NearbyDesires.GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireSpatial* pDesire = NearbyDesires.GetNext( pos );

		CEOSAIRegionPathwayItem* pPathwayItem = InvokeDirectAIRegionMapToEverywhere()->GetRegionResult( pDesire->GetLocation() );
		if( pPathwayItem == NULL ) continue; // no direct path to target

		// Look for easy stuff
		if( ( pDesire->GetType() == EnumCaptureResource || pDesire->GetType() == EnumCaptureCity ) &&
			pDesire->CurrentForeignRelationsPreventsPersuingThisDesire() == false &&
			pDesire->CanFulfillDesire( this->GetAIUnitTemplate() ) )
		{
			// Look for defenses in the area of this target
			float fCombatPower = pDesire->GetEnemyCombatPowerWithinRange( 60.0f );
			if( fCombatPower == 0.0f ){ PotentialOnTheWayDesires.AddTail( pDesire ); }
		}
	}
	// Have a list of desires I might want to do 'on the way'.  Look into adding them to the path.
	bool bHasAPredefinedPath = false;
	CEOSAILocation CurrentLocation = this->GetInitialState()->GetLocation();
	CEOSAILocation NextLocation = this->GetInitialState()->GetLocation();
	CEOSAIPoiObject* pFirstAIPoiObjectTarget = NULL;
	if( m_pAIUnitPathway->GetPreDefinedPath()->IsEmpty() == FALSE )
	{
		CEOSAIUnitPathwayPredefinedStep* pStep = m_pAIUnitPathway->GetPreDefinedPath()->GetHead();
		pFirstAIPoiObjectTarget = pStep->GetAITarget();
		NextLocation = pStep->GetEndLocation();
		bHasAPredefinedPath = true;
	}
	pos = PotentialOnTheWayDesires.GetHeadPosition();
	while( pos )
	{
		CEOSAIDesireSpatial* pDesire = PotentialOnTheWayDesires.GetNext( pos );
		if( pDesire->GetAIPoiObjectTarget() == pFirstAIPoiObjectTarget ) continue; 
		float fOriginalDistance = g_pWorldDistanceTool->GetDistance( CurrentLocation, NextLocation );
		float fNewDistance = g_pWorldDistanceTool->GetDistance( CurrentLocation, pDesire->GetLocation() );
		if( bHasAPredefinedPath )
		{
			fNewDistance += g_pWorldDistanceTool->GetDistance( pDesire->GetLocation(), NextLocation );
		}
		float fDelta = fNewDistance - fOriginalDistance;
		if( ( bHasAPredefinedPath          && fDelta < 50.0f && fOriginalDistance > 60.0f ) ||
			( bHasAPredefinedPath          && fDelta < 20.0f ) ||
			( bHasAPredefinedPath == false && fDelta < 90.0f ) )
		{
			if( pDesire->GetType() == EnumCaptureResource || pDesire->GetType() == EnumCaptureCity )
			{
				// Not really out of my way - do it
				CEOSAIUnitPathwayFinder* pPathwayFinder = GetAIUnitPathwayFinder();
				CEOSAIUnitPathwayPredefinedStep* pNewPredefinedStep = new CEOSAIUnitPathwayPredefinedStep( pPathwayFinder );
				pNewPredefinedStep->SetAIUnitActionIdea( NULL );

				CEOSAIPoiObject* pTargetAIPoiObject = pDesire->GetAIPoiObjectTarget();
				//CEOSAIPoiObject* pTargetAIPoiObject = GetAIBrain()->GetAIPoiObject( pTargetPoiObject->GetObjectId() );
				pNewPredefinedStep->CaptureCitRes( pTargetAIPoiObject );
				pPathwayFinder->InsertAsFirstStep( pNewPredefinedStep );
			}
			else
			{
				ASSERT( false );
			}
		}
	}
}
/*
// If [this] is a Transport
//void CEOSAIUnit::AddTransportAction( EOSAI::UnitPathwayResultStep* pTransporteeStep ) // Support this step with a transport action
void CEOSAIUnit::AddTransportAction( CEOSAIUnitPathwayPredefinedStep* pTransporteeStep ) // Support this step with a transport action
{
	// Step into the AIUnitPathwayFinder, add the TransportAction to the Predefined Steps
	//   Note: I could create a new ResultPathway, test it's value, and then roll-back the result if I wanted
	// Generalize the Predefined-steps sequence optimizer
	// Re-write the entire Movement Results section
	//m_pAIUnitPathway->AddTransportAction( pTransporteeStep );
	m_pAIUnitPathway->InsertTransportSteps( pTransporteeStep );
	//CEOSAIUnitPathwayPredefinedStep* pStep = m_pAIUnitPathway->CreateTransportAction( pTransporteeStep );
	//m_pAIUnitPathway->InsertStep( pStep );
	m_pAIUnitPathway->CalculateResultPath();
	//pTransporteeStep->
}
*/

void CEOSAIUnit::AllocateRepairNearbyTaskOrWarzoneToThisIdleUnit()
{
	ASSERT( GetAIUnitPathwayFinder()->GetPreDefinedPath()->IsEmpty() );
	if( GetAIUnitPathwayFinder()->GetPreDefinedPath()->IsEmpty() == FALSE ) return;

	// Look around for someplace to get repaired
	//CWorldBuildDesc* pWorldBuildDesc = GetCommonState()->GetWorldBuildDesc();
	//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
	CEOSAIRegionPathwayMap* pPathToEverywhere = InvokeDirectAIRegionMapToEverywhere();

	// Possible things to do:
	// (1) Get repaired - if I'm damaged at all
	// (2) Capture something easy (like a resource).  Something nearby.
	// (3) Move into danger (if I'm a combat unit).
	// (4) Move away from danger (if I'm a non-combat unit).

	//bool bThisIsACombatUnit = this->GetUnitTemplate()->GetUnitCombatCapability()->IsACombatUnit();
	bool bThisIsACombatUnit = this->GetAIUnitTemplate()->IsACombatUnit();

	//float fDesireToCaptureANearbyResource01 = 0.0f;
	//float fDesireToGetRepaired01 = 0.0f;
	float fDesireToFulfillDesire01 = 0.0f;
	CEOSAIDesireSpatial* pBestDesireSpatial = NULL;
	float fDesireToMoveToWarzone01 = 0.0f;
	CEOSAIRegion2* pTargetAIRegionWarzoneLocation = NULL;
	//CEOSAIWarzoneLocation* pTargetAIRegionWarzoneLocation = NULL;
	//float fDesireToMoveIntoOrAwayFromDanger01 = 0.0f;

	// Look for a nearby desire to fulfill
	float fRangeCheck = 200.0f;
	if( this->IsGroundUnit() ){ fRangeCheck = 150.0f; }

	// If there is a nearby desire I can fulfill, do it
	bool bTaskAdded = false;
	//CEOSAIThoughtDatabase* pAIThoughtDatabase = m_pAIBrain->GetAIThoughtDatabase();
	CEOSAIThoughtDatabase* pAIThoughtDatabase = g_pEOSAICommonData->GetAIThoughtDatabase( this->GetOwner() );
	POSITION pos2 = pAIThoughtDatabase->GetDesiresSortedByScore()->GetHeadPosition();
	while( pos2 )
	{
		CEOSAIDesireSpatial* pDesireSpatial = pAIThoughtDatabase->GetDesiresSortedByScore()->GetNext( pos2 );
		if( pDesireSpatial->CurrentForeignRelationsPreventsPersuingThisDesire() ) continue;

		float fDistance = g_pWorldDistanceTool->GetDistance( this->GetInitialState()->GetLocation(), pDesireSpatial->GetLocation() );
		if( fDistance < fRangeCheck )
		{
			CEOSAIRegionPathwayMap*  pPathMap  = this->InvokeDirectAIRegionMapToEverywhere();
			CEOSAIRegionPathwayItem* pPathItem = pPathMap->GetRegionResult( pDesireSpatial->GetLocation() );
			if( pPathItem &&
				pPathItem->m_fDistance < fRangeCheck &&
				this->IsWithinRange( pDesireSpatial->GetLocation() ) &&
				pDesireSpatial->CanFulfillDesire( this->GetAIUnitTemplate() ) )
			{
				if( bThisIsACombatUnit ||
				  ( bThisIsACombatUnit == false && pPathItem->m_fDanger == 0.0f ) )
				{
					/*
					CEOSAIUnitActionIdea* pAIUnitActionIdea = new CEOSAIUnitActionIdea( NULL, pAIUnit );
					pAIUnitActionIdea->AddDesireAllocation( pDesireSpatial );
					pAIUnitActionIdea->CreatePredefinedStep();
					*/
					fDesireToFulfillDesire01 = 0.1f*pDesireSpatial->GetSimpleInterest() * 100.0f/(fDistance+100.0f);
					pBestDesireSpatial = pDesireSpatial;
					bTaskAdded = true;
				}
			}
		}
		if( bTaskAdded ) break;
	}

	// Now, find prefered Warzones for each idle unit
	if( bThisIsACombatUnit && this->IsSeaUnit() )
	{
		// Calculate distance to each of the warzones
		CEOSAIRegionPathwayMap* pPathwayMap = this->InvokeDirectAIRegionMapToEverywhere();

		//CWorldDescServer* pWorldDescServer = g_pCommonState->GetWorldDescServer();
		float fValueOfBestAIRegion = 0.0f;
		CEOSAIRegion2* pBestAIRegion = NULL;
		CEOSAIWarzoneLocation* pClosestWarzone = NULL;
		float fBestAIRegionDistance = 1000000.0f;
		POSITION pos2 = pAIThoughtDatabase->GetWarzoneLocations()->GetHeadPosition();
		while( pos2 )
		{
			float fBestDistanceFromCenterOfWarzone = 1000000.0f;
			CEOSAIWarzoneLocation* pWarzoneLocation = pAIThoughtDatabase->GetWarzoneLocations()->GetNext( pos2 );
			POSITION pos3 = pWarzoneLocation->m_AIRegions.GetHeadPosition();
			while( pos3 )
			{
				long iAIRegion = pWarzoneLocation->m_AIRegions.GetNext( pos3 );
				CEOSAIRegionPathwayItem* pRegionPathwayItem = pPathwayMap->GetRegionResult( iAIRegion );
				if( pRegionPathwayItem )
				{
					CEOSAIRegion2* pAIRegion = (CEOSAIRegion2*) pRegionPathwayItem->m_pPathfinderPoint;
					float fPathfindingValue = pRegionPathwayItem->m_fPathfindingValue;
					float fDistanceFromCenterOfWarzone = g_pWorldDistanceTool->GetDistance( pWarzoneLocation->m_Location, pAIRegion->GetLocation() );

					ASSERT( false ); // I should use the AIRegions to calculate this
					/*
					I should use the AIRegions to calculate this
					COwnershipPlayerMap* pOwnershipPlayerMap = g_pEOSAICommonData->GetCurrentOwnershipPlayerMap8();
					long iLocationOwnership = pOwnershipPlayerMap->GetOwner( pAIRegion->m_Location.GetRealX(), pAIRegion->m_Location.GetRealY() );
					EOSAIEnumForeignRelations eRel = g_pEOSAICommonData->GetForeignRelations( GetOwner(), iLocationOwnership );
					if( iLocationOwnership == 0 || eRel.IsNeutral() == false )
					{
						if( fDistanceFromCenterOfWarzone < 100.0f ){ fDistanceFromCenterOfWarzone = 100.0f; }
						if( pAIRegion->IsCity() == false && 
							fPathfindingValue < fBestAIRegionDistance &&
							fDistanceFromCenterOfWarzone <= fBestDistanceFromCenterOfWarzone )
						{
							fBestAIRegionDistance = fPathfindingValue;
							fBestDistanceFromCenterOfWarzone = fDistanceFromCenterOfWarzone;
							pClosestWarzone = pWarzoneLocation;
							pBestAIRegion = pAIRegion;
						}
					}
					*/
				}
			}
		}
		if( pBestAIRegion )
		{
			fDesireToMoveToWarzone01 = 0.1f;
			pTargetAIRegionWarzoneLocation = pBestAIRegion;
			//CEOSAILocation TargetLocation = pBestAIRegion->GetLocation();
			//pAIUnit->GetAIUnitPathwayFinder()->AddWaypoint( TargetLocation );
		}
	}

	// DEBUG-START
	//fDesireToCaptureANearbyResource01 = 0.0f;
	//fDesireToGetRepaired01 = 0.0f;
	// DEBUG-END

	// Task: 0 = None, 1 = Capture Resource, 2 = Repair, 3 = Move Into/Away from Danger
	long iTask = 0;
	float fTask = 0.0f;
	if( m_fNeedForRepair01BasedOnDistanceAndDamage > fTask )
	{
		iTask = 2;
		fTask = m_fNeedForRepair01BasedOnDistanceAndDamage;
	}
	/*
	if( fDesireToCaptureANearbyResource01 > fTask )
	{
		iTask = 1;
		fTask = fDesireToCaptureANearbyResource01;
	}
	*/
	if( fDesireToFulfillDesire01 > fTask )
	{
		iTask = 3;
		fTask = fDesireToFulfillDesire01;
	}
	if( fDesireToMoveToWarzone01 > fTask )
	{
		iTask = 4;
		fTask = fDesireToMoveToWarzone01;
	}
	/*
	if( fDesireToMoveIntoOrAwayFromDanger01 > fTask )
	{
		iTask = 3;
		fTask = fDesireToMoveIntoOrAwayFromDanger01;
	}
	*/
/*
	if( iTask == 1 ) // Best Task: Capture a nearby resource
	{
		ASSERT( pBestTargetAIResource );
		if( pBestTargetAIResource )
		{
			CEOSAIUnitPathwayPredefinedStep* pStep =
				GetAIUnitPathwayFinder()->CreateCaptureTarget( pBestTargetAIResource );
			GetAIUnitPathwayFinder()->AppendStep( pStep );
			//GetAIUnitPathwayFinder()->CalculateResultPath();
			//GetAIUnitPathwayFinder()->CreateOrders();
		}
	}
	*/
	if( iTask == 2 ) // Best Task: Get repaired
	{
		float fBestRepairLocationDistance = 0.0f;
		CEOSAIPoiObject* pBestRepairLocation = NULL;
		//m_pAIBrain->GetAIThoughtDatabase()->GetClosestRepairLocation( this, &pBestRepairLocation, &fBestRepairLocationDistance );
		g_pEOSAICommonData->GetAIThoughtDatabase( GetOwner() )->GetClosestRepairLocation( this, &pBestRepairLocation, &fBestRepairLocationDistance );

		ASSERT( pBestRepairLocation );
		if( pBestRepairLocation )
		{
			if( this->GetInitialState()->GetContainer() == pBestRepairLocation )
			{
				// Do nothing - I am already inside the city
			}
			else
			{
				GetAIUnitPathwayFinder()->AddRepairTask( pBestRepairLocation );
				//GetAIUnitPathwayFinder()->CalculateResultPath();
				//GetAIUnitPathwayFinder()->CreateOrders();
			}
		}
	}
	if( iTask == 3 ) // Spatial Desire
	{
		ASSERT( pBestDesireSpatial );
		if( pBestDesireSpatial )
		{
			CEOSAIUnitActionIdea* pAIUnitActionIdea = new CEOSAIUnitActionIdea();
			pAIUnitActionIdea->SetActor( this );
			pAIUnitActionIdea->AddDesireAllocation( pBestDesireSpatial );
			pAIUnitActionIdea->CreatePredefinedStep();
		}
	}
	if( iTask == 4 ) // Move to warzone
	{
		ASSERT( this->GetAIUnitTemplate()->IsACombatUnit() );
		ASSERT( pTargetAIRegionWarzoneLocation );
		if( pTargetAIRegionWarzoneLocation )
		{
			float fDistance = g_pWorldDistanceTool->GetDistance( this->GetInitialState()->GetLocation(), pTargetAIRegionWarzoneLocation->GetLocation() );
			if( fDistance > 4.0f )
			{
				GetAIUnitPathwayFinder()->AddWaypoint( pTargetAIRegionWarzoneLocation->GetLocation() );
			}
		}
		/*
		if( pPreferedMultiRegion )
		{
			GetAIUnitPathwayFinder()->AddWaypoint( pPreferedMultiRegion->GetLocation() );
			//GetAIUnitPathwayFinder()->CalculateResultPath();
			//GetAIUnitPathwayFinder()->CreateOrders();
		}
		*/
	}
}

float CEOSAIUnit::GetProductionAndConsumptionPerTurnAndMove( CString strResource )
{
	return
		GetProductionAndConsumptionPerTurn( strResource ) +
		GetProductionAndConsumptionPerMove( strResource );
}

float CEOSAIUnit::GetProductionAndConsumptionPerTurn( CString strResource )
{
	return m_pAIUnitTemplate->GetProductionAndConsumptionPerTurn( strResource );
}

float CEOSAIUnit::GetProductionAndConsumptionPerMove( CString strResource )
{
	return m_pAIUnitTemplate->GetProductionAndConsumptionPerMove( strResource );
}

float CEOSAIUnit::GetCurrentDefense()
{
	if( IsGroundUnit() )
	{
		ASSERT( m_iCurrentTerrain != -1 );
		//ASSERT( m_bIsEntrenched != false );
		//ASSERT( m_bIsInsideFortification != false );
		return m_pAIUnitTemplate->GetCurrentDefense( m_iCurrentTerrain, m_bIsEntrenched, m_bIsInsideFortification, false );
		/*
		if( IsMovingAtThisMoment() )
		{
			long iTerrain = GetNormalTerrain();
			return m_pUnitTemplate->GetCurrentDefense( iTerrain, IsEntrenched(),IsInsideFortification(), false );
		}
		else
		{
			long iTerrain = GetWorldDesc()->GetActualTerrainMap().GridValue( GetLocation().GetGridX(), GetLocation().GetGridY() );
			return m_pUnitTemplate->GetCurrentDefense( iTerrain, IsEntrenched(),IsInsideFortification(), false );
		}
		*/
	}
	else
	{
		return m_pAIUnitTemplate->GetCurrentDefense( 0,false,false, IsLandedOrDocked() );
	}
}

float CEOSAIUnit::GetDefenseValueForNormalTerrain()
{
	return m_pAIUnitTemplate->GetDefenseValueForNormalTerrain();
}

bool CEOSAIUnit::IsLandedOrDocked()
{
	//CPoi* pParent = this->GetContainer();
	CEOSAIPoiObject* pParent = m_InitialState.GetContainer();
	while( pParent )
	{
		//if( dynamic_cast< CUnit* >( pParent ) ||
		//	dynamic_cast< CAirfield* >( pParent ) ||
		//	dynamic_cast< CFortification* >( pParent ) ||
		//	dynamic_cast< CCity* >( pParent ) )
		EnumAIPoiObjectType ePoiObjectType = pParent->GetAIPoiObjectType();
		if( ePoiObjectType == EnumAIPoiObjectType::enum_Unit ||
			ePoiObjectType == EnumAIPoiObjectType::enum_City ||
			ePoiObjectType == EnumAIPoiObjectType::enum_Airfield ||
			ePoiObjectType == EnumAIPoiObjectType::enum_Fortification )
		{
			return true;
		}
		pParent = pParent->GetContainer();
	}

	// If sea unit is inside city terrain, then it will also be considered to be landed or docked
	//   There are times when sea-units aren't inside a city container, but are still on city terrain
	/*
	if( this->IsSeaUnit() )
	{
		CWorldDesc* pWorldDesc = dynamic_cast< CWorldDesc* >( GetWorldDesc() );
		if( pWorldDesc && pWorldDesc->GetWorldBuildDesc()->IsValidGridLocation( GetLocation().GetGridX(), GetLocation().GetGridY() ) )
		{
			char cTerrain = pWorldDesc->GetActualTerrainType( this->GetLocation() );// == TERRAIN_CITY{ return m_ActualTerrainMap.GridValue( location.GetGridX(), location.GetGridY() ); }
			if( cTerrain == TERRAIN_CITY )
			{
				return true;
			}
		}
	}
	*/

	return false;
}