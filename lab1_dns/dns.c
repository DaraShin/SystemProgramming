
/*************************************************************************
   LAB 1

	Edit this file ONLY!

*************************************************************************/

#define _CRT_SECURE_NO_WARNINGS 
#define MAX_HASH_TABLE_KEY_VALUE 13001
#define BUFFER_SIZE  200

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <math.h>
#include "dns.h"

typedef struct _DNS_RECORD
{
	char* domainName;
	IPADDRESS    ip;
	struct _DNS_RECORD* nextRecord;

} DNS_RECORD, * PDNS_RECORD;

DNSHandle InitDNS()
{
	PDNS_RECORD* hashTable = (PDNS_RECORD*)calloc((MAX_HASH_TABLE_KEY_VALUE + 1), sizeof(PDNS_RECORD));

	if (hashTable == NULL)
	{
		return INVALID_IP_ADDRESS;
	}

	for (int i = 0; i <= MAX_HASH_TABLE_KEY_VALUE; i++)
	{
		hashTable[i] = NULL;
	}

	return (DNSHandle)(hashTable);
}

unsigned int HashFunction(char* str)
{
	static const unsigned int b = 30013;
	unsigned int a = 15373;
	int hash = 1;
	int i = 0;
	while (str[i] != '\0')
	{
		hash = hash * a + (str[i]);
		a *= b;
		i++;
	}

	//return abs(hash % (MAX_HASH_TABLE_KEY_VALUE + 1));
	return (CalcHashModByHash(abs(hash)));		//use hash mod to avoid accessing array out of bounds
}

int CalcHashModByHash(int hash)
{
	return hash % (MAX_HASH_TABLE_KEY_VALUE + 1);
}

void AddToHashTable(PDNS_RECORD* hashTable, char* domainName, IPADDRESS ip)
{
	unsigned int key = HashFunction(domainName);

	PDNS_RECORD newRecord = (PDNS_RECORD)malloc(sizeof(DNS_RECORD));
	int length = strlen(domainName);
	(newRecord->domainName) = (char*)malloc((length + 1) * sizeof(char));
	strcpy(newRecord->domainName, domainName);
	newRecord->ip = ip;
	newRecord->nextRecord = NULL;

	if (hashTable[key] == NULL)
	{
		hashTable[key] = newRecord;
	}
	else	//list of DNS_RECORD in case of collisions
	{
		PDNS_RECORD pCurDNSRecord = hashTable[key];
		while (pCurDNSRecord->nextRecord != NULL)
		{
			pCurDNSRecord = pCurDNSRecord->nextRecord;
		}
		pCurDNSRecord->nextRecord = newRecord;
	}
}

void LoadHostsFile(DNSHandle hDNS, const char* hostsFilePath)
{
	FILE* fInput = NULL;
	fInput = fopen(hostsFilePath, "r");
	if (fInput == NULL)
	{
		printf("Can't open file %s", hostsFilePath);
		return;
	}

	PDNS_RECORD* hashTable = hDNS;
	if (hDNS == INVALID_DNS_HANDLE || hashTable == NULL)
	{
		printf("Invalid value of hDNS");
		return;
	}

	IPADDRESS ip = 0;
	while (!feof(fInput))
	{
		char buffer[BUFFER_SIZE] = { '\0' };
		int bufferPos = 0;

		char ch = '\0';
		int ipArray[4] = { 0 };
		for (int i = 0; i < 4; i++)		//read ip
		{
			ch = _fgetc_nolock(fInput);
			while (ch != '.' && ch != ' ' && ch != '\n' && ch != EOF)
			{
				buffer[bufferPos] = ch;
				bufferPos++;
				ch = _fgetc_nolock(fInput);
			}
			buffer[bufferPos] = '\0';
			ipArray[i] = atoi(buffer);
			bufferPos = 0;
		}

		for (int i = 0; i < BUFFER_SIZE - 1; i++)	//read domain name
		{
			ch = _fgetc_nolock(fInput);

			if (ch == ' ' || ch == '\t')
			{
				continue;
			}
			if (ch == EOF || ch == '\n' || ch == '\0')
			{
				break;
			}

			buffer[bufferPos] = ch;
			bufferPos++;
		}

		buffer[bufferPos] = '\0';

		ip = (ipArray[0] << 24 | ipArray[1] << 16 | ipArray[2] << 8 | ipArray[3]);

		AddToHashTable(hashTable, buffer, ip);

	}
	fclose(fInput);
}

IPADDRESS DnsLookUp(DNSHandle hDNS, const char* hostName)
{
	PDNS_RECORD* hashTable = hDNS;
	if (hDNS == INVALID_DNS_HANDLE || hashTable == NULL)
	{
		printf("Invalid value of hDNS");
		return INVALID_IP_ADDRESS;
	}

	int key = HashFunction(hostName);
	if (hashTable[key] == NULL)
	{
		return INVALID_IP_ADDRESS;
	}

	PDNS_RECORD pCurDNSRecord = hashTable[key];
	while (pCurDNSRecord != NULL)	//search for the hostName in the list that is available in array hashTable by key(hash of hostName)
	{
		if (strcmp(pCurDNSRecord->domainName, hostName) == 0)
		{
			return (IPADDRESS)(pCurDNSRecord->ip);
		}
		pCurDNSRecord = pCurDNSRecord->nextRecord;
	}
	return INVALID_IP_ADDRESS;
}

void ShutdownDNS(DNSHandle hDNS)
{
	PDNS_RECORD* hashTable = hDNS;
	if (hDNS == INVALID_DNS_HANDLE || hashTable == NULL)
	{
		printf("Invalid value of hDNS");
		return;
	}

	PDNS_RECORD pCurDNSRecord = NULL;
	PDNS_RECORD pNextDNSRecord = NULL;
	for (int i = 0; i <= MAX_HASH_TABLE_KEY_VALUE; i++)
	{
		if (hashTable[i] == NULL)
		{
			continue;
		}
		pCurDNSRecord = hashTable[i];
		while (pCurDNSRecord != NULL)
		{
			free(pCurDNSRecord->domainName);
			pNextDNSRecord = pCurDNSRecord->nextRecord;
			free(pCurDNSRecord);
			pCurDNSRecord = pNextDNSRecord;
		}
	}

	free(hashTable);
	hDNS = INVALID_DNS_HANDLE;
}
