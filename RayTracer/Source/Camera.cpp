#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

void Camera::SetView(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up) //target or center
{
	_view = glm::lookAt(eye, target, up);
}

void Camera::SetProjection(float fieldOfView, float aspectRatio, float nearClipping, float farClipping)
{
	_projection = glm::perspective(glm::radians(fieldOfView), aspectRatio, nearClipping, farClipping);
}

glm::vec3 Camera::ModelToView(const glm::vec3& position) const
{
	//convert point from world space to view space
	return _view * glm::vec4{ position, 1 };
}

glm::vec4 Camera::ViewToProjection(const glm::vec3& position) const
{
	//convert point from view space to projection space
	return _projection * glm::vec4{ position, 1 };
}

glm::ivec2 Camera::ViewToScreen(const glm::vec3& position) const
{
	glm::vec4 clip = ViewToProjection(position);

	//prevent / 0
	if (clip.w == 0) return glm::ivec2{ -1,-1 };

	//convert projection space to ndc (-1 <-> 1)
	glm::vec3 ndc = clip / clip.w;

	//don't draw if outside near and far
	if (ndc.z < -1 || ndc.z > 1) return glm::ivec2{ -1, -1 };

	float x = (ndc.x + 1) * (_width * 0.5f);
	float y = (1 - ndc.y) * (_height * 0.5f);

	return glm::ivec2(x, y);
}
