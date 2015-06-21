#pragma once
#include "SDL.h"
class Tile
{
public:
	Tile();
	~Tile();

	enum TileValue
	{
		TV_O,
		TV_X,
		TV_NONE
	};

	void Initialize(int p_PosX, int p_PosY, int p_index);
	bool Update(SDL_Surface* p_screen, SDL_Event* p_e);
	void Shutdown();
	int GetTileValue();
	void SetTileValue(TileValue p_tileValue);
	int GetTileIndex();
	void SetIfSelected(bool p_isSelected);
	
private:

	SDL_Rect m_rectangle;
	SDL_Surface* m_Cross;
	SDL_Surface* m_Circle;
	SDL_Surface* m_SelectedCross;
	SDL_Surface* m_SelectedCircle;
	SDL_Surface* m_Empty;
	SDL_Surface* m_RenderedSprite;
	TileValue m_TileValue;

	bool m_prevMouseButtonPressedState;
	bool m_currMouseButtonPressedState;

	bool m_selected;

	int m_xMin, m_xMax;
	int m_yMin, m_yMax;
	int m_tileIndex;
};

