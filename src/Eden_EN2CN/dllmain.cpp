#include "../../lib/RxHook/include/API.h"
#include "../../lib/RxHook/include/Mem.h"
#include "../../lib/RxHook/include/Hook.h"

using namespace Rut;


VOID __declspec(naked) ReadChar_Hook()
{
	_asm
	{
		movzx ecx, byte ptr[eax];
		cmp ecx, 0x81;
		jb not_DBCS;

		//is_DBCS:
		xor ecx, ecx;
		mov ch, byte ptr[eax];
		inc eax;
		mov cl, byte ptr[eax];

	not_DBCS:
		mov dword ptr[ebp - 0x10], ecx;
		ret;
	}
}

VOID StartHook()
{
	DWORD image_base = (DWORD)GetModuleHandleW(NULL);

	// Hook Font
	RxHook::HookCreateFontA(0x86, "黑体");

	// Hook ReadChar
	RxHook::SetHookCode_Call(image_base + 0x10E363, (DWORD)ReadChar_Hook, 0x6);

	// Replace Character Name Brackets
	static BYTE aCharacterName0[] = { 0x20,0xA1,0xBF };
	static BYTE aCharacterName1[] = { 0xA1,0xBE,0x20 };
	RxHook::SysMemWrite((LPVOID)(image_base + 0x167DBC), aCharacterName0, sizeof(aCharacterName0));
	RxHook::SysMemWrite((LPVOID)(image_base + 0x167DC0), aCharacterName1, sizeof(aCharacterName1));

	// Change SJIS Range to GBK
	static BYTE aSJISRange[] = { 0xFE };
	RxHook::SysMemWrite((LPVOID)(image_base + 0xC25BF), aSJISRange, sizeof(aSJISRange));
	RxHook::SysMemWrite((LPVOID)(image_base + 0xD20DE), aSJISRange, sizeof(aSJISRange));
	RxHook::SysMemWrite((LPVOID)(image_base + 0x122395), aSJISRange, sizeof(aSJISRange));
	RxHook::SysMemWrite((LPVOID)(image_base + 0x10E18D), aSJISRange, sizeof(aSJISRange));

	// Skip Map Char and Read DBCS
	static BYTE aProcDBCS_1_ASM[] =
	{
		0xC1, 0xEB, 0x08,                           // shr ebx, 0x8
		0x88, 0xC7,                                 // mov bh, al
		0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,   // Nop
		0x66, 0x89, 0x5D, 0x0C,                     // mov word ptr ss:[ebp+0xC], bx
		0x90,0x90,0x90                              // Nop
	};
	RxHook::SysMemWrite((LPVOID)(image_base + 0xC25E5), aProcDBCS_1_ASM, sizeof(aProcDBCS_1_ASM));

	// Replace Character Name String
	//00  "Elica"	01  "エリカ"	 -> 艾丽卡	//02  "General" 03  "本部長"		-> 总部部长
	//04  "Inaba"	05  "稲葉"	 -> 稻叶		//06  "Lavie"   07  "ラヴィ"		-> 拉维
	//08  "Maya"	09  "真夜"	 -> 真夜		//10  "Natsume" 11  "ナツメ"		-> 夏目
	//12  "Ryou"	13  "亮"		 -> 亮		//14  "Sion"    15  "シオン"		-> 诗音
	//16  "Soldier"	17  "兵士" 	 -> 士兵	
	static UCHAR** pTable = (UCHAR**)(image_base + 0x197720);

	static UCHAR aElica[] = { 0xB0, 0xAC, 0xC0, 0xF6, 0xBF, 0xA8, 0x00 };
	static UCHAR aGeneral[] = { 0xD7, 0xDC, 0xB2, 0xBF, 0xB2, 0xBF, 0xB3, 0xA4, 0x00 };
	static UCHAR aInaba[] = { 0xB5, 0xBE, 0xD2, 0xB6, 0x0 };
	static UCHAR aLavie[] = { 0xC0, 0xAD, 0xCE, 0xAC,0x00 };
	static UCHAR aMaya[] = { 0xD5, 0xE6, 0xD2, 0xB9, 0x00 };
	static UCHAR aNatsume[] = { 0xCF, 0xC4, 0xC4, 0xBF, 0x00 };
	static UCHAR aRyou[] = { 0xC1, 0xC1, 0x00 };
	static UCHAR aSion[] = { 0xCA, 0xAB, 0xD2, 0xF4, 0x00 };
	static UCHAR aSoldier[] = { 0xCA, 0xBF, 0xB1, 0xF8, 0x00 };

	pTable[0] = aElica;
	pTable[2] = aGeneral;
	pTable[4] = aInaba;
	pTable[6] = aLavie;
	pTable[8] = aMaya;
	pTable[10] = aNatsume;
	pTable[12] = aRyou;
	pTable[14] = aSion;
	pTable[16] = aSoldier;
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
