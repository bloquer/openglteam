#pragma once

#include "Vector.h"

class Camera
{
public:

	Camera() = default;
	Camera(double EyeX, double EyeY, double EyeZ,
		double AtX, double AtY, double AtZ,
		double UpX, double UpY, double UpZ);

	void MoveForward(double Speed);
	void MoveRight(double Speed);

	void LookRight(double Angle);
	void LookUp(double Angle);

	Vector Eye;
	Vector At;
	Vector Up;
};

