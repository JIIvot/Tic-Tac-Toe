#pragma once

struct SDL_Renderer;
struct SDL_Texture;

class CEntity
{
public:
	virtual ~CEntity() = default;
	
	virtual void Spawn()
	{
	}
	
	virtual void Update( float deltaTime )
	{
	}
	
	virtual void Render( SDL_Renderer* renderer, SDL_Texture* atlasTexture )
	{
	}
};
