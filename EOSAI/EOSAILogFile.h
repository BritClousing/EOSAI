
#pragma once

// The LogFile class is used to write-out data to a file.  It's useful for debugging.

class CEOSAILogFile
{
	public:
		CEOSAILogFile(){ m_fp = NULL; }
		void Initialize( CString strFileLocation );

		void WriteTime();

		void Write( CString strText );
		void Write( char*   strText );
		void WriteNoReturn( char* strText );
		void Write( long val );
		void Write( short val );
		void Write( char val );
		void Write( bool val );
		void Write( float val );
		void Write( DWORD val );

		void Write( CString strText1, CString strText2 );
		void Write( CString strText,  long val );
		void Write( CString strText,  short val );
		void Write( CString strText,  char val );
		void Write( CString strText,  bool val );
		void Write( CString strText,  float val );
		void Write( CString strText,  DWORD val );

		FILE* m_fp;
};

/*
#else

namespace Checkpoint
{
   void Init( TCHAR* strFileLocation );

   inline void WriteTime(){}

   inline void Write( CString strText ){}
   inline void Write( char*   strText ){}
   inline void Write( long val ){}
   inline void Write( short val ){}
   inline void Write( char val ){}
   inline void Write( bool val ){}
   inline void Write( float val ){}
   inline void Write( DWORD val ){}

   inline void Write( CString strText1, CString strText2 ){}
   inline void Write( CString strText,  long val ){}
   inline void Write( CString strText,  short val ){}
   inline void Write( CString strText,  char val ){}
   inline void Write( CString strText,  bool val ){}
   inline void Write( CString strText,  float val ){}
   inline void Write( CString strText,  DWORD val ){}
};

#endif
*/
