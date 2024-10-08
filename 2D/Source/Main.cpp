
#include "Renderer.h"
#include "Framebuffer.h"
#include "MathUtils.h"
#include "Image.h"
#include <iostream>
#include <SDL.h>



int main(int argc, char* argv[])
{    
    Renderer renderer;
    renderer.Initialize();
    renderer.CreateWindow();

    Framebuffer framebuffer(renderer, 800, 600);

    Image image;
    image.Load("enemy.png");

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
        // clear screen
        //SDL_SetRenderDrawColor(renderer._renderer, 0, 0, 0, 0);
        //SDL_RenderClear(renderer._renderer);

        // framebuffer
        framebuffer.Clear(color_t{0,0,0,255});

        for (int i = 0; i < 100; i++)
        {
            int x = rand() % framebuffer._width/2;
            //int x2 = rand() % framebuffer._width;
            int y = rand() % framebuffer._height/2;
            //int y2 = rand() % framebuffer._height;

            framebuffer.DrawImage(x, y, image);


            //framebuffer.DrawPoint(x, y, {255,255,255,255});
        }

        //framebuffer.DrawLine(125, 75, 75, 25, { (uint8_t)(rand() % 255),(uint8_t)(rand() % 255),(uint8_t)(rand() % 255),255 });
        //framebuffer.DrawRect(50, 10, 100, 100, { 0,255,0,255 });
        //framebuffer.DrawTriangle(50, 50, 100, 100, 75, 12, { 0,255,0,255 });
        //framebuffer.DrawCircle(100, 50, 25, { 150,150,12,255 });
        //int mx, my;
        //SDL_GetMouseState(&mx,&my);

        //framebuffer.DrawLinearCurve(100, 100, 200, 200, { 255,255,0,255 });
        //framebuffer.DrawQuadraticCurve(100, 200, mx, my, 300, 200, { 255,255,0,255 });
        //framebuffer.DrawCubicCurve(300, 500, 300, 100, mx, my, 600, 500, { 255,0,0,255 });

        //int ticks = SDL_GetTicks();
        //float time = ticks * 0.001f;
        //float t = std::abs(std::sin(time));
        //int x, y;
        //CubicPoint(300, 400, 300, 100, mx, my, 600, 400, t, x, y);
        //framebuffer.DrawRect(x-20, y-20, 40, 40, { 0,255,0,255 });





        framebuffer.Update();

        renderer.CopyFramebuffer(framebuffer);


        // show screen
        SDL_RenderPresent(renderer._renderer);
    }

    return 0;
}