
#include "Renderer.h"
#include "Framebuffer.h"
#include "Model.h"
#include "MathUtils.h"
#include "Image.h"
#include "PostProcess.h"
#include "Transform.h"
#include "ETime.h"
#include "Input.h"

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

    Framebuffer framebuffer(renderer, 800, 600);
    SetBlendMode(BlendMode::Normal);
    Image image;
    image.Load("AB.jpg");

    Image imageAlpha;
    imageAlpha.Load("colors.png");
    PostProcess::Alpha(imageAlpha._buffer, 64);

    vertices_t vertices{ glm::vec3{-5,5,0}, {5,5,0}, {-5,-5,0} };
    Model model(vertices, { 0,255,0,255 });

    Transform transform{ glm::vec3{240,240,0}, glm::vec3{0}, glm::vec3{3} };

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
        if (input.GetKeyDown(SDL_SCANCODE_UP)) direction.y = -1; //Up
        if (input.GetKeyDown(SDL_SCANCODE_DOWN)) direction.y = 1; //Down
        if (input.GetKeyDown(SDL_SCANCODE_Z)) direction.z = 1; //Forward
        if (input.GetKeyDown(SDL_SCANCODE_X)) direction.z = -1; //Back

        transform.rotation.z += 90 * time.GetDeltaTime();

        transform.position += direction * 70.0f * time.GetDeltaTime();

        model.Draw(framebuffer, transform.GetMatrix());

        framebuffer.Update();

        renderer.CopyFramebuffer(framebuffer);

        // show screen
        SDL_RenderPresent(renderer._renderer);
    }

    return 0;
}