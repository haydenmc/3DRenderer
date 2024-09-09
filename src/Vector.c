#include "pch.h"
#include "Vector.h"
#include <math.h>

// Vector 2D Functions
float Vec2Length(vec2_t v)
{
    return sqrtf(v.x * v.x + v.y * v.y);
}

vec2_t Vec2Add(vec2_t lhs, vec2_t rhs)
{
    vec2_t result = {
        .x = lhs.x + rhs.x,
        .y = lhs.y + rhs.y,
    };
    return result;
}

vec2_t Vec2Subtract(vec2_t lhs, vec2_t rhs)
{
    vec2_t result = {
        .x = lhs.x - rhs.x,
        .y = lhs.y - rhs.y,
    };
    return result;
}

vec2_t Vec2Multiply(vec2_t v, float factor)
{
    vec2_t result = {
        .x = v.x * factor,
        .y = v.y * factor,
    };
    return result;
}

vec2_t Vec2Divide(vec2_t v, float factor)
{
    vec2_t result = {
        .x = v.x / factor,
        .y = v.y / factor,
    };
    return result;
}

float Vec2DotProduct(vec2_t lhs, vec2_t rhs)
{
    return (lhs.x * rhs.x) + (lhs.y * rhs.y);
}

vec2_t Vec2Normalize(vec2_t v)
{
    float length = Vec2Length(v);
    v.x = v.x / length;
    v.y = v.y / length;
    return v;
}

// Vector 3D Functions
float Vec3Length(vec3_t v)
{
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

vec3_t Vec3Add(vec3_t lhs, vec3_t rhs)
{
    vec3_t result = {
        .x = lhs.x + rhs.x,
        .y = lhs.y + rhs.y,
        .z = lhs.z + rhs.z,
    };
    return result;
}

vec3_t Vec3Subtract(vec3_t lhs, vec3_t rhs)
{
    vec3_t result = {
        .x = lhs.x - rhs.x,
        .y = lhs.y - rhs.y,
        .z = lhs.z - rhs.z,
    };
    return result;
}

vec3_t Vec3Multiply(vec3_t v, float factor)
{
    vec3_t result = {
        .x = v.x * factor,
        .y = v.y * factor,
        .z = v.z * factor,
    };
    return result;
}

vec3_t Vec3Divide(vec3_t v, float factor)
{
    vec3_t result = {
        .x = v.x / factor,
        .y = v.y / factor,
        .z = v.z / factor,
    };
    return result;
}

vec3_t Vec3CrossProduct(vec3_t lhs, vec3_t rhs)
{
    vec3_t result = {
        .x = lhs.y * rhs.z - lhs.z * rhs.y,
        .y = lhs.z * rhs.x - lhs.x * rhs.z,
        .z = lhs.x * rhs.y - lhs.y * rhs.x,
    };
    return result;
}

float Vec3DotProduct(vec3_t lhs, vec3_t rhs)
{
    return (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z);
}

vec3_t Vec3Normalize(vec3_t v)
{
    float length = Vec3Length(v);
    v.x = v.x / length;
    v.y = v.y / length;
    v.z = v.z / length;
    return v;
}

vec3_t Vec3RotateX(vec3_t v, float angle)
{
    vec3_t rotated_vector = {
        .x = v.x,
        .y = v.y * cosf(angle) - v.z * sinf(angle),
        .z = v.y * sinf(angle) + v.z * cosf(angle),
    };
    return rotated_vector;
}

vec3_t Vec3RotateY(vec3_t v, float angle)
{
    vec3_t rotated_vector = {
        .x = v.x * cosf(angle) - v.z * sinf(angle),
        .y = v.y,
        .z = v.x * sinf(angle) + v.z * cosf(angle),
    };
    return rotated_vector;
}

vec3_t Vec3RotateZ(vec3_t v, float angle)
{
    vec3_t rotated_vector = {
        .x = v.x * cosf(angle) - v.y * sinf(angle),
        .y = v.x * sinf(angle) + v.y * cosf(angle),
        .z = v.z,
    };
    return rotated_vector;
}