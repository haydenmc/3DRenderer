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

vec3_t Vec3RotateX(vec3_t v, float angle);
vec3_t Vec3RotateY(vec3_t v, float angle);
vec3_t Vec3RotateZ(vec3_t v, float angle);