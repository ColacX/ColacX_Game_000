//water using shader to animate waves and textures

#pragma once

#include "File.h"
#include "Texture.h"

#include "Window.h"
#include "Camera000.h"

class Water000
{
private:
	static const int sizeX = 100;
    static const int sizeY = 100;
    static const int sizeV = 6;
	static const int sizeS = 6;
	unsigned int dataSize;
	float* data;

    GLenum shaderProgram0;
	GLuint vaoHandle;
	GLint uniform_world_matrix;
    GLint uniform_framecount;
	GLint uniform_texture0;
	GLint uniform_texture1;
	
	Texture texture0;
	Texture texture1;

	// Create the buffer objects
	GLuint positionBufferHandle;

public:
    void Construct()
    {
		dataSize = sizeY * sizeX * sizeV * sizeS * sizeof(float);
		data = new float[dataSize];

        for( int y=0; y<sizeY; y++ )
        {
            for( int x=0; x<sizeX; x++ )
            {
				//triangle 0
				//0
				data[y*sizeX*sizeV*sizeS + x*sizeV*sizeS +  0] = x + 0; 
				data[y*sizeX*sizeV*sizeS + x*sizeV*sizeS +  1] = y + 0; 
				data[y*sizeX*sizeV*sizeS + x*sizeV*sizeS +  2] = 0 + 0;

				//1
				data[y*sizeX*sizeV*sizeS + x*sizeV*sizeS +  3] = x + 0;
				data[y*sizeX*sizeV*sizeS + x*sizeV*sizeS +  4] = y + 1;
				data[y*sizeX*sizeV*sizeS + x*sizeV*sizeS +  5] = 0 + 0;

				//2
				data[y*sizeX*sizeV*sizeS + x*sizeV*sizeS +  6] = x + 1;
				data[y*sizeX*sizeV*sizeS + x*sizeV*sizeS +  7] = y + 0;
				data[y*sizeX*sizeV*sizeS + x*sizeV*sizeS +  8] = 0 + 0;

				//triangle 1
				//2
				data[y*sizeX*sizeV*sizeS + x*sizeV*sizeS +  9] = x + 1;
				data[y*sizeX*sizeV*sizeS + x*sizeV*sizeS + 10] = y + 0;
				data[y*sizeX*sizeV*sizeS + x*sizeV*sizeS + 11] = 0 + 0;

				//1
				data[y*sizeX*sizeV*sizeS + x*sizeV*sizeS + 12] = x + 0;
				data[y*sizeX*sizeV*sizeS + x*sizeV*sizeS + 13] = y + 1;
				data[y*sizeX*sizeV*sizeS + x*sizeV*sizeS + 14] = 0 + 0;
				
				//3
				data[y*sizeX*sizeV*sizeS + x*sizeV*sizeS + 15] = x + 1;
				data[y*sizeX*sizeV*sizeS + x*sizeV*sizeS + 16] = y + 1;
				data[y*sizeX*sizeV*sizeS + x*sizeV*sizeS + 17] = 0 + 0;
            }
        }
    }

    void C_Destruct()
	{
		delete[dataSize] data;
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
		File::ReadText("data/shaders/Water000.vert", vs, vsc);
		
		char *fs;
		unsigned int fsc;
		//File::ReadText("data/shaders/simple_multitexture.frag", fs, fsc);
		File::ReadText("data/shaders/Water000.frag", fs, fsc);

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

		
		glGenBuffers(1, &positionBufferHandle);
				
		glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
		glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
		
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

		uniform_framecount = glGetUniformLocation(shaderProgram0, "uniform_framecount");

		if( uniform_framecount == -1 )
			throw "glGetUniformLocation failed";

		uniform_texture0 = glGetUniformLocation(shaderProgram0, "uniform_texture0");

		if( uniform_texture0 == -1 )
			throw "glGetUniformLocation failed";

		uniform_texture1 = glGetUniformLocation(shaderProgram0, "uniform_texture1");

		if( uniform_texture1 == -1 )
			throw "glGetUniformLocation failed";

		texture0.G_Construct("data/images/water_ocean_128.tga");
		texture1.G_Construct("data/images/water_tropics_128.tga");
    }

    void G_Destruct()
	{
		texture0.G_Destruct();
		texture1.G_Destruct();
    }

    void G_Draw(Camera000& camera000, int framecount)
    {
        glm::mat4 matrix = camera000.ProjectionMatrix() * camera000.ViewMatrix();
		//matrix = glm::translate(matrix, glm::vec3(0.0f, 1.0f, 0.0f));
		matrix = glm::rotate(matrix, 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		
		glUseProgram(shaderProgram0);

		//upload shader program variables
		glUniformMatrix4fv(uniform_world_matrix, 1, GL_FALSE, &matrix[0][0]);
		glUniform1i( uniform_framecount, framecount );
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

		//glBindVertexArray(vaoHandle); //cpu demanding
		// Map index 0 to the position buffer
		glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL );
		glDrawArrays(GL_TRIANGLES, 0, sizeY * sizeX * sizeV * sizeS );

        glUseProgram(0);
    }
};