#pragma once

#include "Types.h"

#include <SDL3/SDL_rect.h>

constexpr int32     kCellSpriteSize     = 24;

// Each index corresponds to ECellType enum
constexpr SDL_FRect kCellSpriteRects[]  =  {
	{ 0, 32, kCellSpriteSize, kCellSpriteSize }, // None
	{ 32, 0, kCellSpriteSize, kCellSpriteSize }, // Cross
	{ 0,  0, kCellSpriteSize, kCellSpriteSize }, // Circle
};
