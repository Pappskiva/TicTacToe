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
	//char *test;
	//test = "0123456789";

	//char test2[10];
	//test2[0] = 9;
	//test2[1] = 8;
	//test2[2] = 7;
	//test2[3] = 6;
	//test2[4] = 5;
	//test2[5] = 4;
	//test2[6] = 3;
	//test2[7] = 2;
	//test2[8] = 1;
	//test2[9] = 0;
	//int a = 0;
	//std::strcpy(test2, test);
	//for (unsigned int i = 0; i < 10; i++)
	//{
	//	printf("&d\n", test2[i]);
	//}


	Network::GetInstance()->Update();
	if (Network::GetInstance()->GetState() != 0)
	{
		m_GameBoard->Update(m_screen, &e);
	}

	if (m_ExitButton->IsClicked(m_screen, &e))
	{
		m_exit = true;
	}

	if (m_HostButton->IsClicked(m_screen, &e))
	{
		Network::GetInstance()->InitializeHost();
	}
	else if (m_ConnectButton->IsClicked(m_screen, &e))
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