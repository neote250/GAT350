
#include "Renderer.h"
#include "Framebuffer.h"
#include "Model.h"
#include "MathUtils.h"
#include "Image.h"
#include "PostProcess.h"
#include "Transform.h"
#include "ETime.h"
#include "Input.h"
#include "Camera.h"
#include "Actor.h"
#include "Random.h"
#include "Tracer.h"
#include "Scene.h"
#include "Plane.h"
#include "Triangle.h"

#include <memory>
#include <iostream>
#include <SDL.h>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 


int main(int argc, char* argv[])
{
    srand((unsigned int)time(NULL));
    //initialize
    Time time;

    Renderer renderer;
    renderer.Initialize();
    renderer.CreateWindow(800, 600, "Ray Tracer");

    SetBlendMode(BlendMode::Normal);

    Framebuffer framebuffer(renderer, renderer._width, renderer._height);

    Camera camera{ 70.0f, framebuffer._width / (float)framebuffer._height };
    camera.SetView({ 0, 0, -20 }, { 0, 0, 0 });

    Scene scene;
    std::array<std::shared_ptr<Material>, 4> materials;
    std::shared_ptr<Material> gray = std::make_shared<Lambertian>(color3_t{ 0.5f });
    std::shared_ptr<Material> red = std::make_shared<Metal>(color3_t{ 1, 0, 0 }, 0.0f);
    std::shared_ptr<Material> blue = std::make_shared<Dielectric>(color3_t{ 1, 1, 1 }, 1.33f);
    std::shared_ptr<Material> purple = std::make_shared<Emissive>(color3_t{ 1, 0, 1 }, 10.0f);
    materials[0] = gray;
    materials[1] = red;
    materials[2] = blue;
    materials[3] = purple;



    for (int i = 0; i < 0; i++)
    {
        auto object = std::make_unique<Sphere>(random(glm::vec3{-30, 0, 0}, glm::vec3{30,20,50}), 5.0f, materials[random(4)]);
        scene.AddObject(std::move(object));
    }

    auto plane = std::make_unique<Plane>(glm::vec3{ 0, -5, 0 }, glm::vec3{ 0, 1, 0 }, gray);
    scene.AddObject(std::move(plane));

    //auto sphere = std::make_unique<Sphere>(glm::vec3{ 0, 0, 0 }, 2.0f, blue);
    //scene.AddObject(std::move(sphere));

    auto triangle = std::make_unique<Triangle>(glm::vec3{ 0, 0, 0 }, glm::vec3{ -10, 5, 0 }, glm::vec3{ 5, 5, 0 }, red);
    scene.AddObject(std::move(triangle));

    bool quit = false;

    // Render
    //framebuffer.Clear(ColorConvert(color4_t{ 0,255,0,255 }));
    scene.Render(framebuffer, camera, 40, 5);
    framebuffer.Update();

    while (!quit)
    {
        time.Tick();

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) quit = true;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) quit = true;
        }




        // show screen
        renderer.CopyFramebuffer(framebuffer);
        SDL_RenderPresent(renderer._renderer);
    }

    return 0;
}