#include "pch.h"
#include "Actions.h"

void PrintNoPluginsLoaded(bool lang)
{
	if (!lang)
		printf("%s\n\n", "Плагины не были загружены");
	else
		printf("%s\n\n", "There're no loaded plugins");

}

void LoadPlugins(bool lang)
{
	pluginsCount = 0;
	WIN32_FIND_DATA file;	//file description
	HANDLE hFile = FindFirstFile(L"Plugins\\*.dll", &file);//looks for the file with the specified name

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return;
	}
	do
	{
		if (pluginsCount == MAXPLUGINSALLOWED)
		{
			break;
		}
		wchar_t buf[256] = L"Plugins\\";
		wcscat_s(buf, 256, file.cFileName);
		Plugins[pluginsCount] = LoadLibrary((LPCWSTR)buf);
		wcscpy_s(pluginNames[pluginsCount], 100, file.cFileName);
		++pluginsCount;
	} while (FindNextFile(hFile, &file));
	FindClose(hFile);

	if (!lang)
		printf("Загружено плагинов: %d \n\n", pluginsCount);
	else
		printf("Plugins loaded: %d \n\n", pluginsCount);
}

void freeDlls(bool lang)
{
	do
	{
		FreeLibrary(Plugins[pluginsCount - 1]);//сообщит Windows, что данная программа DLL больше не использует
		Plugins[pluginsCount - 1] = NULL;
		--pluginsCount;
	} while (pluginsCount != 0);

	if (!lang)
		printf("%s\n\n", "Все плагины были выгружены");
	else
		printf("%s\n\n", "All plugins were unloaded.");
}

void UnloadPlugins(bool lang)
{
	if (pluginsCount == 0)
	{
		PrintNoPluginsLoaded(lang);
		return;
	}
	freeDlls(lang);
}

void ShowPluginsList(bool lang)
{
	if (pluginsCount == 0)
	{
		PrintNoPluginsLoaded(lang);
		return;
	}

	if (!lang)
		printf("Доступные плагины:\n");
	else
		printf("Available plugins:\n");

	for (int i = 0; i < pluginsCount; ++i)
	{
		wchar_t buffer[MAXNAMELENGTH];
		swprintf(buffer, MAXNAMELENGTH, L"%s\n", pluginNames[i]);
		fputws(buffer, stdout);
	}
	printf("\n");
}

void ShowInfo(bool lang)
{
	if (pluginsCount == 0)
	{
		PrintNoPluginsLoaded(lang);
		return;
	}

	DWORD bufdword = 0;
	char buffer[512];
	PluginInfo PI;
	for (int i = 0; i < pluginsCount; ++i)
	{
		PI = (PluginInfo)GetProcAddress(Plugins[i], "GetDescription");
		PI(buffer, 512, &bufdword);

		wchar_t buffer2[MAXNAMELENGTH];
		swprintf(buffer2, MAXNAMELENGTH, L"%s\n", pluginNames[i]);
		fputws(buffer2, stdout);

		if (!lang)
			printf("Описание плагина: %s\n", buffer);
		else
			printf("Plugin description: %s\n", buffer);

		PI = (PluginInfo)GetProcAddress(Plugins[i], "GetAuthor");
		PI(buffer, 512, &bufdword);
		if (!lang)
			printf("%s\n\n", buffer);
		else
			printf("%s\n\n", buffer);
	}
}

void RunPlugins(bool lang)
{
	if (pluginsCount == 0)
	{
		PrintNoPluginsLoaded(lang);
		return;
	}

	PluginExecute PE;
	for (int i = 0; i < pluginsCount; ++i)
	{
		PE = (PluginExecute)GetProcAddress(Plugins[i], "Execute");
		PE();
	}
}


void Exit(bool lang)
{
	if (pluginsCount != 0)
	{
		freeDlls(lang);
	}

	if (!lang)
		printf("%s", "Программа завершена...");
	else
		printf("%s", "Program finished...");
}