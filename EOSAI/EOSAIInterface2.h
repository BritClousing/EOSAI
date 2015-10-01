
#pragma once

//
// This class is *not* compiled into the AI dll.
// CEOSAIInterface is used by the EOS (Empires of Steel) application.  It is included in the source as an example of how to build
//   a game-to-AI interface.
//
// The EOSAIInterface code:
//   It creates the objects used by the AI (you have to customize it for your own application).
//     The CreateAIObjects() method needs to:
//       - Create a list of players in the game
//       - Create a list of technologies the AI can research
//       - Construct a list of buildable objects (e.g. what can the AI cities build?)
//       - Construct the AIUnitTemplates which describe the abilities of units (e.g. the "Tank" has these movement and combat capabilities...)
//       - Add all the game objects to the AI data (this means cities, resource, airfields, units, etc)
//       - Construct a map so that the AI can do pathfinding.  (This includes setting up AIRegions and Geos.)
//       - Pass the AI a distance tool so that the AI can figure out distances
//   Once the AI is done processing, the data will exist in this object for export.
//
// An example of using the EOSAIInterface in the game code:
//
//   CAIInterface  AIInterface;
//   YourCode()
//   {
//     AIInterface.CreateAIObjects();
//     AIInterface.TriggerAIProcessing();
//   }
//
//   OccasionallyCheckIfTheAIIsDoneProcessing()
//   {
//     if( AIInterface.DataHasBeenCopiedToGameObjects() == false && AIInterface.ProcessingIsDone() )
//     {
//       AIInterface.UpdateGameObjectsUsingAIObjects();
//     }
//   }
//
//#include "..\EOSAI\Include\AIInterface.h"
#include "EOSAI\Include\EOSAIInterface.h"
class CAIRoad;
class CWorldDescServer;

class CEOSAIInterface2 : public CEOSAIInterface
{
	public:
		CEOSAIInterface2()
		{
			m_bNeedToRebuildData = true;
			m_bAIObjectsHaveBeenCreated = false;
			m_bProcessingIsDone = false;
			m_bDataHasBeenCopiedToGameObjects = false;
			//
			//m_pAIWorldDistanceTool = NULL;
		}

		// Setup
		//
			// Set the AIWorldDistanceTool, etc
			//void Setup( CAIWorldDistanceTool* p )
			//{
			//	m_pAIWorldDistanceTool = p;
			//}
			//	EOSAI.SetAIWorldDistanceTool( &AIWorldDistanceTool );

		// Status
		//
			//bool AIObjectsHaveBeenCreated(){ return m_bAIObjectsHaveBeenCreated; }
			//bool ProcessingIsDone(){ return m_bProcessingIsDone; }
			//bool DataHasBeenCopiedToGameObjects(){ return m_bDataHasBeenCopiedToGameObjects; }

		// Pre-AI Processing
		//
			// Turn the game objects into AIObjects that the AI can understand
			virtual void CreateAIObjects();
			void           CreateAICombatUnitTypes(){}
			void           CreateAIMovementUnitTypes(){}
			void           CreateAIUnitTemplates();
			void           CreateAIBuildingDescs(){}
			void           TurnPoiObjectsIntoAIPoiObjects();

			// Create the AIRegion map
			void GenerateAIRegions( CWorldDescServer* pWorldDescServer );
			//void   CreateRegionSeed( long iRegionId, long x, long y, CList< CAIRegionSeed* >* pRegionSeedList );
			//void   CreateRegion( long iRegionId, long x, long y, CAICity2* pAICity );
			//void   GrowRegionSeeds( CList< CAIRegionSeed* >& RegionSeedList );
			void   CalculateAIUnitCombatCapabilities();
			void   CalculateNationalSummaries();
			void   CalculateThoughtDatabases(){}
			void   AddObjectIdsToAIRegionsAndMultiRegions(){}
			void   CalculateNationwidePathways(){}
			void   CalculateAIRegionOwnership(){}
			void   CalculateMultiRegionOwnership(){}

			// Setup the AIRoads
			void SetupAIRoadConnections();
			void SetupAIRoadConnections( CAIRoad* pAIRoad );

		// Status of AI Processing
		//
			// Runs in its own thread
			//void TriggerAIProcessing(){ m_AIPlayerManager.Process(); }

		// Post-AI Processing
		//
			// Turn the AIObject orders into orders in the game-world
			//virtual void CreateOrders();
			virtual bool CreateAIUnitOrders( CAIUnit2PathwayResult* pAIUnitPathwayResult );

	//private:

		//CAIWorldDistanceTool* m_pAIWorldDistanceTool;

		bool m_bNeedToRebuildData;
		bool m_bAIObjectsHaveBeenCreated;
		bool m_bProcessingIsDone;
		bool m_bDataHasBeenCopiedToGameObjects;
		//CAIPlayerManager  m_AIPlayerManager;
};

extern CEOSAIInterface* g_pAIInterface;



