
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
    image.Load("AB.jpg");

    Image imageAlpha;
    imageAlpha.Load("colors.png");
    PostProcess::Alpha(imageAlpha._buffer, 64);
    
    //models
    std::shared_ptr <Model> model = std::make_shared<Model>();
    model->Load("sphere.obj");
    model->SetColor({ 0,255,0,255 });

    std::vector<std::unique_ptr<Actor>> actors;
    for (int i = 0; i < 20; i++)
    {
        Transform transform{ {randomf(-10.0f,10.0f), randomf(-10.0f,10.0f), randomf(-10.0f,10.0f)}, glm::vec3{0, 0, 0}, glm::vec3{2} };
        std::unique_ptr<Actor>actor = std::make_unique<Actor>(transform, model);
        actor->SetColor({ (uint8_t)random(256),(uint8_t)random(256), (uint8_t)random(256), 255 });
        actors.push_back(std::move(actor));
    }

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

        //draw
        for (auto& actor : actors)
        {
            actor->Draw(framebuffer, camera);
        }

        //update framebuffer
        framebuffer.Update();

        renderer.CopyFramebuffer(framebuffer);

        // show screen
        SDL_RenderPresent(renderer._renderer);
    }

    return 0;
}