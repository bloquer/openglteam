#include <GL/glew.h>
#include <iostream>
#include <GL/glut.h>
#include <math.h> 
#include <stdlib.h>
#include <io.h>
#include <stdio.h>
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "Winmm.lib")
using namespace std;

#define WIDTH 1920 
#define HEIGHT 1080 
#define ITEMMAX 256

struct Vector
{
	Vector() {}
	Vector(double new_x, double new_y, double new_z)
	{
		x = new_x; y = new_y; z = new_z;
	}

	Vector operator - (Vector View_Vector) { return Vector(x - View_Vector.x, y - View_Vector.y, z - View_Vector.z); }

	double x, y, z;
};

class Camera
{
public:

	Vector eye;
	Vector at;
	Vector up;

	void First_Camera(double eye_x, double eye_y, double eye_z, double at_x, double at_y, double at_z,
		double up_x, double up_y, double up_z);
	void Move_Camera(double speed);
	void RLMove_Camera(double speed);

	void At_Move(double rl_angle, double ud_angle);
	void Rotate_View(double speed);
};

void Camera::First_Camera(double eye_x, double eye_y, double eye_z, double at_x, double at_y, double at_z,
	double up_x, double up_y, double up_z)
{
	eye = Vector(eye_x, eye_y, eye_z); // eye 설정
	at = Vector(at_x, at_y, at_z); // at 설정
	up = Vector(up_x, up_y, up_z); // up 설정
}

void Camera::Move_Camera(double speed)
{
	Vector View_Vector = at - eye;   // at과 eye의 벡터(증가분)

	// 거리*speed 만큼 이동
	eye.x = eye.x + View_Vector.x * speed;
	eye.z = eye.z + View_Vector.z * speed;
	at.x = at.x + View_Vector.x * speed;
	at.z = at.z + View_Vector.z * speed;
}

void Camera::RLMove_Camera(double speed)
{
	Vector View_Vector = at - eye;   // at과 eye의 벡터(증가분)
	Vector Pers_Vector;              // perspective 벡터 설정

	// 좌우 이동은 앞뒤와 90도 차이
	Pers_Vector.x = -View_Vector.z;
	Pers_Vector.z = View_Vector.x;

	// 앞뒤 이동과 같은 과정
	eye.x = eye.x + Pers_Vector.x * speed;
	eye.z = eye.z + Pers_Vector.z * speed;
	at.x = at.x + Pers_Vector.x * speed;
	at.z = at.z + Pers_Vector.z * speed;
}

void Camera::Rotate_View(double angle)
{
	Vector View_Vector = at - eye;   // at과 eye의 벡터(증가분)

	at.z = (double)(eye.z + sin(angle)*View_Vector.x + cos(angle)*View_Vector.z);
	at.x = (double)(eye.x + cos(angle)*View_Vector.x - sin(angle)*View_Vector.z);
}

void Camera::At_Move(double rl_angle, double ud_angle)
{
	// 시점 상하는 단순히 at.y만 변경
	at.y += ud_angle * 2;

	Rotate_View(-rl_angle);
}

GLuint LoadTexture(const char * filename, int width, int height, int i);
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

typedef struct cube
{
	bool exist = false;
	int id;
	double x;
	double y;
	double z;
	int texture;
}cube;

GLuint texture[9];      // 텍스쳐 저장을 위함

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

cube map_item[ITEMMAX];
int item_id[ITEMMAX] = { 0, };
int itemcount = 0;
int id = 0;

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
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
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
	texture[0] = LoadTexture("texture/dirt.bmp", 1300, 1300, 0);
	texture[1] = LoadTexture("texture/stones.bmp", 512, 512, 1);
	texture[2] = LoadTexture("texture/3.bmp", 1388, 800, 2);
	texture[3] = LoadTexture("texture/brick.bmp", 914, 800, 3);
	texture[4] = LoadTexture("texture/kr.bmp", 256, 256, 4);
	texture[5] = LoadTexture("texture/ice.bmp", 512, 512, 5);
	texture[6] = LoadTexture("texture/er.bmp", 1001, 800, 6);
	texture[7] = LoadTexture("texture/magma.bmp", 2048, 2048, 7);
	texture[8] = LoadTexture("texture/grass.bmp", 256, 256, 8);
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

	gluLookAt(FPSCmaera.eye.x, FPSCmaera.eye.y, FPSCmaera.eye.z,
		FPSCmaera.at.x, FPSCmaera.at.y, FPSCmaera.at.z,
		FPSCmaera.up.x, FPSCmaera.up.y, FPSCmaera.up.z);

	previous_eye_x = FPSCmaera.eye.x;
	previous_eye_y = FPSCmaera.eye.y;
	previous_eye_z = FPSCmaera.eye.z;
	previous_at_x = FPSCmaera.at.x;
	previous_at_y = FPSCmaera.at.y;
	previous_at_z = FPSCmaera.at.z;
}

