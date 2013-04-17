//cheap static cloud

//constant terrain using shader to map multiplier textures

#pragma once

#include "File.h"
#include "Texture.h"
#include "Random.h"
#include "Misc.h"

#include "Camera000.h"

class TessellationTeapot
{
private:
	static const int sizeX = 100;
    static const int sizeY = 100;
	static const int sizeV = 3;
	static const int sizeQ = 6;
	int vertexCount;
	float* vertexData;
	int triangleCount;
	float* triangleData;

    GLenum shaderProgram0;
	GLuint vaoHandle;
	GLint uniform_world_matrix;
    GLint uniform_framecount;
	GLint uniform_texture0;
	GLint uniform_texture1;
	
	Texture texture0;
	Texture texture1;

public:
    void Construct()
    {
		Random random;
		random.Seed(1337);
		
		//generate points
		vertexCount = (sizeY+1) * (sizeX+1);
		vertexData = new float[vertexCount*sizeV];

		for( int y=0; y<=sizeY; y++ )
        {
            for( int x=0; x<=sizeX; x++ )
            {
				unsigned int vertex0 = y*(sizeX+1)*sizeV + x*sizeV;

				vertexData[vertex0 + 0] = x;
				vertexData[vertex0 + 1] = y;
				vertexData[vertex0 + 2] = random.Next(-1.0f, 1.0f) * random.Next(-1.0f, 1.0f) * random.Next(-1.0f, 1.0f) * random.Next(-1.0f, 1.0f) * random.Next(-1.0f, 1.0f) * 5.0f;
			}
		}

		//for(int i=0; i<vertexCount; i++)
		//	printf("v%d: %f %f %f\n",i, vertexData[i*sizeV + 0], vertexData[i*sizeV + 1], vertexData[i*sizeV + 2]);

		//generate quads of triangles from vertex points
		triangleCount = (sizeY)*(sizeX)*sizeQ;
		triangleData = new float[triangleCount*sizeV];

		for( int y=0; y<sizeY; y++ )
        {
            for( int x=0; x<sizeX; x++ )
            {
				unsigned int vertex0 = (y+0)*(sizeX+1)*sizeV + (x+0)*sizeV;
				unsigned int vertex1 = (y+0)*(sizeX+1)*sizeV + (x+1)*sizeV;
				unsigned int vertex2 = (y+1)*(sizeX+1)*sizeV + (x+0)*sizeV;
				unsigned int vertex3 = (y+1)*(sizeX+1)*sizeV + (x+1)*sizeV;

				unsigned int quad0 = y*sizeX*sizeQ*sizeV + x*sizeQ*sizeV;

				//triangle 0
				unsigned int triangleVertex0 = quad0 + 0*sizeV;
				triangleData[triangleVertex0 + 0] = vertexData[vertex0 + 0];
				triangleData[triangleVertex0 + 1] = vertexData[vertex0 + 1];
				triangleData[triangleVertex0 + 2] = vertexData[vertex0 + 2];

				unsigned int triangleVertex1 = quad0 + 1*sizeV;
				triangleData[triangleVertex1 + 0] = vertexData[vertex2 + 0];
				triangleData[triangleVertex1 + 1] = vertexData[vertex2 + 1];
				triangleData[triangleVertex1 + 2] = vertexData[vertex2 + 2];

				unsigned int triangleVertex2 = quad0 + 2*sizeV;
				triangleData[triangleVertex2 + 0] = vertexData[vertex1 + 0];
				triangleData[triangleVertex2 + 1] = vertexData[vertex1 + 1];
				triangleData[triangleVertex2 + 2] = vertexData[vertex1 + 2];

				//triangle 1
				unsigned int triangleVertex3 = quad0 + 3*sizeV;
				triangleData[triangleVertex3 + 0] = vertexData[vertex1 + 0];
				triangleData[triangleVertex3 + 1] = vertexData[vertex1 + 1];
				triangleData[triangleVertex3 + 2] = vertexData[vertex1 + 2];

				unsigned int triangleVertex4 = quad0 + 4*sizeV;
				triangleData[triangleVertex4 + 0] = vertexData[vertex2 + 0];
				triangleData[triangleVertex4 + 1] = vertexData[vertex2 + 1];
				triangleData[triangleVertex4 + 2] = vertexData[vertex2 + 2];

				unsigned int triangleVertex5 = quad0 + 5*sizeV;
				triangleData[triangleVertex5 + 0] = vertexData[vertex3 + 0];
				triangleData[triangleVertex5 + 1] = vertexData[vertex3 + 1];
				triangleData[triangleVertex5 + 2] = vertexData[vertex3 + 2];
			}
		}

		//for(int i=0; i<triangleCount; i++)
		//	printf("tv%d: %f %f %f\n",i, triangleData[i*sizeV + 0], triangleData[i*sizeV + 1], triangleData[i*sizeV + 2]);
    }

