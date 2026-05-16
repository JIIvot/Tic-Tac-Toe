#pragma once

#include "Entity.h"
#include "Types.h"
#include "Cell.h"

#include <vector>
#include <SDL3/SDL_rect.h>

class CGame;
class CInput;

class CField : public CEntity
{
public:
	CField( SDL_Point size, int32 winSequenceLength );
	
	void Spawn() override;
	void Update( float deltaTime ) override;
	void Render( SDL_Renderer* renderer, SDL_Texture* atlasTexture ) override;
	
private:
	void Reset();
	
	void UpdateHoveredCell( SDL_FPoint mouseCoordinates );
	void UpdateGameState();
	
	void ProcessPlayerClick();
	
	[[nodiscard]]
	std::vector<CCell*> GetSequenceInDirection( SDL_Point startCoordinates, SDL_Point direction, CCell::ECellType type );
	
	[[nodiscard]]
	bool TryProcessSequenceInDirection( SDL_Point coordinates, SDL_Point direction );
	
	[[nodiscard]]
	bool TryProcessWin( SDL_Point coordinates );
	
	[[nodiscard]]
	bool TryProcessDraw();
	
	[[nodiscard]]
	bool IsCoordinatesValid( SDL_Point coordinates ) const
	{
		return coordinates.x >= 0 && coordinates.x < mSize.x && coordinates.y >= 0 && coordinates.y < mSize.y;
	}
	
private:
	CGame*                          mGameInstance   = nullptr;
	const CInput*                   mInput          = nullptr;
	
	SDL_Point                       mSize;
	SDL_FPoint                      mCellRegionSize;
	int32                           mWinSequenceLength;

	std::vector<std::vector<CCell>> mCells;
	CCell*                          mHoveredCell    = nullptr;
	SDL_Point                       mHoveredCoordinates;
	
	SDL_Point                       mLastTurnCoordinates;
	
	bool                            mIsGameFinished = false;
	bool                            mIsCrossTurn    = true;
	int32                           mTurnsNum       = 0;
};
