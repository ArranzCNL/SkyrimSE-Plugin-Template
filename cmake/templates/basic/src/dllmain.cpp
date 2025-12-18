#if defined(_DEBUG)
	#include <Psapi.h>
	#include <iostream>
#endif

BOOL APIENTRY DllMain(HMODULE a_hinstDLL, DWORD a_fdwReason, LPVOID)
{
	switch (a_fdwReason) {

		case DLL_PROCESS_ATTACH: {
			DisableThreadLibraryCalls(a_hinstDLL);
#if defined(_DEBUG)
			// Get dll name
			wchar_t dllName[MAX_PATH]{};
			GetModuleBaseNameW(GetCurrentProcess(), a_hinstDLL, dllName, MAX_PATH);
			// Build title and message
			std::wstring title = std::format(L"{}: Attach Debugger", dllName);
			std::wstring msg = std::format(L"Click OK when you have attached your debugger or to carry on without it.");
			// Wait for a debugger to attach. Note: Makes it easier for quickly testing stuff without needing the debugger present.
			if (!IsDebuggerPresent()) {
				MessageBoxW(nullptr, msg.c_str(), title.c_str(), MB_ICONINFORMATION | MB_OK);
			}
			// Create debug console
			AllocConsole();
			FILE* pConsole = nullptr;
			freopen_s(&pConsole, "CONOUT$", "w", stdout);
			freopen_s(&pConsole, "CONIN$", "w", stdin);
			freopen_s(&pConsole, "CONOUT$", "w", stderr);
			if (pConsole) {
				fclose(pConsole);
			}
			std::cout << "Debug Console Started:" << std::endl;
#endif
			break;
		}
		case DLL_PROCESS_DETACH: {
			break;
		}
	}
	return TRUE;
}
