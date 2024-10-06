#include "pch.h"
#include "Camera.h"

static camera_t g_camera = {
    .position = { 0.0f, 0.0f, 0.0f },
    .direction = { 0.0f, 0.0f, 1.0f },
    .yaw = 0.0f,
    .pitch = 0.0f,
};

camera_t GetCamera(void)
{
    return g_camera;
}

void SetCameraPosition(vec3_t position)
{
    g_camera.position = position;
}

void SetCameraDirection(vec3_t direction)
{
    g_camera.direction = direction;
}

void SetCameraYaw(float yaw)
{
    g_camera.yaw = yaw;
}

void SetCameraPitch(float pitch)
{
    g_camera.pitch = pitch;
}
