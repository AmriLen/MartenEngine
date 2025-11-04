#include "../MartenLogger.hpp"
#include <iostream>
#include <fstream>
#include <ctime>

#ifdef _WIN32
    #include <windows.h>
#endif

namespace MartenLogger {
    static std::ofstream logFile;

    void Initialize() {
        // On ANSI colors in Windows
        #ifdef _WIN32
            system("powershell -command \"Set-ItemProperty HKCU:\\Console VirtualTerminalLevel -Type DWORD 1\"");
        #endif

        logFile.open("MartenEngine.log", std::ios::out | std::ios::trunc);
        if (!logFile.is_open()) {
            std::cerr << "[MartenLogger] Failed to open log file!" << std::endl;
        }
    }

    void Shutdown() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    std::string GetTimestamp() {
        time_t now = time(nullptr);
        tm* localTime = localtime(&now);
        char buffer[32];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localTime);
        return std::string(buffer);
    }

    void Log(Level level, const std::string& messege) {
        std::string prefix;
        std::string prefix_name;
        switch (level) {
            case Level::SECS:
                prefix = "[\033[32mSECSESS\033[0m] ";
                prefix_name = "[SECSESS] ";
                break;
            case Level::INFO:
                prefix = "[\033[36mINFO\033[0m] ";
                prefix_name = "[INFO] ";
                break;
            case Level::WARN:
                prefix = "[\033[33mWARN\033[0m] ";
                prefix_name = "[WARN] ";
                break;
            case Level::ERR:
                prefix = "[\033[31mERROR\033[0m] ";
                prefix_name = "[ERROR] ";
                break;
            case Level::CRIT:
                prefix = "[\033[37;41mCRITICAL\033[0m] ";
                prefix_name = "[CRITICAL] ";
                break;
        }

        std::string timestamp = GetTimestamp();
        std::string finalMessage = "[" + timestamp + "] " + prefix + messege;
        std::string finalFileMessage = "[" + timestamp + "] " + prefix_name + messege;

        std::cout << finalMessage << std::endl;

        if (logFile.is_open()) {
            logFile << finalFileMessage << std::endl;
        }
    }

    void SECS(const std::string& message) {
        Log(Level::SECS, message);
    }
    void INFO(const std::string& message) {
        Log(Level::INFO, message);
    }
    void WARN(const std::string& message) {
        Log(Level::WARN, message);
    }
    void ERR(const std::string& message) {
        Log(Level::ERR, message);
    }
    void CRIT(const std::string& message) {
        Log(Level::CRIT, message);
    }
}