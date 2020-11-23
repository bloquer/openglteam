#include "Box.h"

Box::Box()
	: X(0.0), Y(0.0), Z(0.0), BoxTexture(0)
{}

Box::Box(GLfloat X, GLfloat Y, GLfloat Z, GLuint BoxTexture)
	: X(X), Y(Y), Z(Z), BoxTexture(BoxTexture)
{}

void Box::Generate(GLdouble Size)
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
