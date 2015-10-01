
#pragma once

#include "EOSAIDesirePoiObject.h"
#include "EOSAILongSet.h"
#include "EOSAILocation.h"
class CEOSAIPoiObject;

//

class CAIDesireProtectCitRes : public CEOSAIDesirePoiObject
{
	public:
		// Initialization
		CAIDesireProtectCitRes(){}
		virtual ~CAIDesireProtectCitRes(){}

		virtual void  SetTarget( CEOSAIPoiObject* pTarget );

		// Override
		virtual bool  GetInvolvesAnAttack(){ return false; }

		virtual float GetSimpleInterest();
		virtual float GetExpectedEffort(){ return 0.0f; }

		virtual bool  CanFulfillDesire( CEOSAIPoiObject* pActor ); // CityHunters / LandResHunters / SeaResHunters
		virtual bool  CanFulfillDesire( CEOSAIUnitTemplate* pActor );

		bool  TargetIsACity();
		bool  TargetIsALandResource();
		bool  TargetIsASeaResource();
};


