
#pragma once

class Interproc
{
	public:
		static void Initialize();
		static void IsProcessing(bool b);
		static void UpdateSharedMemory();
		static void Shutdown();
};

