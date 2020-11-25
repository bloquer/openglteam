#pragma once

#include "Box.h"

class WiredBox : public Box
{
public:
	WiredBox() = default;
	WiredBox(GLfloat Width);
	WiredBox(GLfloat X, GLfloat Y, GLfloat Z, GLfloat Width);

	void Update(GLfloat NX, GLfloat NY, GLfloat NZ);
	void Generate(GLdouble Size) override;

private:
	GLfloat Width;
};

