#pragma once

#include "Ray.h"
#include "Material.h"
#include <memory>

class SceneObject
{
public:
	SceneObject() = default;
	SceneObject(std::shared_ptr<Material> material) : m_material{ material } {}

	virtual bool Hit(const ray_t& ray, raycastHit_t& raycastHit, float minDistance, float maxDistance) = 0;

	std::weak_ptr<Material> GetMaterial() { return m_material; }

protected:
	std::shared_ptr<Material> m_material;
};