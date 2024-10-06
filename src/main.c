#include "pch.h"
#include <array/array.h>
#include "Camera.h"
#include "Clipping.h"
#include "Display.h"
#include "Lighting.h"
#include "Matrix.h"
#include "Mesh.h"
#include "Texture.h"
#include "Triangle.h"
#include "Vector.h"

// Constants
#define MAX_TRIANGLES 10000
#define PROJECTION_FOV_Y ((float)M_PI / 3.0f)
#define PROJECTION_Z_NEAR 0.1f
#define PROJECTION_Z_FAR 100.0f

// Globals
bool g_isRunning = false;
uint32_t g_previousFrameTime = 0;
float g_deltaTime = 0;
mat4_t g_viewMatrix;
mat4_t g_projectionMatrix;
triangle_t g_trianglesToRender[MAX_TRIANGLES];
int g_numTrianglesToRender = 0;
static const Uint8* g_keyboardState;

void Setup(void)
{
    g_keyboardState = SDL_GetKeyboardState(NULL);
    InitializeLight((vec3_t){ 0.0f, 0.0f, 1.0f });
    float aspectX = GetWindowWidth() / (float)GetWindowHeight();
    float aspectY = GetWindowHeight() / (float)GetWindowWidth();
    g_projectionMatrix = Matrix4MakePerspective(PROJECTION_FOV_Y, aspectY,
        PROJECTION_Z_NEAR, PROJECTION_Z_FAR);
    float fovX = atanf(tanf(PROJECTION_FOV_Y / 2.0f) * aspectX) * 2.0f;
    InitFrustumPlanes(fovX, PROJECTION_FOV_Y, PROJECTION_Z_NEAR, PROJECTION_Z_FAR);

    LoadMesh("./assets/cube.obj", "./assets/cube.png", (vec3_t){ 1.0f, 1.0f, 1.0f },
        (vec3_t){ 0.0f, 0.0f, 0.0f }, (vec3_t){ -6.0f, 0.0f, 4.0f });

    LoadMesh("./assets/f22.obj", "./assets/f22.png", (vec3_t){ 1.0f, 1.0f, 1.0f },
        (vec3_t){ 0.0f, 0.0f, 0.0f }, (vec3_t){ 0.0f, 0.0f, 4.0f });

    LoadMesh("./assets/f117.obj", "./assets/f117.png", (vec3_t){ 1.0f, 1.0f, 1.0f },
        (vec3_t){ 0.0f, 0.0f, 0.0f }, (vec3_t){ 6.0f, 0.0f, 4.0f });

    LoadMesh("./assets/efa.obj", "./assets/efa.png", (vec3_t){ 1.0f, 1.0f, 1.0f },
        (vec3_t){ 0.0f, 0.0f, 0.0f }, (vec3_t){ 12.0f, 0.0f, 4.0f });

    LoadMesh("./assets/crab.obj", "./assets/crab.png", (vec3_t){ 1.0f, 1.0f, 1.0f },
        (vec3_t){ 0.0f, 0.0f, 0.0f }, (vec3_t){ 18.0f, 0.0f, 4.0f });
}

void ProcessInput(void)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
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
            case SDLK_0:
                SetRenderMode(RENDER_VERTICES |
                    (GetRenderMode() & RENDER_ENABLE_BACK_FACE_CULLING));
                break;
            case SDLK_1:
                SetRenderMode(RENDER_VERTICES | RENDER_WIREFRAME |
                    (GetRenderMode() & RENDER_ENABLE_BACK_FACE_CULLING));
                break;
            case SDLK_2:
                SetRenderMode(RENDER_WIREFRAME |
                    (GetRenderMode() & RENDER_ENABLE_BACK_FACE_CULLING));
                break;
            case SDLK_3:
                SetRenderMode(RENDER_FLAT_SHADING | RENDER_WIREFRAME |
                    (GetRenderMode() & RENDER_ENABLE_BACK_FACE_CULLING));
                break;
            case SDLK_4:
                SetRenderMode(RENDER_FLAT_SHADING |
                    (GetRenderMode() & RENDER_ENABLE_BACK_FACE_CULLING));
                break;
            case SDLK_5:
                SetRenderMode(RENDER_TEXTURED | RENDER_WIREFRAME |
                    (GetRenderMode() & RENDER_ENABLE_BACK_FACE_CULLING));
                break;
            case SDLK_6:
                SetRenderMode(RENDER_TEXTURED |
                    (GetRenderMode() & RENDER_ENABLE_BACK_FACE_CULLING));
                break;
            case SDLK_c:
                SetRenderMode(GetRenderMode() | RENDER_ENABLE_BACK_FACE_CULLING);
                break;
            case SDLK_x:
                SetRenderMode(GetRenderMode() & ~RENDER_ENABLE_BACK_FACE_CULLING);
                break;
            }
            break;
        }
    }

    // Camera controls
    if (g_keyboardState[SDL_SCANCODE_SPACE])
    {
        vec3_t newPosition = GetCamera().position;
        newPosition.y += 3.0f * g_deltaTime;
        SetCameraPosition(newPosition);
    }
    if (g_keyboardState[SDL_SCANCODE_LCTRL])
    {
        vec3_t newPosition = GetCamera().position;
        newPosition.y -= 3.0f * g_deltaTime;
        SetCameraPosition(newPosition);
    }
    if (g_keyboardState[SDL_SCANCODE_W])
    {
        camera_t camera = GetCamera();
        vec3_t forwardVelocity = Vec3Multiply(camera.direction, 5.0f * g_deltaTime);
        SetCameraPosition(Vec3Add(camera.position, forwardVelocity));
    }
    if (g_keyboardState[SDL_SCANCODE_S])
    {
        camera_t camera = GetCamera();
        vec3_t forwardVelocity = Vec3Multiply(camera.direction, 5.0f * g_deltaTime);
        SetCameraPosition(Vec3Subtract(camera.position, forwardVelocity));
    }
    if (g_keyboardState[SDL_SCANCODE_LEFT])
    {
        float yaw = GetCamera().yaw;
        SetCameraYaw(yaw + (1.0f * g_deltaTime));
    }
    if (g_keyboardState[SDL_SCANCODE_RIGHT])
    {
        float yaw = GetCamera().yaw;
        SetCameraYaw(yaw - (1.0f * g_deltaTime));
    }
    if (g_keyboardState[SDL_SCANCODE_UP])
    {
        float pitch = GetCamera().pitch;
        SetCameraPitch(pitch + (1.0f * g_deltaTime));
    }
    if (g_keyboardState[SDL_SCANCODE_DOWN])
    {
        float pitch = GetCamera().pitch;
        SetCameraPitch(pitch - (1.0f * g_deltaTime));
    }
}

