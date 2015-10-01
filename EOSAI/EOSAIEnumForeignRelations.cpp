
#include <stdafx.h>
#include "EOSAIEnumForeignRelations.h"


void  EOSAIEnumForeignRelations::Serialize( CEOSAISerial* pSerial )
{
	char val = m_v;
	pSerial->Serialize( val );
}

void  EOSAIEnumForeignRelations::Deserialize( CEOSAISerial* pSerial )
{
	char val;
	pSerial->Deserialize( val );
	m_v = (EOSAIEnumForeignRelations::Value) val;
}
