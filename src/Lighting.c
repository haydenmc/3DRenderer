#include "pch.h"
#include "Lighting.h"

static light_t g_light = {
    .direction = { 0.0f, 0.0f, 1.0f },
};

void InitializeLight(vec3_t direction)
{
    g_light.direction = direction;
}

uint32_t LightApplyIntensity(uint32_t originalColor, float factor)
{
    uint32_t a = (originalColor & 0xFF000000);
    uint32_t r = (uint32_t)((originalColor & 0x00FF0000) * factor);
    uint32_t g = (uint32_t)((originalColor & 0x0000FF00) * factor);
    uint32_t b = (uint32_t)((originalColor & 0x000000FF) * factor);
    uint32_t newColor = a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);
    return newColor;
}

uint32_t LightCalculateColorForFace(vec3_t faceNormal, uint32_t originalColor)
{
    // Assume both vectors are normalized.
    // Invert the light direction to get the "source" and compare to the face
    // normal via dot product to determine the "intensity factor"
    vec3_t lightSourceDirection = Vec3Multiply(g_light.direction, -1.0f);
    float intensityFactor = Vec3DotProduct(faceNormal, lightSourceDirection);
    // Clamp to [0, 1]
    intensityFactor = min(1.0f, max(0.0f, intensityFactor));
    return LightApplyIntensity(originalColor, intensityFactor);
}