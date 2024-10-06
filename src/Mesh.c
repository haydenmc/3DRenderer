#include "pch.h"
#include <array/array.h>
#include "Mesh.h"

#define MAX_MESHES 16

uint32_t g_faceColors[] = {
    0xFF999999,
};

static mesh_t g_meshes[MAX_MESHES];
static int g_meshCount = 0;

void LoadMeshObjData(mesh_t* targetMesh, char* filename)
{
    FILE* objFile = fopen(filename, "r");
    char line[256];

    if (objFile != NULL)
    {
        int colorIndex = 0;
        tex2_t* textureCoordinates = NULL;
        while (fgets(line, sizeof(line), objFile))
        {
            vec3_t vertex;
            face_t face;
            int textureCoordinateIndices[3];
            tex2_t textureCoordinate;
            if (sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z) == 3)
            {
                array_push(targetMesh->vertices, vertex);
            }
            else if (sscanf(line, "vt %f %f", &textureCoordinate.u, &textureCoordinate.v) == 2)
            {
                array_push(textureCoordinates, textureCoordinate);
            }
            else if (sscanf(line, "f %d/%d/%*d %d/%d/%*d %d/%d/%*d",
                &face.a, &textureCoordinateIndices[0],
                &face.b, &textureCoordinateIndices[1],
                &face.c,&textureCoordinateIndices[2]) == 6)
            {
                face.a -= 1; // Convert to 0-indexed
                face.b -= 1;
                face.c -= 1;
                face.a_uv = textureCoordinates[textureCoordinateIndices[0] - 1];
                face.b_uv = textureCoordinates[textureCoordinateIndices[1] - 1];
                face.c_uv = textureCoordinates[textureCoordinateIndices[2] - 1];
                face.color = g_faceColors[colorIndex];
                if (colorIndex == (sizeof(g_faceColors) / sizeof(g_faceColors[0])) - 1)
                {
                    colorIndex = 0;
                }
                else
                {
                    ++colorIndex;
                }
                array_push(targetMesh->faces, face);
            }
        }
        array_free(textureCoordinates);
        fclose(objFile);
    }
    else
    {
        int error = errno;
        printf("error %d", error);
        exit(1);
    }
}

void LoadMeshPngData(mesh_t* targetMesh, char* texturePngFile)
{
    upng_t* pngImage = upng_new_from_file(texturePngFile);
    if (pngImage != NULL)
    {
        upng_decode(pngImage);
        if (upng_get_error(pngImage) == UPNG_EOK)
        {
            uint32_t* textureBuffer = (uint32_t*)upng_get_buffer(pngImage);
            int textureWidth = upng_get_width(pngImage);
            int textureHeight = upng_get_height(pngImage);

            // Convert to ARGB8888
            for (int i = 0; i < textureWidth * textureHeight; ++i)
            {
                uint32_t color = textureBuffer[i];
                uint32_t a = (color & 0xFF000000);
                uint32_t r = (color & 0x00FF0000) >> 16;
                uint32_t g = (color & 0x0000FF00);
                uint32_t b = (color & 0x000000FF) << 16;
                textureBuffer[i] = (a | r | g | b);
            }

            // Set mesh texture
            targetMesh->texture = pngImage;
        }
    }
}

void LoadMesh(char *meshObjFile, char *texturePngFile, vec3_t scale,
    vec3_t rotation, vec3_t translation)
{
    mesh_t* targetMesh = &g_meshes[g_meshCount];
    LoadMeshObjData(targetMesh, meshObjFile);
    LoadMeshPngData(targetMesh, texturePngFile);
    targetMesh->scale = scale;
    targetMesh->rotation = rotation;
    targetMesh->translation = translation;
    g_meshCount++;
}

int GetNumMeshes(void)
{
    return g_meshCount;
}

mesh_t* GetMeshAtIndex(int index)
{
    return &g_meshes[index];
}

void FreeMeshes(void)
{
    for (int i = 0; i < g_meshCount; ++i)
    {
        mesh_t* mesh = &g_meshes[i];
        upng_free(mesh->texture);
        array_free(mesh->faces);
        array_free(mesh->vertices);
    }
}
