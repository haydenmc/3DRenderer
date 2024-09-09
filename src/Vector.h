#pragma once

typedef struct {
    float x;
    float y;
} vec2_t;

typedef struct {
    float x;
    float y;
    float z;
} vec3_t;

// Vector 2D Functions
float Vec2Length(vec2_t v);
vec2_t Vec2Add(vec2_t lhs, vec2_t rhs);
vec2_t Vec2Subtract(vec2_t lhs, vec2_t rhs);
vec2_t Vec2Multiply(vec2_t v, float factor);
vec2_t Vec2Divide(vec2_t v, float factor);
float Vec2DotProduct(vec2_t lhs, vec2_t rhs);
vec2_t Vec2Normalize(vec2_t v);
// TODO: Vec2Add, Vec2Subtract, Vec2Multiply, etc.

// Vector 3D Functions
float Vec3Length(vec3_t v);
vec3_t Vec3Add(vec3_t lhs, vec3_t rhs);
vec3_t Vec3Subtract(vec3_t lhs, vec3_t rhs);
vec3_t Vec3Multiply(vec3_t v, float factor);
vec3_t Vec3Divide(vec3_t v, float factor);
vec3_t Vec3CrossProduct(vec3_t lhs, vec3_t rhs);
float Vec3DotProduct(vec3_t lhs, vec3_t rhs);
vec3_t Vec3Normalize(vec3_t v);
vec3_t Vec3RotateX(vec3_t v, float angle);
vec3_t Vec3RotateY(vec3_t v, float angle);
vec3_t Vec3RotateZ(vec3_t v, float angle);
// TODO: Vec3Add, Vec3Subtract, Vec3Multiply, etc.