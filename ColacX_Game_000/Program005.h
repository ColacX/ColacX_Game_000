#include "Window.h"
#include "Thread.h"
#include "Camera.h"
#include "Texture.h"
#include "Camera000.h"
#include "Cloud001.h"

class Program : public WindowListener
{
private:
	bool running;
	bool paused;
	bool keyDown[256];
	int mouseCenterX, mouseCenterY;
	bool gameFocused;
	Window window;
	Camera000 camera000;
	unsigned int framecount;

	void CheckControls()
	{
		//check keyboard
		float m = 1.0f;
		
		if( keyDown[ VK_SHIFT ] )
			m = 10.0f;

		if( keyDown[ 'A' ] )
			camera000.MoveLocal( -0.1f * m, +0.0f, +0.0f );
		if( keyDown[ 'D' ] )
			camera000.MoveLocal( +0.1f * m, +0.0f, +0.0f );
		if( keyDown[ 'W' ] )
			camera000.MoveLocal( +0.0f, +0.0f, -0.1f * m);
		if( keyDown[ 'S' ] )
			camera000.MoveLocal( +0.0f, +0.0f, +0.1f * m);
		if( keyDown[ VK_CONTROL ] )
			camera000.MoveLocal( +0.0f, -0.1f * m, +0.0f );
		if( keyDown[ VK_SPACE ] )
			camera000.MoveLocal( +0.0f, +0.1f * m, +0.0f );
			
		if( keyDown[ VK_LEFT ] )
			camera000.RotateLocalY( +1.0f );
		if( keyDown[ VK_RIGHT ] )
			camera000.RotateLocalY( -1.0f );
		if( keyDown[ VK_UP ] )
			camera000.RotateLocalX( +1.0f );
		if( keyDown[ VK_DOWN ] )
			camera000.RotateLocalX( -1.0f );
		if( keyDown[ 'Q' ] )
			camera000.RotateLocalZ( +1.0f );
		if( keyDown[ 'E' ] )
			camera000.RotateLocalZ( -1.0f );

		if( keyDown[ VK_ESCAPE ] )
			Unfocused();

		if( keyDown[ VK_F1 ] )
			Reset();

		if( keyDown[ VK_F2 ] )
			NormalMode();

		//if( keyDown[ VK_F3 ] )
			//PointMode();

		if( keyDown[ VK_F4 ] )
			LineMode();

		//check mouse
		if( gameFocused ){
			POINT point;
			GetCursorPos(&point);
			int mouseCurrentX = point.x;
			int mouseCurrentY = point.y;

			if(mouseCurrentX != mouseCenterX || mouseCurrentY !=mouseCenterY){
				float rx = - (mouseCurrentX - mouseCenterX )*0.1f;
				float ry = - (mouseCurrentY - mouseCenterY )*0.1f;

				camera000.RotateLocalY( rx );
				camera000.RotateLocalX( ry );

				SetCursorPos( mouseCenterX, mouseCenterY ); //global mouse position
			}
		}
	}

	void G_DrawOrigo()
	{
		glActiveTexture( GL_TEXTURE0 );
		glDisable( GL_TEXTURE_2D );
		glActiveTexture( GL_TEXTURE1 );
		glDisable( GL_TEXTURE_2D );

		glBegin(GL_LINES);
		
		glColor4f(1, 0, 0, 1);
		glVertex3f(+1.0f,+0.0f,+0.0f);
		glVertex3f(+0.0f,+0.0f,+0.0f);

		glVertex3f(+0.75f,+0.25f,+0.0f);
		glVertex3f(+0.75f,+0.0f,+0.25f);

		glColor4f(0, 1, 0, 1);
		glVertex3f(+0.0f,+1.0f,+0.0f);
		glVertex3f(+0.0f,+0.0f,+0.0f);

		glVertex3f(+0.0f,+0.75f,+0.25f);
		glVertex3f(+0.25f,+0.75f,+0.0f);

		glColor4f(0, 0, 1, 1);
		glVertex3f(+0.0f,+0.0f,+1.0f);
		glVertex3f(+0.0f,+0.0f,+0.0f);

		glVertex3f(+0.25f,+0.0f,+0.75f);
		glVertex3f(+0.0f,+0.25f,+0.75f);
		
		glEnd();
	}

