#include "TheGame.h"


TheGame::TheGame(){}
TheGame::~TheGame(){}
void TheGame::Initialize()
{
	screen = NULL;
	screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);
	hello = NULL;
	hello = SDL_LoadBMP("1.bmp");
	m_exit = false;
}
bool TheGame::Update()
{
	m_exit = true;
	SDL_BlitSurface(hello, NULL, screen, NULL);
	SDL_Delay(2000);

	SDL_Flip(screen);
	SDL_Delay(2000);
	return m_exit;
}
void TheGame::Shutdown()
{
	SDL_FreeSurface(hello);
	SDL_FreeSurface(screen);
}