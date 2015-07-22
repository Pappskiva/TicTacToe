#include "Button.h"


Button::Button(){}
Button::~Button(){}
void Button::Initialize(char* p_texture, int p_xPos, int p_yPos)
{
	m_button = NULL;
	m_button = SDL_LoadBMP(p_texture);
	m_buttonRect.x = p_xPos;
	m_buttonRect.y = p_yPos;
	m_xMin = p_xPos;
	m_xMax = p_xPos + 100;
	m_yMin = p_yPos;
	m_yMax = p_yPos + 100;
	
}
void Button::Initialize(char* p_texture, int p_xPos, int p_yPos, bool p_bigButton)
{
	m_button = NULL;
	m_button = SDL_LoadBMP(p_texture);
	m_buttonRect.x = p_xPos;
	m_buttonRect.y = p_yPos;
	m_xMin = p_xPos;
	m_xMax = p_xPos + 100;
	m_yMin = p_yPos;
	m_yMax = p_yPos + 200;
}
bool Button::IsClicked(SDL_Surface* p_screen, SDL_Event* p_e)
{
	int x, y;
	bool mouseOverTile = false;
	SDL_GetMouseState(&x, &y);
	if (x > m_xMin && x < m_xMax && y > m_yMin && y < m_yMax)
	{
		mouseOverTile = true;
	}

	SDL_BlitSurface(m_button, NULL, p_screen, &m_buttonRect);
	if (mouseOverTile)
	{
		if (p_e->type == SDL_MOUSEBUTTONUP)
		{
			return true;
		}
	}
	return false;
}
void Button::Shutdown()
{
	SDL_FreeSurface(m_button);
}