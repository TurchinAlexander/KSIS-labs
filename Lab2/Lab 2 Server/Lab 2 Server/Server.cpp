#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define maxsize 1024

#pragma comment(lib, "Ws2_32.lib")

#include <stdio.h>
#include <WinSock2.h>

void FinishClean(SOCKET *sckt) {
closesocket(*sckt);
WSACleanup();
getchar();
}

int main() {
	WSADATA wsaData;
	int iResult;

	SOCKET ServerSocket;
	struct sockaddr_in serverService;

	char recvbuf[maxsize];
	//char   *tstr, node[128], service[16];

	// ������������� Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		printf("WSAStartup failed: %d\n", iResult);
		WSACleanup();
		getchar();
		return 1;
	}

	//�������� ������ ��� �����
	ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//SOCKET_ERROR = -1
	if (INVALID_SOCKET == ServerSocket) {
		printf("�� ������� ������� �����");
		WSACleanup();
		getchar();
		return 1;
	}

	//�����������������������������������������������������
	//sockaddr_in ��������� ���������� ��������� �������
	//IP-����� � ���� �������, � �������� ����� ������������
	serverService.sin_family = AF_INET;
	serverService.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverService.sin_port = htons(27015);

	iResult = bind(ServerSocket, (sockaddr*)&serverService, sizeof(serverService));
	if (SOCKET_ERROR == iResult) {
		wprintf(L"bind function failed with error %d\n", WSAGetLastError());
		FinishClean(&ServerSocket);
		return 1;
	}

	//������������� �������� ����������
	//�� ��������� ������
	if (SOCKET_ERROR == listen(ServerSocket, SOMAXCONN))
		printf("listen function failed with error: %d\n", WSAGetLastError());

	printf("Listening on socket...\n");

	SOCKET AcceptSocket;
	sockaddr_in AcceptService;
	int sz = sizeof(AcceptService);

	//������� ��������� � ��������
	AcceptSocket = accept(ServerSocket, (sockaddr*)&AcceptService, &sz);
	if (AcceptSocket == INVALID_SOCKET) {
		printf("accept failed with error: %ld\n", WSAGetLastError());
		FinishClean(&ServerSocket);
		return 1;
	}
	else
		printf("Client connected.\n");

	//����������� � ��������� ���������
	char sendbuf[maxsize];
	strcpy(sendbuf, "The message is accepted");

	// �������� ��������� � ��������� ������� ����������� � ���������
	do {
		//��������� ���������
		iResult = recv(AcceptSocket, recvbuf, maxsize, 0);
		if (iResult > 0) {
			printf("Bytes received: %d\n", iResult);
			recvbuf[iResult] = '\0';
			printf("The message is %s \n", recvbuf);

		// �������� ����������� � ���������
		//iResult = send(AcceptSocket, sendbuf, (int)strlen(sendbuf), 0);
		iResult = send(AcceptSocket, recvbuf, (int)strlen(recvbuf), 0);
		if (iResult == SOCKET_ERROR) {
			printf("send failed: %d\n", WSAGetLastError());
			shutdown(AcceptSocket, SD_SEND);
			closesocket(AcceptSocket);

			WSACleanup();
			getchar();
			return 1;
		}
	}
	else if (0 == iResult)
		printf("Connection closed\n");
		else
		printf("recv failed: %d\n", WSAGetLastError());

	} while (iResult > 0);
	// �������� �����, ��� ��� ������ ������ ���������� �� �����
	iResult = shutdown(ServerSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(ServerSocket);
		WSACleanup();
		getchar();
		return 1;
	}

	//�������� ������ ����� ������
	FinishClean(&ServerSocket);
	return 0;
}