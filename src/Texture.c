#include "pch.h"
#include "Texture.h"

int g_textureWidth = 64;
int g_textureHeight = 64;

upng_t* g_pngTexture = NULL;
uint32_t* g_meshTexture = NULL;

void LoadPngTextureData(char *pngFilePath)
{
    g_pngTexture = upng_new_from_file(pngFilePath);
    if (g_pngTexture != NULL)
    {
        upng_decode(g_pngTexture);
        if (upng_get_error(g_pngTexture) == UPNG_EOK)
        {
            g_meshTexture = (uint32_t*)upng_get_buffer(g_pngTexture);
            g_textureWidth = upng_get_width(g_pngTexture);
            g_textureHeight = upng_get_height(g_pngTexture);

            // Convert to ARGB8888
            for (int i = 0; i < g_textureWidth * g_textureHeight; ++i)
            {
                uint32_t color = g_meshTexture[i];
                uint32_t a = (color & 0xFF000000);
                uint32_t r = (color & 0x00FF0000) >> 16;
                uint32_t g = (color & 0x0000FF00);
                uint32_t b = (color & 0x000000FF) << 16;
                g_meshTexture[i] = (a | r | g | b);
            }
        }
    }
}
