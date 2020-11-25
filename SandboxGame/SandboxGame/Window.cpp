#include <GL/glew.h>
#include <GL/glut.h>
#include "Window.h"

Window::Window(int Width, int Height)
	: Width(Width), Height(Height)
{
	glutInitWindowSize(Width, Height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(WindowName);
}

void Window::Reshaping(int ChangedWidth, int ChangedHeight)
{
	GLdouble ReWidth = (GLdouble)ChangedWidth / Width;
	GLdouble ReHieght = (GLdouble)ChangedHeight / Height;

	glViewport(0, 0, ChangedWidth, ChangedHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glTranslatef(0.0, -0.6, 0.0);
	gluPerspective(30, 1.8 * ReWidth / ReHieght, 2, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
