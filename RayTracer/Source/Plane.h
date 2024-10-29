#pragma once

#include "SceneObject.h"

class Plane : public SceneObject
{
public:
	Plane() = default;
	Plane(const glm::vec3& center, const glm::vec3& normal, std::shared_ptr<Material> material) :
		SceneObject{ material }
		, _center{ center }
		, _normal{ normal }
	{}

	bool Hit(const ray_t& ray, raycastHit_t& raycastHit, float minDistance, float maxDistance) override;

private:
	glm::vec3 _center{};
	glm::vec3 _normal{};
};