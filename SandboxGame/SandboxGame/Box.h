#pragma once

#include <GL/glew.h>
#include <GL/glut.h>

class Box
{
public:
	Box();
	Box(GLfloat X, GLfloat Y, GLfloat Z, GLuint BoxTexture);

	void Generate(GLdouble Size);

private:
	GLfloat X;
	GLfloat Y;
	GLfloat Z;
	GLuint BoxTexture;
};

