
#pragma once

#include "EOSAIDesireSpatial.h"
#include "EOSAILongSet.h"
#include "EOSAILocation.h"
class CEOSAIPoiObject;

//

class CEOSAIDesirePoiObject : public CEOSAIDesireSpatial
{
	public:
		CEOSAIDesirePoiObject(){ m_pAITarget = NULL; }
		virtual ~CEOSAIDesirePoiObject(){}

		// Initialization
		virtual void  SetTarget( CEOSAIPoiObject* pAITarget ){ m_pAITarget = pAITarget; }
		//virtual void  SetTarget( CPoiObject* pTarget ){ m_pTarget = pTarget; }

		// PoiObject
		virtual long           GetTargetObjectId();
		//virtual CEOSAIPoiObject*  GetAIPoiObjectTarget();
		virtual CEOSAIPoiObject*  GetAIPoiObjectTarget(){ return m_pAITarget; }

		// Override
		virtual bool  GetInvolvesAnAttack(){ return false; }

		// Location
		virtual CEOSAILocation GetLocation();
		virtual float     GetRadius(){ return 0.0f; }
		//virtual float     GetDistance( CEOSAILocation Location );

		// Only used spatial maps
		//virtual bool  IsWithinRadius( CEOSAILocation Location, float fRadius );

		//virtual void  AttachToMultiRegion();

		CEOSAIPoiObject* m_pAITarget; // optional (may be a target, may be a PoiObject to protect, etc)
		//CEOSAILocation   m_Location;
		//CEOSAILongSet    m_MultiRegions; // optional
};


