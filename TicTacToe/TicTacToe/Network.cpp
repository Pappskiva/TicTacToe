#define  _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Network.h"
#include <iostream>
#include "TextRenderer.h"

#define DEFAULT_BUFLEN 11
Network* Network::m_instance;
Network::Network()
{
	m_hostIsInitialized = false;
	m_clientIsInitialized = false;

	m_savedIp = false;
	m_disconnect = false;
	m_victory = 2;
	m_whooseTurn = 0;
}
Network::~Network(){}
Network* Network::GetInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new Network();
	}
	return m_instance;
}

void Network::print_adapter(PIP_ADAPTER_ADDRESSES aa)
{
	char buf[BUFSIZ];
	memset(buf, 0, BUFSIZ);
	WideCharToMultiByte(CP_ACP, 0, aa->FriendlyName, wcslen(aa->FriendlyName), buf, BUFSIZ, NULL, NULL);
	printf("adapter_name:%s\n", buf);
}

void Network::print_addr(PIP_ADAPTER_UNICAST_ADDRESS ua)
{
	char buf[BUFSIZ];

	int family = ua->Address.lpSockaddr->sa_family;
	if (family == AF_INET)
	{
		//printf("\t%s ", family == AF_INET ? "IPv4" : "IPv6");

		memset(buf, 0, BUFSIZ);
		getnameinfo(ua->Address.lpSockaddr, ua->Address.iSockaddrLength, buf, sizeof(buf), NULL, 0, NI_NUMERICHOST);
		printf("%s\n", buf);
		if (!m_savedIp)
		{
			if (buf[0] == '1'&&
				buf[1] == '9'&&
				buf[2] == '2'&&
				buf[3] == '.'/*&&
				buf[4] == '1'&&
				buf[5] == '6'&&
				buf[6] == '8'&&
				buf[7] == '.'&&
				buf[8] == '0'*/)
			{
				for (int i = 0; i < BUFSIZ; i++)
				{
					m_ip[i] = buf[i];
				}
				m_savedIp = true;
			}
		}
	}
}

