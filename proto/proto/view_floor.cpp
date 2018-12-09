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

GLuint LoadTexture(const char * filename, int width, int height, int i);
void FreeTexture(GLuint texture);
void Item_Load();
void Display();
void Main_Game();
void Main_Camera();
void Make_Floor();
void Set_Item();
void EnableLight();
void Highlight();
void Keyboard(unsigned char key, int x, int y);
void Reshape(int width, int height);
void Init_Camera();

// ---------------현재 미사용--------------
void DrawScene();
void ResetViewport();
void ItemDisplay();
void Mouse(int x, int y);
void CreateObject(int x, int y, int z);
// ---------------------------------------

//double z(const double &x, const double &y); 

typedef struct cube
{
	bool exist = false;
	int id;
	double x;
	double y;
	double z;
	int texture;
}cube;

GLuint texture[8]; //the array for our texture

GLuint MainWindow;
GLuint sub1;
GLuint sub2;

GLuint idList = 0;

bool run_state = false;
bool eyetop = false;
bool only_rotate;
double speed = 2.0;

double trans_x = 0.0;
double trans_y = 0.0;
double trans_z = 0.0;

double previous_trans_x = 0.0;
double previous_trans_y = 0.0;
double previous_trans_z = 0.0;

double eye_x = 0.0;
double eye_y = 3.0;
double eye_z = 0.0;

double at_x = 0.0;
double at_y = 0.0;
double at_z = 0.0;

double initAt_x = 0.0;
double initAt_y = 0.0;
double initAt_z = 0.0;

double previous_eye_x = 0.0;
double previous_eye_y = 0.0;
double previous_eye_z = 0.0;

double previous_at_x = 0.0;
double previous_at_y = 0.0;
double previous_at_z = 0.0;

double highlight_x = 0.0;
double highlight_y = 0.0;
double highlight_z = 0.0;

double rl_angle = 0.0;
double previous_rl_angle = 0.0;
double ud_angle = 0.0;

double r = 3;
double updown = 0;
double theta = 1.5;
int mouse_x = 0;
int mouse_y = 0;
int itemkey = 0;

cube map_item[ITEMMAX];
int item_id[ITEMMAX] = { 0, };
int itemcount = 0;
int id = 0;

// -------------------------------현재 미사용--------------------------------
int framebufferWidth, framebufferHeight;
GLuint triangleVertexArrayObject;
GLuint triangleShaderProgramID;
GLuint trianglePositionVertexBufferObjectID, triangleColorVertexBufferObjectID;
GLuint triangleTextureCoordinateBufferObjectID;
// -------------------------------------------------------------------------

GLuint LoadTexture(const char * filename, int width, int height, int i)
{
	unsigned char * data;
	FILE * file;

	fopen_s(&file, filename, "rb");
	if (file == NULL) return 0;
	data = (unsigned char *)malloc(width * height * 3);
	fread(data, width * height * 3, 1, file);
	fclose(file);
	glGenTextures(1, &texture[i]);
	glBindTexture(GL_TEXTURE_2D, texture[i]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
	free(data);
	return texture[i];
}

void FreeTexture(GLuint texture)
{
	glDeleteTextures(1, &texture);
}

void Item_Load()
{
	texture[0] = LoadTexture("1.bmp", 256, 256, 0);
	texture[1] = LoadTexture("2.bmp", 256, 256, 1);
	texture[2] = LoadTexture("3.bmp", 256, 256, 2);
	texture[3] = LoadTexture("4.bmp", 256, 256, 3);
	texture[4] = LoadTexture("5.bmp", 256, 256, 4);
	texture[5] = LoadTexture("6.bmp", 256, 256, 5);
	texture[6] = LoadTexture("7.bmp", 256, 256, 6);
	texture[7] = LoadTexture("8.bmp", 256, 256, 7);
}

void Display()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Main_Game();
	Main_Camera();

	glCallList(idList);

	GLfloat pos[] = { 0, 0, 0, 1 };
	glLightfv(GL_LIGHT0, GL_POSITION, pos);

	glFlush();
	glutSwapBuffers();
	/*glutCreateSubWindow(MainWindow, 0, 0, WIDTH, HEIGHT*0.1);
	glutDisplayFunc(ItemDisplay);*/
}

