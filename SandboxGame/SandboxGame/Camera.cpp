#include "Camera.h"
#include <cmath>

Camera::Camera(double EyeX, double EyeY, double EyeZ,
	double AtX, double AtY, double AtZ,
	double UpX, double UpY, double UpZ)
	:
	Eye(Vector(EyeX, EyeY, EyeZ)),
	At(Vector(AtX, AtY, AtZ)),
	Up(Vector(UpX, UpY, UpZ))
{}

void Camera::MoveForward(double Speed)
{
	Vector ViewVector = At - Eye;

	Eye.x += ViewVector.x * Speed;
	Eye.z += ViewVector.z * Speed;
	At.x += ViewVector.x * Speed;
	At.z += ViewVector.z * Speed;
}

void Camera::MoveRight(double Speed)
{
	Vector ViewVector = At - Eye;

	double Temp = ViewVector.x;
	ViewVector.x = -ViewVector.z;
	ViewVector.z = Temp;

	Eye.x += ViewVector.x * Speed;
	Eye.z += ViewVector.z * Speed;
	At.x += ViewVector.x * Speed;
	At.z += ViewVector.z * Speed;
}

void Camera::LookRight(double Angle)
{
	Vector ViewVector = At - Eye;

	At.x = Eye.x + cos(Angle) * ViewVector.x - sin(Angle) * ViewVector.z;
	At.z = Eye.z + sin(Angle) * ViewVector.x + cos(Angle) * ViewVector.z;
}

void Camera::LookUp(double Angle)
{
	At.y += Angle;
}
