#pragma once
#include "Vector.h"

typedef struct {
    float m[4][4];
} mat4_t;

mat4_t Matrix4Identity(void);
mat4_t Matrix4MakeScale(float sx, float sy, float sz);
mat4_t Matrix4MakeTranslation(float tx, float ty, float tz);
mat4_t Matrix4MakeRotationX(float angle);
mat4_t Matrix4MakeRotationY(float angle);
mat4_t Matrix4MakeRotationZ(float angle);
mat4_t Matrix4MultiplyM(mat4_t lhs, mat4_t rhs);
vec4_t Matrix4MultiplyV(mat4_t m, vec4_t v);