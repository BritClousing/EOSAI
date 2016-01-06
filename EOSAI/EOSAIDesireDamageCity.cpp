
#include "stdafx.h"
//#include "WorldDescServer.h"
#include "EOSAIBrain.h"
#include "EOSAIDesireDamageCity.h"
#include "EOSAIResource.h"
#include "EOSAIUnit2.h"
#include "City.h"
#include "EOSAICommonData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CAIDesireDamageCity::CAIDesireDamageCity()
{
	m_JobsToDo.m_fKillPopulation = 1.0f;
	m_JobsToDo.m_fDestroyBuildings = 1.0f;
	//m_fDistanceFromMyClosestAirbase = 1000000.0f;
}

void CAIDesireDamageCity::SetTarget( CEOSAIPoiObject* pAITarget )
{
	CEOSAICity* pTargetAICity = dynamic_cast< CEOSAICity* >( pAITarget );
	//CResourceSource* pTargetResource = dynamic_cast< CResourceSource* >( pTarget );
	IsAggressiveAgainstPlayer( pAITarget->GetOwner() );

	ASSERT( pTargetAICity );
	if( pTargetAICity )
	{
		m_pAITarget = pAITarget;
	}
}

CString CAIDesireDamageCity::Dump()
{
	CString str, str2;
	if( dynamic_cast< CEOSAICity* >( GetAIPoiObjectTarget() ) )
	{
		CEOSAICity* pAICity = dynamic_cast< CEOSAICity* >( GetAIPoiObjectTarget() );
		str.Format( _T("DamageCity %s Prod:%1.0f  ObjectId:%d  Owner:%d\r\n"), 
			pAICity->GetName(), pAICity->GetTotalProduction( false,false ), GetAIPoiObjectTarget()->GetObjectId(), GetAIPoiObjectTarget()->GetOwner() );
	}
	str2.Format( _T("  Simple Interest: %0.1f\r\n"), GetSimpleInterest() ); str += str2;
	str2.Format( _T("  Area Simple Interest: %0.1f\r\n"), GetAreaSimpleInterest() ); str += str2;
	str2.Format( _T("  Accessibility01: %0.2f\r\n"), GetAccessibility01() ); str += str2;
	str2.Format( _T("  Score: %0.1f\r\n"), GetScore() ); str += str2;
	return str;
}

float CAIDesireDamageCity::GetSimpleInterest()
{
	ASSERT( m_pAIBrain );
	long iAIPlayer = m_pAIBrain->GetAIPlayerNumber();

	CEOSAICity* pTargetAICity = dynamic_cast< CEOSAICity* >( m_pAITarget );

	float fScore = 0.0f;
	ASSERT( pTargetAICity );
	if( pTargetAICity )
	{
		// Slightly higher value for enemy CitRes - because he is using it against me
		float fMult = 0.7f;
		//CWorldDescServer* pWorldDescServer = GetCommonState()->GetWorldDescServer();
		if( m_pAITarget->GetOwner() == 0 )
		{
			fMult = 0.0f;
		}
		else if( g_pEOSAICommonData->GetForeignRelations( iAIPlayer, m_pAITarget->GetOwner() ).IsEnemy() )
		{
			fMult = 1.0f;
		}
		else if( g_pEOSAICommonData->GetForeignRelations( iAIPlayer, m_pAITarget->GetOwner() ).IsTeammate() )
		{
			fMult = 0.0f;
		}

		//fMult = 1.0f; // TEMP
		if( fMult > 0.0f )
		{
			CEOSAIMathFunction ProductionValueFunc;
			ProductionValueFunc.SetInputOutput(  1.0f,  0.0f );
			ProductionValueFunc.SetInputOutput(  5.0f,  2.0f );
			ProductionValueFunc.SetInputOutput( 10.0f, 10.0f );
			ProductionValueFunc.SetInputOutput(100.0f,100.0f );
			fScore = fMult * ProductionValueFunc.GetOutput( pTargetAICity->GetTotalProduction() );
		}
	}
	return fScore;
}

//float CEOSAIDesireCaptureCitRes::GetExpectedEffort(){ return 0.0f; }

bool CAIDesireDamageCity::CanFulfillDesire( CEOSAIPoiObject* pActor ) // CityHunters / LandResHunters / SeaResHunters
{
	CEOSAICity* pActorAICity = dynamic_cast< CEOSAICity* >( pActor );
	if( pActorAICity ){ return true; }

	CEOSAIUnit* pActorAIUnit = dynamic_cast< CEOSAIUnit* >( pActor );
	if( pActorAIUnit ){ return CanFulfillDesire( pActorAIUnit->GetAIUnitTemplate() ); }

	return false;
}

bool CAIDesireDamageCity::CanFulfillDesire( CEOSAIUnitTemplate* pAIActor )
{
	if( pAIActor->CanDegradeCities() )
	{
		return true;
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
/*
bool  CEOSAIDesireCaptureCitRes::TargetIsACity()
{
	CCity* pCity = dynamic_cast< CCity* >( m_pTarget );
	return pCity != NULL;
}

bool  CEOSAIDesireCaptureCitRes::TargetIsALandResource()
{
	CResourceSource* pTargetRes = dynamic_cast< CResourceSource* >( m_pTarget );
	if( pTargetRes )
	{
		return pTargetRes->ResourceSource_IsOnLand();
		//return pTargetRes->IsOnLand3();
	}
	return false;
}

bool  CEOSAIDesireCaptureCitRes::TargetIsASeaResource()
{
	CResourceSource* pTargetRes = dynamic_cast< CResourceSource* >( m_pTarget );
	if( pTargetRes )
	{
		return pTargetRes->ResourceSource_IsOnLand() == false;
		//return pTargetRes->IsOnWater3();
	}
	return false;
}
*/
