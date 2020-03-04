#include "IOThread.h"


IOThread::IOThread() : thread{ nullptr }, task_event{ INVALID_HANDLE_VALUE }
{
	task_event = CreateEvent(NULL, TRUE, FALSE, NULL);

	assert_if_false(NULL != task_event);
}

IOThread::~IOThread()
{
	CloseHandle(task_event);
}

void IOThread::start()
{
	this->thread = new std::thread( workerThread, this );
	
}

void IOThread::giveTask(Task* task)
{
	task_queue.push(task);

	SetEvent(task_event);
}

std::queue<Task*>& IOThread::getCompletionQueue()
{
	return this->completion_queue;
}

void IOThread::workerThread(IOThread* thread)
{
	for (;;)
	{
		WaitForSingleObject(thread->task_event, INFINITE);

		for (;;)
		{
			if (false == thread->task_queue.empty())
			{
				Task* task = thread->task_queue.front();

				switch (task->task_type)
				{
				case TT_SAVE:
					break;
				case TT_LOAD:
					break;
				case TT_EXIT:
					return;
				default:
					crash();
				}

				thread->task_queue.pop();
				thread->completion_queue.push(task);
			}
		}

		ResetEvent(thread->task_event);
	}
}