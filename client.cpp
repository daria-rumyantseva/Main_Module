
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <inaddr.h>
#include <stdio.h>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

using namespace std;


int main(void)
{
	//Ключевые константы
	const char SERVER_IP[] = "";					// Введите IPv4-адрес сервера
	const short SERVER_PORT_NUM = 0;				// Введите порт прослушивания на стороне сервера
	const short BUFF_SIZE = 1024;					// Максимальный размер буфера для обмена информацией между сервером и клиентом

	// Ключевые переменные для всей программы
	int erStat;										// Для проверки ошибок в функциях сокетов

 	// IP в строковом формате в числовой формат для функций сокета. Данные находятся в «ip_to_num»
	in_addr ip_to_num;		
	inet_pton(AF_INET, SERVER_IP, &ip_to_num);


	// Инициализация WinSock
	WSADATA wsData;
	erStat = WSAStartup(MAKEWORD(2,2), &wsData);

	if (erStat != 0) {
		cout << "Error WinSock version initializaion #";
		cout << WSAGetLastError();
		return 1;
	}
	else 
		cout << "WinSock initialization is OK" << endl;
	
	// Инициализация Socket
	SOCKET ClientSock = socket(AF_INET, SOCK_STREAM, 0);

	if (ClientSock == INVALID_SOCKET) {
		cout << "Error initialization socket # " << WSAGetLastError() << endl;
		closesocket(ClientSock);
		WSACleanup();
	}
	else 
		cout << "Client socket initialization is OK" << endl;

	// Установление соединения с сервером
	sockaddr_in servInfo;

	ZeroMemory(&servInfo, sizeof(servInfo));

	servInfo.sin_family = AF_INET;
	servInfo.sin_addr = ip_to_num;	
	servInfo.sin_port = htons(SERVER_PORT_NUM);

	erStat = connect(ClientSock, (sockaddr*)&servInfo, sizeof(servInfo));
	
	if (erStat != 0) {
		cout << "Connection to Server is FAILED. Error # " << WSAGetLastError() << endl;
		closesocket(ClientSock);
		WSACleanup();
		return 1;
	}
	else 
		cout << "Connection established SUCCESSFULLY. Ready to send a message to Server" << endl;


	// Обмен текстовыми данными между Сервером и Клиентом. Отключение, если Клиент отправляет «xxx»

	vector <char> servBuff(BUFF_SIZE), clientBuff(BUFF_SIZE);							// Буферы для отправки и получения данных
	short packet_size = 0;												// Размер отправляемого/принимаемого пакета в байтах
		
	while (true) {

		cout << "Your (Client) message to Server: ";
		fgets(clientBuff.data(), clientBuff.size(), stdin);

		// Проверьте, хочет ли клиент прекратить общение
		if (clientBuff[0] == 'x' && clientBuff[1] == 'x' && clientBuff[2] == 'x') {
			shutdown(ClientSock, SD_BOTH);
			closesocket(ClientSock);
			WSACleanup();
			return 0;
		}

		packet_size = send(ClientSock, clientBuff.data(), clientBuff.size(), 0);

		if (packet_size == SOCKET_ERROR) {
			cout << "Can't send message to Server. Error # " << WSAGetLastError() << endl;
			closesocket(ClientSock);
			WSACleanup();
			return 1;
		} 

		packet_size = recv(ClientSock, servBuff.data(), servBuff.size(), 0);

		if (packet_size == SOCKET_ERROR) {
			cout << "Can't receive message from Server. Error # " << WSAGetLastError() << endl;
			closesocket(ClientSock);
			WSACleanup();
			return 1;
		}
		else
			cout << "Server message: " << servBuff.data() << endl;

	}

	closesocket(ClientSock);
	WSACleanup();

	return 0;
}