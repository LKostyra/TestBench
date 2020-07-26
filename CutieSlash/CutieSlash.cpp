struct IUnknown;
#include "framework.h"
#include "CutieSlash.h"

#include <shellapi.h>
#include <Shlwapi.h>
#include <iostream>
#include <vector>

#define MAX_LOADSTRING 100
#define NOTIFICATION_TRAY_ICON_MSG (WM_USER + 0x100)
#define MAX_STRING 1024

// Global Variables:
HINSTANCE hInst;                                // current instance
const WCHAR* szWindowClass = L"CutieSlash";     // the main window class name
const HKEY hAutostartRegKey = HKEY_CURRENT_USER;
const WCHAR* szAutostartRegPath = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run"; // path to autostart reg key
const WCHAR* szAutostartRegValue = L"CutieSlash";
const WCHAR* szErrorMessageTitle = L"Cutie slash fail :(";
WCHAR sExePath[MAX_STRING];
DWORD sExePathSize = 0;
HHOOK hKeyboardHook;
HMENU hPopupMenu;
NOTIFYICONDATA niData;

LRESULT CALLBACK KeyboardEvent(int nCode, WPARAM wParam, LPARAM lParam)
{
    bool eaten = false;

    if  (nCode == HC_ACTION)
    {
        KBDLLHOOKSTRUCT* keyHooked = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
        int key = keyHooked->vkCode;

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

void ShowContextMenu(HWND hWnd)
{
    POINT curPoint;
    GetCursorPos(&curPoint);
    SetForegroundWindow(hWnd);

    // TrackPopupMenu blocks the app until TrackPopupMenu returns
    UINT clicked = TrackPopupMenu(hPopupMenu, 0, curPoint.x, curPoint.y, 0, hWnd, NULL);
}

bool IsAutostartRegistered(bool& registered)
{
    HKEY key;
    LSTATUS status = RegOpenKeyEx(hAutostartRegKey, szAutostartRegPath, 0, KEY_QUERY_VALUE | KEY_READ | KEY_WRITE, &key);
    if (status != ERROR_SUCCESS)
    {
        if (status == ERROR_FILE_NOT_FOUND)
        {
            std::cout << "Autostart entry does not exist" << std::endl;
            registered = false;
            return true;
        }
        else
        {
            MessageBox(0, L"Got some poopy error when opening autostart registry key", szErrorMessageTitle, 0);
            return false;
        }
    }

    DWORD dataSize;
    DWORD dataType;
    status = RegQueryValueEx(key, szAutostartRegValue, 0, &dataType, NULL, &dataSize);
    if (status != ERROR_SUCCESS)
    {
        MessageBox(0, L"Got some poopy error when getting value of autostart", szErrorMessageTitle, 0);
        RegCloseKey(key);
        return false;
    }

    if (dataType != REG_SZ)
    {
        MessageBox(0, L"That registry cutie key is not a string! Someone pooped us!", szErrorMessageTitle, 0);
        RegCloseKey(key);
        return false;
    }

    std::vector<WCHAR> data(dataSize / sizeof(WCHAR)); // dataSize is in bytes
    status = RegQueryValueEx(key, szAutostartRegValue, 0, &dataType, reinterpret_cast<BYTE*>(data.data()), &dataSize);
    if (status != ERROR_SUCCESS)
    {
        MessageBox(0, L"Failed to read registry data (oop)", szErrorMessageTitle, 0);
        RegCloseKey(key);
        return false;
    }

    bool equal = true;
    if (data.size() != sExePathSize)
        equal = false;

    if (equal)
    {
        for (uint32_t i = 0; i < data.size(); ++i)
        {
            if (data[i] != sExePath[i])
            {
                equal = false;
                break;
            }
        }
    }

    if (!equal)
    {
        // somethin wrong - overwrite with actual new path to autostart so we are up to date yay
        status = RegSetValueEx(key, szAutostartRegValue, 0, REG_SZ, reinterpret_cast<BYTE*>(sExePath), (sExePathSize) * sizeof(WCHAR));
        if (status != ERROR_SUCCESS)
        {
            MessageBox(0, L"Failed to set autostart registry value (help me)", szErrorMessageTitle, 0);
            RegCloseKey(key);
            return false;
        }
    }

    RegCloseKey(key);
    registered = true;
    return true;
}

bool RegisterAutostart()
{
    HKEY key = NULL;
    LSTATUS status = RegCreateKeyEx(hAutostartRegKey, szAutostartRegPath, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &key, NULL);
    if (status != ERROR_SUCCESS)
    {
        MessageBox(0, L"Oop, I failed to create a registry entry ;;", szErrorMessageTitle, 0);
        return false;
    }

    status = RegSetValueEx(key, szAutostartRegValue, 0, REG_SZ, reinterpret_cast<BYTE*>(sExePath), (sExePathSize + 1) * sizeof(WCHAR));
    if (status != ERROR_SUCCESS)
    {
        MessageBox(0, L"Failed to set autostart registry value (help me)", szErrorMessageTitle, 0);
        RegCloseKey(key);
        return false;
    }

    RegCloseKey(key);
    return true;
}

bool UnregisterAutostart()
{
    HKEY key = NULL;
    LSTATUS status = RegCreateKeyEx(hAutostartRegKey, szAutostartRegPath, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &key, NULL);
    if (status != ERROR_SUCCESS)
    {
        MessageBox(0, L"Oop, I failed to open a registry entry ;;", szErrorMessageTitle, 0);
        return false;
    }

    status = RegDeleteValue(key, szAutostartRegValue);
    if (status != ERROR_SUCCESS)
    {
        MessageBox(0, L"Cannot delete this poopy autostart key", szErrorMessageTitle, 0);
        return false;
    }

    RegCloseKey(key);
    return true;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case NIN_BALLOONHIDE: break;
    case NIN_BALLOONSHOW: break;
    case NIN_BALLOONTIMEOUT: break;
    case NIN_BALLOONUSERCLICK: break;
    case NIN_POPUPCLOSE: break;
    case NIN_POPUPOPEN: break;
    case NOTIFICATION_TRAY_ICON_MSG:
    {
        switch (lParam)
        {
        case WM_RBUTTONDOWN:
        case WM_CONTEXTMENU:
        {
            ShowContextMenu(hWnd);
        }
        break;
        }
    }
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_AUTOSTART:
        {
            MENUITEMINFO info;
            ZeroMemory(&info, sizeof(info));
            info.cbSize = sizeof(MENUITEMINFO);
            info.fMask = MIIM_STATE;
            GetMenuItemInfo(hPopupMenu, wmId, FALSE, &info);
            if ((info.fState & MFS_CHECKED) == MFS_CHECKED)
                UnregisterAutostart();
            else
                RegisterAutostart();

            info.fState ^= MFS_CHECKED;
            SetMenuItemInfo(hPopupMenu, wmId, FALSE, &info);
            break;
        }
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    }
    case WM_ENDSESSION:
    case WM_DESTROY:
        Shell_NotifyIcon(NIM_DELETE, &niData);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

ATOM Register(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;
    ZeroMemory(&wcex, sizeof(WNDCLASSEXW));

    wcex.cbSize         = sizeof(WNDCLASSEX);
    wcex.style          = 0;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszClassName  = szWindowClass;

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

    HWND hWnd = CreateWindowW(szWindowClass, L"Cutie Slash", WS_OVERLAPPEDWINDOW,
                              CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr,
                              nullptr, hInstance, nullptr);

    if (!hWnd)
        return FALSE;

    bool autostartRegistered = false;
    if (!IsAutostartRegistered(autostartRegistered))
        return FALSE;

    hPopupMenu = CreatePopupMenu();
    AppendMenu(hPopupMenu, MF_STRING | MF_GRAYED, NULL, L"Hi cutie ily <3");
    AppendMenu(hPopupMenu, MF_SEPARATOR, NULL, NULL);
    AppendMenu(hPopupMenu, MF_STRING | (autostartRegistered ? MF_CHECKED : 0), IDM_AUTOSTART, L"Autostart");
    AppendMenu(hPopupMenu, MF_SEPARATOR, NULL, NULL);
    AppendMenu(hPopupMenu, MF_STRING, IDM_EXIT, L"Exit");

    ZeroMemory(&niData, sizeof(NOTIFYICONDATA));
    niData.cbSize = sizeof(NOTIFYICONDATAW);
    niData.uID = TRAYICON_ID;
    niData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    niData.hIcon = (HICON)LoadImage(hInstance, L"res/CutieSlash.ico", IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_LOADFROMFILE);
    if (niData.hIcon == NULL)
    {
        MessageBox(0, L"Couldn't find cute icon for cute program", szErrorMessageTitle, 0);
        return FALSE;
    }
    niData.hWnd = hWnd;
    niData.dwInfoFlags = NIIF_INFO;
    niData.uVersion = NOTIFYICON_VERSION_4;
    wcsncpy_s(niData.szTip, 128, L"Cutie slash replacer hihi <3", 28);
    niData.uCallbackMessage = NOTIFICATION_TRAY_ICON_MSG;
    Shell_NotifyIcon(NIM_ADD, &niData);

    #ifdef _DEBUG
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    #endif // _DEBUG

    return TRUE;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR    lpCmdLine,
                      _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    HMODULE exe = GetModuleHandle(0);
    sExePath[0] = '"';
    sExePathSize = GetModuleFileName(exe, sExePath + 1, 1024);
    sExePath[sExePathSize + 1] = '"';
    sExePathSize += 3;

    // Initialize global strings
    Register(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    MSG msg;

    hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardEvent, hInstance, 0);

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hKeyboardHook);

    UnregisterClass(szWindowClass, hInstance);

    return (int)msg.wParam;
}
