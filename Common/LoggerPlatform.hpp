#pragma once

#include <string>


namespace Common {

enum class LogColor: unsigned char
{
    Default = 0,
    Red,
    Blue,
    Yellow
};

void LogPlatform(LogColor color, const std::string& text);

} // namespace Common
