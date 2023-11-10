#include <iostream>
#include <Windows.h>
#include <cstdio>
#include <cstdint>
#include <string>
#include <tlhelp32.h>  // Add this for Process32First, Process32Next, PROCESSENTRY32, TH32CS_SNAPPROCESS
#include "driver.h"

int processid = 0;

DWORD GetProcessID(const std::wstring processName)
{
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);

	HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (processesSnapshot == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	Process32First(processesSnapshot, &processInfo);
	if (!processName.compare(processInfo.szExeFile))
	{
		CloseHandle(processesSnapshot);
		return processInfo.th32ProcessID;
	}

	while (Process32Next(processesSnapshot, &processInfo))
	{
		if (!processName.compare(processInfo.szExeFile))
		{
			CloseHandle(processesSnapshot);
			return processInfo.th32ProcessID;
		}
	}

	CloseHandle(processesSnapshot);
	return 0;
}

int main()
{
	processid = GetProcessID(L"cs2.exe");
	printf("> Process ID: %lu\n", processid);
}