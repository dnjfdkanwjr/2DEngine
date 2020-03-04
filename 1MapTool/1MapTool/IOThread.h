#pragma once
#include "MapTool.h"
#include "Assert.h"
#include <iostream>
#include <thread>
#include <queue>
#include <Windows.h>


enum TaskType : long
{
	TT_SAVE, TT_LOAD, TT_EXIT
};

struct Task
{
	TaskType	task_type;
	const char* file_name;
	const char* file_path;
	long		file_length;
	char*		buffer;
	bool		is_completed;
};

class IOThread
{
public:
	IOThread();
	~IOThread();

	void start();
	void giveTask(Task* task);
	std::queue<Task*>& getCompletionQueue();

private:
	static void workerThread(IOThread* thread);

private:
	HANDLE				task_event;
	std::thread*		thread;
	std::queue<Task*>	task_queue;
	std::queue<Task*>	completion_queue;
};
