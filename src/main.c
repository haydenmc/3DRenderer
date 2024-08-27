#include "pch.h"
#include "Display.h"

// Globals
bool g_isRunning = false;

void Setup(void)
{
    g_colorBuffer = malloc(sizeof(uint32_t) * g_windowWidth * g_windowHeight);
    g_colorBufferTexture = SDL_CreateTexture(
        g_renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        g_windowWidth,
        g_windowHeight
    );
}

void ProcessInput(void)
{
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type)
    {
    case SDL_QUIT:
        g_isRunning = false;
        break;
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE)
        {
            g_isRunning = false;
        }
        break;
    }
}

void Update(void)
{
    // TODO
}

void Render(void)
{
    SDL_SetRenderDrawColor(g_renderer, 255, 0, 0, 255);
    SDL_RenderClear(g_renderer);

    DrawGrid(10, 0xFF333333);
    DrawRect(30, 30, 300, 200, 0xFFFF0000);
    RenderColorBuffer();
    ClearColorBuffer(0xFF000000);

    SDL_RenderPresent(g_renderer);
}

int main(void)
{
    g_isRunning = InitializeWindow();

    Setup();

    while (g_isRunning)
    {
        ProcessInput();
        Update();
        Render();
    }

    DestroyWindow();

    return 0;
}