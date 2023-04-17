#pragma once
#include <Windows.h>

BOOL WriteMemory(LPVOID lpAddress, LPCVOID lpBuffer, SIZE_T nSize);
BOOL WriteHookCode(DWORD dwRawAddress, DWORD dwNewAddress, SIZE_T szHookCode);
BOOL HookCreateFontA(DWORD dwCharSet, LPCSTR lpFontName);
BOOL DetourAttachFunc(PVOID ppRawFunc, PVOID pNewFunc);