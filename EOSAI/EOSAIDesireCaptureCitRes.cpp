
#include "stdafx.h"
//#include "WorldDescServer.h"
#include "EOSAIBrain.h"
#include "EOSAIDesireCaptureCitRes.h"
#include "City.h"
#include "EOSAIResource.h"
#include "EOSAIUnit2.h"
#include "City.h"
#include "EOSAIResource.h"
#include "AIPlayer.h"
#include "EOSAICommonData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CEOSAIDesireCaptureCitRes::CEOSAIDesireCaptureCitRes()
{
}

void CEOSAIDesireCaptureCitRes::SetTarget( CEOSAIPoiObject* pAITarget )
{
	pAITarget->AddAIDesire( this );

	CEOSAICity* pTargetAICity = dynamic_cast< CEOSAICity* >( pAITarget );
	CEOSAIResource* pTargetAIResource = dynamic_cast< CEOSAIResource* >( pAITarget );

	IsAggressiveAgainstPlayer( pAITarget->GetOwner() );

	ASSERT( m_pAITarget == NULL );
	ASSERT( pTargetAICity || pTargetAIResource );
	if( pTargetAICity || pTargetAIResource )
	{
		m_pAITarget = pAITarget;

		if( TargetIsACity() || TargetIsALandResource() )
		{
			m_JobsToDo.m_fGroundCitResHunter = 1.0f;
		}
		if( TargetIsASeaResource() )
		{
			m_JobsToDo.m_fSeaResHunter = 1.0f;
		}
	}
}

CString CEOSAIDesireCaptureCitRes::Dump()
{
	CString str, str2;
	if( dynamic_cast< CEOSAICity* >( GetAIPoiObjectTarget() ) )
	{
		CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( GetAIPoiObjectTarget() );
		str.Format( _T("CaptureCity %s Prod:%1.0f  ObjectId:%d  Owner:%d\r\n"), 
			pAICity->GetName(), pAICity->GetTotalProduction( false,false ), GetAIPoiObjectTarget()->GetObjectId(), GetAIPoiObjectTarget()->GetOwner() );
	}
	if( dynamic_cast< CEOSAIResource* >( GetAIPoiObjectTarget() ) )
	{
		CEOSAIResource* pAIRes = dynamic_cast< CEOSAIResource* >( GetAIPoiObjectTarget() );
		str.Format( _T("CaptureRes %s Prod:%d  ObjectId %d  Owner:%d\r\n"), 
			pAIRes->GetResourceType(), pAIRes->GetResourcePerTurn(), GetAIPoiObjectTarget()->GetObjectId(), GetAIPoiObjectTarget()->GetOwner() );
	}
	str2.Format( _T("  Simple Interest: %0.1f\r\n"), GetSimpleInterest() ); str += str2;
	str2.Format( _T("  Area Simple Interest: %0.1f\r\n"), GetAreaSimpleInterest() ); str += str2;
	str2.Format( _T("  Accessibility01: %0.2f\r\n"), GetAccessibility01() ); str += str2;
	str2.Format( _T("  Score: %0.1f\r\n"), GetScore() ); str += str2;
	return str;
}

EOSAIEnumAIDesireType  CEOSAIDesireCaptureCitRes::GetType()
{
	if( dynamic_cast< CEOSAICity* >( m_pAITarget ) )
	{
		return EnumCaptureCity;
	}
	if( dynamic_cast< CEOSAIResource* >( m_pAITarget ) )
	{
		return EnumCaptureResource;
	}
	ASSERT( false );
	return EOSAIEnumAIDesireType_Undefined;
}

bool  CEOSAIDesireCaptureCitRes::InvolvesGroundTargets()
{
	return ( GetMultiRegion()->GetGeo()->GetGeoType() == CEOSAIGeo::Land );
}

float CEOSAIDesireCaptureCitRes::GetSimpleInterest()
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
		float fForeignRelationsBasedMultiplier = 1.0f;
		//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
		if( m_pAITarget->GetOwner() == 0 )
		{
			fForeignRelationsBasedMultiplier = 1.0f;
		}
		else if( g_pEOSAICommonData->GetForeignRelations( iAIPlayer, m_pAITarget->GetOwner() ).IsEnemy() )
		{
			fForeignRelationsBasedMultiplier = 1.3f;
		}
		else if( g_pEOSAICommonData->GetForeignRelations( iAIPlayer, m_pAITarget->GetOwner() ).IsTeammate() )
		{
			fForeignRelationsBasedMultiplier = 0.0f;
		}

		if( pTargetAICity )
		{
			// DEBUG
			float f1 = m_pAIBrain->GetAIPlayer()->GetCitResValue_CityMultiplier();
			float f2 = pTargetAICity->GetTotalProduction();

			fScore = 
				fForeignRelationsBasedMultiplier * 
				m_pAIBrain->GetAIPlayer()->GetCitResValue_CityMultiplier() *
				pTargetAICity->GetTotalProduction();
		}
		if( pTargetAIResource )
		{
			fScore = 
				fForeignRelationsBasedMultiplier * 
				m_pAIBrain->GetAIPlayer()->GetCitResValue_ResourceMultiplier( pTargetAIResource->GetResourceType() ) *
				pTargetAIResource->GetResourcePerTurn();
		}
	}
	return fScore;
}

//float CEOSAIDesireCaptureCitRes::GetExpectedEffort(){ return 0.0f; }

bool CEOSAIDesireCaptureCitRes::CanFulfillDesire( CEOSAIPoiObject* pActor ) // CityHunters / LandResHunters / SeaResHunters
{
	CEOSAICity* pActorCity = dynamic_cast< CEOSAICity* >( pActor );
	if( pActorCity ){ return true; }

	CEOSAIUnit2* pActorUnit = dynamic_cast< CEOSAIUnit2* >( pActor );
	if( pActorUnit ){ return CanFulfillDesire( pActorUnit->GetAIUnitTemplate() ); }

	return false;
}

bool CEOSAIDesireCaptureCitRes::CanFulfillDesire( CEOSAIUnitTemplate* pActor )
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

bool  CEOSAIDesireCaptureCitRes::TargetIsACity()
{
	CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( m_pAITarget );
	return pAICity != NULL;
}

bool  CEOSAIDesireCaptureCitRes::TargetIsALandResource()
{
	CEOSAIResource* pTargetAIRes = dynamic_cast< CEOSAIResource* >( m_pAITarget );
	if( pTargetAIRes )
	{
		return pTargetAIRes->ResourceSource_IsOnLand();
		//return pTargetRes->IsOnLand3();
	}
	return false;
}

bool  CEOSAIDesireCaptureCitRes::TargetIsASeaResource()
{
	CEOSAIResource* pTargetAIRes = dynamic_cast< CEOSAIResource* >( m_pAITarget );
	if( pTargetAIRes )
	{
		return pTargetAIRes->ResourceSource_IsOnLand() == false;
		//return pTargetRes->IsOnWater3();
	}
	return false;
}


