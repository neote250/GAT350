
#include "Renderer.h"
#include "Framebuffer.h"
#include "MathUtils.h"
#include "Image.h"
#include "PostProcess.h"

#include <iostream>
#include <SDL.h>



int main(int argc, char* argv[])
{    
    Renderer renderer;
    renderer.Initialize();
    renderer.CreateWindow();

    Framebuffer framebuffer(renderer, 800, 600);
    Image image;
    image.Load("AB.jpg");

    Image imageAlpha;
    imageAlpha.Load("colors.png");
    PostProcess::Alpha(imageAlpha._buffer, 64);

    bool quit = false;

    while (!quit)
    {
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


        int mx, my;
        SDL_GetMouseState(&mx,&my);

        //float time = ticks * 0.001f;
        //float t = std::abs(std::sin(time));
        //int x, y;

        SetBlendMode(BlendMode::Normal);
        framebuffer.DrawImage(0, 0, image);
        //SetBlendMode(BlendMode::Alpha);
        //framebuffer.DrawImage(mx, my, imageAlpha);

#pragma region post_process
        //PostProcess::BoxBlur(framebuffer._buffer,framebuffer._width,framebuffer._height);
        //PostProcess::GaussianBlur(framebuffer._buffer, framebuffer._width, framebuffer._height);
        //PostProcess::Sharpen(framebuffer._buffer, framebuffer._width, framebuffer._height);
        //PostProcess::Edge(framebuffer._buffer, framebuffer._width, framebuffer._height, 10);
        //PostProcess::Emboss(framebuffer._buffer, framebuffer._width, framebuffer._height, 50);
#pragma endregion




        framebuffer.Update();

        renderer.CopyFramebuffer(framebuffer);


        // show screen
        SDL_RenderPresent(renderer._renderer);
    }

    return 0;
}