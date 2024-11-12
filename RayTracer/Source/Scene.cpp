#include "Scene.h"
#include "Framebuffer.h"
#include "Camera.h"
#include "Tracer.h"
#include "Random.h"
#include "ETime.h"

#include <iostream>

void Scene::Update()
{
	for (auto& object : m_objects)
	{
		object->Update();
	}
}

void Scene::Render(Framebuffer& framebuffer, const Camera& camera, int numSamples, int depth)
{
	Time frameTimer;
	Time scanlineTimer;
	for (int y = 0; y < framebuffer._height; y++)
	{
		scanlineTimer.Reset();
		for (int x = 0; x < framebuffer._width; x++)
		{
			color3_t color{ 0 };
			for (int i = 0; i < numSamples; i++)
			{
				glm::vec2 pixel{ x,y };
				pixel += glm::vec2{ randomf(), randomf() };
				glm::vec2 point = pixel / glm::vec2{ framebuffer._width, framebuffer._height};
				point.y = 1 - point.y;

				ray_t ray = camera.GetRay(point);

				color += Tracer::Trace(*this, ray, 0.001f, 100.0f, depth);
			}
			color /= (float)numSamples;
			framebuffer.DrawPoint(x, y, ColorConvert(color));
		}
		std::cout << "y: " << y << " : " << scanlineTimer.GetElapsedTime() << std::endl;
	}
	std::cout << "frame time: " << frameTimer.GetElapsedTime() << std::endl;
}
