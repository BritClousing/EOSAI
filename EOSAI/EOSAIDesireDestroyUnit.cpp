
#include "stdafx.h"
//#include "WorldDescServer.h"
#include "EOSAIBrain.h"
#include "EOSAIDesireDestroyUnit.h"
#include "City.h"
#include "EOSAIResource.h"
#include "EOSAIUnit2.h"
//#include "CombatManager.h"
#include "EOSAICommonData2.h"
#include "EOSAIMultiRegionNationwidePathways.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CAIDesireDestroyUnit::CAIDesireDestroyUnit()
{
	
}

void CAIDesireDestroyUnit::SetTarget( CEOSAIPoiObject* pAITarget )
{
	pAITarget->AddAIDesire( this );

	IsAggressiveAgainstPlayer( pAITarget->GetOwner() );

	m_JobsToDo.m_EnemyUnits.RemoveAll();

	CEOSAIUnit2* pTargetAIUnit = dynamic_cast< CEOSAIUnit2* >( pAITarget );
	ASSERT( pTargetAIUnit );
	if( pTargetAIUnit )
	{
		m_pAITarget = pAITarget;
		m_JobsToDo.m_EnemyUnits.AddTail( pTargetAIUnit );
	}
}

CString CAIDesireDestroyUnit::Dump()
{
	CString str, str2;

	CEOSAIUnit2* pAIUnitTarget = dynamic_cast< CEOSAIUnit2* >( m_pAITarget );
	ASSERT( pAIUnitTarget );
	if( pAIUnitTarget )
	{
		CEOSAIUnitTemplate* pUnitTemplate = pAIUnitTarget->GetAIUnitTemplate();
		ASSERT( pUnitTemplate );

		str.Format( _T("DestroyUnit %s ObjectId:%d  Owner:%d\r\n"), 
			pUnitTemplate->GetInternalName(), pAIUnitTarget->GetObjectId(), pAIUnitTarget->GetOwner() );
	}
	str2.Format( _T("  Simple Interest: %0.1f\r\n"), GetSimpleInterest() ); str += str2;
	str2.Format( _T("  Area Simple Interest: %0.1f\r\n"), GetAreaSimpleInterest() ); str += str2;
	str2.Format( _T("  Accessibility01: %0.2f\r\n"), GetAccessibility01() ); str += str2;
	str2.Format( _T("  Score: %0.1f\r\n"), GetScore() ); str += str2;
	return str;
}

bool  CAIDesireDestroyUnit::InvolvesGroundTargets()
{
	return TargetIsAnGroundUnit();
}

float CAIDesireDestroyUnit::GetSimpleInterest()
{
	ASSERT( m_pAIBrain );
	long iAIPlayer = m_pAIBrain->GetAIPlayerNumber();

	CEOSAIUnit2* pTargetAIUnit = dynamic_cast< CEOSAIUnit2* >( m_pAITarget );

	float fScore = 0.0f;
	ASSERT( pTargetAIUnit );
	if( pTargetAIUnit )
	{
		if( m_pAITarget->GetOwner() == 0 )
		{
			return 0.0f; // destroying militia isn't important as a goal in itself
		}

		// Slightly higher value for enemy CitRes - because he is using it against me
		float fEnemyMult = 0.0f;
		//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
		EOSAIEnumForeignRelations eRel = g_pEOSAICommonData->GetForeignRelations( iAIPlayer, m_pAITarget->GetOwner() );
		if( eRel.IsTeammate() )
		{
			fEnemyMult = 0.0f;
		}
		eif( eRel.IsAlly() )
		{
			fEnemyMult = 0.1f;
		}
		eif( eRel.IsNeutral() )
		{
			fEnemyMult = 0.5f;
		}
		eif( eRel.IsEnemy() )
		{
			fEnemyMult = 1.0f;
		}

		// Inside Owned area of the map?
		float fAIRegionOwnershipMult = 1.0f;
		CEOSAIRegionManager2* pAIRegionManager = g_pEOSAICommonData->GetAIRegionManager();
		//CEOSAIRegion2* pAIRegion = pAIRegionManager->GetAIRegion( m_pAITarget->GetInitialState()->GetAIRegion() );
		CEOSAIRegion2* pAIRegion = m_pAITarget->GetInitialState()->GetAIRegion();
		float fOwnerControl01 = pAIRegion->m_fSingleOwnerControl;
		if( pAIRegion->m_iSingleOwner == iAIPlayer )
		{
			fAIRegionOwnershipMult += 0.5f;
		}
		CEOSAIMultiRegionNationwidePathways* pPathway = g_pEOSAICommonData->GetNationwidePathways( iAIPlayer );
		CEOSAIRegionPathwayItem* pMultiRegionItem = pPathway->GetDistGeoPathfindingValueFromMyCitRes( m_pAITarget->GetInitialState()->GetMultiRegion()->GetId() );
		ASSERT( pMultiRegionItem );
		if( pMultiRegionItem )
		{
			if( pMultiRegionItem->m_fDistance < 200.0f ){ fAIRegionOwnershipMult += 0.3f; }
			if( pMultiRegionItem->m_fDistance < 400.0f ){ fAIRegionOwnershipMult += 0.3f; }
		}

		// Easy targets get higher scores
		//   INCOMPLETE: I need to normalize this information - it will change based on the unitset
		//   So, I need to calculate a lot of relative information
		//   Maybe I need to figure out the best-case 1.0 city production = how much AIPower?
		fScore = 
			fEnemyMult * 
			fAIRegionOwnershipMult *
			pTargetAIUnit->GetAIUnitTemplate()->GetProductionAndIronCost1() / (pTargetAIUnit->GetAIPowerValue()+10.0f);
	}
	return fScore;
}

