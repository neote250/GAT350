#pragma once

#include "Color.h"
#include "SceneObject.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <memory>

using vertex_t = glm::vec3;
using vertices_t = std::vector<vertex_t>;

class Model : public SceneObject
{
public:
	Model(std::shared_ptr<Material> material) :
		SceneObject{ material }
	{}	
	Model(const Transform& transform, std::shared_ptr<Material> material) :
		SceneObject{ transform, material }
	{}
	Model(const vertices_t& vertices, std::shared_ptr<Material> material) :
		SceneObject{ material }, m_local_vertices{ vertices }
	{}

	void Update() override;

	bool Load(const std::string& filename);
	bool Hit(const ray_t& ray, raycastHit_t& raycastHit, float minDistance, float maxDistance) override;

private:
	vertices_t m_vertices;
	vertices_t m_local_vertices;

	glm::vec3 m_center{ 0 };
	float m_radius = 0;
};
