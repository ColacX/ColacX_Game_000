#pragma once

#include <Windows.h>
#include <gl\GL.h>
#include <stdio.h>
#include <list>

namespace ColacX
{
	class ASEcompiler{
	private:
		FILE *file0;
		char s[1000];
		float f;
		int scanResult;

		int MESH_NUMVERTEX;
		float *MESH_VERTEX_ARRAY;
		int MESH_NUMFACES;
		int *MESH_FACE_ARRAY;
		int MESH_NUMTVERTEX;
		float *MESH_TVERT_ARRAY;
		int MESH_NUMTVFACES;
		int *MESH_TFACE_ARRAY;
		float *MESH_FACENORMAL_ARRAY;
		float *MESH_VERTEXNORMAL_ARRAY;

		bool ReadString()
		{
			scanResult = fscanf_s( file0, "%s", s, sizeof(s));

			if( scanResult == EOF )
				return false;

			if( scanResult != 1 )
				throw "ASEcompiler: fscanf failed";

			return true;
		}

		bool ReadFloat()
		{
			scanResult = fscanf_s( file0, "%f", &f);

			if( scanResult == EOF )
				return false;

			if( scanResult != 1 )
				throw "ASEcompiler: fscanf failed";

			return true;
		}

		bool Matches( const char* value )
		{
			return strcmp( s, value ) == 0;
		}

		void PARSE()
		{
			while( ReadString() )
			{
				if( Matches( "*GEOMOBJECT" ) )
					GEOMOBJECT();
			}
		}

		void GEOMOBJECT()
		{
			while( ReadString() )
			{
				if( Matches( "*NODE_TM" ) )
					NODE_TM();
				else if( Matches( "*MESH" ) )
					MESH();
				else if( Matches( "}" ) )
					break;
			}
		}

		void NODE_TM()
		{
			while( ReadString() )
			{
				if( Matches( "}" ) )
					break;
			}
		}

		void MESH()
		{
			while( ReadString() )
			{
				if( Matches( "*MESH_NUMVERTEX" ) )
				{
					ReadFloat();
					MESH_NUMVERTEX = (int)f;
					MESH_VERTEX_ARRAY = new float[ MESH_NUMVERTEX * 3 ];
				}
				else if( Matches( "*MESH_NUMFACES" ) )
				{
					ReadFloat();
					MESH_NUMFACES = (int)f;
					MESH_FACE_ARRAY = new int[ MESH_NUMFACES * 3 ];
					MESH_FACENORMAL_ARRAY = new float[ MESH_NUMFACES * 3 ];
					MESH_VERTEXNORMAL_ARRAY = new float[ MESH_NUMFACES * 3 ];
				}
				else if( Matches( "*MESH_VERTEX_LIST" ) )
					MESH_VERTEX_LIST();
				else if( Matches( "*MESH_FACE_LIST" ) )
					MESH_FACE_LIST();
				else if( Matches( "*MESH_NUMTVERTEX" ) )
				{
					ReadFloat();
					MESH_NUMTVERTEX = (int)f;
					MESH_TVERT_ARRAY = new float[ MESH_NUMTVERTEX * 3 ];
				}
				else if( Matches( "*MESH_TVERTLIST" ) )
					MESH_TVERTLIST();
				else if( Matches( "*MESH_NUMTVFACES" ) )
				{
					ReadFloat();
					MESH_NUMTVFACES = (int)f;
					MESH_TFACE_ARRAY = new int[ MESH_NUMTVFACES * 3 ];
				}
				else if( Matches( "*MESH_TFACELIST" ) )
					MESH_TFACELIST();
				else if( Matches( "*MESH_NORMALS" ) )
					MESH_NORMALS();
				else if( Matches( "}" ) )
					break;
			}
		}

		void MESH_VERTEX_LIST()
		{
			while( ReadString() )
			{
				if( Matches( "*MESH_VERTEX" ) )
				{
					ReadFloat();
					int n = (int)f * 3;
					ReadFloat();
					MESH_VERTEX_ARRAY[n+0] = f;
					ReadFloat();
					MESH_VERTEX_ARRAY[n+1] = f;
					ReadFloat();
					MESH_VERTEX_ARRAY[n+2] = f;
				}
				else if( Matches( "}" ) )
					break;
			}
		}

		void MESH_FACE_LIST()
		{
			while( ReadString() )
			{
				if( Matches( "*MESH_FACE" ) )
				{
					ReadFloat();
					int n = (int)f*3;
					ReadString();
					ReadString();
					ReadFloat();
					MESH_FACE_ARRAY[n+0] = (int)f;
					ReadString();
					ReadFloat();
					MESH_FACE_ARRAY[n+1] = (int)f;
					ReadString();
					ReadFloat();
					MESH_FACE_ARRAY[n+2] = (int)f;
				}
				else if( Matches( "}" ) )
					break;
			}
		}

		void MESH_TVERTLIST()
		{
			while( ReadString() )
			{
				if( Matches( "*MESH_TVERT" ) )
				{
					ReadFloat();
					int n = (int)f * 3;
					ReadFloat();
					MESH_TVERT_ARRAY[n+0] = f;
					ReadFloat();
					MESH_TVERT_ARRAY[n+1] = f;
					ReadFloat();
					MESH_TVERT_ARRAY[n+2] = f;
				}
				else if( Matches( "}" ) )
					break;
			}
		}

		void MESH_TFACELIST()
		{
			while( ReadString() )
			{
				if( Matches( "*MESH_TFACE" ) )
				{
					ReadFloat();
					int n = (int)f * 3;
					ReadFloat();
					MESH_TFACE_ARRAY[n+0] = (int)f;
					ReadFloat();
					MESH_TFACE_ARRAY[n+1] = (int)f;
					ReadFloat();
					MESH_TFACE_ARRAY[n+2] = (int)f;
				}
				else if( Matches( "}" ) )
					break;
			}
		}

