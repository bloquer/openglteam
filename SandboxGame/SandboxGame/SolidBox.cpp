#include "SolidBox.h"

SolidBox::SolidBox(GLuint BoxTexture)
	: Box(), BoxTexture(BoxTexture)
{}

SolidBox::SolidBox(GLfloat X, GLfloat Y, GLfloat Z, GLuint BoxTexture)
	: Box(X, Y, Z), BoxTexture(BoxTexture)
{}

void SolidBox::Generate(GLdouble Size)
{
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glTranslatef(X, Y, Z);
	glBindTexture(GL_TEXTURE_2D, BoxTexture); // ¹Ù´Ú ÅØ½ºÃÄ
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glutSolidCube(Size);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glPopMatrix();
}