void Make_Floor()
{
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			glEnable(GL_TEXTURE_2D);
			glPushMatrix();
			glTranslatef(i * 2, -1, 0);
			glTranslatef(0, 0, j * 2);
			glBindTexture(GL_TEXTURE_2D, texture[8]); // 바닥 텍스쳐
			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);
			glutSolidCube(2);
			glDisable(GL_TEXTURE_GEN_S);
			glDisable(GL_TEXTURE_GEN_T);
			glTranslatef((i * -2), 1, (j * -2));
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
			glBindTexture(GL_TEXTURE_2D, texture[map_item[j].texture]); // 각 아이템의 텍스쳐를 불러온다
			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);
			glutSolidCube(2);
			glDisable(GL_TEXTURE_GEN_S);
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
	glLineWidth(14);
	glPushMatrix();
	glColor3ub(255, 255, 51);
	glTranslatef(highlight_x, highlight_y, highlight_z);
	glutWireCube(2);
	glTranslatef(-highlight_x, -highlight_y, -highlight_z);
	glPopMatrix();
	glColor3ub(255, 255, 255);
}

void Keyboard(unsigned char key, int x, int y)
{
#define ESCAPE '\033' 

	double temprun_at_y = 2.0;
	bool exist = false;
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
	case 'w':
		FPSCmaera.Move_Camera(cameraspeed);
		break;
	case 's':
		FPSCmaera.Move_Camera(-0.3);
		break;
	case 'a':
		FPSCmaera.RLMove_Camera(-0.3);
		break;
	case 'd':
		FPSCmaera.RLMove_Camera(0.3);
		break;
	case 'j':
		FPSCmaera.At_Move(0.05, 0);
		temprun_at_y = FPSCmaera.at.y;
		break;
	case 'l':
		FPSCmaera.At_Move(-0.05, 0);
		temprun_at_y = FPSCmaera.at.y;
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
		sndPlaySoundA("C:\\Users\\995sk\\Desktop\\proto\\proto\\set.mp3", SND_ASYNC | SND_ALIAS);
		// 시점에 대해 큐브 중점의 x, z 결정
		map_item[itemcount].x = ((int)FPSCmaera.at.x / 2) * 2 + 6;
		map_item[itemcount].z = ((int)FPSCmaera.at.z / 2) * 2;
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
				item_id[id] += 1;
				itemcount++;
			}
		};
		break;
	case 'r':
		temp.x = ((int)FPSCmaera.at.x / 2) * 2 + 6;
		temp.z = ((int)FPSCmaera.at.z / 2) * 2;
		for (int i = 255; 0 <= i; i--)
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
	if (ud_angle > -10 && ud_angle < 25)
	{
		if (key == 'i')
			FPSCmaera.At_Move(0, 0.01);
		else if (key == 'k')
			FPSCmaera.At_Move(0, -0.01);
	}

	temprun_at_y = FPSCmaera.at.y;
	if (run_state == true && key == 'w')
	{
		if (eyetop == false)
		{
			FPSCmaera.at.y += 0.1;
			FPSCmaera.eye.y += 0.1;
			if (FPSCmaera.eye.y >= 3.5)
				eyetop = true;
		}
		else
		{
			FPSCmaera.at.y -= 0.1;
			FPSCmaera.eye.y -= 0.1;
			if (FPSCmaera.eye.y <= 3.0)
				eyetop = false;
		}
	}
	else if (run_state == false)
	{
		FPSCmaera.at.y = temprun_at_y;
		FPSCmaera.eye.y = 3.0;
	}

	bool tem = false;
	highlight_x = ((int)FPSCmaera.at.x / 2) * 2 + 6;
	highlight_z = ((int)FPSCmaera.at.z / 2) * 2;

	for (int i = 0; i < itemcount; i++)
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
				highlight_y = (item_id[map_item[i].id] - 1) * 2 + 1;
				break;
			}
			tem = true;
			break;
		}
	}
	if (!tem)
		highlight_y = -1;

	// 충돌 처리
	for (int j = 0; j < itemcount; j++)
	{
		if (map_item[j].exist)
		{
			int dis = sqrt(pow(FPSCmaera.eye.x - map_item[j].x, 2) + pow(FPSCmaera.eye.z - map_item[j].z, 2));

			if (dis >= 1.5 && dis <= 2.0)
			{
				FPSCmaera.eye.x = previous_eye_x;
				FPSCmaera.eye.y = previous_eye_y;
				FPSCmaera.eye.z = previous_eye_z;
				FPSCmaera.at.x = previous_at_x;
				FPSCmaera.at.y = previous_at_y;
				FPSCmaera.at.z = previous_at_z;
			}
		}
	}
	if (FPSCmaera.eye.x <= 0 || FPSCmaera.eye.z <= 0 || FPSCmaera.eye.x >= 199 || FPSCmaera.eye.z >= 199)
	{
		FPSCmaera.eye.x = previous_eye_x;
		FPSCmaera.eye.y = previous_eye_y;
		FPSCmaera.eye.z = previous_eye_z;
		FPSCmaera.at.x = previous_at_x;
		FPSCmaera.at.y = previous_at_y;
		FPSCmaera.at.z = previous_at_z;
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
	FPSCmaera.First_Camera(50, 3.0, 50, 53, 2.0, 50, 0, 1, 0);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("3d game");
	glutDisplayFunc(Display);
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