
#pragma once

#include "EOSAIDesirePoiObject.h"
#include "EOSAILongSet.h"
#include "EOSAILocation.h"
class CPoiObject;

//

class CAIDesireBuildAirfield : public CEOSAIDesireSpatial
{
	public:
		// Initialization
		CAIDesireBuildAirfield();
		//virtual void  SetTarget( CPoiObject* pTarget );

		// Information
		EOSAIEnumAIDesireType  GetType(){ return EnumBuildAirfield; }

		// Override
		virtual bool  InvolvesGroundTargets(){ return true; }
		virtual bool  GetInvolvesAnAttack(){ return false; }

		virtual float GetSimpleInterest();
		virtual float GetExpectedEffort(){ return 0.0f; }

		virtual bool  CanFulfillDesire( CEOSAIPoiObject* pActor ); // CityHunters / LandResHunters / SeaResHunters
		virtual bool  CanFulfillDesire( CEOSAIUnitTemplate* pActor );
		virtual float GetUnitJobValue01( CEOSAIUnitTemplate* pActor ){ return CanFulfillDesire( pActor ) ? 1.0f : 0.0f; }
};
