#include "Light.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <cmath>

Light::Light()
{
	Pos = new float[] { 200.0, 200.0, 200.0, 1.0 };
	Ambient = new float[] { 0.05, 0.05, 0.05, 0.0 };
	Diffuse = new float[] { 1.0, 1.0, 1.0, 0.0 };
}

Light::~Light()
{
	delete[] Pos;
	delete[] Ambient;
	delete[] Diffuse;
}

void Light::Generate()
{
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, Pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, Ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Diffuse);
	glEnable(GL_LIGHT0);
}
