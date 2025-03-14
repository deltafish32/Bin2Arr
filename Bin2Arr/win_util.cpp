#include "pch.h"
#include "win_util.h"


BOOL CopyToClipboard(HWND hWnd, const wchar_t* str) {
    if (!OpenClipboard(hWnd)) {
        return FALSE;
    }

    BOOL result = FALSE;
    int buffer_len = (int)wcslen(str);

    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, (buffer_len + 1) * sizeof(wchar_t));
    if (hMem != NULL) {
        wchar_t* p = (wchar_t*)GlobalLock(hMem);
        if (p != nullptr) {
            wcscpy_s(p, buffer_len + 1, str);
            result = TRUE;
        }
        GlobalUnlock(hMem);
    }

    EmptyClipboard();
    SetClipboardData(CF_UNICODETEXT, hMem);

    CloseClipboard();

    return result;
}


BOOL CopyToClipboard(HWND hWnd, const char* str) {
    int buffer_len = MultiByteToWideChar(CP_ACP, 0, str, -1, nullptr, 0);
    wchar_t* buffer = new wchar_t[buffer_len + 1];
    MultiByteToWideChar(CP_ACP, 0, str, -1, buffer, buffer_len);

    BOOL result = CopyToClipboard(hWnd, buffer);

    delete[] buffer;

    return result;
}


std::wstring GetWindowTextAutoW(HWND hWnd) {
    int len = GetWindowTextLengthW(hWnd);
    std::wstring str(len + 1, 0);
    GetWindowTextW(hWnd, &str[0], len + 1);
    return str;
}


std::string GetWindowTextAutoA(HWND hWnd) {
    int len = GetWindowTextLengthA(hWnd);
    std::string str(len + 1, 0);
    GetWindowTextA(hWnd, &str[0], len + 1);
    return str;
}


std::wstring LoadStringAutoW(UINT uID, HINSTANCE hInstance)
{
    PWCHAR wsBuf;
    std::wstring str;
    size_t len = LoadStringW(hInstance, uID, (PWCHAR)&wsBuf, 0);

    if (len > 0) {
        str.assign(wsBuf, len);
    }

    return str;
}


std::string LoadStringAutoA(UINT uID, HINSTANCE hInstance)
{
    PWCHAR wsBuf;
    std::string str;
    size_t len = LoadStringW(hInstance, uID, (PWCHAR)&wsBuf, 0) * sizeof(WCHAR);

    if (len > 0) {
        str.resize(len);
        str.resize(LoadStringA(hInstance, uID, &str[0], (int)(len + 1)));
    }

    return str;
}
