#pragma comment(lib, "Ws2_32.lib") //windows network socket library
#pragma comment(lib, "opengl32.lib") //opengl library for 32-bit computers
#pragma comment(lib, "glu32.lib") //openglu32 librarary extension to opengl
#pragma comment(lib, "glew32.lib") //glew librarary extension to opengl
//#pragma comment(lib, "OpenAL32.lib")
//#pragma comment(lib, "libofgg.lib")
#pragma comment(lib, "cublas.lib")
#pragma comment(lib, "cuda.lib")
#pragma comment(lib, "cudart.lib")
#pragma comment(lib, "cufft.lib")
#pragma comment(lib, "curand.lib")
#pragma comment(lib, "cusparse.lib")
#pragma comment(lib, "nvcuvenc.lib")
#pragma comment(lib, "nvcuvid.lib")
#pragma comment(lib, "OpenCL.lib")

#include <stdio.h>
#include <crtdbg.h>
#include <math.h>
#include <Windows.h>
#include <gl\glew.h>
#include <gl\gl.h>

//#include "Program000.h"
#include "Program001.h"

int main( int argc, char** argv )
{
	//set memory allocation break point
	//_CrtSetBreakAlloc(155);

	printf("main: start\n");

	try
	{
		Program001 p;
		p.Run();
	}
	catch( char* ex )
	{
		printf("%s\n",ex);
		//getchar();
	}

	printf("main: end\n");

	//check for memory leaks
	if( _CrtDumpMemoryLeaks() ){
		printf("main: you have memory leaks!\n");
		//getchar();
	}

	getchar();

	return 1337;
}
