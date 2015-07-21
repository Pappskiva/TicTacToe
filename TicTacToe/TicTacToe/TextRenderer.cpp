#include "TextRenderer.h"

#include <stdio.h>
#include <cmath>

TextRenderer::TextRenderer(){}
TextRenderer::~TextRenderer(){}
void TextRenderer::Initialize()
{
	if (TTF_Init() == -1)
	{
		printf("Failed to init TTF");
	}
	m_font = TTF_OpenFont("lazy.ttf", 28);
	if (m_font == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
	}
}
std::string TextRenderer::GetIpFromPlayer(SDL_Window* p_window, SDL_Surface* p_screen, bool p_isIp)
{
	SDL_StartTextInput();
	std::string returnString = "";
	std::string preText;
	bool textFinished = false;
	int yPos;
	if (p_isIp)
	{
		 preText = "Ip: ";
		 yPos = 0;
	}
	else
	{
		preText = "Port: ";
		yPos = 50;
	}

	while (!textFinished)
	{

	}

	PrintText(yPos, p_window, (preText + returnString), p_screen);

	return returnString;
}
void TextRenderer::PrintIp(SDL_Window* p_window, std::string p_ip, std::string p_port, SDL_Surface* p_screen)
{
	//////Render text surface
	std::string printiPText = "Ip: " + p_ip;
	std::string printPortText = "Port: " + p_port;
	PrintText(0, p_window, printiPText, p_screen);
	PrintText(50, p_window, printPortText, p_screen);
}
void TextRenderer::PrintText(int p_yPos, SDL_Window* p_window, std::string p_text, SDL_Surface* p_screen)
{
	SDL_Rect rect;
	rect.x = 0;
	rect.y = p_yPos;
	SDL_Surface *m_texture = TTF_RenderText_Solid(m_font, p_text.c_str(), { 0, 250, 0 });
	if (m_texture == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		SDL_BlitSurface(m_texture, NULL, p_screen, &rect);
		SDL_UpdateWindowSurface(p_window);
	}
	SDL_FreeSurface(m_texture);
}
void TextRenderer::Shutdown()
{
}