#pragma once

#include <GL/glew.h>
#include <GL/glut.h>

class Texture
{
public:
	Texture() = default;
	Texture(GLsizei Width, GLsizei Height, const char* FileName);

	void LoadTexture(GLuint& Target);

private:
	GLsizei Width;
	GLsizei Height;
	const char* FileName;
};

