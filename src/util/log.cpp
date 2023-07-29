#include "log.h"

// 获取当前时间和日期
std::string getCurrentTime() {
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);

    std::ostringstream oss; // 使用 std::ostringstream 来存储输出结果
    oss << std::put_time(localTime, "%Y-%m-%d %H:%M:%S"); // 将输出结果写入到 oss 中
    return oss.str(); // 将 oss 中的输出结果转换为 std::string 并返回
}

// 辅助函数：将日志信息输出到指定流
void logToStream(LogLevel level, std::ostream& stream, const std::string& message) {
    // 定义日志级别标签和颜色
    const char* logTags[] = {
        "\033[1;31m[ERROR] \033[0m",
        "\033[1;32m[INFO] \033[0m",
        "\033[1;33m[WARN] \033[0m"
    };

    // 获取当前时间和日期
    std::string currentTime = getCurrentTime();

    // 输出时间和日期
    stream << "[" << currentTime << "] ";

    // 输出日志级别标签和信息
    stream << logTags[static_cast<int>(level)] << message << std::endl;
}

// log函数封装
void log(LogLevel level, const std::string& message) {
    // 创建文件名，例如 log_2023-07-27_12-34-56.txt
    static std::string filename = "log_" + getCurrentTime() + ".txt";

    // 打开文件
    static std::ofstream logFile(filename, std::ios::app);
    static int lineCounter = 0;

    // 判断是否需要切换文件
    if (lineCounter >= 500) {
        logFile.close();
        filename = "log_" + getCurrentTime() + ".txt";
        logFile.open(filename, std::ios::app);
        lineCounter = 0;
    }

    // 输出日志信息到文件
    logToStream(level, logFile, message);

    // 输出日志信息到屏幕
    logToStream(level, std::cout, message);

    // 更新行计数器
    ++lineCounter;
}