void Main_Game()
{
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glTranslatef(20, 1, 0);
	glBindTexture(GL_TEXTURE_2D, texture[0]); //<0>
	glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
	glEnable(GL_TEXTURE_GEN_T);
	glutSolidCube(2);
	glDisable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
	glDisable(GL_TEXTURE_GEN_T);
	glPopMatrix();

	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glTranslatef(0, 0, -25);
	glBindTexture(GL_TEXTURE_2D, texture[2]); //<2>
	glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
	glEnable(GL_TEXTURE_GEN_T);
	glutSolidCube(2);
	glDisable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
	glDisable(GL_TEXTURE_GEN_T);
	glPopMatrix();
	/*int idInnerList = glGenLists(1);
	glNewList(idInnerList, GL_COMPILE);

	glColor3ub(255, 0, 0);
	glPushMatrix();
	glTranslatef(20, 1, 0);
	glutSolidCube(2);
	glPopMatrix();

	glEndList();

	idList = glGenLists(1);
	glNewList(idList, GL_COMPILE);
	glCallList(idInnerList);

	glPushMatrix();
	glTranslatef(0, 0, -10);
	glCallList(idInnerList);
	glPopMatrix();
	glEndList();*/

	Make_Floor();
	Set_Item();
	Highlight();
}

void Main_Camera()
{
	glLoadIdentity();
	int vec = ceil(theta / 3.1415);
	int y;
	if (vec % 2)
		y = 1;
	else
		y = -1;
	
	gluLookAt(eye_x, eye_y, eye_z, at_x, at_y, at_z, 0, y, 0);

	previous_eye_x = eye_x;
	previous_eye_y = eye_y;
	previous_eye_z = eye_z;
	previous_at_x = at_x;
	previous_at_y = at_y;
	previous_at_z = at_z;
}

void Make_Floor()
{
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			glEnable(GL_TEXTURE_2D);
			glPushMatrix();
			glTranslatef(i * 2 - 10, -1, -10);
			glTranslatef(-10, 0, j * 2 - 10);
			glBindTexture(GL_TEXTURE_2D, texture[1]); //<1>
			glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
			glEnable(GL_TEXTURE_GEN_T);
			glutSolidCube(2);
			glDisable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
			glDisable(GL_TEXTURE_GEN_T);
			glTranslatef((i * -2) - 10, 1, (j * -2) - 10);
			glPopMatrix();
		}
	}
}

void Set_Item()
{
	for (int j = 0; j < itemcount; j++)
	{
		if (map_item[j].exist)
		{
			glEnable(GL_TEXTURE_2D);
			glPushMatrix();
			glTranslatef(map_item[j].x, map_item[j].y, map_item[j].z);
			glBindTexture(GL_TEXTURE_2D, texture[map_item[j].texture]); //<1>
			glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
			glEnable(GL_TEXTURE_GEN_T);
			glutSolidCube(2);
			glDisable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
			glDisable(GL_TEXTURE_GEN_T);
			glTranslatef(-1 * map_item[j].x, -1 * map_item[j].y, -1 * map_item[j].z);
			glPopMatrix();
		}
	}
}

void EnableLight()
{
	glEnable(GL_LIGHTING);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	GLfloat diffuse[] = { 0.7, 0.7, 0.7, 1 };

	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	glEnable(GL_LIGHT0);
}

void Highlight()
{
	glPushMatrix();
	glLineWidth(10);
	glColor3ub(255, 255, 255);
	glTranslatef(highlight_x, highlight_y, highlight_z);
	glutWireCube(2);
	glTranslatef(-highlight_x, -highlight_y, -highlight_z);
	glPopMatrix();
}

