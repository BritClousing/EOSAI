
#pragma once

#ifdef CREATE_EOSAI_DLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

#include "EOSAIListSort.h"
//#include "SemiGeo.h"
#include "EOSAILocation.h"
class CEOSAIRegion2;
class CEOSAIMultiRegion2;
class CEOSAIMultiRegionManager2;
class CEOSAIPoiObject;
//class C9MovementPathwaysFromEverywhereToMRX;
//class CAIMultiRegionPathwayScoresForTransports;
//class CAISeaPathways_EverywhereMRX;
//class CAIThought_WhatUnitsCanIGetToMultiRegion;
//class CAIThought_ShouldSendUnitsToCaptureTheLandCitRes;
//class CAIThought_ShouldSendUnitsToDegradePlayerCities;
//class CAIThought_ShouldSendUnitsToAttackHisUnits;
class CAIThought_ShouldSendUnitsHere;
#include "EOSAISettings.h"
class CEOSAIMultiRegion2;
class CWorldDescPlayer;
class CEOSAIGeo;
#include "EOSAIBCDumbArray1D.h"
#include "EOSAICitResUnitSummary.h"
class CAITacticalProject;
#include "EOSAIActionRequirement.h"
#include "EOSAIGenericRegion.h"
#include "EOSAIRegionPathwayMap.h"
//#include "EOSAIPublic.h"
class CEOSAIUnitProximity;
class CPoiObject;
class CEOSAIBrain;
class CEOSAIDesireSpatial;
class CEOSAIRegionPathwayMap;
//class CEOSAIActionList;

#define EOSAI_MAX_NUMBER_OF_PLAYERS 10

/*
enum EnumMultiRegionType
{
	UnoccupiedNear,   // no player presence, but borders on someone else's
	UnoccupiedMedium, // no player presence, but borders on UnoccupiedNear
	UnoccupiedFar,    // no player presence, but borders on UnoccupiedMedium or UnoccupiedFar
	Disputed,       // my CitResUnits are here, and so are someone else's
	MyBorderMultiRegion, // my CitResUnits are here, no one elses are, and I border on unoccupied
	MyInteriorMultiRegion, // my CitResUnits are here, no one elses are, and I border on unoccupied
	OtherPlayerBorderMultiRegion, // my CitResUnits are here, no one elses are, and I border on unoccupied
	OtherPlayerInteriorMultiRegion, // my CitResUnits are here, no one elses are, and I border on unoccupied
};
*/

// Each region is given a type for each player
//   e.g. a MultRegion might be "Player 0: BorderArea1, Player 1: BorderArea1, Player 2: UnoccupiedFar"
// An area that is a BorderArea1 for two players might be in dispute or simply a peaceful border

// Potential Invasion tags are most concerned with MultiRegions that are accessible,
//   not hidden behind other owned regions, and on the side facing me (rather than the nation's
//   backside).
/*
class CMultiRegionPotentialInvasionTag
{
	public:
		CMultiRegionPotentialInvasionTag()
		{
			m_pMultiRegion = NULL;
			m_iInvader = 0;
			m_iInvadee = 0;
			m_fDistanceFromInvadersCitRes = 0.0f;
			m_fObviousness = 1.0f;
		}

		CEOSAIMultiRegion2* m_pMultiRegion;

		// Might be me invading someone else, someone else invading me
		//   Note: the Invadee can be Player0 - which identifies which MultiRegions
		//   are close and easily invadable (no reason passing up decent CitRes to get
		//   more distant ones).
		long  m_iInvader;
		long  m_iInvadee;
		float m_fDistanceFromInvadersCitRes;
		float m_fObviousness; // 1.0 = obvious, 0.5 = semi-obvious, 0.0 = out of the way
};
*/

