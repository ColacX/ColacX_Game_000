//notes gör arrays istället för object för prestanda optimering med CUDA i framtiden möjligtvis
//notes focusera på genetiska algorithmen istället för magnetism och kraft förändringar

#include "Window.h"
#include <Windows.h>
#include <list>

std::list<WindowListener*> listListener;

LRESULT CALLBACK WindowsMessageHandler( HWND windowHandle, UINT message, WPARAM parameterA, LPARAM parameterB ){
	std::list<WindowListener*>::iterator it = listListener.begin();
	std::list<WindowListener*>::iterator end = listListener.end();

	//intercept window messages
	while( it != end )
	{
		(*it)->WindowListenerNotifiy( windowHandle, message, parameterA, parameterB );
		it++;
	}

	//pass unhandled messages to default window processor and return its result
	return DefWindowProc( windowHandle, message, parameterA, parameterB );
}

Window::Window()
{
	w = 800;
	h = 600;
	x = 0;
	y = 0;
	colorDepth = 32;
	fullscreen = false;
	visible = true;
	maximized = false;
}

Window::~Window()
{
	listListener.clear();
}

int Window::X()
{
	return x;
}

void Window::X( int value )
{
	x = value;
}

int Window::Y()
{
	return y;
}

void Window::Y( int value )
{
	y = value;
}

int Window::W()
{
	return w;
}

void Window::W( int value )
{
	w = value;
}

int Window::H()
{
	return h;
}

void Window::H( int value )
{
	h = value;
}

bool Window::Fullscreen()
{
	return fullscreen;
}

void Window::Fullscreen( bool state )
{
	fullscreen = state;
}

bool Window::Visible()
{
	return visible;
}

void Window::Visible( bool state )
{
	visible = state;
}

bool Window::Maximised()
{
	return maximized;
}

void Window::Maximised( bool state )
{
	maximized = state;
}

void Window::UpdateWindow()
{
	Window::addListener(this);

	LPCSTR windowClassName;
	windowClassName = "windowClassName";

	LPCSTR windowTitle;
	windowTitle = "windowTitle";

	HINSTANCE moduleHandle;
	moduleHandle = GetModuleHandle( 0 );

	WNDCLASS windowClass;
	ZeroMemory( &windowClass, sizeof(windowClass) );
	windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc = WindowsMessageHandler;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = moduleHandle;
	windowClass.hIcon = LoadIcon(0, IDI_WINLOGO);
	windowClass.hCursor = LoadCursor(0, IDC_ARROW);
	windowClass.hbrBackground = 0;
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = windowClassName;

	if( !RegisterClass( &windowClass ) )
	{
		throw "Window: RegisterClass failed";
	}

	DWORD windowStyle;
	DWORD windowStyleExtended;

	if( fullscreen )
	{
		DEVMODE deviceMode;
		memset( &deviceMode, 0, sizeof(deviceMode) );
		deviceMode.dmSize = sizeof(deviceMode);
		deviceMode.dmPelsWidth = w;
		deviceMode.dmPelsHeight = h;
		deviceMode.dmBitsPerPel = colorDepth;
		deviceMode.dmFields = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		if( ChangeDisplaySettings( &deviceMode, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL )
		{
			throw "Window: ChangeDisplaySettings failed";
		}
		else
		{
			windowStyle = WS_POPUP;
			windowStyleExtended = WS_EX_APPWINDOW;
		}
	}
	else
	{
		windowStyle = WS_OVERLAPPEDWINDOW;
		windowStyleExtended = WS_EX_APPWINDOW|WS_EX_WINDOWEDGE;
	}

	RECT windowRectangle;
	windowRectangle.left = 0;
	windowRectangle.right = w;
	windowRectangle.top = 0;
	windowRectangle.bottom = h;

	//adjust window rectangle to true requested size according to the window styles
	AdjustWindowRectEx( &windowRectangle, windowStyle, false, windowStyleExtended);

	HWND windowHandle;
	windowHandle = CreateWindowEx( windowStyleExtended, windowClassName, windowTitle,
		windowStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0, 
		windowRectangle.right - windowRectangle.left, windowRectangle.bottom - windowRectangle.top, 0, 0, moduleHandle, 0);
	
	if( windowHandle == 0)
	{
		throw "Window: CreateWindowEx failed";
	}

	windowDeviceContext = GetDC( windowHandle );

	if( windowDeviceContext == 0 ){
		throw "Window: GetDC failed";
	}

	ShowWindow( windowHandle, SW_SHOW );
	SetForegroundWindow( windowHandle );
	SetFocus( windowHandle );

	PIXELFORMATDESCRIPTOR pixelformatDescriptor = { // pfd Tells Windows How We Want Things To Be
		sizeof(PIXELFORMATDESCRIPTOR), // Size Of This Pixel Format Descriptor
		1, // Version Number
		PFD_DRAW_TO_WINDOW | // Format Must Support Window
		PFD_SUPPORT_OPENGL | // Format Must Support OpenGL
		PFD_DOUBLEBUFFER, // Must Support Double Buffering
		PFD_TYPE_RGBA, // Request An RGBA Format
		this->colorDepth, // Select Our Color Depth
		0, 0, 0, 0, 0, 0, // Color Bits IgnocolorR
		0, // No Alpha Buffer
		0, // Shift Bit IgnocolorR
		0, // No Accumulation Buffer
		0, 0, 0, 0, // Accumulation Bits IgnocolorR
		24, // 24Bit Z-Buffer (Depth Buffer)
		0, // No Stencil Buffer //TODO turn this shit on !
		0, // No Auxiliary Buffer
		PFD_MAIN_PLANE, // Main Drawing Layer
		0, // Reserved
		0, 0, 0 // Layer Masks IgnocolorR
	};

	//windows will choose a pixel format thats as close as possible to what we wanted
	int pixelFormat;
	pixelFormat = ChoosePixelFormat( windowDeviceContext, &pixelformatDescriptor);

	if( pixelFormat == 0 )
	{
		throw "Window: ChoosePixelFormat failed";
	}

	//set the pixel format for the device context
	if( !SetPixelFormat( windowDeviceContext, pixelFormat, &pixelformatDescriptor) )
	{
		throw "Window: SetPixelFormat failed";
	}

	if( maximized )
	{
		ShowWindow( windowHandle, SW_SHOWMAXIMIZED );
	}

	//get rendering context
	HGLRC renderingContext;
	renderingContext = wglCreateContext( windowDeviceContext );

	//makes the callings thread current rendering context into the opengl rendering context
	if( !wglMakeCurrent( windowDeviceContext, renderingContext ) ){
		throw "GameClient: wglMakeCurrent failed";
	}

	
}

void Window::CheckMessages()
{
	MSG windowMessage;
	PeekMessage( &windowMessage, 0, 0, 0, PM_REMOVE );
	DispatchMessage( &windowMessage );
}

void Window::UpdateBuffers()
{
	SwapBuffers( windowDeviceContext );
}

void Window::addListener( WindowListener* listener )
{
	listListener.push_back( listener );
}