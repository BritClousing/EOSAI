
#pragma once

#include "EOSAIDesire2.h"
//#include "EOSAILongSet.h"
//#include "EOSAILocation.h"
//class CPoiObject;
class CEOSAIMultiRegion2;

//
// This is an abstract class
// Spatial desires have a location associated with them.
//   Example: Capture a city, protect a multiregion, bomb a target, defend isthmus, etc.
// (As opposed to more general desires like "use strategic bombing against the enemy")
//

class CEOSAIDesireMultiRegions : public CEOSAIDesireSpatial
{
	public:
		// Initialization
		void  AddMultiRegion( long iMultiRegion ){ m_MultiRegions.Add( iMultiRegion ); }

		//CPoiObject* m_pTarget; // optional (may be a target, may be a PoiObject to protect, etc)
		//CEOSAILocation   m_Location;
		//CEOSAILongSet    m_MultiRegions; // optional
		CList< CMultiRegion* >  m_MultiRegions;
};
