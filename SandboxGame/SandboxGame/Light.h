#pragma once

class Light
{
public:
	Light();
	~Light();

	void Generate();

private:
	float* Pos;
	float* Ambient;
	float* Diffuse;
};

