#include "MapTool.h"
#include "Window.h"


MapTool::MapTool() //: io_event{ INVALID_HANDLE_VALUE }, io_thread{}
{
    //io_event = CreateEvent(NULL, TRUE, FALSE, NULL);

}

void MapTool::Initialize()
{
    Window::initialize();
}

void MapTool::Run()
{
    //io_thread.reset(new std::thread{ IoWorkerThread, this });

    Window::run();
}

void MapTool::Finalize()
{
    //io_thread->join();
  
    Window::finalize();
}


