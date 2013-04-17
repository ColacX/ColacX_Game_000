#include "Window.h"
#include "Thread.h"
#include "Camera.h"
#include "Texture.h"
#include "Terrain000.h"
#include "File.h"

using namespace glm;

class Program003 : public WindowListener
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
	Program003()
	{
		ZeroMemory( keyDown, sizeof(keyDown) );
		gameFocused = false;
		framecount = 0;
	}
	
	void G_Run()
	{
	
		//window.Maximised( true );
		window.UpdateWindow();
		window.addListener( this );

		SetForegroundWindow(GetConsoleWindow());

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

			//glEnable(GL_CULL_FACE);
			//glCullFace(GL_BACK);

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
		Texture texture0;
		texture0.G_Construct("data/images/water.tga");
		Texture texture1;
		texture1.G_Construct("data/images/courier_new_40_r.tga");
		Texture texture2;
		texture2.G_Construct("data/images/grass.tga");

		//load shaders
		{
			glewInit();

			if(GL_TRUE != glewGetExtension("GL_ARB_fragment_shader"))
			   throw "GL_ARB_fragment_shader extension is not available!";
			
			if(GL_TRUE != glewGetExtension("GL_ARB_vertex_shader"))
			   throw "GL_ARB_vertex_shader extension is not available!";
			
			if(GL_TRUE != glewGetExtension("GL_ARB_shader_objects"))
			   throw "GL_ARB_shader_objects extension is not available!";

			const GLubyte* glVendor = glGetString(GL_VENDOR);
			printf("GL_VENDOR: %s\n", glVendor);

			const GLubyte* glRenderer = glGetString(GL_RENDERER);
			printf("GL_RENDERER: %s\n", glRenderer);

			const GLubyte* glVersion = glGetString(GL_VERSION);
			printf("GL_VERSION: %s\n", glVersion);

			const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
			printf("GL_SHADING_LANGUAGE_VERSION: %s\n", glslVersion);

			/*
			GLint nExtensions;
			glGetIntegerv(GL_NUM_EXTENSIONS, &nExtensions);
			for( int i = 0; i < nExtensions; i++ )
				printf("GL_EXTENSIONS %d2: %s\n", i, glGetStringi( GL_EXTENSIONS, i ) ); 
			*/
		}

		char *vs;
		unsigned int vsc;
		//File::ReadText("data/shaders/simple_multitexture.vert", vs, vsc);
		File::ReadText("data/shaders/trival_triangle.vert", vs, vsc);

		char *gs;
		unsigned int gsc;
		File::ReadText("data/shaders/trival_triangle.geom", gs, gsc);
		
		char *fs;
		unsigned int fsc;
		//File::ReadText("data/shaders/simple_multitexture.frag", fs, fsc);
		File::ReadText("data/shaders/trival_triangle.frag", fs, fsc);

		GLenum p = glCreateProgram();

		GLuint v;
		v = glCreateShader(GL_VERTEX_SHADER);
		GLuint g;
		g = glCreateShader(GL_GEOMETRY_SHADER_EXT);
		GLuint f;
		f = glCreateShader(GL_FRAGMENT_SHADER);

		{
			glShaderSource(v, 1, (const GLchar**)&vs, (GLint*)&vsc);
			glCompileShader(v);

			int status = 0;
			glGetObjectParameterivARB(v, GL_OBJECT_COMPILE_STATUS_ARB, &status);

			if(!status){
				GLbyte infoLog[1000];
				glGetInfoLogARB(v, sizeof(infoLog), 0, (GLcharARB*)infoLog);
				printf("Info log: %s\n", infoLog);
				throw "Shader: error glCompileShader";
			}

			glAttachShader(p,v);
		}

		if(true)
		{
			glShaderSource(g, 1, (const GLchar**)&gs, (GLint*)&gsc);
			glCompileShader(g);

			int status = 0;
			glGetObjectParameterivARB(g, GL_OBJECT_COMPILE_STATUS_ARB, &status);

			if(!status){
				GLbyte infoLog[1000];
				glGetInfoLogARB(g, sizeof(infoLog), 0, (GLcharARB*)infoLog);
				printf("Info log: %s\n", infoLog);
				throw "Shader: error glCompileShader";
			}

			glAttachShader(p,g);
		}

		{
			glShaderSource(f, 1, (const GLchar**)&fs, (GLint*)&fsc);
			glCompileShader(f);

			int status = 0;
			glGetObjectParameterivARB(f, GL_OBJECT_COMPILE_STATUS_ARB, &status);

			if(!status){
				GLbyte infoLog[1000];
				glGetInfoLogARB(f, sizeof(infoLog), 0, (GLcharARB*)infoLog);
				printf("Info log: %s\n", infoLog);
				throw "Shader: error glCompileShader";
			}

			glAttachShader(p,f);
		}

        glProgramParameteriEXT(p, GL_GEOMETRY_INPUT_TYPE_EXT, GL_TRIANGLES);
		glProgramParameteriEXT(p, GL_GEOMETRY_OUTPUT_TYPE_EXT, GL_TRIANGLES);

		int temp;
		glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT,&temp);
		glProgramParameteriEXT(p,GL_GEOMETRY_VERTICES_OUT_EXT,temp);

		// Bind index 0 to the shader input variable "VertexPosition"
		// Bind index 1 to the shader input variable "VertexColor"
		glBindAttribLocation(p, 0, "VertexPosition");		
		glBindAttribLocation(p, 1, "VertexColor");

		// Create the buffer objects
		GLuint vboHandles[2];
		glGenBuffers(2, vboHandles);
				
		// Populate the position buffer
		float positionData[] = {
			-0.8f, -0.8f, +0.0f,
			+0.8f, -0.8f, +0.0f,
			+0.0f, +0.8f, +0.0f
		};

		GLuint positionBufferHandle = vboHandles[0];
		glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
		glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), positionData, GL_STATIC_DRAW);
		
		// Populate the color buffer
		float colorData[] = {
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f
		};

		GLuint colorBufferHandle = vboHandles[1];
		glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
		glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colorData, GL_STATIC_DRAW);

		// Create and set-up the vertex array object
		GLuint vaoHandle;
		glGenVertexArrays( 1, &vaoHandle );
		glBindVertexArray(vaoHandle);
		// Enable the vertex attribute arrays
		glEnableVertexAttribArray(0);  // Vertex position
		glEnableVertexAttribArray(1);  // Vertex color
		// Map index 0 to the position buffer
		glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );
		// Map index 1 to the color buffer
		glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
		glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );

		glLinkProgram(p);
		
		//create shader program variables
		
		GLint uniform_world_matrix;
		uniform_world_matrix = glGetUniformLocation(p, "uniform_world_matrix");

		if( uniform_world_matrix == -1 )
			;//throw "glGetUniformLocation failed";

		GLint uniform_framecount;
		uniform_framecount = glGetUniformLocation(p, "uniform_framecount");

		if( uniform_framecount == -1 )
			;//throw "glGetUniformLocation failed";

		GLint uniform_texture0;
		uniform_texture0 = glGetUniformLocation(p, "texture0");

		if( uniform_texture0 == -1 )
			;//throw "glGetUniformLocation failed";

		GLint uniform_texture1;
		uniform_texture1 = glGetUniformLocation(p, "texture1");

		if( uniform_texture1 == -1 )
			;//throw "glGetUniformLocation failed";

		//setup scene //todo encapsulate scene code
		camera.MoveZGlobal( +10.0f );
		camera.MoveYGlobal( +3.0f );

		Terrain000 terrain;
		terrain.Construct();
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
			/*
			glActiveTexture( GL_TEXTURE0 );
			glEnable( GL_TEXTURE_2D );
			texture2.bind();
			glActiveTexture( GL_TEXTURE1 );
			glEnable( GL_TEXTURE_2D );
			texture0.bind();
			*/

            float aspect = float(window.W())/float(window.H());
            mat4 projectionMatrix = perspective(60.0f, aspect, 0.01f, 1000.0f);
            mat4 modelviewMatrix = camera.CamerMatrix();
            mat4 matrix = projectionMatrix * modelviewMatrix;

			glUseProgram(p);

			glBindVertexArray(vaoHandle);
			glDrawArrays(GL_TRIANGLES, 0, 3 );

			//upload shader program variables
			glUniform1i( uniform_framecount, framecount );
			glUniformMatrix4fv(uniform_world_matrix, 1, GL_FALSE, &matrix[0][0]);
			
			/*
			glUniform1i( uniform_texture0, 0 );
			glUniform1i( uniform_texture1, 1 );
			*/

			//glEnable(GL_TEXTURE_2D);
			//texture0.bind();
			//DrawFloor();
			
			glUseProgram(0);			

			G_DrawOrigo();
			
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

	void G_DrawOrigo(){
		glPushAttrib(GL_ALL_ATTRIB_BITS);

		glDisable(GL_LIGHTING);
		glActiveTexture( GL_TEXTURE0 );
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
