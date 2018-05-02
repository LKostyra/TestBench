SET(COMMON_SRCS Logger.cpp
                )

SET(COMMON_HDRS Logger.hpp
                LoggerPlatform.hpp
                )

IF(WIN32)
    SET(COMMON_PLATFORM_SRCS Win/LoggerPlatform.cpp
                             )
ELSEIF(UNIX)
    SET(COMMON_PLATFORM_SRCS Linux/LoggerPlatform.cpp
                             )
ELSE()
    MESSAGE(FATAL_ERROR "Platform unsupported")
ENDIF()

ADD_LIBRARY(${COMMON_TARGET}
            ${COMMON_SRCS} ${COMMON_HDRS}
            ${COMMON_PLATFORM_SRCS}
            )

TESTBENCH_COPY_RESULT(${COMMON_TARGET})
