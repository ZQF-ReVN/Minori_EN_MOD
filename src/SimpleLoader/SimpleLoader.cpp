#include "../../lib/RxHook/include/Hook.h"


INT APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	STARTUPINFOW si = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	si.cb = sizeof(si);

	LPCWSTR applicationName = L"eden_en.exe";
	LPCSTR dllName[1] = { "eden_cn.dll" };

	if (Rut::RxHook::CreateProcessWithDlls(applicationName, CREATE_SUSPENDED, 1, dllName, &si, &pi))
	{
		if (pi.hThread && pi.hProcess)
		{
			ResumeThread(pi.hThread);
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
		}
	}
	else
	{
		MessageBoxW(NULL, L"DetourCreateProcessWithDll Failed!", NULL, MB_OK);
	}
}