
#pragma once

#include "EOSAIJobsToDo.h"
#include "EOSAIJobCapability.h"
class CAIPlayer;

class CEOSAIJobSolution
{
	public:
		CEOSAIJobSolution( CEOSAIJobsToDo* pAIJobsToDo = NULL, CEOSAIJobCapability* pAIJobCapability = NULL )
		{
			m_pAIJobsToDo = pAIJobsToDo;
			m_pAIJobCapability = pAIJobCapability;
			//
			m_fTransportNeed = 0;
			m_fCarrierNeed = 0;
			m_fExpandJobSignificance = 1.0f;
			m_fCombatJobSignificance = 1.0f;
		}

		void  Clear();

		// The AIPlayer information is used to favor certain types of units over others
		//   (If the AI is in the 'expansion' phase of the game, then favor CitRes Hunters.)
		//   This is important for 
		//void  SetAIPlayer( CAIPlayer* pAIPlayer ){ m_pAIPlayer = pAIPlayer; }
		void  MultiplyExpandJobSignificance( float f );
		void  MultiplyConquestJobSignificance( float f );

		CEOSAIJobsToDo*       GetAIJobsToDo(){ return m_pAIJobsToDo; }
		CEOSAIJobCapability*  GetAIJobCapability(){ return m_pAIJobCapability; }

		void  SetAIUnitNeed( CEOSAIJobsToDo* pAIJobsToDo ){ m_pAIJobsToDo = pAIJobsToDo; }
		void  SetAIUnitCapability( CEOSAIJobCapability* pAIJobCapability ){ m_pAIJobCapability = pAIJobCapability; }
		//
		bool  CanFulfillAJob( CEOSAIUnitTemplate* pActor );
		float GetCurrentCapabilityNeed();
		float GetCurrentCapabilityValue();
		float GetValueOfAddingToCapability( CEOSAIUnitTemplate* pActor );
		//
		void  SetTransportNeed( float i ){ m_fTransportNeed = i; }
		void  AddTransportNeed( float i ){ m_fTransportNeed += i; }
		float GetTransportNeed(){ return m_fTransportNeed; }
		void  SetCarrierNeed( float i ){ m_fCarrierNeed = i; }
		void  AddCarrierNeed( float i ){ m_fCarrierNeed += i; }
		float GetCarrierNeed(){ return m_fCarrierNeed; }

		//bool  CanFulfillAJob( CUnitTemplate* pActor ){ return CanFulfillANonCombatJob( pActor ) || CanFulfillACombatJob( pActor ); }
		//bool  CanFulfillACombatJob( CUnitTemplate* pActor );
		//bool  CanFulfillANonCombatJob( CUnitTemplate* pActor );
		//bool  CanFulfillANonCombatJob( CEOSAIUnit2Capability* pAIUnitCapability );
		//
		

		//bool  UnitIsValuableForAccomplishingNeeds_IgnoreCapability( CUnitTemplate* pUnitTemplate );
		//bool  UnitIsValuableForAccomplishingNeeds_AboveCurrentCapability( CUnitTemplate* pUnitTemplate );
		//
		//void  Calculate();
		//float GetSeedValue(); 

	private:

		float  m_fExpandJobSignificance;
		float  m_fCombatJobSignificance;

		//CAIPlayer*         m_pAIPlayer;
		CEOSAIJobsToDo*       m_pAIJobsToDo;
		CEOSAIJobCapability*  m_pAIJobCapability;

		// Derived Needs
		//   If I need to destroy an enemy GroundUnit, it's not clear that I need a Transport or Carrier
		//   On the other hand, if I need a CitRes hunter, I can tell very quickly if I need a transport
		//
			float  m_fTransportNeed; // How many ground units do I need to move? (One transport = 2,3,...)
			float  m_fCarrierNeed;

};

