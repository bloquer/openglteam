#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <gl/glut.h>
#include <math.h> 
#include <stdlib.h>
#include <io.h>
using namespace std;

#define WIDTH 1920 
#define HEIGHT 1080 
#define PI 3.14159265
#define ITEMMAX 256

void DrawScene();
void ResetViewport();
void display();
void ItemDisplay();
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void Mouse(int x, int y);
void moveCamera();
void init();
void mkList();
void enableLight();
void CreateObject(int x, int y, int z);
GLuint CreateTexture(char const* filename);

// 아이템 0~9번
void SelectItem0();
void SelectItem1();
void SelectItem2();
void SelectItem3();
void SelectItem4();
void SelectItem5();
void SelectItem6();
void SelectItem7();
//double z(const double &x, const double &y); 

typedef struct cube
{
	bool exist = false;
	int id;
	double x;
	double y;
	double z;
}cube;

GLuint MainWindow;

GLuint idList = 0;

bool run_state = false;
bool eyetop = false;
double speed = 2.0;

double eye_x = 0.0;
double eye_y = 10.0;
double eye_z = 0.0;

double at_x = 0.0;
double at_y = 0.0;
double at_z = 0.0;

double initAt_x = 0.0;
double initAt_y = 0.0;
double initAt_z = 0.0;

double rl_angle = 0.0;
double ud_angle = 0.0;

double r = 10;
double updown = 0;
double theta = 1.5;
int mouse_x = 0;
int mouse_y = 0;

cube map_item[ITEMMAX];
int item_id[ITEMMAX] = { 0, };
int itemcount = 0;
int id = 0;
GLuint texture; //the array for our texture

int framebufferWidth, framebufferHeight;
GLuint triangleVertexArrayObject;
GLuint triangleShaderProgramID;
GLuint trianglePositionVertexBufferObjectID, triangleColorVertexBufferObjectID;
GLuint triangleTextureCoordinateBufferObjectID;

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
		GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_NEAREST);


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

void SelectItem0()
{
	glColor3ub(102, 0, 0);
}

void CreateObject(int x, int y, int z)
{
	glPushMatrix();
	glBegin(GL_POLYGON);
	glColor3f(1.0, 0.502, 0.502);
	glVertex3f(-1.0, -0.9, 0.6);
	glVertex3f(-1.0, -0.9, -0.6);
	glVertex3f(-1.0, -2.0, -0.6);
	glVertex3f(-1.0, -2.0, 0.6);
	glEnd();
	glPopMatrix();
}

