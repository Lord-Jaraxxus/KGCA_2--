#include "k_Vector.h"

k_Vector::k_Vector() 
{
	x = y = z = 0.0f;
}

k_Vector::k_Vector(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}
//k_Vector::k_Vector(k_Vector& v) {}

void k_Vector::Set(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}


k_Vector	k_Vector::operator + (k_Vector& v) 
{
	return k_Vector(x+v.x, y+v.y, z+v.z);
}

k_Vector	k_Vector::operator += (k_Vector& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

k_Vector	k_Vector::operator - (k_Vector& v) 
{
	return k_Vector(x-v.x, y-v.y, z-v.z);
}

k_Vector	k_Vector::operator * (float scala) 
{
	return k_Vector(x*scala, y*scala, z*scala);
}

k_Vector	k_Vector::operator / (float scala)
{
	return k_Vector(x/scala, y/scala, z/scala);
}

k_Vector k_Vector::operator *= (float scala) // ¾êµµ ÀÏ´Ü
{
	x *= scala;
	y *= scala;
	z *= scala;
	return *this;
}

bool k_Vector::operator == (k_Vector& v) {
	if (fabs(x - v.x) < k_Epsilon)
	{
		if (fabs(y - v.y) < k_Epsilon)
		{
			if (fabs(z - v.z) < k_Epsilon) 
			{
				return true;
			}
		}
	}
	return false;
}

bool k_Vector::operator <= (k_Vector& v) {
	if (x <= v.x)
	{
		if (y <= v.y)
		{
			if (z <= v.z)
			{
				return true;
			}
		}
	}
	return false;
}

bool k_Vector::operator >= (k_Vector& v) {
	if (x >= v.x)
	{
		if (y >= v.y)
		{
			if (z >= v.z)
			{
				return true;
			}
		}
	}
	return false;
}


float		k_Vector::LengthSquared() 
{
	return (x*x + y*y + z*z);
}

float		k_Vector::Length() 
{
	return sqrt(LengthSquared());
}

void		k_Vector::Normalize() 
{
	float fInvertLength = 1.0f / Length();
	x = x * fInvertLength;
	y = y * fInvertLength;
	z = z * fInvertLength;
}

k_Vector	k_Vector::Identity() 
{
	float fInvertLength = 1.0f / Length();
	return k_Vector(x*fInvertLength, y*fInvertLength, z*fInvertLength);
}

//float		k_Vector::Angle(k_Vector) {}