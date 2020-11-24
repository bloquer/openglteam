#include <GL/glew.h>
#include <iostream>
#include <GL/glut.h>
#include <math.h> 
#include <stdlib.h>
#include <io.h>
#include <stdio.h>
#include <Windows.h>
#include <mmsystem.h>
#include <vector>
#include <list>
#include <map>
#include "Vector.h"
#include "Camera.h"
#include "Texture.h"
#include "SolidBox.h"
#include "WiredBox.h"

#pragma comment(lib, "Winmm.lib")

using namespace std;

#define WIDTH 1920 
#define HEIGHT 1080 
#define ITEMMAX 256

Camera FPSCmaera;
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

struct Cube
{
	double x;
	double y;
	double z;
	int texture;
};

GLuint texture[9];      // 텍스쳐 저장을 위함
Texture Tex[9];

double cameraspeed = 0.3;
double ud_angle = 0.0;
bool run_state = false;
bool eyetop = false;

bool jump = false;
double timer = 0.0;

double previous_trans_x = 0.0;
double previous_trans_y = 0.0;
double previous_trans_z = 0.0;

double previous_eye_x = 0.0;
double previous_eye_y = 0.0;
double previous_eye_z = 0.0;

double previous_at_x = 0.0;
double previous_at_y = 0.0;
double previous_at_z = 0.0;

double highlight_x = 0.0;
double highlight_y = 0.0;
double highlight_z = 0.0;

int itemkey = 0;

map<pair<int, int>, list<Cube>> ItemsCnt;

Cube map_item[ITEMMAX];
int item_id[ITEMMAX] = { 0, };
int itemcount = 0;
int id = 0;

void FreeTexture(GLuint texture)
{
	glDeleteTextures(1, &texture);
}

void Item_Load()
{
	Tex[0] = Texture(1300, 1300, "texture/dirt.bmp");
	Tex[1] = Texture(512, 512, "texture/stones.bmp");
	Tex[2] = Texture(1388, 800, "texture/3.bmp");
	Tex[3] = Texture(914, 800, "texture/brick.bmp");
	Tex[4] = Texture(256, 256, "texture/kr.bmp");
	Tex[5] = Texture(512, 512, "texture/ice.bmp");
	Tex[6] = Texture(1001, 800, "texture/er.bmp");
	Tex[7] = Texture(2048, 2048, "texture/magma.bmp");
	Tex[8] = Texture(256, 256, "texture/grass.bmp");

	for (int i = 0; i < 9; i++)
	{
		Tex[i].LoadTexture(texture[i]);
	}
}

void Display()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Main_Camera();
	Main_Game();

	GLfloat pos[] = { 0, 0, 0, 1 };
	glLightfv(GL_LIGHT0, GL_POSITION, pos);

	glFlush();
	glutSwapBuffers();
}

void Main_Game()
{
	Make_Floor();
	Set_Item();
	Highlight();
}

void Main_Camera()
{
	glLoadIdentity();

	gluLookAt(FPSCmaera.Eye.x, FPSCmaera.Eye.y, FPSCmaera.Eye.z,
		FPSCmaera.At.x, FPSCmaera.At.y, FPSCmaera.At.z,
		FPSCmaera.Up.x, FPSCmaera.Up.y, FPSCmaera.Up.z);

	previous_eye_x = FPSCmaera.Eye.x;
	previous_eye_y = FPSCmaera.Eye.y;
	previous_eye_z = FPSCmaera.Eye.z;
	previous_at_x = FPSCmaera.At.x;
	previous_at_y = FPSCmaera.At.y;
	previous_at_z = FPSCmaera.At.z;
}

void Make_Floor()
{
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			SolidBox Floor(i * 2, -1, j * 2, texture[8]);
			Floor.Generate(2);
		}
	}
}

