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
	m_backGround = NULL;
	m_backGround = SDL_LoadBMP("Background.bmp");
	m_exit = false;

	m_GameBoard = new GameBoard();
	m_GameBoard->Initialize();

	m_ConnectButton = new Button();
	m_DisconnectButton = new Button();
	m_ExitButton = new Button();
	m_HostButton = new Button();
	m_ConnectButton->Initialize("Button_Connect.bmp", 315, 0);
	m_HostButton->Initialize("Button_Host.bmp", 315, 105);
	m_ExitButton->Initialize("Button_Exit.bmp", 315, 210);
	m_DisconnectButton->Initialize("Button_Disconnect.bmp", 315, 0, true);
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

	SDL_BlitSurface(m_backGround, NULL, m_screen, NULL);
	Network::GetInstance()->Update();
	if (Network::GetInstance()->GetState() != 0)
	{
		m_GameBoard->Update(m_screen, &e);
	}

	if (m_ExitButton->IsClicked(m_screen, &e))
	{
		m_exit = true;
	}

	if (Network::GetInstance()->GetState() == 0)
	{
		if (m_HostButton->IsClicked(m_screen, &e))
		{
			Network::GetInstance()->InitializeHost();
		}
		else if (m_ConnectButton->IsClicked(m_screen, &e))
		{
			Network::GetInstance()->InitializeClient();
		}
		
	}
	else
	{
		if (m_DisconnectButton->IsClicked(m_screen, &e))
		{
			Network::GetInstance()->Shutdown();
		}
	}

	SDL_Flip(m_screen);
	return m_exit;
}
void TheGame::Shutdown()
{
	SDL_FreeSurface(m_screen);
	SDL_FreeSurface(m_backGround);
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