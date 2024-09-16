#include "ConsoleRenderer.hpp"

#include <string>

#define _WIN32_WINNT 0x0501
#include <windows.h>


namespace ConsoleRenderer
{
	//Si le new style est à 0, la fenetre n'a absolument rien à part son contenu
	LONG_PTR setConsoleWindowStyle(INT n_index,LONG_PTR new_style)
	{
		/*The function does not clear the last error information. if last value was zero.*/
		SetLastError(NO_ERROR);        

		HWND hwnd_console = GetConsoleWindow();
		LONG_PTR style_ptr = SetWindowLongPtr(hwnd_console,n_index,new_style);
		SetWindowPos(hwnd_console,0,0,0,0,0, SWP_NOZORDER|SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE|SWP_DRAWFRAME);

		//show window after updating
		ShowWindow(hwnd_console,SW_SHOW);

		return style_ptr;  
	}


	ConsoleWindow::ConsoleWindow(int width, int height)
		: m_width(width), m_height(height)
	{
		::SendMessage(::GetConsoleWindow(), WM_SYSKEYDOWN, VK_RETURN, 0x20000000);
		//
		// LONG_PTR new_style =  WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL;
		// setConsoleWindowStyle(GWL_STYLE, new_style);
	}

	ConsoleWindow::~ConsoleWindow() = default;

	std::ostream& operator<<(std::ostream& stream, const ConsoleWindow& consoleWindow)
	{
		return stream << "Width: " + std::to_string(consoleWindow.m_width) + ", Height: " + std::to_string(consoleWindow.m_height);
	}

}
