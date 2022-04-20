#pragma once
HMODULE WINAPI LoadLibraryAHook(LPCSTR lpFileName);
typedef HMODULE(WINAPI* LoadLibrary_t)(LPCSTR);
void Hook_Kernel32_LoadLibraryA();
