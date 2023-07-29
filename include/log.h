#ifndef __LOG_H__
#define __LOG_H__

#include "util.h"

// 枚举表示日志级别
enum class LogLevel {
    ERROR,
    INFO,
    WARN
};

// log函数封装
void log(LogLevel level, const std::string& message);

#endif // __LOG_H__