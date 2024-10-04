
#include "Renderer.h"
#include "Framebuffer.h"

#include <iostream>
#include <SDL.h>



int main(int argc, char* argv[])
{    
    Renderer renderer;
    renderer.Initialize();
    renderer.CreateWindow();

    Framebuffer framebuffer(renderer, 200, 150);

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
            int x = rand() % framebuffer._width;
            int x2 = rand() % framebuffer._width;
            int y = rand() % framebuffer._height;
            int y2 = rand() % framebuffer._height;

            framebuffer.DrawPoint(x, y, {255,255,255,255});
            //framebuffer.DrawLine(x, y, x2, y2, { (uint8_t)(rand() % 255),(uint8_t)(rand() % 255),(uint8_t)(rand() % 255),255 });
        }

        framebuffer.DrawRect(50, 10, 100, 100, { 0,255,0,255 });
        framebuffer.DrawTriangle(50, 50, 100, 100, 75, 12, { 0,255,0,255 });
        framebuffer.Update();

        renderer.CopyFramebuffer(framebuffer);


        // show screen
        SDL_RenderPresent(renderer._renderer);
    }

    return 0;
}