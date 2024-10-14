#pragma once

#define FPS 60.0f
#define FRAME_TARGET_TIME (1000 / FPS)

enum RenderMode
{
    RENDER_VERTICES                 = 1 << 0,
    RENDER_WIREFRAME                = 1 << 1,
    RENDER_FLAT_SHADING             = 1 << 2,
    RENDER_TEXTURED                 = 1 << 3,
    RENDER_ENABLE_BACK_FACE_CULLING = 1 << 4,
} RenderMode;

int GetWindowWidth(void);
int GetWindowHeight(void);
enum RenderMode GetRenderMode(void);
void SetRenderMode(enum RenderMode mode);

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
void CleanupWindow(void);
float GetZBufferValue(int x, int y);
void SetZBufferValue(int x, int y, float value);