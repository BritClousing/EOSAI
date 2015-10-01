// EOSAI.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "EOSAI.h"
#include "EOSAIPlayerManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

#define ThisIsADLL
#include "DllExports.h"
#include "EOSAIInterface.h"
#include "EOSAIWorldDistanceTool.h"

int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void*)
{
	return 1;
}


/*
void EXPORT SetEOSAI( CInterface* pEOSAIInterface, CEOSAIWorldDistanceTool* pEOSAIWorldDistanceTool )
{
	g_pEOSAIInterface = pEOSAIInterface;
	g_pWorldDistanceTool = pEOSAIWorldDistanceTool;
}
*/
//void* IMPEXP CreateFooClassInstance()
/*
void* EXPORT CreateAIPlayerManager2()
{
	return static_cast< void* >( new CEOSAIPlayerManager );
}
*/
/*
void  EXPORT SetEOSAI( CInterface* pEOSAIInterface )
{
	g_pEOSAIInterface = pEOSAIInterface;
	//g_pAIWorldDistanceTool = g_pEOSAIInterface->pAIWorldDistanceTool;
}
*/

// CEOSAIApp
/*
BEGIN_MESSAGE_MAP(CEOSAIApp, CWinApp)
END_MESSAGE_MAP()

// CEOSAIApp construction

CEOSAIApp::CEOSAIApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CEOSAIApp object

CEOSAIApp theApp;


// CEOSAIApp initialization

BOOL CEOSAIApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
*/





