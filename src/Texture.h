#pragma once
#include <stdint.h>

typedef struct {
    float u;
    float v;
} tex2_t;

extern int g_textureWidth;
extern int g_textureHeight;

extern upng_t* g_pngTexture;
extern uint32_t* g_meshTexture;

void LoadPngTextureData(char* pngFilePath);