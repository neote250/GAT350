
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
#include "Shader.h"

#include <memory>
#include <iostream>
#include <SDL.h>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 


int main(int argc, char* argv[])
{
    //initialize
    Time time;

    Renderer renderer;
    renderer.Initialize();
    renderer.CreateWindow();

    Input input;
    input.Initialize();
    input.Update();

    SetBlendMode(BlendMode::Normal);

    Camera camera(renderer._width, renderer._height);
    camera.SetView(glm::vec3{ 0,0,-50 }, glm::vec3{ 0 }); //set camera a bit behind subject, set subject at center
    camera.SetProjection(90.0f, 800.0f / 600, 0.1f, 200.0f);
    Transform cameraTransform({ 0,0,-20 });


    Framebuffer framebuffer(renderer, 800, 600);
    Image image;
    //image.Load("AB.jpg");

    Image imageAlpha;
    imageAlpha.Load("colors.png");
    PostProcess::Alpha(imageAlpha._buffer, 64);
    
    // materials
    std::shared_ptr<material_t> blueMat = std::make_shared<material_t>();
    blueMat->albedo = color3_t{ 0, 0, 1 };
    blueMat->specular = color3_t{ 1 };
    blueMat->shininess = 256.0f;

    std::shared_ptr<material_t> redMat = std::make_shared<material_t>();
    redMat->albedo = color3_t{ 1, 0, 0 };
    redMat->specular = color3_t{ 100 };
    redMat->shininess = 256.0f;

    //shader
    Shader::uniforms.view = camera.GetView();
    Shader::uniforms.projection = camera.GetProjection();
    Shader::uniforms.ambient = color3_t{ 0.01f };
    Shader::uniforms.light.position = glm::vec3{ 0, 0, -10 };
    Shader::uniforms.light.direction = glm::vec3{ 0, -1, 0 }; // light pointing down
    Shader::uniforms.light.color = color3_t{ 1 }; // white light

    Shader::framebuffer = &framebuffer;

    //models
    std::shared_ptr <Model> ogreModel = std::make_shared<Model>();
    ogreModel->Load("models/cube.obj");
    //ogreModel->SetColor({ 0,0,1,1 });

    std::shared_ptr <Model> dragonModel = std::make_shared<Model>();
    dragonModel->Load("models/cube.obj");
    //dragonModel->SetColor({ 1,0,0,1 });


    //actors
    std::vector<std::unique_ptr<Actor>> actors;

    Transform ogreTransform{ glm::vec3{-5,0,0}, glm::vec3{0}, glm::vec3{10} };
    std::unique_ptr<Actor>ogreActor = std::make_unique<Actor>(ogreTransform, ogreModel, blueMat);
    //actors.push_back(std::move(ogreActor));

    Transform dragonTransform{ glm::vec3{5,0,0}, glm::vec3{0}, glm::vec3{10} };
    std::unique_ptr<Actor>dragonActor = std::make_unique<Actor>(dragonTransform, dragonModel, redMat);
    actors.push_back(std::move(dragonActor));

    bool quit = false;
    while (!quit)
    {
        time.Tick();
        input.Update();

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
            {
                quit = true;
            }
        }

        // framebuffer
        framebuffer.Clear(color_t{0,0,0,255});

        //mouse
        if (input.GetMouseButtonDown(2))
        {
            input.SetRelativeMode(true);

            glm::vec3 direction{ 0 };
            if (input.GetKeyDown(SDL_SCANCODE_RIGHT)) direction.x = 1; //Right
            if (input.GetKeyDown(SDL_SCANCODE_LEFT)) direction.x = -1; //Left
            if (input.GetKeyDown(SDL_SCANCODE_UP)) direction.y = 1; //Up
            if (input.GetKeyDown(SDL_SCANCODE_DOWN)) direction.y = -1; //Down
            if (input.GetKeyDown(SDL_SCANCODE_Z)) direction.z = 1; //Forward
            if (input.GetKeyDown(SDL_SCANCODE_X)) direction.z = -1; //Back

            cameraTransform.rotation.y += input.GetMouseRelative().x * 0.25f;
            cameraTransform.rotation.x += input.GetMouseRelative().y * 0.25f;

            glm::vec3 offset = cameraTransform.GetMatrix() * glm::vec4(direction,0);

            cameraTransform.position += direction * 70.0f * time.GetDeltaTime();
        }
        else
        {
            input.SetRelativeMode(false);
        }
        camera.SetView(cameraTransform.position, cameraTransform.position + glm::vec3{0,0,1});
        Shader::uniforms.view = camera.GetView();


        framebuffer.DrawImage(0, 0, image);

        //draw
        for (auto& actor : actors)
        {
            actor->GetTransform().rotation.y += time.GetDeltaTime() * 90;
            actor->Draw();
        }

        //update framebuffer
        framebuffer.Update();

        renderer.CopyFramebuffer(framebuffer);

        // show screen
        SDL_RenderPresent(renderer._renderer);
    }

    return 0;
}