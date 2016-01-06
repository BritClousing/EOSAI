
#include "stdafx.h"
//#include "WorldDescServer.h"
#include "EOSAIBrain.h"
#include "EOSAIDesireDestroyAirfield.h"
//#include "City.h"
//#include "ResourceSource.h"
//#include "Unit.h"
#include "EOSAIAirfield.h"
#include "City.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CAIDesireDestroyAirfield::CAIDesireDestroyAirfield()
{
}

void CAIDesireDestroyAirfield::SetTarget( CEOSAIPoiObject* pAITarget )
{
	CEOSAIAirfield* pTargetAIAirfield = dynamic_cast< CEOSAIAirfield* >( pAITarget );

	IsAggressiveAgainstPlayer( pAITarget->GetOwner() );

	ASSERT( m_pAITarget == NULL );
	ASSERT( pTargetAIAirfield );
	if( pTargetAIAirfield )
	{
		m_pAITarget = pAITarget;
		//m_Jobs.m_bSeaResHunter = true;
	}
}

float CAIDesireDestroyAirfield::GetSimpleInterest()
{
	ASSERT( m_pAIBrain );
	long iAIPlayer = m_pAIBrain->GetAIPlayerNumber();

	CEOSAIAirfield* pTargetAIAirfield = dynamic_cast< CEOSAIAirfield* >( m_pAITarget );

	float fScore = 0.0f;
	ASSERT( pTargetAIAirfield );
	if( pTargetAIAirfield )
	{
		/*
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
		*/
	}
	return fScore;
}

bool CAIDesireDestroyAirfield::CanFulfillDesire( CEOSAIPoiObject* pActor ) // CityHunters / LandResHunters / SeaResHunters
{
	CEOSAICity* pActorCity = dynamic_cast< CEOSAICity* >( pActor );
	if( pActorCity ){ return true; }

	CEOSAIUnit* pActorUnit = dynamic_cast< CEOSAIUnit* >( pActor );
	if( pActorUnit ){ return CanFulfillDesire( pActorUnit->GetAIUnitTemplate() ); }

	return false;
}

bool CAIDesireDestroyAirfield::CanFulfillDesire( CEOSAIUnitTemplate* pActor )
{
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
	ASSERT( false );
	return false;
}
