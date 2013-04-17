//dynamic cloud
//do with simulated points and use geometry shader to create quads
//calculate the sun lighting, reflection, refraction

#pragma once

#include "File.h"
#include "OpenGL.h"

#include <cuda_runtime.h>
#include <cuda_gl_interop.h>
#include <cutil_inline.h> 

extern void CUDA_sortParticles(float* sortKeys, unsigned int* indices, unsigned int numParticles);
extern void CUDA_calculateDepth(float3* pos, float* keys, unsigned int* indices, float3 sortVector,	int numParticles);

class Cloud001
{
private:
	GLuint lightTexture;
	GLuint lightFBO;
	GLuint lightRBO;
	unsigned int fboWidth;
	unsigned int fboHeight;
	
	GLenum shaderProgram0;
	GLint p0_modelM;
	GLint p0_lightPV;
	
	GLenum shaderProgram1;
	GLint p1_modelM;
	GLint p1_lightPV;
	GLint p1_cameraPV;
	GLint p1_lightTexture;

	unsigned int particleCount;
	GLuint vaoHandle;
	GLuint vboPosition;
	GLuint vboIndices;
	
	cudaGraphicsResource* cuda_vboPosition;
	cudaGraphicsResource* cuda_vboIndices;
	unsigned int cuda_timer;
	float* cuda_devicePointer_sortKeys;


public:
	void Construct()
	{
	}

	void C_Destruct()
	{
	}

	void G_Construct()
	{
		GLint maxTextureSize;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);

		fboWidth = 512;
		fboHeight = 512;

		//generate texture object
		{
			glGenTextures(1, &lightTexture);
			glBindTexture(GL_TEXTURE_2D, lightTexture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
			//glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
			//glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
			
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, fboWidth, fboHeight, 0, GL_RGBA, GL_FLOAT, 0);
		}

