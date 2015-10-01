
#pragma once

#include "EOSAIDesirePoiObject.h"
#include "EOSAILongSet.h"
#include "EOSAILocation.h"
class CPoiObject;

//
// DamageCity is fulfilled by strategic bombing, artillery attacks, missiles, and nukes.
//   It means: damage buildings, in-production, or population

class CAIDesireDestroyUnit : public CEOSAIDesirePoiObject
{
	public:
		// Initialization
		CAIDesireDestroyUnit();
		virtual ~CAIDesireDestroyUnit(){}

		virtual void  SetTarget( CEOSAIPoiObject* pTarget );

		virtual CString Dump();

		// Information
		virtual EOSAIEnumAIDesireType  GetType(){ return EnumDestroyUnit; }

		// Override
		virtual bool  InvolvesGroundTargets();
		virtual bool  GetInvolvesAnAttack(){ return true; }

		virtual float GetSimpleInterest();
		virtual float GetExpectedEffort(){ return 0.0f; }

		virtual bool  CanFulfillDesire( CEOSAIPoiObject* pActor ); // CityHunters / LandResHunters / SeaResHunters
		virtual bool  CanFulfillDesire( CEOSAIUnitTemplate* pActor );
		virtual float GetUnitJobValue01( CEOSAIUnitTemplate* pActor );
		//virtual float GetCombatAttritionValue( CUnitTemplate* pActor );

		bool  TargetIsAnAirUnit();
		bool  TargetIsAnGroundUnit();
		bool  TargetIsAnSeaUnit();
};

