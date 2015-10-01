
#pragma once

#include "EOSAISerial.h"

//
// The EOSAIEnumForeignRelations acts like an enum with some additional functions.
// For example, it allows if statements like: "if( ForeignRelations > EOSAIEnumForeignRelations::enum_Neutral ){ .. }"
//

class EOSAIEnumForeignRelations
{
	public:
		enum Value
		{
			enum_Undefined = 0,
			enum_War = 1,
			enum_Neutral = 2,
			enum_Alliance = 3,
			enum_Teammate = 4,
			enum_Self = 5
		};

	public:

		EOSAIEnumForeignRelations(){ m_v = enum_Undefined; }
		EOSAIEnumForeignRelations( EOSAIEnumForeignRelations::Value val ){ m_v = val; }

		void  operator=( EOSAIEnumForeignRelations::Value val ){ m_v = val; }
		bool  operator==( EOSAIEnumForeignRelations::Value val ){ return m_v == val; }
		bool  operator!=( EOSAIEnumForeignRelations::Value val ){ return m_v != val; }
		bool  operator>=( EOSAIEnumForeignRelations::Value val ){ return m_v >= val; }
		bool  operator> ( EOSAIEnumForeignRelations::Value val ){ return m_v >  val; }
		bool  operator<=( EOSAIEnumForeignRelations::Value val ){ return m_v <= val; }
		bool  operator< ( EOSAIEnumForeignRelations::Value val ){ return m_v <  val; }

		Value GetValue(){ return m_v; }

		//bool  IsAlly(){ return m_v == enum_PermanentAlliance || m_v == enum_Alliance; }
		bool  IsTeammate(){ return m_v == enum_Teammate; }
		bool  IsAlly(){ return m_v == enum_Alliance; }
		bool  IsNeutral(){ return m_v == enum_Neutral; }
		bool  IsEnemy(){ return m_v == enum_War; }

		void  Serialize( CEOSAISerial* pSerial );
		void  Deserialize( CEOSAISerial* pSerial );

		Value m_v;
};

