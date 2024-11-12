
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

void InitScene(Scene& scene);
void InitScene01(Scene& scene, Camera& camera);


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
    camera.SetView({ 0,5, -20 }, { 0, 0, 0 });

    Scene scene;

    //InitScene(scene);
    InitScene01(scene, camera);


    // Render
    //framebuffer.Clear(ColorConvert(color4_t{ 0,255,0,255 }));
    scene.Update();
    scene.Render(framebuffer, camera, 80, 6);
    framebuffer.Update();

    bool quit = false;
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

void InitScene(Scene& scene)
{
    //scene.SetSky(HSVtoRGB(240, 1, 0.2f), HSVtoRGB(180, 0.7f, 1.0f));



    std::shared_ptr<Material> lGray = std::make_shared<Lambertian>(color3_t{ 0.5f });
    std::shared_ptr<Material> lPurple = std::make_shared<Lambertian>(color3_t{ 1, 0, 1 });
    std::shared_ptr<Material> lRed = std::make_shared<Lambertian>(color3_t{ 1, 0, 0 });
    std::shared_ptr<Material> lGreen = std::make_shared<Lambertian>(color3_t{ 0, 1, 0 });
    std::shared_ptr<Material> lWhite = std::make_shared<Lambertian>(color3_t{ 1, 1, 1 });


    std::shared_ptr<Material> red = std::make_shared<Metal>(color3_t{ 1, 0, 0 }, 0.0f);

    std::shared_ptr<Material> blue = std::make_shared<Dielectric>(color3_t{ 0, 0, 1 }, 1.33f);

    std::shared_ptr<Material> purple = std::make_shared<Emissive>(color3_t{ 1, 0, 1 }, 10.0f);
    std::shared_ptr<Material> eWhite = std::make_shared<Emissive>(color3_t{ 1, 1, 1 }, 10.0f);


    std::array<std::shared_ptr<Material>, 5> materials;
    materials[0] = lGray;
    materials[1] = red;
    materials[2] = blue;
    materials[3] = purple;
    materials[4] = lPurple;


    auto object = std::make_unique<Sphere>(Transform{ glm::vec3{5, 0, 0}, glm::vec3{0,0,0} }, 2.0f, lWhite);
    scene.AddObject(std::move(object));

    std::unique_ptr<Model> cube = std::make_unique<Model>(
        Transform{ glm::vec3{-5, 0, 0}, glm::vec3{0,0,0}, glm::vec3{4} }, 
        lWhite);

    cube->Load("Models/cube.obj");
    scene.AddObject(std::move(cube));
    
    //transform position(x,y,z)
    //transform rotation(along the x, along the y, along the z)
    auto floor = std::make_unique<Plane>(Transform{ glm::vec3{0,-2,0}, glm::vec3{0, 0, 0} }, lWhite);
    scene.AddObject(std::move(floor));   
    auto ceiling = std::make_unique<Plane>(Transform{ glm::vec3{0,13,0}, glm::vec3{-5, 0, 0} }, lWhite);
    scene.AddObject(std::move(ceiling));

    auto backWall = std::make_unique<Plane>(Transform{ glm::vec3{0,0,15}, glm::vec3{-90, 0, 0} }, lWhite);
    scene.AddObject(std::move(backWall));

    auto leftWall = std::make_unique<Plane>(Transform{ glm::vec3{-11,0,0}, glm::vec3{0, 3, 85} }, lRed);
    scene.AddObject(std::move(leftWall));
    auto rightWall = std::make_unique<Plane>(Transform{ glm::vec3{11,0,0}, glm::vec3{0, -3, -85} }, lGreen);
    scene.AddObject(std::move(rightWall));

    //auto sphere = std::make_unique<Sphere>(glm::vec3{ 0, 0, 0 }, 2.0f, blue);
    //scene.AddObject(std::move(sphere));

    //auto triangle = std::make_unique<Triangle>(glm::vec3{ 0, 0, 0 }, glm::vec3{ -10, 5, 0 }, glm::vec3{ 5, 5, 0 }, red);
    //scene.AddObject(std::move(triangle));

    color3_t color = HSVtoRGB(randomf(0,360), 1.0f, 1.0f);
    std::unique_ptr<Model> model = std::make_unique<Model>(
        Transform{ glm::vec3{0, 14, 0}, glm::vec3{0,0,0}, glm::vec3{4} }
        , eWhite
    );

    model->Load("Models/cube.obj");
    scene.AddObject(std::move(model));
}

