#pragma once

#include <stdio.h>
#include <windows.h>

#define  MAXPLUGINSALLOWED 10
#define  MAXNAMELENGTH 100

#ifdef __cplusplus

extern "C"
{
#endif

	typedef void(*PluginExecute)();
	typedef bool(*PluginInfo)(LPSTR, DWORD, DWORD*);
	//__declspec(dllexport) - object is available for import from dd
	__declspec(dllexport) HINSTANCE Plugins[MAXPLUGINSALLOWED];
	__declspec(dllexport) int pluginsCount;
	//
	__declspec(dllexport) wchar_t pluginNames[MAXPLUGINSALLOWED][MAXNAMELENGTH];

	__declspec(dllexport) void LoadPlugins(bool lang);
	__declspec(dllexport) void UnloadPlugins(bool lang);
	__declspec(dllexport) void ShowPluginsList(bool lang);
	__declspec(dllexport) void ShowInfo(bool lang);
	__declspec(dllexport) void RunPlugins(bool lang);
	__declspec(dllexport) void Exit(bool lang);
	__declspec(dllexport) void ShowPluginsList(bool lang);

#ifdef __cplusplus
};
#endif