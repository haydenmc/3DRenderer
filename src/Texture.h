#pragma once
#include <stdint.h>

typedef struct {
    float u;
    float v;
} tex2_t;

extern int g_textureWidth;
extern int g_textureHeight;

extern uint32_t* g_meshTexture;
extern const uint8_t REDBRICK_TEXTURE[];