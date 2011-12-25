#include "TriangleCompiler.h"

#include <Windows.h>
#include <gl\GL.h>
#include <stdio.h>

using namespace ColacX;

void TriangleCompiler::convert( const char *fileIn, const char *fileOut ){
	FILE *file0;
	file0 = fopen( fileIn, "r" );

	FILE *file1;
	file1 = fopen( fileOut, "w" );

	if( file0 == 0 || file1 == 0 )
		throw "TriangleCompiler: fopen failed";

	while( true )
	{
		float f;
		int scanResult = fscanf( file0, "%f", &f );

		if( scanResult != 1 )
			break;

		fwrite( &f, 1, sizeof(float), file1 );
	}

	fclose( file0 );
	fclose( file1 );
}

int TriangleCompiler::compile( const char *binaryFile )
{
	//compile from binary
	{
		//open file
		FILE *file0;
		file0 = fopen( binaryFile, "r" );

		if( file0 == 0 )
			throw "TriangleCompiler: fopen failed";
		
		//get size of file
		fseek( file0, 0, SEEK_END );
		int size = ftell( file0 );
		
		//allocate memory
		char* data = new char[size];
		
		//reset the file
		fseek( file0, 0, SEEK_SET );

		//read file into memory
		int readR = fread( data, 1, size, file0 );

		if( readR != size )
			throw "TriangleCompiler: fread failed";

		//close file
		fclose( file0 );

		float* f = (float*)data;

		int displayID = glGenLists( 1 );

		glNewList( displayID, GL_COMPILE );

		glBegin( GL_TRIANGLES );

		for( int i=0; i<size/4; i+=6 )
		{
			glNormal3f( f[i+0], f[i+1], f[i+2] );
			glVertex3f( f[i+3], f[i+4], f[i+5] );
		}

		glEnd();

		glEndList();

		delete[] data;

		return displayID;
	}
}