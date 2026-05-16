#include "Cell.h"

#include <SDL3/SDL_render.h>
#include <cmath>

void CCell::Update( float deltaTime )
{
	if ( mTargetSize == mSize )
	{
		return;
	}
	
	const float t = 1.0f - std::exp( -kSizeChangeSpeed * deltaTime );
	mSize = std::lerp( mSize, mTargetSize, t );
	
	if ( std::fabsf( mTargetSize - mSize ) <= kSizeLerpError )
	{
		mSize = mTargetSize;
	}
}

void CCell::Render( SDL_Renderer* renderer, SDL_Texture* atlasTexture )
{
	const SDL_FRect dstRect = {
		mPosition.x - mSize / 2.0f,
		mPosition.y - mSize / 2.0f,
		mSize,
		mSize
	};
	
	SDL_RenderTexture( renderer, atlasTexture, &kCellSpriteRects[mType], &dstRect );
}
