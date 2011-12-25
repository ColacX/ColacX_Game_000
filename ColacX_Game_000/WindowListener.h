#pragma once

#include <Windows.h>

class WindowListener
{
protected:
	WindowListener();
public:
	virtual void WindowListenerNotifiy( HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam ) = 0;
};
