#include "pch.h"
#include "resource.h"
#include <commctrl.h>
#include "cvt_hex.h"
#include "win_util.h"



#define IDM_ABOUTBOX    0x0010

INT_PTR CALLBACK AboutProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);


static const uint64_t MAX_FILE_SIZE = 1048576;


INT_PTR CALLBACK MainProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HWND txtPath = NULL;
    static HWND txtOutput = NULL;
    static HWND copied = NULL;
    static HWND copied2 = NULL;
    static HFONT font = NULL;

    switch (message) {
    case WM_INITDIALOG:
        txtPath = GetDlgItem(hDlg, IDC_TXT_FILENAME);
        txtOutput = GetDlgItem(hDlg, IDC_TXT_OUTPUT);
        copied = GetDlgItem(hDlg, IDC_COPIED);
        copied2 = GetDlgItem(hDlg, IDC_COPIED2);

        if (1) {
            HDC hdc = GetDC(hDlg);
            LOGFONT	lf = { 0, };
            lf.lfCharSet = DEFAULT_CHARSET;
            lf.lfHeight = -MulDiv(9, GetDeviceCaps(hdc, LOGPIXELSY), 72);
            lf.lfWeight = FW_NORMAL;
            lf.lfPitchAndFamily = FIXED_PITCH;
            font = CreateFontIndirect(&lf);
            ReleaseDC(hDlg, hdc);

            SendMessage(txtOutput, WM_SETFONT, (WPARAM)font, TRUE);

            HMENU hMenu = GetSystemMenu(hDlg, FALSE);
            if (hMenu != NULL) {
                AppendMenu(hMenu, MF_SEPARATOR, 0, 0);
                AppendMenu(hMenu, MF_STRING, IDM_ABOUTBOX, LoadStringAuto(IDS_ABOUTBOX).c_str());
            }
        }

        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (1) {
            int id = LOWORD(wParam);

            switch (id) {
            case IDOK:
            case IDCANCEL:
                EndDialog(hDlg, id);
                return (INT_PTR)TRUE;

            case IDC_BTN_FILENAME:
                if (1) {
                    TCHAR filename[MAX_PATH] = { 0, };
                    OPENFILENAME ofn = { 0, };
                    ofn.lStructSize = sizeof(OPENFILENAME);
                    ofn.hwndOwner = NULL;
                    ofn.lpstrFile = filename;
                    ofn.nMaxFile = MAX_PATH;
                    ofn.lpstrFilter = _T("All files(*.*)\0*.*\0");
                    ofn.nFilterIndex = 0;
                    ofn.lpstrFileTitle = NULL;
                    ofn.nMaxFileTitle = 0;
                    ofn.lpstrInitialDir = NULL;
                    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_READONLY;
                    if (GetOpenFileName(&ofn)) {
                        SetWindowText(txtPath, ofn.lpstrFile);
                    }
                }
                return (INT_PTR)TRUE;
            case IDC_BTN_CONV:
                if (1) {
                    STD_TSTR filename = GetWindowTextAuto(txtPath);
                    HANDLE hFile = CreateFile(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
                    if (hFile != INVALID_HANDLE_VALUE) {
                        uint64_t size;
                        GetFileSizeEx(hFile, (PLARGE_INTEGER)&size);
                        CloseHandle(hFile);

                        if (size >= MAX_FILE_SIZE) {
                            if (IDYES != MessageBox(hDlg, LoadStringAuto(IDS_FILE_TOO_LARGE).c_str(), _T(""), MB_ICONEXCLAMATION | MB_YESNO | MB_DEFBUTTON2)) {
                                return (INT_PTR)TRUE;
                            }
                        }
                    }
                    auto conv = convert_hex(filename.c_str(), _T("DATA"));
                    SetWindowText(txtOutput, conv.c_str());
                }
                return (INT_PTR)TRUE;
            case IDC_BTN_COPY:
                if (1) {
                    STD_TSTR str = GetWindowTextAuto(txtOutput);
                    CopyToClipboard(hDlg, str.c_str());
                    ShowWindow(str[0] != 0 ? copied : copied2, SW_SHOW);
                    SetTimer(hDlg, 1, 3000, nullptr);
                }
                return (INT_PTR)TRUE;
            }
        }
        break;
    case WM_SYSCOMMAND:
        if ((wParam & 0xFFF0) == IDM_ABOUTBOX) {
            DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ABOUTBOX), hDlg, AboutProc);
            return (INT_PTR)TRUE;
        }
        break;

    case WM_TIMER:
        if (wParam == 1) {
            KillTimer(hDlg, wParam);
            ShowWindow(copied, SW_HIDE);
            ShowWindow(copied2, SW_HIDE);
            return (INT_PTR)TRUE;
        }
        break;
    }

    return (INT_PTR)FALSE;
}


INT_PTR CALLBACK AboutProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (1) {
            int id = LOWORD(wParam);

            switch (id) {
            case IDOK:
            case IDCANCEL:
                EndDialog(hDlg, id);
                return (INT_PTR)TRUE;
            }
        }
        break;

    case WM_NOTIFY:
        if (1) {
            LPNMHDR pnmh = (LPNMHDR)lParam;
            if (pnmh->idFrom == IDC_SYSLINK) {
                if (pnmh->code == NM_CLICK || pnmh->code == NM_RETURN) {
                    PNMLINK pNMLink = (PNMLINK)lParam;
                    ShellExecuteW(nullptr, L"open", pNMLink->item.szUrl, nullptr, nullptr, SW_SHOWNORMAL);
                    return (INT_PTR)TRUE;
                }
            }
        }
        break;
    }

    return (INT_PTR)FALSE;
}


int APIENTRY _tWinMain(HINSTANCE hInst, HINSTANCE, LPTSTR, int nCmdShow)
{
    DialogBox(hInst, MAKEINTRESOURCE(IDD_MAIN), NULL, MainProc);

    return 0;
}
