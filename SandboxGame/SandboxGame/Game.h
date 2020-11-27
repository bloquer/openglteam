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
	void SetLookPoint();
	void MakeFloor(int N, int M);
	void SetItems();
	void Go();
	void Keyboard(unsigned char Key, int x, int y);

private:
	struct Cube
	{
		GLdouble x;
		GLdouble y;
		GLdouble z;
		GLuint Texture;
	};

	float Speed;
	Camera FPSCamera;
	Camera PreviousCam;

	std::vector<std::vector<SolidBox>> Floor;
	std::map<std::pair<double, double>, std::list<Cube>> Items;
	int ItemTexture;
	WiredBox HighlightBox;
	std::vector<GLuint> Textures;
};

