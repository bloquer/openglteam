#pragma once

#include <GL/glew.h>
#include <GL/glut.h>

class Box
{
public:
	Box() : X(0.0), Y(0.0), Z(0.0) {}
	Box(GLfloat X, GLfloat Y, GLfloat Z) : X(X), Y(Y), Z(Z) {}

	virtual void Generate(GLdouble Size) = 0;

protected:
	GLfloat X;
	GLfloat Y;
	GLfloat Z;
};

