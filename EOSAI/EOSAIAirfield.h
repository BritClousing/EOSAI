
#pragma once

#ifdef CREATE_EOSAI_DLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

#include "PoiObject.h"
//#include "Airfield.h"
class CEOSAIBrain;
class CAirfield;

class DLLIMPEXP CEOSAIAirfield : public CEOSAIPoiObject
{
	public:
		CEOSAIAirfield();
		virtual ~CEOSAIAirfield();

		//virtual void  SetServerPoiObject( CPoiObject* pPoiObject );
		//virtual void  SetPlayerPoiObject( CPoiObject* pPoiObject );

		//CAirfield*          GetServerAirfield(){ return m_pServerAirfield; }
		//virtual CPoiObject* GetServerPoiObject();
		//virtual long        GetObjectId();

		virtual bool  CanContain_IgnoreForeignRelations( CEOSAIPoiObject* pAIPoiObject );

		void          CalculateUnexploredArea();
		//virtual float GetUnexploredPercent( float fRange );

	protected:
		//void AddRegion( float fDistance, long iSize );
		//void AddUnexploredRegion( float fDistance, long iSize );

//		CAirfield* m_pServerAirfield;
/*
		float  m_fUnexploredArea_Range100;
		float  m_fUnexploredArea_Range150;
		float  m_fUnexploredArea_Range200;
		float  m_fUnexploredArea_Range250;
		float  m_fUnexploredArea_Range300;
		float  m_fUnexploredArea_Range400;
		float  m_fUnexploredArea_Range500;
		float  m_fUnexploredArea_Range600;
		float  m_fUnexploredArea_Range700;
		float  m_fUnexploredArea_Range800;
		float  m_fUnexploredArea_Range900;
		float  m_fUnexploredArea_Range1000;
		float  m_fUnexploredArea_Range1200;
		float  m_fUnexploredArea_Range1500;

		float  m_fArea_Range100;
		float  m_fArea_Range150;
		float  m_fArea_Range200;
		float  m_fArea_Range250;
		float  m_fArea_Range300;
		float  m_fArea_Range400;
		float  m_fArea_Range500;
		float  m_fArea_Range600;
		float  m_fArea_Range700;
		float  m_fArea_Range800;
		float  m_fArea_Range900;
		float  m_fArea_Range1000;
		float  m_fArea_Range1200;
		float  m_fArea_Range1500;
		*/
};

