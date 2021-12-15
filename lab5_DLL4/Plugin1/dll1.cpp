#include "pch.h"
#include "dll1.h"
#include <stdio.h>

BOOLEAN GetAuthor(LPSTR buffer, DWORD dwBufferSize, DWORD* pdwBytesWritten)
{
	int result = _snprintf_s(buffer, dwBufferSize, 50, "Created by Daria Shinkevich, FAMCS, BSU");
	*pdwBytesWritten = result;
	return result;
}

BOOLEAN GetDescription(LPSTR buffer, DWORD dwBufferSize, DWORD* pdwBytesWritten)
{
	int result = _snprintf_s(buffer, dwBufferSize, 17, "Shows system info!");
	*pdwBytesWritten = result;
	return result;
}

VOID Execute()
{
	SYSTEM_INFO systInfo;

	// Copy the hardware information to the SYSTEM_INFO structure. 

	GetSystemInfo(&systInfo);

	// Display the contents of the SYSTEM_INFO structure. 

	printf("Hardware information: \n");
	printf("  Number of processors: %u\n",
		systInfo.dwNumberOfProcessors);
	//printf("  Processor type: %u\n", systInfo.dwProcessorType);
	printf("  Processor type: %u\n", systInfo.wProcessorArchitecture);
	printf("  Page size: %u\n", systInfo.dwPageSize);
	printf("  Minimum application address: %lx\n",
		(unsigned long)systInfo.lpMinimumApplicationAddress);
	printf("  Maximum application address: %lx\n",
		(unsigned long)systInfo.lpMaximumApplicationAddress);
	printf("  Active processor mask: %u\n",
		systInfo.dwActiveProcessorMask);
}