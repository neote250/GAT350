#include "Image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>


bool Image::Load(const std::string& filename)
{
	// load image getting the width and height (pass 4 for the last parameter to create RGBA image)
	int channels;
	uint8_t* data = stbi_load(filename.c_str(), &_width, &_height, &channels,4);
	if (!data)
	{
		std::cerr << "Error loading image: " << filename << std::endl;
		return false;
	}

	// resize the image buffer to the number of pixels in the image
	_buffer.resize(_width * _height);

	// copy the image data into the buffer, !!takes in number of bytes to copy!! 
	std::memcpy(_buffer.data(), data, _width * _height * sizeof(color_t)); // https://en.cppreference.com/w/cpp/string/byte/memcpy

	// free the image data
	stbi_image_free(data);

	return true;
}
