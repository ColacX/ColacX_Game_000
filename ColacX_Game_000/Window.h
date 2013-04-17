#pragma once

#include "WindowListener.h"

class Window : WindowListener
{
private:
	bool fullscreen, visible, maximized;
	int w, h, x, y;
	int colorDepth;

	HDC windowDeviceContext;
public:
	Window();
	~Window();
	int X();
	void X( int value );
	int Y();
	void Y( int value );
	int W();
	void W( int value );
	int H();
	void H( int value );
	bool Fullscreen();
	void Fullscreen( bool state );
	bool Visible();
	void Visible( bool state );
	bool Maximised();
	void Maximised( bool state );

	void UpdateWindow();
	void CheckMessages();
	void UpdateBuffers();
	
	void WindowListenerNotifiy( HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam )
	{
		switch(message)
		{
		case WM_SIZE:
			this->w = LOWORD(lParam);
			this->h = HIWORD(lParam);
		}
	}
	
	static void addListener( WindowListener* windowListener );
};
