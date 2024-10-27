#include "Camera.h"
#include "MathUtils.h"
#include <glm/gtc/matrix_transform.hpp>

void Camera::SetView(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up) //target or center
{
	m_eye = eye;
	//create camera axis
	m_forward = glm::normalize(target - eye);
	m_right = glm::normalize(Cross(up, m_forward));
	m_up = Cross(m_forward, m_right);

	CalculateViewPlane();
}

ray_t Camera::GetRay(const glm::vec2& point) const
{
	ray_t ray;

	ray.origin = m_eye;
	//subtract head from tail. Where we are going, to where we are.
	ray.direction = (m_lowerLeft + (m_horizontal * point.x) + (m_vertical * point.y)) - m_eye; 

	return ray;
}

void Camera::CalculateViewPlane()
{
	float theta = glm::radians(m_fov);

	float halfHeight = glm::tan(theta * 0.5f);
	float height = halfHeight * 2;
	float width = height * m_aspectRatio;

	m_horizontal = m_right * width;
	m_vertical = m_up * height;

	m_lowerLeft = m_eye - (m_horizontal * 0.5f) - (m_vertical * 0.5f) + m_forward;
}
