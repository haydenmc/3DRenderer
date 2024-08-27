#include "pch.h"
#include "Display.h"

int g_windowWidth = 800;
int g_windowHeight = 600;
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

void DrawGrid(uint32_t gridSize, uint32_t gridColor)
{
    for (int y = 0; y < g_windowHeight; y += gridSize)
    {
        for (int x = 0; x < g_windowWidth; x += gridSize)
        {
            g_colorBuffer[(g_windowWidth * y) + x] = gridColor;
        }
    }
}

void DrawRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color)
{
    // Top and bottom
    for (uint32_t i = 0; i <= width; ++i)
    {
        g_colorBuffer[(g_windowWidth * y) + x + i] = color;
        g_colorBuffer[(g_windowWidth * (y + height)) + x + i] = color;
    }
    // Left and right
    for (uint32_t i = 0; i <= height; ++i)
    {
        g_colorBuffer[(g_windowWidth * (y + i)) + x] = color;
        g_colorBuffer[(g_windowWidth * (y + i)) + x + width] = color;
    }
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

void DestroyWindow(void)
{
    SDL_DestroyTexture(g_colorBufferTexture);
    free(g_colorBuffer);
    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);
    SDL_Quit();
}