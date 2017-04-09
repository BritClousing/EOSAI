
#include "stdafx.h"

#include "EOSAILogFile.h"
#include "mmsystem.h"
#include "EOSAISettings.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void CEOSAILogFile::Initialize( CString strFileLocation )
{
	//if( Public::m_bCreateCheckpointFile == false ) return;
	if( m_fp != NULL )
	{
		delete m_fp;
		m_fp = NULL;
	}
	ASSERT( m_fp == NULL );
	//fpCheckpoint = _tfopen( strFileLocation, _T("w") );
	_tfopen_s( &m_fp, strFileLocation, _T("w") );
	ASSERT( m_fp != NULL );
	//if( m_fp == NULL ){ Public::m_bCreateCheckpointFile = false; }

	SYSTEMTIME time;
	GetLocalTime( &time );
	CString strTime;
	strTime.Format( _T("%hd/%02hd/%02hd %02hd:%02hd:%02hd"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond );

	//#ifdef WRITE_CHECKPOINTS
	//strTime += _T(" LogFile On");
	//#else
	//strTime += " Checkpoints Off";
	//#endif
	Write( strTime );

	{
		/*
		// Grab some of the data that I need.
		BOOL bHasOSInfo = FALSE;
		OSVERSIONINFO g_OSVersion;
		ZeroMemory( &g_OSVersion, sizeof(g_OSVersion) );
		g_OSVersion.dwOSVersionInfoSize = sizeof(g_OSVersion);
		bHasOSInfo = GetVersionEx( &g_OSVersion );
		//char g_windowsOSTypes[6][50] = { "Windows 95", "Windows 98", "Windows ME", "Windows NT", "Windows 2000", "Windows XP" };
		//char g_windowsOSTypes[7][50] = { "Windows 95", "Windows 98", "Windows ME", "Windows NT", "Windows 2000", "Windows XP", "[Windows OS]" };
		TCHAR g_windowsOSTypes[7][50] = { _T("Windows 95"), _T("Windows 98"), _T("Windows ME"), _T("Windows NT"), _T("Windows 2000"), _T("Windows XP"), _T("Windows Vista/7") };

		//int osIndex = 0;
		int osIndex = 6;
		if ( g_OSVersion.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS ) {
			if ( g_OSVersion.dwMinorVersion == 0 ) {
				osIndex = 0;
			}
			else if ( g_OSVersion.dwMinorVersion > 0 ) {
				osIndex = 1;
			}
		}
		else if ( g_OSVersion.dwPlatformId == VER_PLATFORM_WIN32_NT ) {
			if ( g_OSVersion.dwMajorVersion == 4 ) {
				osIndex = 3;
			}
			else if ( g_OSVersion.dwMajorVersion == 5 && g_OSVersion.dwMinorVersion == 0 )
			{
				osIndex = 4;
			}
			else if ( g_OSVersion.dwMajorVersion == 5 && g_OSVersion.dwMinorVersion > 0 )
			{
				osIndex = 5;
			}
		}

		CString strOS;
		strOS.Format( _T("Operating System: %s Version %d.%d Build %d %s"), 
			g_windowsOSTypes[osIndex], 
			g_OSVersion.dwMajorVersion,
			g_OSVersion.dwMinorVersion,
			g_OSVersion.dwBuildNumber,
			g_OSVersion.szCSDVersion );
		Write( strOS );
		*/

		// Memory
		MEMORYSTATUS g_sysMemoryStatus = {0};
		ZeroMemory( &g_sysMemoryStatus, sizeof(MEMORYSTATUS) );
		g_sysMemoryStatus.dwLength = sizeof(MEMORYSTATUS);
		GlobalMemoryStatus ( &g_sysMemoryStatus );
		CString strMemory;
		strMemory.Format( _T("Total Physical Memory: %d MB"), g_sysMemoryStatus.dwTotalPhys / (1024*1024) );
		Write( strMemory );

		strMemory.Format( _T("Total Available Memory: %d MB"), g_sysMemoryStatus.dwAvailPhys / (1024*1024) );
		Write( strMemory );
		/*
		wsprintf( g_memInfoString, "Current Memory Load: %d%%\r\n" \
									"Total Physical Memory: %d MB\r\n" \
									"Available Physical Memory: %d MB\r\n"\
									"Total Virtual Memory: %d MB\r\n" \
											"Available Virtual Memory: %d MB\r\n",
		g_sysMemoryStatus.dwMemoryLoad,
		g_sysMemoryStatus.dwTotalPhys / (1024*1024),
		g_sysMemoryStatus.dwAvailPhys / (1024*1024),
		g_sysMemoryStatus.dwTotalVirtual / (1024*1024),
		g_sysMemoryStatus.dwAvailVirtual / (1024*1024)  );
		*/
	}
}

//#ifdef WRITE_CHECKPOINTS

void CEOSAILogFile::WriteTime()
{
	if( m_fp == NULL ) return;

	CTime Time = CTime::GetCurrentTime();
	_ftprintf( m_fp, _T("%d/%d/%d %d:%02d:%02d\n"), 
		Time.GetYear(), Time.GetMonth(), Time.GetDay(),
		Time.GetHour(), Time.GetMinute(), Time.GetSecond() );
	/*
	long iTopDigits = dw/1000;
	long iBottomDigits = dw%1000;
	_ftprintf( fpCheckpoint, _T("%u %u  (%u)\n"), iTopDigits, iBottomDigits, dw );
	*/
	fflush( m_fp );
}

void CEOSAILogFile::Write( CString strText )
{
	if( m_fp == NULL ) return;
	lkjljk

	_ftprintf( m_fp, _T("%s\n"), strText );
	fflush( m_fp );
}

void CEOSAILogFile::Write( char*  szText )
{
	if( m_fp == NULL ) return;

	//CString strText2 = strText;
	//fprintf( m_fp, ANSI8("%s\n"), szText );
	fprintf( m_fp, "%s\n", szText );
	fflush( m_fp );
}

void CEOSAILogFile::WriteNoReturn( char* szText )
{
	if( m_fp == NULL ) return;

	//CString strText2 = strText;
	fprintf( m_fp, szText );
	fflush( m_fp );
}

void CEOSAILogFile::Write( long val )
{
	if( m_fp == NULL ) return;

	_ftprintf( m_fp, _T("%ld\n"), val );
	fflush( m_fp );
}

void CEOSAILogFile::Write( short val )
{
	if( m_fp == NULL ) return;

	_ftprintf( m_fp, _T("%d\n"), val );
	fflush( m_fp );
}

void CEOSAILogFile::Write( char val )
{
	if( m_fp == NULL) return;

	_ftprintf( m_fp, _T("%d\n"), val );
	fflush( m_fp );
}

void CEOSAILogFile::Write( bool val )
{
	if( m_fp == NULL ) return;

	if( val )
	{
		_ftprintf( m_fp, _T("true\n") );
	}
	else
	{
		_ftprintf( m_fp, _T("false\n") );
	}
	fflush( m_fp );
}

void CEOSAILogFile::Write( float val )
{
	if( m_fp == NULL ) return;

	_ftprintf( m_fp, _T("%f\n"), val );
	fflush( m_fp );
}

void CEOSAILogFile::Write( DWORD val )
{
	if( m_fp == NULL ) return;

	_ftprintf( m_fp, _T("%ul\n"), val );
	fflush( m_fp );
}

void CEOSAILogFile::Write( CString strText1, CString strText2 )
{
	if( m_fp == NULL ) return;

	_ftprintf( m_fp, _T("%s = %s\n"), strText1, strText2 );
	fflush( m_fp );
}

void CEOSAILogFile::Write( CString strText, long val )
{
	if( m_fp == NULL ) return;

	_ftprintf( m_fp, _T("%s = %ld\n"), strText, val );
	fflush( m_fp );
}

void CEOSAILogFile::Write( CString strText, short val )
{
	if( m_fp == NULL ) return;

	_ftprintf( m_fp, _T("%s = %d\n"), strText, val );
	fflush( m_fp );
}

void CEOSAILogFile::Write( CString strText, char val )
{
	if( m_fp == NULL ) return;

	_ftprintf( m_fp, _T("%s = %d\n"), strText, val );
	fflush( m_fp );
}

void CEOSAILogFile::Write( CString strText, bool val )
{
	if( m_fp == NULL ) return;

	if( val )
	{
		_ftprintf( m_fp, _T("%s = true\n"), strText );
	}
	else
	{
		_ftprintf( m_fp, _T("%s = false\n"), strText );
	}
	fflush( m_fp );
}

void CEOSAILogFile::Write( CString strText, float val )
{
	if( m_fp == NULL ) return;

	_ftprintf( m_fp, _T("%s = %f\n"), strText, val );
	fflush( m_fp );
}

void CEOSAILogFile::Write( CString strText, DWORD val )
{
	if( m_fp == NULL ) return;

	_ftprintf( m_fp, _T("%s = %ul\n"), strText, val );
	fflush( m_fp );
}

//#endif
