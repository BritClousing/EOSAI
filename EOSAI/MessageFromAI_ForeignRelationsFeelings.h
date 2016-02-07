#pragma once

#include <assert.h>
#include "Message.h"
#include "EOSAIGlobalForeignRelations.h"
#include "EOSAIBCDumbArray2D.h"

namespace EOSAI
{
	class MessageFromAI_ForeignRelationsFeelings : public Message
	{
		public:
			void Set(CEOSAIGlobalForeignRelations* pRel)
			{
				//ASSERT(false); // Need to test this

				long iSizeX = pRel->GetForeignRelations()->m_iSizeX;
				long iSizeY = pRel->GetForeignRelations()->m_iSizeY;
				m_Feelings.SetSize(iSizeX, iSizeY);
				for (int x = 0; x < iSizeX; x++)
				{
					for (int y = 0; y < iSizeY; y++)
					{
						m_Feelings.Value(x,y) = pRel->GetFeelings()->Value(x, y);
					}
				}
			}

			CEOSAIBCDumbArray2D< float >   m_Feelings;
	};
}
