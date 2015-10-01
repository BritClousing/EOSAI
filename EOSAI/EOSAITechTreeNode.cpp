
#include "stdafx.h"
#include "EOSAITechTreeNode.h"
//#include "BCXMLReader.h"
//#include "Unitset.h"
#include "EOSAIGameRules.h"
#include "EOSAISerial.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CEOSAITechTreeNode::CEOSAITechTreeNode( EOSAI::CGameRules* pGameRules )
{
	m_pGameRules = pGameRules;
	pGameRules->AddTechTreeNode( this );
	//m_iHeightOfMeAndMyChildren = 1;
	m_pParentNode = NULL;
}
/*
void  CTechTreeNode::DisconnectFromParents()
{
	if( m_pParentNode )
	{
		m_pParentNode->DisconnectChild( this );
	}
	/*
	POSITION pos = m_ParentNodes.GetHeadPosition();
	while( pos )
	{
		CTechTreeNode* pTechTreeNode = m_ParentNodes.GetNext( pos );
		if( pTechTreeNode )
		{
			pTechTreeNode->DisconnectChild( this );
		}
	}
	*-/
	// Delete child objects
	while( m_ChildNodes.IsEmpty() == FALSE )
	{
		CTechTreeNode* pChild = m_ChildNodes.RemoveHead();
	//pos = m_ChildNodes.GetHeadPosition();
	//while( pos )
	//{
		//CTechTreeNode* pChild = m_ChildNodes.GetNext( pos );
		//pChild->DisconnectChild();
		pChild->DisconnectFromParents();
		delete pChild;
	}
}
*/

/*
void  CTechTreeNode::DisconnectChild( CTechTreeNode* pTechTreeNode )
{
	POSITION pos = m_ChildNodes.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		CTechTreeNode* pTechTreeNodeInList = m_ChildNodes.GetNext( pos );
		if( pTechTreeNodeInList == pTechTreeNode )
		{
			m_ChildNodes.RemoveAt( prevPos );
		}
	}
}
*/
void  CEOSAITechTreeNode::RemoveChild( CEOSAITechTreeNode* pChildNode )
{
	POSITION pos = m_ChildNodes.GetHeadPosition();
	while( pos )
	{
		POSITION prevPos = pos;
		CEOSAITechTreeNode* pTechTreeNodeInList = m_ChildNodes.GetNext( pos );
		if( pTechTreeNodeInList == pChildNode )
		{
			m_ChildNodes.RemoveAt( prevPos );
		}
	}
}

void  CEOSAITechTreeNode::Serialize( CEOSAISerial* pSerial )
{
	char cVersion = 1;
	pSerial->Serialize( cVersion );
	//
	pSerial->SerializeANSI8( m_strTechName );

	long iCount = m_ChildNodes.GetCount();
	pSerial->Serialize( iCount );
	POSITION pos = m_ChildNodes.GetHeadPosition();
	while( pos )
	{
		CEOSAITechTreeNode* pChild = m_ChildNodes.GetNext( pos );
		//pChild->AppendDataToXMLString( strData );
		pChild->Serialize( pSerial );
	}
}

void  CEOSAITechTreeNode::Deserialize( CEOSAISerial* pSerial )
{
	char cVersion = 1;
	pSerial->Deserialize( cVersion );
	//
	pSerial->DeserializeANSI8( m_strTechName );

	long iChildren;
	pSerial->Deserialize( iChildren );
	for( long i=0; i<iChildren; i++ )
	{
		CEOSAITechTreeNode* pChild = new CEOSAITechTreeNode( m_pGameRules );
		pChild->m_pParentNode = this;
		pChild->Deserialize( pSerial );
		this->m_ChildNodes.AddTail( pChild );
	}
}


