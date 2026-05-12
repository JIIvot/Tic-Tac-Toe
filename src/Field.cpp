#include "Field.h"
#include "Cell.h"
#include "Game.h"
#include "Input.h"

#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>

CField::CField( SDL_Point size, int32 winSequenceLength )
	: mSize( size )
	, mCellRegionSize( { CGame::kGameWidth / static_cast<float>( size.x ), CGame::kGameHeight / static_cast<float>( size.y ) } )
	, mWinSequenceLength( winSequenceLength )
{
	mCells.resize( size.x );
	
	for ( int32 x = 0; x < size.x; ++x )
	{
		mCells[x].reserve( size.y );
		
		for ( int32 y = 0; y < size.y; ++y )
		{
			const SDL_FPoint point = {
				mCellRegionSize.x * ( static_cast<float>( x ) + 0.5f ),
				mCellRegionSize.y * ( static_cast<float>( y ) + 0.5f )
			};
			
			mCells[x].emplace_back( point );
		}
	}
}

void CField::Spawn()
{
	mGameInstance = CGame::GetInstance();
	mInput        = mGameInstance->GetInput();
}

void CField::Reset()
{
	mTurnsNum       = 0;
	mIsCrossTurn    = true;
	mIsGameFinished = false;
	
	for ( int32 x = 0; x < mSize.x; ++x )
	{
		for ( int32 y = 0; y < mSize.y; ++y )
		{
			mCells[x][y].mType = eCellType_None;
			mCells[x][y].SetNormalSize();
		}
	}
}

void CField::Update( float deltaTime )
{
	// Reset action (R)
	if ( mInput->IsKeyJustPressed( SDL_SCANCODE_R ) )
	{
		Reset();
	}
	
	for ( int32 x = 0; x < mSize.x; ++x )
	{
		for ( int32 y = 0; y < mSize.y; ++y )
		{
			mCells[x][y].Update( deltaTime );
		}
	}
	
	if ( mIsGameFinished )
	{
		return;
	}
	
	const SDL_FPoint mouseCoordinates = mGameInstance->GetRenderMouseCoordinates();
	const bool       isMouseValidX    = mouseCoordinates.x >= 0 && mouseCoordinates.x < CGame::kGameWidth;
	const bool       isMouseValidY    = mouseCoordinates.y >= 0 && mouseCoordinates.y < CGame::kGameHeight;
	
	if ( !isMouseValidX || !isMouseValidY )
	{
		return;
	}

	UpdateHoveredCell( mouseCoordinates );
	
	if ( mInput->IsButtonJustPressed( SDL_BUTTON_LEFT ) )
	{
		ProcessPlayerClick();
	}
}

void CField::UpdateHoveredCell( SDL_FPoint mouseCoordinates )
{
	SDL_Point newHoveredCoordinates;
	newHoveredCoordinates.x = static_cast<int32>( mouseCoordinates.x / mCellRegionSize.x );
	newHoveredCoordinates.y = static_cast<int32>( mouseCoordinates.y / mCellRegionSize.y );
	
	CCell* newHoveredCell = &mCells[newHoveredCoordinates.x][newHoveredCoordinates.y];
	if ( newHoveredCell == mHoveredCell )
	{
		return;
	}
	
	if ( mHoveredCell )
	{
		mHoveredCell->SetNormalSize();
	}
	
	if ( newHoveredCell->mType != eCellType_None )
	{
		mHoveredCell = nullptr;
		return;
	}
	
	mHoveredCell  = newHoveredCell;
	mHoveredCoordinates = newHoveredCoordinates;
	
	mHoveredCell->SetHoveredSize();
}

void CField::UpdateGameState()
{
	if ( TryProcessWin( mLastTurnCoordinates) || TryProcessDraw() )
	{
		mIsGameFinished = true;
	}
}

void CField::ProcessPlayerClick()
{
	if ( !mHoveredCell || mHoveredCell->mType != eCellType_None )
	{
		return;
	}
	
	mHoveredCell->mType = mIsCrossTurn ? eCellType_Cross : eCellType_Circle;
	mIsCrossTurn        = !mIsCrossTurn;
	
	++mTurnsNum;
	
	mLastTurnCoordinates = mHoveredCoordinates;
	
	mHoveredCell->SetNormalSize();
	mHoveredCell = nullptr;
	
	UpdateGameState();
}

std::vector<CCell*> CField::GetSequenceInDirection( SDL_Point startCoordinates, SDL_Point direction, ECellType type )
{
	std::vector<CCell*> sequence;
	
	SDL_Point coordinates;
	coordinates.x = startCoordinates.x + direction.x;
	coordinates.y = startCoordinates.y + direction.y;

	while ( IsCoordinatesValid( coordinates ) && mCells[coordinates.x][coordinates.y].mType == type )
	{
		sequence.push_back( &mCells[coordinates.x][coordinates.y] );
		
		coordinates.x += direction.x;
		coordinates.y += direction.y;
	}
	
	return sequence;
}

// Tries to find a win sequence in direction and process cells
// Returns true if the sequence was found and processed
[[nodiscard]]
bool CField::TryProcessSequenceInDirection( SDL_Point coordinates, SDL_Point direction )
{
	const ECellType type = mCells[coordinates.x][coordinates.y].mType;
	if ( type == eCellType_None )
	{
		return false;
	}

	const std::vector<CCell*> positiveDirectionCells
		= GetSequenceInDirection( coordinates, direction, type );
	
	const std::vector<CCell*> negativeDirectionCells
		= GetSequenceInDirection( coordinates, { -direction.x, -direction.y }, type );
	
	if ( positiveDirectionCells.size() + negativeDirectionCells.size() + 1 < mWinSequenceLength )
	{
		return false;
	}
	
	mCells[coordinates.x][coordinates.y].SetClosedSize();
	
	for ( CCell* cell : positiveDirectionCells )
	{
		cell->SetClosedSize();
	}
	
	for ( CCell* cell : negativeDirectionCells )
	{
		cell->SetClosedSize();
	}
	
	return true;
}

bool CField::TryProcessWin( SDL_Point coordinates )
{
	return
		TryProcessSequenceInDirection( coordinates, { 1,  0 } ) ||
		TryProcessSequenceInDirection( coordinates, { 0,  1 } ) ||
		TryProcessSequenceInDirection( coordinates, { 1,  1 } ) ||
		TryProcessSequenceInDirection( coordinates, { 1, -1 } );
}

bool CField::TryProcessDraw()
{
	if ( mTurnsNum != mSize.x * mSize.y )
	{
		return false;
	}
	
	for ( int32 x = 0; x < mSize.x; ++x )
	{
		for ( int32 y = 0; y < mSize.y; ++y )
		{
			mCells[x][y].SetClosedSize();
		}
	}
	
	return true;
}

void CField::Render( SDL_Renderer* renderer, SDL_Texture* atlasTexture )
{
	for ( int32 x = 0; x < mSize.x; ++x )
	{
		for ( int32 y = 0; y < mSize.y; ++y )
		{
			mCells[x][y].Render( renderer, atlasTexture );
		}
	}
}
