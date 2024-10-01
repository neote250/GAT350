#pragma once
#include <string>


class Renderer
{
public:
	void Initialize();
	void CreateWindow(int sizeX = 800, int sizeY = 600, std::string name = "Game Engine");
};