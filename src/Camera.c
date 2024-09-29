#include "pch.h"
#include "Camera.h"

camera_t g_camera = {
    .position = { 0.0f, 0.0f, 0.0f },
    .direction = { 0.0f, 0.0f, 1.0f },
    .forwardVelocity = { 0.0f, 0.0f, 0.0f },
    .yaw = 0.0f,
};