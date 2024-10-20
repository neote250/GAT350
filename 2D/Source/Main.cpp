
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


#include <iostream>
#include <SDL.h>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 


int main(int argc, char* argv[])
{
    //initialize
    Time time;
    Input input;
    input.Initialize();

    Renderer renderer;
    renderer.Initialize();
    renderer.CreateWindow();

    Camera camera(renderer._width, renderer._height);
    camera.SetView(glm::vec3{ 0,0,-50 }, glm::vec3{ 0 }); //set camera a bit behind subject, set subject at center
    camera.SetProjection(90.0f, 800.0f / 600, 0.1f, 200.0f);
    Transform cameraTransform({ 0,0,-20 });


    Framebuffer framebuffer(renderer, 800, 600);
    SetBlendMode(BlendMode::Normal);
    Image image;
    image.Load("AB.jpg");

    Image imageAlpha;
    imageAlpha.Load("colors.png");
    PostProcess::Alpha(imageAlpha._buffer, 64);

    vertices_t vertices{ glm::vec3{-5,5,0}, {5,5,0}, {-5,-5,0} };
    Model model(vertices, { 0,255,0,255 });

    Model sphere{ {}, { 255,0,255,255 } };
    sphere.Load("sphere.obj");
    

    Transform transform{ glm::vec3{0,0,0}, glm::vec3{0, 0, 45}, glm::vec3{3} };

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

#pragma region mouse
        int mx, my;
        SDL_GetMouseState(&mx, &my);

#pragma endregion
#pragma region alpha_blend
        //SetBlendMode(BlendMode::Normal);
        //framebuffer.DrawImage(0, 0, image);
        //SetBlendMode(BlendMode::Multiply);
        //framebuffer.DrawImage(mx, my, imageAlpha);
#pragma endregion
#pragma region post_process
        //PostProcess::BoxBlur(framebuffer._buffer,framebuffer._width,framebuffer._height);
        //PostProcess::GaussianBlur(framebuffer._buffer, framebuffer._width, framebuffer._height);
        //PostProcess::Sharpen(framebuffer._buffer, framebuffer._width, framebuffer._height);
        //PostProcess::Edge(framebuffer._buffer, framebuffer._width, framebuffer._height, 10);
        //PostProcess::Emboss(framebuffer._buffer, framebuffer._width, framebuffer._height, 50);
#pragma endregion

        glm::vec3 direction{ 0 };
        if (input.GetKeyDown(SDL_SCANCODE_RIGHT)) direction.x = 1; //Right
        if (input.GetKeyDown(SDL_SCANCODE_LEFT)) direction.x = -1; //Left
        if (input.GetKeyDown(SDL_SCANCODE_UP)) direction.y = 1; //Up
        if (input.GetKeyDown(SDL_SCANCODE_DOWN)) direction.y = -1; //Down
        if (input.GetKeyDown(SDL_SCANCODE_Z)) direction.z = 1; //Forward
        if (input.GetKeyDown(SDL_SCANCODE_X)) direction.z = -1; //Back

        cameraTransform.position += direction * 70.0f * time.GetDeltaTime();
        camera.SetView(cameraTransform.position, cameraTransform.position + glm::vec3{0,0,1});

        transform.rotation.z += 90 * time.GetDeltaTime();


        model.Draw(framebuffer, transform.GetMatrix(), camera);

        sphere.Draw(framebuffer, transform.GetMatrix(), camera);

        framebuffer.Update();

        renderer.CopyFramebuffer(framebuffer);

        // show screen
        SDL_RenderPresent(renderer._renderer);
    }

    return 0;
}