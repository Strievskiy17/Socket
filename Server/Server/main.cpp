#include <iostream>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

using namespace std;

void main()
{
	//������������� WinSock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsok = WSAStartup(ver, &wsData);
	if (wsok != 0) 
	{
		cout << "No Initialising WinSock" << endl;
		return;
	}

	//�������� socet
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cout << "Can't create a socket!" << endl;
		return;
	}

	//�������� ����� � IP ������ � socket

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = (54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	//������� WinSock ���������� listening
	listen(listening, SOMAXCONN);

	//����� �����������
	sockaddr_in client;
	int clientsize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientsize);

	char host[NI_MAXHOST];
	char service[NI_MAXSERV];

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		cout << host << "Connected oh port " << service << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " connected on port " << ntohs(client.sin_port) << endl;
	}

	//������� listening socket

	closesocket(listening);
	
	//����: ������� � ��������� ��������� �������

	char buff[4096];
	while (true)
	{
		ZeroMemory(buff, 4096);
		//�������� ����������� ��������� �� �������
		int byteRecived = recv(clientSocket, buff, 4096, 0);
		if (byteRecived == SOCKET_ERROR)
		{
			cout << "Erorr in recv(). Quiting" << endl;
			break;
		}

		if (byteRecived == 0)
		{
			cout << " Client Disconected" << endl;
			break;
		}

		//����� ��������� ������� �������

		send(clientSocket, buff, byteRecived + 1, 0);

	}

	//�������� socket

	closesocket(clientSocket);

	//�������� WinSock

	WSACleanup();

}