#pragma once

class Window
{
public:
	Window(int Width, int Height);

private:
	int Width;
	int Height;
	const char* WindowName = "Sandbox Game";
};

