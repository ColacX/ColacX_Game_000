//graphics librarys
#pragma comment(lib, "opengl32.lib") //opengl library for 32-bit computers
#pragma comment(lib, "glu32.lib") //openglu32 librarary extension to opengl
#pragma comment(lib, "glew32.lib") //glew librarary extension to opengl

//cuda gpu computing librarys
#pragma comment(lib, "cutil32D.lib")
#pragma comment(lib, "cudart.lib")

//network librarys
//#pragma comment(lib, "Ws2_32.lib") //windows network socket library

//sound librarys
//#pragma comment(lib, "OpenAL32.lib")
//#pragma comment(lib, "libogg.lib")

//#pragma comment(lib, "cublas.lib")
//#pragma comment(lib, "cuda.lib")

//#pragma comment(lib, "cufft.lib")
//#pragma comment(lib, "curand.lib")
//#pragma comment(lib, "cusparse.lib")
//#pragma comment(lib, "nvcuvenc.lib")
//#pragma comment(lib, "nvcuvid.lib")
//#pragma comment(lib, "OpenCL.lib")
//#pragma comment(lib, "Nafxcwd.lib")
//#pragma comment(lib, "Libcmtd.lib")

#include "..\external\include\glm\gtx\matrix_cross_product.hpp"
#include "..\external\include\glm\glm.hpp"
#include "..\external\include\glm\gtc\matrix_transform.hpp"

#include <stdio.h>
#include <crtdbg.h>
#include <math.h>
#include <Windows.h>

#include "..\external\include\gl\glew.h"
#include "..\external\include\gl\glut.h"
#include "..\external\include\gl\glext.h"
#include "..\external\include\gl\gl.h"

//#include "Program000.h"
//#include "Program001.h"
//#include "Program002.h"
//#include "Program003.h"
//#include "Program004.h"
#include "Program005.h"

int main( int argc, char** argv )
{
	//set memory allocation break point
	//_CrtSetBreakAlloc(155);

	printf("main: start\n");

	try
	{
		srand(1337);

		Program p;
		p.Construct();
		p.G_Run();
	}
	catch(char* ex)
	{
		printf("%s\n",ex);
		getchar();
	}

	printf("main: end\n");
	/*
	//check for memory leaks
	if( _CrtDumpMemoryLeaks() ){
		printf("main: you have memory leaks!\n");
		getchar();
	}
	*/
	//getchar();

	return 1337;
}
