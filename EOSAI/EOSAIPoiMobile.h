
#pragma once

#ifdef _USRDLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

#include "PoiObject.h"
//class CEOSAILogicalAction;
class CEOSAIRegion2;
#include "EOSAILimitedList.h"
#include "EOSAIListSort.h"
#include "EOSAIListUnowned.h"
//#include "AIDefinedAction.h"
#include "EOSAIEnumTypeOfUnit.h"
//#include "AIPoiMobileState.h"
//class EOSAI::UnitTask;
class CEOSAICity;
class CEOSAIPoiMobile;
namespace EOSAI
{
	class PoiObjectState;
}
#include "EOSAIEnumMovementType.h"
#include "EOSAIEnumFieldOrders.h"
class CEOSAIMultiRegion2;
class CEOSAIUnitTemplate;
//#include "AIPoiObjectStateProgression.h"

/*
class CMultiRegionMovementStep
{
	public:
		CMultiRegionMovementStep(){ m_pMultiRegion = NULL; m_fTime = 0.0f; }

		CMultiRegion*  m_pMultiRegion;
		float          m_fTime;
};

class CMultiRegionMovementPathway
{
	public:
		CMultiRegionMovementPathway(){}
		~CMultiRegionMovementPathway()
		{
			while( m_Steps.IsEmpty() == FALSE ){ delete m_Steps.RemoveHead(); }
		}
		CList< CMultiRegionMovementStep* >*  GetSteps(){ return &m_Steps; }
		//
		CList< CMultiRegionMovementStep* >   m_Steps;
};
*/
namespace EOSAI
{
class DLLIMPEXP PoiMobile : public CEOSAIPoiObject
{
public:
	PoiMobile();
	virtual ~PoiMobile();

	// Initialization
	//virtual void  SetServerPoiObject( CPoiObject* pPoiObject );
	//virtual void  SetPlayerPoiObject( CPoiObject* pPoiObject );
	//void         SetPoiMobile( CPoiMobile* pPoiMobile );
	//CPoiMobile*  GetServerPoiMobile(){ return m_pServerPoiMobile; }
	//CPoiMobile*  GetPlayerPoiMobile();
	//
	void  SetInitialAirbase(CEOSAIPoiObject* pNewAirbase);

	//
	bool  IsOfType(EOSAIEnumTypeOfUnit eTypeOfUnit);

	virtual bool IsMobile() { return true; }

	// Initialization
	//void CreateTypeInformation();
	//virtual void SetInitialContainer( CAIPlan* pAIPlan, long iContainer );
	//virtual long GetObjectId();

	// This is used by the AI to help decide the valuableness of destroying the unit
	//    This *should* include iron cost, iron value, the threat that it poses to
	//    my units (like nearby transports), cities, and airports.
	//float GetAIDestructionValue();

	// Containment
	virtual bool CanContain_IgnoreForeignRelations(CEOSAIPoiObject* pAIPoiObject);
	virtual bool CanContain_IgnoreForeignRelations(CEOSAIUnitTemplate* pUnitTemplate);
	virtual long GetNumberOfGroundUnitsThatICanContain();
	virtual long GetNumberOfAirUnitsThatICanContain();
	//
	virtual bool CanCaptureCities();

	//
	virtual float               GetMovementRate();
	virtual float               GetMovementLimit();
	bool                        IsWithinRange(CEOSAILocation loc); // if I appended move-to loc, can I return?
	//bool                      IsRangedUnit();
	virtual EOSAIEnumMovementType3 GetMovementType();
	virtual bool                IsAirUnit() { return GetMovementType() == EOSAIEnumMovementType3_Air; }
	virtual bool                IsGroundUnit() { return GetMovementType() == EOSAIEnumMovementType3_Land; }
	virtual bool                IsSeaUnit() { return GetMovementType() == EOSAIEnumMovementType3_Water; }
	virtual float               GetMovementConsumed();
	virtual bool                HasARange();

	//CUnit*            GetUnit(){ return m_pUnit; }
	//CUnitTemplate*    GetUnitTemplate(){ return m_pUnitTemplate; }

	float  GetFinalDistanceTravelled(); // MovementConsumed + MovementPlanned

	void   ValidateFinalStateOfActionsAndTasks();

	// Orders
	//
	EOSAI::EnumFieldOrders  GetFieldOrders() { return m_eFieldOrders; }
	void                    SetFieldOrders(EOSAI::EnumFieldOrders e) { m_eFieldOrders = e; }

	//void DeleteOrders();//{ ASSERT( false ); }


//virtual CAIPoiObjectStateAndActions* GetFinalStateAccordingToActions();
//virtual EOSAI::PoiObjectState*           GetFinalStateAccordingToTasks();
//CAIPoiObjectStateAndActions*         GetEndStateOfPreviousAction( CAIDefinedAction* pAction );
//
// Region Pathway
//
//virtual void UpdateAIRegionPathway();

//
// Add an AIUnitTask
//
//virtual void AppendTask( CEOSAITask* pNewTask );
//virtual void InsertTask( long iPosition, EOSAI::UnitTask* pTask );  // 0 = first, -1 = last
//void         InsertTaskBefore( EOSAI::UnitTask* pPostTask, EOSAI::UnitTask* pNewTask );
//void         InsertTaskAfter(  EOSAI::UnitTask* pPreTask, EOSAI::UnitTask* pNewTask );
//virtual bool CreateOrders();
//virtual void UpdateTaskProgression();

//void SetCheckpoint( CAISymbolProcessor* Proc );
//void RestoreToCheckpoint( CAISymbolProcessor* Proc );

	EOSAI::EnumFieldOrders  m_eFieldOrders;

	//
	//CEOSAICity* m_pParentAICity;  // used only if the unit isn't built yet
	//CPoiMobile*  m_pServerPoiMobile;

	//CList< CEOSAIRegion2* >           m_TraversedRegionList;
	//CMultiRegionMovementPathway  m_ScheduledMultiRegionPathway;

	// Poi Mobiles should remember a few potential actions
	//CEOSAILimitedList< CAIDefinedAction* >  m_DecentActionsIAmConsidering;
};
}