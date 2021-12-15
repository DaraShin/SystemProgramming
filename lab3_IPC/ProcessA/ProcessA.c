#include <windows.h>
#include <stdio.h>
#include <TlHelp32.h>

#define MAXLENGTH 101

DWORD GetProcessID(wchar_t* process) {
	PROCESSENTRY32 processEntry;
	DWORD processID = 0;
	HANDLE Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (Snapshot != INVALID_HANDLE_VALUE) {
		processEntry.dwSize = sizeof(processEntry);
		Process32First(Snapshot, &processEntry);

		do {
			if (wcscmp(processEntry.szExeFile, process) == 0) {
				processID = processEntry.th32ProcessID;
				break;
			}
		} while (Process32Next(Snapshot, &processEntry));

		CloseHandle(Snapshot);
	}

	return processID;
}

int main()
{
	LPCTSTR pipename = L"\\\\.\\pipe\\mynamedpipe";
	HANDLE hProcessB;
	LPVOID procBAddress;
	DWORD numOfWrittenBytes, numOfReadBytes, dwProcBAdress;	
	BOOL success;		//used to mark if the operation was successful
	DWORD inputStringMaxSize = sizeof(char) * MAXLENGTH;
	char* inputString = (char*)malloc(inputStringMaxSize);
	HANDLE hPipe = INVALID_HANDLE_VALUE;

	printf("Enter the string( max length - %d): ", MAXLENGTH);
	fgets(inputString, MAXLENGTH - 1, stdin);

	DWORD PID = GetProcessID(L"ProcessB.exe");
	//Open existing process B
	hProcessB = OpenProcess(
		PROCESS_ALL_ACCESS,								// access to the process object
		FALSE,											// whether or not processes created by this process will inherit the handle
		PID);
	if (hProcessB == INVALID_HANDLE_VALUE) {
		printf("Opening process B FAILED\n");
		return -1;
	}
	else {
		printf("Opened ProcessB\n");
	}

	//Allocate memory in process's B address space, procBAddress - address of the allocated memory
	procBAddress = VirtualAllocEx(
		hProcessB,
		NULL,											// the function determines where to allocate the region.
		inputStringMaxSize,
		// reserve and commit pages
		MEM_RESERVE |									// reserves a range of the process's virtual address space
		MEM_COMMIT,										// allocates memory charges for the specified reserved memory pages
		PAGE_EXECUTE_READWRITE);						// enable execute, read-only, or read/write access to the committed region of pages
	printf("Allocated memory in ProcessB\n");

	//write the string into process B memory
	success = WriteProcessMemory(
		hProcessB,
		procBAddress,
		inputString,
		inputStringMaxSize,
		0);
	if (success) {
		printf("Wrote the string in ProcessB memory\n");
	}
	else {
		printf("Writing the string in ProcessB memory FAILED");
		return -1;
	}

	//creating named pipe
	while (hPipe == INVALID_HANDLE_VALUE) {
		hPipe = CreateFile(
			pipename,
			GENERIC_READ |								// access to the pipe
			GENERIC_WRITE,
			0,											// without sharing
			NULL,										// default security descriptor
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);										// ignored parameter(template file)
	}
	printf("Connected to named pipe\n");

	//send address of the string to print to ProcessB 
	dwProcBAdress = (DWORD)procBAddress;
	success = WriteFile(
		hPipe,
		&dwProcBAdress,			// data to write to the pipe 
		sizeof(dwProcBAdress),
		&numOfWrittenBytes,
		NULL);
	if (success) {
		printf("Wrote memory address\n");
	}
	else {
		printf("Writing memory address FAILED\n");
		return -1;
	}

	//get answer from process b
	success = ReadFile(
		hPipe,
		&procBAddress,
		sizeof(procBAddress),
		&numOfReadBytes,								
		NULL);											
	if (success) {
		printf("Received answer from ProcessB\n");
	}
	else {
		printf("Receiving answer from ProcessB FAILED\n");
		return -1;
	}

	success = VirtualFreeEx(
		hProcessB,
		procBAddress,
		0,
		MEM_RELEASE);
	if (success) {
		printf("Released ProcessB memory\n");
	}
	else {
		printf("Releasing ProcessB memory FAILED\n");
		return -1;
	}

	success = CloseHandle(hPipe);
	if (success) {
		printf("Closed named pipe\n");
	}
	else {
		printf("Closing named pipe FAILED\n");
		return -1;
	}

	success = CloseHandle(hProcessB);
	if (success) {
		printf("Closed ProcessB\n");
	}
	else {
		printf("Closing ProcessB FAILED\n");
		return -1;
	}

	printf("ProcessA was finished...\n");
	system("pause");
	return 0;
}
