#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>

namespace Memory
{

	std::string RandomString(const size_t length)
	{
		std::string r;
		static const char bet[] = { "ABCDEFGHIJKLMNOPQRSTUVWXYabcdefghijklmnopqrstuvwxyzZ1234567890" };
		srand((unsigned)time(NULL) * 5);
		for (int i = 0; i < length; ++i)
			r += bet[rand() % (sizeof(bet) - 1)];
		return r;
	}

	DWORD GetProcID(const wchar_t* procName)
	{
		PROCESSENTRY32 procEntry = { 0 };
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

		if (!hSnapshot)
		{
			return 0;
		}

		procEntry.dwSize = sizeof(procEntry);

		if (!Process32First(hSnapshot, &procEntry))
		{
			return 0;
		}

		do
		{
			if (!wcscmp(procEntry.szExeFile, procName))
			{
				CloseHandle(hSnapshot);
				return procEntry.th32ProcessID;
			}
		} while (Process32Next(hSnapshot, &procEntry));

		CloseHandle(hSnapshot);
		return 0;
	}

	uintptr_t GetModule(DWORD dwProcessID, const wchar_t* lpszModuleName)
	{
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessID);
		uintptr_t dwModuleBaseAddress = 0;
		if (hSnapshot != INVALID_HANDLE_VALUE)
		{
			MODULEENTRY32 ModuleEntry32 = { 0 };
			ModuleEntry32.dwSize = sizeof(MODULEENTRY32);
			if (Module32First(hSnapshot, &ModuleEntry32))
			{
				do
				{
					if (wcscmp(ModuleEntry32.szModule, lpszModuleName) == 0)
					{
						dwModuleBaseAddress = (uintptr_t)ModuleEntry32.modBaseAddr;
						break;
					}
				} while (Module32Next(hSnapshot, &ModuleEntry32));
			}
			CloseHandle(hSnapshot);
		}
		return dwModuleBaseAddress;

	}

	uintptr_t process = Memory::GetProcID(L"csgo.exe");

	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, NULL, process);

	template <typename T>
	T Read(DWORD address)
	{
		T value = {  };
		ReadProcessMemory(processHandle, (LPCVOID)address, &value, sizeof(value), NULL);
		return value;
	}

	template <typename T>
	const void Write(DWORD address, T value)
	{
		WriteProcessMemory(processHandle, (LPVOID)address, &value, sizeof(value), NULL);
	}
}