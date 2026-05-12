#include "Input.h"

#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_mouse.h>

CInput::CInput()
	: mCurrKeyboardState( SDL_GetKeyboardState( &mKeysNum ) )
	, mLastKeyboardState( new bool[mKeysNum] )
{
}

void CInput::Update()
{
	mLastButtonFlags = mCurrButtonFlags;
	mCurrButtonFlags = SDL_GetMouseState( &mMouseCoordinates.x, &mMouseCoordinates.y );
	
	memcpy( mLastKeyboardState, mCurrKeyboardState, mKeysNum * sizeof( bool ) );
}
