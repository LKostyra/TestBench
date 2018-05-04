#pragma once

#ifdef WIN32
#define TB_INLINE __forceinline
#elif defined(__linux__) || defined(__LINUX__)
#define TB_INLINE __attribute__((always_inline))
#endif
