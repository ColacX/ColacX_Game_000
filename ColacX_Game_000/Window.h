#pragma once

#include "WindowListener.h"

namespace ColacX
{
	class Window
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
		
		
		static void addListener( WindowListener* windowListener );
	};
}
