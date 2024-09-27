#include "pch.h"
#include <array/array.h>
#include "Display.h"
#include "Lighting.h"
#include "Matrix.h"
#include "Mesh.h"
#include "Texture.h"
#include "Triangle.h"
#include "Vector.h"

enum RenderMode
{
    RENDER_VERTICES                 = 1 << 0,
    RENDER_WIREFRAME                = 1 << 1,
    RENDER_FLAT_SHADING             = 1 << 2,
    RENDER_TEXTURED                 = 1 << 3,
    RENDER_ENABLE_BACK_FACE_CULLING = 1 << 4,
} RenderMode;

// Constants
#define PROJECTION_FOV ((float)M_PI / 3.0f)
#define PROJECTION_Z_NEAR 0.1f
#define PROJECTION_Z_FAR 100.0f

// Globals
bool g_isRunning = false;
uint32_t g_previousFrameTime = 0;
vec3_t g_cameraPosition = { .x = 0, .y = 0, .z = 0 };
light_t g_light;
mat4_t g_projectionMatrix;
triangle_t* g_trianglesToRender = NULL;
enum RenderMode g_renderMode = RENDER_TEXTURED | RENDER_ENABLE_BACK_FACE_CULLING;

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
    g_projectionMatrix = Matrix4MakePerspective(PROJECTION_FOV,
        (g_windowHeight / (float)g_windowWidth), PROJECTION_Z_NEAR, PROJECTION_Z_FAR);
    g_light = (light_t){
        .direction = Vec3Normalize((vec3_t){
             .x = 0,
             .y = 0,
             .z = 1 })
        };
    
    // Manually load hard-coded texture data from static array
    g_meshTexture = (uint32_t*)REDBRICK_TEXTURE;
    g_textureWidth = 64;
    g_textureHeight = 64;

    // LoadObjFileData("./assets/f22.obj");
    // LoadObjFileData("assets/cube.obj");
    LoadCubeMeshData();
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
        switch (event.key.keysym.sym)
        {
        case SDLK_ESCAPE:
            g_isRunning = false;
            break;
        case SDLK_1:
            g_renderMode = RENDER_VERTICES | RENDER_WIREFRAME | (g_renderMode & RENDER_ENABLE_BACK_FACE_CULLING);
            break;
        case SDLK_2:
            g_renderMode = RENDER_WIREFRAME | (g_renderMode & RENDER_ENABLE_BACK_FACE_CULLING);
            break;
        case SDLK_3:
            g_renderMode = RENDER_FLAT_SHADING | (g_renderMode & RENDER_ENABLE_BACK_FACE_CULLING);
            break;
        case SDLK_4:
            g_renderMode = RENDER_FLAT_SHADING | RENDER_WIREFRAME | (g_renderMode & RENDER_ENABLE_BACK_FACE_CULLING);
            break;
        case SDLK_5:
            g_renderMode = RENDER_TEXTURED | (g_renderMode & RENDER_ENABLE_BACK_FACE_CULLING);
            break;
        case SDLK_6:
            g_renderMode = RENDER_TEXTURED | RENDER_WIREFRAME | (g_renderMode & RENDER_ENABLE_BACK_FACE_CULLING);
            break;
        case SDLK_c:
            g_renderMode = g_renderMode | RENDER_ENABLE_BACK_FACE_CULLING;
            break;
        case SDLK_d:
            g_renderMode = g_renderMode & ~RENDER_ENABLE_BACK_FACE_CULLING;
            break;
        }
        break;
    }
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

    // g_Mesh.scale.x -= 0.001f;
    // g_Mesh.scale.y -= 0.001f;
    g_Mesh.rotation.x += 0.01f;
    g_Mesh.rotation.y += 0.01f;
    //g_Mesh.rotation.z += 0.01f;
    g_Mesh.translation.z = 5.0f;

    mat4_t scaleMatrix = Matrix4MakeScale(g_Mesh.scale.x, g_Mesh.scale.y, g_Mesh.scale.z);
    mat4_t rotationMatrixX = Matrix4MakeRotationX(g_Mesh.rotation.x);
    mat4_t rotationMatrixY = Matrix4MakeRotationY(g_Mesh.rotation.y);
    mat4_t rotationMatrixZ = Matrix4MakeRotationZ(g_Mesh.rotation.z);
    mat4_t translationMatrix = Matrix4MakeTranslation(g_Mesh.translation.x, g_Mesh.translation.y,
        g_Mesh.translation.z);
    mat4_t worldMatrix = Matrix4Identity();
    worldMatrix = Matrix4MultiplyM(scaleMatrix, worldMatrix);
    worldMatrix = Matrix4MultiplyM(rotationMatrixX, worldMatrix);
    worldMatrix = Matrix4MultiplyM(rotationMatrixY, worldMatrix);
    worldMatrix = Matrix4MultiplyM(rotationMatrixZ, worldMatrix);
    worldMatrix = Matrix4MultiplyM(translationMatrix, worldMatrix);

    int numFaces = array_length(g_Mesh.faces);
    for (int i = 0; i < numFaces; ++i)
    {
        face_t meshFace = g_Mesh.faces[i];
        vec3_t faceVertices[3];
        faceVertices[0] = g_Mesh.vertices[meshFace.a - 1];
        faceVertices[1] = g_Mesh.vertices[meshFace.b - 1];
        faceVertices[2] = g_Mesh.vertices[meshFace.c - 1];

        triangle_t projectedTriangle;

        // Transform vertices
        vec4_t transformedVertices[3];
        for (int j = 0; j < 3; ++j)
        {
            vec4_t transformedVertex = Vec4FromVec3(faceVertices[j]);
            transformedVertex = Matrix4MultiplyV(worldMatrix, transformedVertex);
            transformedVertices[j] = transformedVertex;
        }

        // Calculate face normal
        vec3_t vectorA = Vec3FromVec4(transformedVertices[0]);
        vec3_t vectorB = Vec3FromVec4(transformedVertices[1]);
        vec3_t vectorC = Vec3FromVec4(transformedVertices[2]);
        vec3_t vectorAB = Vec3Subtract(vectorB, vectorA);
        vectorAB = Vec3Normalize(vectorAB);
        vec3_t vectorAC = Vec3Subtract(vectorC, vectorA);
        vectorAC = Vec3Normalize(vectorAC);
        vec3_t faceNormal = Vec3CrossProduct(vectorAB, vectorAC);
        faceNormal = Vec3Normalize(faceNormal);

        // Calculate lighting
        projectedTriangle.color = LightCalculateColorForFace(faceNormal, g_light, meshFace.color);

        // Cull back-faces if enabled
        if (g_renderMode & RENDER_ENABLE_BACK_FACE_CULLING)
        {
            vec3_t cameraRay = Vec3Subtract(g_cameraPosition, vectorA);
            float cameraDotNormal = Vec3DotProduct(faceNormal, cameraRay);
            if (cameraDotNormal <= 0)
            {
                continue;
            }
        }

        // Calculate average depth for sorting
        float averageDepth = (transformedVertices[0].z + transformedVertices[1].z +
            transformedVertices[2].z) / 3.0f;
        projectedTriangle.averageDepth = averageDepth;

        // Project to screen space
        for (int j = 0; j < 3; ++j)
        {
            vec4_t projectedPoint = Matrix4MultiplyVProject(g_projectionMatrix,
                transformedVertices[j]);

            // Scale from screen space to actual screen size
            projectedPoint.x *= (g_windowWidth / 2.0f);
            projectedPoint.y *= (g_windowHeight / 2.0f);

            // Invert the y values to account for flipped screen y coordinates
            projectedPoint.y *= -1;

            // Translate points to the middle of the screen
            projectedPoint.x += (g_windowWidth / 2.0f);
            projectedPoint.y += (g_windowHeight / 2.0f);

            projectedTriangle.points[j] = (vec4_t){ .x = projectedPoint.x, .y = projectedPoint.y,
                .z = projectedPoint.z, .w = projectedPoint.w };
        }

        // Populate texture coordinates
        projectedTriangle.texCoords[0] = (tex2_t){ meshFace.a_uv.u, meshFace.a_uv.v };
        projectedTriangle.texCoords[1] = (tex2_t){ meshFace.b_uv.u, meshFace.b_uv.v };
        projectedTriangle.texCoords[2] = (tex2_t){ meshFace.c_uv.u, meshFace.c_uv.v };

        array_push(g_trianglesToRender, projectedTriangle);
    }

    // Sort the triangles to render by average depth, descending.
    for (int i = 0; i < max(array_length(g_trianglesToRender) - 1, 0); ++i)
    {
        // Find the highest depth triangle
        triangle_t currentTriangle = g_trianglesToRender[i];
        int highestDepthTriangleIndex = i;
        for (int j = i + 1; j < array_length(g_trianglesToRender); ++j)
        {
            if (g_trianglesToRender[j].averageDepth >
                g_trianglesToRender[highestDepthTriangleIndex].averageDepth)
            {
                highestDepthTriangleIndex = j;
            }
        }
        // Swap it
        if (i != highestDepthTriangleIndex)
        {
            g_trianglesToRender[i] = g_trianglesToRender[highestDepthTriangleIndex];
            g_trianglesToRender[highestDepthTriangleIndex] = currentTriangle;
        }
    }
}

