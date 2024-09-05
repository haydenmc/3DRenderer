#include "pch.h"
#include "Display.h"
#include "Vector.h"

// Defines
#define NUM_CUBE_POINTS (9 * 9 * 9)

// Globals
bool g_isRunning = false;
uint32_t g_previousFrameTime = 0;
vec3_t g_cubePoints[NUM_CUBE_POINTS]; // 9x9x9 cube
vec2_t g_projectedPoints[NUM_CUBE_POINTS];
float g_fovFactor = 640;
vec3_t g_cameraPosition = { .x = 0, .y = 0, .z = -5 };
vec3_t g_cubeRotation = { .x = 0, .y = 0, .z = 0 };

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
    float timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - g_previousFrameTime);
    if ((timeToWait > 0.0f) && (timeToWait <= FRAME_TARGET_TIME))
    {
        SDL_Delay((uint32_t)(timeToWait));
    }

    g_previousFrameTime = SDL_GetTicks();

    g_cubeRotation.x += 0.01f;
    g_cubeRotation.y += 0.01f;
    g_cubeRotation.z += 0.01f;

    for (int i = 0; i < NUM_CUBE_POINTS; ++i)
    {
        vec3_t point = g_cubePoints[i];

        vec3_t transformedPoint = Vec3RotateX(point, g_cubeRotation.x);
        transformedPoint = Vec3RotateY(transformedPoint, g_cubeRotation.y);
        transformedPoint = Vec3RotateZ(transformedPoint, g_cubeRotation.z);

        // HACK: Translate the points away from the camera
        transformedPoint.z -= g_cameraPosition.z;

        vec2_t projectedPoint = Project(transformedPoint);
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