#include "pch.h"
#include "Display.h"
#include "Mesh.h"
#include "Vector.h"

// Globals
bool g_isRunning = false;
uint32_t g_previousFrameTime = 0;
float g_fovFactor = 640;
vec3_t g_cameraPosition = { .x = 0, .y = 0, .z = -5 };
vec3_t g_cubeRotation = { .x = 0, .y = 0, .z = 0 };
triangle_t g_trianglesToRender[N_MESH_FACES];

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

    for (int i = 0; i < N_MESH_FACES; ++i)
    {
        face_t meshFace = g_meshFaces[i];
        vec3_t faceVertices[3];
        faceVertices[0] = g_meshVertices[meshFace.a - 1];
        faceVertices[1] = g_meshVertices[meshFace.b - 1];
        faceVertices[2] = g_meshVertices[meshFace.c - 1];

        triangle_t projectedTriangle;

        for (int j = 0; j < 3; ++j)
        {
            vec3_t transformedVertex = faceVertices[j];

            // Rotate
            transformedVertex = Vec3RotateX(transformedVertex, g_cubeRotation.x);
            transformedVertex = Vec3RotateY(transformedVertex, g_cubeRotation.y);
            transformedVertex = Vec3RotateZ(transformedVertex, g_cubeRotation.z);

            // Translate
            transformedVertex.z -= g_cameraPosition.z;

            // Project
            vec2_t projectedPoint = Project(transformedVertex);

            // Center
            projectedPoint.x += (g_windowWidth / 2);
            projectedPoint.y += (g_windowHeight / 2);

            projectedTriangle.points[j] = projectedPoint;
        }
        g_trianglesToRender[i] = projectedTriangle;
    }
}

void Render(void)
{
    DrawGrid(10, 0xFF333333);

    for (int i = 0; i < N_MESH_FACES; ++i)
    {
        triangle_t triangleToRender = g_trianglesToRender[i];
        DrawFilledRect(
            (int)triangleToRender.points[0].x,
            (int)triangleToRender.points[0].y,
            3,
            3,
            0xFFFFFF00);
        DrawFilledRect((int)triangleToRender.points[1].x,
            (int)triangleToRender.points[1].y,
            3,
            3,
            0xFFFFFF00);
        DrawFilledRect(
            (int)triangleToRender.points[2].x,
            (int)triangleToRender.points[2].y,
            3,
            3,
            0xFFFFFF00);
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