void Network::InitializeHost(char* p_port, SDL_Window* p_window, SDL_Surface* p_screen, TextRenderer *p_textRenderer)
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
	iResult = getaddrinfo(NULL, p_port, &hints, &result);
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
	
	// Get host ip here
	DWORD rv, size;
	PIP_ADAPTER_ADDRESSES adapter_addresses, aa;
	PIP_ADAPTER_UNICAST_ADDRESS ua;

	rv = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, NULL, &size);
	if (rv != ERROR_BUFFER_OVERFLOW) {
		fprintf(stderr, "GetAdaptersAddresses() failed...");
		return;
	}
	adapter_addresses = (PIP_ADAPTER_ADDRESSES)malloc(size);

	rv = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, adapter_addresses, &size);
	if (rv != ERROR_SUCCESS) {
		fprintf(stderr, "GetAdaptersAddresses() failed...");
		free(adapter_addresses);
		return;
	}
	for (aa = adapter_addresses; aa != NULL; aa = aa->Next) {
		//print_adapter(aa);
		for (ua = aa->FirstUnicastAddress; ua != NULL; ua = ua->Next) {
			print_addr(ua);
		}
	}
	free(adapter_addresses);
	// Get host ip hereBB

	printf("%s\n", m_ip);
	

	p_textRenderer->PrintIp(p_window, m_ip, p_port,p_screen);
	///////////Sleep 2 second


	//USHORT port = 27015;
	//SOCKADDR_IN temp;
	//temp.sin_family = AF_INET;
	//temp.sin_port = htons(port);
	//temp.sin_addr.s_addr = inet_addr("127.0.0.1");
	//int len = sizeof(temp);
	//int tt = getsockname(ListenSocket, (SOCKADDR*)&temp, &len);
	//if ( tt == -1)
	//{
	//	int errorcode = WSAGetLastError();
	//	int i = 0;
	//}
	//else
	//{
	//	printf("Client: Receiver IP(s) used: %s\n", inet_ntoa(temp.sin_addr));
	//	printf("Client: Receiver port used: %d\n", htons(temp.sin_port));
	//}
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

	u_long iMode = 1;

	iResult = ioctlsocket(ClientSocket, FIONBIO, &iMode);
	if (iResult == SOCKET_ERROR)
	{
		printf("ioctsocket failed with error thingy");
	}

	for (unsigned int i = 0; i < 9; i++)
	{
		m_tableLayout[i].value = 2;
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
	printf("Initialized Table Layout\n");
}
void Network::InitializeClient(char* p_port, char* p_ip)
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
	iResult = getaddrinfo(p_ip, p_port, &hints, &result);
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

	u_long iMode = 1;

	iResult = ioctlsocket(ConnectSocket, FIONBIO, &iMode);
	if (iResult == SOCKET_ERROR)
	{
		printf("ioctsocket failed with error thingy");
	}


	for (unsigned int i = 0; i < 9; i++)
	{
		m_tableLayout[i].value = 2;
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
	printf("Initialized Table Layout\n");
}
void Network::Update()
{
	int recvbuflen = DEFAULT_BUFLEN;
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	if (m_hostIsInitialized)
	{
		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);

		if (iResult > 0) 
		{
			char tempBuf[DEFAULT_BUFLEN];
			std::strcpy(tempBuf, recvbuf);
			if (tempBuf[0] >= 0)
			{
				HandleServerMessage(tempBuf);
			}
		}
		else if (iResult == 0)
		{
			printf("No message\n");
		}
	}
	else if (m_clientIsInitialized)
	{
		//Taking care of the message
		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
		{
			char tempBuf[DEFAULT_BUFLEN];
			std::strcpy(tempBuf, recvbuf);
			if (tempBuf[0] != -52)
			{
				HandleServerMessage(tempBuf);
			}
		}
		else if (iResult == 0)
		{
			printf("No message\n");
		}
	}
	CheckForVictory();
}
void Network::Shutdown()
{
	SendDisconnectMessage();
	for (unsigned int i = 0; i < 9; i++)
	{
		m_tableLayout[i].value = 2;
	}
	m_instance->Disconnect();
	m_instance = nullptr;
	delete m_instance;
}
void Network::Disconnect()
{
	SendText("33333333333");
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
	char temp[DEFAULT_BUFLEN];
	temp[0] = '1';
	for (unsigned int i = 1; i < 10; i++)
	{
		if (m_tableLayout[i-1].value == 0)
			temp[i] = '0';
		if (m_tableLayout[i-1].value == 1)
			temp[i] = '1';
		if (m_tableLayout[i-1].value == 2)
			temp[i] = '2';
	}	
	if (m_whooseTurn == 0)
	{
		temp[10] = '1';
		m_whooseTurn = 1;
	}
	else
	{
		temp[10] = '0';
		m_whooseTurn = 0;
	}
	SendText(temp);
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
		//	printf("send from host failed: %d\n", WSAGetLastError());
		}
	}
	else if (m_clientIsInitialized)
	{
		int iResult = send(ConnectSocket, p_text, (int)strlen(p_text), 0);
		if (iResult == SOCKET_ERROR) 
		{
		//	printf("send from client failed: %d\n", WSAGetLastError());
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
			m_victory = 1;
		}
		else if (tile1.yPos == tile2.yPos && tile1.yPos == tile3.yPos)
		{
			//Win
			m_victory = 1;
		}
		else if (tile2.xPos == 1 && tile2.yPos == 1 && tile1.yPos == 0 && tile3.yPos == 2)
		{
			if (tile1.xPos == 0)
			{
				if (tile3.xPos == 2)
				{
					//Win
					m_victory = 1;
				}
			}
			if (tile1.xPos == 2)
			{
				if (tile3.xPos == 0)
				{
					//Win
					m_victory = 1;
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
			m_victory = 0;
		}
		else if (tile1.yPos == tile2.yPos && tile1.yPos == tile3.yPos)
		{
			//Win
			m_victory = 0;
		}
		else if (tile2.xPos == 1 && tile2.yPos == 1 && tile1.yPos == 0 && tile3.yPos == 2)
		{
			if (tile1.xPos == 0)
			{
				if (tile3.xPos == 2)
				{
					//Win
					m_victory = 0;
				}
			}
			if (tile1.xPos == 2)
			{
				if (tile3.xPos == 0)
				{
					//Win
					m_victory = 0;
				}
			}
		}
	}
	if (m_victory == 0)
	{
		SendText("20000000000");
	}
	else if (m_victory == 1)
	{
		SendText("21000000000");
	}
}
void Network::SetTile(int p_index)
{
	if (GetState() == 1)
	{
		m_tableLayout[p_index - 1].value = 0;
	}
	else if (GetState() == 2)
	{
		m_tableLayout[p_index - 1].value = 1;
	}
}
int Network::GetTileValue(int p_index)
{
	return m_tableLayout[p_index - 1].value;
}
void Network::HandleServerMessage(char p_message[])
{
	if (m_hostIsInitialized)
	{
		switch (p_message[0])
		{
		case '0'://idle
			break;
		case '1'://Update Table
			for (unsigned int i = 0; i < 9; i++)
			{
				if (p_message[i + 1] == '0')
					m_tableLayout[i].value = 0;
				else if (p_message[i + 1] == '1')
					m_tableLayout[i].value = 1;
				else if (p_message[i + 1] == '2')
					m_tableLayout[i].value = 2;
			}
			if (p_message[10] == '1')
			{
				m_whooseTurn = 1;
			}
			else
			{
				m_whooseTurn = 0;
			}

			break;
		case '2'://Someone won
			if (p_message[1] == '0')
			{//Circle won
				m_victory = 0;
			}
			else if (p_message[1] == '1')
			{//Cross won
				m_victory = 1;
			}
			break;
		case '3'://Disconnect
			m_disconnect = true;
			break;
		default:
			printf("Not a message\n");
			break;
		}
	}
	if (m_clientIsInitialized)
	{
		switch (p_message[0])
		{
		case '0'://idle
			//printf("Host is idle");
			break;
		case '1'://Update Table
			for (unsigned int i = 0; i < 9; i++)
			{
				if (p_message[i + 1] == '0')
					m_tableLayout[i].value = 0;
				if (p_message[i + 1] == '1')
					m_tableLayout[i].value = 1;
				if (p_message[i + 1] == '2')
					m_tableLayout[i].value = 2;
			}
			if (p_message[10] == '1')
			{
				m_whooseTurn = 1;
			}
			else
			{
				m_whooseTurn = 0;
			}
			break;
		case '2'://Someone won
			if (p_message[1] == '0')
			{//Circle won
				m_victory = 0;
			}
			else if (p_message[1] == '1')
			{//Cross won
				m_victory = 1;
			}
			break;
		case '3'://Disconnectu
			m_disconnect = true;
		default:
			printf("Not a message\n");
			break;
		}
	}
}
int Network::GetNumberOfPlacedTiles()
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

	if (GetState() == 1)
	{
		return circle;
	}
	else if (GetState() == 2)
	{
		return cross;
	}
	else
	{
		return 0;
	}
}
void Network::MoveTile(int p_selectedTileIndex, int p_destinationTileIndex)
{
	m_tableLayout[p_destinationTileIndex-1].value = m_tableLayout[p_selectedTileIndex-1].value;
	m_tableLayout[p_selectedTileIndex-1].value = 2;
}
bool Network::MyTurn()
{
	if (m_whooseTurn == 0 && GetState() == 1)
	{
		return true;
	}
	else if (m_whooseTurn == 1 && GetState() == 2)
	{
		return true;
	}
	return false;
}
bool Network::StartDisconnect()
{
	return m_disconnect;
}
int Network::VictoryState()
{
	return m_victory;
}
void Network::SendDisconnectMessage()
{
	SendText("33333333333");
}

int Network::GetWhooseTurn()
{
	if (GetState() == 0)
	{
		return 2;
	}
	return m_whooseTurn;
}