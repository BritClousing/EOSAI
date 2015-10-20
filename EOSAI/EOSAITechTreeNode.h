
#pragma once
//class CBCXMLItem;
//class CUnitset;
class CEOSAISerial;
namespace EOSAI
{
	class CGameRules;
}

#ifdef CREATE_EOSAI_DLL
#define DLLIMPEXP __declspec(dllexport)
#else
#define DLLIMPEXP __declspec(dllimport)
#endif

class DLLIMPEXP CEOSAITechTreeNode
{
	public:
		CEOSAITechTreeNode( EOSAI::CGameRules* pGameRules );

		//void  DisconnectFromParents();
		//void  DisconnectChild( CTechTreeNode* pTechTreeNode );
		void  RemoveChild( CEOSAITechTreeNode* pChildNode );

		void  Serialize( CEOSAISerial* pSerial );
		void  Deserialize( CEOSAISerial* pSerial );

		//void  AppendDataToXMLString( CStringANSI8& strData );
		//void  ReadXMLData( CBCXMLItem* pXMLItem );

		//void  CalculateHeightOfChildren();

		EOSAI::CGameRules* m_pGameRules;
		//CUnitset* m_pUnitset;
		CString   m_strTechName;
		//long      m_iHeightOfMeAndMyChildren;
		CList< CEOSAITechTreeNode* >  m_ChildNodes;
		//CList< CTechTreeNode* >  m_ParentNodes;
		CEOSAITechTreeNode*  m_pParentNode;  // Except for the head-TechTreeNodes, all have one parent
};


