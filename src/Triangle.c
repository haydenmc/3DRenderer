#include "pch.h"
#include "Display.h"
#include "Swap.h"
#include "Triangle.h"

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

void DrawTrianglePixel(int x, int y, uint32_t color, vec4_t pointA, vec4_t pointB, vec4_t pointC)
{
    if ((x >= GetWindowWidth()) || (y >= GetWindowHeight()))
    {
        return;
    }
    vec2_t p = { (float)x, (float)y };
    vec2_t a = Vec2FromVec4(pointA);
    vec2_t b = Vec2FromVec4(pointB);
    vec2_t c = Vec2FromVec4(pointC);
    vec3_t weights = BarycentricWeights(a, b, c, p);
    float alpha = weights.x;
    float beta = weights.y;
    float gamma = weights.z;

    float interpolatedReciprocalW = (1 / pointA.w) * alpha +
        (1 / pointB.w) * beta + (1 / pointC.w) * gamma;
    // Adjust 1/w so closer pixels have smaller values.
    float depthValue = 1.0f - interpolatedReciprocalW;
    // Only draw pixel if depth value is less than previously stored
    if (depthValue < GetZBufferValue(x, y))
    {
        SetZBufferValue(x, y, depthValue);
        DrawPixel(x, y, color);
    }
}

void DrawFilledTriangle(vec4_t a, vec4_t b, vec4_t c, uint32_t color)
{
    // First, sort vertices by descending y coordinate
    if (a.y > b.y)
    {
        Vec4Swap(&a, &b);
    }
    if (b.y > c.y)
    {
        Vec4Swap(&b, &c);
    }
    if (a.y > b.y)
    {
        Vec4Swap(&a, &b);
    }
    int ax = (int)a.x;
    int ay = (int)a.y;
    int bx = (int)b.x;
    int by = (int)b.y;
    int cx = (int)c.x;
    int cy = (int)c.y;

    // Render the upper part of the triangle (flat bottom)
    float inverseLeftSlope = 0.0f;
    float inverseRightSlope = 0.0f;
    if ((by - ay) != 0)
    {
        inverseLeftSlope = (float)(bx - ax) / abs(by - ay);
    }
    if ((cy - ay) != 0)
    {
        inverseRightSlope = (float)(cx - ax) / abs(cy - ay);
    }
    if ((by - ay) != 0)
    {
        for (int y = ay; y <= by; ++y)
        {
            int xStart = (int)(bx + (y - by) * inverseLeftSlope);
            int xEnd = (int)(ax + (y - ay) * inverseRightSlope);
            if (xEnd < xStart)
            {
                // Make sure we're filling from left to right
                SwapInt(&xStart, &xEnd);
            }
            for (int x = xStart; x < xEnd; ++x)
            {
                // Paint the pixel
                DrawTrianglePixel(x, y, color, a, b, c);
            }
        }
    }

    // Render the lower part of the triangle (flat top)
    inverseLeftSlope = 0.0f;
    inverseRightSlope = 0.0f;
    if ((cy - by) != 0)
    {
        inverseLeftSlope = (float)(cx - bx) / abs(cy - by);
    }
    if ((cy - ay) != 0)
    {
        inverseRightSlope = (float)(cx - ax) / abs(cy - ay);
    }
    if ((cy - by) != 0)
    {
        for (int y = by; y <= (int)cy; ++y)
        {
            int xStart = (int)(bx + (y - by) * inverseLeftSlope);
            int xEnd = (int)(ax + (y - ay) * inverseRightSlope);
            if (xEnd < xStart)
            {
                // Make sure we're filling from left to right
                SwapInt(&xStart, &xEnd);
            }
            for (int x = xStart; x < xEnd; ++x)
            {
                // Paint the pixel
                DrawTrianglePixel(x, y, color, a, b, c);
            }
        }
    }
}

