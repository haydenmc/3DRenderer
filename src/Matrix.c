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

mat4_t Matrix4MakePerspective(float fov, float aspect, float zNear, float zFar)
{
    mat4_t m = {{{ 0.0f }}};
    m.m[0][0] = aspect * (1 / tanf(fov / 2.0f));
    m.m[1][1] = 1 / tanf(fov / 2.0f);
    m.m[2][2] = zFar / (zFar - zNear);
    m.m[2][3] = (-zFar * zNear) / (zFar - zNear);
    m.m[3][2] = 1.0f;
    return m;
}

mat4_t Matrix4MakeLookAt(vec3_t eye, vec3_t target, vec3_t up)
{
    vec3_t z = Vec3Subtract(target, eye);
    z = Vec3Normalize(z);
    vec3_t x = Vec3CrossProduct(up, z);
    x = Vec3Normalize(x);
    vec3_t y = Vec3CrossProduct(z, x);

    mat4_t viewMatrix = {{
        { x.x, x.y, x.z, -Vec3DotProduct(x, eye) },
        { y.x, y.y, y.z, -Vec3DotProduct(y, eye) },
        { z.x, z.y, z.z, -Vec3DotProduct(z, eye) },
        {   0,   0,   0,                       1 }
    }};
    return viewMatrix;
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

vec4_t Matrix4MultiplyVProject(mat4_t projectionMatrix, vec4_t v)
{
    vec4_t result = Matrix4MultiplyV(projectionMatrix, v);

    // Perform perspective divide with original z value that is now stored in w
    if (result.w != 0)
    {
        result.x /= result.w;
        result.y /= result.w;
        result.z /= result.w;
    }

    return result;
}