void Render(void)
{
    DrawGrid(10, 0xFF333333);

    int numTriangles = array_length(g_trianglesToRender);
    for (int i = 0; i < numTriangles; ++i)
    {
        triangle_t triangleToRender = g_trianglesToRender[i];
        int x0 = (int)triangleToRender.points[0].x;
        int y0 = (int)triangleToRender.points[0].y;
        float z0 = triangleToRender.points[0].z;
        float w0 = triangleToRender.points[0].w;
        float u0 = triangleToRender.texCoords[0].u;
        float v0 = triangleToRender.texCoords[0].v;
        int x1 = (int)triangleToRender.points[1].x;
        int y1 = (int)triangleToRender.points[1].y;
        float z1 = triangleToRender.points[1].z;
        float w1 = triangleToRender.points[1].w;
        float u1 = triangleToRender.texCoords[1].u;
        float v1 = triangleToRender.texCoords[1].v;
        int x2 = (int)triangleToRender.points[2].x;
        int y2 = (int)triangleToRender.points[2].y;
        float z2 = triangleToRender.points[2].z;
        float w2 = triangleToRender.points[2].w;
        float u2 = triangleToRender.texCoords[2].u;
        float v2 = triangleToRender.texCoords[2].v;

        if (g_renderMode & RENDER_TEXTURED)
        {
            DrawTexturedTriangle(x0, y0, z0, w0, u0, v0,
                x1, y1, z1, w1, u1, v1,
                x2, y2, z2, w2, u2, v2, g_meshTexture);
        }
        if (g_renderMode & RENDER_FLAT_SHADING)
        {
            DrawFilledTriangle(x0, y0, x1, y1, x2, y2, triangleToRender.color);
        }
        if (g_renderMode & RENDER_WIREFRAME)
        {
            DrawTriangle(x0, y0, x1, y1, x2, y2, 0xFF00FF00);
        }
        if (g_renderMode & RENDER_VERTICES)
        {
            DrawFilledRect(x0 - 1, y0 - 1, 2, 2, 0xFFFF0000);
            DrawFilledRect(x1 - 1, y1 - 1, 2, 2, 0xFFFF0000);
            DrawFilledRect(x2 - 1, y2 - 1, 2, 2, 0xFFFF0000);
        }
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