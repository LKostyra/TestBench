struct IUnknown;

#include <Windows.h>
#include <iostream>


HHOOK hKeyboardHook;

LRESULT CALLBACK KeyboardEvent(int nCode, WPARAM wParam, LPARAM lParam)
{
    DWORD keyCtrl = 0;
    DWORD keyAlt = 0;
    bool eaten = false;

    if  (nCode == HC_ACTION)
    {
        KBDLLHOOKSTRUCT* keyHooked = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
        int key = keyHooked->vkCode;

        keyCtrl = GetAsyncKeyState(VK_CONTROL);
        keyAlt = GetAsyncKeyState(VK_MENU);

        if (keyCtrl != 0 && keyAlt != 0 && key == 'Q' )
        {
            std::cout << "I exit, bye cutie ily" << std::endl;
            PostQuitMessage(0);
            return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
        }

        #ifdef _DEBUG
        if (wParam == WM_KEYDOWN)
        {
            if ((key >= 'A' && key <= 'Z') || (key == ' '))
                std::cout << "Keycode = " << static_cast<char>(key) << " (" << key << ")" << std::endl;
            else
                std::cout << "Keycode = " << key << std::endl;
        }
        #endif // _DEBUG

        switch (wParam)
        {
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            if (key == 220)
            {
                #ifdef _DEBUG
                std::cout << "WE GOT BACKSLASH DOWN, changing to " << VK_BACK << std::endl;
                #endif // _DEBUG
                keybd_event(8, 0, 0, 0);
                eaten = true;
            }
            break;

        case WM_KEYUP:
        case WM_SYSKEYUP:
            if (key == 220)
            {
                #ifdef _DEBUG
                std::cout << "WE GOT BACKSLASH UP, changing to " << VK_BACK << std::endl;
                #endif // _DEBUG
                keybd_event(8, 0, KEYEVENTF_KEYUP, 0);
                eaten = true;
            }
            break;
        }
    }

    return (eaten ? 1 : CallNextHookEx(hKeyboardHook, nCode, wParam, lParam));
}

void MessageLoop()
{
    MSG message;
    while (GetMessage(&message, nullptr, 0, 0))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}

int main()
{
    std::cout << "Keyboard hook to replace cutie's slash" << std::endl;
    std::cout << "Press Ctrl+Alt+Q to shut me down cutie" << std::endl;

    HINSTANCE instance = GetModuleHandle(0);

    hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardEvent, instance, 0);
    MessageLoop();
    UnhookWindowsHookEx(hKeyboardHook);

    Sleep(1000);
    return 0;
}
