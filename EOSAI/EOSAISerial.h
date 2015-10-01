
#pragma once

/*
	CEOSAISerial is a container for serialized information.
	Serialization is necessary for passing information across networks and saving to a file.
*/

#include <map>
#include <vector>
class CWorldDescBase;
class CEOSAISerial;
typedef ATL::CStringT< char, StrTraitMFC_DLL<char> >  CStringANSI8;

#define DEFAULT_SERIAL_EXPAND_SIZE 500


class CBCEOSAISerialRec
{
	public:
		CBCEOSAISerialRec(){ m_iClassId = 0; }

		CStringANSI8 m_strClassId;
		long    m_iClassId;

		void* (*m_pInstanciateFunction)( CEOSAISerial* pSerial, CWorldDescBase* pWorldDesc );
		//void* (*m_pSetContext)( CWorldDescBase* pWorldDesc );
		//void  (*m_pSerializeFunction)( CEOSAISerial* pSerial );
		//long  (*m_pGetId)();
};

class CEOSAISerial
{
	public:
		enum EnumSerializedType
		{
			enumSerializedType_Undefined,
			enumSerializedType_MyPoi,
			enumSerializedType_MyPoiNoOrders,
			enumSerializedType_OtherPlayersPoi,
		};

	public:
		//static void Initialize();

		static void AddBCEOSAISerialObj( CStringANSI8 strClassId, 
									CStringANSI8* pClassId,
									void* (*pInstanciateFunction)( CEOSAISerial* pSerial, CWorldDescBase* pWorldDesc ) );
		static void AddBCEOSAISerialObj( CStringANSI8 strClassId,
									CStringANSI8 (*pGetCEOSAISerialClassId)(),
									void* (*pInstanciateFunction)( CEOSAISerial* pSerial, CWorldDescBase* pWorldDesc ) );
		static void AddBCEOSAISerialObj( long  iClassId, 
									long* pClassId,
									void* (*pInstanciateFunction)( CEOSAISerial* pSerial, CWorldDescBase* pWorldDesc ) );
		static void AddBCEOSAISerialObj( long  iClassId,
									long  (*pGetCEOSAISerialClassId)(),
									void* (*pInstanciateFunction)( CEOSAISerial* pSerial, CWorldDescBase* pWorldDesc ) );

		static CBCEOSAISerialRec* GetBCEOSAISerialRec( CStringANSI8 strClassId );
		static CBCEOSAISerialRec* GetBCEOSAISerialRec( unsigned long iClassId );

		static std::map< CStringANSI8, CBCEOSAISerialRec* >  s_ClassMap;  // used for string ClassIds
		static std::vector< CBCEOSAISerialRec* >     s_ClassVector;  // used for long ClassIds

	public:
		CEOSAISerial();
		~CEOSAISerial();

		void    SetCurrentLocation( long i );
		long    GetCurrentLocation();

		void    Serialize(   long     val );
		void    Deserialize( long&    val );
		void    Serialize(   DWORD    val );
		void    Deserialize( DWORD&   val );
		void    Peek(        long&    val );
		//void    Serialize(   unsigned long  val );
		//void    Deserialize( unsigned long& val );
		void    Serialize(   short    val );
		void    Deserialize( short&   val );
		void    Serialize(   unsigned short    val );
		void    Deserialize( unsigned short&   val );
		void    Serialize(   char     val );
		void    Deserialize( char&    val );
		void    Serialize(   unsigned char     val );
		void    Deserialize( unsigned char&    val );
		void    SerializeTCHAR(   CString  val );
		void    DeserializeTCHAR( CString& val );
		void    PeekTCHAR(        CString& val );

		#ifdef UNICODE
		void    SerializeUnicode( CString& val );
		void    DeserializeUnicode( CString& val );
		void    SerializeANSI8( CString& val );
		void    DeserializeANSI8( CString& val );
		#endif UNICODE
		void    SerializeANSI8( CStringANSI8& val );
		void    DeserializeANSI8( CStringANSI8& val );
		void    Serialize( const char*  val );
		void    SerializeANSI8( const char*  val );

		void    Serialize(   bool     val );
		void    Deserialize( bool&    val );
		void    Serialize(   float    val );
		void    Deserialize( float&   val );
		void    Serialize(   double   val );
		void    Deserialize( double&  val );

		//void    Serialize(   DWORD    val );

