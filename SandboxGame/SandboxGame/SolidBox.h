#pragma once

#include "Box.h"

class SolidBox : public Box
{
public:
	SolidBox() = default;
	SolidBox(GLuint BoxTexture);
	SolidBox(GLfloat X, GLfloat Y, GLfloat Z, GLuint BoxTexture);

	void Generate(GLdouble Size) override;

private:
	GLuint BoxTexture;
};

