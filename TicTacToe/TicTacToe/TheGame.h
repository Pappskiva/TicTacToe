#pragma once
#include "SDL.h"
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

	SDL_Surface* screen;


	SDL_Surface* hello;



};

