#include "pch.h"
#include "Clipping.h"

plane_t g_frustumPlanes[NUM_FRUSTUM_PLANES];

void InitFrustumPlanes(float fovX, float fovY, float zNear, float zFar)
{
    float xCosHalfFov = cosf(fovX / 2.0f);
    float xSinHalfFov = sinf(fovX / 2.0f);
    float yCosHalfFov = cosf(fovY / 2.0f);
    float ySinHalfFov = sinf(fovY / 2.0f);
    vec3_t origin = { 0.0f, 0.0f, 0.0f };

    g_frustumPlanes[LEFT_FRUSTUM_PLANE].point = origin;
    g_frustumPlanes[LEFT_FRUSTUM_PLANE].normal.x = xCosHalfFov;
    g_frustumPlanes[LEFT_FRUSTUM_PLANE].normal.y = 0;
    g_frustumPlanes[LEFT_FRUSTUM_PLANE].normal.z = xSinHalfFov;

    g_frustumPlanes[RIGHT_FRUSTUM_PLANE].point = origin;
    g_frustumPlanes[RIGHT_FRUSTUM_PLANE].normal.x = -xCosHalfFov;
    g_frustumPlanes[RIGHT_FRUSTUM_PLANE].normal.y = 0;
    g_frustumPlanes[RIGHT_FRUSTUM_PLANE].normal.z = xSinHalfFov;

    g_frustumPlanes[TOP_FRUSTUM_PLANE].point = origin;
    g_frustumPlanes[TOP_FRUSTUM_PLANE].normal.x = 0;
    g_frustumPlanes[TOP_FRUSTUM_PLANE].normal.y = -yCosHalfFov;
    g_frustumPlanes[TOP_FRUSTUM_PLANE].normal.z = ySinHalfFov;

    g_frustumPlanes[BOTTOM_FRUSTUM_PLANE].point = origin;
    g_frustumPlanes[BOTTOM_FRUSTUM_PLANE].normal.x = 0;
    g_frustumPlanes[BOTTOM_FRUSTUM_PLANE].normal.y = yCosHalfFov;
    g_frustumPlanes[BOTTOM_FRUSTUM_PLANE].normal.z = ySinHalfFov;

    g_frustumPlanes[NEAR_FRUSTUM_PLANE].point = (vec3_t){ 0.0f, 0.0f, zNear };
    g_frustumPlanes[NEAR_FRUSTUM_PLANE].normal = (vec3_t){ 0.0f, 0.0f, 1.0f };

    g_frustumPlanes[FAR_FRUSTUM_PLANE].point = (vec3_t){ 0.0f, 0.0f, zFar };
    g_frustumPlanes[FAR_FRUSTUM_PLANE].normal = (vec3_t){ 0.0f, 0.0f, -1.0f };
}

polygon_t CreatePolygonFromTriangle(vec3_t v0, vec3_t v1, vec3_t v2,
    tex2_t t0, tex2_t t1, tex2_t t2)
{
    polygon_t polygon = {
        .vertices = { v0, v1, v2 },
        .textureCoordinates = { t0, t1, t2 },
        .numVertices = 3,
    };
    return polygon;
}

int TrianglesFromPolygon(polygon_t polygon, triangle_t triangles[])
{
    for (int i = 0; i < (polygon.numVertices - 2); ++i)
    {
        int firstIndex = 0;
        int secondIndex = i + 1;
        int thirdIndex = i + 2;
        triangles[i].points[0] = Vec4FromVec3(polygon.vertices[firstIndex]);
        triangles[i].texCoords[0] = polygon.textureCoordinates[firstIndex];
        triangles[i].points[1] = Vec4FromVec3(polygon.vertices[secondIndex]);
        triangles[i].texCoords[1] = polygon.textureCoordinates[secondIndex];
        triangles[i].points[2] = Vec4FromVec3(polygon.vertices[thirdIndex]);
        triangles[i].texCoords[2] = polygon.textureCoordinates[thirdIndex];
    }
    return (polygon.numVertices - 2);
}

float LerpF(float a, float b, float t)
{
    return a + t * (b - a);
}

polygon_t ClipPolygonAgainstPlane(polygon_t polygon, enum FrustumPlaneKind plane)
{
    polygon_t result = { 0 };
    vec3_t planePoint = g_frustumPlanes[plane].point;
    vec3_t planeNormal = g_frustumPlanes[plane].normal;

    vec3_t* currentVertex = &polygon.vertices[0];
    tex2_t* currentTextureCoordinate = &polygon.textureCoordinates[0];
    vec3_t* previousVertex = &polygon.vertices[polygon.numVertices - 1];
    tex2_t* previousTextureCoordinate = &polygon.textureCoordinates[polygon.numVertices - 1];

    float previousDot = Vec3DotProduct(Vec3Subtract(*previousVertex, planePoint), planeNormal);
    while (currentVertex != &polygon.vertices[polygon.numVertices])
    {
        float currentDot = Vec3DotProduct(Vec3Subtract(*currentVertex, planePoint), planeNormal);

        // Clever way of determining whether signs differ between the dots
        if (currentDot * previousDot < 0.0f)
        {
            // Calculate intersection point: I = Q1 + t(Q2 - Q1)
            float t = previousDot / (previousDot - currentDot);
            vec3_t intersectionPoint = {
                .x = LerpF(previousVertex->x, currentVertex->x, t),
                .y = LerpF(previousVertex->y, currentVertex->y, t),
                .z = LerpF(previousVertex->z, currentVertex->z, t),
            };
            tex2_t interpolatedTextureCoordinate = {
                .u = LerpF(previousTextureCoordinate->u, currentTextureCoordinate->u, t),
                .v = LerpF(previousTextureCoordinate->v, currentTextureCoordinate->v, t),
            };

            result.vertices[result.numVertices] = intersectionPoint;
            result.textureCoordinates[result.numVertices] = interpolatedTextureCoordinate;
            result.numVertices++;
        }

        if (currentDot > 0.0f)
        {
            // Current vertex is inside the plane
            result.vertices[result.numVertices] = *currentVertex;
            result.textureCoordinates[result.numVertices] = *currentTextureCoordinate;
            result.numVertices++;
        }
        
        previousDot = currentDot;
        previousVertex = currentVertex;
        ++currentVertex;
        previousTextureCoordinate = currentTextureCoordinate;
        ++currentTextureCoordinate;
    }

    return result;
}

polygon_t ClipPolygon(polygon_t polygon)
{
    for (enum FrustumPlaneKind plane = LEFT_FRUSTUM_PLANE; plane < NUM_FRUSTUM_PLANES; ++plane)
    {
        polygon = ClipPolygonAgainstPlane(polygon, plane);
    }

    return polygon;
}
