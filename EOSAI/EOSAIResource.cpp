
#include "stdafx.h"
#include "EOSAIResource.h"
#include "EOSAIBrain.h"
#include "AIPlayer.h"
//#include "AIThought_ShouldICaptureThatResource.h"
//#include "AIActionDefinition_ProtectCitRes.h"
#include "EOSAITacticalProject2.h"
//#include "AIActionDefinition_CaptureCitRes.h"
#include "EOSAIPlayerManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CEOSAIResource::CEOSAIResource() : CEOSAIPoiObject()
{
	m_fResourceProduction = 0.0f;
	m_bResourceSource_IsOnLand = false;
	//m_pServerResource = NULL;
	//m_pAIThought_ShouldICaptureThatCitRes = new CAIThought_ShouldICaptureThatCitRes( this );
}
/*
void  CAIResource::SetServerPoiObject( CPoiObject* pPoiObject )
{
	ASSERT( dynamic_cast< CResourceSource* >( pPoiObject ) );
	m_pServerResource = dynamic_cast< CResourceSource* >( pPoiObject );
	//SetPoiObject( m_pResource );
	CAIPoiObject::SetServerPoiObject( m_pServerResource );
}
*/
/*
void  CAIResource::SetPlayerPoiObject( CPoiObject* pPoiObject )
{
	if( pPoiObject == NULL ) return;
	//
	ASSERT( dynamic_cast< CResourceSource* >( pPoiObject ) );
	CAIPoiObject::SetPlayerPoiObject( pPoiObject );
}
*/
//CPoiObject* CAIResource::GetPoiObject(){ return m_pResource; }
/*
float CAIResource::GetResourcePerTurn()
{
	return (float) m_pServerResource->GetResourcePerTurn();
}
*/
/*
long  CAIResource::GetObjectId()
{
	return m_pServerResource->ObjectId();
}
*/
float  CEOSAIResource::GetAIValueOfResource( long iAccordingToPlayer )
{
	EOSAI::AIPlayer* pAIPlayer = g_pAIPlayerManager->GetAIPlayer( iAccordingToPlayer );
	ASSERT( pAIPlayer );
	//float fMult = m_pAIBrain->GetAIPlayer()->GetCitResValue_ResourceMultiplier( this->GetResourceType() );
	float fMult = pAIPlayer->GetCitResValue_ResourceMultiplier( this->GetResourceType() );
	return m_fResourceProduction * fMult;
}

void CEOSAIResource::GetResourcesProducedPerTurn(std::map<CString, float>& ResourcesProducedAssumingNoOrders, std::map<CString, float>& ResourcesProducedByCurrentOrders)
{
	ResourcesProducedAssumingNoOrders[m_strResourceType] += m_fResourceProduction;
}


// Thinking
//
/*
void CAIResource::CreateSomeThoughtsAboutThisObject()
{
	CEOSAIThoughtDatabase* pAIThoughtDatabase = GetAIBrain()->GetAIThoughtDatabase();

	/-*
	if( this->IsMine() )
	{
		CAITacticalProject_ProtectCitRes* pProtectCitRes = new CAITacticalProject_ProtectCitRes( m_pAIBrain, this );
		pAIThoughtDatabase->AddActionDefinition( pProtectCitRes );
	}
	//else if( this->IsPAlly() )
	else if( this->IsAnAlly() )
	{
		CAITacticalProject_ProtectCitRes* pProtectCitRes = new CAITacticalProject_ProtectCitRes( m_pAIBrain, this );
		pAIThoughtDatabase->AddActionDefinition( pProtectCitRes );
	}
	/-*
	else if( this->GetOwner() == 0 )
	{
		CAITacticalProject_CaptureCitRes* pCaptureCitRes = new CAITacticalProject_CaptureCitRes( m_pAIBrain, this );
		pAIThoughtDatabase->AddActionDefinition( pCaptureCitRes );
	}
	else // Owned by TAlly, Neutral, or Enemy
	{
		CAITacticalProject_CaptureCitRes* pCaptureCitRes = new CAITacticalProject_CaptureCitRes( m_pAIBrain, this );
		pAIThoughtDatabase->AddActionDefinition( pCaptureCitRes );
	}
	*-/
}
*/
/*
class CAIResourceAmountTick
{
	public:
		float m_fAmount;
		float m_fDelta;
};
*/
/*
CAIResourceTracker::CAIResourceTracker()
{
	for( long i=0; i<10; i++ ){ m_Data[i].Clear(); }
}

void CAIResourceTracker::AddDataPoint( float fAmount, float fDelta )
{
	// Push the old information to the back
	for( long i=9; i>0; i-- )
	{
		m_Data[i] = m_Data[i-1];
	}
	m_Data[0].m_fAmount = fAmount;
	m_Data[0].m_fDelta  = fDelta;
}

float CAIResourceTracker::GetEstimatedDelta() // Figure out when/if there will be a resource shortage
{
	long iValidDataPoints = 0;
	for( long i=0; i<10; i++ )
	{
		if( m_Data[i].m_fAmount == 0.0f && m_Data[i].m_fDelta == 0.0f )
		{
			break;
		}
		iValidDataPoints = i+1;
	}

	float fImplicitDelta = 0.0f;
	float fExplicitDelta = 0.0f;
	for( long i=0; i<iValidDataPoints; i++ )
	{
		fExplicitDelta += m_Data[i].m_fDelta;
		if( i > 0 )
		{
			fImplicitDelta += ( m_Data[0].m_fAmount - m_Data[i].m_fAmount ) / i;
		}
	}

	if( iValidDataPoints >= 1 )
	{
		fExplicitDelta /= iValidDataPoints;
	}
	if( iValidDataPoints >= 2 )
	{
		fImplicitDelta /= ( iValidDataPoints - 1 );
	}

	if( iValidDataPoints == 1 )
	{
		return fExplicitDelta;
	}
	return ( fExplicitDelta + fImplicitDelta ) / 2.0f;
}

float CAIResourceTracker::GetEstimatedAmount( float fTimeInTheFuture )
{
	float fDelta = GetEstimatedDelta();
	return m_Data[0].m_fAmount + fTimeInTheFuture*fDelta;
}

float CAIResourceTracker::GetEstimatedShortageTime()
{
	float fDelta = GetEstimatedDelta();

	// Reduce the delta a bit (to represent possible future variation)
	fDelta -= 5.0f;
	if( fDelta >= 0.0f ) return 1000000.0f;

	float fTime = m_Data[0].m_fAmount / (-fDelta);
	if( fTime > 1000000.0f ) fTime = 1000000.0f;
	return fTime;
}
*/