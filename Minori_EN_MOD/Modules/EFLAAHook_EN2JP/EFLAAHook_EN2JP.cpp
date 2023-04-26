#include "../../ThirdParty/TDA/HookX.h"
#include "../../ThirdParty/TDA/MemX.h"

using namespace TDA::MemX;
using namespace TDA::HookX;

static DWORD g_dwExeBase = (DWORD)GetModuleHandleW(NULL);


static DWORD g_dwReadCharRaw = NULL, g_dwReadCharRet = NULL;
VOID __declspec(naked) ProcDBCS_0()
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
		jmp g_dwReadCharRet;
	}
}

VOID StartHook()
{
	HookCreateFontA(0x80, "MS Mincho");

	g_dwReadCharRaw = g_dwExeBase + 0x11A657;
	g_dwReadCharRet = g_dwReadCharRaw + 0x6;
	WriteHookCode(g_dwReadCharRaw, (DWORD)ProcDBCS_0, 0x6);

	static BYTE aCharacterName0[] = { 0x20,0x81,0x7A };
	static BYTE aCharacterName1[] = { 0x81,0x79,0x20 };
	WriteMemory((LPVOID)(g_dwExeBase + 0x175ECC), aCharacterName0, sizeof(aCharacterName0));
	WriteMemory((LPVOID)(g_dwExeBase + 0x175ED0), aCharacterName1, sizeof(aCharacterName1));

	//UCHAR** pTable = (UCHAR**)(g_dwExeBase + 0x198720);
	//pTable[0] = pTable[1];
	//pTable[2] = pTable[3];
	//pTable[4] = pTable[5];
	//pTable[6] = pTable[7];
	//pTable[8] = pTable[9];
	//pTable[10] = pTable[11];
	//pTable[12] = pTable[13];
	//pTable[14] = pTable[15];
	//pTable[16] = pTable[17];

	static BYTE aProcDBCS_1_ASM[] =
	{
		0x66, 0x89, 0x5D, 0xF0,        // mov word ptr ss:[ebp-0x10], bx
		0x90, 0x90, 0x90               // NOP
	};
	WriteMemory((LPVOID)(g_dwExeBase + 0xCB0BC), aProcDBCS_1_ASM, sizeof(aProcDBCS_1_ASM));

	static BYTE aJmpCharMap_0[] =
	{
		0xE9, 0x8E, 0x00, 0x00, 0x00,  // JMP
		0x90                           // NOP
	};
	WriteMemory((LPVOID)(g_dwExeBase + 0xCB0EB), aJmpCharMap_0, sizeof(aJmpCharMap_0));

	static BYTE aProcDBCS_2_ASM[] =
	{
		0xC1, 0xE0, 0x08,             // shl eax, 0x8
		0xC1, 0xEB, 0x08,             // shr ebx, 0x8
		0x09, 0xC3,                   // or ebx, eax
		0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90
	};
	WriteMemory((LPVOID)(g_dwExeBase + 0xCB17E), aProcDBCS_2_ASM, sizeof(aProcDBCS_2_ASM));


	static BYTE aJmpCharMap_1[] = { 0xEB };
	WriteMemory((LPVOID)(g_dwExeBase + 0x1259C7), aJmpCharMap_1, sizeof(aJmpCharMap_1));

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
	WriteMemory((LPVOID)(g_dwExeBase + 0x125A0F), aProcDBCS_3_ASM, sizeof(aProcDBCS_3_ASM));
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