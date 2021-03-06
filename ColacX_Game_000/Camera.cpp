#include "Camera.h"

#include <Windows.h>
#include <gl\GL.h>

Camera::Camera()
{
}

void Camera::SetView()
{
	//reset to 0
	glLoadIdentity();

	//apply cameras inverse rotation
	float rotationInversion[16] = {
		rm[ 0], rm[ 4], rm[ 8], 0,
		rm[ 1], rm[ 5], rm[ 9], 0,
		rm[ 2], rm[ 6], rm[10], 0,
		0, 0, 0, 1
	};
	glMultMatrixf(rotationInversion);

	//apply cameras inverse position
	glTranslatef( -px, -py, -pz );
}
