#include "PCH.hpp"
#include "Log.hpp"
#include "Timer.hpp"


namespace {

const char* LOG_FILE_NAME = "CutieSlashLog_";
const char* LOG_FILE_EXTENSION = ".txt";
std::fstream gLogFile;
Timer gAppTimer;
std::stringstream ss;

} // namespace


bool InitLog()
{
    SYSTEMTIME curTime;
    GetLocalTime(&curTime);

    uint32_t year = curTime.wYear % 100;
    std::stringstream fileName;
    fileName << LOG_FILE_NAME;
    fileName << std::setw(2) << std::setfill('0') << year;
    fileName << std::setw(2) << std::setfill('0') << curTime.wMonth;
    fileName << std::setw(2) << std::setfill('0') << curTime.wDay;
    fileName << "_";
    fileName << std::setw(2) << std::setfill('0') << curTime.wHour;
    fileName << std::setw(2) << std::setfill('0') << curTime.wMinute;
    fileName << std::setw(2) << std::setfill('0') << curTime.wSecond;
    fileName << LOG_FILE_EXTENSION;

    gLogFile.open(fileName.str(), std::fstream::out);
    if (!gLogFile.good())
        return false;

    ss.setf(std::ios::fixed, std::ios::floatfield);
    ss.precision(6);
    ss.str(std::string());

    gAppTimer.Start();
    LOG("CutieSlash log started!");

    return true;
}

void CleanUpLog()
{
    LOG("CutieSlash log closing. Bai cutie <3");
    gLogFile.close();
}

void Log(const char* file, int line, const char* func, const char* msg)
{
    ss << "[" << gAppTimer.Stop() << "] ";
    ss << file << ":" << line << " @ " << func << " - " << msg << std::endl;
    std::cout << ss.str();
    gLogFile << ss.str();
    gLogFile.flush();
    ss.str(std::string());
}
