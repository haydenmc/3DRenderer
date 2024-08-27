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

void DrawGrid(int gridSize, uint32_t gridColor)
{
    for (int y = 0; y < g_windowHeight; y += gridSize)
    {
        for (int x = 0; x < g_windowWidth; x += gridSize)
        {
            DrawPixel(x, y, gridColor);
        }
    }
}

void DrawPixel(int x, int y, uint32_t color)
{
    if ((x < g_windowWidth) && (y < g_windowHeight))
    {
        g_colorBuffer[(g_windowWidth * y) + x] = color;
    }
}

void DrawFilledRect(int x, int y, int width, int height, uint32_t color)
{
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            DrawPixel((x + j), (y + i), color);
        }
    }
}

void DrawRect(int x, int y, int width, int height, uint32_t color)
{
    // Top and bottom
    for (int i = 0; i <= width; ++i)
    {
        DrawPixel((x + i), y, color);
        DrawPixel((x + i), (y + height), color);
    }
    // Left and right
    for (int i = 0; i <= height; ++i)
    {
        DrawPixel(x, (y + i), color);
        DrawPixel((x + width), (y + i), color);
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
            DrawPixel(x, y, color);
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