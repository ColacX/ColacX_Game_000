#include "ShaderProgram.h"

Shader::Shader(const char* shaderSource, GLenum shaderType){
	this->shaderID = glCreateShader(shaderType);
    if(this->shaderID == 0){
        throw "Shader: error glCreateShader";
    }

    unsigned int bufferCapacity = 0;
    char* bufferSource = 0;
    readFile(shaderSource, bufferSource, bufferCapacity);
    this->compile(bufferSource, bufferCapacity);
	delete[] bufferSource;
}

Shader::~Shader(){
    glDeleteObjectARB(this->shaderID);
}

void Shader::readFile(const char* filePath, char*& bufferPointer, unsigned int& bufferCapacity){
    std::ifstream file(filePath, std::ios::in | std::ios::binary | std::ios::ate);
	
	if( !file.is_open() ){
		throw "Shader: error readFile";
	}
	
	bufferCapacity = (unsigned int)file.tellg();
	bufferPointer = new char[bufferCapacity+1];
	file.seekg(0, std::ios::beg);
	file.read(bufferPointer, bufferCapacity);
	file.close();
	bufferPointer[bufferCapacity] = 0;
}

void Shader::compile(const char* bufferSource, unsigned int bufferCapacity)const{
    glShaderSource(shaderID, 1, (const GLcharARB**)&bufferSource, (int*)&bufferCapacity);

    glCompileShader(shaderID);

	int iscv = 0;
	glGetObjectParameterivARB(shaderID, GL_OBJECT_COMPILE_STATUS_ARB, &iscv);
	if(!iscv){
		GLbyte infoLog[1000];
		glGetInfoLogARB(shaderID, sizeof(infoLog), 0, (GLcharARB*)infoLog);
		printf("Info log: %s\n", infoLog);
		throw "Shader: error glCompileShader";
	}
}

GLenum Shader::getID()const{
	return this->shaderID;
}

ShaderProgram::ShaderProgram(){
	shaderprogramID = glCreateProgram();
    if(shaderprogramID == 0){
        throw "ShaderProgram: error glCreateProgram";
    }
}

ShaderProgram::~ShaderProgram(){
    glDeleteObjectARB(this->shaderprogramID);
}

void ShaderProgram::attachShader(Shader& shader)const{
	glAttachShader(shaderprogramID, shader.getID());
}

void ShaderProgram::link()const{
	glLinkProgram(shaderprogramID);
	
	int islp = 0;
	glGetObjectParameterivARB(shaderprogramID, GL_OBJECT_LINK_STATUS_ARB, &islp);
	if(!islp){
		GLbyte infoLog[1000];
		glGetInfoLogARB(shaderprogramID, sizeof(infoLog), 0, (GLcharARB*)infoLog);
		printf("Info log: %s\n", infoLog);
		throw "ShaderProgram: error glLinkProgram";
	}
}

void ShaderProgram::activate()const{
	glUseProgramObjectARB(shaderprogramID);
}

void ShaderProgram::deactivate()const{
	glUseProgramObjectARB(0);
}

GLenum ShaderProgram::getID()const{
	return shaderprogramID;
}

void ShaderProgram::update(){
}

void ShaderProgram::init(){
    glewInit();

	if(GL_TRUE != glewGetExtension("GL_ARB_fragment_shader")){
	   throw "GL_ARB_fragment_shader extension is not available!";
	}
	
	if(GL_TRUE != glewGetExtension("GL_ARB_vertex_shader")){
	   throw "GL_ARB_vertex_shader extension is not available!";
	}
	
	if(GL_TRUE != glewGetExtension("GL_ARB_shader_objects")){
	   throw "GL_ARB_shader_objects extension is not available!";
	}
}

GLint ShaderProgram::getUniform(const char* uniformName)const{
    return glGetUniformLocation( this->shaderprogramID, uniformName );
}