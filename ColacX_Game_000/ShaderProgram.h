#pragma once

#include <fstream>

#include <Windows.h>
#include "..\external\include\gl\glew.h"
#include "../external/include/gl/gl.h"

class Shader{

protected:
	void compile(const char* bufferSource, unsigned int bufferCapacity)const;

public:
	const static GLenum TYPE_VERTEX_SHADER = GL_VERTEX_SHADER;
	const static GLenum TYPE_FRAGMENT_SHADER = GL_FRAGMENT_SHADER;
    const static GLenum TYPE_GEOMETRY_SHADER = GL_GEOMETRY_SHADER_EXT;
	GLenum shaderID;

	Shader(const char* shaderSource, GLenum shaderType);
	~Shader();
    
    GLenum getID()const;

    static void readFile(const char* filePath, char*& bufferPointer, unsigned int& bufferCapacity);    
};

class ShaderProgram{

protected:
	GLenum shaderprogramID;

public:
	ShaderProgram();
	virtual ~ShaderProgram();
	void attachShader(Shader& shader)const;
	void link()const;
	void activate()const;
	void deactivate()const;
    GLenum getID()const;
    GLint getUniform(const char* uniformName)const;

    virtual void update();
    
    static void init();
    
};