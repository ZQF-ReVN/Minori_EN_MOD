#pragma once
#include <Windows.h>
#include <cstdint>


namespace TDA
{
	namespace HookX
	{
		bool HookCreateFontA(const uint32_t uiCharSet, const char* cpFontName);
		bool HookCreateFontIndirectA(const uint32_t uiCharSet, const char* cpFontName);
		bool HookTitleExA(const char* cpRawTitle, const char* cpPatchTitle);
	}
}