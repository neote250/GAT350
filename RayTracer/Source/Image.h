#pragma once

#include <SDL.h>
#include <string>
#include <vector>

using color_t = SDL_Color;

class Image
{
public:
	Image() = default;
	~Image() = default;

	bool Load(const std::string& filename);

public:
	int _width{ 0 };
	int _height{ 0 };

	std::vector<color_t> _buffer;
};