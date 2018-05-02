#include "Logger.hpp"
#include "LoggerPlatform.hpp"


namespace Common {

void Log(LogSeverity severity, const std::string& text, const std::string& file, const uint32_t line)
{
    std::string tag;

    switch (severity)
    {
    case LogSeverity::Error: tag = "[ERROR]"; break;
    case LogSeverity::Warning: tag = "[ WRN ]"; break;
    case LogSeverity::Info: tag = "[ INF ]"; break;
    case LogSeverity::Debug: tag = "[ DBG ]"; break;
    }

    std::stringstream ss;
    ss << tag << " " << file << " @ " << std::to_string(line) << ": " << text;

    switch (severity)
    {
    case LogSeverity::Error: LogPlatform(LogColor::Red, ss.str()); break;
    case LogSeverity::Warning: LogPlatform(LogColor::Yellow, ss.str()); break;
    case LogSeverity::Info: LogPlatform(LogColor::Default, ss.str()); break;
    case LogSeverity::Debug: LogPlatform(LogColor::Blue, ss.str()); break;
    }
}

} // namespace Common
