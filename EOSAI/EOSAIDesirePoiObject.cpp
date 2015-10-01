
#include "stdafx.h"
#include "EOSAIDesirePoiObject.h"
#include "EOSAIBrain.h"
#include "PoiObject.h"
#include "EOSAIMultiRegionManager2.h"
#include "EOSAIMultiRegion2.h"
#include "PoiObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

long  CEOSAIDesirePoiObject::GetTargetObjectId()
{
	if( m_pAITarget ) return m_pAITarget->GetObjectId();
	return 0;
}
/*
CPoiObject*  CEOSAIDesirePoiObject::GetPoiObjectTarget()
{
	if( m_pAITarget ) return m_pAITarget->GetServerPoiObject();
	return NULL;
}
*/
CEOSAILocation CEOSAIDesirePoiObject::GetLocation()
{
	ASSERT( m_pAITarget );
	return m_pAITarget->GetInitialState()->GetLocation();
}

/*
float CEOSAIDesirePoiObject::GetDistance( CEOSAILocation Location )
{
	ASSERT( m_pTarget );
	CWorldBuildDesc* pWorldBuildDesc = GetCommonState()->GetWorldBuildDesc();
	return pWorldBuildDesc->GetPixelDistance( m_pTarget->GetLocation(), Location );
}
*/
/*
void  CEOSAIDesirePoiObject::AttachToMultiRegion()
{
	ASSERT( m_pTarget );
	ASSERT( m_pAIBrain );

	CEOSAILocation Location = m_pTarget->GetLocation();
	CMultiRegion* pMultiRegion = m_pAIBrain->GetMultiRegionManager()->GetMultiRegion( Location );
	ASSERT( pMultiRegion );
	pMultiRegion->AddSpatialDesire( this );
}
*/
