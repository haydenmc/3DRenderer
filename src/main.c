#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// Disable warnings for SDL
#pragma warning(push, 0)
#define SDL_MAIN_HANDLED 1
#include <SDL.h>
#pragma warning(pop)

// Globals
int g_windowWidth = 800;
int g_windowHeight = 600;
bool g_isRunning = false;
SDL_Window* g_window = NULL;
SDL_Renderer* g_renderer = NULL;
uint32_t* g_colorBuffer = NULL;
SDL_Texture* g_colorBufferTexture = NULL;

bool InitializeWindow(void)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "Error initializing SDL\n");
        return false;
    }

    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    g_windowWidth = displayMode.w;
    g_windowHeight = displayMode.h;

    g_window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        g_windowWidth,
        g_windowHeight,
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

    SDL_SetWindowFullscreen(g_window, SDL_WINDOW_FULLSCREEN_DESKTOP);

    return true;
}

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

void RenderColorBuffer(void)
{
    SDL_UpdateTexture(
        g_colorBufferTexture,
        NULL,
        g_colorBuffer,
        (g_windowWidth * sizeof(uint32_t))
    );
    SDL_RenderCopy(
        g_renderer,
        g_colorBufferTexture,
        NULL,
        NULL
    );
}

void ClearColorBuffer(uint32_t color)
{
    for (int y = 0; y < g_windowHeight; ++y)
    {
        for (int x = 0; x < g_windowWidth; ++x)
        {
            g_colorBuffer[(g_windowWidth * y) + x] = color;
        }
    }
}

void DrawGrid(void)
{
    uint32_t gridSize = 10;
    uint32_t gridColor = 0xFF333333;

    for (int y = 0; y < g_windowHeight; y += gridSize)
    {
        for (int x = 0; x < g_windowWidth; x += gridSize)
        {
            g_colorBuffer[(g_windowWidth * y) + x] = gridColor;
        }
    }
}

void Render(void)
{
    SDL_SetRenderDrawColor(g_renderer, 255, 0, 0, 255);
    SDL_RenderClear(g_renderer);

    DrawGrid();
    RenderColorBuffer();
    ClearColorBuffer(0xFF000000);

    SDL_RenderPresent(g_renderer);
}

void DestroyWindow(void)
{
    SDL_DestroyTexture(g_colorBufferTexture);
    free(g_colorBuffer);
    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);
    SDL_Quit();
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