    void C_Destruct()
	{
		delete[] vertexData; //can be deleted earlier after G_Construct
		delete[] triangleData; //can be deleted earlier after G_Construct
    }

    void Run()
	{
    }

    void G_Run()
	{
    }

    void G_Construct()
    {
        char *vs;
		unsigned int vsc;
		//File::ReadText("data/shaders/simple_multitexture.vert", vs, vsc);
		File::ReadText("data/shaders/test.vert", vs, vsc);

		char *gs;
		unsigned int gsc;
		File::ReadText("data/shaders/test.geom", gs, gsc);
		
		char *fs;
		unsigned int fsc;
		//File::ReadText("data/shaders/simple_multitexture.frag", fs, fsc);
		File::ReadText("data/shaders/test.frag", fs, fsc);

		shaderProgram0 = glCreateProgram();

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

			glAttachShader(shaderProgram0,v);
		}

		if(false)
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

			glAttachShader(shaderProgram0,g);

			glProgramParameteriEXT(shaderProgram0, GL_GEOMETRY_INPUT_TYPE_EXT, GL_TRIANGLES);
			glProgramParameteriEXT(shaderProgram0, GL_GEOMETRY_OUTPUT_TYPE_EXT, GL_TRIANGLES);

			int temp;
			glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT,&temp);
			glProgramParameteriEXT(shaderProgram0,GL_GEOMETRY_VERTICES_OUT_EXT,temp);
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

			glAttachShader(shaderProgram0,f);
		}

		// Bind index 0 to the shader input variable "VertexPosition"
		glBindAttribLocation(shaderProgram0, 0, "VertexPosition");		

		// Create the buffer objects
		GLuint positionBufferHandle;
		glGenBuffers(1, &positionBufferHandle);
				
		glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
		glBufferData(GL_ARRAY_BUFFER, triangleCount*sizeV*sizeof(float), triangleData, GL_STATIC_DRAW);

		// Create and set-up the vertex array object
		glGenVertexArrays( 1, &vaoHandle );
		glBindVertexArray(vaoHandle);
		// Enable the vertex attribute arrays
		glEnableVertexAttribArray(0);  // Vertex position
		// Map index 0 to the position buffer
		glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );

		glLinkProgram(shaderProgram0);
		
		//fetch shader program variables
		uniform_world_matrix = glGetUniformLocation(shaderProgram0, "uniform_world_matrix");

		if( uniform_world_matrix == -1 )
			throw "glGetUniformLocation failed";

		uniform_texture0 = glGetUniformLocation(shaderProgram0, "uniform_texture0");

		if( uniform_texture0 == -1 )
			throw "glGetUniformLocation failed";

		uniform_texture1 = glGetUniformLocation(shaderProgram0, "uniform_texture1");

		if( uniform_texture1 == -1 )
			throw "glGetUniformLocation failed";

		texture0.G_Construct("data/images/sand_128.tga");
		texture1.G_Construct("data/images/border_128.tga");
    }

    void G_Destruct()
	{
		texture0.G_Destruct();
		texture1.G_Destruct();
    }

    void G_Draw(Camera000& camera000, int framecount)
    {
        glm::mat4 matrix = camera000.ProjectionMatrix() * camera000.ViewMatrix();
		//matrix = glm::translate(matrix, glm::vec3(-sizeX/2, 0.0f, -sizeY/2));
		matrix = glm::rotate(matrix, 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		
		glUseProgram(shaderProgram0);

		//upload shader program variables
		glUniformMatrix4fv(uniform_world_matrix, 1, GL_FALSE, &matrix[0][0]);
		glUniform1i( uniform_texture0, 0 );
		glUniform1i( uniform_texture1, 1 );

		//draw object water
		//bind textures
		glActiveTexture( GL_TEXTURE0 );
		glEnable( GL_TEXTURE_2D );
		texture0.G_Bind();
		glActiveTexture( GL_TEXTURE1 );
		glEnable( GL_TEXTURE_2D );
		texture1.G_Bind();

		glBindVertexArray(vaoHandle); //cpu demanding
		glDrawArrays(GL_TRIANGLES, 0, sizeY * sizeX * sizeV * sizeQ);

        glUseProgram(0);
    }
};