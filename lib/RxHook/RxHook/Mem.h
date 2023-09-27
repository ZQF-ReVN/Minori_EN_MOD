#pragma once
#include <cstdint>
#include <Windows.h>



namespace Rut::RxHook
{
	void SysMemRead(void* pAddress, void* pBuffer, size_t nSize);
	void SysMemWrite(void* pAddress, void* pBuffer, size_t nSize);
	void SysMemFill(void* pAddress, uint8_t ucValue, size_t nSize);
	void SysMemAccess(void* pAddress, size_t nSize, uint32_t uiAccess);

	DWORD MemSearch(DWORD pFind, SIZE_T szFind, PBYTE pToFind, SIZE_T szToFind, BOOL backward = FALSE);
}


