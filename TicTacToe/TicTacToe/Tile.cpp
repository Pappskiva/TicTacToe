#include "Tile.h"
#include <stdio.h>
#include "Network.h"

Tile::Tile(){}
Tile::~Tile(){}
void Tile::Initialize(int p_PosX, int p_PosY, int p_index)
{
	m_Cross = NULL;
	m_Cross = SDL_LoadBMP("Cross.bmp");
	m_Circle = NULL;
	m_Circle = SDL_LoadBMP("Circle.bmp");
	m_SelectedCross = NULL;
	m_SelectedCross = SDL_LoadBMP("SelectedCross.bmp");
	m_SelectedCircle = NULL;
	m_SelectedCircle = SDL_LoadBMP("SelectedCircle.bmp");
	m_Empty = NULL;
	m_Empty = SDL_LoadBMP("Empty.bmp");
	m_RenderedSprite = NULL;
	m_rectangle.x = p_PosX;
	m_rectangle.y = p_PosY;
	m_TileValue = TV_NONE;
	m_xMin = p_PosX;
	m_xMax = p_PosX + 100;
	m_yMin = p_PosY;
	m_yMax = p_PosY + 100;
	m_tileIndex = p_index;
	m_selected = false;
}
bool Tile::Update(SDL_Surface* p_screen, SDL_Event* p_e)
{
	bool isClicked = false;
	if (Network::GetInstance()->MyTurn())//If clients turn
	{
		int x, y;
		bool mouseOverTile = false;
		SDL_GetMouseState(&x, &y);
		if (x > m_xMin && x < m_xMax && y > m_yMin && y < m_yMax)
		{
			mouseOverTile = true;
		}
		if (mouseOverTile)
		{
			if (p_e->type == SDL_MOUSEBUTTONUP)
			{
				/*if (m_TileValue == TV_NONE)
				{
					m_TileValue = TV_O;
				}
				else if (m_TileValue == TV_O)
				{
					m_TileValue = TV_X;
				}
				else if (m_TileValue == TV_X)
				{
					m_TileValue = TV_NONE;
				}*/

				isClicked = true;
			}
		}
	}

	if (m_TileValue == TV_NONE)
		m_RenderedSprite = m_Empty;
	else if (m_TileValue == TV_O && m_selected)
		m_RenderedSprite = m_SelectedCircle;
	else if (m_TileValue == TV_X && m_selected)
		m_RenderedSprite = m_SelectedCross;
	else if (m_TileValue == TV_O)
		m_RenderedSprite = m_Circle;
	else if (m_TileValue == TV_X)
		m_RenderedSprite = m_Cross;
	SDL_BlitSurface(m_RenderedSprite, NULL, p_screen, &m_rectangle);
	m_TileValue = (TileValue)Network::GetInstance()->GetTileValue(m_tileIndex);
	return isClicked;
}
void Tile::Shutdown()
{
	SDL_FreeSurface(m_SelectedCross);
	SDL_FreeSurface(m_SelectedCircle);
	SDL_FreeSurface(m_Cross);
	SDL_FreeSurface(m_Circle);
	SDL_FreeSurface(m_Empty);
}
int Tile::GetTileValue()
{
	return m_TileValue;
}
void Tile::SetTileValue(TileValue p_tileValue)
{
	m_TileValue = p_tileValue;
}
int Tile::GetTileIndex()
{
	return m_tileIndex;
}
void Tile::SetIfSelected(bool p_isSelected)
{
	m_selected = p_isSelected;
}