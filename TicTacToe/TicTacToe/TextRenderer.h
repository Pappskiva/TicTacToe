#pragma once

#include "SDL.h"
#include <string>
#include "SDL_ttf.h"
class TextRenderer
{
public:
	TextRenderer();
	~TextRenderer();

	void Initialize();
	void Update(bool p_writable, std::string p_text, SDL_Surface* p_screen);
	void PrintIp(SDL_Window* p_window, std::string p_text, std::string p_port, SDL_Surface* p_screen);
	void Shutdown();


private:

	TTF_Font* m_font;
	SDL_Surface* m_texture;
	SDL_Rect m_rect;
	std::string m_ipString;

};

