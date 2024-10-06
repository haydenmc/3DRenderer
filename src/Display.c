#include "pch.h"
#include "Display.h"

static int g_windowWidth = 640;
static int g_windowHeight = 360;
static SDL_Window* g_window = NULL;
static SDL_Renderer* g_renderer = NULL;
static uint32_t* g_colorBuffer = NULL;
static float* g_zBuffer = NULL;
static SDL_Texture* g_colorBufferTexture = NULL;
enum RenderMode g_renderMode = RENDER_TEXTURED | RENDER_ENABLE_BACK_FACE_CULLING;

int GetWindowWidth(void)
{
    return g_windowWidth;
}

int GetWindowHeight(void)
{
    return g_windowHeight;
}

enum RenderMode GetRenderMode(void)
{
    return g_renderMode;
}

void SetRenderMode(enum RenderMode mode)
{
    g_renderMode = mode;
}

bool InitializeWindow(void)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
    {
        fprintf(stderr, "Error initializing SDL\n");
        return false;
    }

    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    int fullscreenWidth = displayMode.w;
    int fullscreenHeight = displayMode.h;

    g_window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        fullscreenWidth,
        fullscreenHeight,
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

    g_colorBuffer = malloc(sizeof(uint32_t) * g_windowWidth * g_windowHeight);
    g_zBuffer = malloc(sizeof(float) * g_windowWidth * g_windowHeight);
    g_colorBufferTexture = SDL_CreateTexture(
        g_renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        g_windowWidth,
        g_windowHeight
    );

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
    if ((x < 0) || (x >= g_windowWidth) || (y < 0) || (y >= g_windowHeight))
    {
        return;
    }
    g_colorBuffer[(g_windowWidth * y) + x] = color;
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

void DrawLine(int x0, int y0, int x1, int y1, uint32_t color)
{
    int deltaX = (x1 - x0);
    int deltaY = (y1 - y0);
    int sideLength = abs(deltaX) >= abs(deltaY) ? abs(deltaX) : abs(deltaY);

    float xInc = deltaX / (float)sideLength;
    float yInc = deltaY / (float)sideLength;

    float currentX = (float)x0;
    float currentY = (float)y0;

    for (int i = 0; i <= sideLength; ++i)
    {
        DrawPixel((int)roundf(currentX), (int)roundf(currentY), color);
        currentX += xInc;
        currentY += yInc;
    }
}

void DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    DrawLine(x0, y0, x1, y1, color);
    DrawLine(x1, y1, x2, y2, color);
    DrawLine(x2, y2, x0, y0, color);
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
    SDL_RenderPresent(g_renderer);
}

void ClearZBuffer(void)
{
    for (int y = 0; y < g_windowHeight; ++y)
    {
        for (int x = 0; x < g_windowWidth; ++x)
        {
            g_zBuffer[(g_windowWidth * y) + x] = 1.0f;
        }
    }
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
    free(g_colorBuffer);
    free(g_zBuffer);
    SDL_DestroyTexture(g_colorBufferTexture);
    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);
    SDL_Quit();
}

float GetZBufferValue(int x, int y)
{
    if ((x < 0) || (x >= g_windowWidth) || (y < 0) || (y >= g_windowHeight))
    {
        return 1.0f;
    }
    return g_zBuffer[(g_windowWidth * y) + x];
}

void SetZBufferValue(int x, int y, float value)
{
    if ((x < 0) || (x >= g_windowWidth) || (y < 0) || (y >= g_windowHeight))
    {
        return;
    }
    g_zBuffer[(g_windowWidth * y) + x] = value;
}
