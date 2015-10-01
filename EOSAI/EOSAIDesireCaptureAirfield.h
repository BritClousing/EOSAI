
#pragma once

#include "EOSAIDesirePoiObject.h"
#include "EOSAILongSet.h"
#include "EOSAILocation.h"
#include "EOSAIEnumDesireType.h"
class CPoiObject;

//

class CEOSAIDesireCaptureAirfield : public CEOSAIDesirePoiObject
{
	public:
		// Initialization
		CEOSAIDesireCaptureAirfield();
		virtual ~CEOSAIDesireCaptureAirfield(){}

		virtual void  SetTarget( CEOSAIPoiObject* pTarget );

		virtual CString Dump();

		// Information
		virtual EOSAIEnumAIDesireType GetType();

		// Overrides
		virtual bool  InvolvesGroundTargets();
		virtual bool  GetInvolvesAnAttack(){ return true; }

		virtual float GetSimpleInterest();
		virtual float GetExpectedEffort(){ return 0.0f; }

		virtual bool  CanFulfillDesire( CEOSAIPoiObject* pActor ); // CityHunters / LandResHunters / SeaResHunters
		virtual bool  CanFulfillDesire( CEOSAIUnitTemplate* pActor );
		virtual float GetUnitJobValue01( CEOSAIUnitTemplate* pActor ){ return CanFulfillDesire( pActor ) ? 1.0f : 0.0f; }
};
