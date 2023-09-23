#include "../../lib/Rhk/RxHook.h"

using namespace Rut;


VOID __declspec(naked) ReadChar_Hook()
{
	_asm
	{
		movzx ecx, byte ptr[eax];
		cmp ecx, 0x81;
		jb not_DBCS;

		//is_DBCS:
		movzx ecx, word ptr[eax];
		inc eax;

	not_DBCS:
		mov dword ptr[ebp - 0x10], ecx;
		ret;
	}
}

VOID StartHook()
{
	DWORD image_base = (DWORD)GetModuleHandleW(NULL);

	RxHook::HookCreateFontA(0x80, "MS Mincho");

	RxHook::SetHookCode_Call(image_base + 0x11A657, (DWORD)ReadChar_Hook, 0x6);

	static BYTE aCharacterName0[] = { 0x20,0x81,0x7A };
	static BYTE aCharacterName1[] = { 0x81,0x79,0x20 };
	RxHook::SysMemWrite((LPVOID)(image_base + 0x175ECC), aCharacterName0, sizeof(aCharacterName0));
	RxHook::SysMemWrite((LPVOID)(image_base + 0x175ED0), aCharacterName1, sizeof(aCharacterName1));

	static BYTE aProcDBCS_1_ASM[] =
	{
		0x66, 0x89, 0x5D, 0xF0,        // mov word ptr ss:[ebp-0x10], bx
		0x90, 0x90, 0x90               // NOP
	};
	RxHook::SysMemWrite((LPVOID)(image_base + 0xCB0BC), aProcDBCS_1_ASM, sizeof(aProcDBCS_1_ASM));

	static BYTE aJmpCharMap_0[] =
	{
		0xE9, 0x8E, 0x00, 0x00, 0x00,  // JMP
		0x90                           // NOP
	};
	RxHook::SysMemWrite((LPVOID)(image_base + 0xCB0EB), aJmpCharMap_0, sizeof(aJmpCharMap_0));

	static BYTE aProcDBCS_2_ASM[] =
	{
		0xC1, 0xE0, 0x08,             // shl eax, 0x8
		0xC1, 0xEB, 0x08,             // shr ebx, 0x8
		0x09, 0xC3,                   // or ebx, eax
		0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90
	};
	RxHook::SysMemWrite((LPVOID)(image_base + 0xCB17E), aProcDBCS_2_ASM, sizeof(aProcDBCS_2_ASM));


	static BYTE aJmpCharMap_1[] = { 0xEB };
	RxHook::SysMemWrite((LPVOID)(image_base + 0x1259C7), aJmpCharMap_1, sizeof(aJmpCharMap_1));

	static BYTE aProcDBCS_3_ASM[] =
	{
		0x6A, 0x02,
		0x66, 0x89, 0x45, 0xF8,
		0x8D, 0x45, 0xF4,
		0x50,
		0x6A, 0x02,
		0x8D, 0x4D, 0xF8,
		0x51,
		0x6A, 0x00,
		0x68, 0xA4, 0x03, 0x00, 0x00,
		0x89, 0x55, 0xF4,
		0xFF, 0x15, 0x2C, 0x62, 0x55, 0x00,
		0x8B, 0x55, 0xF4,
		0x89, 0x55, 0xFC,
		0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90
	};
	RxHook::SysMemWrite((LPVOID)(image_base + 0x125A0F), aProcDBCS_3_ASM, sizeof(aProcDBCS_3_ASM));
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