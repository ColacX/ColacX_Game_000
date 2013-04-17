//wrapper for some of the typical things that you want to do with opengl
//mostly contains the error and debug handling that takes a lot of line space

#pragma once

#include <string.h>
#include <Windows.h>
#include <gl\glew.h>
#include <gl\gl.h>

#include "File.h"

class OpenGL
{
public:
	static GLuint CreateVertexShader(const char* filepath)
	{
		GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);

		char* vsText;
		unsigned int vsTextSize;
		File::ReadText(filepath, vsText, vsTextSize);

		printf("compiling vertex shader %s ...\n", filepath);
		glShaderSource(vertexShaderId, 1, (const GLchar**)&vsText, (GLint*)&vsTextSize);
		glCompileShader(vertexShaderId);

		int status = 0;
		glGetObjectParameterivARB(vertexShaderId, GL_OBJECT_COMPILE_STATUS_ARB, &status);

		if(!status)
		{
			GLbyte infoLog[1000];
			glGetInfoLogARB(vertexShaderId, sizeof(infoLog), 0, (GLcharARB*)infoLog);
			printf("Info log: %s\n", infoLog);
			throw "OpenGL: CreateVertexShader: glCompileShader failed";
		}

		printf("compiled vertex shader\n");

		delete[] vsText;

		return vertexShaderId;
	}

	static GLuint CreateGeometryShader(const char* filepath)
	{
		GLuint geometryShaderId = glCreateShader(GL_GEOMETRY_SHADER_EXT);

		char* gsText;
		unsigned int gsSize;
		File::ReadText(filepath, gsText, gsSize);

			printf("compiling geometry shader %s ...\n", filepath);
			glShaderSource(geometryShaderId, 1, (const GLchar**)&gsText, (GLint*)&gsSize);
			glCompileShader(geometryShaderId);

			int status = 0;
			glGetObjectParameterivARB(geometryShaderId, GL_OBJECT_COMPILE_STATUS_ARB, &status);

			if(!status)
			{
				GLbyte infoLog[1000];
				glGetInfoLogARB(geometryShaderId, sizeof(infoLog), 0, (GLcharARB*)infoLog);
				printf("Info log: %s\n", infoLog);
				throw "OpenGL: CreateGeometryShader: glCompileShader failed";
			}

			
			printf("compiled fragment shader\n");

		delete[] gsText;

		return geometryShaderId;
	}

	static GLuint CreateFragmentShader(const char* filepath)
	{
		GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

		char* fsText;
		unsigned int fsSize;
		File::ReadText(filepath, fsText, fsSize);

		printf("compiling fragment shader %s ...\n", filepath);
		glShaderSource(fragmentShaderId, 1, (const GLchar**)&fsText, (GLint*)&fsSize);
		glCompileShader(fragmentShaderId);

		int status = 0;
		glGetObjectParameterivARB(fragmentShaderId, GL_OBJECT_COMPILE_STATUS_ARB, &status);

		if(!status)
		{
			GLbyte infoLog[1000];
			glGetInfoLogARB(fragmentShaderId, sizeof(infoLog), 0, (GLcharARB*)infoLog);
			printf("Info log: %s\n", infoLog);
			throw "OpenGL: CreateFragmentShader: glCompileShader failed";
		}

		printf("compiling fragment shader\n");

		delete[] fsText;

		return fragmentShaderId;
	}

	static GLint GetUniformLocation(GLenum shaderProgramId, const GLchar* uniformName)
	{
		GLint uniformLocationId = glGetUniformLocation(shaderProgramId, uniformName);

		if(uniformLocationId == -1)
		{
			std::string* es = new std::string();
			es->append("glGetUniformLocation failed ");
			es->append(uniformName);
			char* exceptionPointer = (char*)es->c_str();
			throw exceptionPointer;
		}

		return uniformLocationId;
	}
};
