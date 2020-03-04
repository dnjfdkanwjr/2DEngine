#pragma once

#include <Windows.h>
#include <string_view>
#include <tchar.h>


static class Window
{
public:
	static void initialize();
	static void run();
	static void finalize();

	static std::wstring_view& getTitle();
	static HWND&			  getHwnd();
	static WNDCLASSEX&		  getWcEx();

private:
	static LRESULT WINAPI wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	static HWND					hwnd;
	static WNDCLASSEX			wc;
	static std::wstring_view	title;
};


