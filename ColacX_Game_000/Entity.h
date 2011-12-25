#pragma once

class Entity
{
protected:
	float px, py, pz;
	float rm[16];
public:
	Entity();
	virtual void MoveXGlobal( float value );
	virtual void MoveYGlobal( float value );
	virtual void MoveZGlobal( float value );
	virtual void MoveXLocal( float value );
	virtual void MoveYLocal( float value );
	virtual void MoveZLocal( float value );
	virtual void RotateXLocal( float value );
	virtual void RotateYLocal( float value );
	virtual void RotateZLocal( float value );
	virtual float X() const;
	virtual void X( float value );
	virtual float Y() const;
	virtual void Y( float value );
	virtual float Z() const;
	virtual void Z( float value );
	virtual const float* Rotation();
	virtual void Rotation( const float* matrix );
};
