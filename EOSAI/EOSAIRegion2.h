
#pragma once

#ifdef CREATE_EOSAI_DLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

#include <afxtempl.h>
#include "EOSAIGenericRegion.h"
#include "EOSAILocation.h"
#include "EOSAIRegionPathwayMap.h"
#include "EOSAIBCArray.h"
//#include "Geo.h"

//class CAIThought_IWouldLikeToExploreTheRegion;
class CAISpatialMap;
class CEOSAIBrain;
class CEOSAIRegion2;
class CEOSAIPoiObject;
class CEOSAIUnit;
class CEOSAICity;
class CEOSAIForeignRelationsState;
//class CWorldDescServer;
class CEOSAIRegionManager2;

#define EOSAI_MAX_NUMBER_OF_PLAYERS 10

/*
class CAIRegionCoastLocation
{
	public:
		CAIRegionCoastLocation()
		{
			m_pLandRegion = NULL;
			m_pWaterRegion = NULL;
		}
		CEOSAIRegion2* m_pLandRegion;
		CEOSAIRegion2* m_pWaterRegion;
		CEOSAILocation  m_CoastLocationWater;
		CEOSAILocation  m_CoastLocationLand;
};
*/
class DLLIMPEXP CEOSAIRegion2 : public CEOSAIGenericRegion
{
	public:
		//CEOSAIRegion2(){ ASSERT(FALSE); }
		CEOSAIRegion2( CEOSAIGenericRegionManager* pRegionManager, CEOSAILocation Location, long iGeo, long iNewAIRegionId );
		virtual ~CEOSAIRegion2();

		void  SetLocation( CEOSAILocation Location );
		//void  SetLocationAndGeo( CEOSAILocation Location );

		CEOSAILocation GetLocation(){ return m_Location; }
		//CEOSAICity*   GetCity(){ return m_pAICity; }
		long      GetMultiRegionId(){ return m_iMultiRegionId; }

		void      SetRegionSize( long i ){ m_iRegionSize = i; }
		void      Add1ToRegionSize(){ m_iRegionSize++; ASSERT( m_iGeo > 0 ); }
		long      GetRegionSize(){ return m_iRegionSize; }

		//void  Clear();
		//virtual void  ClearPoiData(){ CEOSAIGenericRegion::ClearPoiData(); }

		// Use a typical ground unit and typical sea-transport for this calculation
		void               ClearGroundUnitAndSeaTransportPathToEverywhere();
		CEOSAIRegionPathwayMap* InvokeGroundUnitAndSeaTransportPathToEverywhere( CEOSAIBrain* pAIBrain );

		// 0.0-0.2 = deep in safe territory (me + allies)
		// 0.2-0.4 = my border territory
		// 0.4-0.6 = uncontrolled area (e.g. open waters)
		// 0.6-0.8 = neutral/enemy border territory
		// 0.8-1.0 = deep in neutral/enemy player territory
		//void  UpdateAreaCitResOwnerBalance();
		//float GetCitResOwnerBalance( long iAccordingToPlayer, CEOSAIForeignRelationsState* pState );

		//void  UpdateBorderProximity();
		//float GetBorderProximity( long iPlayer ){ return 0.0f; }//return m_fBorderProximity; }

		CEOSAIRegionPathwayMap  m_GroundUnitAndSeaTransportPathToEverywhere;

		//
		long  m_iMultiRegionId;

		long  m_iMinX;
		long  m_iMaxX;
		long  m_iMinY;
		long  m_iMaxY;

	private:
		long  m_iRegionSize;
};
