#include "MinoriFileHook.h"
#include "MinoriFileHook_DEF.h"
#include "../../lib/RxHook/RxHook.h"

#include <Windows.h>

using namespace Rut;


namespace Minori::FileHook
{
	static uint32_t* sg_asPngDecode = (uint32_t*)0x00418050;
	static uint32_t* sg_asScriptXor = (uint32_t*)0x00527E28;

	typedef char(__thiscall* pMinoriLoadRes)(Pack_Read* pPackRead, uint8_t* pResName);
	static pMinoriLoadRes sg_fn_MinoriLoadRes = (pMinoriLoadRes)0x00522AC0;

	typedef void* (__thiscall* pMinoriMemAlloc)(uint32_t nSize, MemStream* pMemStream, uint32_t nType);
	static pMinoriMemAlloc sg_fn_MinoriMemAlloc = (pMinoriMemAlloc)0x004398B0;

	static const char* sg_cpFileHookFolder = "eden_pm_cn\\";


	static const char* MakePath(const char* cpBase, const char* cpPack, const char* cpRes)
	{
		static char path_buffer[260] = { 0 };
		strcpy_s(path_buffer, 260, cpBase);
		strcat_s(path_buffer, 260, cpPack);
		strcat_s(path_buffer, 260, "\\");
		strcat_s(path_buffer, 260, cpRes);
		return path_buffer;
	}

	static HANDLE OpenResFile(const char* cpFile)
	{
		if (GetFileAttributesA(cpFile) == INVALID_FILE_ATTRIBUTES) { return INVALID_HANDLE_VALUE; }
		return CreateFileA(cpFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	}

	static char __fastcall MinoriLoadRes_New(Pack_Read* pPackRead, uint32_t uiEDX, uint8_t* pResName)
	{
		char read_state = sg_fn_MinoriLoadRes(pPackRead, pResName);

		if (read_state)
		{
			*sg_asPngDecode = 0x57EC8B55l;

			HANDLE hfile = OpenResFile(MakePath(sg_cpFileHookFolder, (char*)(pPackRead->msPackName.aStr), (char*)pResName));
			if (hfile == INVALID_HANDLE_VALUE) { return read_state; }

			DWORD read_bytes = GetFileSize(hfile, NULL);
			MemStream* p_mem_stream = pPackRead->pMemStream;
			sg_fn_MinoriMemAlloc(read_bytes, p_mem_stream, p_mem_stream->uiType);
			(VOID)ReadFile(hfile, (LPVOID)p_mem_stream->uiBuffer, p_mem_stream->uiSize, &read_bytes, NULL);
			CloseHandle(hfile);

			pPackRead->uiResBuffer = p_mem_stream->uiBuffer;
			pPackRead->uiResSize_0 = p_mem_stream->uiSize;
			pPackRead->uiResSize_1 = p_mem_stream->uiSize;
			pPackRead->uiResSize_2 = p_mem_stream->uiSize;

			*sg_asPngDecode = 0x570008C2;
		}

		return read_state;
	}

	void SetFileHook()
	{
		//SetConsole(L"Eden Hook 2023年5月20日14点35分");
		DWORD old = 0;
		VirtualProtectEx(GetCurrentProcess(), sg_asPngDecode, 4, PAGE_EXECUTE_READWRITE, &old);
		VirtualProtectEx(GetCurrentProcess(), sg_asScriptXor, 4, PAGE_EXECUTE_READWRITE, &old);
		RxHook::DetourAttachFunc(&sg_fn_MinoriLoadRes, MinoriLoadRes_New);
	}
}