/*
class CPoiObjectProximity
{
	public:
		CPoiObjectProximity()
		{
			m_fDistance = 0.0f;
			m_pPoiObject = NULL;
		}

		float m_fDistance;
		CPoiObject* m_pPoiObject;
};
*/
#define MultiRegionType_Undefined -1
#define MultiRegionType_UnoccupiedFar 0
#define MultiRegionType_UnoccupiedMedium 1
#define MultiRegionType_UnoccupiedNear 2
#define MultiRegionType_BorderArea1 3
#define MultiRegionType_BorderArea2 4
#define MultiRegionType_InteriorArea 5
/*
enum EnumMultiRegionType
{
	Undefined = -1,
	UnoccupiedFar = 0,
	UnoccupiedMedium = 1,
	UnoccupiedNear = 2,
	BorderArea1 = 3,
	BorderArea2 = 4,
	InteriorArea = 5,
/-*
	UnoccupiedNear,   // no player presence, but borders on someone else's
	UnoccupiedMedium, // no player presence, but borders on UnoccupiedNear
	UnoccupiedFar,    // no player presence, but borders on UnoccupiedMedium or UnoccupiedFar
	Disputed,       // my CitResUnits are here, and so are someone else's
	MyBorderMultiRegion, // my CitResUnits are here, no one elses are, and I border on unoccupied
	MyInteriorMultiRegion, // my CitResUnits are here, no one elses are, and I border on unoccupied
	OtherPlayerBorderMultiRegion, // my CitResUnits are here, no one elses are, and I border on unoccupied
	OtherPlayerInteriorMultiRegion, // my CitResUnits are here, no one elses are, and I border on unoccupied
*-/
};
*/

//
// MultiRegions are of three types: Land, Water, and cities (these are small MultiRegions)
//
// The purpose of MultiRegions is to help the AI
//   - Quickly find what units are in/moving through an area
//   - Provide a callback system so that I can be notified if I move a unit
//        through an area later
//   - Provide the AI with a framework for doing flanking manuvers
//        (especially with transports - they shouldn't always move directly)
//   - Provide the AI with a sense of spatial area.  This can be useful
//        in the helping the AI figure out which region to invade next and
//        help it avoid spreading itself out (ball-shaped countries are easier
//        to defend than drawn-out countries).
//
// Note: a 1000x1000 map will have around 46 AIRegions; ~20000 or 100x200 each
/*
enum EnumAIRegionType
{
	enum_Undefined = 0,
	enum_SATDeepTerritory = 1, // Self, Ally, Teammate
	enum_SATIntermediateArea = 2,
	enum_SATBorderArea = 3,
	enum_NoMansLand = 4,
	enum_ConflictZone = 5,
	enum_OtherPlayerBorderArea = 6, // (Neutral or Enemy) Which player?
	enum_OtherPlayerIntermediateArea = 7, // (Neutral or Enemy) Which player?
	enum_OtherPlayerDeepTerritory = 8, // (Neutral or Enemy) Which player?
};
*/
class DLLIMPEXP CEOSAIMultiRegion2 : public CEOSAIGenericRegion
{
	public:
		CEOSAIMultiRegion2( CEOSAIMultiRegionManager2* pManager, CEOSAIRegion2* pCenterAIRegion, long iId );
		~CEOSAIMultiRegion2();

		CEOSAIMultiRegionManager2*  GetMultiRegionManager(){ return m_pMultiRegionManager; }

		// Initialization
		//
		void  GrowRegion();
		void  ReCenter();
		void  RemoveRegion( CEOSAIRegion2* pAIRegion );
		long  GetSize();

		//void  UpdateDistancesBasedOnPoiObjects( CPoiObject* pPoiObject );
		void  AddAIRegion( CEOSAIRegion2* pAIRegion ){ m_AIRegions.AddTail( pAIRegion ); }

		// 1.0=Deep in enemy territory
		//   This can be used for detecting which areas are my border areas, and need to be protected
		//bool  GetPotentialFor
		float GetDanger01( long iPlayerViewpoint );
		//EnumAIRegionType  GetMultiRegionType( long iPlayerViewpoint );
		// 1.0=Deep in my territory, 0.5=No man's land, 0.0=Deep in enemy territory
		float GetSATOwnershipLevel01( long iPlayerViewpoint );

		//
		CEOSAIMultiRegionManager2* m_pMultiRegionManager;
		CEOSAIRegion2*             m_pCenterAIRegion;
		CList< CEOSAIRegion2* >    m_AIRegions;

		float m_OwnerControl01[ EOSAI_MAX_NUMBER_OF_PLAYERS+1 ]; // This is a modified value

		// Used by the AI to cache pathways (originally in AIDesireSpatial)
		CEOSAIRegionPathwayMap* InvokePathToEverywhere();
		CEOSAIRegionPathwayMap  m_PathwayToEverywhereResult; // will contain pathways from everywhere to [here]

		CEOSAIRegionPathwayMap* InvokeGroundUnitAndSeaTransportPathToEverywhere( CEOSAIBrain* pAIBrain );
		CEOSAIRegionPathwayMap  m_GroundUnitAndSeaTransportPathToEverywhere;
};
