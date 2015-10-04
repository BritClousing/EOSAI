
#include "stdafx.h"
#include "EOSAIResource.h"
//#include "ResourceSource.h"
//#include "Airfield.h"
#include "EOSAIAirfield.h"
#include "EOSAIBrain.h"
#include "AIPlayer.h"
#include "EOSAICommonData.h"
//#include "WorldDesc.h"
//#include "WorldDescPlayer.h"
//#include "WorldDescServer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CEOSAIAirfield::CEOSAIAirfield() 
	: CEOSAIPoiObject()
{
	//m_pServerAirfield = NULL;
/*
	m_fArea_Range100 = 0.0f;
	m_fArea_Range150 = 0.0f;
	m_fArea_Range200 = 0.0f;
	m_fArea_Range250 = 0.0f;
	m_fArea_Range300 = 0.0f;
	m_fArea_Range400 = 0.0f;
	m_fArea_Range500 = 0.0f;
	m_fArea_Range600 = 0.0f;
	m_fArea_Range700 = 0.0f;
	m_fArea_Range800 = 0.0f;
	m_fArea_Range900 = 0.0f;
	m_fArea_Range1000 = 0.0f;
	m_fArea_Range1200 = 0.0f;
	m_fArea_Range1500 = 0.0f;

	m_fUnexploredArea_Range100 = 0.0f;
	m_fUnexploredArea_Range150 = 0.0f;
	m_fUnexploredArea_Range200 = 0.0f;
	m_fUnexploredArea_Range250 = 0.0f;
	m_fUnexploredArea_Range300 = 0.0f;
	m_fUnexploredArea_Range400 = 0.0f;
	m_fUnexploredArea_Range500 = 0.0f;
	m_fUnexploredArea_Range600 = 0.0f;
	m_fUnexploredArea_Range700 = 0.0f;
	m_fUnexploredArea_Range800 = 0.0f;
	m_fUnexploredArea_Range900 = 0.0f;
	m_fUnexploredArea_Range1000 = 0.0f;
	m_fUnexploredArea_Range1200 = 0.0f;
	m_fUnexploredArea_Range1500 = 0.0f;
	*/
}

