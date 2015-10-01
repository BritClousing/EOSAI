
#pragma once

#include "EOSAIDesirePoiObject.h"
#include "EOSAILongSet.h"
#include "EOSAILocation.h"
class CPoiObject;

//
// DamageCity is fulfilled by strategic bombing, artillery attacks, missiles, and nukes.
//   It means: damage buildings, in-production, or population

class CAIDesireDamageCity : public CEOSAIDesirePoiObject
{
	public:
		// Initialization
		CAIDesireDamageCity();
		virtual ~CAIDesireDamageCity(){}

		virtual void  SetTarget( CEOSAIPoiObject* pTarget );

		virtual CString Dump();

		// Information
		virtual EOSAIEnumAIDesireType  GetType(){ return EnumDegradeCity; }

		// Override
		virtual bool  GetInvolvesAnAttack(){ return true; }

		virtual float GetSimpleInterest();
		virtual float GetExpectedEffort(){ return 0.0f; }

		virtual bool  CanFulfillDesire( CEOSAIPoiObject* pActor ); // CityHunters / LandResHunters / SeaResHunters
		virtual bool  CanFulfillDesire( CEOSAIUnitTemplate* pActor );
		virtual float GetUnitJobValue01( CEOSAIUnitTemplate* pActor ){ return CanFulfillDesire( pActor ) ? 1.0f : 0.0f; }

		//bool  TargetIsACity();
		//bool  TargetIsALandResource();
		//bool  TargetIsASeaResource();
	private:
		//float  m_fDistanceFromMyClosestAirbase; // Used in determining if aircraft can reach it
};


