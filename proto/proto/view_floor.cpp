#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <gl/glut.h>
#include <stb_image.h>
#include <FreeImage.h>
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
double updown = 0;
double theta = 1.5;
int mouse_x = 0;
int mouse_y = 0;

cube temp[256];
int itemcount = 0;

int framebufferWidth, framebufferHeight;
GLuint triangleVertexArrayObject;
GLuint triangleShaderProgramID;
GLuint trianglePositionVertexBufferObjectID, triangleColorVertexBufferObjectID;
GLuint triangleTextureCoordinateBufferObjectID;

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

GLuint CreateTexture(char const* filename)
{
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename, 0);

	if (format == -1)
		exit(-1);

	if (format == FIF_UNKNOWN)
	{
		format = FreeImage_GetFIFFromFilename(filename);
		if (!FreeImage_FIFSupportsReading(format))
			exit(-1);
	}

	FIBITMAP* bitmap = FreeImage_Load(format, filename);

	int bitsPerPixel = FreeImage_GetBPP(bitmap);

	FIBITMAP* bitmap32;

	if (bitsPerPixel == 32)
		bitmap32 = bitmap;
	else
		bitmap32 = FreeImage_ConvertTo32Bits(bitmap);

	int imageWidth = FreeImage_GetWidth(bitmap32);
	int imageHeight = FreeImage_GetHeight(bitmap32);

	GLubyte* textureData = FreeImage_GetBits(bitmap32);

	GLuint tempTextureID;
	glGenTextures(1, &tempTextureID);
	glBindTexture(GL_TEXTURE_2D, tempTextureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, textureData);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glBindTexture(GL_TEXTURE_2D, 0);
	FreeImage_Unload(bitmap32);

	if (bitsPerPixel != 32)
		FreeImage_Unload(bitmap);

	return tempTextureID;
}
bool initShaderProgram() {

	//#3
	const GLchar* vertexShaderSource =
		"#version 330 core\n"
		"in vec3 positionAttribute;"
		//"in vec3 colorAttribute;"
		"in vec2 textureCoordinateAttribute;"
		//"out vec3 passColorAttribute;"
		"out vec2 passTextureCoordinateAttribute;"
		"void main()"
		"{"
		"gl_Position = vec4(positionAttribute, 1.0);"
		//"passColorAttribute = colorAttribute;"
		"passTextureCoordinateAttribute = textureCoordinateAttribute;"
		"}";


	//#4
	const GLchar* fragmentShaderSource =
		"#version 330 core\n"
		//"in vec3 passColorAttribute;"
		"in vec2 passTextureCoordinateAttribute;"
		"out vec4 fragmentColor;"
		"uniform sampler2D tex;"
		"void main()"
		"{"
		//컬러만 출력
		//"fragmentColor = vec4(passColorAttribute, 1.0);"
		//텍스처만 출력
		"fragmentColor = texture(tex, passTextureCoordinateAttribute);"
		//텍스처와 컬러 같이 출력
		//"fragmentColor = texture(tex, passTextureCoordinateAttribute)*vec4(passColorAttribute, 1.0); "
		"}";



	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << endl;
		glDeleteShader(vertexShader);
		return false;
	}


	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << endl;

		return false;
	}




	//#5
	triangleShaderProgramID = glCreateProgram();

	glAttachShader(triangleShaderProgramID, vertexShader);
	glAttachShader(triangleShaderProgramID, fragmentShader);

	glLinkProgram(triangleShaderProgramID);


	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	glGetProgramiv(triangleShaderProgramID, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(triangleShaderProgramID, 512, NULL, errorLog);
		cerr << "ERROR: shader program 연결 실패\n" << errorLog << endl;
		return false;
	}

	return true;
}
bool defineVertexArrayObject() {

	glVertex3f(-1.0, 2.0, 1.0);
	glVertex3f(-1.0, 2.0, -1.0);
	glVertex3f(-1.0, -2.0, -1.0);
	glVertex3f(-1.0, -2.0, 1.0);
	//#1
	//삼각형을 구성하는 vertex 데이터 - position과 color
	float position[] = {
		-1.0f,  2.0f, 1.0f, //vertex 1  위 중앙
		-1.0f, 2.0f, -1.0f, //vertex 2  오른쪽 아래
		-1.0f, -2.0f, -1.0f //vertex 3  왼쪽 아래
		-1.0f, -2.0f, 1.0f //vertex 3  왼쪽 아래
	};

	float color[] = {
		1.0f, 0.0f, 0.0f, //vertex 1 : RED (1,0,0)
		0.0f, 1.0f, 0.0f, //vertex 2 : GREEN (0,1,0) 
		0.0f, 0.0f, 1.0f  //vertex 3 : BLUE (0,0,1)
	};

	float textureCoordinate[] = {
		0.5f, 1.0f,  //vertex 1  
		1.0f, 0.0f,  //vertex 2
		0.0f, 0.0f   //vertex 3        
	};



	//#2
	//Vertex Buffer Object(VBO)를 생성하여 vertex 데이터를 복사한다.
	glGenBuffers(1, &trianglePositionVertexBufferObjectID);
	glBindBuffer(GL_ARRAY_BUFFER, trianglePositionVertexBufferObjectID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(position), position, GL_STATIC_DRAW);

	glGenBuffers(1, &triangleColorVertexBufferObjectID);
	glBindBuffer(GL_ARRAY_BUFFER, triangleColorVertexBufferObjectID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

	glGenBuffers(1, &triangleTextureCoordinateBufferObjectID);
	glBindBuffer(GL_ARRAY_BUFFER, triangleTextureCoordinateBufferObjectID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoordinate), textureCoordinate, GL_STATIC_DRAW);


	//#6
	glGenVertexArrays(1, &triangleVertexArrayObject);
	glBindVertexArray(triangleVertexArrayObject);


	GLint positionAttribute = glGetAttribLocation(triangleShaderProgramID, "positionAttribute");
	if (positionAttribute == -1) {
		cerr << "position 속성 설정 실패" << endl;
		return false;
	}
	glBindBuffer(GL_ARRAY_BUFFER, trianglePositionVertexBufferObjectID);
	glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(positionAttribute);


	/*
	GLint colorAttribute = glGetAttribLocation(triangleShaderProgramID, "colorAttribute");
	if (colorAttribute == -1) {
		cerr << "color 속성 설정 실패" << endl;
		return false;
	}
	glBindBuffer(GL_ARRAY_BUFFER, triangleColorVertexBufferObjectID);
	glVertexAttribPointer(colorAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(colorAttribute);
	*/


	GLint textureCoordinateAttribute = glGetAttribLocation(triangleShaderProgramID, "textureCoordinateAttribute");
	if (textureCoordinateAttribute == -1) {
		cerr << "Texture Coordinate 속성 설정 실패" << endl;
		return false;
	}
	glBindBuffer(GL_ARRAY_BUFFER, triangleTextureCoordinateBufferObjectID);
	glVertexAttribPointer(textureCoordinateAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(textureCoordinateAttribute);


	glBindVertexArray(0);


	return true;
}
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	//처음 2개의 파라미터는 viewport rectangle의 왼쪽 아래 좌표
	//다음 2개의 파라미터는 viewport의 너비와 높이이다.
	//framebuffer의 width와 height를 가져와 glViewport에서 사용한다.
	glViewport(0, 0, width, height);

	framebufferWidth = width;
	framebufferHeight = height;
}


