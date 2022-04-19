#include <stdio.h>
#include <windows.h>
#include <iostream>
#include <tlhelp32.h>

char* targetProcess = "th06e.exe";

HANDLE* getProcessHandler(){
	PROCESSENTRY32 pTable;
	pTable.dwSize = sizeof(PROCESSENTRY32);
	HANDLE processSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, TH32CS_SNAPALL);
	
	if(Process32First(processSnapshot, &pTable) == TRUE){
				while(Process32Next(processSnapshot, &pTable) == TRUE){
					if(strcmp(pTable.szExeFile, targetProcess) == 0){
						printf("g0t process [ ID :: %lu | NAME :: %s | FLAGS :: %lu | SIZE :: %lu ]", pTable.th32ProcessID, pTable.szExeFile, pTable.dwFlags, pTable.dwSize);
						HANDLE *hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pTable.th32ProcessID);
						return hProcess;
					}
				}
	}
}


void EnableDebugPriv(){
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

int main(){
	EnableDebugPriv();
	setbuf(stdout, NULL);
	HANDLE tProcHandler = getProcessHandler();
	if (tProcHandler == 0){
        printf("Cannot get a handle to %s process\n", targetProcess);
        printf("Error : %d\n\n", GetLastError());
	}
	TerminateProcess(tProcHandler, 0); // testing purposes
	
						
	CloseHandle(tProcHandler);
}
