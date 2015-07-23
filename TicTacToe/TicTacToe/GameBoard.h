#pragma once
#include <vector>
#include "SDL.h"
class Tile;

class GameBoard
{
public:
	GameBoard();
	~GameBoard();
	void Initialize();
	bool Update(SDL_Surface* p_screen, SDL_Event* p_e);
	void Shutdown();
	void SetAllToDefault();

private:
	//std::vector<Tile> m_Tiles;
	void TileClickCheck(Tile *p_tile, SDL_Surface* p_screen, SDL_Event* p_e);
	SDL_Surface* m_Background;
	Tile* m_Tile1;
	Tile* m_Tile2;
	Tile* m_Tile3;
	Tile* m_Tile4;
	Tile* m_Tile5;
	Tile* m_Tile6;
	Tile* m_Tile7;
	Tile* m_Tile8;
	Tile* m_Tile9;
	Tile* m_selectedTile;

};

