#pragma once

class Vector
{
public:
	Vector() : x(0.0), y(0.0), z(0.0) {}
	Vector(double x, double y, double z)
		: x(x), y(y), z(z)
	{}

	Vector operator-(const Vector& End)
	{
		return Vector(x - End.x, y - End.y, z - End.z);
	}

	double x;
	double y;
	double z;
};