CEOSAIAirfield::~CEOSAIAirfield()
{}
/*
void  CEOSAIAirfield::SetServerPoiObject( CPoiObject* pPoiObject )
{
	ASSERT( dynamic_cast< CAirfield* >( pPoiObject ) );
	m_pServerAirfield = dynamic_cast< CAirfield* >( pPoiObject );
	CAIPoiObject::SetServerPoiObject( pPoiObject );
}
*/
/*
void  CEOSAIAirfield::SetPlayerPoiObject( CPoiObject* pPoiObject )
{
	if( pPoiObject == NULL ) return;
	//
	ASSERT( dynamic_cast< CAirfield* >( pPoiObject ) );
	//m_pCity = dynamic_cast< CAirfield* >( pPoiObject );
	CAIPoiObject::SetPlayerPoiObject( pPoiObject );
}
*/
void  CEOSAIAirfield::CalculateUnexploredArea()
{
	ASSERT( false );

	long iNumberOfRegions = 0;
/*
	// Construct the unexplored area range information
	CWorldBuildDesc* pWorldBuildDesc = m_pAIBrain->GetAIPlayer()->GetWorldDescServer()->GetWorldBuildDesc();
	POSITION pos = m_pAIBrain->GetAIObjectList()->GetHeadPosition();
	while( pos )
	{
		CAIObject* pAIObject = m_pAIBrain->GetAIObjectList()->GetNext( pos );
		CEOSAIRegion2* pAIRegion = dynamic_cast< CEOSAIRegion2* >( pAIObject );
		if( pAIRegion )
		{
			iNumberOfRegions++;
			float fDistance = g_pEOSAICommonData->GetWorldDistanceTool()->GetDistance( m_pAIAirfield->GetLocation(), pAIRegion->GetLocation() );
			/*
			AddRegion( fDistance, pAIRegion->GetSizeOfRegion() );
			if( pAIRegion->HasBeenExplored() == false )
			{
				AddUnexploredRegion( fDistance, pAIRegion->GetSizeOfRegion() );
			}
			*-/
		}
	}
*/
	// If this ASSERT fails, it means that the AIRegions haven't been constructed yet.
	//   Code elsewhere needs to be adjusted so that they exist before calling this function.
	ASSERT( iNumberOfRegions > 0 );
}
/*
CPoiObject* CEOSAIAirfield::GetServerPoiObject()
{
	return m_pServerAirfield;
}

long  CEOSAIAirfield::GetObjectId()
{
	ASSERT( m_pServerAirfield );
	if( m_pServerAirfield )
	{
		return m_pServerAirfield->ObjectId();
	}
	return 0;
}
*/
bool  CEOSAIAirfield::CanContain_IgnoreForeignRelations( CEOSAIPoiObject* pAIPoiObject )
{
	EOSAI::PoiMobile* pOtherPoiMobile = dynamic_cast< EOSAI::PoiMobile* >( pAIPoiObject );
	if( pOtherPoiMobile )
	{
		return this->CanContain_IgnoreForeignRelations( pOtherPoiMobile );
	}
	return false;
}
/*
float CEOSAIAirfield::GetUnexploredPercent( float fRange )
{
	if( fRange <  100.0f ){ return 0.0f; }
	if( fRange <  150.0f ){ return m_fUnexploredArea_Range100 / m_fArea_Range100; }
	if( fRange <  200.0f ){ return m_fUnexploredArea_Range150 / m_fArea_Range150; }
	if( fRange <  250.0f ){ return m_fUnexploredArea_Range200 / m_fArea_Range200; }
	if( fRange <  300.0f ){ return m_fUnexploredArea_Range250 / m_fArea_Range250; }
	if( fRange <  400.0f ){ return m_fUnexploredArea_Range300 / m_fArea_Range300; }
	if( fRange <  500.0f ){ return m_fUnexploredArea_Range400 / m_fArea_Range400; }
	if( fRange <  600.0f ){ return m_fUnexploredArea_Range500 / m_fArea_Range500; }
	if( fRange <  700.0f ){ return m_fUnexploredArea_Range600 / m_fArea_Range600; }
	if( fRange <  800.0f ){ return m_fUnexploredArea_Range700 / m_fArea_Range700; }
	if( fRange <  900.0f ){ return m_fUnexploredArea_Range800 / m_fArea_Range800; }
	if( fRange < 1000.0f ){ return m_fUnexploredArea_Range900 / m_fArea_Range900; }
	if( fRange < 1200.0f ){ return m_fUnexploredArea_Range1000 / m_fArea_Range1000; }
	if( fRange < 1500.0f ){ return m_fUnexploredArea_Range1200 / m_fArea_Range1200; }
	return m_fUnexploredArea_Range1500 / m_fArea_Range1500;
}

void CEOSAIAirfield::AddRegion( float fDistance, long iSize )
{
	if( fDistance >  1500.0f ) return;
	if( fDistance <=  100.0f ){ m_fArea_Range100 += iSize; }
	if( fDistance <=  150.0f ){ m_fArea_Range150 += iSize; }
	if( fDistance <=  200.0f ){ m_fArea_Range200 += iSize; }
	if( fDistance <=  250.0f ){ m_fArea_Range250 += iSize; }
	if( fDistance <=  300.0f ){ m_fArea_Range300 += iSize; }
	if( fDistance <=  400.0f ){ m_fArea_Range400 += iSize; }
	if( fDistance <=  500.0f ){ m_fArea_Range500 += iSize; }
	if( fDistance <=  600.0f ){ m_fArea_Range600 += iSize; }
	if( fDistance <=  700.0f ){ m_fArea_Range700 += iSize; }
	if( fDistance <=  800.0f ){ m_fArea_Range800 += iSize; }
	if( fDistance <=  900.0f ){ m_fArea_Range900 += iSize; }
	if( fDistance <= 1000.0f ){ m_fArea_Range1000 += iSize; }
	if( fDistance <= 1200.0f ){ m_fArea_Range1200 += iSize; }
	if( fDistance <= 1500.0f ){ m_fArea_Range1500 += iSize; }
}

void CEOSAIAirfield::AddUnexploredRegion( float fDistance, long iSize )
{
	if( fDistance >  1500.0f ) return;
	if( fDistance <=  100.0f ){ m_fUnexploredArea_Range100 += iSize; }
	if( fDistance <=  150.0f ){ m_fUnexploredArea_Range150 += iSize; }
	if( fDistance <=  200.0f ){ m_fUnexploredArea_Range200 += iSize; }
	if( fDistance <=  250.0f ){ m_fUnexploredArea_Range250 += iSize; }
	if( fDistance <=  300.0f ){ m_fUnexploredArea_Range300 += iSize; }
	if( fDistance <=  400.0f ){ m_fUnexploredArea_Range400 += iSize; }
	if( fDistance <=  500.0f ){ m_fUnexploredArea_Range500 += iSize; }
	if( fDistance <=  600.0f ){ m_fUnexploredArea_Range600 += iSize; }
	if( fDistance <=  700.0f ){ m_fUnexploredArea_Range700 += iSize; }
	if( fDistance <=  800.0f ){ m_fUnexploredArea_Range800 += iSize; }
	if( fDistance <=  900.0f ){ m_fUnexploredArea_Range900 += iSize; }
	if( fDistance <= 1000.0f ){ m_fUnexploredArea_Range1000 += iSize; }
	if( fDistance <= 1200.0f ){ m_fUnexploredArea_Range1200 += iSize; }
	if( fDistance <= 1500.0f ){ m_fUnexploredArea_Range1500 += iSize; }
}
*/