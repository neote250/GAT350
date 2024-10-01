#include <iostream>
#include <SDL.h>
#include "Renderer.h"


int main(int argc, char* argv[])
{    
    Renderer renderer;
    renderer.Initialize();
    renderer.CreateWindow();


    return 0;
}