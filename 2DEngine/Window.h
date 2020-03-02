#ifndef WINDOW_H
#define WINDOW_H

#define WIN32_LEAN_AND_MEAN
#include <vector>
#include <thread>
#include <string>
#include <windows.h>
#include <stdio.h>

#pragma warning(disable:4996)
#define START_CONSOLE() {AllocConsole();  freopen("CONOUT$", "w", stdout); freopen("CONIN$", "r", stdin);}
#define STOP_CONSOLE()  {FreeConsole();}


using uint = unsigned int;
using uchar = unsigned char;

namespace rp {

	class Window
	{
	private:
		static bool				debug_console;
		static HINSTANCE		handle_instance;
		static WNDCLASSEX		window_class_ex;
		static HWND				handle_window;

		static int				width,height,max_threads;
		static std::wstring		class_name;
		static std::wstring		application_name;
		
		static float			frame_set_timer;
		static int				drawed_frame;

		static float				cam_x;
		static float				cam_z;

		static int				fps;
	public:
		static bool				init(bool, HINSTANCE, int, LPCWSTR, LPCWSTR,UINT t = std::thread::hardware_concurrency());
		static bool				exit();
		static void				update(float&);
		static HWND				getHWND()				{ return  handle_window;}
		static HINSTANCE		getHandleInstance()		{ return  handle_instance;}
		static int				getWidth()				{ return  width;}
		static int				getHeight()				{ return  height;}
		static int				getMaxThreads()			{ return  max_threads; }
		static void				setWidth(int width)		{ rp::Window::width = width;} 
		static void				setHeight(int height)	{ rp::Window::height = height;}

		static float				getZoomX() { return  cam_x; }
		static float				getZoomZ() { return  cam_z; }

		static int				getFPS() { return (fps); }
		friend LRESULT CALLBACK windowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	};	
}


#endif