void maindis()
{
	glClearColor(0, 0, 0, 1);
	glutSwapBuffers();
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
		eye_x -= 2.0 * cos(fbmove_radian);
		eye_z += 2.0 * sin(fbmove_radian);
		break;
	case 'y':
		ud_angle += 5;
		break;
	case 'h':
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
	case 't':
		temp[itemcount].x = 20.0;
		temp[itemcount].y = 20.0;
		temp[itemcount].z = 0.0;
		itemcount++;
		break;
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
			if (eye_y >= 9.0)
				eyetop = true;
		}
		else
		{
			eye_y -= 1.0;
			if (eye_y <= 1.0)
				eyetop = false;
		}
	}
	else if (run_state == true)
	{
		eye_y = 1.0;
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

	for (int j = 0; j < itemcount; j++)
	{
		glPushMatrix();
		glColor3ub(102, 0, 0);
		glTranslatef(0, j * 100, -200);
		glutSolidCube(50);
		glTranslatef(-1 * temp[j].x, -1 * temp[j].y, -1 * temp[j].z);
		glPopMatrix();
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
	GLUquadric * sphere;
	sphere = gluNewQuadric();
	GLuint textureID = CreateTexture("sand.png");
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, textureID);
	/*initShaderProgram();
	defineVertexArrayObject();
	glUniform1i(glGetUniformLocation(triangleTextureCoordinateBufferObjectID, "tex"), 0);
	*/


	glPushMatrix();
	glBegin(GL_POLYGON);
	glColor3f(1.0, 0.502, 0.502);
	glVertex3f(-1.0, -0.9, 0.6);
	glVertex3f(-1.0, -0.9, -0.6);
	glVertex3f(-1.0, -2.0, -0.6);
	glVertex3f(-1.0, -2.0, 0.6);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 1);
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