#pragma once

#include "SDL.h"
class GameBoard;
class Button;

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
	SDL_Surface* m_backGround;
	
	GameBoard* m_GameBoard;
	Button* m_ConnectButton;
	Button* m_ExitButton;
	Button* m_HostButton;
	Button* m_DisconnectButton;
};

