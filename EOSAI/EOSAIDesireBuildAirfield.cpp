
#include "stdafx.h"
//#include "WorldDescServer.h"
#include "EOSAIBrain.h"
#include "EOSAIDesireBuildAirfield.h"
#include "City.h"
#include "EOSAIResource.h"
#include "EOSAIUnit2.h"
#include "EOSAIAirfield.h"

CAIDesireBuildAirfield::CAIDesireBuildAirfield()
{
	m_JobsToDo.m_iAirfieldBuilder = 1;
}
/*
void CAIDesireBuildAirfield::SetTarget( CPoiObject* pTarget )
{
	CAirfield* pTargetAirfield = dynamic_cast< CAirfield* >( pTarget );
	CResourceSource* pTargetResource = dynamic_cast< CResourceSource* >( pTarget );

	ASSERT( m_pTarget == NULL );
	ASSERT( pTargetCity || pTargetResource );
	if( pTargetCity || pTargetResource )
	{
		m_pTarget = pTarget;

		if( TargetIsACity() || TargetIsALandResource() )
		{
			m_Jobs.m_bGroundCitResHunter = true;
		}
		if( TargetIsASeaResource() )
		{
			m_Jobs.m_bSeaResHunter = true;
		}
	}
}
*/
float CAIDesireBuildAirfield::GetSimpleInterest()
{
	ASSERT( m_pAIBrain );
	long iAIPlayer = m_pAIBrain->GetAIPlayerNumber();

	return 0.0f;
	/*
	CAirfield* pTargetAirfield = dynamic_cast< CAirfield* >( m_pTarget );

	float fScore = 0.0f;
	ASSERT( pTargetAirfield );
	if( pTargetAirfield )
	{
		/-*
		// Slightly higher value for enemy CitRes - because he is using it against me
		float fMult = 1.0f;
		CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
		if( m_pTarget->GetOwner() == 0 )
		{
			fMult = 1.0f;
		}
		else if( pWorldDescServer->GetForeignRelations( iAIPlayer, m_pTarget->GetOwner() ).IsEnemy() )
		{
			fMult = 1.3f;
		}

		if( pTargetCity )
		{
			fScore = fMult * pTargetCity->GetTotalProduction();
		}
		if( pTargetResource )
		{
			fScore = fMult * pTargetResource->GetAIValue( m_pAIBrain->GetCitResValueMultiplier() );
		}
		*-/
	}
	return fScore;
	*/
}

bool CAIDesireBuildAirfield::CanFulfillDesire( CEOSAIPoiObject* pActor ) // CityHunters / LandResHunters / SeaResHunters
{
	CEOSAICity* pActorAICity = dynamic_cast< CEOSAICity* >( pActor );
	if( pActorAICity ){ return true; }

	CEOSAIUnit2* pActorAIUnit = dynamic_cast< CEOSAIUnit2* >( pActor );
	if( pActorAIUnit ){ return CanFulfillDesire( pActorAIUnit->GetAIUnitTemplate() ); }

	return false;
}

bool CAIDesireBuildAirfield::CanFulfillDesire( CEOSAIUnitTemplate* pAIActor )
{
	return pAIActor->CanBuildAirfields();
}
