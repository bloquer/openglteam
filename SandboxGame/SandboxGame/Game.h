#pragma once

#include <vector>
#include "Window.h"
#include "SolidBox.h"

class Game
{
	Game();

	void MakeFloor(int N, int M);
	void SetTextures();

private:
	Window Wnd;
	std::vector<std::vector<SolidBox>> Floor;
	std::vector<GLuint> Textures;
};

