#pragma once

extern int g_windowWidth;
extern int g_windowHeight;
extern SDL_Window* g_window;
extern SDL_Renderer* g_renderer;
extern uint32_t* g_colorBuffer;
extern SDL_Texture* g_colorBufferTexture;

bool InitializeWindow(void);
void DrawGrid(uint32_t gridSize, uint32_t gridColor);
void DrawRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color);
void RenderColorBuffer(void);
void ClearColorBuffer(uint32_t color);
void DestroyWindow(void);