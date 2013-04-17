#include "Window.h"
#include "Thread.h"
#include "Camera.h"
#include "Texture.h"
#include "Terrain000.h"
#include "Water000.h"
#include "Camera000.h"
#include "Gun000.hpp"
#include "Cloud001.h"

class Program001 : public WindowListener
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

public:
	Program001()
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

		//setup scene //todo encapsulate scene code
		camera000.G_Construct();
		camera000.G_ResizeView( window.W(), window.H() );
		camera000.MoveGlobal( 10, 2, 0 );
		camera000.LookAt(0,0,0);
		
		Terrain000 terrain000;
        terrain000.Construct();
		terrain000.G_Construct();

		Water000 water000;
        water000.Construct();
		water000.G_Construct();

		Gun000 gun000;
		gun000.Construct();
		gun000.G_Construct();
		
		//start loop
		running = true;
		paused = false;

		while( running )
		{
			/*
			if( paused ){
				Thread::Sleep(1000);
				continue;
			}
			*/

			//check window messages
			for( int i=0; i<5; i++ )
				window.CheckMessages();

			CheckControls();
			
			//reset drawing buffer
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

			//reset to the cameras viewpoint
			camera000.G_SetModelView();

			//render entitys
			terrain000.G_Draw(camera000, framecount);
			water000.G_Draw(camera000, framecount);
			gun000.G_Draw(camera000, framecount);
			cloud001.G_Draw(camera000, framecount);
			G_DrawOrigo();
			
			//show rendering
			window.UpdateBuffers();

			//Thread::Sleep( 1000/60 );
			framecount++;
		}
	}

	void CheckControls()
	{
		//check keyboard
		if( keyDown[ 'A' ] )
			camera000.MoveLocal( -0.1f, +0.0f, +0.0f );
		if( keyDown[ 'D' ] )
			camera000.MoveLocal( +0.1f, +0.0f, +0.0f );
		if( keyDown[ 'W' ] )
			camera000.MoveLocal( +0.0f, +0.0f, -0.1f );
		if( keyDown[ 'S' ] )
			camera000.MoveLocal( +0.0f, +0.0f, +0.1f );
		if( keyDown[ VK_CONTROL ] )
			camera000.MoveLocal( +0.0f, -0.1f, +0.0f );
		if( keyDown[ VK_SPACE ] )
			camera000.MoveLocal( +0.0f, +0.1f, +0.0f );
			
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

	void DrawFloor(){
		glActiveTexture( GL_TEXTURE0 );
		glDisable( GL_TEXTURE_2D );
		glActiveTexture( GL_TEXTURE1 );
		glDisable( GL_TEXTURE_2D );
		glColor4f(0, 0, 1, 1);

		float s = 10.0f;

		glBegin( GL_TRIANGLES );

		glNormal3f( 0.0f, 1.0f, 0.0f );

		glTexCoord2f( 0, 0 ); glVertex3f( -s, 0, -s );
		glTexCoord2f( 0, 1 ); glVertex3f( -s, 0, +s );
		glTexCoord2f( 1, 0 ); glVertex3f( +s, 0, -s );

		glTexCoord2f( 1, 0 ); glVertex3f( +s, 0, -s );
		glTexCoord2f( 0, 1 ); glVertex3f( -s, 0, +s );
		glTexCoord2f( 1, 1 ); glVertex3f( +s, 0, +s );

		glEnd();
	}

	void DrawBox( float x, float y, float z, float s )
	{
		glBegin( GL_TRIANGLES );

		glVertex3f( x + 0, y + 0, z + 0 ); //0
		glVertex3f( x + s, y + 0, z + 0 ); //1
		glVertex3f( x + 0, y + s, z + 0 ); //2

		glVertex3f( x + 0, y + s, z + 0 ); //2
		glVertex3f( x + s, y + s, z + 0 ); //3
		glVertex3f( x + s, y + 0, z + 0 ); //1

		glVertex3f( x + s, y + 0, z + 0 ); //1
		glVertex3f( x + 0, y + 0, z + s ); //4
		glVertex3f( x + s, y + 0, z + s ); //5

		glVertex3f( x + s, y + 0, z + s ); //5
		glVertex3f( x + 0, y + s, z + s ); //6
		glVertex3f( x + s, y + 0, z + 0 ); //1

		glEnd();
	}

	void G_DrawOrigo(){
		glActiveTexture( GL_TEXTURE0 );
		glDisable( GL_TEXTURE_2D );
		glActiveTexture( GL_TEXTURE1 );
		glDisable( GL_TEXTURE_2D );

		glDisable(GL_LIGHTING);

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
			break;

		case WM_CLOSE:
			if( !keyDown[ VK_CONTROL ])
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
};