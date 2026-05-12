#pragma once

#include "Types.h"

#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_mouse.h>

class CInput
{
public:
	CInput();
	
	~CInput()
	{
		delete[] mLastKeyboardState;
	}
	
	void Update();
	
	[[nodiscard]]
	bool IsKeyPressed( SDL_Scancode code ) const
	{
		return mCurrKeyboardState[code];
	}
	
	[[nodiscard]]
	bool IsKeyJustPressed( SDL_Scancode code ) const
	{
		return mCurrKeyboardState[code] && !mLastKeyboardState[code];
	}
	
	[[nodiscard]]
	bool IsButtonPressed( uint32 button ) const
	{
		return mCurrButtonFlags & SDL_BUTTON_MASK( button );
	}
	
	[[nodiscard]]
	bool IsButtonJustPressed( uint32 button ) const
	{
		return ( mCurrButtonFlags & SDL_BUTTON_MASK( button ) ) && !( mLastButtonFlags & SDL_BUTTON_MASK( button ) );
	}
	
	[[nodiscard]]
	SDL_FPoint GetMouseCoordinates() const
	{
		return mMouseCoordinates;
	}
	
private:
	const bool*          mCurrKeyboardState;
	bool*                mLastKeyboardState;
	int32                mKeysNum;
	
	SDL_FPoint           mMouseCoordinates = { 0.0f, 0.0f };
	SDL_MouseButtonFlags mCurrButtonFlags  = 0;
	SDL_MouseButtonFlags mLastButtonFlags  = 0;
};
