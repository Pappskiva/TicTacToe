#include "TheGame.h"
#include "GameBoard.h"
#include "Network.h"
#include "Button.h"

TheGame::TheGame(){}
TheGame::~TheGame(){}
void TheGame::Initialize()
{
	m_screen = NULL;
	m_screen = SDL_SetVideoMode(415, 310, 32, SDL_SWSURFACE);
	m_exit = false;

	m_GameBoard = new GameBoard();
	m_GameBoard->Initialize();

	m_ConnectButton = new Button();
	m_DisconnectButton = new Button();
	m_ExitButton = new Button();
	m_HostButton = new Button();
	m_ConnectButton->Initialize("Button_Connect.bmp",315,0);
	m_HostButton->Initialize("Button_Host.bmp", 315, 105);
	m_ExitButton->Initialize("Button_Exit.bmp", 315, 210);
	m_DisconnectButton->Initialize("Button_Disconnect.bmp", 315, 105);
}
bool TheGame::Update()
{
	m_exit = false;
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
	Network::GetInstance()->Update();
	m_GameBoard->Update(m_screen, &e);

	if (m_ExitButton->IsClicked(m_screen, &e))
	{
		m_exit = true;
	}
	if (m_HostButton->IsClicked(m_screen, &e))
	{
		Network::GetInstance()->InitializeHost();
	}
	if (m_ConnectButton->IsClicked(m_screen, &e))
	{
		Network::GetInstance()->InitializeClient();
	}

	SDL_Flip(m_screen);
	return m_exit;
}
void TheGame::Shutdown()
{
	SDL_FreeSurface(m_screen);
	m_GameBoard->Shutdown();
	delete m_GameBoard;

	Network::GetInstance()->Shutdown();

	m_ConnectButton->Shutdown();
	m_DisconnectButton->Shutdown();
	m_ExitButton->Shutdown();
	m_HostButton->Shutdown();
	delete m_ConnectButton;
	delete m_DisconnectButton;
	delete m_ExitButton;
	delete m_HostButton;
}