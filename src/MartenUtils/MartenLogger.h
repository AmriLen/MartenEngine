#pragma once
#include <string>

namespace MartenLogger {
    enum class Level {
        SECS,
        INFO,
        WARN,
        ERR,
        CRIT
    };

    void Initialize();
    void Shutdown();
    void Log(Level level, const std::string& message);

    void SECS(const std::string& message);
    void INFO(const std::string& message);
    void WARN(const std::string& message);
    void ERR(const std::string& message);
    void CRIT(const std::string& message);
}