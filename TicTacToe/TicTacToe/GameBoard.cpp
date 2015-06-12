#include "GameBoard.h"
#include "Tile.h"

GameBoard::GameBoard(){}
GameBoard::~GameBoard(){}
void GameBoard::Initialize()
{
	//int yPos = 0;
	//int xPos = 0;
	//for (unsigned int i = 0; i < 9; i++)
	//{
	//	Tile tile;
	//	tile.Initialize(xPos, yPos);
	//	m_Tiles.push_back(tile);
	//}
	m_Tile1 = new Tile();
	m_Tile2 = new Tile();
	m_Tile3 = new Tile();
	m_Tile4 = new Tile();
	m_Tile5 = new Tile();
	m_Tile6 = new Tile();
	m_Tile7 = new Tile();
	m_Tile8 = new Tile();
	m_Tile9 = new Tile();
	m_Tile1->Initialize(0,0);
	m_Tile2->Initialize(105, 0);
	m_Tile3->Initialize(210, 0);
	m_Tile4->Initialize(0, 105);
	m_Tile5->Initialize(105, 105);
	m_Tile6->Initialize(210, 105);
	m_Tile7->Initialize(0, 210);
	m_Tile8->Initialize(105, 210);
	m_Tile9->Initialize(210, 210);
	m_Background = NULL;
	m_Background = SDL_LoadBMP("TicTactoeGrid.bmp");
}
bool GameBoard::Update(SDL_Surface* p_screen, SDL_Event* p_e)
{
	//for (unsigned int i = 0; i < 9; i++)
	//{
	//	m_Tiles[i].Update();
	//}
	SDL_BlitSurface(m_Background, NULL, p_screen, NULL);
	m_Tile1->Update(p_screen, p_e);
	m_Tile2->Update(p_screen, p_e);
	m_Tile3->Update(p_screen, p_e);
	m_Tile4->Update(p_screen, p_e);
	m_Tile5->Update(p_screen, p_e);
	m_Tile6->Update(p_screen, p_e);
	m_Tile7->Update(p_screen, p_e);
	m_Tile8->Update(p_screen, p_e);
	m_Tile9->Update(p_screen, p_e);
	return true;
}
void GameBoard::Shutdown()
{
	//for (unsigned int i = 0; i < 9; i++)
	//{
	//	//m_Tiles[i].Shutdown();
	//}
	m_Tile1->Shutdown();
	m_Tile2->Shutdown();
	m_Tile3->Shutdown();
	m_Tile4->Shutdown();
	m_Tile5->Shutdown();
	m_Tile6->Shutdown();
	m_Tile7->Shutdown();
	m_Tile8->Shutdown();
	m_Tile9->Shutdown();

	delete m_Tile1;
	delete m_Tile2;
	delete m_Tile3;
	delete m_Tile4;
	delete m_Tile5;
	delete m_Tile6;
	delete m_Tile7;
	delete m_Tile8;
	delete m_Tile9;
	SDL_FreeSurface(m_Background);
}