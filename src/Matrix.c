#include "pch.h"
#include "Matrix.h"

mat4_t Matrix4Identity()
{
    mat4_t m = {{
        { 1, 0, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 0, 1 },
    }};
    return m;
}

mat4_t Matrix4MakeScale(float sx, float sy, float sz)
{
    mat4_t m = {{
        { sx,  0,  0, 0 },
        {  0, sy,  0, 0 },
        {  0,  0, sz, 0 },
        {  0,  0,  0, 1 },
    }};
    return m;
}

mat4_t Matrix4MakeTranslation(float tx, float ty, float tz)
{
    mat4_t result = {{
        { 1, 0, 0, tx },
        { 0, 1, 0, ty },
        { 0, 0, 1, tz },
        { 0, 0, 0,  1 },
    }};
    return result;
}

mat4_t Matrix4MakeRotationX(float angle)
{
    float c = cosf(angle);
    float s = sinf(angle);
    mat4_t result = {{
        { 1,  0, 0, 0 },
        { 0,  c, s, 0 },
        { 0, -s, c, 0 },
        { 0,  0, 0, 1 }
    }};
    return result;
}

mat4_t Matrix4MakeRotationY(float angle)
{
    float c = cosf(angle);
    float s = sinf(angle);
    mat4_t result = {{
        { c, 0, -s, 0 },
        { 0, 1,  0, 0 },
        { s, 0,  c, 0 },
        { 0, 0,  0, 1 }
    }};
    return result;
}

mat4_t Matrix4MakeRotationZ(float angle)
{
    float c = cosf(angle);
    float s = sinf(angle);
    mat4_t result = {{
        {  c, s, 0, 0 },
        { -s, c, 0, 0 },
        {  0, 0, 1, 0 },
        {  0, 0, 0, 1 }
    }};
    return result;
}

mat4_t Matrix4MultiplyM(mat4_t lhs, mat4_t rhs)
{
    mat4_t result;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            result.m[i][j] = (lhs.m[i][0] * rhs.m[0][j]) +
                (lhs.m[i][1] * rhs.m[1][j]) + 
                (lhs.m[i][2] * rhs.m[2][j]) + 
                (lhs.m[i][3] * rhs.m[3][j]);
        }
    }
    return result;
}

vec4_t Matrix4MultiplyV(mat4_t m, vec4_t v)
{
    vec4_t result = {
        .x = (m.m[0][0] * v.x) + (m.m[0][1] * v.y) + (m.m[0][2] * v.z) + (m.m[0][3] * v.w),
        .y = (m.m[1][0] * v.x) + (m.m[1][1] * v.y) + (m.m[1][2] * v.z) + (m.m[1][3] * v.w),
        .z = (m.m[2][0] * v.x) + (m.m[2][1] * v.y) + (m.m[2][2] * v.z) + (m.m[2][3] * v.w),
        .w = (m.m[3][0] * v.x) + (m.m[3][1] * v.y) + (m.m[3][2] * v.z) + (m.m[3][3] * v.w)
    };
    return result;
}
