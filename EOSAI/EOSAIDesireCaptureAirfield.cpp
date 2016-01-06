
#include "stdafx.h"
//#include "WorldDescServer.h"
#include "EOSAIBrain.h"
#include "EOSAIDesireCaptureAirfield.h"
//#include "City.h"
//#include "ResourceSource.h"
//#include "Unit.h"
#include "City.h"
#include "EOSAIResource.h"
#include "EOSAIUnit2.h"
#include "EOSAIAirfield.h"
#include "AIPlayer.h"
#include "EOSAICommonData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CEOSAIDesireCaptureAirfield::CEOSAIDesireCaptureAirfield()
{
}

void CEOSAIDesireCaptureAirfield::SetTarget( CEOSAIPoiObject* pAITarget )
{
	pAITarget->AddAIDesire( this );

	CEOSAIAirfield* pTargetAIAirfield = dynamic_cast< CEOSAIAirfield* >( pAITarget );

	IsAggressiveAgainstPlayer( pAITarget->GetOwner() );

	ASSERT( m_pAITarget == NULL );
	ASSERT( pTargetAIAirfield );
	if( pTargetAIAirfield )
	{
		m_pAITarget = pAITarget;
		m_JobsToDo.m_fGroundCitResHunter = 1.0f;
	}
}

CString CEOSAIDesireCaptureAirfield::Dump()
{
	CString str, str2;
	if( dynamic_cast< CEOSAIAirfield* >( GetAIPoiObjectTarget() ) )
	{
		CEOSAIAirfield* pAIAirfield = dynamic_cast< CEOSAIAirfield* >( GetAIPoiObjectTarget() );
		str.Format( _T("CaptureAirfield %s  ObjectId:%d  Owner:%d\r\n"), 
			pAIAirfield->GetName(), GetAIPoiObjectTarget()->GetObjectId(), GetAIPoiObjectTarget()->GetOwner() );
	}
	str2.Format( _T("  Simple Interest: %0.1f\r\n"), GetSimpleInterest() ); str += str2;
	str2.Format( _T("  Area Simple Interest: %0.1f\r\n"), GetAreaSimpleInterest() ); str += str2;
	str2.Format( _T("  Accessibility01: %0.2f\r\n"), GetAccessibility01() ); str += str2;
	str2.Format( _T("  Score: %0.1f\r\n"), GetScore() ); str += str2;
	return str;
}

EOSAIEnumAIDesireType  CEOSAIDesireCaptureAirfield::GetType()
{
	return EnumCaptureAirfield;
}

bool  CEOSAIDesireCaptureAirfield::InvolvesGroundTargets()
{
	return ( GetMultiRegion()->GetGeo()->GetGeoType() == CEOSAIGeo::Land );
}

float CEOSAIDesireCaptureAirfield::GetSimpleInterest()
{
	ASSERT( m_pAIBrain );
	long iAIPlayer = m_pAIBrain->GetAIPlayerNumber();

	CEOSAIAirfield* pTargetAIAirfield = dynamic_cast< CEOSAIAirfield* >( m_pAITarget );

	float fScore = 0.0f;
	ASSERT( pTargetAIAirfield );
	if( pTargetAIAirfield )
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

		fScore = 
			fForeignRelationsBasedMultiplier * 6.0f;
			//m_pAIBrain->GetAIPlayer()->GetCitResValue_CityMultiplier() *
			//pTargetAICity->GetTotalProduction();

		/*
		if( pTargetAICity )
		{
			fScore = 
				fForeignRelationsBasedMultiplier * 
				m_pAIBrain->GetAIPlayer()->GetCitResValue_CityMultiplier() *
				pTargetAICity->GetTotalProduction();
		}
		if( pTargetAIResource )
		{
			fScore = 
				fForeignRelationsBasedMultiplier * 
				m_pAIBrain->GetAIPlayer()->GetCitResValue_ResourceMultiplier( pTargetAIResource->GetResource()->GetResourceType() ) *
				pTargetAIResource->GetResourcePerTurn();
		}
		*/
	}
	return fScore;
}

//float CEOSAIDesireCaptureAirfield::GetExpectedEffort(){ return 0.0f; }

bool CEOSAIDesireCaptureAirfield::CanFulfillDesire( CEOSAIPoiObject* pActor ) // CityHunters / LandResHunters / SeaResHunters
{
	CEOSAICity* pActorCity = dynamic_cast< CEOSAICity* >( pActor );
	if( pActorCity ){ return true; }

	CEOSAIUnit* pActorUnit = dynamic_cast< CEOSAIUnit* >( pActor );
	if( pActorUnit ){ return CanFulfillDesire( pActorUnit->GetAIUnitTemplate() ); }

	return false;
}

bool CEOSAIDesireCaptureAirfield::CanFulfillDesire( CEOSAIUnitTemplate* pActor )
{
	return pActor->CanCaptureAirfields();
}
/*
bool  CEOSAIDesireCaptureAirfield::TargetIsACity()
{
	CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( m_pAITarget );
	return pAICity != NULL;
}

bool  CEOSAIDesireCaptureAirfield::TargetIsALandResource()
{
	CEOSAIResource* pTargetAIRes = dynamic_cast< CEOSAIResource* >( m_pAITarget );
	if( pTargetAIRes )
	{
		return pTargetAIRes->GetResource()->ResourceSource_IsOnLand();
		//return pTargetRes->IsOnLand3();
	}
	return false;
}

bool  CEOSAIDesireCaptureAirfield::TargetIsASeaResource()
{
	CEOSAIResource* pTargetAIRes = dynamic_cast< CEOSAIResource* >( m_pAITarget );
	if( pTargetAIRes )
	{
		return pTargetAIRes->GetResource()->ResourceSource_IsOnLand() == false;
		//return pTargetRes->IsOnWater3();
	}
	return false;
}
*/


