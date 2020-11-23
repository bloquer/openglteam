#include "Game.h"
#include "Texture.h"

Game::Game()
	: Wnd(Window(1920, 1080))
{

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
