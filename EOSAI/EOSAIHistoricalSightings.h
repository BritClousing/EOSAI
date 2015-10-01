
#pragma once

#include <list>
#include "EOSAILocation.h"
//#include "GameTime.h"
class CEOSAIPoiObject;
//class CWorldDescBase;
class CAISpatialMap;
//class CEOSAIBrain;
class CAIPlayer;

/*
	Sightings can happen multiple times for a single PoiObject.  
		The earliest sighting is remembered for citres (because they don't move) 
		and they increase in time.
		For mobile units, old sightings are remembered, new sightings are created
		(at most) once every turn, old sightings significance decay over time and 
		they decay each time I make a new sighting of the same unit.
*/
/*
class CAIPoiObjectSighting;

class CAIPoiObjectInvolvedInSighting
{
	public:
		CAIPoiObjectInvolvedInSighting( CEOSAIPoiObject* pAIPoiObject );
		~CAIPoiObjectInvolvedInSighting();

		float GetBaseSignificance();
		void  ConsiderAddingASighting( float fTime, CEOSAILocation location );
		//void  AddPlayerSighting( CAIPoiObjectSighting* pSighting );

		CEOSAIPoiObject*  m_pAIPoiObject;
		long            m_iOwner; // This can change for capturable poi (citres)
		bool            m_bIsMobile;
		std::list< CAIPoiObjectSighting* > m_Sightings;
};

class CAIPoiObjectSighting
{
	public:
		CAIPoiObjectSighting( CAIPoiObjectInvolvedInSighting* pPoiObjectInvolvedInSighting, CEOSAILocation Location, float fTime )
		{
			m_pPoiObjectInvolvedInSighting = pPoiObjectInvolvedInSighting;
			m_Location = Location;
			//m_TimeOfSighting.Set( fTime );
			m_TimeOfSighting = fTime;
		}

		CAIPoiObjectInvolvedInSighting* m_pPoiObjectInvolvedInSighting;

		CEOSAILocation  m_Location;
		float      m_fSignificance; // Citres are most significant, ground units significant, transports somewhat significant, ships and subs less significant
		//CGameTime    m_TimeOfSighting;
		float      m_TimeOfSighting;
};
*/
/*
	The BorderTrackingMap remembers information about other player's citres and
		past sightings of enemy units to get a sense of the player's boundaries.

	This is important for several reasons:
		- Determining the player's strength (and whether the AI should attack them)
		- Determining when the citres have mostly been captured, which affects:
			- Deciding when the AI should shift to an explore stance to combat stance
			  (This question is difficult to answer when there is still significant
			   amounts of unknown area on the map.)
		- Determining which areas to keep weak units away from.
*/

/*
class CAIHistoricalSightings
{
	public:
		CAIHistoricalSightings(){}
		~CAIHistoricalSightings();
		void  Initialize( CWorldDescBase* pWorldDesc, CAIPlayer* pAIPlayer ){ m_pWorldDesc = pWorldDesc; m_pAIPlayer = pAIPlayer; }
		void  DeleteObjects();

		void  AddSighting( CPoiObject* pPoiObject, float fTime, CEOSAILocation location );

		// Need to update periodically (every turn or so)
		void  UpdateSignificanceOfSightings( float fCurrentTime );

		// Figure out the likihood of player presence at a location
		float GetAssumedPlayerPresence( CEOSAILocation location, long iPlayer );      // presence of iPlayer
		float GetAssumedOtherPlayerPresence( CEOSAILocation location, long iPlayer ); // presence of everyone but iPlayer

		float EstimatePlayerProduction( long iPlayer, CAISpatialMap* pSpatialMap, long iTurn );

	private:
		CAIPoiObjectInvolvedInSighting* InvokePoiObject( CPoiObject* pPoiObject );

		CWorldDescBase* m_pWorldDesc;
		//CEOSAIBrain*       m_pAIBrain;
		CAIPlayer*      m_pAIPlayer;
		std::list< CAIPoiObjectInvolvedInSighting* > m_PoiObjectIsSighting;
};
*/
