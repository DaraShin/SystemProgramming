#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h> 
#include <iostream> 
#include "..\Actions\Actions.h"
#include <windows.h>
#include <string>
#pragma comment(lib, "WS2_32.lib")
#pragma comment (lib, "..\\Debug\\Actions.lib")

void print_menu_en() {
	printf("Available actions:\n");
	printf("1 - Load plugins\n");
	printf("2 - Unload plugins\n");
	printf("3 - Show list of plugins\n");
	printf("4 - Show info about loaded plugins\n");
	printf("5 - Execute plugins\n");
	printf("6 - Clear console\n");
	printf("7 - Exit\n");
}

void print_menu_ru() {
	printf("Меню:\n");
	printf("1 - Загрузить плагины\n");
	printf("2 - Выгрузить плагины\n");
	printf("3 - Показать список плагинов\n");
	printf("4 - Показать информацию о загруженных плагинах\n");
	printf("5 - Выполнить плагины\n");
	printf("6 - Очистить консоль\n");
	printf("7 - Выход\n");
}

DWORD WINAPI clientSend(LPVOID lpParam, char* message) { //Отправка данных на сервер
	char buffer[1024];
	strcpy_s(buffer, 1024, message);
	SOCKET server = *(SOCKET*)lpParam;
	if (send(server, buffer, sizeof(buffer), 0) == SOCKET_ERROR) {
		std::cout << "send failed with error: " << WSAGetLastError() << std::endl;
		return -1;
	}
	return 1;
}

int main() {
	setlocale(LC_ALL, "Russian");

	WSADATA WSAData;
	SOCKET server;
	SOCKADDR_IN addr;
	WSAStartup(MAKEWORD(2, 0), &WSAData);
	if ((server = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		std::cout << "Socket creation failed with error: " << WSAGetLastError() << std::endl;
		return -1;
	}

	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //коннект к серверу
	addr.sin_family = AF_INET;
	addr.sin_port = htons(5555); //порт
	if (connect(server, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
		std::cout << "Server connection failed with error: " << WSAGetLastError() << std::endl;
		return -1;
	}

	std::cout << "Connected to server!" << std::endl;

	bool isRunning = true;
	int choice = 2;
	bool lang = true;
	bool correctChoice;
	char strChoice[100];

	printf("Choose the language\n");
	printf("1 - русский\n");
	printf("2 - english\n");
	scanf_s("%s", strChoice, 50);

	try {
		choice = std::stoi(strChoice);
		switch (choice) {
		case 1:
			lang = false;
			break;
		case 2:
			lang = true;
			break;
		default:
			printf("Invalid input. English languge was set\n");
		}

	}
	catch (...) {
		printf("Invalid input. English languge was set\n");
		printf("Send error report to server\n");

		char message[100] = "Invalid language choice. Input: ";
		strcat_s(message, 100, strChoice);
		strcat_s(message, 100, "\n");
		clientSend(&server, message);
	}

	while (isRunning)
	{
		if (lang)
		{
			print_menu_en();
			printf("Choose action:\n");
		}
		else
		{
			print_menu_ru();
			printf("Выберите действие:\n");
		}

		correctChoice = false;

		while (!correctChoice)
		{

			if (lang)
			{
				printf("%s", "Your choice: ");
			}
			else
			{
				printf("%s", "Ваш выбор: ");
			}
			scanf_s("%s", strChoice, 50);

			char exit[10] = "exit\n";

			try {
				choice = std::stoi(strChoice);
				switch (choice)
				{
				case 1:
					LoadPlugins(lang);
					correctChoice = true;
					break;
				case 2:
					UnloadPlugins(lang);
					correctChoice = true;
					break;
				case 3:
					ShowPluginsList(lang);
					correctChoice = true;
					break;
				case 4:
					ShowInfo(lang);
					correctChoice = true;
					break;
				case 5:
					RunPlugins(lang);
					correctChoice = true;
					break;
				case 6:
					system("cls");
					correctChoice = true;
					break;
				case 7:
					Exit(lang);
					correctChoice = true;
					isRunning = false;
					clientSend(&server, exit);
					break;
				default:
					if (!lang)
						printf("%s\n\n", "Некорректный выбор.");
					else
						printf("%s\n\n", "Incorrect choose.");
					break;
				}
			}
			catch (...) {
				if (!lang) {
					printf("Некорректный выбор\n");
					printf("Отчет об оштбке отправлен на сервер\n");
				}
				else {
					printf("Incorrect choose\n");
					printf("Send error report to server\n");
				}
					

				char message[100] = "Invalid action choice. Input: ";
				strcat_s(message, 100, strChoice);
				strcat_s(message, 100, "\n\0");
				clientSend(&server, message);
			}
		}
	}
	closesocket(server);
	WSACleanup();
}
