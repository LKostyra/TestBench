#include "../LoggerPlatform.hpp"

#include <Windows.h>
#include <iostream>


namespace Common {

void LogPlatform(LogColor color, const std::string& text)
{
    static HANDLE console = 0;
    if (!console)
        console = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO conInfo;
    GetConsoleScreenBufferInfo(console, &conInfo);

    switch (color)
    {
    case LogColor::Default: SetConsoleTextAttribute(console, conInfo.wAttributes); break;
    case LogColor::Red: SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_INTENSITY); break;
    case LogColor::Blue: SetConsoleTextAttribute(console, FOREGROUND_BLUE); break;
    case LogColor::Yellow: SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN); break;
    }

    std::cout << text << std::endl;

    SetConsoleTextAttribute(console, conInfo.wAttributes);
}

} // namespace Common
