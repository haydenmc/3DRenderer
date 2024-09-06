#include "pch.h"
#include <array/array.h>
#include "Display.h"
#include "Mesh.h"
#include "Vector.h"

// Globals
bool g_isRunning = false;
uint32_t g_previousFrameTime = 0;
float g_fovFactor = 640;
vec3_t g_cameraPosition = { .x = 0, .y = 0, .z = -4 };
triangle_t* g_trianglesToRender = NULL;

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
    LoadObjFileData("./assets/f22.obj");
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

    g_trianglesToRender = NULL;

    g_Mesh.rotation.x += 0.025f;
    g_Mesh.rotation.y -= 0.005f;

    int numFaces = array_length(g_Mesh.faces);
    for (int i = 0; i < numFaces; ++i)
    {
        face_t meshFace = g_Mesh.faces[i];
        vec3_t faceVertices[3];
        faceVertices[0] = g_Mesh.vertices[meshFace.a - 1];
        faceVertices[1] = g_Mesh.vertices[meshFace.b - 1];
        faceVertices[2] = g_Mesh.vertices[meshFace.c - 1];

        triangle_t projectedTriangle;

        for (int j = 0; j < 3; ++j)
        {
            vec3_t transformedVertex = faceVertices[j];

            // Rotate
            transformedVertex = Vec3RotateX(transformedVertex, g_Mesh.rotation.x);
            transformedVertex = Vec3RotateY(transformedVertex, g_Mesh.rotation.y);
            transformedVertex = Vec3RotateZ(transformedVertex, g_Mesh.rotation.z);

            // Translate
            transformedVertex.z -= g_cameraPosition.z;

            // Project
            vec2_t projectedPoint = Project(transformedVertex);

            // Center
            projectedPoint.x += (g_windowWidth / 2);
            projectedPoint.y += (g_windowHeight / 2);

            projectedTriangle.points[j] = projectedPoint;
        }
        array_push(g_trianglesToRender, projectedTriangle);
    }
}

void Render(void)
{
    DrawGrid(10, 0xFF333333);
    int numTriangles = array_length(g_trianglesToRender);
    for (int i = 0; i < numTriangles; ++i)
    {
        triangle_t triangleToRender = g_trianglesToRender[i];
        DrawTriangle(
            (int)triangleToRender.points[0].x,
            (int)triangleToRender.points[0].y,
            (int)triangleToRender.points[1].x,
            (int)triangleToRender.points[1].y,
            (int)triangleToRender.points[2].x,
            (int)triangleToRender.points[2].y,
            0xFF00FF00);
    }

    array_free(g_trianglesToRender);

    RenderColorBuffer();

    ClearColorBuffer(0xFF000000);

    SDL_RenderPresent(g_renderer);
}

void FreeResources(void)
{
    free(g_colorBuffer);
    array_free(g_Mesh.faces);
    array_free(g_Mesh.vertices);
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

    FreeResources();

    DestroyWindow();

    return 0;
}