#include "Renderer.h"
#include "Framebuffer.h"
#include <iostream>

bool Renderer::Initialize()
{
    // initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

bool Renderer::CreateWindow(int sizeX, int sizeY, std::string name)
{
    // create window
    // returns pointer to window if successful or nullptr if failed
    _window = SDL_CreateWindow(name.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        sizeX, sizeY,
        SDL_WINDOW_SHOWN);
    if (_window == nullptr)
    {
        std::cerr << "Error creating SDL window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    // create renderer
    _renderer = SDL_CreateRenderer(_window, -1, 0);

    //store values
    _width = sizeX;
    _height = sizeY;
    _name = name;

    return true;
}

void Renderer::CopyFramebuffer(const Framebuffer& framebuffer)
{
    SDL_RenderCopy(_renderer, framebuffer.m_texture, NULL, NULL);
}