		void    Serialize(   CString strDesc, long     val );
		void    Deserialize( CString strDesc, long&    val );
		long    PeekLong();
		void    Serialize(   CString strDesc, short    val );
		void    Deserialize( CString strDesc, short&   val );
		short   PeekShort();
		void    Serialize(   CString strDesc, char     val );
		void    Deserialize( CString strDesc, char&    val );
		void    SerializeTCHAR(   CString strDesc, CString  val );
		void    DeserializeTCHAR( CString strDesc, CString& val );
		void    Serialize(   CString strDesc, char*    val );
		char    PeekChar();
		void    Serialize(   CString strDesc, bool     val );
		void    Deserialize( CString strDesc, bool&    val );
		void    Serialize(   CString strDesc, float    val );
		void    Deserialize( CString strDesc, float&   val );
		CString PeekStringTCHAR();
		CStringANSI8  PeekStringANSI8();

		//
		void    SerializeBlock( char* data, long iBlockSize );
		void    DeserializeBlock( char* data, long iExpectedBlockSize );
		void    SerializeBlockDirectly( char* data, long iBlockSize );

		void    WriteCheckpointInformation( bool val ){ m_bWriteCheckpointInformation = val; }
		void    Rewind(){ m_iCurrentLocation = 0; }
		void    RemoveExcessAllocation();

		// Reads the next text line and constructs an object based on the string classname
		void*   InstanciateIfBlock( CWorldDescBase* pWorldDesc = 0 );
		void*   Instanciate( CWorldDescBase* pWorldDesc = 0 );

		long    GetAllocatedSize(){ return m_iAllocatedSize; }
		long    GetUsedSize(){ return m_iUsedSize; }
		char*   GetRawData(){ return m_pData; }

		void    SetSerialExpandSize( long i ){ m_iSerialExpandSize = i; }

		bool    WriteSanityCheckToFile( FILE* fp, long iSanityCheck );
		bool    WriteToFile( FILE* fp );   // write a block of data from the CEOSAISerial object
		long    ReadSanityCheckFromFile( FILE* fp );
		long    ReadSanityCheckFromFile( CFile* pFile );
		bool    ReadFromFile( FILE* fp );  // read a block of data into the CEOSAISerial object
		bool    ReadFromFile( CFile* pFile );  // read a block of data into the CEOSAISerial object

		// WriteFile2/ReadFile2 does not read a size-block at the beginning
		bool    WriteFile2( CString strFullFilename );
		bool    ReadFile2( CString strFullFilename, ULONG uMaximumSize = 0 );
		//bool    ReadFile2( CFile* pFile, ULONG iMaximumSize = 0 );

		//
		//
		long         ExtractLong();
		#ifdef UNICODE
		CString      ExtractStringUnicode();
		#endif UNICODE
		CString      ExtractStringTCHAR();
		CStringANSI8  ExtractStringANSI8();
		bool    ExtractBool();
		DWORD   ExtractDWORD();
		short   ExtractShort();
		unsigned short ExtractUnsignedShort();
		char    ExtractChar();
		unsigned char  ExtractUnsignedChar();

		//void          AppendUnsignedLong( unsigned long val );
		//unsigned long ExtractUnsignedLong();
		void    SerializeClassId( CStringANSI8 strClassId );
		void    SerializeClassId( long        iClassId );
		void    ExtractAndVerifyClassId( CStringANSI8 strClassId );
		void    ExtractAndVerifyClassId( long    iClassId );
		//
		void    AppendLong( long val );
	private:

		void    AppendShort( short val );
		void    AppendUnsignedShort( unsigned short val );
		void    AppendChar( char val );
		void    AppendUnsignedChar( unsigned char val );
		void    AppendStringTCHAR( CString strText );
		#ifdef UNICODE
		void    AppendStringUnicode( CString strText );
		void    AppendStringANSI8( CString strText );
		#endif UNICODE
		void    AppendStringANSI8( CStringANSI8 strText );
		void    AppendCharString( const char* szString );
		void    AppendBool( bool bVal );
		void    AppendFloat( float fVal );
		float   ExtractFloat();
		void    AppendDouble( double val );
		double  ExtractDouble();
		void    AppendDWORD( DWORD val );

		//
		void    ForceAllocationSize( int iNewSize );
		void    Reallocate( int iMinimumNewSize );

	private:

		bool   m_bWriteCheckpointInformation;

		long   m_iSerialExpandSize;
		long   m_iCurrentLocation;

		long   m_iUsedSize;
		long   m_iAllocatedSize;
		char*  m_pData;
	//
		friend class CMessage;
};