//float CEOSAIDesireCaptureCitRes::GetExpectedEffort(){ return 0.0f; }

bool CAIDesireDestroyUnit::CanFulfillDesire( CEOSAIPoiObject* pActor ) // CityHunters / LandResHunters / SeaResHunters
{
	CEOSAICity* pActorCity = dynamic_cast< CEOSAICity* >( pActor );
	if( pActorCity ){ return true; }

	CEOSAIUnit2* pActorUnit = dynamic_cast< CEOSAIUnit2* >( pActor );
	if( pActorUnit ){ return CanFulfillDesire( pActorUnit->GetAIUnitTemplate() ); }

	return false;
}

bool CAIDesireDestroyUnit::CanFulfillDesire( CEOSAIUnitTemplate* pActor )
{
	// If the actor has any combat ability against the target, then return true
	CEOSAIAttackVs2* pAttackVs = pActor->GetAttackVs( m_pAITarget );
	if( pAttackVs )
	{
		return pAttackVs->CanTarget();
	}

	/*
	if( TargetIsACity() )
	{
		return pActor->IsCityHunter();
	}
	if( TargetIsALandResource() )
	{
		return pActor->IsGroundCitResHunter();
	}
	if( TargetIsASeaResource() )
	{
		return pActor->IsSeaResHunter();
	}
	*/
	return false;
}

float CAIDesireDestroyUnit::GetUnitJobValue01( CEOSAIUnitTemplate* pActor )
{
	// This should be the combat value of the unit against the actor
	// Attrition matters since I want the best unit for the job, but so does the total combat value
	ASSERT( false );
	return 1.0f;
	//return GetCombatAttritionValue( pActor );
}
/*
float CAIDesireDestroyUnit::GetCombatAttritionValue( CUnitTemplate* pActor )
{
	ASSERT( m_pAITarget );
	CEOSAIUnit2* pTargetAIUnit = dynamic_cast< CEOSAIUnit2* >( m_pAITarget );
	ASSERT( pTargetAIUnit );
	if( pTargetAIUnit )
	{
		// Get the attrition value
		CAttritionBalanceResult  BalanceResult;
		CCombatManager::GetAttritionBalance(
			CUnitAndCombatState( pTargetAIUnit->GetUnitTemplate() ), CUnitAndCombatState( pActor ), true, &BalanceResult );
		return BalanceResult.m_fAttritionBalance;
	}
	return 0.0f;
}
*/
bool  CAIDesireDestroyUnit::TargetIsAnAirUnit()
{
	CEOSAIUnit2* pActorAIUnit = dynamic_cast< CEOSAIUnit2* >( m_pAITarget );
	if( pActorAIUnit )
	{
		pActorAIUnit->IsAirUnit();
	}
	return false;
}

bool  CAIDesireDestroyUnit::TargetIsAnGroundUnit()
{
	CEOSAIUnit2* pActorAIUnit = dynamic_cast< CEOSAIUnit2* >( m_pAITarget );
	if( pActorAIUnit )
	{
		pActorAIUnit->IsGroundUnit();
	}
	return false;
}

bool  CAIDesireDestroyUnit::TargetIsAnSeaUnit()
{
	CEOSAIUnit2* pActorAIUnit = dynamic_cast< CEOSAIUnit2* >( m_pAITarget );
	if( pActorAIUnit )
	{
		pActorAIUnit->IsSeaUnit();
	}
	return false;
}

