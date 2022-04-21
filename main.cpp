#include <stdio.h>
#include <windows.h>
#include <iostream>
#include <tlhelp32.h>
#include <wchar.h>
#include <psapi.h>
#include <libloaderapi.h>
#include <vector>
#include "offsets.h"
#include "libhook.h"
#pragma comment( lib, "psapi.lib" )

const wchar_t* targetProcess = L"th06e.exe";

uintptr_t findDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets) {
	uintptr_t addr = ptr;
	for (unsigned int i = 0; i < offsets.size(); ++i) {
		ReadProcessMemory(hProc, (BYTE*)addr, &addr, sizeof(addr), 0);
		addr += offsets[i];
	}
	return addr;

}
uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName)
{
	printf("GetModuleBaseAddress :: negotiation started on PID 0x%08X\n", GetProcessId((HANDLE)procId));
	uintptr_t modBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetProcessId((HANDLE)procId));
	if (hSnap != INVALID_HANDLE_VALUE){
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry)){
			wprintf(L"mod name :: %s\n", modEntry.szExePath);
			do{
				if (!_wcsicmp(modEntry.szModule, modName)){
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
	else {
		printf("error recovering modules\n");
	}
	CloseHandle(hSnap);
	return modBaseAddr;
}

HANDLE* getProcessHandler() {
	PROCESSENTRY32 pTable;
	pTable.dwSize = sizeof(PROCESSENTRY32);
	HANDLE *processSnapshot = (HANDLE*)CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, TH32CS_SNAPALL);
	if (Process32First(processSnapshot, &pTable) == TRUE) {
		while (Process32Next(processSnapshot, &pTable) == TRUE) {
			if (wcscmp(pTable.szExeFile, targetProcess) == 0) {
				wprintf(L"g0t process [ ID :: %lu | NAME :: %s | FLAGS :: %lu | SIZE :: %lu ]\n\n", pTable.th32ProcessID, pTable.szExeFile, pTable.dwFlags, pTable.dwSize);
				HANDLE* hProcess = (HANDLE*)OpenProcess(PROCESS_ALL_ACCESS, FALSE, pTable.th32ProcessID);
				return hProcess;
			}
		}
	}
}

DWORD WINAPI controllerThread(LPVOID lpParameter){
	DWORD powerValue;

	while (true) {
		ReadProcessMemory(GetCurrentProcess(), (LPVOID)(0x69D4BB), &powerValue, sizeof(powerValue), nullptr);
		printf("\nCURRENT BOMBS :: %d", powerValue);
	}
}



void EnableDebugPriv() {
	HANDLE hToken;
	LUID luid;
	TOKEN_PRIVILEGES tkp;

	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);

	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid);

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = luid;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	AdjustTokenPrivileges(hToken, false, &tkp, sizeof(tkp), NULL, NULL);

	CloseHandle(hToken);
}




int main() {
	EnableDebugPriv();
	setvbuf(stdout, NULL, _IONBF, 0);
	HANDLE *tProcHandler = NULL;
	tProcHandler = getProcessHandler();
	HMODULE tProcModules[1024];
	DWORD cbNeeded;
	char szModName[MAX_PATH];
	int* pointerArray[1024];

	if (tProcHandler == 0) {
		printf("Cannot get a handle to %s process\n", targetProcess);
		printf("Error : %d\n\n", GetLastError());
	}
	printf("g0t module bases\n\n");
	if (EnumProcessModules(tProcHandler, tProcModules, sizeof(tProcModules), &cbNeeded)) {
		for (int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
			if (GetModuleFileNameExA(tProcHandler, tProcModules[i], szModName, sizeof(szModName) / sizeof(WCHAR))) {
				printf("%s\t | (0x%08X)\n", szModName, tProcModules[i]);
			}
		}
	}

	uintptr_t gameBase = GetModuleBaseAddress((DWORD)tProcHandler, targetProcess);

	CreateRemoteThread(tProcHandler, NULL, NULL, (LPTHREAD_START_ROUTINE)controllerThread, NULL, NULL, NULL);
	while (true) {
		printf(" ");
	}

	CloseHandle(tProcHandler);
}
