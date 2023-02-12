#include "k_Vector.h"

k_Vector2D::k_Vector2D() 
{
	x = y = 0.0f;
}

k_Vector2D::k_Vector2D(float x, float y) 
{
	this->x = x;
	this->y = y;
}
//k_Vector2D::k_Vector2D(k_Vector2D& v) {}


k_Vector2D	k_Vector2D::operator + (k_Vector2D& v) 
{
	return k_Vector2D(x + v.x, y + v.y);
}

k_Vector2D	k_Vector2D::operator += (k_Vector2D& v)
{
	x += v.x;
	y += v.y;
	return *this;
}

k_Vector2D	k_Vector2D::operator - (k_Vector2D& v) 
{
	return k_Vector2D(x - v.x, y - v.y);
}

k_Vector2D	k_Vector2D::operator * (float scala) 
{
	return k_Vector2D(x * scala, y * scala);
}

k_Vector2D	k_Vector2D::operator / (float scala)
{
	return k_Vector2D(x / scala, y / scala);
}

k_Vector2D k_Vector2D::operator *= (float scala) // ¾êµµ ÀÏ´Ü
{
	x *= scala;
	y *= scala;
	return *this;
}

bool k_Vector2D::operator == (k_Vector2D& v) {
	if (fabs(x - v.x) < k_Epsilon)
	{
		if (fabs(y - v.y) < k_Epsilon)
		{
			return true;
		}
	}
	return false;
}


float		k_Vector2D::LengthSquared() 
{
	return (x*x + y*y);
}

float		k_Vector2D::Length() 
{
	return sqrt(LengthSquared());
}

void		k_Vector2D::Normalize() 
{
	float fInvertLength = 1.0f / Length();
	x = x * fInvertLength;
	y = y * fInvertLength;
}

k_Vector2D	k_Vector2D::Identity() 
{
	float fInvertLength = 1.0f / Length();
	return k_Vector2D(x* fInvertLength, y* fInvertLength);
}

//float		k_Vector2D::Angle(k_Vector2D) {}