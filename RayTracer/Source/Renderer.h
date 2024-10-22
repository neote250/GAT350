#pragma once
#include <string>
#include <SDL.h>

class Renderer
{
public:
	bool Initialize();
	bool CreateWindow(int sizeX = 800, int sizeY = 600, std::string name = "Game Engine");
	void CopyFramebuffer(const class Framebuffer& framebuffer);

	SDL_Window* _window;
	SDL_Renderer* _renderer;

	int _width = 0;
	int _height = 0;
	std::string _name;
private:


};