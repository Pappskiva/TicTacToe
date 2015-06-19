#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#pragma comment(lib, "Ws2_32.lib")
class Network
{
public:
	static Network* GetInstance();
	void InitializeHost();
	void InitializeClient();
	void Update();
	void Shutdown();
	void Disconnect();
	int GetState();
	void SendText(char* p_text);
	void SetTile(int p_index, int p_value);
	int GetTileValue(int p_index);
	void SendTable();

private:
	Network();
	~Network();
	void CheckForVictory();
	void HandleServerMessage(char p_message[]);

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


};

