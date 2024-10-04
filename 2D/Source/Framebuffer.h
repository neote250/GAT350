#pragma once
#include <SDL.h>
#include <vector>

using color_t = SDL_Color;

class Framebuffer
{
public:
	Framebuffer(const class Renderer& renderer, int width, int height);
	~Framebuffer();

	void Update();
	void Clear(const color_t& color);

	void DrawPoint(int x, int y, const color_t& color);
	void DrawRect(int x, int y, int width, int height, const color_t& color);
	void DrawLine(int x1, int y1, int x2, int y2, const color_t& color);
	void DrawLineSlope(int x1, int y1, int x2, int y2, const color_t& color);
	void DrawCircle(int x, int y, int radius, const color_t& color);
	void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const color_t& color);

public:
	int _width{ 0 };
	int _height{ 0 };
	int _pitch{ 0 };

	SDL_Texture* _texture{ nullptr };
	std::vector<color_t> _buffer;
};