void DrawTexel(int x, int y, upng_t* texture, vec4_t pointA, vec4_t pointB, vec4_t pointC,
    tex2_t uvA, tex2_t uvB, tex2_t uvC)
{
    if ((x >= GetWindowWidth()) || (y >= GetWindowHeight()))
    {
        return;
    }
    int textureWidth = upng_get_width(texture);
    int textureHeight = upng_get_height(texture);
    uint32_t* textureBuffer = (uint32_t*)upng_get_buffer(texture);
    vec2_t p = { (float)x, (float)y };
    vec2_t a = Vec2FromVec4(pointA);
    vec2_t b = Vec2FromVec4(pointB);
    vec2_t c = Vec2FromVec4(pointC);
    vec3_t weights = BarycentricWeights(a, b, c, p);
    float alpha = weights.x;
    float beta = weights.y;
    float gamma = weights.z;

    float interpolatedU;
    float interpolatedV;
    float interpolatedReciprocalW;

    interpolatedU = (uvA.u / pointA.w) * alpha + (uvB.u / pointB.w) * beta + (uvC.u / pointC.w) * gamma;
    interpolatedV = (uvA.v / pointA.w) * alpha + (uvB.v / pointB.w) * beta + (uvC.v / pointC.w) * gamma;
    interpolatedReciprocalW = (1 / pointA.w) * alpha + (1 / pointB.w) * beta + (1 / pointC.w) * gamma;

    interpolatedU /= interpolatedReciprocalW;
    interpolatedV /= interpolatedReciprocalW;

    // Intelligently clamp with wraparound to [0, 1]
    // (apparently some OBJ files use UV values > 1 to 'wrap around')
    interpolatedU = interpolatedU < 0.0f ? 0.0f :
        (interpolatedU > 1.0f ? (interpolatedU - floorf(interpolatedU)) : interpolatedU);
    interpolatedV = interpolatedV < 0.0f ? 0.0f :
        (interpolatedV > 1.0f ? (interpolatedV - floorf(interpolatedV)) : interpolatedV);

    int textureX = (int)(interpolatedU * textureWidth);
    int textureY = (int)(interpolatedV * textureHeight);
    int textureIndex = (textureWidth * textureY) + textureX;
    if ((textureIndex >= 0) && (textureIndex < (textureWidth * textureHeight)))
    {
        // Adjust 1/w so closer pixels have smaller values.
        float depthValue = 1.0f - interpolatedReciprocalW;
        // Only draw pixel if depth value is less than previously stored
        if (depthValue < GetZBufferValue(x, y))
        {
            SetZBufferValue(x, y, depthValue);
            DrawPixel(x, y, textureBuffer[textureIndex]);
        }
    }
}

void DrawTexturedTriangle(int x0, int y0, float z0, float w0, float u0, float v0,
    int x1, int y1, float z1, float w1, float u1, float v1,
    int x2, int y2, float z2, float w2, float u2, float v2,
    upng_t* texture)
{
    // First, sort vertices by ascending y coordinate
    if (y0 > y1)
    {
        SwapInt(&y0, &y1);
        SwapInt(&x0, &x1);
        SwapFloat(&z0, &z1);
        SwapFloat(&w0, &w1);
        SwapFloat(&u0, &u1);
        SwapFloat(&v0, &v1);
    }
    if (y1 > y2)
    {
        SwapInt(&x1, &x2);
        SwapInt(&y1, &y2);
        SwapFloat(&z1, &z2);
        SwapFloat(&w1, &w2);
        SwapFloat(&u1, &u2);
        SwapFloat(&v1, &v2);
    }
    if (y0 > y1)
    {
        SwapInt(&y0, &y1);
        SwapInt(&x0, &x1);
        SwapFloat(&z0, &z1);
        SwapFloat(&w0, &w1);
        SwapFloat(&u0, &u1);
        SwapFloat(&v0, &v1);
    }

    // Flip the V component to account for inverted UV coordinate space
    v0 = 1 - v0;
    v1 = 1 - v1;
    v2 = 1 - v2;

    // Create vector points
    vec4_t pointA = { (float)x0, (float)y0, z0, w0 };
    vec4_t pointB = { (float)x1, (float)y1, z1, w1 };
    vec4_t pointC = { (float)x2, (float)y2, z2, w2 };
    tex2_t uvA = { u0, v0 };
    tex2_t uvB = { u1, v1 };
    tex2_t uvC = { u2, v2 };

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
                DrawTexel(x, y, texture, pointA, pointB, pointC, uvA, uvB, uvC);
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
                DrawTexel(x, y, texture, pointA, pointB, pointC, uvA, uvB, uvC);
            }
        }
    }
}
