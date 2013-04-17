#pragma once

#include "..\external\include\glm\gtx\vector_angle.hpp"
#include "..\external\include\glm\gtx\matrix_cross_product.hpp"
#include "..\external\include\glm\glm.hpp"
#include "..\external\include\glm\gtc\matrix_transform.hpp"
#include "..\external\include\glm\gtc\quaternion.hpp"

class Camera000
{
private:
	glm::mat4 projection;
	glm::mat4 rotation;
	glm::vec3 position;
	unsigned int width;
	unsigned int height;

public:
	glm::mat4 ProjectionMatrix()
	{
		return projection;
	}

	glm::mat4 RotationMatrix()
	{
		return rotation;
	}

	glm::mat4 ViewMatrix()
	{
		glm::mat4 inverseRotation = glm::inverse(rotation);
		return glm::translate(inverseRotation, position*(-1.0f));
	}

	glm::mat4 CameraMatrix()
	{
		return glm::translate(rotation, position);
	}

	void MoveLocal(float x, float y, float z)
	{
		glm::vec4 globalMotion = rotation * glm::vec4(x, y, z, 0);
		MoveGlobal(globalMotion.x, globalMotion.y, globalMotion.z);
	}

	void MoveGlobal(float x, float y, float z)
	{
		position.x += x;
		position.y += y;
		position.z += z;
	}

	void RotateLocalX(float v)
	{
		glm::vec3 axis(1,0,0);
		rotation = glm::rotate(rotation, v, axis);
	}

	void RotateLocalY(float v)
	{
		glm::vec3 axis(0,1,0);
		rotation = glm::rotate(rotation, v, axis);
	}

	void RotateLocalZ(float v)
	{
		glm::vec3 axis(0,0,1);
		rotation = glm::rotate(rotation, v, axis);
	}

	void LookAt(float tx, float ty, float tz)
	{
		//todo fix this
		//effect wanted is arcball rotation determine the shortest arc from current rotation to next rotation where it looks at target xyz
	
		glm::vec3 targetDirection = glm::normalize(glm::vec3(tx,ty,tz) - position);
		glm::vec3 currentDirection = glm::normalize(-glm::vec3(rotation[2]));
		glm::vec3 orthogonalVector = glm::normalize(glm::cross(targetDirection, currentDirection));
		float angle = -glm::angle(currentDirection, targetDirection);

		if(angle != angle) //check for NaN
			return; //current direction is already close to target direction

		glm::mat4 targetRotation = glm::rotate(rotation, angle, orthogonalVector);

		rotation = targetRotation;
		

		/*
		glm::vec3 up(rotation[1]); //the roll of the rotation
		//glm::vec3 up(ux,uy,uz); //the roll of the rotation
		glm::vec3 target(tx,ty,tz); //the direction of the rotation
		glm::mat4 lookAt = glm::lookAt(position, target, up);
		rotation = glm::inverse(lookAt); //inverse inverse?
		*/
	}

	void Rotate()
	{
		//quaternion type slerp
	}

	void G_Construct()
	{
		//setup basic rendering options
		{
			glShadeModel(GL_SMOOTH);
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClearDepth(1.0f);

			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);

			//glEnable(GL_LINE_SMOOTH);
			//glEnable(GL_POINT_SMOOTH);
			//glEnable(GL_POLYGON_SMOOTH);

			glHint(GL_FRAGMENT_SHADER_DERIVATIVE_HINT, GL_NICEST); //is available only if the GL version is 2.0 or greater.
			//glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST); //is available only if the GL version is 1.4 or greater.
			//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
			//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
			//glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
			//glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
			//glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST); //is available only if the GL version is 1.3 or greater.
	
			glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);
			glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
			glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
		}

		//wireframe mode
		if(false)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		//draw only front side of polygon
		if(false)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
		}

		//alpha blending
		if(true)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		//lighting
		if(false)
		{
			glEnable(GL_LIGHT0);

			float ambientLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			float diffuseLight[] = { 1.0f, 1.0f, 1.0, 1.0f };
			float specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
			float position[] = { 0.0f, 0.0f, 10.0f, 0.0f };

			glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
			glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
			glLightfv(GL_LIGHT0, GL_POSITION, position);

			glEnable(GL_LIGHTING);
		}

		//fog
		if(false)
		{
			GLfloat fogColor[] = {0.0f, 0.0f, 0.0f, 1.0f};
			glFogi(GL_FOG_MODE, GL_LINEAR);
			glFogfv(GL_FOG_COLOR, fogColor);
			
			glFogf(GL_FOG_DENSITY, 0.5f);
			glHint(GL_FOG_HINT, GL_DONT_CARE);
			glFogf(GL_FOG_START, 5.0f);
			glFogf(GL_FOG_END, 20.0f);
			
			glEnable(GL_FOG);
		}
	}

	void ResizeView(unsigned int w, unsigned int h)
	{
		projection = glm::perspective(60.0f, float(w)/float(h), 0.01f, 1000.0f);
	}

	void G_ResizeView(unsigned int w, unsigned int h)
	{
		width = w;
		height = h;

		ResizeView(w, h);
		
		//cartesian coordinate system
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);		
		glLoadMatrixf( &projection[0][0] );
	}

	void G_ResizeView()
	{
		G_ResizeView(width, height);
	}

	void G_SetModelView()
	{
		glm::mat4 modelviewMatrix = ViewMatrix();

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf( &modelviewMatrix[0][0] );
	}
};