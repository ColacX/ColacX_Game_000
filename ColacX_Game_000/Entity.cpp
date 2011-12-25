#include "Entity.h"

#include <math.h>

Entity::Entity()
{
	px = 0;
	py = 0;
	pz = 0;

	rm[ 0] = 1; rm[ 4] = 0; rm[ 8] = 0; rm[12] = 0;
	rm[ 1] = 0; rm[ 5] = 1; rm[ 9] = 0; rm[13] = 0;
	rm[ 2] = 0; rm[ 6] = 0; rm[10] = 1; rm[14] = 0;
	rm[ 3] = 0; rm[ 7] = 0; rm[11] = 0; rm[15] = 1;
}

void Entity::MoveXGlobal( float value )
{
	px += value;
}

void Entity::MoveYGlobal( float value )
{
	py += value;
}

void Entity::MoveZGlobal( float value )
{
	pz += value;
}

void Entity::MoveXLocal( float value )
{
	px += rm[ 0] * value;
	py += rm[ 1] * value;
	pz += rm[ 2] * value;
}

void Entity::MoveYLocal( float value )
{
	px += rm[ 4]*value;
	py += rm[ 5]*value;
	pz += rm[ 6]*value;

}

void Entity::MoveZLocal( float value )
{
	px += rm[ 8]*value;
	py += rm[ 9]*value;
	pz += rm[10]*value;
}

void Entity::RotateXLocal( float value )
{
	float c = cos( value );
	float s = sin( value );

	//rotate matrix
	rm[ 4] = rm[ 4]*(+c) + rm[ 8]*(+s);	 rm[ 8] = rm[ 4]*(-s) + rm[ 8]*(+c);
	rm[ 5] = rm[ 5]*(+c) + rm[ 9]*(+s);	 rm[ 9] = rm[ 5]*(-s) + rm[ 9]*(+c);
	rm[ 6] = rm[ 6]*(+c) + rm[10]*(+s);	 rm[10] = rm[ 6]*(-s) + rm[10]*(+c);

	//normalize matrix
	float nx = sqrtf( rm[ 0]*rm[ 0] + rm[ 1]*rm[ 1] + rm[ 2]*rm[ 2] );
	float ny = sqrtf( rm[ 4]*rm[ 4] + rm[ 5]*rm[ 5] + rm[ 6]*rm[ 6] );
	float nz = sqrtf( rm[ 8]*rm[ 8] + rm[ 9]*rm[ 9] + rm[10]*rm[10] );
	
	rm[ 0] /= nx; rm[ 1] /= nx; rm[ 2] /= nx;
	rm[ 4] /= ny; rm[ 5] /= ny; rm[ 6] /= ny;
	rm[ 8] /= nz; rm[ 9] /= nz; rm[10] /= nz;
}

void Entity::RotateYLocal( float value )
{
	float c = cos( value );
	float s = sin( value );

	//rotate matrix
	rm[ 0] = rm[ 0]*(+c) + rm[ 8]*(-s); rm[ 8] = rm[ 0]*(+s) + rm[ 8]*(+c);
	rm[ 1] = rm[ 1]*(+c) + rm[ 9]*(-s); rm[ 9] = rm[ 1]*(+s) + rm[ 9]*(+c);
	rm[ 2] = rm[ 2]*(+c) + rm[10]*(-s); rm[10] = rm[ 2]*(+s) + rm[10]*(+c);

	//normalize matrix
	float nx = sqrt(rm[ 0]*rm[ 0] + rm[ 1]*rm[ 1] + rm[ 2]*rm[ 2]);
	float ny = sqrt(rm[ 4]*rm[ 4] + rm[ 5]*rm[ 5] + rm[ 6]*rm[ 6]);
	float nz = sqrt(rm[ 8]*rm[ 8] + rm[ 9]*rm[ 9] + rm[10]*rm[10]);
	
	rm[ 0] /= nx; rm[ 1] /= nx; rm[ 2] /= nx;
	rm[ 4] /= ny; rm[ 5] /= ny; rm[ 6] /= ny;
	rm[ 8] /= nz; rm[ 9] /= nz; rm[10] /= nz;
}

void Entity::RotateZLocal( float value )
{
	float c = cos( value );
	float s = sin( value );

	//rotate matrix
	rm[ 0] = rm[ 0]*(+c) + rm[ 4]*(+s);	 rm[ 4] = rm[ 0]*(-s) + rm[ 4]*(+c);
	rm[ 1] = rm[ 1]*(+c) + rm[ 5]*(+s);	 rm[ 5] = rm[ 1]*(-s) + rm[ 5]*(+c);
	rm[ 2] = rm[ 2]*(+c) + rm[ 6]*(+s);	 rm[ 6] = rm[ 2]*(-s) + rm[ 6]*(+c);

	//normalize matrix
	float nx = sqrt(rm[ 0]*rm[ 0] + rm[ 1]*rm[ 1] + rm[ 2]*rm[ 2]);
	float ny = sqrt(rm[ 4]*rm[ 4] + rm[ 5]*rm[ 5] + rm[ 6]*rm[ 6]);
	float nz = sqrt(rm[ 8]*rm[ 8] + rm[ 9]*rm[ 9] + rm[10]*rm[10]);
	
	rm[ 0] /= nx; rm[ 1] /= nx; rm[ 2] /= nx;
	rm[ 4] /= ny; rm[ 5] /= ny; rm[ 6] /= ny;
	rm[ 8] /= nz; rm[ 9] /= nz; rm[10] /= nz;
}

float Entity::X() const
{
	return px;
}

void Entity::X( float value )
{
	px = value;
}

float Entity::Y() const
{
	return py;
}

void Entity::Y( float value )
{
	py = value;
}

float Entity::Z() const
{
	return pz;
}

void Entity::Z( float value )
{
	pz = value;
}

const float* Entity::Rotation()
{
	return rm;
}

void Entity::Rotation( const float* matrix )
{
	for( int i = 0; i < 16; i++ )
	{
		rm[i] = matrix[i];
	}
}
