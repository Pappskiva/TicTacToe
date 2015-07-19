#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#pragma comment(lib, "Ws2_32.lib")

#include <stdio.h>
#pragma comment(lib, "iphlpapi.lib")
class Network
{
public:
	static Network* GetInstance();
	void InitializeHost(char* p_port, char* p_ip);
	void InitializeClient(char* p_port, char* p_ip);
	void Update();
	void Shutdown();
	void Disconnect();
	int GetState();
	void SendText(char* p_text);
	void SetTile(int p_index);
	int GetTileValue(int p_index);
	void SendTable();
	int GetNumberOfPlacedTiles();
	void MoveTile(int p_selectedTileIndex, int p_destinationTileIndex);
	bool MyTurn();
	bool StartDisconnect();
	int VictoryState();

private:
	Network();
	~Network();
	void CheckForVictory();
	void HandleServerMessage(char p_message[]);

	void print_adapter(PIP_ADAPTER_ADDRESSES aa);
	void print_addr(PIP_ADAPTER_UNICAST_ADDRESS ua);

	static Network* m_instance;
	bool m_hostIsInitialized;
	bool m_clientIsInitialized;
	WSAData wsaData;
	SOCKET ConnectSocket;
	SOCKET ClientSocket;

	struct tile
	{
		int xPos;
		int yPos;
		int value;
		//0 = cirkel
		//1 = kryss
		//2 = tom
	};
	tile m_tableLayout[9];

	int m_whooseTurn;

	int test;
	bool m_disconnect;
	int m_victory;
	char m_ip[BUFSIZ];
	bool m_savedIp;
};

