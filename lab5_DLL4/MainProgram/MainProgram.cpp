#include <stdio.h>
#include <iostream> 
#include <windows.h>
#include "..\Actions\Actions.h"

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

int main() {
	setlocale(LC_ALL, "Russian");

	bool isRunning = true;
	int choice;
	bool lang;
	bool correctChoice;

	printf("Choose the language\n");
	printf("1 - русский\n");
	printf("2 - english\n");
	scanf_s("%d", &choice);

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
				scanf_s("%d", &choice);
			}
			else
			{
				printf("%s", "Ваш выбор: ");
				scanf_s("%d", &choice);
			}

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
				break;
			default:
				if (!lang)
					printf("%s\n\n", "Некорректный выбор.");
				else
					printf("%s\n\n", "Incorrect choose.");
				break;
			}
		}
	}

}