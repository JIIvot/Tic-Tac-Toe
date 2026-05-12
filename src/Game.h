#pragma once

#include "Types.h"
#include "Field.h"

#include <SDL3/SDL_rect.h>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;
class CInput;

class CGame
{
public:
	static void Start();
	
	void Quit()
	{
		mShouldRun = false;
	}
	
	[[nodiscard]]
	static CGame* GetInstance()
	{
		return sInstance;
	}
	
	[[nodiscard]]
	float GetDeltaTime() const
	{
		return mDeltaTime;
	}
	
	[[nodiscard]]
	const CInput* GetInput() const
	{
		return mInput;
	}
	
	[[nodiscard]]
	SDL_FPoint GetRenderMouseCoordinates() const;
	
private:
	CGame();
	~CGame();
	
	void Run();
	
	void UpdateDeltaTime();
	void ProcessEvents();
	void Update();
	void Render();
	
public:
	static constexpr int32     kGameWidth      = 360;
	static constexpr int32     kGameHeight     = 360;
	static constexpr SDL_FRect kBackgroundRect = { 0, 0, kGameWidth, kGameHeight };
	
	static constexpr int32     kFieldWidth     = 3;
	static constexpr int32     kFieldHeight    = 3;
	
private:
	static CGame* sInstance;
	
	CInput*       mInput           = nullptr;
	
	SDL_Window*   mWindow          = nullptr;
	SDL_Renderer* mRenderer        = nullptr;
	SDL_Texture*  mAtlasTexture    = nullptr;

	float         mDeltaTime       = 0.0f;
	float         mPerfFrequency;
	uint64        mLastPerfCounter = 0;

	bool          mShouldRun       = false;
	bool          mIsFullscreen    = false;
	
	CField        mField           = CField( { kFieldWidth, kFieldHeight }, 3 );
};
