#include "TheGame.h"

void Initialize();
void Shutdown();
TheGame m_game;

int main(int argc, char* args[])
{
	Initialize();

	while (!m_game.Update());

	Shutdown();

	return 0;
}
void Initialize()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	m_game.Initialize();
}
void Shutdown()
{
	m_game.Shutdown();
	SDL_Quit();
}