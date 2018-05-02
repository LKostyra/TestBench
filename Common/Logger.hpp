#pragma once

#include <string>
#include <sstream>


namespace Common {

enum class LogSeverity: unsigned char
{
    Error,
    Warning,
    Info,
    Debug
};

void Log(LogSeverity severity, const std::string& text, const std::string& file, const uint32_t line);

} // namespace Common


#define LOG(sev, x) do { \
        std::stringstream ss; \
        ss << x; \
        Common::Log(sev, ss.str(), __FILE__, __LINE__); \
    } while(0)

#define LOGE(x) LOG(Common::LogSeverity::Error, x)
#define LOGW(x) LOG(Common::LogSeverity::Warning, x)
#define LOGI(x) LOG(Common::LogSeverity::Info, x)
#define LOGD(x) LOG(Common::LogSeverity::Debug, x)

