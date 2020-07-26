#include "PCH.hpp"

#include "framework.h"
#include "CutieSlash.h"
#include "Log.hpp"


// Global defines
#define MAX_LOADSTRING 100
#define NOTIFICATION_TRAY_ICON_MSG (WM_USER + 0x100)
#define MAX_STRING 1024


// Global constants
const WCHAR* WINDOW_CLASS = L"CutieSlash";
const HKEY AUTOSTART_REG_KEY = HKEY_CURRENT_USER;
const WCHAR* AUTOSTART_REG_KEY_PATH = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";
const WCHAR* AUTOSTART_REG_VALUE = L"CutieSlash";
const WCHAR* ERROR_MESSAGE_TITLE = L"Cutie slash fail :(";
const WCHAR* GLOBAL_MUTEX_NAME = L"Global\\CutieSlash_GlobalMutex_Omigosh";


// Global variables
HANDLE gGlobalMutex;
HINSTANCE gInstance;
WCHAR gExePath[MAX_STRING];
DWORD gExePathSize = 0;
HHOOK gKeyboardHook;
HMENU gPopupMenu;
NOTIFYICONDATA gNotifyIconData;


void CleanUp()
{
    LOG("CutieSlash shutting down");
    Shell_NotifyIcon(NIM_DELETE, &gNotifyIconData);
    UnhookWindowsHookEx(gKeyboardHook);
    UnregisterClass(WINDOW_CLASS, gInstance);
    ReleaseMutex(gGlobalMutex);
    CleanUpLog();
}

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
                LOGD("Keycode = " << static_cast<char>(key) << " (" << key << ")");
            else
                LOGD("Keycode = " << key);
        }
        #endif // _DEBUG

        switch (wParam)
        {
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            if (key == 220)
            {
                LOGD("WE GOT BACKSLASH DOWN, changing to " << VK_BACK);
                keybd_event(8, 0, 0, 0);
                eaten = true;
            }
            break;

        case WM_KEYUP:
        case WM_SYSKEYUP:
            if (key == 220)
            {
                LOGD("WE GOT BACKSLASH UP, changing to " << VK_BACK);
                keybd_event(8, 0, KEYEVENTF_KEYUP, 0);
                eaten = true;
            }
            break;
        }
    }

    return (eaten ? 1 : CallNextHookEx(gKeyboardHook, nCode, wParam, lParam));
}

void ShowContextMenu(HWND hWnd)
{
    POINT curPoint;
    GetCursorPos(&curPoint);
    SetForegroundWindow(hWnd);

    // TrackPopupMenu blocks the app until TrackPopupMenu returns
    UINT clicked = TrackPopupMenu(gPopupMenu, 0, curPoint.x, curPoint.y, 0, hWnd, NULL);
}

bool IsAutostartRegistered(bool& registered)
{
    HKEY key;
    LSTATUS status = RegOpenKeyEx(AUTOSTART_REG_KEY, AUTOSTART_REG_KEY_PATH, 0, KEY_QUERY_VALUE | KEY_READ | KEY_WRITE, &key);
    if (status != ERROR_SUCCESS)
    {
        MessageBox(0, L"Got some poopy error when opening autostart registry key", ERROR_MESSAGE_TITLE, 0);
        return false;
    }

    DWORD dataSize;
    DWORD dataType;
    status = RegQueryValueEx(key, AUTOSTART_REG_VALUE, 0, &dataType, NULL, &dataSize);
    if (status != ERROR_SUCCESS)
    {
        if (status == ERROR_FILE_NOT_FOUND)
        {
            LOG("Autostart entry does not exist in Windows registry");
            registered = false;
            return true;
        }
        else
        {
            MessageBox(0, L"Got some poopy error when getting value of autostart", ERROR_MESSAGE_TITLE, 0);
            RegCloseKey(key);
            return false;
        }
    }

    if (dataType != REG_SZ)
    {
        MessageBox(0, L"That registry cutie key is not a string! Someone pooped us!", ERROR_MESSAGE_TITLE, 0);
        RegCloseKey(key);
        return false;
    }

    std::vector<WCHAR> data(dataSize / sizeof(WCHAR)); // dataSize is in bytes
    status = RegQueryValueEx(key, AUTOSTART_REG_VALUE, 0, &dataType, reinterpret_cast<BYTE*>(data.data()), &dataSize);
    if (status != ERROR_SUCCESS)
    {
        MessageBox(0, L"Failed to read registry data (oop)", ERROR_MESSAGE_TITLE, 0);
        RegCloseKey(key);
        return false;
    }

    bool equal = true;
    if (data.size() != gExePathSize)
        equal = false;

    if (equal)
    {
        for (uint32_t i = 0; i < data.size(); ++i)
        {
            if (data[i] != gExePath[i])
            {
                equal = false;
                break;
            }
        }
    }

    if (!equal)
    {
        LOG("Autostart registry entry exists but is invalid - someone moved the binary? Updating it");
        // somethin wrong - overwrite with actual new path to autostart so we are up to date yay
        status = RegSetValueEx(key, AUTOSTART_REG_VALUE, 0, REG_SZ, reinterpret_cast<BYTE*>(gExePath), (gExePathSize) * sizeof(WCHAR));
        if (status != ERROR_SUCCESS)
        {
            MessageBox(0, L"Failed to set autostart registry value (help me)", ERROR_MESSAGE_TITLE, 0);
            RegCloseKey(key);
            return false;
        }
    }

    RegCloseKey(key);

    LOG("Found Autostart registry value - Autostart is enabled");
    registered = true;
    return true;
}

