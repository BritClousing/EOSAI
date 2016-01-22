#include "stdafx.h"
#include "EOSAIMain.h"
#include "Interproc.h"

EOSAI::Main* g_pEOSAIMain = NULL;
using namespace EOSAI;
Main* EOSAI::Main::s_pMain;

Main::Main()
{
	s_pMain = this;
	g_pEOSAIMain = this;
}

Main::~Main()
{
	// free the memory
	g_pEOSAIMain = NULL;
}

void Main::InitializeInterprocessCommunication()
{
	Interproc::Initialize();
}

void Main::ShutdownInterprocessCommunication()
{
	Interproc::Shutdown();
}

CEOSAIRegionManager2* Main::GetAIRegionManager()
{
	return m_AICommonData.GetAIRegionManager();
}

