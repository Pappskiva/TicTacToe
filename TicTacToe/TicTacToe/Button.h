#pragma once
#include "SDL.h"
class Button
{
public:
	Button();
	~Button();
	void Initialize(char* p_texture, int p_xPos, int p_yPos);
	bool IsClicked(SDL_Surface* p_screen, SDL_Event* p_e);
	void Shutdown();
private:
	SDL_Surface* m_button;
	SDL_Rect m_buttonRect;
	int m_xMin, m_xMax;
	int m_yMin, m_yMax;
};

