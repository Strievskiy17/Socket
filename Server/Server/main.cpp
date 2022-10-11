#include <iostream>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

using namespace std;

void main()
{
	//Инициализация WinSock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsok = WSAStartup(ver, &wsData);
	if (wsok != 0) 
	{
		cout << "No Initialising WinSock" << endl;
		return;
	}

	//Сохдание socet
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cout << "Can't create a socket!" << endl;
		return;
	}

	//привязка порта и IP адреса к socket

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = (54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	//Сказать WinSock установить listening
	listen(listening, SOMAXCONN);

	//ждать подключения
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

	//закрыть listening socket

	closesocket(listening);
	
	//Цикл: Принять и отправить сообщение клиенту

	char buff[4096];
	while (true)
	{
		ZeroMemory(buff, 4096);
		//Ожидание отправления сообщения от клиента
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

		//Вывод сообщения обратно клиенту

		send(clientSocket, buff, byteRecived + 1, 0);

	}

	//закрытие socket

	closesocket(clientSocket);

	//Закртыие WinSock

	WSACleanup();

}