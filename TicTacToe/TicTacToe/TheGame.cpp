#include "TheGame.h"
#include "GameBoard.h"

TheGame::TheGame(){}
TheGame::~TheGame(){}
void TheGame::Initialize()
{
	m_screen = NULL;
	m_screen = SDL_SetVideoMode(310, 310, 32, SDL_SWSURFACE);
	//hello = NULL;
	//hello = SDL_LoadBMP(SPRITE_GRID);
	m_exit = false;

	m_GameBoard = new GameBoard();
	m_GameBoard->Initialize();
}
bool TheGame::Update()
{
	m_exit = false;
	//SDL_BlitSurface(hello, NULL, screen, NULL);
	SDL_Event e;
	//Handle events on queue
	while (SDL_PollEvent(&e) != 0)
	{
		//User requests quit
		if (e.type == SDL_QUIT)
		{
			m_exit = true;
		}
	}

	m_GameBoard->Update(m_screen, &e);
	SDL_Flip(m_screen);

	return m_exit;
}
void TheGame::Shutdown()
{
	//SDL_FreeSurface(hello);
	SDL_FreeSurface(m_screen);
	m_GameBoard->Shutdown();
	delete m_GameBoard;
}