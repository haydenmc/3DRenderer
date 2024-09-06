#pragma once
#include "Triangle.h"
#include "Vector.h"

#define N_CUBE_VERTICES 8
#define N_CUBE_FACES (6 * 2) // 6 cube faces with 2 triangles per face
extern vec3_t g_cubeVertices[N_CUBE_VERTICES];
extern face_t g_cubeFaces[N_CUBE_FACES];

typedef struct {
    vec3_t* vertices; // dynamic array of vertices
    face_t* faces;    // dynamic array of faces
    vec3_t rotation;
} mesh_t;

extern mesh_t g_Mesh;

void LoadCubeMeshData(void);

void LoadObjFileData(char* filename);