#include "pch.h"
#include "Display.h"
#include "Triangle.h"

void Swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void FillFlatBottomTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    float firstSlope = (x1 - x0)/(float)(y1 - y0);
    float secondSlope = (x2 - x0)/(float)(y2 - y0);

    float startX = (float)x0;
    float endX = (float)x0;

    for (int y = y0; y <= y2; ++y)
    {
        DrawLine((int)startX, y, (int)endX, y, color);
        startX += firstSlope;
        endX += secondSlope;
    }
}

void FillFlatTopTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    float firstSlope = (x0 - x2)/(float)(y2 - y0);
    float secondSlope = (x1 - x2)/(float)(y2 - y1);

    float startX = (float)x2;
    float endX = (float)x2;

    for (int y = y2; y >= y0; --y)
    {
        DrawLine((int)startX, y, (int)endX, y, color);
        startX += firstSlope;
        endX += secondSlope;
    }
}

void DrawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
    // First, sort vertices by ascending y coordinate
    if (y0 > y1)
    {
        Swap(&y0, &y1);
        Swap(&x0, &x1);
    }
    if (y1 > y2)
    {
        Swap(&x1, &x2);
        Swap(&y1, &y2);
    }
    if (y0 > y1)
    {
        Swap(&y0, &y1);
        Swap(&x0, &x1);
    }

    if (y1 == y2)
    {
        FillFlatBottomTriangle(x0, y0, x1, y1, x2, y2, color);
    }
    else if (y0 == y1)
    {
        FillFlatTopTriangle(x0, y0, x1, y1, x2, y2, color);
    }
    else
    {
        // Calculate mid-point vertex (Mx, My)
        int mx = (int)(((x2 - x0) * (y1 - y0)) / (float)(y2 - y0) + x0);
        int my = y1;

        // Draw flat-bottom triangle
        FillFlatBottomTriangle(x0, y0, x1, y1, mx, my, color);

        // Draw flat-top triangle
        FillFlatTopTriangle(x1, y1, mx, my, x2, y2, color);
    }
}
