
#pragma once

#ifdef _USRDLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

#include "ResourceAmounts.h"
class CEOSAIUnitTemplate;
//class CGLTexture;
//class CMaskedBitmap;
class CEOSAIBuildingDescription;
namespace EOSAI
{
	class CGameRules;
	//class ResourceAmounts;
};
//class CEOSAISerial;

class DLLIMPEXP CEOSAIBuildOption
{
	public:

		enum Type
		{
			Undefined,
			Money,
			Unit,
			Building
		};

		CEOSAIBuildOption()
		{
			m_eType = Undefined;
			m_pAIGameRules = NULL;
			//m_fProductionCost = 0.0f;
			m_pAIUnitTemplate = NULL;
			m_pAIBuildingDescription = NULL;
		}
		~CEOSAIBuildOption(){}

		//void Serialize( CEOSAISerial* pSerial );
		//void Deserialize( CEOSAISerial* pSerial );

		void   SetType( CEOSAIBuildOption::Type eType ){ m_eType = eType; }

		bool   IsMoney(){ return m_eType == Money; }// == "<Money>"; }
		bool   IsAUnit(){ return m_eType == Unit; }//m_strBuildTarget.Find( "<" ) == -1; }
		bool   IsABuilding(){ return m_eType == Building; }

		void   SetGameRules( EOSAI::CGameRules* pAIGameRules ){ m_pAIGameRules = pAIGameRules; }

	//	void   SetBuildTargetToMoney(){ m_eType = Money; m_strBuildTarget = "<Money>"; }
	//	void   SetBuildTargetToUnit( CEOSAIUnitTemplate* pAIUnitTemplate );
	//	void   SetBuildTargetToBuilding( CEOSAIBuildingDescription* pAIBuildingDescription );

		//void           SetBuildTarget( CString strBuildTarget ){ m_strBuildTarget = strBuildTarget; }
		//CString                GetBuildTarget(){ return m_strBuildTarget; }
		//CUnitTemplate*         GetUnitTemplate();
		CEOSAIUnitTemplate*        GetAIUnitTemplate(){ return m_pAIUnitTemplate; }
		void                       SetAIUnitTemplate( CEOSAIUnitTemplate* p ){ m_pAIUnitTemplate = p; }
		CEOSAIBuildingDescription* GetBuildingDescription(){ return m_pAIBuildingDescription; }
		void                       SetBuildingDescription( CEOSAIBuildingDescription* p ){ m_pAIBuildingDescription = p; }

		bool  CanOnlyBeBuiltInCitiesAdjacentToWater(); // Example: Ships can only be built in locatins next to water

	//	void           SetGrayMBitmap( CString strFilename );
	//	CMaskedBitmap* GetGrayMBitmap();

	//	void           SetGLTexture( CGLTexture* pGLImage ){ m_pGLTexture = pGLImage; }
	//	CGLTexture*    GetGLTexture();

		float    GetProductionCost();//{ ASSERT( false ); return m_fProductionCost; }
		float    ProductionNeeded();

	//	void     Enable( bool bVal );

		CString  GetInternalName();
	//	CString  GetNameText();
	//	CString  GetTimeText( float fPlayerProduction, float fCityProduction );
	//	CString  GetProductionCostText();
	//	float    GetProductionCost(){ return (float) ProductionNeeded(); }
	//	CString  GetIronCostText();
	//	float    GetIronCost();

		//float    GetResourceCost(CString strResource);
		EOSAI::ResourceAmounts  GetResourceProductionPerTurn(float fCityProduction);
		EOSAI::ResourceAmounts  GetResourceConsumptionPerTurn(float fCityProduction);

		//static float   GetProductionToCashConversion(){ return 1.0f; }
		//static float   GetProductionToCashConversion(){ return 0.5f; }

		//CString  GetHtmlDesc();
	//	CString  GetTextDescription();

		// All requirements must evaluate to TRUE in order for a player
		//   to use this build option.
		//void     AddRequirement( CBuildRequirement* pRequirement );

	//	void            SetId( DWORD id ){ m_dwId = id; }
	//	DWORD           GetId(){ return m_dwId; }

	private:
		// Local information
	//	DWORD            m_dwId; // used in the city dialog, assigned by GlobalState
	//	CMaskedBitmap*   m_pGrayMBitmap;
	//	CGLTexture*      m_pGLTexture;

		// Global information
		EOSAI::CGameRules*  m_pAIGameRules;
		//CUnitset*  m_pUnitset;
		//CString      m_strBuildTarget;

		CEOSAIBuildOption::Type  m_eType; // Money, Unit, or City Improvement

		//float  m_fProductionCost;

		// Cache
		CEOSAIUnitTemplate*        m_pAIUnitTemplate;
		CEOSAIBuildingDescription* m_pAIBuildingDescription; // City Improvements

		//CString CreateHtml( CString strTitle, CString strImage, CString strDescription );
};
