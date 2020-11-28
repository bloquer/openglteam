#pragma once

#include "Window.h"
#include "Camera.h"
#include "SolidBox.h"
#include "WiredBox.h"
#include "Light.h"
#include <vector>
#include <map>
#include <list>

class Game
{
public:
	Game();

	void SetTextures();
	void InitSetting();
	void SetLookPoint(Camera Cam);
	void MakeFloor();
	void SetItems();
	void Go();
	void Keyboard(unsigned char Key, int x, int y);

	float Speed;
	Camera FPSCamera;
	Camera PreviousCam;

private:
	struct Cube
	{
		GLdouble x;
		GLdouble y;
		GLdouble z;
		GLuint Texture;
	};

	std::vector<std::vector<SolidBox>> Floor;
	std::map<std::pair<double, double>, std::list<Cube>> Items;
	int ItemTexture;
	std::vector<GLuint> Textures;
};