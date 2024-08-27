#include <stdio.h>
#include <stdbool.h>

// Disable warnings for SDL
#pragma warning(push, 0)
#define SDL_MAIN_HANDLED 1
#include <SDL.h>
#pragma warning(pop)

// Globals
SDL_Window* g_window;
SDL_Renderer* g_renderer;
bool g_isRunning = false;

bool InitializeWindow(void)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "Error initializing SDL\n");
        return false;
    }

    g_window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600,
        SDL_WINDOW_BORDERLESS
    );
    if (!g_window)
    {
        fprintf(stderr, "Error creating SDL window.\n");
        return false;
    }

    g_renderer = SDL_CreateRenderer(g_window, -1, 0);
    if (!g_renderer)
    {
        fprintf(stderr, "Error creating SDL renderer.\n");
        return false;
    }

    return true;
}

void Setup(void)
{
    // TODO
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

    // ...

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

    return 0;
}