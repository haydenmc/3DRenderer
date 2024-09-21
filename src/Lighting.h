#pragma once
#include "Vector.h"

typedef struct {
    vec3_t direction;
} light_t;

uint32_t LightApplyIntensity(uint32_t originalColor, float factor);
uint32_t LightCalculateColorForFace(vec3_t faceNormal, light_t light, uint32_t originalColor);