void Keyboard(unsigned char key, int x, int y)
{
#define ESCAPE '\033' 

	double rl_radian;
	double ud_radian;
	double fbmove_radian = rl_angle * PI / 180;
	double rlmove_radian = (rl_angle + 90) * PI / 180;
	bool exist = false;
	only_rotate = false;
	cube temp;

	switch (key)
	{
	case ESCAPE:
		exit(0);
		break;
	case '1':
		itemkey = 0;
		break;
	case '2':
		itemkey = 1;
		break;
	case '3':
		itemkey = 2;
		break;
	case '4':
		itemkey = 3;
		break;
	case '5':
		itemkey = 4;
		break;
	case '6':
		itemkey = 5;
		break;
	case '7':
		itemkey = 6;
		break;
	case '8':
		itemkey = 7;
		break;
	case 'j':
		rl_angle = 10;
		//rl_angle += 10;
		only_rotate = true;
		break;
	case 'l':
		rl_angle = -10;
		//rl_angle -= 10;
		only_rotate = true;
		break;
	case 'w':
		eye_x = previous_eye_x + speed * cos(fbmove_radian);
		eye_z = previous_eye_z - speed * sin(fbmove_radian);
		/*eye_x += speed * cos(fbmove_radian);
		eye_z -= speed * sin(fbmove_radian);*/
		break;
	case 's':
		eye_x = previous_eye_x - 2.0 * cos(fbmove_radian);
		eye_z = previous_eye_z + 2.0 * sin(fbmove_radian);
		/*eye_x -= 2.0 * cos(fbmove_radian);
		eye_z += 2.0 * sin(fbmove_radian);*/
		break;
	case 'i':
		ud_angle = 1;
		//ud_angle += 1;
		only_rotate = true;
		break;
	case 'k':
		ud_angle = -1;
		//ud_angle -= 1;
		only_rotate = true;
		break;
	case 'a':
		eye_x = previous_eye_x + 2.0 * cos(rlmove_radian);
		eye_z = previous_eye_z - 2.0 * sin(rlmove_radian);
		/*eye_x += 2.0 * cos(rlmove_radian);
		eye_z -= 2.0 * sin(rlmove_radian);*/
		break;
	case 'd':
		eye_x = previous_eye_x - 2.0 * cos(rlmove_radian);
		eye_z = previous_eye_z + 2.0 * sin(rlmove_radian);
		/*eye_x -= 2.0 * cos(rlmove_radian);
		eye_z += 2.0 * sin(rlmove_radian);*/
		break;
	case 'b':
		if (run_state == false)
		{
			run_state = true;
			speed = 3.0;
		}
		else
		{
			run_state = false;
			speed = 2.0;
		}
		break;
	case 'h':
		// 시점에 대해 큐브 중점의 x, z 결정
		map_item[itemcount].x = ((int)at_x / 2) * 2 + 1;
		map_item[itemcount].z = ((int)at_z / 2) * 2 + 1;
		// 첫번째 아이템에 대한 작업
		if (itemcount == 0)
		{
			map_item[itemcount].y = 1;
			map_item[itemcount].exist = true;
			map_item[itemcount].texture = itemkey;
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
					map_item[itemcount].y = item_id[i] * 2 + 1;
					map_item[itemcount].id = i;
					map_item[itemcount].exist = true;
					map_item[itemcount].texture = itemkey;
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
				map_item[itemcount].y = 1;
				map_item[itemcount].id = id;
				map_item[itemcount].exist = true;
				map_item[itemcount].texture = itemkey;
				itemcount++;
			}
		}
		break;
	case 'r':
		temp.x = ((int)at_x / 2) * 2 + 1;
		temp.z = ((int)at_z / 2) * 2 + 1;
		for (int i = 256; 0 <= i; i--)
		{
			if (map_item[i].exist == true)
			{
				if (temp.x == map_item[i].x && temp.z == map_item[i].z)
				{
					map_item[i].exist = false;
					item_id[map_item[i].id] -= 1;
					itemcount--;
					break;
				}
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
		if (key == 'i' || key == 'k')
		{
			at_x = (previous_at_x * cos(ud_radian)) - (previous_at_y * sin(ud_radian));
			at_y = (previous_at_y * cos(ud_radian)) + (previous_at_x * sin(ud_radian));
		}
		else if (key == 'j' || key == 'l')
		{
			at_x = (previous_at_x * cos(rl_radian)) + (previous_at_z * sin(rl_radian));
			at_z = (previous_at_z * cos(rl_radian)) - (previous_at_x * sin(rl_radian));
		}
	}

	if (key == 'a')
	{
		at_x = previous_at_x + 2.0 * cos(rlmove_radian);
		at_z = previous_at_z - 2.0 * sin(rlmove_radian);
		/*at_x += 2.0 * cos(rlmove_radian);
		at_z -= 2.0 * sin(rlmove_radian);*/
	}
	else if (key == 'd')
	{
		at_x = previous_at_x - 2.0 * cos(rlmove_radian);
		at_z = previous_at_z + 2.0 * sin(rlmove_radian);
		/*at_x -= 2.0 * cos(rlmove_radian);
		at_z += 2.0 * sin(rlmove_radian);*/
	}
	else if (key == 'w')
	{
		at_x = previous_at_x + 2.0 * cos(fbmove_radian);
		at_z = previous_at_z - 2.0 * sin(fbmove_radian);
		/*at_x += speed * cos(fbmove_radian);
		at_z -= speed * sin(fbmove_radian);*/
	}
	else if (key == 's')
	{
		at_x = previous_at_x - 2.0 * cos(rlmove_radian);
		at_z = previous_at_z + 2.0 * sin(rlmove_radian);
		/*at_x -= 2.0 * cos(fbmove_radian);
		at_z += 2.0 * sin(fbmove_radian);*/
	}
	if (run_state == true && key == 'w')
	{
		if (eyetop == false)
		{
			eye_y += 0.1;
			if (eye_y >= 4.0)
				eyetop = true;
		}
		else
		{
			eye_y -= 0.1;
			if (eye_y <= 3.0)
				eyetop = false;
		}
	}
	else if (run_state == true)
	{
		eye_y = 3.0;
	}

	bool tem = false;
	highlight_x = ((int)at_x / 2) * 2 + 7;
	highlight_z = ((int)at_z / 2) * 2 + 1;

	for (int i = 0; i <= itemcount; i++)
	{
		if (highlight_x == map_item[i].x && highlight_z == map_item[i].z)
		{
			int temp = item_id[map_item[i].id];
			switch (temp)
			{
			case 0:
				highlight_y = -1;
				break;
			case 1:
				highlight_y = 1;
				break;
			default:
				highlight_y = item_id[map_item[i].id] * 2 + 1;
				break;
			}
			tem = true;
			break;
		}
	}
	if (!tem)
		highlight_y = -1;


	glutPostRedisplay();
}

void Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glTranslatef(0.0, -0.6, 0.0);
	// glFrustum(-0.5356, 0.5356, -1.0712, 1.0712, 2, 100);
	gluPerspective(30, 2, 2, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Init_Camera()
{
	initAt_x = r * cos(0.0);
	initAt_y = 3.0;
	initAt_z = r * sin(0.0);

	at_x = initAt_x;
	at_y = initAt_y;
	at_z = initAt_z;
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(0, 0);
	MainWindow = glutCreateWindow("3d game");
	glutDisplayFunc(Display);
	glutMotionFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutReshapeFunc(Reshape);

	glEnable(GL_DEPTH_TEST);
	Init_Camera();

	EnableLight();
	Item_Load();

	glutMainLoop();

	FreeTexture(*texture);

	return 0;
}



// 이하는 현재 미사용

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
	gluLookAt(1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glTranslatef(0.0, 0.0, 1.6);
	glBindTexture(GL_TEXTURE_2D, texture[2]); //<2>
	glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
	glEnable(GL_TEXTURE_GEN_T);
	glutSolidCube(2);
	glDisable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
	glDisable(GL_TEXTURE_GEN_T);
	glPopMatrix();
	glutSwapBuffers();
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
	glColor3ub(255, 255, 255);
	glutSolidCube(20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.0, 1.6);
	glColor3ub(255, 255, 255);
	glutSolidCube(0.2);
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