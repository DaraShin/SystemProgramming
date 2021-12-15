#include <windows.h>
#include <stdio.h>

int main(void) {

	LPCTSTR pipename = L"\\\\.\\pipe\\mynamedpipe";					
	HANDLE hNamedPipe;												
	DWORD address;									// address of the allocared memory
	DWORD numOfReadBytes, numOfBytesToWrite;
	char* receivedStr;												
	BOOL success;									//used to mark if the operation was successful


	hNamedPipe = CreateNamedPipe(
		pipename,													
		PIPE_ACCESS_DUPLEX,											
		PIPE_TYPE_MESSAGE |							// Data is written from and read from the pipe as a stream of messages
		PIPE_READMODE_MESSAGE |
		PIPE_WAIT,									// waiting for a client process to perform an action
		PIPE_UNLIMITED_INSTANCES,									
		sizeof(DWORD),								// output buffer reserved size
		sizeof(DWORD),								// input buffer reserved size
		100,														
		NULL);										// default security descriptor
	if (hNamedPipe == INVALID_HANDLE_VALUE) {
		printf("Creating named pipe FAILED");
		system("pause");
		return -1;
	}
	else {
		printf("Created named pipe\n");
	}

	//wait for a client process to connect to the named pipe
	ConnectNamedPipe(hNamedPipe, NULL);
	printf("Connected to pipe\n");

	//receive address of the string to output
	success = ReadFile(
		hNamedPipe,	
		&address,			// buffer to read
		sizeof(address),
		&numOfReadBytes,
		NULL);
	if (success) {
		printf("Read address from pipe\n");
	}
	else {
		printf("Reading address from pipe FAILED\n");
		system("pause");
		return -1;
	}

	printf("Your string: ");
	receivedStr = (char*)address;
	printf_s("%s\n", receivedStr);

	success = WriteFile(
		hNamedPipe,
		&address,				
		sizeof(address),
		&numOfBytesToWrite,	
		NULL);
	if (success) {
		printf("Sent answer to ProccessB\n");
	}
	else {
		printf("Sending answer to ProccessB FAILED\n");
		system("pause");
		return -1;
	}

	success = DisconnectNamedPipe(hNamedPipe);
	if (success) {
		printf("Disconnected from named pipe\n");
	}
	else {
		printf("Disconnecting from named pipe FAILED\n");
		system("pause");
		return -1;
	}

	success = CloseHandle(hNamedPipe);
	if (success) {
		printf("Closed named pipe named pipe\n");
	}
	else {
		printf("Closing named pipe FAILED\n");
		system("pause");
		return -1;
	}


	printf("ProcessB was finished...\n");
	system("pause");
	return 0;
}