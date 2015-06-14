#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
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

private:
	Network();
	~Network();
	void SendTable();

	static Network* m_instance;
	bool m_hostIsInitialized;
	bool m_clientIsInitialized;
	WSAData wsaData;
	SOCKET ConnectSocket;
	SOCKET ClientSocket;
};