		//generate render buffer object
		{
			glGenRenderbuffers(1, &lightRBO);
			glBindRenderbuffer(GL_RENDERBUFFER, lightRBO);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, fboWidth, fboHeight);
		}

		//generate frame buffer object
		{
			glGenFramebuffers(1, &lightFBO);
			glBindFramebuffer(GL_FRAMEBUFFER, lightFBO);
			//attach 2D texture to this FBO
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, lightTexture, 0);
			//attach depth buffer to FBO
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, lightRBO);

			//check does the GPU support current FBO configuration
			if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				throw "glCheckFramebufferStatus failed";

			//reset to default render to backbuffer
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		//program0
		{
			GLuint vertexShader = OpenGL::CreateVertexShader("data/shaders/cloud001Light.vert");
			GLuint geometryShader = OpenGL::CreateGeometryShader("data/shaders/cloud001Light.geom");
			GLuint fragmentShader = OpenGL::CreateFragmentShader("data/shaders/cloud001Light.frag");

			shaderProgram0 = glCreateProgram();
			glAttachShader(shaderProgram0, vertexShader);
			glAttachShader(shaderProgram0, geometryShader);
			glAttachShader(shaderProgram0, fragmentShader);

			//geometry shader settings
			{
				//set geometry shader input and output types
				glProgramParameteri(shaderProgram0, GL_GEOMETRY_INPUT_TYPE_EXT, GL_POINTS);
				glProgramParameteri(shaderProgram0, GL_GEOMETRY_OUTPUT_TYPE_EXT, GL_TRIANGLE_STRIP);

				//fetch and set the maximum amount of vertices that can be created by the geomery shader
				int max_amount_of_vertices;
				glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT, &max_amount_of_vertices);
				glProgramParameteri(shaderProgram0, GL_GEOMETRY_VERTICES_OUT_EXT, max_amount_of_vertices);
			}

			//complete and link all shaders into a shader program
			glLinkProgram(shaderProgram0);
			p0_modelM = OpenGL::GetUniformLocation(shaderProgram0, "p0_modelM");
			p0_lightPV = OpenGL::GetUniformLocation(shaderProgram0, "p0_lightPV");
		}

		//program1
		{
			GLuint vertexShader = OpenGL::CreateVertexShader("data/shaders/cloud001.vert");
			GLuint geometryShader = OpenGL::CreateGeometryShader("data/shaders/cloud001.geom");
			GLuint fragmentShader = OpenGL::CreateFragmentShader("data/shaders/cloud001.frag");
		
			shaderProgram1 = glCreateProgram();
			glAttachShader(shaderProgram1, vertexShader);
			glAttachShader(shaderProgram1, geometryShader);
			glAttachShader(shaderProgram1, fragmentShader);

			//geometry shader settings
			{
				//set geometry shader input and output types
				glProgramParameteri(shaderProgram1, GL_GEOMETRY_INPUT_TYPE_EXT, GL_POINTS);
				glProgramParameteri(shaderProgram1, GL_GEOMETRY_OUTPUT_TYPE_EXT, GL_TRIANGLE_STRIP);

				//fetch and set the maximum amount of vertices that can be created by the geomery shader
				int max_amount_of_vertices;
				glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT, &max_amount_of_vertices);
				glProgramParameteri(shaderProgram1, GL_GEOMETRY_VERTICES_OUT_EXT, max_amount_of_vertices);
			}

			//complete and link all shaders into a shader program
			glLinkProgram(shaderProgram1);

			//fetch shader program input variables handle ids
			{
				p1_modelM = OpenGL::GetUniformLocation(shaderProgram1, "p1_modelM");
				p1_cameraPV = OpenGL::GetUniformLocation(shaderProgram1, "p1_cameraPV");
				p1_lightPV = OpenGL::GetUniformLocation(shaderProgram1, "p1_lightPV");
				p1_lightTexture = OpenGL::GetUniformLocation(shaderProgram1, "p1_lightTexture");
			}
		}

		//using vbo (vertex buffer object) a collection of vertexes stored on the gpu
		//using voa (vertex array object) a collection of vbo's
		{
			{
				//create particles
				particleCount = 8;
				glm::vec3* particleInitialState = new glm::vec3[particleCount];

				//randomize positions
				for(unsigned int ia=0; ia<particleCount; ia++)
				{
					particleInitialState[ia].x = ((float)rand() / (float)RAND_MAX) * 5.0f;
					particleInitialState[ia].y = ((float)rand() / (float)RAND_MAX) * 5.0f;
					particleInitialState[ia].z = ((float)rand() / (float)RAND_MAX) * 30.0f + 20.0f;
				}

				//create the vertex buffer objects
				vboPosition = 0;
				glGenBuffers(1, &vboPosition);
				
				//fill and upload vboPosition with initial start data
				glBindBuffer(GL_ARRAY_BUFFER, vboPosition);
				glBufferData(GL_ARRAY_BUFFER, particleCount * sizeof(glm::vec3), particleInitialState, GL_DYNAMIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				delete[] particleInitialState;

				//create and set-up the vertex array object
				glGenVertexArrays(1, &vaoHandle);
				glBindVertexArray(vaoHandle);

				//enable the vertex attribute arrays
				glEnableVertexAttribArray(0);

				//bind vbo index 0 to the shader variable "shader_vertexPosition"
				glBindAttribLocation(shaderProgram0, 0, "shader_vertexPosition");
				glBindAttribLocation(shaderProgram1, 0, "shader_vertexPosition");

				//map index 0 to the position buffer
				glBindBuffer(GL_ARRAY_BUFFER, vboPosition);
				glVertexAttribPointer(
					0, //which vertex attribute index this applies to
					3, //how many floats per vertex are to be used, must be 1, 2, 3 or 4
					GL_FLOAT, //what data type
					GL_FALSE, //should vertex be normalized?
					0, //specifies the offset between consecutive generic vertex attributes
					(GLubyte*)0 //specifies the offset of the first
				);
				glBindBuffer(GL_ARRAY_BUFFER, 0);

				//init cuda in opengl mode
				cudaGLSetGLDevice(cutGetMaxGflopsDeviceId());

				//register opengl graphics buffer to cuda
				cuda_vboPosition = 0;
				cudaGraphicsGLRegisterBuffer(
					&cuda_vboPosition,
					vboPosition,
					cudaGraphicsMapFlagsWriteDiscard //cuda will write to vbo and discard and previosly stored data
				);
			}

			{
				unsigned int* indicesInitialState = new unsigned int[particleCount];

				//alphabetical start state
				for(unsigned int ia=0; ia<particleCount; ia++)
				{
					indicesInitialState[ia] = ia;
				}

				//create the vertex buffer objects
				vboIndices = 0;
				glGenBuffers(1, &vboIndices);

				//fill and upload vboIndices with initial start data
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, particleCount * sizeof(unsigned int), indicesInitialState, GL_DYNAMIC_DRAW);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				delete[] indicesInitialState;

				//init cuda in opengl mode
				cudaGLSetGLDevice(cutGetMaxGflopsDeviceId());

				//register opengl graphics buffer to cuda
				cuda_vboIndices = 0;
				cudaGraphicsGLRegisterBuffer(
					&cuda_vboIndices,
					vboIndices,
					cudaGraphicsMapFlagsWriteDiscard //cuda will write to vbo and discard and previosly stored data
				);
			}

			//allocate sort keys for sorting particles along the sorting vector
			cuda_devicePointer_sortKeys = 0;
			cudaMalloc(&cuda_devicePointer_sortKeys, particleCount * sizeof(float));
		}
	}

	void G_Destruct()
	{
		cudaGraphicsUnregisterResource(cuda_vboPosition);
		glDeleteBuffers(1, &vboPosition);
	}

	void G_Run()
	{
	}

	void G_DrawParticles()
	{
		/*
		float zf = 100;
		glBegin(GL_POINTS);
		glVertex3f(0, 0, 30 + zf);
		glVertex3f(0, 0, 50 + zf);
		glVertex3f(30, 0, 70 + zf);
		glVertex3f(0, 10, 40 + zf);
		glVertex3f(-10, -10, 70 + zf);
		glVertex3f(+30, -10, 50 + zf);
		glVertex3f(+30, +10, 40 + zf);
		glEnd();
		*/

		/*
		glBindVertexArray(vaoHandle);
		glDrawArrays(GL_POINTS, 0, particleCount);
		*/

		//bind vertex data and indice data then draw
		glBindBufferARB(GL_ARRAY_BUFFER, vboPosition);
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
        glDrawElements(GL_POINTS, particleCount, GL_UNSIGNED_INT, (void*)( (0) * sizeof(unsigned int)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void G_Draw(Camera000& camera000, unsigned int framecount)
	{
		//run cuda calculations
		{
			//map cuda graphics resources
			//when mapped opengl may not use theese resources or else the results are undefined
			float3* cuda_devicePointer_vboPosition = 0;

			{
				cudaGraphicsMapResources(1, &cuda_vboPosition, 0);
				size_t byteCount = 0;
				
				cudaGraphicsResourceGetMappedPointer((void**)&cuda_devicePointer_vboPosition, &byteCount, cuda_vboPosition);
			}

			unsigned int* cuda_devicePointer_vboIndices = 0;
			
			{
				cudaGraphicsMapResources(1, &cuda_vboIndices, 0);
				size_t byteCount = 0;
				cudaGraphicsResourceGetMappedPointer((void**)&cuda_devicePointer_vboIndices, &byteCount, cuda_vboIndices);
			}

			//calculate sortKeys
			glm::vec3 halfVector(0, 0, -1);
			float3 sortVector = make_float3(halfVector.x, halfVector.y, halfVector.z);
			CUDA_calculateDepth(cuda_devicePointer_vboPosition, cuda_devicePointer_sortKeys, cuda_devicePointer_vboIndices, sortVector, particleCount);

			//sort by keys
			CUDA_sortParticles(cuda_devicePointer_sortKeys, cuda_devicePointer_vboIndices, particleCount);

			//unmap cuda graphics resources
			{
				cudaGraphicsUnmapResources(1, &cuda_vboPosition, 0);
				cuda_devicePointer_vboPosition = 0;
			}

			{
				cudaGraphicsUnmapResources(1, &cuda_vboIndices, 0);
				cuda_devicePointer_vboIndices = 0;
			}
		}

		glm::mat4 modelM = glm::rotate(glm::mat4(), 225.0f, 0.0f, 1.0f, 0.0f);
		
		//light point of view
		glm::mat4 lightP = glm::perspective(60.0f, float(fboWidth)/float(fboHeight), 0.01f, 1000.0f);
		glm::mat4 lightV = glm::inverse(glm::rotate(glm::mat4(), 180.0f, 0.0f, 1.0f, 0.0f));
		glm::mat4 lightPV = lightP * lightV;

		//render to lightFBO and lightTexture
		{
			glBindFramebuffer(GL_FRAMEBUFFER, lightFBO);
			glViewport(0, 0, fboWidth, fboHeight);

			glClearColor(0.0, 0.0, 0.0, 0.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//render something
			{
				glEnable(GL_DEPTH_TEST);
				// front-to-back
				//glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_ONE);
				// back-to-front
				//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

				//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
				//glEnable(GL_BLEND);
				glDisable(GL_BLEND);

				glUseProgram(shaderProgram0);

				//upload shader program variables
				glUniformMatrix4fv(p0_modelM, 1, GL_FALSE, &modelM[0][0]);
				glUniformMatrix4fv(p0_lightPV, 1, GL_FALSE, &lightPV[0][0]);

				G_DrawParticles();

				glUseProgram(0);
				//glDisable(GL_BLEND);
			}

			//undo gpu state changes
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			//glClearColor(0.0, 0.0, 0.0, 0.0);
			camera000.G_ResizeView();
			camera000.G_SetModelView();
		}

		//draw the fbo for debugging
		if(1){
			glViewport(0, 0, fboWidth, fboHeight);

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0.0, fboWidth, 0.0, fboHeight, -1.0, 1.0);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			glClearColor(0.0, 0.0, 0.0, 0.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glColor4f(1, 1, 1, 1);
			glActiveTexture(GL_TEXTURE0);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, lightTexture);
			glGenerateMipmap(GL_TEXTURE_2D);

			glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2f(0, 0);
			glVertex3f(0, 0, 0);

			glTexCoord2f(1, 0);
			glVertex3f(fboWidth, 0, 0);

			glTexCoord2f(0, 1);
			glVertex3f(0, fboHeight, 0);

			glTexCoord2f(1, 1);
			glVertex3f(fboWidth, fboHeight, 0);
			glEnd();

			//undo gpu state changes
			glBindTexture(GL_TEXTURE_2D, 0);
			camera000.G_ResizeView();
			camera000.G_SetModelView();
		}

		glm::mat4 cameraPV = camera000.ProjectionMatrix() * camera000.ViewMatrix();
		//cameraPV = lightPV;

		{
			glEnable(GL_DEPTH_TEST);
			// front-to-back
			//glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_ONE);
			// back-to-front
			//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);
			//glDisable(GL_BLEND);

			glUseProgram(shaderProgram1);

			//upload shader program variables
			glUniformMatrix4fv(p1_modelM, 1, GL_FALSE, &modelM[0][0]);
			glUniformMatrix4fv(p1_lightPV, 1, GL_FALSE, &lightPV[0][0]);
			glUniformMatrix4fv(p1_cameraPV, 1, GL_FALSE, &cameraPV[0][0]);

			glUniform1i(p1_lightTexture, 0);
			glActiveTexture(GL_TEXTURE0);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, lightTexture);

			//draw the input type not the output type
			//glBindVertexArray(vaoHandle);
			//glDrawArrays(GL_POINTS, 0, particleCount);

			G_DrawParticles();

			glUseProgram(0);

			glDisable(GL_BLEND);
		}
	}
};
