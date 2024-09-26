#pragma once
#include "Texture.h"
#include "Vector.h"

typedef struct {
    int a;
    int b;
    int c;
    tex2_t a_uv;
    tex2_t b_uv;
    tex2_t c_uv;
    uint32_t color;
} face_t;

typedef struct {
    vec2_t points[3];
    tex2_t texCoords[3];
    uint32_t color;
    float averageDepth;
} triangle_t;

void DrawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
void DrawTexel(int x, int y, uint32_t* texture, vec2_t pointA, vec2_t pointB, vec2_t pointC,
    float u0, float v0, float u1, float v1, float u2, float v2);
void DrawTexturedTriangle(int x0, int y0, float u0, float v0,
    int x1, int y1, float u1, float v1,
    int x2, int y2, float u2, float v2, uint32_t* texture);