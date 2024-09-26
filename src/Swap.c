#include "pch.h"
#include "Swap.h"

void SwapInt(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void SwapFloat(float* a, float* b)
{
    float temp = *a;
    *a = *b;
    *b = temp;
}