void ItemDisplay()
{
	ResetViewport();
	glClearColor(0, 0, 0, 0);
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
	glFrustum(-20, 20, 5, 25, 100, 200);
	// gluPerspective(45, 2, 100, 200);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void SetHighlight()
{

}

void keyboard(unsigned char key, int x, int y)
{
#define ESCAPE '\033' 

	double rl_radian;
	double ud_radian;
	double fbmove_radian = rl_angle * PI / 180;
	double rlmove_radian = (rl_angle + 90) * PI / 180;
	bool exist = false;
	cube temp;

	switch (key)
	{
	case ESCAPE:
		exit(0);
		break;
	case 'j':
		rl_angle += 5;
		break;
	case 'l':
		rl_angle -= 5;
		break;
	case 'w':
		eye_x += speed * cos(fbmove_radian);
		eye_z -= speed * sin(fbmove_radian);
		break;
	case 's':
		eye_x -= 2.0 * cos(fbmove_radian);
		eye_z += 2.0 * sin(fbmove_radian);
		break;
	case 'i':
		ud_angle += 5;
		break;
	case 'k':
		ud_angle -= 5;
		break;
	case 'a':
		eye_x += 2.0 * cos(rlmove_radian);
		eye_z -= 2.0 * sin(rlmove_radian);
		break;
	case 'd':
		eye_x -= 2.0 * cos(rlmove_radian);
		eye_z += 2.0 * sin(rlmove_radian);
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
		break;
	case 'h':
		// 시점에 대해 큐브 중점의 x, z 결정
		map_item[itemcount].x = ((int)at_x / 10) * 10 + 5;
		map_item[itemcount].z = ((int)at_z / 10) * 10 + 5;
		// 첫번째 아이템에 대한 작업
		if (itemcount == 0)
		{
			map_item[itemcount].y = -40;
			map_item[itemcount].exist = true;
			map_item[itemcount].id = itemcount;
			// 아이템 개수 증가(id 사용)
			item_id[itemcount] += 1;
			itemcount++;
		}
		else
		{
			// 현재 아이템과 기존 아이템들을 비교
			for (int i = 0; i < itemcount; i++)
			{
				// 현재 아이템과 x, z 값이 같은 아이템이 존재하면
				if (map_item[itemcount].x == map_item[i].x && map_item[itemcount].z == map_item[i].z)
				{
					// 해당 y축을 아이템 갯수만큼 증가
					map_item[itemcount].y = item_id[i] * 10 - 40;
					map_item[itemcount].id = i;
					map_item[itemcount].exist = true;
					// 개수 증가
					item_id[i] += 1;
					itemcount++;
					exist = true;
					break;
				}
			}
			// 동일 아이템이 발견 후 y를 계산한 경우 이 과정 패스
			if (exist == false)
			{
				id++;
				map_item[itemcount].y = -40;
				map_item[itemcount].id = id;
				map_item[itemcount].exist = true;
				itemcount++;
			}
		}
		break;
	case 'r':
		temp.x = ((int)at_x / 10) * 10 + 5;
		temp.z = ((int)at_z / 10) * 10 + 5;
		for (int i = 0; i <= itemcount; i++)
		{
			if (temp.x == map_item[i].x && temp.z == map_item[i].z)
			{
				map_item[i].exist = false;
				item_id[map_item[i].id] -= 1;
				itemcount--;
				break;
			}
		}
		break;
	default:
		break;
	}
	if (ud_angle >= 40)
		ud_angle = 40;
	else if (ud_angle <= -15)
		ud_angle = -15;

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
		at_x += 2.0 * cos(rlmove_radian);
		at_z -= 2.0 * sin(rlmove_radian);
	}
	else if (key == 'd')
	{
		at_x -= 2.0 * cos(rlmove_radian);
		at_z += 2.0 * sin(rlmove_radian);
	}
	else if (key == 'w')
	{
		at_x += speed * cos(fbmove_radian);
		at_z -= speed * sin(fbmove_radian);
	}
	else if (key == 's')
	{
		at_x -= 2.0 * cos(fbmove_radian);
		at_z += 2.0 * sin(fbmove_radian);
	}
	if (run_state == true && key == 'w')
	{
		if (eyetop == false)
		{
			eye_y += 1.0;
			if (eye_y >= 19.0)
				eyetop = true;
		}
		else
		{
			eye_y -= 1.0;
			if (eye_y <= 10.0)
				eyetop = false;
		}
	}
	else if (run_state == true)
	{
		eye_y = 10.0;
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
	initAt_y = 10.0;
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
	glTranslatef(150, 0, 0);
	glutSolidCube(10);
	glPopMatrix();

	glEndList();

	idList = glGenLists(1);
	glNewList(idList, GL_COMPILE);
	glCallList(idInnerList);

	glPushMatrix();
	glTranslatef(0, 0, -50);
	glCallList(idInnerList);
	glPopMatrix();
	glEndList();

	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			glColor3ub(0, 255, 0);
			glPushMatrix();
			glTranslatef(i * 10 - 100, -5, -100);
			glTranslatef(-100, 0, j * 10 - 100);
			glutSolidCube(10);
			glTranslatef((i * -10) - 100, 5, (j * -10) - 200);
			glPopMatrix();
		}
	}

	for (int j = 0; j < itemcount; j++)
	{
		if (map_item[j].exist)
		{
			glPushMatrix();
			glColor3ub(102, 0, 0);
			glTranslatef(map_item[j].x, map_item[j].y, map_item[j].z);
			glutSolidCube(50);
			glTranslatef(-1 * map_item[j].x, -1 * map_item[j].y, -1 * map_item[j].z);
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
	glPushMatrix();
	glBegin(GL_POLYGON);
	glColor3ub(255, 128, 128);
	glVertex3f(-1.0, -0.9, 0.6);
	glVertex3f(-1.0, -0.9, -0.6);
	glVertex3f(-1.0, -2.0, -0.6);
	glVertex3f(-1.0, -2.0, 0.6);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, 1.6);
	glBegin(GL_POLYGON);
	glColor3ub(255, 255, 255);
	glVertex3f(-1.0, 2.0, 1.0);
	glVertex3f(-1.0, 2.0, -1.0);
	glVertex3f(-1.0, -2.0, -1.0);
	glVertex3f(-1.0, -2.0, 1.0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, -1.6);
	glBegin(GL_POLYGON);
	glVertex3f(-1.0, 2.0, 1.0);
	glVertex3f(-1.0, 2.0, -1.0);
	glVertex3f(-1.0, -2.0, -1.0);
	glVertex3f(-1.0, -2.0, 1.0);
	glEnd();
	glPopMatrix();
}