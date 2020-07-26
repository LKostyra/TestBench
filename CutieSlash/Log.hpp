#pragma once


bool InitLog();
void CleanUpLog();
void Log(const char* file, int line, const char* func, const char* msg);


#define LOG(x) \
    do { \
        std::stringstream __ss; \
        __ss << x; \
        Log(__FILE__, __LINE__, __func__, __ss.str().c_str()); \
    } while(0)

#ifdef _DEBUG
#define LOGD(x) LOG(x)
#else
#define LOGD(x) do { } while(0)
#endif
