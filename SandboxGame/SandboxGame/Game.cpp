#include "Game.h"
#include "Texture.h"

Game::Game()
	:
	Wnd(Window(1920, 1080)),
	FPSCamera(Camera(50.0, 3.0, 50.0, 53.0, 2.0, 50.0, 0.0, 1.0, 0.0))
{
	Vector Norm = FPSCamera.At - FPSCamera.Eye;
	HighlightBox = WiredBox(Norm.x, Norm.y, Norm.z, 14.0);
}

void Game::Go()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	InitSetting();
	SetLookPoint();
	MakeFloor(100, 100);
	SetItems();
	Sun.Generate();

	GLfloat pos[] = { 0, 0, 0, 1 };
	glLightfv(GL_LIGHT0, GL_POSITION, pos);

	glFlush();
	glutSwapBuffers();
}

void Game::MakeFloor(int N, int M)
{
	for (int X = 0; X < N; X++)
	{
		Floor.emplace_back();
		for (int Z = 0; Z < M; Z++)
		{
			Floor.back().emplace_back(X * 2, -1, Z * 2, Textures[8]);
		}
	}
}

void Game::SetTextures()
{
	std::vector<Texture> Tex;

	Tex.emplace_back(1300, 1300, "texture/dirt.bmp");
	Tex.emplace_back(512, 512, "texture/stones.bmp");
	Tex.emplace_back(1388, 800, "texture/3.bmp");
	Tex.emplace_back(914, 800, "texture/brick.bmp");
	Tex.emplace_back(256, 256, "texture/kr.bmp");
	Tex.emplace_back(512, 512, "texture/ice.bmp");
	Tex.emplace_back(1001, 800, "texture/er.bmp");
	Tex.emplace_back(2048, 2048, "texture/magma.bmp");
	Tex.emplace_back(256, 256, "texture/grass.bmp");

	for (int i = 0; i < 9; i++)
	{
		Tex[i].LoadTexture(Textures[i]);
	}
}

void Game::SetLookPoint()
{
	glLoadIdentity();
	gluLookAt(FPSCamera.Eye.x, FPSCamera.Eye.y, FPSCamera.Eye.z,
		FPSCamera.At.x, FPSCamera.At.y, FPSCamera.At.z,
		FPSCamera.Up.x, FPSCamera.Up.y, FPSCamera.Up.z);
	PreviousCam = FPSCamera;
}

void Game::SetItems()
{
	for (auto& Item : Items)
	{
		for (auto& cube : Item.second)
		{
			glEnable(GL_TEXTURE_2D);
			glPushMatrix();
			glTranslatef(cube.x, cube.y, cube.z);
			glBindTexture(GL_TEXTURE_2D, Textures[cube.Texture]);
			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);
			glutSolidCube(2);
			glDisable(GL_TEXTURE_GEN_S);
			glDisable(GL_TEXTURE_GEN_T);
			glPopMatrix();
		}
	}
}

void Game::InitSetting()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
}
