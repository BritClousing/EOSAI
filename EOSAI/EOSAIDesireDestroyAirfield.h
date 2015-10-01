
#pragma once

#include "EOSAIDesirePoiObject.h"
#include "EOSAILongSet.h"
#include "EOSAILocation.h"
class CPoiObject;


class CAIDesireDestroyAirfield : public CEOSAIDesirePoiObject
{
	public:
		// Initialization
		CAIDesireDestroyAirfield();
		virtual ~CAIDesireDestroyAirfield(){}

		virtual void  SetTarget( CEOSAIPoiObject* pTarget );

		// Information
		virtual EOSAIEnumAIDesireType  GetType(){ return EnumDestroyAirfield; }

		// Override
		virtual bool  GetInvolvesAnAttack(){ return true; }

		virtual float GetSimpleInterest();
		virtual float GetExpectedEffort(){ return 0.0f; }

		virtual bool  CanFulfillDesire( CEOSAIPoiObject* pActor ); // CityHunters / LandResHunters / SeaResHunters
		virtual bool  CanFulfillDesire( CEOSAIUnitTemplate* pActor );
		virtual float GetUnitJobValue01( CEOSAIUnitTemplate* pActor ){ return CanFulfillDesire( pActor ) ? 1.0f : 0.0f; }
};
