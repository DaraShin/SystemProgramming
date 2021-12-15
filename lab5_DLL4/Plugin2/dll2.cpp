#include "pch.h"
#include "dll2.h"
#include <iostream>

BOOLEAN GetAuthor(LPSTR buffer, DWORD dwBufferSize, DWORD* pdwBytesWritten)
{
	int result = _snprintf_s(buffer, dwBufferSize, 50, "Created by Daria Shinkevich, FAMCS, BSU");
	*pdwBytesWritten = result;
	return result;
}

BOOLEAN GetDescription(LPSTR buffer, DWORD dwBufferSize, DWORD* pdwBytesWritten)
{
	int result = _snprintf_s(buffer, dwBufferSize, 50, "Opens \"System information\" window");
	*pdwBytesWritten = result;
	return result;
}

VOID Execute()
{
	system("msinfo32");
	printf("\n");
}