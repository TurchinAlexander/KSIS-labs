#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define maxsize 1024
#define DEFAULT_PORT "27015"

//Связь с Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

#include <stdio.h>
#include <WinSock2.h>
#include <string.h>
#include <cstdio>

int main() {
	WSADATA wsaData;
	int iResult;

	SOCKET ConnectSocket;
	sockaddr_in clientService;

	char *sendbuf;
	sendbuf = (char*)malloc(maxsize * sizeof(char));
	strcpy(sendbuf, "this is a test\0");
	char recvbuf[maxsize];

	//char   *tstr, node[128], service[16];
	
	// Инициализация Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		printf("WSAStartup failed: %d\n", iResult);
		getchar();
		return 1;
	}

	//Создание сокета для связи
	ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//INVALID_SOCKET = -1
	if (INVALID_SOCKET == ConnectSocket) {
		printf("Не удалось создать сокет");
		getchar();
		return 1;
	}

	//----------------------
	// The sockaddr_in структура содержит семейство адресов,
	// IP адрес и порт сервера, к которому нужно подключиться.
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr("127.0.0.1");
	clientService.sin_port = htons(27015);

	//----------------------
	// Соединение с сервером.
	//SOCKET_ERROR = -1 
	iResult = connect(ConnectSocket, (SOCKADDR*)&clientService, sizeof(clientService));
	if (SOCKET_ERROR == iResult) {
		closesocket(ConnectSocket);
		printf("Unable to connect to server: %ld\n", WSAGetLastError());
		WSACleanup();
		getchar();
		return 1;
	}

	/*// Попытка отправить тестовое сообщение
	iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		getchar();
		return 1;
	}*/

	printf("Bytes Sent: %ld\n", iResult);

	bool isWorking = true;

	printf("Enter # to exit\n");

	// Receive until the peer closes the connection
	do {
		gets_s(sendbuf, maxsize);
		//get("%s", sendbuf);
		if ('#' == *sendbuf)
			isWorking = false;
		else {

			// Отправка сообщения
			iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
			if (iResult == SOCKET_ERROR) {
				printf("send failed: %d\n", WSAGetLastError());
				closesocket(ConnectSocket);
				WSACleanup();
				getchar();
				return 1;
			}

			//Получение сообщения
			iResult = recv(ConnectSocket, recvbuf, maxsize, 0);
			if (iResult > 0) {
				printf("Bytes received: %d\n", iResult);
				recvbuf[iResult] = '\0';
				printf("%s \n", recvbuf);

			}
			else if (0 == iResult)
				printf("Connection closed\n");
			else
				printf("recv failed: %d\n", WSAGetLastError());
		}

	} while ((iResult > 0) and isWorking);

	// Закрытие связи, так как больше ничего посылаться не будет
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		getchar();
		return 1;
	}


	//Закрытие сокета после работы
	closesocket(ConnectSocket);
	WSACleanup();
	return 0;
}