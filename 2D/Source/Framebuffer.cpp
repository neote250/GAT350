#include "Framebuffer.h"
#include "Renderer.h"
#include <iostream>

Framebuffer::Framebuffer(const Renderer& renderer, int width, int height)
{
	_width = width;
	_height = height;
	_pitch = width * sizeof(color_t);

	_texture = SDL_CreateTexture(renderer._renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, width, height);
	if (!_texture)
	{
		std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
	}
	_buffer.resize(_width * _height);
}

Framebuffer::~Framebuffer()
{
	SDL_DestroyTexture(_texture);
}

void Framebuffer::Update()
{
	SDL_UpdateTexture(_texture, NULL, _buffer.data(), _pitch);
}

void Framebuffer::Clear(const color_t& color)
{
	std::fill(_buffer.begin(), _buffer.end(), color);
}

void Framebuffer::DrawPoint(int x, int y, const color_t& color)
{
	if (x >= _width || x < 0) return;
	if (y >= _height || y < 0) return;
	_buffer[x + y * _width] = color;
}

void Framebuffer::DrawRect(int x, int y, int width, int height, const color_t& color)
{
	if (x >= _width || x < 0 || x + width >= _width) return;
	if (y >= _height || y < 0 || y + height >= _height) return;

	int x1 = std::max(x, 0);
	int x2 = std::min(x + width, _width);
	int y1 = std::max(y, 0);
	int y2 = std::min(y + height, _height);

	for (int sy = y1; sy < y2; sy++) 
	{
		int index = x1 + sy * _width;
			//_buffer[(x + i) + (y + j)*_width] = color;
			//_buffer[(x + i) + (y) * _width] = color;
			//_buffer[(x + i) + (y + height) * _width] = color;
			//_buffer[(x) + (y + j) * _width] = color;
			//_buffer[(x + width) + (y + j) * _width] = color;
		std::fill(_buffer.begin() + index, _buffer.begin() + (index + x2 - x1), color);
	}
}

void Framebuffer::DrawLine(int x1, int y1, int x2, int y2, const color_t& color)
{
	//calculate deltas
	int dx = x2 - x1;
	int dy = y2 - y1;

	bool steep = std::abs(dy) > std::abs(dx);
	if (steep)
	{
		std::swap(x1, y1);
		std::swap(x2, y2);
	}

	//ensure left to right drawing
	if (x1 > x2)
	{
		std::swap(x1, x2);
		std::swap(y1, y2);
	}

	//recalculate deltas
	dx = x2 - x1;
	dy = std::abs(y2 - y1);

	//set error term and y step direction
	int error = dx / 2;
	int ystep = (y1 < y2) ? 1 : -1;

	//draw line points
	for (int x = x1, y = y1; x <= x2; x++)
	{
		(steep) ? DrawPoint(y, x, color) : DrawPoint(x, y, color);
		error -= dy;

		//update error term
		if (error < 0)
		{
			y += ystep;
			error += dx;
		}
	}
}

void Framebuffer::DrawLineSlope(int x1, int y1, int x2, int y2, const color_t& color)
{
	int dx = x2 - x1;
	int dy = y2 - y1;
	float m = dy / (float)dx;
	float b = y1 - (m * x1);
	if (dx == 0)
	{
		if (y1 > y2) std::swap(y1, y2);
		for (int y = y1; y < y2; y++)
		{
			_buffer[x1 + y * _width] = color;
		}
	}
	else
	{
		if (std::abs(dx) > std::abs(dy))
		{
			for (int x = x1; x <= x2; x++)
			{
				int y = (int)round((m * x) + b);
				_buffer[x + y * _width] = color;
			}
		}
		else
		{
			for (int y = y1; y <= y2; y++)
			{
				//solve for x = y-b/m
				int x = (int)round((y - b) / m);
				_buffer[x + y * _width] = color;
			}
		}
	}
}

void Framebuffer::DrawCircle(int xc, int yc, int radius, const color_t& color)
{
	int x = 0, y = radius;
	int d = 3 - 2 * radius;
	DrawCurve(xc, yc, x, y, color);
	while (y >= x) {

		// check for decision parameter
		// and correspondingly 
		// update d, y
		if (d > 0) {
			y--;
			d = d + 4 * (x - y) + 10;
		}
		else { d = d + 4 * x + 6; }

		// Increment x after updating decision parameter
		x++;

		// Draw the circle using the new coordinates
		DrawCurve(xc, yc, x, y, color);
		//delay(50);
	}
}

void Framebuffer::DrawCurve(int xc, int yc, int x, int y, const color_t& color)
{
	DrawPoint(xc + x, yc + y, color);
	DrawPoint(xc - x, yc + y, color);
	DrawPoint(xc + x, yc - y, color);
	DrawPoint(xc - x, yc - y, color);
	DrawPoint(xc + y, yc + x, color);
	DrawPoint(xc - y, yc + x, color);
	DrawPoint(xc + y, yc - x, color);
	DrawPoint(xc - y, yc - x, color);
}

void Framebuffer::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const color_t& color)
{
	DrawLine(x1, y1, x2, y2, color);
	DrawLine(x2, y2, x3, y3, color);
	DrawLine(x3, y3, x1, y1, color);

}
