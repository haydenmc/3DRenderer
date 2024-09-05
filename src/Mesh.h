#pragma once
#include "Triangle.h"
#include "Vector.h"

#define N_MESH_VERTICES 8
extern vec3_t g_meshVertices[N_MESH_VERTICES];

#define N_MESH_FACES (6 * 2) // 6 cube faces with 2 triangles per face
extern face_t g_meshFaces[N_MESH_FACES];