// Model space
//     -> World space
//         -> Camera space
//             -> Clipping
//                 -> Projection
//                     -> Image space (NDC)
//                         -> Screen space
void ProcessGraphicsPipelineStages(mesh_t* mesh)
{
    mat4_t scaleMatrix = Matrix4MakeScale(mesh->scale.x,
        mesh->scale.y, mesh->scale.z);
    mat4_t rotationMatrixX = Matrix4MakeRotationX(mesh->rotation.x);
    mat4_t rotationMatrixY = Matrix4MakeRotationY(mesh->rotation.y);
    mat4_t rotationMatrixZ = Matrix4MakeRotationZ(mesh->rotation.z);
    mat4_t translationMatrix = Matrix4MakeTranslation(
        mesh->translation.x, mesh->translation.y, mesh->translation.z);
    mat4_t worldMatrix = Matrix4Identity();
    worldMatrix = Matrix4MultiplyM(scaleMatrix, worldMatrix);
    worldMatrix = Matrix4MultiplyM(rotationMatrixX, worldMatrix);
    worldMatrix = Matrix4MultiplyM(rotationMatrixY, worldMatrix);
    worldMatrix = Matrix4MultiplyM(rotationMatrixZ, worldMatrix);
    worldMatrix = Matrix4MultiplyM(translationMatrix, worldMatrix);

    int numFaces = array_length(mesh->faces);
    for (int i = 0; i < numFaces; ++i)
    {
        face_t meshFace = mesh->faces[i];
        vec3_t faceVertices[3];
        faceVertices[0] = mesh->vertices[meshFace.a];
        faceVertices[1] = mesh->vertices[meshFace.b];
        faceVertices[2] = mesh->vertices[meshFace.c];

        // Transform vertices
        vec4_t transformedVertices[3];
        for (int j = 0; j < 3; ++j)
        {
            vec4_t transformedVertex = Vec4FromVec3(faceVertices[j]);
            transformedVertex = Matrix4MultiplyV(worldMatrix, transformedVertex);
            transformedVertex = Matrix4MultiplyV(g_viewMatrix, transformedVertex);
            transformedVertices[j] = transformedVertex;
        }

        // Calculate face normal
        vec3_t faceNormal = GetTriangleNormal(transformedVertices);

        // Cull back-faces if enabled
        if (GetRenderMode() & RENDER_ENABLE_BACK_FACE_CULLING)
        {
            vec3_t cameraRay = Vec3Subtract((vec3_t){ 0.0f, 0.0f, 0.0f },
                Vec3FromVec4(transformedVertices[0]));
            float cameraDotNormal = Vec3DotProduct(faceNormal, cameraRay);
            if (cameraDotNormal <= 0)
            {
                continue;
            }
        }

        // Clipping
        polygon_t polygon = CreatePolygonFromTriangle(
            Vec3FromVec4(transformedVertices[0]),
            Vec3FromVec4(transformedVertices[1]),
            Vec3FromVec4(transformedVertices[2]),
            meshFace.a_uv,
            meshFace.b_uv,
            meshFace.c_uv);
        polygon = ClipPolygon(polygon);
        // Break polygon into triangles
        triangle_t clippedTriangles[MAX_POLYGON_TRIANGLES];
        int numClippedTriangles = TrianglesFromPolygon(polygon, clippedTriangles);

        for (int t = 0; t < numClippedTriangles; ++t)
        {
            triangle_t clippedTriangle = clippedTriangles[t];
            triangle_t projectedTriangle;
            projectedTriangle.color = LightCalculateColorForFace(faceNormal, meshFace.color);

            // Project to screen space
            for (int j = 0; j < 3; ++j)
            {
                vec4_t projectedPoint = Matrix4MultiplyVProject(g_projectionMatrix,
                    clippedTriangle.points[j]);

                // Scale from screen space to actual screen size
                projectedPoint.x *= (GetWindowWidth() / 2.0f);
                projectedPoint.y *= (GetWindowHeight() / 2.0f);

                // Invert the y values to account for flipped screen y coordinates
                projectedPoint.y *= -1;

                // Translate points to the middle of the screen
                projectedPoint.x += (GetWindowWidth() / 2.0f);
                projectedPoint.y += (GetWindowHeight() / 2.0f);

                projectedTriangle.points[j] = (vec4_t){ .x = projectedPoint.x,
                    .y = projectedPoint.y, .z = projectedPoint.z, .w = projectedPoint.w };
            }

            // Populate texture
            projectedTriangle.texture = mesh->texture;
            projectedTriangle.texCoords[0] = (tex2_t){
                clippedTriangle.texCoords[0].u, clippedTriangle.texCoords[0].v };
            projectedTriangle.texCoords[1] = (tex2_t){
                clippedTriangle.texCoords[1].u, clippedTriangle.texCoords[1].v };
            projectedTriangle.texCoords[2] = (tex2_t){
                clippedTriangle.texCoords[2].u, clippedTriangle.texCoords[2].v };

            g_trianglesToRender[g_numTrianglesToRender] = projectedTriangle;
            g_numTrianglesToRender++;
        }
    }
}

