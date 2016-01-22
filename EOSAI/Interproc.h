
#pragma once

class Interproc
{
	public:
		static void Initialize();
		static void IsProcessing(bool b);

		static void AddStatusLine(CString str);

		static void UpdateSharedMemory();
		static void Shutdown();

	// Data to send over SharedMemory
	//
		static bool    m_bStatusUpdated;
		static CString m_strFullStatus;
};

