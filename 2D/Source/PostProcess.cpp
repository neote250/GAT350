#include "PostProcess.h"
#include "MathUtils.h"
#include <algorithm>
#include <cmath>

namespace PostProcess
{
	void Invert(std::vector<color_t>& buffer)
	{
		std::for_each(buffer.begin(), buffer.end(), [](color_t& color)
			{
				color.r = 255 - color.r;
				color.g = 255 - color.g;
				color.b = 255 - color.b;
			});
	}

	void Monochrome(std::vector<color_t>& buffer)
	{
		std::for_each(buffer.begin(), buffer.end(), [](color_t& color)
			{
				uint8_t average = static_cast<uint8_t>((color.r + color.g + color.b) / 3);

				color.r = average;
				color.g = average;
				color.b = average;
			});
	}
	void Brightness(std::vector<color_t>& buffer, int brightness)
	{
		std::for_each(buffer.begin(), buffer.end(), [brightness](color_t& color)
			{
				color.r = static_cast<uint8_t> (Clamp(color.r + brightness, 0, 255));
				color.g = static_cast<uint8_t> (Clamp(color.g + brightness, 0, 255));
				color.b = static_cast<uint8_t> (Clamp(color.b + brightness, 0, 255));
			});
	}
	void ColorBalance(std::vector<color_t>& buffer, int ro, int go, int bo)
	{
		std::for_each(buffer.begin(), buffer.end(), [ro, go, bo](color_t& color)
			{
				color.r = static_cast<uint8_t> (Clamp(color.r + ro, 0, 255));
				color.g = static_cast<uint8_t> (Clamp(color.g + go, 0, 255));
				color.b = static_cast<uint8_t> (Clamp(color.b + bo, 0, 255));
			});
	}
	void Noise(std::vector<color_t>& buffer, uint8_t noise)
	{
		std::for_each(buffer.begin(), buffer.end(), [noise](color_t& color)
			{
				int random = std::rand() % noise;
				int offset = (rand() % ((noise << 1) + 1)) - noise;
				color.r = static_cast<uint8_t> (Clamp(color.r + offset, 0, 255));
				color.g = static_cast<uint8_t> (Clamp(color.g + offset, 0, 255));
				color.b = static_cast<uint8_t> (Clamp(color.b + offset, 0, 255));
			});
	}
	void Thresholding(std::vector<color_t>& buffer, uint8_t threshold)
	{
		std::for_each(buffer.begin(), buffer.end(), [threshold](color_t& color)
			{
				uint8_t average = static_cast<uint8_t>((color.r + color.g + color.b) / 3);

				if (average > threshold)
				{
					color.r = 255;
					color.g = 255;
					color.b = 255;
				}
				else
				{
					color.r = 0;
					color.g = 0;
					color.b = 0;
				}

			});
	}
	void Posterization(std::vector<color_t>& buffer, uint8_t levels)
	{
		uint8_t level = 255 / levels; // calculate the number of colors per level

		std::for_each(buffer.begin(), buffer.end(), [level](color_t& color)
			{

				color.r = (color.r / level) * level;
				color.g = (color.g / level) * level;
				color.b = (color.b / level) * level;
			});
	}

	void Alpha(std::vector<color_t>& buffer, uint8_t alpha)
	{
		std::for_each(buffer.begin(), buffer.end(), [alpha](color_t& color)
			{
				color.a = alpha;
			});
	}


