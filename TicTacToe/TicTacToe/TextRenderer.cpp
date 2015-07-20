#include "TextRenderer.h"

#include <stdio.h>
#include <cmath>

TextRenderer::TextRenderer(){}
TextRenderer::~TextRenderer(){}
void TextRenderer::Initialize()
{
	//////Open the font

	//int h = 9;
	if (TTF_Init() == -1)
	{
		printf("Failed to init TTF");
	}
	m_font = TTF_OpenFont("lazy.ttf", 28);
	if (m_font == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
	}
	//else
	//{
	//	//Render text
	//	SDL_Color textColor = { 0, 0, 0 };
	//	if (!gTextTexture.loadFromRenderedText("The quick brown fox jumps over the lazy dog", textColor))
	//	{
	//		printf("Failed to render text texture!\n");
	//	}
	//}
	m_rect.x = 0;
	m_rect.y = 0;
	m_ipString = "No Ip lawed";
}
void TextRenderer::Update(bool p_writable, std::string p_text, SDL_Surface* p_screen)
{
	SDL_Color textColor = { 0, 0, 0 };
	//////Render text surface
	m_texture = TTF_RenderText_Solid(m_font, m_ipString.c_str(), textColor);
	if (m_texture == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		////Create texture from surface pixels
		//m_texture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		//if (mTexture == NULL)
		//{
		//	printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		//}
		//else
		//{
		//	//Get image dimensions
		//	mWidth = textSurface->w;
		//	mHeight = textSurface->h;
		//}
		SDL_BlitSurface(m_texture, NULL, p_screen, &m_rect);
	}

}
void TextRenderer::PrintIp(SDL_Window* p_window, std::string p_ip, std::string p_port, SDL_Surface* p_screen)
{
	SDL_Rect ipRect, portRect;

	ipRect.x = 0;
	ipRect.y = 0;

	portRect.x = 0;
	portRect.y = 50;

	SDL_Color textColor = { 0, 250, 0 };
	//////Render text surface
	std::string printiPText = "Ip: " + p_ip;
	std::string printPortText = "Port: " + p_port;
	SDL_Surface* ipTexture = TTF_RenderText_Solid(m_font, printiPText.c_str(), textColor);
	SDL_Surface* portTexture = TTF_RenderText_Solid(m_font, printPortText.c_str(), textColor);
	if (ipTexture == NULL || portTexture == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		SDL_BlitSurface(ipTexture, NULL, p_screen, &ipRect);
		SDL_BlitSurface(portTexture, NULL, p_screen, &portRect);
		SDL_UpdateWindowSurface(p_window);
	}
	SDL_FreeSurface(ipTexture);
	SDL_FreeSurface(portTexture);
}
void TextRenderer::Shutdown()
{
	SDL_FreeSurface(m_texture);
}