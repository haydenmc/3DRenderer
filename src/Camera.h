#pragma once
#include "Vector.h"

typedef struct {
    vec3_t position;
    vec3_t direction;
    float yaw;
    float pitch;
} camera_t;

camera_t GetCamera(void);
void SetCameraPosition(vec3_t position);
void SetCameraDirection(vec3_t direction);
void SetCameraYaw(float yaw);
void SetCameraPitch(float pitch);