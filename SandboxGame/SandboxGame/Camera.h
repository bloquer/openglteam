#pragma once

#include "Vector.h"

class Camera
{
public:

	Camera() = default;
	Camera(double EyeX, double EyeY, double EyeZ,
		double AtX, double AtY, double AtZ,
		double UpX, double UpY, double UpZ);

	void MoveForward(float Speed);
	void MoveRight(float Speed);

	void LookRight(float Angle);
	void LookUp(float Angle);

	Vector Eye;
	Vector At;
	Vector Up;
};

