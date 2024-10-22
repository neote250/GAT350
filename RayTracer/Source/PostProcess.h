#pragma once

#include "Framebuffer.h"

namespace PostProcess
{
	void Invert(std::vector<color_t>& buffer);
	void Monochrome(std::vector<color_t>& buffer);
	void Brightness(std::vector<color_t>& buffer, int brightness);
	void ColorBalance(std::vector<color_t>& buffer, int ro, int go, int bo);
	void Noise(std::vector<color_t>& buffer, uint8_t noise);
	void Thresholding(std::vector<color_t>& buffer, uint8_t threshold);
	void Posterization(std::vector<color_t>& buffer, uint8_t levels);
	void Alpha(std::vector<color_t>& buffer, uint8_t alpha);

	void BoxBlur(std::vector<color_t>& buffer, int width, int height);
	void GaussianBlur(std::vector<color_t>& buffer, int width, int height);
	void Sharpen(std::vector<color_t>& buffer, int width, int height);
	void Edge(std::vector<color_t>& buffer, int width, int height, int threshold);
	void Emboss(std::vector<color_t>& buffer, int width, int height, int threshold);
}