	void WindowListenerNotifiy( HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam )
	{
		switch( message )
		{
		case WM_LBUTTONDOWN:
			if( !gameFocused )
			{
				gameFocused = true;
				POINT point;
				GetCursorPos(&point);
				mouseCenterX = point.x;
				mouseCenterY = point.y;
				
				while( ShowCursor( false ) > -1 ){}
			}

			break;
		case WM_LBUTTONUP:
			break;

		case WM_RBUTTONDOWN:
			break;
		case WM_RBUTTONUP:
			break;

		case WM_MOUSEMOVE:
			break;

		case WM_KEYDOWN:
			keyDown[ wParam ] = true;
			break;
		case WM_KEYUP:
			keyDown[ wParam ] = false;
			break;

		case WM_SIZE:
			camera000.G_ResizeView( LOWORD(lParam), HIWORD(lParam) );
			//camera000.G_ResizeView(1600, 900);
			break;

		case WM_CLOSE:
			//if( !keyDown[ VK_CONTROL ])
				exit( 1337 );
			running = false;
			break;

		case WM_KILLFOCUS:
			Unfocused();
			break;
		}
	}

	void Unfocused()
	{
		ZeroMemory( keyDown, sizeof(keyDown) );
		gameFocused = false;
		while( ShowCursor(true) < 0 ){}
	}

	void Reset()
	{
		camera000.LookAt(0,0,0);
	}	

	void PointMode()
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	}

	void LineMode()
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	void NormalMode()
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

public:
	void Construct()
	{
		ZeroMemory(keyDown, sizeof(keyDown));
		gameFocused = false;
		framecount = 0;
	}
	
	void G_Run()
	{
		window.Maximised( true );
		window.UpdateWindow();
		window.addListener( this );

		SetForegroundWindow(GetConsoleWindow());
		
		//print hardware info
		{
			const GLubyte* glVendor = glGetString(GL_VENDOR);
			printf("GL_VENDOR: %s\n", glVendor);

			const GLubyte* glRenderer = glGetString(GL_RENDERER);
			printf("GL_RENDERER: %s\n", glRenderer);

			const GLubyte* glVersion = glGetString(GL_VERSION);
			printf("GL_VERSION: %s\n", glVersion);

			const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
			printf("GL_SHADING_LANGUAGE_VERSION: %s\n", glslVersion);
		}

		//initiate shaders
		{
			glewInit();

			if(GL_TRUE != glewGetExtension("GL_ARB_fragment_shader"))
			   throw "GL_ARB_fragment_shader extension is not available!";
			
			if(GL_TRUE != glewGetExtension("GL_ARB_vertex_shader"))
			   throw "GL_ARB_vertex_shader extension is not available!";
			
			if(GL_TRUE != glewGetExtension("GL_ARB_shader_objects"))
			   throw "GL_ARB_shader_objects extension is not available!";
		}

		//setup scene
		camera000.G_Construct();
		camera000.G_ResizeView( window.W(), window.H() );
		camera000.MoveGlobal( 0, 0, 0 );
		camera000.RotateLocalY(180);
		//camera000.C_LookAt(0,0,0);

		Cloud001 cloud001;
		cloud001.Construct();
		cloud001.G_Construct();
		
		//start loop
		running = true;
		paused = false;

		while(running)
		{
			while(paused)
			{
				Thread::Sleep(1000);
			}

			//check window messages
			for( int i=0; i<5; i++ )
				window.CheckMessages();

			CheckControls();
			
			//reset drawing buffer
			glClearColor(0.0, 0.0, 0.0, 0.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//reset to the cameras viewpoint
			camera000.G_SetModelView();
			
			//render entitys
			cloud001.G_Draw(camera000, framecount);
			G_DrawOrigo();
			
			//show rendering
			window.UpdateBuffers();

			//Thread::Sleep( 1000/60 );
			framecount++;
		}
	}
};