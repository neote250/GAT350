#include "Scene.h"
#include "Framebuffer.h"
#include "Camera.h"
#include "Tracer.h"

void Scene::Render(Framebuffer& framebuffer, const Camera& camera)
{
	for (int y = 0; y < framebuffer._height; y++)
	{
		for (int x = 0; x < framebuffer._width; x++)
		{
			glm::vec2 pixel{ x,y };
			glm::vec2 point = pixel / glm::vec2{ framebuffer._width, framebuffer._height};
			point.y = 1 - point.y;

			ray_t ray = camera.GetRay(point);


			color3_t color = Tracer::Trace(*this, ray);
			//color4_t color = { 1,0,0,1 };

			framebuffer.DrawPoint(x, y, ColorConvert(color));
		}
	}
}
