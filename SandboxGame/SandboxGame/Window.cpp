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
