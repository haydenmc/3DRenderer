#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// Disable warnings for SDL
#pragma warning(push, 0)
#define SDL_MAIN_HANDLED 1
#include <SDL.h>
#pragma warning(pop)