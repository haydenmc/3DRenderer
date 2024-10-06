#pragma once
#include "Triangle.h"
#include "Vector.h"

typedef struct {
    vec3_t* vertices; // dynamic array of vertices
    face_t* faces;    // dynamic array of faces
    upng_t* texture;
    vec3_t  rotation;
    vec3_t  scale;
    vec3_t  translation;
} mesh_t;

void LoadMesh(char* meshObjFile, char* texturePngFile, vec3_t scale,
    vec3_t rotation, vec3_t translation);
int GetNumMeshes(void);
mesh_t* GetMeshAtIndex(int index);
void FreeMeshes(void);