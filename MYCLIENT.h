#define _WINSOCK_DEPRECATED_NO_WARNINGS


#include <winsock2.h>
#include <stdio.h>
#include <windows.h>
#include <string>
#include <vector>
#include <chrono>

#ifndef MYCLIENT_H
#define MYCLIENT_H

//#define HOST_IP "161.109.18.42"

class MyClient;

extern MyClient *client;

class MyClient {
public:

	char recvData[1024];
	std::vector<char*> recvMessages;
	// char HOST_IP[15] = "161.109.32.107";
	char HOST_IP[15] = "161.109.32.105";
	bool isOpenConnection;

	SOCKET ConnectSocket;

	MyClient() {}

	//	SETS UP EVERYTHING
	bool GetReadyToDoStuff() {
		//----------------------
		// Initialize Winsock.
		WSADATA wsaData;
		int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != NO_ERROR) {
			//wprintf(L"WSAStartup failed with error: %ld\n", iResult);
			return false;
		}

		//----------------------
		// Create a SOCKET for connecting to server
		ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (ConnectSocket == INVALID_SOCKET) {
			//printf("Error at socket(): %ld\n", WSAGetLastError());
			WSACleanup();
			return false;
		}
		//----------------------
		// The sockaddr_in structure specifies the address family,
		// IP address, and port for the socket that is being bound.
		sockaddr_in addrServer;
		addrServer.sin_family = AF_INET;
		addrServer.sin_addr.s_addr = inet_addr(HOST_IP);
		addrServer.sin_port = htons(20131);

		//----------------------
		// Connect to server.
		iResult = connect(ConnectSocket, (SOCKADDR*)&addrServer, sizeof(addrServer));
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			return false;
		}

		// MessageBox(0, (LPCSTR)clientNum, 0, 0);

		u_long iMode = 1;
		ioctlsocket(ConnectSocket, FIONBIO, &iMode);

		return true;
	}
	
	//	recieve messages from the server and put them in a stack
	void RecvMessages() {
		//while (!recvMessages.empty())
			//recvMessages.pop_back();
		int RecvBytes;
		do {
			RecvBytes = recv(ConnectSocket, recvData, sizeof(recvData), 0);
			recvMessages.push_back(recvData);
		} while (RecvBytes != -1);
	}

	//	shut down client/reset for next use
	void Reset() {
		recvMessages.clear();
		memset(&recvData[0], 0, sizeof(recvData));
		int iResult = shutdown(ConnectSocket, SD_SEND);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			WSACleanup();
		}
	}

};


#endif




