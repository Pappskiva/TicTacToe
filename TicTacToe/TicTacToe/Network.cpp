#include "Network.h"
#include <iostream>
#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 24
Network* Network::m_instance;
Network::Network()
{
	m_hostIsInitialized = false;
	m_clientIsInitialized = false;
	m_tableLayout[9] = { };
	for (unsigned int i = 0; i < 9; i++)
	{
		m_tableLayout[i].value = 0;
	}
	m_tableLayout[0].xPos = 0;
	m_tableLayout[0].yPos = 0;
	m_tableLayout[1].xPos = 1;
	m_tableLayout[1].yPos = 0;
	m_tableLayout[2].xPos = 2;
	m_tableLayout[2].yPos = 0;
	m_tableLayout[3].xPos = 0;
	m_tableLayout[3].yPos = 1;
	m_tableLayout[4].xPos = 1;
	m_tableLayout[4].yPos = 1;
	m_tableLayout[5].xPos = 2;
	m_tableLayout[5].yPos = 1;
	m_tableLayout[6].xPos = 0;
	m_tableLayout[6].yPos = 2;
	m_tableLayout[7].xPos = 1;
	m_tableLayout[7].yPos = 2;
	m_tableLayout[8].xPos = 2;
	m_tableLayout[8].yPos = 2;
}
Network::~Network()
{
}
Network* Network::GetInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new Network();
	}
	return m_instance;
}
void Network::InitializeHost()
{
	int iResult;
	printf("This is the host\n");

	if (m_clientIsInitialized)//If changed from client to host
	{
		Shutdown();
	}
	m_hostIsInitialized = true;
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return;
	}
	else
	{
		printf("WSAStartup Success Host \n");
	}


	struct addrinfo *result = NULL, *ptr = NULL, hints;

	ZeroMemory(&hints, sizeof (hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return;
	}
	else
	{
		printf("Host initialized address and port \n");
	}


	SOCKET ListenSocket = INVALID_SOCKET;

	// Create a SOCKET for the server to listen for client connections

	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (ListenSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return;
	}
	else
	{
		printf("Host initialized socket\n");
	}

	// Setup the TCP listening socket
		iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}
	else
	{
		printf("Host initialized TCP\n");
	}

	freeaddrinfo(result);


	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
		printf("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}

	ClientSocket = INVALID_SOCKET;

	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return;
	}
	else
	{
		printf("client has connected\n");
	}

}
void Network::InitializeClient()
{
	int iResult;
	printf("This is the client\n");
	if (m_hostIsInitialized)//If changed from host to client
	{
		Shutdown();
	}

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return;
	}
	else
	{
		printf("WSAStartup Success CLient\n");
	}

	m_clientIsInitialized = true;

	struct addrinfo *result = NULL, *ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	char* ipAdress = "127.0.0.1";
	iResult = getaddrinfo(ipAdress, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return;
	}

	ConnectSocket = INVALID_SOCKET;

	// Attempt to connect to the first address returned by
	// the call to getaddrinfo
	ptr = result;

	// Create a SOCKET for connecting to server
	ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return;
	}
	// Connect to server.
	iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
	}

	// Should really try the next address returned by getaddrinfo
	// if the connect call failed
	// But for this simple example we just free the resources
	// returned by getaddrinfo and print an error message

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return;
	}
	else
	{
		printf("Connected to server\n");
	}
}
void Network::Update()
{
	//if (m_hostIsInitialized)
	//{
	//	// Echo the buffer back to the sender
	//	int iSendResult = send(ClientSocket, p_text, (int)strlen(p_text), 0);
	//	if (iSendResult == SOCKET_ERROR)
	//	{
	//		printf("send failed: %d\n", WSAGetLastError());
	//		closesocket(ClientSocket);
	//		WSACleanup();
	//		return;
	//	}
	//}
	//else if (m_clientIsInitialized)
	//{
	//	int iResult = send(ConnectSocket, p_text, (int)strlen(p_text), 0);
	//	if (iResult == SOCKET_ERROR)
	//	{
	//		printf("send failed: %d\n", WSAGetLastError());
	//		closesocket(ConnectSocket);
	//		WSACleanup();
	//		return;
	//	}
	//}
	int recvbuflen = DEFAULT_BUFLEN;
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	if (m_hostIsInitialized)
	{
		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) 
		{
			printf(" %s\n", recvbuf);
		}
		else if (iResult == 0)
		{
			printf("No message\n");
		}
		else
		{
			printf("recv failed: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return;
		}
		char* text = "tyty for da message";
		int iSendResult = send(ClientSocket, text, (int)strlen(text), 0);
		if (iSendResult == SOCKET_ERROR)
		{
			printf("send failed: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return;
		}
	}
	else if (m_clientIsInitialized)
	{
		char* text = "Here is a message";
		iResult = send(ConnectSocket, text, (int)strlen(text), 0);
		if (iResult == SOCKET_ERROR)
		{
			printf("send failed: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
			return;
		}

		//Taking care of the message
		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
		{
			printf(" %s\n", recvbuf);
		}
		else if (iResult == 0)
		{
			printf("No message\n");
		}
		else
		{
			printf("recv failed: %d\n", WSAGetLastError());
		}
	}
}
void Network::Shutdown()
{
	//if (m_hostIsInitialized)
	//{
	//}
	//if (m_clientIsInitialized)
	//{
	//}
	m_instance->Disconnect();
}
void Network::Disconnect()
{
	if (m_hostIsInitialized)
	{
		// shutdown the send half of the connection since no more data will be sent
		int iResult = shutdown(ClientSocket, SD_SEND);
		if (iResult == SOCKET_ERROR) {
			printf("shutdown failed: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return;
		}
		// cleanup
		closesocket(ClientSocket);
		WSACleanup();
		m_hostIsInitialized = false;
	}
	else if (m_clientIsInitialized)
	{
		// shutdown the send half of the connection since no more data will be sent
		int iResult = shutdown(ConnectSocket, SD_SEND);
		if (iResult == SOCKET_ERROR) {
			printf("shutdown failed: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
			return;
		}
		// cleanup
		closesocket(ConnectSocket);
		WSACleanup();
		m_clientIsInitialized = false;
	}
}
void Network::SendTable()
{
	if (m_hostIsInitialized)
	{

	}
	else if (m_clientIsInitialized)
	{

	}
}
int Network::GetState()
{
	if (m_hostIsInitialized && !m_clientIsInitialized)
	{
		return 1;
	}
	if (m_clientIsInitialized && !m_hostIsInitialized)
	{
		return 2;
	}
	return 0;
}
void Network::SendText(char* p_text)
{
	if (m_hostIsInitialized)
	{
		// Echo the buffer back to the sender
		int iSendResult = send(ClientSocket, p_text, (int)strlen(p_text), 0);
		if (iSendResult == SOCKET_ERROR) 
		{
			printf("send failed: %d\n", WSAGetLastError());
			closesocket(ClientSocket);
			WSACleanup();
			return;
		}
	}
	else if (m_clientIsInitialized)
	{
		int iResult = send(ConnectSocket, p_text, (int)strlen(p_text), 0);
		if (iResult == SOCKET_ERROR) 
		{
			printf("send failed: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
			return;
		}
	}
}
void Network::CheckForVictory()
{
	int cross = 0;
	int circle = 0;
	for (unsigned int i = 0; i < 9; i++)
	{
		if (m_tableLayout[i].value == 0)
		{
			circle++;
		}
		if (m_tableLayout[i].value == 1)
		{
			cross++;
		}
	}
	if (cross == 3)
	{
		tile tile1;
		tile tile2;
		tile tile3;
		tile1.value = 2;
		tile2.value = 2;
		tile3.value = 2;
		for (unsigned int i = 0; i < 9; i++)
		{
			if (m_tableLayout[i].value == 1)
			{
				if (tile1.value == 2)
				{
					tile1 = m_tableLayout[i];
				}
				else if (tile2.value == 2)
				{
					tile2 = m_tableLayout[i];
				}
				else if (tile3.value == 2)
				{
					tile3 = m_tableLayout[i];
				}
			}
		}

		if (tile1.xPos == tile2.xPos && tile1.xPos == tile3.xPos)
		{
			//Win
		}
		else if (tile1.yPos == tile2.yPos && tile1.yPos == tile3.yPos)
		{
			//Win
		}
		else if (tile2.xPos == 1 && tile2.yPos == 1 && tile1.yPos == 0 && tile3.yPos == 2)
		{
			if (tile1.xPos == 0)
			{
				if (tile3.xPos == 2)
				{
					//Win
				}
			}
			if (tile1.xPos == 2)
			{
				if (tile3.xPos == 0)
				{
					//Win
				}
			}
		}
	}
	if (circle == 3)
	{
		tile tile1;
		tile tile2;
		tile tile3;
		tile1.value = 2;
		tile2.value = 2;
		tile3.value = 2;
		for (unsigned int i = 0; i < 9; i++)
		{
			if (m_tableLayout[i].value == 0)
			{
				if (tile1.value == 2)
				{
					tile1 = m_tableLayout[i];
				}
				else if (tile2.value == 2)
				{
					tile2 = m_tableLayout[i];
				}
				else if (tile3.value == 2)
				{
					tile3 = m_tableLayout[i];
				}
			}
		}

		if (tile1.xPos == tile2.xPos && tile1.xPos == tile3.xPos)
		{
			//Win
		}
		else if (tile1.yPos == tile2.yPos && tile1.yPos == tile3.yPos)
		{
			//Win
		}
		else if (tile2.xPos == 1 && tile2.yPos == 1 && tile1.yPos == 0 && tile3.yPos == 2)
		{
			if (tile1.xPos == 0)
			{
				if (tile3.xPos == 2)
				{
					//Win
				}
			}
			if (tile1.xPos == 2)
			{
				if (tile3.xPos == 0)
				{
					//Win
				}
			}
		}
	}
}
void Network::SetTile(int p_index, int p_value)
{
	m_tableLayout[p_index - 1].value = p_value;
}