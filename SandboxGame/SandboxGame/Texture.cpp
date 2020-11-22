#include "Texture.h"
#include <cstdio>
#include <memory>

Texture::Texture(GLsizei Width, GLsizei Height, const char* FileName)
	: Width(Width), Height(Height), FileName(FileName)
{}

void Texture::LoadTexture(GLuint& Target)
{
	std::unique_ptr<unsigned char[]> Data(new unsigned char[Width * Height * 3]);
	FILE* Stream;

	fopen_s(&Stream, FileName, "rb");
	if (!Stream) return;

	fread(Data.get(), Width * Height * 3, 1, Stream);
	fclose(Stream);
	glGenTextures(1, &Target);
	glBindTexture(GL_TEXTURE_2D, Target);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_BGR, GL_UNSIGNED_BYTE, Data.get());
}
