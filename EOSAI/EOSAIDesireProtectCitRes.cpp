
#include "stdafx.h"
//#include "WorldDescServer.h"
#include "EOSAIBrain.h"
#include "EOSAIDesireProtectCitRes.h"
#include "City.h"
#include "EOSAIResource.h"
#include "AIPlayer.h"
#include "EOSAIUnit2.h"
#include "EOSAICommonData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CAIDesireProtectCitRes::SetTarget( CEOSAIPoiObject* pAITarget )
{
	CEOSAICity* pTargetAICity = dynamic_cast< CEOSAICity* >( pAITarget );
	CEOSAIResource* pTargetAIResource = dynamic_cast< CEOSAIResource* >( pAITarget );

	ASSERT( pTargetAICity || pTargetAIResource );
	if( pTargetAICity || pTargetAIResource )
	{
		m_pAITarget = pAITarget;
	}
}

float CAIDesireProtectCitRes::GetSimpleInterest()
{
	ASSERT( m_pAIBrain );
	long iAIPlayer = m_pAIBrain->GetAIPlayerNumber();

	CEOSAICity* pTargetAICity = dynamic_cast< CEOSAICity* >( m_pAITarget );
	CEOSAIResource* pTargetAIResource = dynamic_cast< CEOSAIResource* >( m_pAITarget );

	float fScore = 0.0f;
	ASSERT( pTargetAICity || pTargetAIResource );
	if( pTargetAICity || pTargetAIResource )
	{
		// Slightly higher value for enemy CitRes - because he is using it against me
		float fMult = 1.0f;
		//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
		if( m_pAITarget->GetOwner() == 0 )
		{
			fMult = 1.0f;
		}
		else if( g_pEOSAICommonData->GetForeignRelations( iAIPlayer, m_pAITarget->GetOwner() ).IsEnemy() )
		{
			fMult = 1.3f;
		}

		if( pTargetAICity )
		{
			fScore = fMult * pTargetAICity->GetTotalProduction();
		}
		if( pTargetAIResource )
		{
			//fScore = fMult * pTargetResource->GetAIValue( m_pAIBrain );
			fScore = 
				fMult * 
				pTargetAIResource->GetResourcePerTurn() * 
				m_pAIBrain->GetAIPlayer()->GetCitResValue_ResourceMultiplier( pTargetAIResource->GetResourceType() );
		}
	}
	return fScore;
}

//float CEOSAIDesireCaptureCitRes::GetExpectedEffort(){ return 0.0f; }

bool CAIDesireProtectCitRes::CanFulfillDesire( CEOSAIPoiObject* pActor ) // CityHunters / LandResHunters / SeaResHunters
{
	CEOSAICity* pActorCity = dynamic_cast< CEOSAICity* >( pActor );
	if( pActorCity ){ return true; }

	CEOSAIUnit* pActorUnit = dynamic_cast< CEOSAIUnit* >( pActor );
	if( pActorUnit ){ return CanFulfillDesire( pActorUnit->GetAIUnitTemplate() ); }

	return false;
}

bool CAIDesireProtectCitRes::CanFulfillDesire( CEOSAIUnitTemplate* pActor )
{
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
	return false;
}

bool  CAIDesireProtectCitRes::TargetIsACity()
{
	CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( m_pAITarget );
	return pAICity != NULL;
}

bool  CAIDesireProtectCitRes::TargetIsALandResource()
{
	CEOSAIResource* pTargetAIRes = dynamic_cast< CEOSAIResource* >( m_pAITarget );
	if( pTargetAIRes )
	{
		return pTargetAIRes->ResourceSource_IsOnLand();
		//return pTargetRes->IsOnLand3();
	}
	return false;
}

bool  CAIDesireProtectCitRes::TargetIsASeaResource()
{
	CEOSAIResource* pTargetAIRes = dynamic_cast< CEOSAIResource* >( m_pAITarget );
	if( pTargetAIRes )
	{
		return pTargetAIRes->ResourceSource_IsOnLand() == false;
		//return pTargetRes->IsOnWater3();
	}
	return false;
}


