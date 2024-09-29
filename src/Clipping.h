#pragma once
#include "Triangle.h"
#include "Vector.h"

#define MAX_POLYGON_VERTICES 16
#define MAX_POLYGON_TRIANGLES (MAX_POLYGON_VERTICES - 2)

enum FrustumPlaneKind {
    LEFT_FRUSTUM_PLANE,
    RIGHT_FRUSTUM_PLANE,
    TOP_FRUSTUM_PLANE,
    BOTTOM_FRUSTUM_PLANE,
    NEAR_FRUSTUM_PLANE,
    FAR_FRUSTUM_PLANE,
    NUM_FRUSTUM_PLANES,
} FrustumPlaneKind;

typedef struct {
    vec3_t point;
    vec3_t normal;
} plane_t;

typedef struct {
    vec3_t vertices[MAX_POLYGON_VERTICES];
    tex2_t textureCoordinates[MAX_POLYGON_VERTICES];
    int numVertices;
} polygon_t;

void InitFrustumPlanes(float fovX, float fovY, float zNear, float zFar);
polygon_t CreatePolygonFromTriangle(vec3_t v0, vec3_t v1, vec3_t v2,
    tex2_t t0, tex2_t t1, tex2_t t2);
int TrianglesFromPolygon(polygon_t polygon, triangle_t triangles[]);
polygon_t ClipPolygon(polygon_t polygon);