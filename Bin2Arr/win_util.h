#pragma once
#include <Windows.h>
#include <string>


BOOL CopyToClipboard(HWND hWnd, const wchar_t* str);
BOOL CopyToClipboard(HWND hWnd, const char* str);

std::wstring GetWindowTextAutoW(HWND hWnd);
std::string GetWindowTextAutoA(HWND hWnd);

std::wstring LoadStringAutoW(UINT uID, HINSTANCE hInstance = GetModuleHandle(NULL));
std::string LoadStringAutoA(UINT uID, HINSTANCE hInstance = GetModuleHandle(NULL));


#ifdef UNICODE
#define STD_TSTR std::wstring
#define LoadStringAuto LoadStringAutoW
#define GetWindowTextAuto GetWindowTextAutoW
#else
#define STD_TSTR std::string
#define LoadStringAuto LoadStringAutoA
#define GetWindowTextAuto GetWindowTextAutoA
#endif