void InitScene01(Scene& scene, Camera& camera)
{
    camera.SetFOV(20.0f);
    camera.SetView({ 13, 2, 3 }, { 0, 0, 0 });

    auto ground_material = std::make_shared<Lambertian>(color3_t(0.5f));
    scene.AddObject(std::make_unique<Plane>(Transform{ glm::vec3{ 0 } }, ground_material));

    std::shared_ptr<Material> purple = std::make_shared<Emissive>(color3_t{ 1, 0, 1 }, 10.0f);
    std::shared_ptr<Material> red = std::make_shared<Metal>(color3_t{ 1, 0, 0 }, 0.0f);
    std::unique_ptr<Model> teapot = std::make_unique<Model>(Transform{ glm::vec3{-5, 0, 0}, glm::vec3{0,0,0}, glm::vec3{1} }, red);
    teapot->Load("Models/teapot.obj");
    scene.AddObject(std::move(teapot));

    auto triangle = std::make_unique<Triangle>(glm::vec3{ 0, -2, 5 }, glm::vec3{ -10, 3, 5 }, glm::vec3{ 5, 3, 5 }, purple);
    scene.AddObject(std::move(triangle));

    auto triangle2 = std::make_unique<Triangle>(glm::vec3{ -10, 5, 5 }, glm::vec3{ -20, 10, 5 }, glm::vec3{ -5, 10, 5 }, purple);
    scene.AddObject(std::move(triangle2));

    auto triangle3 = std::make_unique<Triangle>(glm::vec3{ 10, 0, 5 }, glm::vec3{ 0, 5, 5 }, glm::vec3{ 15, 5, 5 }, purple);
    scene.AddObject(std::move(triangle3));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = randomf();
            glm::vec3 center(a + 0.9 * randomf(), 0.2, b + 0.9 * randomf());

            if ((center - glm::vec3(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = HSVtoRGB(randomf(0, 360), 1.0f, 1.0f);
                    sphere_material = std::make_shared<Lambertian>(albedo);
                    scene.AddObject(std::make_unique<Sphere>(Transform{ center }, 0.2f, sphere_material));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = HSVtoRGB(randomf(0, 360), 1.0f, 1.0f);
                    auto fuzz = randomf(0.0f, 0.5f);
                    sphere_material = std::make_shared<Metal>(albedo, fuzz);
                    scene.AddObject(std::make_unique<Sphere>(Transform{ center }, 0.2f, sphere_material));
                }
                else {
                    // glass
                    sphere_material = std::make_shared<Dielectric>(color3_t{ 1 }, 1.5f);
                    scene.AddObject(std::make_unique<Sphere>(Transform{ center }, 0.2f, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(color3_t{ 1 }, 1.5f);
    scene.AddObject(std::make_unique<Sphere>(Transform{ glm::vec3{ 0, 1, 0 } }, 1.0f, material1));

    auto material2 = std::make_shared<Lambertian>(color3_t(0.4f, 0.2f, 0.1f));
    scene.AddObject(std::make_unique<Sphere>(Transform{ glm::vec3{ -4, 1, 0 } }, 1.0f, material2));

    auto material3 = std::make_shared<Metal>(color3_t(0.7f, 0.6f, 0.5f), 0.0f);
    scene.AddObject(std::make_unique<Sphere>(Transform{ glm::vec3{ 4, 1, 0 } }, 1.0f, material3));
}