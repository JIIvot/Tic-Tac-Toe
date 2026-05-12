#include "Game.h"
#include "Input.h"

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#define BACKGROUND_COLOR 255, 255, 255, 255

CGame* CGame::sInstance;

CGame::CGame()
{
	if ( !SDL_Init( SDL_INIT_VIDEO ) )
	{
		return;
	}
	
	mWindow = SDL_CreateWindow( "Tic-Tac-Toe", 720, 720, SDL_WINDOW_RESIZABLE );
	if ( !mWindow )
	{
		return;
	}
	
	mRenderer = SDL_CreateRenderer( mWindow, nullptr );
	if ( !mRenderer )
	{
		return;
	}
	
	mAtlasTexture  = IMG_LoadTexture( mRenderer, "res/atlas.png" );
	if ( !mAtlasTexture )
	{
		return;
	}
	
	SDL_SetTextureScaleMode( mAtlasTexture, SDL_SCALEMODE_NEAREST );
	SDL_SetRenderVSync( mRenderer, 1 );
	SDL_SetRenderLogicalPresentation( mRenderer, kGameWidth, kGameHeight, SDL_LOGICAL_PRESENTATION_LETTERBOX );
	
	mInput         = new CInput;
	mPerfFrequency = static_cast<float>( SDL_GetPerformanceFrequency() );
	mShouldRun     = true;
}

CGame::~CGame()
{
	if ( mInput )
	{
		delete mInput;
		mInput = nullptr;
	}
	
	SDL_DestroyTexture( mAtlasTexture );
	SDL_DestroyRenderer( mRenderer );
	SDL_DestroyWindow( mWindow );
	SDL_Quit();
}

void CGame::Start()
{
	if ( sInstance )
	{
		return;
	}
	
	sInstance = new CGame;
	sInstance->Run();
	
	delete sInstance;
	sInstance = nullptr;
}

void CGame::Run()
{
	mField.Spawn();
	
	mLastPerfCounter = SDL_GetPerformanceCounter();
	
	while ( mShouldRun )
	{
		UpdateDeltaTime();
		
		mInput->Update();
		ProcessEvents();
		
		Update();
		Render();
	}
}

void CGame::ProcessEvents()
{
	SDL_Event event;
	while ( SDL_PollEvent( &event ) )
	{
		if ( event.type == SDL_EVENT_QUIT )
		{
			Quit();
		}
	}
}

void CGame::UpdateDeltaTime()
{
	const uint64 currPerfCounter = SDL_GetPerformanceCounter();
	mDeltaTime       = static_cast<float>( currPerfCounter - mLastPerfCounter ) / mPerfFrequency;
	mLastPerfCounter = currPerfCounter;
}

void CGame::Update()
{
	// Exit action (ESCAPE)
	if ( mInput->IsKeyJustPressed( SDL_SCANCODE_ESCAPE ) )
	{
		Quit();
	}
	
	// Toggle fullscreen action (F11)
	if ( mInput->IsKeyJustPressed( SDL_SCANCODE_F11 ) )
	{
		mIsFullscreen = !mIsFullscreen;
		SDL_SetWindowFullscreen( mWindow, mIsFullscreen );
	}
	
	mField.Update( mDeltaTime );
}

void CGame::Render()
{
	SDL_SetRenderDrawColor( mRenderer, 0, 0, 0, 255 );
	SDL_RenderClear( mRenderer );
	
	SDL_SetRenderDrawColor( mRenderer, BACKGROUND_COLOR );
	SDL_RenderFillRect( mRenderer, &kBackgroundRect );
	
	mField.Render( mRenderer, mAtlasTexture );
	
	SDL_RenderPresent( mRenderer );
}

SDL_FPoint CGame::GetRenderMouseCoordinates() const
{
	const SDL_FPoint mouseCoordinates = mInput->GetMouseCoordinates();
	
	SDL_FPoint renderCoordinates;
	SDL_RenderCoordinatesFromWindow( mRenderer, mouseCoordinates.x, mouseCoordinates.y, &renderCoordinates.x, &renderCoordinates.y );
	
	return renderCoordinates;
}
