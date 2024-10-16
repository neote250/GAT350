#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/glm.hpp>

struct Transform
{
	glm::vec3 position { 0 };
	glm::vec3 rotation { 0 };
	glm::vec3 scale { 1 };
	
	Transform() = default;
	Transform(const glm::vec3& position
		, const glm::vec3& rotation = glm::vec3{ 0 }
		, const glm::vec3& scale = glm::vec3{1}) :
		position{position}
		, rotation { rotation }
		, scale{ scale } 
	{}

	glm::mat4 GetMatrix()
	{
		glm::mat4 mxScale = glm::scale(scale);
		glm::mat4 mxTranslate = glm::translate(position);
		glm::mat4 mxRotation = glm::eulerAngleYXZ(glm::radians(rotation.y), glm::radians(rotation.x), glm::radians(rotation.z));

		return mxTranslate * mxScale * mxRotation;
	}
};