	void BoxBlur(std::vector<color_t>& buffer, int width, int height)
	{
		std::vector<color_t> source = buffer;
		int kernel[3][3] =
		{
			{1,1,1},
			{1,1,1},
			{1,1,1}
		};

		for (int i = 0; i < buffer.size(); i++)
		{
			int x = i % width;
			int y = i / width;

			//skip out of range
			if (x < 1 || x + 1 >= width) continue;
			if (y < 1 || y + 1 >= height) continue;

			int r = 0;
			int g = 0;
			int b = 0;

			for (int iy = 0; iy < 3; iy++)
			{
				for (int ix = 0; ix < 3; ix++)
				{
					color_t pixel = source[(x + ix - 1) + ((y + iy - 1) * width)];
					int weight = kernel[iy][ix];

					r += pixel.r * weight;
					g += pixel.g * weight;
					b += pixel.b * weight;
				}
			}
			buffer[i].r = static_cast<uint8_t>(r / 9);
			buffer[i].g = static_cast<uint8_t>(g / 9);
			buffer[i].b = static_cast<uint8_t>(b / 9);
		}
	}
	void GaussianBlur(std::vector<color_t>& buffer, int width, int height)
	{
		std::vector<color_t> source = buffer;
		int kernel[3][3] =
		{
			{1,2,1},
			{2,4,2},
			{1,2,1}
		};

		for (int i = 0; i < buffer.size(); i++)
		{
			int x = i % width;
			int y = i / width;

			//skip out of range
			if (x < 1 || x + 1 >= width) continue;
			if (y < 1 || y + 1 >= height) continue;

			int r = 0;
			int g = 0;
			int b = 0;

			for (int iy = 0; iy < 3; iy++)
			{
				for (int ix = 0; ix < 3; ix++)
				{
					color_t pixel = source[(x + ix - 1) + ((y + iy - 1) * width)];
					int weight = kernel[iy][ix];

					r += pixel.r * weight;
					g += pixel.g * weight;
					b += pixel.b * weight;
				}
			}
			buffer[i].r = static_cast<uint8_t>(r / 16);
			buffer[i].g = static_cast<uint8_t>(g / 16);
			buffer[i].b = static_cast<uint8_t>(b / 16);
		}
	}
	void Sharpen(std::vector<color_t>& buffer, int width, int height)
	{
		std::vector<color_t> source = buffer;
		int kernel[3][3] =
		{
			{0,-1,0},
			{-1,5,-1},
			{0,-1,0}
		};

		for (int i = 0; i < buffer.size(); i++)
		{
			int x = i % width;
			int y = i / width;

			//skip out of range
			if (x < 1 || x + 1 >= width) continue;
			if (y < 1 || y + 1 >= height) continue;

			int r = 0;
			int g = 0;
			int b = 0;

			for (int iy = 0; iy < 3; iy++)
			{
				for (int ix = 0; ix < 3; ix++)
				{
					color_t pixel = source[(x + ix - 1) + ((y + iy - 1) * width)];
					int weight = kernel[iy][ix];

					r += pixel.r * weight;
					g += pixel.g * weight;
					b += pixel.b * weight;
				}
			}
			buffer[i].r = static_cast<uint8_t>(Clamp(r, 0, 255));
			buffer[i].g = static_cast<uint8_t>(Clamp(g, 0, 255));
			buffer[i].b = static_cast<uint8_t>(Clamp(b, 0, 255));
		}
	}
	void Edge(std::vector<color_t>& buffer, int width, int height, int threshold)
	{
		std::vector<color_t> source = buffer;
		int hKernel[3][3] =
		{
			{1,0,-1},
			{2,0,-2},
			{1,0,-1}
		};
		int vKernel[3][3] =
		{
			{-1,-2,-1},
			{0,0,0},
			{1,2,1}
		};

		for (int i = 0; i < buffer.size(); i++)
		{
			int x = i % width;
			int y = i / width;

			//skip out of range
			if (x < 1 || x + 1 >= width) continue;
			if (y < 1 || y + 1 >= height) continue;

			int h = 0;
			int v = 0;
			

			for (int iy = 0; iy < 3; iy++)
			{
				for (int ix = 0; ix < 3; ix++)
				{
					color_t pixel = source[(x + ix - 1) + ((y + iy - 1) * width)];
					h += pixel.r * hKernel[iy][ix];
					v += pixel.r * vKernel[iy][ix];
				}
			}

			int magnitude = std::sqrt((h * h) + (v * v));
			magnitude = (magnitude >= threshold) ? magnitude : 0;

			uint8_t color = Clamp(magnitude, 0, 255);

			buffer[i].r = color;
			buffer[i].g = color;
			buffer[i].b = color;
		}
	}
	void Emboss(std::vector<color_t>& buffer, int width, int height, int threshold)
	{
		std::vector<color_t> source = buffer;
		int kernel[3][3] =
		{
			{-2,-1,0},
			{-1,1,1},
			{0,1,2}
		};

		int min = (threshold > 128) ? 0 : 128 - threshold;
		int max = (threshold > 127) ? 255 : 128 + threshold;

		for (int i = 0; i < buffer.size(); i++)
		{
			int x = i % width;
			int y = i / width;

			//skip out of range
			if (x < 1 || x + 1 >= width) continue;
			if (y < 1 || y + 1 >= height) continue;

			int r = 0;
			int g = 0;
			int b = 0;

			for (int iy = 0; iy < 3; iy++)
			{
				for (int ix = 0; ix < 3; ix++)
				{
					color_t pixel = source[(x + ix - 1) + ((y + iy - 1) * width)];
					int weight = kernel[iy][ix];
					int avg = (pixel.r + pixel.g + pixel.b) / 3;
					//r += pixel.r * weight;
					//g += pixel.g * weight;
					//b += pixel.b * weight;
					r += avg * weight;
					g += avg * weight;
					b += avg * weight;
				}
			}

			buffer[i].r = static_cast<uint8_t>(Clamp(r, min, max));
			buffer[i].g = static_cast<uint8_t>(Clamp(g, min, max));
			buffer[i].b = static_cast<uint8_t>(Clamp(b, min, max));
		}
	}
}

