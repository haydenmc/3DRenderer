#pragma once

#define FPS 60.0f
#define FRAME_TARGET_TIME (1000 / FPS)

extern int g_windowWidth;
extern int g_windowHeight;
extern SDL_Window* g_window;
extern SDL_Renderer* g_renderer;
extern uint32_t* g_colorBuffer;
extern float* g_zBuffer;
extern SDL_Texture* g_colorBufferTexture;

bool InitializeWindow(void);
void DrawGrid(int gridSize, uint32_t gridColor);
void DrawPixel(int x, int y, uint32_t color);
void DrawFilledRect(int x, int y, int width, int height, uint32_t color);
void DrawRect(int x, int y, int width, int height, uint32_t color);
void DrawLine(int x0, int y0, int x1, int y1, uint32_t color);
void DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
void RenderColorBuffer(void);
void ClearZBuffer(void);
void ClearColorBuffer(uint32_t color);
void DestroyWindow(void);