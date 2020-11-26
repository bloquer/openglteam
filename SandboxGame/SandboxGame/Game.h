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

	void Go();
	void MakeFloor(int N, int M);
	void SetTextures();
	void SetLookPoint();
	void SetItems();
	void InitSetting();

private:
	struct Cube
	{
		GLdouble x;
		GLdouble y;
		GLdouble z;
		GLuint Texture;
	};

	Window Wnd;
	Camera FPSCamera;
	Camera PreviousCam;
	Light Sun;
	std::vector<std::vector<SolidBox>> Floor;
	std::map<std::pair<double, double>, std::list<Cube>> Items;
	WiredBox HighlightBox;
	std::vector<GLuint> Textures;
};

