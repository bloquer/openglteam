#include <iostream> 
#include <gl/glut.h>
#include <math.h> 
using namespace std;

#define WIDTH 1920 
#define HEIGHT 1080 
#define PI 3.14159265
void DrawScene();
void ResetViewport();
void display();
void ItemDisplay();
void maindis();
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void Mouse(int x, int y);
void moveCamera();
void init();
void mkList();
void enableLight();
//double z(const double &x, const double &y); 

GLuint MainWindow;

GLuint idList = 0;

bool run_state = false;
double speed = 2.0;

double eye_x = 0.0;
double eye_y = 1.0;
double eye_z = 0.0;

double at_x = 0.0;
double at_y = 0.0;
double at_z = 0.0;

double initAt_x = 0.0;
double initAt_y = 0.0;
double initAt_z = 0.0;

double rl_angle = 0.0;
double ud_angle = 0.0;

double r = 300;
double phi = 0;
double updown = 0;
double theta = 1.5;
int mouse_x = 0;
int mouse_y = 0;

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(0, 0);
	MainWindow = glutCreateWindow("3d game");
	glutDisplayFunc(display);
	glutMotionFunc(Mouse);
	glutKeyboardFunc(keyboard);

	glutReshapeFunc(reshape);

	glEnable(GL_DEPTH_TEST);
	init();

	enableLight();

	glutMainLoop();
	return 0;
}

void maindis()
{
	glClearColor(0, 0, 0, 1);
	glutSwapBuffers();
}

void ItemDisplay()
{
	ResetViewport();
	glClearColor(0, 0, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);
	glPushMatrix();
	gluLookAt(1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	DrawScene();
	glPopMatrix();
	glutSwapBuffers();
}

void display()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mkList();
	moveCamera();

	glCallList(idList);

	GLfloat pos[] = { 0, 0, 0, 1 };
	glLightfv(GL_LIGHT0, GL_POSITION, pos);

	glFlush();
	glutSwapBuffers();
	glutCreateSubWindow(MainWindow, 0, 0, WIDTH, HEIGHT*0.1);
	glutDisplayFunc(ItemDisplay);
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 2, 100, 2000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
#define ESCAPE '\033' 

	double rl_radian;
	double ud_radian;
	double fbmove_radian = rl_angle * PI / 180;
	double rlmove_radian = (rl_angle + 90) * PI / 180;

	switch (key)
	{
	case ESCAPE:
		exit(0);
		break;
	case 'q':
		rl_angle += 5;
		break;
	case 'e':
		rl_angle -= 5;
		break;
	case 'w':
		eye_x += speed * cos(fbmove_radian);
		eye_z -= speed * sin(fbmove_radian);
		break;
	case 's':
		eye_x -= speed * cos(fbmove_radian);
		eye_z += speed * sin(fbmove_radian);
		break;
	case 'y':
		ud_angle += 5;
		break;
	case 'h':
		ud_angle -= 5;
		break;
	case 'a':
		eye_x += speed * cos(rlmove_radian);
		eye_z -= speed * sin(rlmove_radian);
		break;
	case 'd':
		eye_x -= speed * cos(rlmove_radian);
		eye_z += speed * sin(rlmove_radian);
		break;
	case 'b':
		if (run_state == false)
		{
			run_state = true;
			speed = 10.0;
		}
		else
		{
			run_state = false;
			speed = 2.0;
		}
	default:
		break;
	}
	if (ud_angle >= 40)
		ud_angle = 40;
	else if (ud_angle <= -25)
		ud_angle = -25;

	rl_radian = rl_angle * PI / 180;
	ud_radian = ud_angle * PI / 180;

	if (key != 'a' && key != 'd' && key != 'w' && key != 's')
	{
		at_x = (initAt_x * cos(ud_radian)) - (initAt_y * sin(ud_radian));
		at_y = (initAt_y * cos(ud_radian)) + (initAt_x * sin(ud_radian));

		at_x = (initAt_x * cos(rl_radian)) + (initAt_z * sin(rl_radian));
		at_z = (initAt_z * cos(rl_radian)) - (initAt_x * sin(rl_radian));
	}
	else if (key == 'a')
	{
		at_x += speed * cos(rlmove_radian);
		at_z -= speed * sin(rlmove_radian);
	}
	else if (key == 'd')
	{
		at_x -= speed * cos(rlmove_radian);
		at_z += speed * sin(rlmove_radian);
	}
	else if (key == 'w')
	{
		at_x += speed * cos(fbmove_radian);
		at_z -= speed * sin(fbmove_radian);
	}
	else if (key == 's')
	{
		at_x -= speed * cos(fbmove_radian);
		at_z += speed * sin(fbmove_radian);
	}


	glutPostRedisplay();
}

void Mouse(int x, int y)
{
	if (mouse_y < y)
	{
		updown += 0.05;
		mouse_y = y;
	}
	else if (mouse_y > y)
	{
		updown -= 0.05;
		mouse_y = y;
	}
	glutPostRedisplay();
}

void moveCamera()
{
	glLoadIdentity();
	int vec = ceil(theta / 3.1415);
	int y;
	if (vec % 2)
		y = 1;
	else
		y = -1;
	gluLookAt(eye_x, eye_y, eye_z, at_x, at_y, at_z, 0, y, 0);
}

void init()
{
	initAt_x = r * cos(0.0);
	initAt_y = 0.0;
	initAt_z = r * sin(0.0);

	at_x = initAt_x;
	at_y = initAt_y;
	at_z = initAt_z;
}

void mkList()
{
	int idInnerList = glGenLists(1);
	glNewList(idInnerList, GL_COMPILE);



	glColor3ub(255, 0, 0);
	glPushMatrix();
	glTranslatef(400, -75, 0);
	glutSolidCube(50);
	glPopMatrix();

	glEndList();

	idList = glGenLists(1);
	glNewList(idList, GL_COMPILE);
	glCallList(idInnerList);

	glPushMatrix();
	glTranslatef(0, 0, -200);
	glCallList(idInnerList);
	glPopMatrix();
	glEndList();

	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			glColor3ub(0, 255, 0);
			glPushMatrix();
			glTranslatef(i * 100 - 600, -150, -600);
			glTranslatef(-600, 0, j * 100 - 600);
			glutSolidCube(100);
			glTranslatef((i * -100) - 600, 150, (j * -100) - 600);
			glPopMatrix();
		}
	}
}

void enableLight()
{
	glEnable(GL_LIGHTING);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	GLfloat diffuse[] = { 0.7, 0.7, 0.7, 1 };

	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	glEnable(GL_LIGHT0);
}
void ResetViewport()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2.0, 2.0, -2.0, 2.0, 0.5, 5.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void DrawScene()
{

	glColor3f(0.7, 0.7, 0.7);
	glPushMatrix();
	//glTranslatef(0.0, -1.0, 0.0);

	glBegin(GL_QUADS);
	glVertex3f(2.0, 0.0, 2.0);
	glVertex3f(2.0, 0.0, -2.0);
	glVertex3f(-2.0, 0.0, -2.0);
	glVertex3f(-2.0, 0.0, 2.0);
	glEnd();

	glPopMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	glTranslatef(0.0, 0.0, -0.5);
	glutWireTeapot(1.0);
	glPopMatrix();

}