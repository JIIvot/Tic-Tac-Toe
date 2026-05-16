#pragma once

#include "Entity.h"
#include "SpriteData.h"

#include <SDL3/SDL_rect.h>

class CCell : public CEntity
{
public:
	enum ECellType : uint8
	{
		eCellType_None = 0,
		eCellType_Cross,
		eCellType_Circle
	};
	
public:
	CCell( SDL_FPoint position )
		: mPosition( position )
	{
	}
	
	void Update( float deltaTime ) override;
	void Render( SDL_Renderer* renderer, SDL_Texture* atlasTexture ) override;
	
	void SetNormalSize()
	{
		mTargetSize = kNormalSize;
	}
	
	void SetHoveredSize()
	{
		mTargetSize = kHoveredSize;
	}
	
	void SetClosedSize()
	{
		mTargetSize = kClosedSize;
	}
	
public:
	ECellType mType = eCellType_None;
	
private:
	static constexpr float kNormalSize      = kCellSpriteSize * 4;
	static constexpr float kHoveredSize     = kCellSpriteSize * 5;
	static constexpr float kClosedSize      = kCellSpriteSize * 3;
	static constexpr float kSizeLerpError   = 0.1f;
	static constexpr float kSizeChangeSpeed = 5.0f;
	
	SDL_FPoint mPosition;
	float      mSize       = kNormalSize;
	float      mTargetSize = kNormalSize;
};
