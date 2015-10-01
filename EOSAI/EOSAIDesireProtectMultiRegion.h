
#pragma once

#include "EOSAIDesirePoiObject.h"
#include "EOSAILongSet.h"
#include "EOSAILocation.h"
class CPoiObject;

//
// This can involve defense against ships, submarines, aircraft, missiles, etc.
//   The AI should know what type of units to protect against (it will probably be a combination)
// This information can go into JobsToDo - which can store the types of units to defend against
//

class CEOSAIDesireProtectMultiRegion : public CEOSAIDesireMultiRegions
{
	public:
		// Initialization
		CEOSAIDesireProtectMultiRegion(){}
		virtual void  SetTarget( CPoiObject* pTarget );

		// Override
		virtual bool  GetInvolvesAnAttack(){ return false; }

		virtual float GetSimpleInterest();
		virtual float GetExpectedEffort(){ return 0.0f; }

		virtual bool  CanFulfillDesire( CPoiObject* pActor ); // CityHunters / LandResHunters / SeaResHunters
		virtual bool  CanFulfillDesire( CUnitTemplate* pActor );

		//


		//bool  TargetIsACity();
		//bool  TargetIsALandResource();
		//bool  TargetIsASeaResource();
};


