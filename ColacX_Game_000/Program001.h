#include "Window.h"
#include "Thread.h"
#include "Camera.h"
#include "TriangleCompiler.h"
#include "Texture.h"
#include "ASEcompiler.h"
#include "Bitmap.h"
#include "Misc.h"
#include "ShaderProgram.h"
#include "Terrain000.h"

using namespace ColacX;

class Program001 : public WindowListener
{
private:
	bool running;
	bool keyDown[256];
	int mouseCenterX, mouseCenterY;
	bool gameFocused;
	Window window;
	Camera camera;
	unsigned int framecount;

public:
	Program001()
	{
		ZeroMemory( keyDown, sizeof(keyDown) );
		gameFocused = false;
		framecount = 0;
	}
	
	void Run()
	{
		//window.Maximised( true );
		window.UpdateWindow();
		window.addListener( this );

		//setup rendering options
		{
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode
			glShadeModel(GL_SMOOTH);
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClearDepth(1.0f);

			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);

			//glEnable(GL_LINE_SMOOTH);
			//glEnable(GL_POINT_SMOOTH);
			//glEnable(GL_POLYGON_SMOOTH);

			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);

			//glHint(GL_FRAGMENT_SHADER_DERIVATIVE_HINT, GL_NICEST); //is available only if the GL version is 2.0 or greater.
			//glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST); //is available only if the GL version is 1.4 or greater.
			//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
			//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
			//glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
			//glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
			//glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST); //is available only if the GL version is 1.3 or greater.
	
			glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);
			glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
			glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
		}

		//alpha blending
		{
			glEnable (GL_BLEND);
			glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		//lighting
		{
			//glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);

			float ambientLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			float diffuseLight[] = { 1.0f, 1.0f, 1.0, 1.0f };
			float specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			float position[] = { 0.0f, 0.0f, 10.0f, 0.0f };

			glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
			glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
			glLightfv(GL_LIGHT0, GL_POSITION, position);
		}

		//fog
		{
			GLfloat fogColor[] = {0.0f, 0.0f, 0.0f, 1.0f};
			glFogi(GL_FOG_MODE, GL_LINEAR);
			glFogfv(GL_FOG_COLOR, fogColor);
			
			glFogf(GL_FOG_DENSITY, 0.5f);
			glHint(GL_FOG_HINT, GL_DONT_CARE);
			glFogf(GL_FOG_START, 5.0f);
			glFogf(GL_FOG_END, 20.0f);
			
			//glEnable(GL_FOG);
		}

		resizeScene( window.W(), window.H() );

		//setup pseudo random
		srand( 1337 );
		float randomValue;
		randomValue = ( (float)rand() / (float)RAND_MAX ) * 0.01f;

		//load textures
		Texture texture0("data/water.tga");
		Texture texture1("data/courier_new_40_r.tga");
		Texture texture2("data/grass.tga");
		Texture texture3("data/star.tga");
		Texture texture4("data/white_border.tga");
		//Texture texture5("data/8388Carnelian_02.jpg");
		//Texture texture6("data/abstract-blue-16806.jpg");
		//Texture texture5("data/8388Carnelian_02.jpg");
		//Texture texture6("data/abstract-blue-16806.jpg");

		//load shaders
		ShaderProgram::init();

		Shader vertexShader("data/water.svert", Shader::TYPE_VERTEX_SHADER);
		Shader fragmentShader("data/water.sfrag", Shader::TYPE_FRAGMENT_SHADER);
		//Shader geometryShader("data/water.sgeom", Shader::TYPE_GEOMETRY_SHADER);

		ShaderProgram shaderProgram;
		shaderProgram.attachShader(vertexShader);
		shaderProgram.attachShader(fragmentShader);
		shaderProgram.link();

		GLint uniform_framecount = shaderProgram.getUniform("framecount");
		GLint uniform_texture0 = shaderProgram.getUniform("texture0");
		GLint uniform_texture1 = shaderProgram.getUniform("texture1");

		//setup scene //todo encapsulate scene code
		camera.MoveZGlobal( +10.0f );
		camera.MoveYGlobal( +3.0f );

		Terrain000 terrain;
		terrain.C_Construct();
		
		reset();

		//start loop
		running = true;

		while( running )
		{
			//check window messages
			for( int i=0; i<5; i++ )
				window.CheckMessages();

			CheckControls();
			
			//reset drawing buffer
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

			//reset to the cameras viewpoint
			camera.SetView();

			

			//render entitys
			glActiveTexture( GL_TEXTURE0 );
			glEnable( GL_TEXTURE_2D );
			texture2.bind();
			glActiveTexture( GL_TEXTURE1 );
			glEnable( GL_TEXTURE_2D );
			texture0.bind();

			shaderProgram.activate();
			glUniform1i( uniform_framecount, framecount );
			glUniform1i( uniform_texture0, 0 );
			glUniform1i( uniform_texture1, 1 );

			terrain.G_Draw();

			glEnable(GL_TEXTURE_2D);
			texture0.bind();
			DrawFloor();
			DrawOrigo();

			shaderProgram.deactivate();
			
			//show rendering
			window.UpdateBuffers();

			//Thread::Sleep( 1000/60 );
			framecount++;
		}
	}

	void reset()
	{
	}

	void CheckControls()
	{
		//check keyboard
		if( keyDown[ 'A' ] )
			camera.MoveXLocal( -0.1f );
		if( keyDown[ 'D' ] )
			camera.MoveXLocal( +0.1f );
		if( keyDown[ 'W' ] )
			camera.MoveZLocal( -0.1f );
		if( keyDown[ 'S' ] )
			camera.MoveZLocal( +0.1f );
		if( keyDown[ VK_CONTROL ] )
			camera.MoveYLocal( -0.1f );
		if( keyDown[ VK_SPACE ] )
			camera.MoveYLocal( +0.1f );
			
		if( keyDown[ VK_LEFT ] )
			camera.RotateYLocal( +0.01f );
		if( keyDown[ VK_RIGHT ] )
			camera.RotateYLocal( -0.01f );
		if( keyDown[ VK_UP ] )
			camera.RotateXLocal( +0.01f );
		if( keyDown[ VK_DOWN ] )
			camera.RotateXLocal( -0.01f );
		if( keyDown[ 'Q' ] )
			camera.RotateZLocal( +0.01f );
		if( keyDown[ 'E' ] )
			camera.RotateZLocal( -0.01f );

		if( keyDown[ VK_ESCAPE ] )
		{
			gameFocused = false;
			while( ShowCursor(true) < 0 ){}
		}

		if( keyDown[ VK_F1 ] )
			reset();

		//check mouse
		if( gameFocused ){
			POINT point;
			GetCursorPos(&point);
			int mouseCurrentX = point.x;
			int mouseCurrentY = point.y;

			if(mouseCurrentX != mouseCenterX || mouseCurrentY !=mouseCenterY){
				float rx = - (mouseCurrentX - mouseCenterX )*0.001f;
				float ry = - (mouseCurrentY - mouseCenterY )*0.001f;

				camera.RotateYLocal( rx );
				camera.RotateXLocal( ry );

				SetCursorPos( mouseCenterX, mouseCenterY ); //global mouse position
			}
		}
	}

	void perspectiveGL( GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar )
	{
		//	Very long (& in theory accurate!) version of Pi. Hopefully an optimizing compiler
		//	will replace references to this with the value!
		const GLdouble pi = 3.1415926535897932384626433832795;

		//	Half of the size of the x and y clipping planes.
		GLdouble fW, fH;

		//	Calculate the distance from 0 of the y clipping plane. Basically trig to calculate
		//	position of clipper at zNear.

		//	Note:	tan( double ) uses radians but OpenGL works in degrees so we convert
		//			degrees to radians by dividing by 360 then multiplying by pi.
		fH = tan( (fovY / 2) / 180 * pi ) * zNear;	
		// Same as fH = tan( fovY / 360 * pi ) * zNear;
	
		//	Calculate the distance from 0 of the x clipping plane based on the aspect ratio.
		fW = fH * aspect;

		//	Finally call glFrustum, this is all gluPerspective does anyway!
		//	This is why we calculate half the distance between the clipping planes - glFrustum
		//	takes an offset from zero for each clipping planes distance. (Saves 2 divides)
		glFrustum( -fW, fW, -fH, fH, zNear, zFar );
	}

	void resizeScene(unsigned int windowWidth, unsigned int windowHeight){
		//Cartesian coordinate system
		glViewport(0, 0, windowWidth, windowHeight);
	
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		float aspect = float(windowWidth)/float(windowHeight);
		perspectiveGL(60.0f,aspect,0.01f,1000.0f);
	
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	void DrawFloor(){
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

	void DrawOrigo(){
		glPushAttrib(GL_ALL_ATTRIB_BITS);

		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);

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

		glPopAttrib();
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
			resizeScene( LOWORD(lParam), HIWORD(lParam) );
			break;

		case WM_CLOSE:
			if( !keyDown[ VK_CONTROL ])
				exit( 1337 );
			running = false;
			break;
		}
	}
};