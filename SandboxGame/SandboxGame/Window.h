#pragma once

class Window
{
public:
	Window(int Width, int Height);

	void CreateMainWindow();
	void Reshaping(int ChangedWidth, int ChangedHeight);

private:
	int Width;
	int Height;
	const char* WindowName = "Sandbox Game";
};

