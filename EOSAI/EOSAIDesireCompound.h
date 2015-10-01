/*
#pragma once

//
// Compound Desires organize low-level desires under one heading.
//   This might include "invade MultiRegion", "capture CitRes", and "capture airfield" under one heading
// I may want to move all this under an AIActionDefinition instead.
//

#include "EOSAIDesire2.h"

class CAIDesireCompound : public CEOSAIDesire2
{
	public:
		CAIDesireCompound(){}

		virtual float GetRawScore(){ return 0.0f; }
		virtual float GetExpectedEffort(){ return 0.0f; }

		// 
		CList< CEOSAIDesireSpatial* >  m_ComponentDesires;
};
*/


