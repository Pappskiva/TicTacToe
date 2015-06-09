#include "Tile.h"

Tile::Tile(){}
Tile::~Tile(){}
void Tile::Initialize(int p_PosX, int p_PosY)
{
	m_Cross = NULL;
	m_Cross = SDL_LoadBMP("Cross.bmp");
	m_Circle = NULL;
	m_Circle = SDL_LoadBMP("Circle.bmp");
	m_Empty = NULL;
	m_Empty = SDL_LoadBMP("Empty.bmp");
	m_RenderedSprite = NULL;
	m_rectangle.x = p_PosX;
	m_rectangle.y = p_PosY;
	m_TileValue = TV_NONE;
}
void Tile::Update(SDL_Surface* p_screen)
{
	if (m_TileValue == TV_NONE)
		m_RenderedSprite = m_Empty;
	if (m_TileValue == TV_O)
		m_RenderedSprite = m_Circle;
	if (m_TileValue == TV_X)
		m_RenderedSprite = m_Cross;


	SDL_BlitSurface(m_RenderedSprite, NULL, p_screen, &m_rectangle);
}
void Tile::Shutdown()
{
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