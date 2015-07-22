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
	std::string GetIpFromPlayer(SDL_Window* p_window, SDL_Surface* p_screen, SDL_Surface* p_background, bool p_isIp);
	void PrintIp(SDL_Window* p_window, std::string p_text, std::string p_port, SDL_Surface* p_screen);
	void Shutdown();


private:
	void PrintText(int p_yPos, SDL_Window* p_window, std::string p_text, SDL_Surface* p_screen);
	TTF_Font* m_font;

};

