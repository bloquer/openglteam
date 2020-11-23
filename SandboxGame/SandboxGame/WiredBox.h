#pragma once

#include "Box.h"

class WiredBox : public Box
{
public:
	WiredBox(GLfloat Width);
	WiredBox(GLfloat X, GLfloat Y, GLfloat Z, GLfloat Width);

	void Generate(GLdouble Size) override;

private:
	GLfloat Width;
};