void Set_Item()
{
	for (auto& elem : ItemsCnt)
	{
		for (auto& cube : elem.second)
		{
			glEnable(GL_TEXTURE_2D);
			glPushMatrix();
			glTranslatef(cube.x, cube.y, cube.z);
			glBindTexture(GL_TEXTURE_2D, texture[cube.texture]);
			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);
			glutSolidCube(2);
			glDisable(GL_TEXTURE_GEN_S);
			glDisable(GL_TEXTURE_GEN_T);
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
	glLineWidth(14);
	glColor3ub(255, 255, 51);
	glTranslatef(highlight_x, highlight_y, highlight_z);
	glutWireCube(2);
	glPopMatrix();
}

void Keyboard(unsigned char key, int x, int y)
{
#define ESCAPE '\033' 

	double temprun_at_y = 2.0;
	bool exist = false;
	Cube temp;

	Vector Norm = FPSCmaera.At - FPSCmaera.Eye;
	double tmpx = ((int)(FPSCmaera.At.x + Norm.x * 2 + 1) / 2) * 2;
	double tmpz = ((int)(FPSCmaera.At.z + Norm.z * 2 + 1) / 2) * 2;
	pair<double, double> Target = make_pair(tmpx, tmpz);

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
	case 'w':
		FPSCmaera.MoveForward(cameraspeed);
		break;
	case 's':
		FPSCmaera.MoveForward(-0.3);
		break;
	case 'a':
		FPSCmaera.MoveRight(-0.3);
		break;
	case 'd':
		FPSCmaera.MoveRight(0.3);
		break;
	case 'j':
		FPSCmaera.LookRight(-0.05);
		temprun_at_y = FPSCmaera.At.y;
		break;
	case 'l':
		FPSCmaera.LookRight(0.05);
		temprun_at_y = FPSCmaera.At.y;
		break;
	case 'i':
		if (ud_angle >= 25)
			break;
		ud_angle += 1;
		break;
	case 'k':
		if (ud_angle <= -10)
			break;
		ud_angle -= 1;
		break;
	case 'b':
		if (run_state == false)
		{
			run_state = true;
			cameraspeed = 0.6;
		}
		else
		{
			run_state = false;
			cameraspeed = 0.3;
		}
		break;
	case 'h':
		sndPlaySoundA("C:\\Users\\995sk\\Desktop\\SandboxGame\\SandboxGame\\set.mp3", SND_ASYNC | SND_ALIAS);

		if (ItemsCnt.count(Target) == 0 || ItemsCnt[Target].size() == 0)
		{
			ItemsCnt[make_pair(tmpx, tmpz)].emplace_back();
			ItemsCnt[make_pair(tmpx, tmpz)].back().x = tmpx;
			ItemsCnt[make_pair(tmpx, tmpz)].back().z = tmpz;
			ItemsCnt[make_pair(tmpx, tmpz)].back().y = 1;
			ItemsCnt[make_pair(tmpx, tmpz)].back().texture = itemkey;
		}
		else
		{
			ItemsCnt[make_pair(tmpx, tmpz)].emplace_back();
			ItemsCnt[make_pair(tmpx, tmpz)].back().x = tmpx;
			ItemsCnt[make_pair(tmpx, tmpz)].back().z = tmpz;
			ItemsCnt[make_pair(tmpx, tmpz)].back().y = (ItemsCnt[Target].size() - 1) * 2 + 1;
			ItemsCnt[make_pair(tmpx, tmpz)].back().texture = itemkey;
		}
		break;
	case 'r':
		if (ItemsCnt.count(Target) > 0)
		{
			ItemsCnt[Target].pop_back();
			if (ItemsCnt[Target].size() == 0)
				ItemsCnt.erase(Target);
		}
		break;
	default:
		break;
	}
	if (ud_angle > -10 && ud_angle < 25)
	{
		if (key == 'i')
			FPSCmaera.LookUp(0.01);
		else if (key == 'k')
			FPSCmaera.LookUp(-0.01);
	}

	temprun_at_y = FPSCmaera.At.y;
	if (run_state == true && key == 'w')
	{
		if (eyetop == false)
		{
			FPSCmaera.At.y += 0.1;
			FPSCmaera.Eye.y += 0.1;
			if (FPSCmaera.Eye.y >= 3.5)
				eyetop = true;
		}
		else
		{
			FPSCmaera.At.y -= 0.1;
			FPSCmaera.Eye.y -= 0.1;
			if (FPSCmaera.Eye.y <= 3.0)
				eyetop = false;
		}
	}
	else if (run_state == false)
	{
		FPSCmaera.At.y = temprun_at_y;
		FPSCmaera.Eye.y = 3.0;
	}

	bool tem = false;
	highlight_x = ((int)(FPSCmaera.At.x + Norm.x * 2 + 1) / 2) * 2;
	highlight_z = ((int)(FPSCmaera.At.z + Norm.z * 2 + 1) / 2) * 2;
	if (ItemsCnt.count(make_pair(highlight_x, highlight_z)) == 0)
		highlight_y = -1;
	else
		highlight_y = (ItemsCnt[make_pair(highlight_x, highlight_z)].size() - 1) * 2 + 1;

	// 충돌 처리
	for (auto& elem : ItemsCnt)
	{
		double dis = sqrt(
			pow(elem.first.first - FPSCmaera.Eye.x, 2) +
			pow(elem.first.second - FPSCmaera.Eye.z, 2)
		);

		if (dis >= 1.5 && dis <= 2.0)
		{
			FPSCmaera.Eye.x = previous_eye_x;
			FPSCmaera.Eye.y = previous_eye_y;
			FPSCmaera.Eye.z = previous_eye_z;
			FPSCmaera.At.x = previous_at_x;
			FPSCmaera.At.y = previous_at_y;
			FPSCmaera.At.z = previous_at_z;
		}
	}
	if (FPSCmaera.Eye.x <= 0 || FPSCmaera.Eye.z <= 0 || FPSCmaera.Eye.x >= 199 || FPSCmaera.Eye.z >= 199)
	{
		FPSCmaera.Eye.x = previous_eye_x;
		FPSCmaera.Eye.y = previous_eye_y;
		FPSCmaera.Eye.z = previous_eye_z;
		FPSCmaera.At.x = previous_at_x;
		FPSCmaera.At.y = previous_at_y;
		FPSCmaera.At.z = previous_at_z;
	}

	glutPostRedisplay();
}

void Reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glTranslatef(0.0, -0.6, 0.0);
	gluPerspective(30, 2, 2, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Init_Camera()
{
	FPSCmaera = Camera(50.0, 3.0, 50.0, 53.0, 2.0, 50.0, 0.0, 1.0, 0.0);
}

int bx = WIDTH / 2, by = HEIGHT / 2;
void Mouse(int x, int y)
{
	FPSCmaera.LookRight((x - bx) * 0.05);
	std::cout << x << ' ' << bx << '\n';
	bx = x;
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("3d game");
	glutDisplayFunc(Display);
	glutPassiveMotionFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutReshapeFunc(Reshape);

	glEnable(GL_DEPTH_TEST);
	Init_Camera();

	//EnableLight();
	Item_Load();

	glutMainLoop();

	FreeTexture(*texture);

	return 0;
}