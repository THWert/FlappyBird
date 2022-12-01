#pragma once

class Vector3
{
public:

	Vector3()
		:x(0), y(0), z(0) {}
	template<typename T, typename U, typename V>
	Vector3(T x, U y, V z)
	{
		this->x = (int)x;
		this->y = (int)y;
		this->z = (int)z;
	}
	Vector3 operator- (const Vector3& other)
	{
		Vector3 temp(x - other.x, y - other.y, z - other.z);
		return temp;
	}

	int x, y, z;
};