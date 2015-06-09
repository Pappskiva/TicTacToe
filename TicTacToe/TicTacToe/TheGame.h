#pragma once

#include "SDL.h"
class GameBoard;
class TheGame
{
public:
	TheGame();
	~TheGame();
	void Initialize();
	bool Update();
	void Shutdown();

private:
	bool m_exit;

	SDL_Surface* m_screen;
	
	GameBoard* m_GameBoard;

	//SDL_Surface* hello;


};

