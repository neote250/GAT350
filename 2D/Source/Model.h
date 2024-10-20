#pragma once

#include "Color.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>

using vertex_t = glm::vec3;
using vertices_t = std::vector<vertex_t>;

class Model
{
public:
	Model() = default;
	Model(const vertices_t& vertices, const color_t& color) : 
		_vertices{ vertices }, _color{ color } 
	{}

	void Draw(class Framebuffer& framebuffer, const glm::mat4& model, const class Camera& camera);
	bool Load(const std::string& filename);

private:
	vertices_t _vertices;
	color_t _color;
};
