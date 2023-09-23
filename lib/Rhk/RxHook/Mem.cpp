﻿#include "Mem.h"


namespace Rut::RxHook
{
	void SysMemRead(void* pAddress, void* pBuffer, size_t nSize)
	{
		SysMemAccess(pAddress, nSize, PAGE_EXECUTE_READWRITE);
		memcpy(pBuffer, pAddress, nSize);
	}

	void SysMemWrite(void* pAddress, void* pBuffer, size_t nSize)
	{
		SysMemAccess(pAddress, nSize, PAGE_EXECUTE_READWRITE);
		memcpy(pAddress, pBuffer, nSize);
	}

	void SysMemSet(void* pAddress, uint8_t ucValue, size_t nSize)
	{
		SysMemAccess(pAddress, nSize, PAGE_EXECUTE_READWRITE);
		memset(pAddress, ucValue, nSize);
	}

	void SysMemAccess(void* pAddress, size_t nSize, uint32_t uiAccess)
	{
		DWORD old = 0;
		bool is_set = VirtualProtect(pAddress, nSize, uiAccess, &old);
		if (is_set == false) { MessageBoxW(0, L"SetMemoryAccess Error!", 0, 0); ExitProcess(-1); }
	}

	DWORD MemSearch(DWORD pFind, SIZE_T szFind, PBYTE pToFind, SIZE_T szToFind, BOOL backward)
	{
		if ((pFind >= 0x7FFF0000) || (pFind <= 0x00010000) || !szToFind) return NULL;

		if (!backward)
		{
			for (size_t ite = 0; ite < szFind; ite++)
			{
				if (!memcmp(pToFind, (void*)pFind++, szToFind)) return (pFind - 1);
			}
		}
		else
		{
			for (size_t ite = 0; ite < szFind; ite++)
			{
				if (!memcmp(pToFind, (void*)pFind--, szToFind)) return (pFind + 1);
			}
		}

		MessageBoxW(NULL, L"MemSearch Failed!!", NULL, NULL);
		ExitProcess(0);
		return NULL;
	}

}