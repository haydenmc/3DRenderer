#pragma once
#include "Texture.h"
#include "Vector.h"
#define VERTEX_FLOAT_TOLERANCE 0.1f

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
    vec4_t points[3];
    tex2_t texCoords[3];
    uint32_t color;
    upng_t* texture;
} triangle_t;

void DrawFilledTriangle(vec4_t a, vec4_t b, vec4_t c, uint32_t color);
void DrawTexturedTriangle(int x0, int y0, float z0, float w0, float u0, float v0,
    int x1, int y1, float z1, float w1, float u1, float v1,
    int x2, int y2, float z2, float w2, float u2, float v2, upng_t* texture);