		void MESH_NORMALS()
		{
			while( ReadString() )
			{
				if( Matches( "*MESH_FACENORMAL" ) )
				{
					ReadFloat();
					int n = (int)f * 3;
					ReadFloat();
					MESH_FACENORMAL_ARRAY[n+0] = f;
					ReadFloat();
					MESH_FACENORMAL_ARRAY[n+1] = f;
					ReadFloat();
					MESH_FACENORMAL_ARRAY[n+2] = f;
				}
				else if( Matches( "*MESH_VERTEXNORMAL" ) )
				{
					ReadFloat();
					int n = (int)f * 3;
					ReadFloat();
					MESH_VERTEXNORMAL_ARRAY[n+0] = f;
					ReadFloat();
					MESH_VERTEXNORMAL_ARRAY[n+1] = f;
					ReadFloat();
					MESH_VERTEXNORMAL_ARRAY[n+2] = f;
				}
				else if( Matches( "}" ) )
					break;
			}
		}

	public:

		unsigned int Compile( const char* filePath ){
			//open file
			if( fopen_s( &file0, filePath, "r" ) != 0 )
				throw "TriangleCompiler: fopen failed";

			//read contents
			PARSE();

			//close file
			fclose( file0 );

			//compile
			unsigned int displayID;

			displayID = glGenLists(1);

			glNewList( displayID, GL_COMPILE );

			glBegin( GL_TRIANGLES );

			for( int i=0; i<MESH_NUMFACES; i++ )
			{
				int f = i * 3;

				//each face points to 3 vertexes
				int a = MESH_FACE_ARRAY[ f + 0 ] * 3;
				int b = MESH_FACE_ARRAY[ f + 1 ] * 3;
				int c = MESH_FACE_ARRAY[ f + 2 ] * 3;

				//each vertex has 3 float values
				float vaa = MESH_VERTEX_ARRAY[ a + 0 ];
				float vab = MESH_VERTEX_ARRAY[ a + 1 ];
				float vac = MESH_VERTEX_ARRAY[ a + 2 ];

				float vba = MESH_VERTEX_ARRAY[ b + 0 ];
				float vbb = MESH_VERTEX_ARRAY[ b + 1 ];
				float vbc = MESH_VERTEX_ARRAY[ b + 2 ];

				float vca = MESH_VERTEX_ARRAY[ c + 0 ];
				float vcb = MESH_VERTEX_ARRAY[ c + 1 ];
				float vcc = MESH_VERTEX_ARRAY[ c + 2 ];

				//each face has a vertex normal
				float naa = MESH_VERTEXNORMAL_ARRAY[ a + 0 ];
				float nab = MESH_VERTEXNORMAL_ARRAY[ a + 1 ];
				float nac = MESH_VERTEXNORMAL_ARRAY[ a + 2 ];

				float nba = MESH_VERTEXNORMAL_ARRAY[ b + 0 ];
				float nbb = MESH_VERTEXNORMAL_ARRAY[ b + 1 ];
				float nbc = MESH_VERTEXNORMAL_ARRAY[ b + 2 ];

				float nca = MESH_VERTEXNORMAL_ARRAY[ c + 0 ];
				float ncb = MESH_VERTEXNORMAL_ARRAY[ c + 1 ];
				float ncc = MESH_VERTEXNORMAL_ARRAY[ c + 2 ];

				//each texture face points to 3 texture vertexes
				int ta = MESH_TFACE_ARRAY[ i*3 + 0 ] * 3;
				int tb = MESH_TFACE_ARRAY[ i*3 + 1 ] * 3;
				int tc = MESH_TFACE_ARRAY[ i*3 + 2 ] * 3;

				//each texture vertex has 3 float values
				float taa = MESH_TVERT_ARRAY[ ta + 0 ];
				float tab = MESH_TVERT_ARRAY[ ta + 1 ];
				float tac = MESH_TVERT_ARRAY[ ta + 2 ];

				float tba = MESH_TVERT_ARRAY[ tb + 0 ];
				float tbb = MESH_TVERT_ARRAY[ tb + 1 ];
				float tbc = MESH_TVERT_ARRAY[ tb + 2 ];

				float tca = MESH_TVERT_ARRAY[ tc + 0 ];
				float tcb = MESH_TVERT_ARRAY[ tc + 1 ];
				float tcc = MESH_TVERT_ARRAY[ tc + 2 ];

				//draw face vertexes with normals and textures
				//glNormal3f( MESH_FACENORMAL_ARRAY[f+0], MESH_FACENORMAL_ARRAY[f+1], MESH_FACENORMAL_ARRAY[f+2] );

				glNormal3f( naa, nab, nac );
				glTexCoord3f( taa, tab, tac );
				glVertex3f( vaa, vab, vac );

				glNormal3f( nba, nbb, nbc );
				glTexCoord3f( tba, tbb, tbc );
				glVertex3f( vba, vbb, vbc );

				glNormal3f( nca, ncb, ncc );
				glTexCoord3f( tca, tcb, tcc );
				glVertex3f( vca, vcb, vcc );
			}

			glEnd();

			glEndList();

			delete[] MESH_VERTEX_ARRAY;
			delete[] MESH_FACE_ARRAY;
			delete[] MESH_TVERT_ARRAY;
			delete[] MESH_TFACE_ARRAY;
			delete[] MESH_FACENORMAL_ARRAY;
			delete[] MESH_VERTEXNORMAL_ARRAY;

			return displayID;
		}
	};
};