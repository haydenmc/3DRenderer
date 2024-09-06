#include "pch.h"
#include <array/array.h>
#include "Mesh.h"

vec3_t g_cubeVertices[N_CUBE_VERTICES] = {
    { .x = -1, .y = -1, .z = -1 }, // 1
    { .x = -1, .y =  1, .z = -1 }, // 2
    { .x =  1, .y =  1, .z = -1 }, // 3
    { .x =  1, .y = -1, .z = -1 }, // 4
    { .x =  1, .y =  1, .z =  1 }, // 5
    { .x =  1, .y = -1, .z =  1 }, // 6
    { .x = -1, .y =  1, .z =  1 }, // 7
    { .x = -1, .y = -1, .z =  1 }, // 8
};

face_t g_cubeFaces[N_CUBE_FACES] = {
    // front
    { .a = 1, .b = 2, .c = 3 },
    { .a = 1, .b = 3, .c = 4 },
    // right
    { .a = 4, .b = 3, .c = 5 },
    { .a = 4, .b = 5, .c = 6 },
    // back
    { .a = 6, .b = 5, .c = 7 },
    { .a = 6, .b = 7, .c = 8 },
    // left
    { .a = 8, .b = 7, .c = 2 },
    { .a = 8, .b = 2, .c = 1 },
    // top
    { .a = 2, .b = 7, .c = 5 },
    { .a = 2, .b = 5, .c = 3 },
    // bottom
    { .a = 6, .b = 8, .c = 1 },
    { .a = 6, .b = 1, .c = 4 },
};

mesh_t g_Mesh = {
    .vertices = NULL,
    .faces = NULL,
    .rotation = { 0, 0, 0 }
};

void LoadCubeMeshData(void)
{
    for (int i = 0; i < N_CUBE_VERTICES; ++i)
    {
        vec3_t cubeVertex = g_cubeVertices[i];
        array_push(g_Mesh.vertices, cubeVertex);
    }
    for (int i = 0; i < N_CUBE_FACES; ++i)
    {
        face_t cubeFace = g_cubeFaces[i];
        array_push(g_Mesh.faces, cubeFace);
    }
}

void LoadObjFileData(char* filename)
{
    FILE* objFile = fopen(filename, "r");
    char line[256];
    if (objFile != NULL)
    {
        while (fgets(line, sizeof(line), objFile))
        {
            vec3_t vertex;
            face_t face;
            if (sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z) == 3)
            {
                array_push(g_Mesh.vertices, vertex);
            }
            else if (sscanf(line, "f %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d",
                &face.a, &face.b, &face.c) == 3)
            {
                array_push(g_Mesh.faces, face);
            }
        }
    }
    fclose(objFile);
}