void Update(void)
{
    // Calculate delta time
    float timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - g_previousFrameTime);
    if ((timeToWait > 0.0f) && (timeToWait <= FRAME_TARGET_TIME))
    {
        SDL_Delay((uint32_t)(timeToWait));
    }
    g_deltaTime = (SDL_GetTicks() - g_previousFrameTime) / 1000.0f;
    g_previousFrameTime = SDL_GetTicks();

    // Calculate camera transformation
    camera_t camera = GetCamera();
    vec3_t cameraUp = { 0.0f, 1.0f, 0.0f };
    vec3_t cameraTarget = { 0.0f, 0.0f, 1.0f };
    mat4_t cameraYawRotation = Matrix4MakeRotationY(camera.yaw);
    mat4_t cameraPitchRotation = Matrix4MakeRotationX(camera.pitch);
    mat4_t cameraRotation = Matrix4MultiplyM(cameraYawRotation, cameraPitchRotation);
    vec3_t cameraDirection = Vec3FromVec4(Matrix4MultiplyV(
        cameraRotation, Vec4FromVec3(cameraTarget)));
    SetCameraDirection(cameraDirection);
    cameraTarget = Vec3Add(camera.position, cameraDirection);
    g_viewMatrix = Matrix4MakeLookAt(camera.position, cameraTarget, cameraUp);

    // Calculate mesh transformations
    g_numTrianglesToRender = 0;
    for (int meshIndex = 0; meshIndex < GetNumMeshes(); ++meshIndex)
    {
        ProcessGraphicsPipelineStages(GetMeshAtIndex(meshIndex));
    }
}

void Render(void)
{
    ClearColorBuffer(0xFF000000);
    ClearZBuffer();

    DrawGrid(10, 0xFF333333);

    for (int i = 0; i < g_numTrianglesToRender; ++i)
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

        if (GetRenderMode() & RENDER_TEXTURED)
        {
            DrawTexturedTriangle(x0, y0, z0, w0, u0, v0,
                x1, y1, z1, w1, u1, v1,
                x2, y2, z2, w2, u2, v2, triangleToRender.texture);
        }

        if (GetRenderMode() & RENDER_FLAT_SHADING)
        {
            DrawFilledTriangle(triangleToRender.points[0], triangleToRender.points[1],
                triangleToRender.points[2], triangleToRender.color);
        }

        if (GetRenderMode() & RENDER_WIREFRAME)
        {
            DrawTriangle(x0, y0, x1, y1, x2, y2, 0xFF00FF00);
        }

        if (GetRenderMode() & RENDER_VERTICES)
        {
            DrawFilledRect(x0 - 1, y0 - 1, 2, 2, 0xFFFF0000);
            DrawFilledRect(x1 - 1, y1 - 1, 2, 2, 0xFFFF0000);
            DrawFilledRect(x2 - 1, y2 - 1, 2, 2, 0xFFFF0000);
        }
    }

    RenderColorBuffer();
}

void FreeResources(void)
{
    FreeMeshes();
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