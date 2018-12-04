#include <GL/glut.h>
#include <windows.h>
#include <iostream>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>

GLuint texture; //the array for our texture
GLfloat angle = 0.0;
//function to load the RAW file

GLuint LoadTexture(const char * filename, int width, int
	height)
{
	GLuint texture;
	unsigned char * data;
	FILE * file;

	fopen_s(&file, filename, "rb");
	if (file == NULL) return 0;
	data = (unsigned char *)malloc(width * height * 3);
	fread(data, width * height * 3, 1, file);
	fclose(file);
	glGenTextures(1, &texture); 
	glBindTexture(GL_TEXTURE_2D, texture); 
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,
		GL_MODULATE); 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_LINEAR);


	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
		GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
		GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
		GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data);
	return texture;
}

void FreeTexture(GLuint texture)
{
	glDeleteTextures(1, &texture);
}


void display(void) {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glRotatef(45.0, 1, 0, 0);
	glRotatef(45.0, 0, 1, 0);
	glRotatef(45.0, 0, 0, 1);
	glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, texture);
	glutSolidCube(1);
	glDisable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
	glDisable(GL_TEXTURE_GEN_T);


	glutSwapBuffers();
	angle++;
}
void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("A basic OpenGL Window");
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutReshapeFunc(reshape);

	//Load our texture
	texture = LoadTexture("texture.bmp", 256, 256);

	glutMainLoop();

	//Free our texture
	FreeTexture(texture);

	return 0;
}