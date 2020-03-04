#pragma once
#include "IOThread.h"

class IOThread;

class MapTool
{
public:
	MapTool();

	void Initialize();
	void Run();
	void Finalize();
	
private:
	IOThread* task_thread;
};

