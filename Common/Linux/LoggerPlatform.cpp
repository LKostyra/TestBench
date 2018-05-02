#include "../LoggerPlatform.hpp"

#include <iostream>


namespace Common {

void LogPlatform(LogColor color, const std::string& text)
{
    std::string colortag;
    const std::string terminator = "\033[0m";

    switch (color)
    {
    case LogColor::Default: colortag = "\033[2;37m"; break;
    case LogColor::Red: colortag = "\033[1;31m"; break;
    case LogColor::Blue: colortag = "\033[34m"; break;
    case LogColor::Yellow: colortag = "\033[33m"; break;
    };

    std::cout << colortag << text << terminator << std::endl;
}

} // namespace Common
