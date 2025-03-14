#include "pch.h"
#include "cvt_hex.h"
#include <stdio.h>


static int append_fmt(std::string& str, const char* fmt, ...) {
    char tmp[256] = { 0, };
    va_list vl;

    va_start(vl, fmt);
    int len = vsprintf_s(tmp, fmt, vl);
    va_end(vl);
    str += tmp;

    return len;
}


static int append_fmt(std::wstring& str, const wchar_t* fmt, ...) {
    wchar_t tmp[256] = { 0, };
    va_list vl;

    va_start(vl, fmt);
    int len = vswprintf_s(tmp, fmt, vl);
    va_end(vl);
    str += tmp;

    return len;
}


std::string convert_hex(const char* path, const char* name) {
    const int pad = 16;
    const char* tab = "  ";
    const char* newline = "\r\n";

    FILE* f = nullptr;
    fopen_s(&f, path, "rb");
    if (f == nullptr) {
        return std::string();
    }

    std::string str;

    append_fmt(str, "static const uint8_t PROGMEM %s[] = {%s", name, newline);

    uint8_t* buffer = new uint8_t[pad];

    while (!feof(f)) {
        int read = (int)fread(buffer, sizeof(uint8_t), pad, f);

        str += tab;
        for (int i = 0; i < read; i++) {
            append_fmt(str, "0x%02x, ", (unsigned int)buffer[i]);
        }

        str += newline;
    }

    delete[] buffer;
    fclose(f);

    str += "};";
    str += newline;

    return str;
}


std::wstring convert_hex(const wchar_t* path, const wchar_t* name) {
    const int pad = 16;
    const wchar_t* tab = L"  ";
    const wchar_t* newline = L"\r\n";

    FILE* f = nullptr;
    _wfopen_s(&f, path, L"rb");
    if (f == nullptr) {
        return std::wstring();
    }

    std::wstring str;

    append_fmt(str, L"static const uint8_t PROGMEM %s[] = {%s", name, newline);

    uint8_t* buffer = new uint8_t[pad];

    while (!feof(f)) {
        int read = (int)fread(buffer, sizeof(uint8_t), pad, f);

        str += tab;
        for (int i = 0; i < read; i++) {
            append_fmt(str, L"0x%02x, ", (unsigned int)buffer[i]);
        }

        str += newline;
    }

    delete[] buffer;
    fclose(f);

    str += L"};";
    str += newline;

    return str;
}
