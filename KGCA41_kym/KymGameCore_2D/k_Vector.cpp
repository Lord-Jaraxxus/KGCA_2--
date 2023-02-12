#include "k_Vector.h"
#include "K_Matrix.h"

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

// 곱셈 연산자 재정의, 행렬용
k_Vector k_Vector::operator * (K_Matrix mat)
{
	k_Vector result;

	result.x = x * mat._11 + y * mat._21 + z * mat._31 + 1.0f * mat._41;
	result.y = x * mat._12 + y * mat._22 + z * mat._32 + 1.0f * mat._42;
	result.z = x * mat._13 + y * mat._23 + z * mat._33 + 1.0f * mat._43;

	return result;
}

k_Vector	k_Vector::operator / (float scala)
{
	return k_Vector(x/scala, y/scala, z/scala);
}

k_Vector k_Vector::operator *= (float scala) // 얘도 일단
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

// 내적
float k_Vector::operator|(k_Vector& const v0)
{
	return x * v0.x + y * v0.y + z * v0.z;
}

// 외적
k_Vector k_Vector::operator^(k_Vector& const v0)
{
	return k_Vector((y * v0.z - z * v0.y), (z * v0.x - x * v0.z), (x * v0.y - y * v0.x));
}


float		k_Vector::LengthSquared() 
{
	return (x*x + y*y + z*z);
}

float		k_Vector::Length() 
{
	return sqrt(LengthSquared());
}

// 스스로를 정규화
void		k_Vector::Normalize() 
{
	float fInvertLength = 1.0f / Length();
	x = x * fInvertLength;
	y = y * fInvertLength;
	z = z * fInvertLength;
}

// 스스로를 정규화한 벡터(단위벡터)를 반환 (지는 안바뀜)
k_Vector	k_Vector::Normal()
{
	float fInvertLength = 1.0f / Length();
	return k_Vector(x*fInvertLength, y*fInvertLength, z*fInvertLength);
}

//float		k_Vector::Angle(k_Vector) {}

k_Vector4D::k_Vector4D()
{
	x = y = z = w = 0.0f;
}

k_Vector4D::k_Vector4D(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}
//k_Vector4D::k_Vector4D(k_Vector4D& v) {}

void k_Vector4D::Set(float x, float y, float z, float w)
{ 
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}


k_Vector4D	k_Vector4D::operator + (k_Vector4D& v)
{
	return k_Vector4D(x+v.x, y+v.y, z+v.z, w+v.w);
}

k_Vector4D	k_Vector4D::operator += (k_Vector4D& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}

k_Vector4D	k_Vector4D::operator - (k_Vector4D& v)
{
	return k_Vector4D(x-v.x, y-v.y, z-v.z, w-v.w);
}

k_Vector4D	k_Vector4D::operator * (float scala)
{
	return k_Vector4D(x*scala, y*scala, z*scala, w*scala);
}

k_Vector4D	k_Vector4D::operator / (float scala)
{
	return k_Vector4D(x/scala, y/scala, z/scala, w/scala);
}

k_Vector4D k_Vector4D::operator *= (float scala) // 얘도 일단
{
	x *= scala;
	y *= scala;
	z *= scala;
	w *= scala;
	return *this;
}

bool k_Vector4D::operator == (k_Vector4D& v) {
	if (fabs(x - v.x) < k_Epsilon)
	{
		if (fabs(y - v.y) < k_Epsilon)
		{
			if (fabs(z - v.z) < k_Epsilon)
			{
				if (fabs(w - v.w) < k_Epsilon)
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool k_Vector4D::operator <= (k_Vector4D& v) {
	if (x <= v.x)
	{
		if (y <= v.y)
		{
			if (z <= v.z)
			{
				if (w <= v.w)
				{
					return true;
				}
			}
		}
	}
	return false;
}

bool k_Vector4D::operator >= (k_Vector4D& v) {
	if (x >= v.x)
	{
		if (y >= v.y)
		{
			if (z >= v.z)
			{
				if (w >= v.w)
				{
					return true;
				}
			}
		}
	}
	return false;
}


float		k_Vector4D::LengthSquared()
{
	return (x*x + y*y + z*z + w*w);
}

float		k_Vector4D::Length()
{
	return sqrt(LengthSquared());
}

void		k_Vector4D::Normalize()
{
	float fInvertLength = 1.0f / Length();
	x = x * fInvertLength;
	y = y * fInvertLength;
	z = z * fInvertLength;
	w = w * fInvertLength;
}

k_Vector4D	k_Vector4D::Identity()
{
	float fInvertLength = 1.0f / Length();
	return k_Vector4D(x*fInvertLength, y*fInvertLength, z*fInvertLength, w*fInvertLength);
}