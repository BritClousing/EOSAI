
#pragma once

#include "EOSAIDesirePoiObject.h"
#include "EOSAILongSet.h"
#include "EOSAILocation.h"
class CPoiObject;

//

class CEOSAIDesireCaptureCitRes : public CEOSAIDesireMultiRegions
{
	public:
		// Initialization
		CEOSAIDesireCaptureCitRes(){}
		virtual void  SetTarget( CPoiObject* pTarget );

		virtual float GetSimpleInterest();
		virtual float GetExpectedEffort(){ return 0.0f; }

		virtual bool  CanFulfillDesire( CPoiObject* pActor ); // CityHunters / LandResHunters / SeaResHunters
		virtual bool  CanFulfillDesire( CUnitTemplate* pActor );

		//bool  TargetIsACity();
		//bool  TargetIsALandResource();
		//bool  TargetIsASeaResource();
};


