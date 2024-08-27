#include "pch.h"
#include "Display.h"
#include "Vector.h"

// Defines
#define NUM_CUBE_POINTS (9 * 9 * 9)

// Globals
bool g_isRunning = false;
vec3_t g_cubePoints[NUM_CUBE_POINTS]; // 9x9x9 cube
vec2_t g_projectedPoints[NUM_CUBE_POINTS];
float g_fovFactor = 640;
vec3_t g_cameraPosition = { .x = 0, .y = 0, .z = -5 };

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

    int pointCount = 0;
    for (float x = -1; x <= 1; x += 0.25)
    {
        for (float y = -1; y <= 1; y += 0.25)
        {
            for (float z = -1; z <= 1; z += 0.25)
            {
                vec3_t newPoint = { .x = x, .y = y, .z = z };
                g_cubePoints[pointCount] = newPoint;
                ++pointCount;
            }
        }
    }
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

vec2_t Project(vec3_t point)
{
    vec2_t projectedPoint = {
        .x = (g_fovFactor * point.x) / point.z,
        .y = (g_fovFactor * point.y) / point.z
    };
    return projectedPoint;
}

void Update(void)
{
    for (int i = 0; i < NUM_CUBE_POINTS; ++i)
    {
        vec3_t point = g_cubePoints[i];

        // HACK: move the points away from the camera
        point.z -= g_cameraPosition.z;

        vec2_t projectedPoint = Project(point);
        g_projectedPoints[i] = projectedPoint;
    }
}

void Render(void)
{
    DrawGrid(10, 0xFF333333);

    for (int i = 0; i < NUM_CUBE_POINTS; ++i)
    {
        vec2_t projectedPoint = g_projectedPoints[i];
        DrawFilledRect(
            (int)(projectedPoint.x + (g_windowWidth / 2)),
            (int)(projectedPoint.y + (g_windowHeight / 2)),
            4,
            4,
            0xFFFFFF00
        );
    }
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