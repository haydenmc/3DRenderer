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

typedef struct {
    float x;
    float y;
    float z;
    float w;
} vec4_t;

// Vector 2D Functions
void Vec2Swap(vec2_t* a, vec2_t* b);
float Vec2Length(vec2_t v);
vec2_t Vec2Add(vec2_t lhs, vec2_t rhs);
vec2_t Vec2Subtract(vec2_t lhs, vec2_t rhs);
vec2_t Vec2Multiply(vec2_t v, float factor);
vec2_t Vec2Divide(vec2_t v, float factor);
float Vec2DotProduct(vec2_t lhs, vec2_t rhs);
vec2_t Vec2Normalize(vec2_t v);

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

// Vector 4D Functions
void Vec4Swap(vec4_t* a, vec4_t* b);

// Vector Conversion Functions
vec4_t Vec4FromVec3(vec3_t v);
vec3_t Vec3FromVec4(vec4_t v);
vec2_t Vec2FromVec4(vec4_t v);