#include "WiredBox.h"

WiredBox::WiredBox(GLfloat Width)
	: Box(), Width(Width)
{}

WiredBox::WiredBox(GLfloat X, GLfloat Y, GLfloat Z, GLfloat Width)
	: Box(X, Y, Z), Width(Width)
{}

void WiredBox::Update(GLfloat NX, GLfloat NY, GLfloat NZ)
{
	X = NX;
	Y = NY;
	Z = NZ;
}

void WiredBox::Generate(GLdouble Size)
{
	glPushMatrix();
	glLineWidth(Width);
	glColor3ub(255, 255, 51);
	glTranslatef(X, Y, Z);
	glutWireCube(Size);
	glPopMatrix();
}
