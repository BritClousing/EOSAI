
#pragma once

#include "PoiObjectState.h"
class CEOSAIActionOutline;
class CEOSAIMultiRegion2;
namespace EOSAI
{
	class UnitTask;
}
class CEOSAIBrain;
class CAITacticalProject;
class CEOSAIUnitActionIdea;

// High-level description of action:
//
// CityBuildW, GroundMovementToMultiRegionX, TransportMovementToMultiRegionY, 
//   GroundMovementToMultiRegionY, CaptureCityZ
// CityBuildW, GroundMovementToMultiRegionX, CaptureCityZ
// GroundMovementToRegionX, CaptureCityZ
//

class CEOSAIAction
{
	public:
		CEOSAIAction( CEOSAIUnitActionIdea* pUnitActionIdea );
		virtual ~CEOSAIAction();

		CEOSAIUnitActionIdea*  GetAIUnitActionIdea(){ return m_pUnitActionIdea; }

		void  UpdateFinalTime( float fTime );

		bool  HasBeenTurnedIntoTasks(){ return m_bHasBeenTurnedIntoTasks; }
		void  HasBeenTurnedIntoTasks( bool b ){ m_bHasBeenTurnedIntoTasks = b; }

		virtual float GetScore(){ return 0.0f; }
		virtual void  UpdateScore(){}

	protected:

		CEOSAIUnitActionIdea*     m_pUnitActionIdea;

		// The general outline of the action I'm going to pursue
		//CEOSAIActionOutline*      m_pActionOutline; // owned

		CList< EOSAI::UnitTask* >  m_AITasks; // owned, but controlled by the AIObject
		bool                   m_bHasBeenTurnedIntoTasks;
};

