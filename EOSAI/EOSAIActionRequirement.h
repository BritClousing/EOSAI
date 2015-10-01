
#pragma once

#include "EOSAIUnitTemplate.h"
#include "EOSAIMathFunction.h"
#include "EOSAIUnit2Summary.h"
//#include "EOSAIMultiRegion2.h"
class CEOSAIMultiRegion2;
#include "EOSAILongSet.h"

// Thought Requirements define a need (e.g. a transport) and 
//   some general details (from multiregions a,b,c to multiregion x,y,z)
// The ThoughtRequirements interact with the ThoughtDatabase, can (if chosen for 
//   future fulfillment) increase the desire to build the required unit for future use.

class CEOSAIActionRequirement
{
	public:
		enum EnumUnitAction
		{
			enum_Undefined,
			// Transport
			enum_GroundTransport,  // Sea or Air-based transport
			// CitRes Capture
			enum_GroundCitResHunter,
			enum_SeaResHunter,
			// Combat
			enum_AntiSoftCombat, // eg ground units, tactical bombers/missiles, ship bombardment, ...
			enum_AntiHardCombat,
			enum_AntiShipCombat,
			enum_AntiSubmarineCombat,
			enum_StrategicBombing,
			//
			enum_AircraftBasing,   // Airfield or Aircraft Carrier
		};

		CEOSAIActionRequirement()
		{
			m_eUnitActionNeeded = enum_Undefined;
		}

		void AddRequiredGeo( long iGeo ){ m_UnitMustBeInGeo.Add( iGeo ); }

		// To what degree would this Unit satisfy my need?
		//virtual float  GetUnitTemplateValue( CEOSAIUnitTemplate* pAIUnitTemplate ){ ASSERT( false ); return 0.0f; }

		// Sooner=better.  In some cases (like enemies threatening something), sooner is premium
		//void   SetTimeValue( float fInput, float fOutput ){ m_TimeFunction.SetInputOutput( fInput, fOutput ); }
		//float  GetTimeValue( float fArrivalTime ){ return 0.0f; }

		CEOSAILongSet  m_UnitMustBeInGeo; // usually just one geo, but it can be multiple (in the case of transports)
		//long  m_iRequiredGeo;
		EnumUnitAction  m_eUnitActionNeeded;
		//CAIFunction     m_TimeFunction;
};

class CEOSAIActionRequirement_GroundTransport : public CEOSAIActionRequirement
{
	public:
		CEOSAIActionRequirement_GroundTransport(){ m_eUnitActionNeeded = enum_GroundTransport; }

		CList< CEOSAIMultiRegion2* >  m_StartingMultiRegions;
		CList< CEOSAIMultiRegion2* >  m_EndingMultiRegions;

		// Varies from 1 to n (if this is a dangerous area, I might want to bring more 
		//   than one transport in case one gets destroyed)
		CEOSAIMathFunction  m_NumberOfTransportsNeeded; 
};

class CEOSAIActionRequirement_AntiGroundCombat : public CEOSAIActionRequirement
{
	public:
		CEOSAIActionRequirement_AntiGroundCombat(){ m_eUnitActionNeeded = enum_AntiSoftCombat; }

		CList< CEOSAIMultiRegion2* >  m_TargetMultiRegions;
		CEOSAIUnit2Summary          m_EnemyUnitSummary; // When I need to counter an enemy force
};


class CEOSAIActionRequirement_StrategicBombing : public CEOSAIActionRequirement
{
	public:
		CEOSAIActionRequirement_StrategicBombing(){ m_eUnitActionNeeded = enum_StrategicBombing; }

		CList< CEOSAIMultiRegion2* >  m_TargetMultiRegions;
};

class CEOSAIActionRequirement_GroundCitResHunters : public CEOSAIActionRequirement
{
	public:
		CEOSAIActionRequirement_GroundCitResHunters(){ m_eUnitActionNeeded = enum_GroundCitResHunter; }
		void  AddTargetMultiRegion( CEOSAIMultiRegion2* pMultiRegion ){ m_TargetMultiRegions.AddTail( pMultiRegion ); }

		CList< CEOSAIMultiRegion2* >  m_TargetMultiRegions;
};

class CEOSAIActionRequirement_SeaResHunters : public CEOSAIActionRequirement
{
	public:
		CEOSAIActionRequirement_SeaResHunters(){ m_eUnitActionNeeded = enum_SeaResHunter; }

		CList< CEOSAIMultiRegion2* >  m_TargetMultiRegions;
};

