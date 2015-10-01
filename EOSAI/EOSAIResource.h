
#pragma once

#ifdef _USRDLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

#include "PoiObject.h"
//class CResourceSource;
//class CAIThought_ShouldICaptureThatCitRes;
class CEOSAIBrain;

class DLLIMPEXP CEOSAIResource : public CEOSAIPoiObject
{
	public:
		CEOSAIResource();
		virtual ~CEOSAIResource(){}
		//
		//virtual void  SetServerPoiObject( CPoiObject* pPoiObject );
		//virtual void  SetPlayerPoiObject( CPoiObject* pPoiObject );
		//
		//CResourceSource*    GetResource(){ return m_pServerResource; }
		//virtual CPoiObject* GetServerPoiObject(){ return (CPoiObject*) m_pServerResource; }

		void     SetResourceExternalText( CString str ){ m_strResourceExternalText = str; }
		CString  GetResourceExternalText(){ return m_strResourceExternalText; }// ASSERT( false ); return _T("Resource"); }
		void     SetResourceType( CString str ){ m_strResourceType = str; }
		CString  GetResourceType(){ return m_strResourceType; }
		void     SetResourcePerTurn( float f ){ m_fResourceProduction = f; }
		float    GetResourcePerTurn(){ return m_fResourceProduction; }

		float    GetAIValueOfResource( long iAccordingToPlayer );

		//bool  ResourceSource_IsOnLand(){ ASSERT( false ); return false; }
		void  SetResourceSource_IsOnLand( bool b ){ m_bResourceSource_IsOnLand = b; }
		bool  ResourceSource_IsOnLand(){ return m_bResourceSource_IsOnLand; }

		//virtual long  GetObjectId();

	// Thinking
	//
		//virtual void CreateSomeThoughtsAboutThisObject();

	protected:
		// Must link to an actual resource - this includes production information
		//CResourceSource*  m_pServerResource;
		CString  m_strResourceExternalText;
		CString  m_strResourceType;
		float    m_fResourceProduction;

		bool     m_bResourceSource_IsOnLand;
};
/*
class CAIResourceAmountTick
{
	public:
		void  Clear(){ m_fAmount = m_fDelta = m_fConsumption = m_fProduction = 0.0f; }

		float m_fAmount;
		float m_fDelta;
		float m_fConsumption; // Resources consumed by cities, units, player-player-trade, research
		float m_fProduction;  // Resources produced by resources, player-player-trade
};

class CAIResourceTracker
{
	public:
		CAIResourceTracker();

		void  AddDataPoint( float fAmount, float fDelta );

		float GetEstimatedDelta();
		float GetEstimatedAmount( float fTimeInTheFuture );
		float GetEstimatedShortageTime(); // Figure out when/if there will be a resource shortage

	private:
		CAIResourceAmountTick  m_Data[10];
		//CList< CAIResourceAmountTick* >  m_ResourceTicks;
};
*/