#include "TheGame.h"
#include "GameBoard.h"
#include "Network.h"
#include "Button.h"

TheGame::TheGame(){}
TheGame::~TheGame(){}
void TheGame::Initialize()
{
	m_screen = NULL;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return;
	}
	m_window = SDL_CreateWindow("TicTacToe", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 415, 415, SDL_SWSURFACE);
	if (m_window == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
	}
	else
	{
		//Get window surface
		m_screen = SDL_GetWindowSurface(m_window);
	}
	m_backGround = NULL;
	m_backGround = SDL_LoadBMP("Background.bmp");
	m_circleVictorySign = NULL;
	m_circleVictorySign = SDL_LoadBMP("CircleVictorySign.bmp");
	m_crossVictorySign = NULL;
	m_crossVictorySign = SDL_LoadBMP("CrossVictorySign.bmp");
	m_exit = false;
	m_VictorySignRect.x = 0;
	m_VictorySignRect.y = 315;

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

	//m_textRenderer = new TextRenderer();
	m_textRenderer.Initialize();
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
	if (Network::GetInstance()->VictoryState() == 2)
	{
		SDL_BlitSurface(m_backGround, NULL, m_screen, NULL);
	}
	if (Network::GetInstance()->GetState() != 0 && Network::GetInstance()->VictoryState() == 2)
	{
		m_GameBoard->Update(m_screen, &e);
		if (Network::GetInstance()->StartDisconnect())
		{
			///BILD?
			Network::GetInstance()->Shutdown();
		}
	}

	if (m_ExitButton->IsClicked(m_screen, &e))
	{
		m_exit = true;
	}
	//char* ip = "192.168.0.101";//Laptop
	char* ip = "169.254.80.80";//Desktop
	if (Network::GetInstance()->GetState() == 0)
	{
		if (m_HostButton->IsClicked(m_screen, &e))
		{
			Network::GetInstance()->InitializeHost("27015", ip);
		}
		else if (m_ConnectButton->IsClicked(m_screen, &e))
		{
			Network::GetInstance()->InitializeClient("27015", ip);
		}
	}
	else
	{
		if (m_DisconnectButton->IsClicked(m_screen, &e))
		{
			Network::GetInstance()->Shutdown();
		}
	}


	if (Network::GetInstance()->VictoryState() == 0)
		SDL_BlitSurface(m_circleVictorySign, NULL, m_screen, &m_VictorySignRect);
	else if (Network::GetInstance()->VictoryState() == 1)
		SDL_BlitSurface(m_crossVictorySign, NULL, m_screen, &m_VictorySignRect);

	SDL_UpdateWindowSurface(m_window);
	if (m_exit)
	{
		Network::GetInstance()->Shutdown();
	}
	return m_exit;
}
void TheGame::Shutdown()
{
	SDL_FreeSurface(m_screen);
	SDL_FreeSurface(m_backGround);
	SDL_FreeSurface(m_circleVictorySign);
	SDL_FreeSurface(m_crossVictorySign);
	m_GameBoard->Shutdown();
	delete m_GameBoard;

	Network::GetInstance()->Shutdown();

	m_ConnectButton->Shutdown();
	m_DisconnectButton->Shutdown();
	m_ExitButton->Shutdown();
	m_HostButton->Shutdown();
	m_textRenderer.Shutdown();
	delete m_ConnectButton;
	delete m_DisconnectButton;
	delete m_ExitButton;
	delete m_HostButton;
	//delete m_textRenderer;
	SDL_DestroyWindow(m_window);
}