#pragma once

#include "SDL.h"
#include "TextRenderer.h"
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

	SDL_Window* m_window;
	SDL_Surface* m_screen;
	SDL_Surface* m_backGround;
	SDL_Surface* m_circleVictorySign;
	SDL_Surface* m_crossVictorySign;
	SDL_Rect m_VictorySignRect;
	
	GameBoard* m_GameBoard;
	Button* m_ConnectButton;
	Button* m_ExitButton;
	Button* m_HostButton;
	Button* m_DisconnectButton;
	TextRenderer m_textRenderer;
};

