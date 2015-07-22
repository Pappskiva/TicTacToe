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
	SDL_Event e;
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

	PrintText(yPos, p_window, preText, p_screen);
	while (!textFinished)
	{
		bool renderText = false;
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				textFinished = true;
			}
			else if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_KP_ENTER)
				{
					textFinished = true;
					renderText = true;
				}
				if (e.key.keysym.sym == SDLK_BACKSPACE && returnString.length() > 0)
				{
					returnString.pop_back();
					renderText = true;
				}
			}
			//Special text input event
			else if (e.type == SDL_TEXTINPUT)
			{
				//Not copy or pasting
				if (!((e.text.text[0] == 'c' || e.text.text[0] == 'C') && (e.text.text[0] == 'v' || e.text.text[0] == 'V') && SDL_GetModState() & KMOD_CTRL))
				{
					//Append character
					returnString += e.text.text;
					renderText = true;
				}
			}
			if (renderText)
			{
				PrintText(yPos, p_window, (preText + returnString), p_screen);
			}
		}
	}

	SDL_StopTextInput();
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