bool RegisterAutostart()
{
    HKEY key = NULL;
    LSTATUS status = RegCreateKeyEx(AUTOSTART_REG_KEY, AUTOSTART_REG_KEY_PATH, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &key, NULL);
    if (status != ERROR_SUCCESS)
    {
        MessageBox(0, L"Oop, I failed to create a registry entry ;;", ERROR_MESSAGE_TITLE, 0);
        return false;
    }

    status = RegSetValueEx(key, AUTOSTART_REG_VALUE, 0, REG_SZ, reinterpret_cast<BYTE*>(gExePath), (gExePathSize + 1) * sizeof(WCHAR));
    if (status != ERROR_SUCCESS)
    {
        MessageBox(0, L"Failed to set autostart registry value (help me)", ERROR_MESSAGE_TITLE, 0);
        RegCloseKey(key);
        return false;
    }

    RegCloseKey(key);
    LOG("Autostart entry registered");
    return true;
}

bool UnregisterAutostart()
{
    HKEY key = NULL;
    LSTATUS status = RegCreateKeyEx(AUTOSTART_REG_KEY, AUTOSTART_REG_KEY_PATH, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &key, NULL);
    if (status != ERROR_SUCCESS)
    {
        MessageBox(0, L"Oop, I failed to open a registry entry :(", ERROR_MESSAGE_TITLE, 0);
        return false;
    }

    status = RegDeleteValue(key, AUTOSTART_REG_VALUE);
    if (status != ERROR_SUCCESS)
    {
        MessageBox(0, L"Cannot delete this poopy autostart key", ERROR_MESSAGE_TITLE, 0);
        return false;
    }

    RegCloseKey(key);
    LOG("Autostart entry unregistered");
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
            break;
        }
        default:
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
            GetMenuItemInfo(gPopupMenu, wmId, FALSE, &info);
            if ((info.fState & MFS_CHECKED) == MFS_CHECKED)
                UnregisterAutostart();
            else
                RegisterAutostart();

            info.fState ^= MFS_CHECKED;
            SetMenuItemInfo(gPopupMenu, wmId, FALSE, &info);
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
    {
        PostQuitMessage(0);
        break;
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

bool Register(HINSTANCE hInstance)
{
    gGlobalMutex = CreateMutexEx(NULL, GLOBAL_MUTEX_NAME, CREATE_MUTEX_INITIAL_OWNER, DELETE);
    if (gGlobalMutex == NULL)
    {
        MessageBox(0, L"Failed to create CutieSlash global mutex .-.", ERROR_MESSAGE_TITLE, 0);
        return false;
    }

    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        MessageBox(0, L"I'm already running, cutie. Check your tray <3", L"Cutie slash is already zoomin", 0);
        return false;
    }

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
    wcex.lpszClassName  = WINDOW_CLASS;
    RegisterClassExW(&wcex);
    return true;
}

BOOL Init(int nCmdShow)
{
    HWND hWnd = CreateWindowW(WINDOW_CLASS, L"Cutie Slash", WS_OVERLAPPEDWINDOW,
                              CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr,
                              nullptr, gInstance, nullptr);

    if (!hWnd)
        return FALSE;

    bool autostartRegistered = false;
    if (!IsAutostartRegistered(autostartRegistered))
        return FALSE;

    gPopupMenu = CreatePopupMenu();
    AppendMenu(gPopupMenu, MF_STRING | MF_GRAYED, NULL, L"Hi cutie ily <3");
    AppendMenu(gPopupMenu, MF_SEPARATOR, NULL, NULL);
    AppendMenu(gPopupMenu, MF_STRING | (autostartRegistered ? MF_CHECKED : 0), IDM_AUTOSTART, L"Autostart");
    AppendMenu(gPopupMenu, MF_SEPARATOR, NULL, NULL);
    AppendMenu(gPopupMenu, MF_STRING, IDM_EXIT, L"Exit");

    ZeroMemory(&gNotifyIconData, sizeof(NOTIFYICONDATA));
    gNotifyIconData.cbSize = sizeof(NOTIFYICONDATAW);
    gNotifyIconData.uID = TRAYICON_ID;
    gNotifyIconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    gNotifyIconData.hIcon = (HICON)LoadImage(gInstance, L"res/CutieSlash.ico", IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_LOADFROMFILE);
    if (gNotifyIconData.hIcon == NULL)
    {
        MessageBox(0, L"Couldn't find cute icon for cute program", ERROR_MESSAGE_TITLE, 0);
        return FALSE;
    }
    gNotifyIconData.hWnd = hWnd;
    gNotifyIconData.dwInfoFlags = NIIF_INFO;
    gNotifyIconData.uVersion = NOTIFYICON_VERSION_4;
    wcsncpy_s(gNotifyIconData.szTip, 128, L"Cutie slash replacer hihi <3", 28);
    gNotifyIconData.uCallbackMessage = NOTIFICATION_TRAY_ICON_MSG;
    Shell_NotifyIcon(NIM_ADD, &gNotifyIconData);

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

    if (!InitLog())
        return 0;

    // Get executable path
    HMODULE exe = GetModuleHandle(0);
    gExePath[0] = '"';
    gExePathSize = GetModuleFileName(exe, gExePath + 1, 1024);
    gExePath[gExePathSize + 1] = '"';
    gExePathSize += 3;

    // Register our application class
    if (!Register(hInstance))
        return 0;

    // Perform application initialization:
    if (!Init(nCmdShow))
        return 0;

    LOG("Hooking up CutieSlash keyboard hook");
    gKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardEvent, hInstance, 0);

    // Main message loop:
    LOG("CutieSlash started and running");
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    CleanUp();
    return (int)msg.wParam;
}
