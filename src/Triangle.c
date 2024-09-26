#include "pch.h"
#include "Display.h"
#include "Swap.h"
#include "Triangle.h"

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
        SwapInt(&y0, &y1);
        SwapInt(&x0, &x1);
    }
    if (y1 > y2)
    {
        SwapInt(&x1, &x2);
        SwapInt(&y1, &y2);
    }
    if (y0 > y1)
    {
        SwapInt(&y0, &y1);
        SwapInt(&x0, &x1);
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

vec3_t BarycentricWeights(vec2_t a, vec2_t b, vec2_t c, vec2_t p) {
    // Find the vectors between the vertices ABC and point p
    vec2_t ac = Vec2Subtract(c, a);
    vec2_t ab = Vec2Subtract(b, a);
    vec2_t ap = Vec2Subtract(p, a);
    vec2_t pc = Vec2Subtract(c, p);
    vec2_t pb = Vec2Subtract(b, p);

    // Compute the area of the full parallegram/triangle ABC using 2D cross product
    float area_parallelogram_abc = (ac.x * ab.y - ac.y * ab.x); // || AC x AB ||

    // Alpha is the area of the small parallelogram/triangle PBC divided by the area of the full parallelogram/triangle ABC
    float alpha = (pc.x * pb.y - pc.y * pb.x) / area_parallelogram_abc;

    // Beta is the area of the small parallelogram/triangle APC divided by the area of the full parallelogram/triangle ABC
    float beta = (ac.x * ap.y - ac.y * ap.x) / area_parallelogram_abc;

    // Weight gamma is easily found since barycentric coordinates always add up to 1.0
    float gamma = 1 - alpha - beta;

    vec3_t weights = { alpha, beta, gamma };
    return weights;
}

void DrawTexel(int x, int y, uint32_t* texture, vec2_t pointA, vec2_t pointB, vec2_t pointC,
    float u0, float v0, float u1, float v1, float u2, float v2)
{
    vec2_t pointP = { (float)x, (float)y };
    vec3_t weights = BarycentricWeights(pointA, pointB, pointC, pointP);
    float alpha = weights.x;
    float beta = weights.y;
    float gamma = weights.z;
    float interpolatedU = u0 * alpha + u1 * beta + u2 * gamma;
    float interpolatedV = v0 * alpha + v1 * beta + v2 * gamma;
    int textureX = (int)(interpolatedU * g_textureWidth);
    int textureY = (int)(interpolatedV * g_textureHeight);
    int textureIndex = (g_textureWidth * textureY) + textureX;
    if ((textureIndex >= 0) && (textureIndex < (g_textureWidth * g_textureHeight)))
    {
        DrawPixel(x, y, texture[textureIndex]);
        // printf("A (%.2f, %.2f) B(%.2f, %.2f) C(%.2f, %.2f), P(%.2f, %.2f) UV(%.2f, %.2f) T(%d, %d)\n",
        //     pointA.x, pointA.y, pointB.x, pointB.y, pointC.x, pointC.y, pointP.x, pointP.y, interpolatedU, interpolatedV, textureX, textureY);
    }
}

void DrawTexturedTriangle(int x0, int y0, float u0, float v0,
    int x1, int y1, float u1, float v1, int x2, int y2, float u2, float v2,
    uint32_t* texture)
{
    // First, sort vertices by ascending y coordinate
    if (y0 > y1)
    {
        SwapInt(&y0, &y1);
        SwapInt(&x0, &x1);
        SwapFloat(&u0, &u1);
        SwapFloat(&v0, &v1);
    }
    if (y1 > y2)
    {
        SwapInt(&x1, &x2);
        SwapInt(&y1, &y2);
        SwapFloat(&u1, &u2);
        SwapFloat(&v1, &v2);
    }
    if (y0 > y1)
    {
        SwapInt(&y0, &y1);
        SwapInt(&x0, &x1);
        SwapFloat(&u0, &u1);
        SwapFloat(&v0, &v1);
    }

    // Create vector points
    vec2_t pointA = { (float)x0, (float)y0 };
    vec2_t pointB = { (float)x1, (float)y1 };
    vec2_t pointC = { (float)x2, (float)y2 };

    // Render the upper part of the triangle (flat bottom)
    float inverseLeftSlope = 0.0f;
    float inverseRightSlope = 0.0f;
    if ((y1 - y0) != 0)
    {
        inverseLeftSlope = (float)(x1 - x0) / abs(y1 - y0);
    }
    if ((y2 - y0) != 0)
    {
        inverseRightSlope = (float)(x2 - x0) / abs(y2 - y0);
    }
    if ((y1 - y0) != 0)
    {
        for (int y = y0; y <= y1; ++y)
        {
            int xStart = (int)(x1 + (y - y1) * inverseLeftSlope);
            int xEnd = (int)(x0 + (y - y0) * inverseRightSlope);
            if (xEnd < xStart)
            {
                // Make sure we're filling from left to right
                SwapInt(&xStart, &xEnd);
            }
            for (int x = xStart; x < xEnd; ++x)
            {
                // Paint the pixel
                DrawTexel(x, y, texture, pointA, pointB, pointC, u0, v0, u1, v1, u2, v2);
            }
        }
    }

    // Render the lower part of the triangle (flat top)
    inverseLeftSlope = 0.0f;
    inverseRightSlope = 0.0f;
    if ((y2 - y1) != 0)
    {
        inverseLeftSlope = (float)(x2 - x1) / abs(y2 - y1);
    }
    if ((y2 - y0) != 0)
    {
        inverseRightSlope = (float)(x2 - x0) / abs(y2 - y0);
    }
    if ((y2 - y1) != 0)
    {
        for (int y = y1; y <= y2; ++y)
        {
            int xStart = (int)(x1 + (y - y1) * inverseLeftSlope);
            int xEnd = (int)(x0 + (y - y0) * inverseRightSlope);
            if (xEnd < xStart)
            {
                // Make sure we're filling from left to right
                SwapInt(&xStart, &xEnd);
            }
            for (int x = xStart; x < xEnd; ++x)
            {
                // Paint the pixel
                DrawTexel(x, y, texture, pointA, pointB, pointC, u0, v0, u1, v1, u2, v2);
            }
        }
    }
}
