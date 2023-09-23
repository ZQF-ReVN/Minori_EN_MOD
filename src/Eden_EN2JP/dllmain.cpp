#include "../../lib/Rhk/RxHook.h"

using namespace Rut;


VOID __declspec(naked) ReadChar_Hook()
{
	_asm
	{
		movzx esi, byte ptr[eax];
		cmp esi, 0x81;
		jb Not_DBCS;

		//is_DBCS:
		movzx esi, word ptr[eax];
		inc eax;

	Not_DBCS:
		mov dword ptr[ebp - 0x10], esi;
		ret
	}
}

VOID __declspec(naked) ToWideChar_Hook()
{
	__asm
	{
		// Process Return Address
		pop edx;
		add edx, 0x77;
		push edx;

		// If Not DBCS Just Reture
		cmp eax, 0x81;
		jb Not_DBCS;

		// If DBCS Convert to WideChar
		xor edx, edx;
		mov dword ptr ss : [ebp - 0x8] , edx;   // Cls MultiByteStr Buffer
		mov dword ptr ss : [ebp - 0xC] , edx;   // Cls WideCharStr Buffer
		mov word ptr ss : [ebp - 0x8] , ax;     // Write MultiByteStr
		push 0x1;                               // Param6: cchWideChar
		lea eax, ss: [ebp - 0xC] ;              // &WideCharStr
		push eax;                               // Param5: lpWideCharStr
		push 0x2;                               // Param4: cbMultiByte
		lea ecx, ss: [ebp - 0x8] ;              // &MultiByteStr
		push ecx;                               // Param3: lpMultiByteStr
		push 0;                                 // Param2: dwFlags
		push 0x3A4;                             // Param1: CodePage
		call MultiByteToWideChar;
		mov edx, dword ptr ss : [ebp - 0xC] ;
		mov dword ptr ss : [ebp - 0x8] , edx;
		mov dword ptr ss : [ebp - 0x4] , edx;   // Out Buffer

	Not_DBCS:
		ret;
	}
}

VOID StartHook()
{
	DWORD image_base = (DWORD)GetModuleHandleW(NULL);

	// Hook Font
	RxHook::HookCreateFontA(0x80, "MS Mincho");

	// Hook Read Char
	RxHook::SetHookCode_Call(image_base + 0x10EFD7, (DWORD)ReadChar_Hook, 0x6);

	// Hook ToWideChar
	RxHook::SetHookCode_Call(image_base + 0x119F94, (DWORD)ToWideChar_Hook, 0x7C);

	// Replace Character Name Brackets
	static BYTE bracket_r[] = { 0x20,0x81,0x7A };
	static BYTE bracket_l[] = { 0x81,0x79,0x20 };
	RxHook::SysMemWrite((LPVOID)(image_base + 0x167DBC), bracket_r, sizeof(bracket_r));
	RxHook::SysMemWrite((LPVOID)(image_base + 0x167DC0), bracket_l, sizeof(bracket_l));

	// Skip Map Char and Read DBCS
	static BYTE asm_map_char[] =
	{
		0xC1, 0xEB, 0x08,                           // shr ebx, 0x8
		0x88, 0xC7,                                 // mov bh, al
		0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,   // Nop
		0x66, 0x89, 0x5D, 0x0C,                     // mov word ptr ss:[ebp+0xC], bx
		0x90,0x90,0x90                              // Nop
	};
	RxHook::SysMemWrite((LPVOID)(image_base + 0xC29A5), asm_map_char, sizeof(asm_map_char));
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		StartHook();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

VOID __declspec(dllexport) DirA() {}
