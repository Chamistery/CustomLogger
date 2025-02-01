#include "logger.hpp"
#include <iomanip>
#include <iostream>

Logger::Logger(const std::string& filename, Level level) : log_level_(level) {
    log_file_.open(filename, std::ios::app); //Инициализируем журнал
    if (!log_file_.is_open()) {
        throw std::runtime_error("Ошибка: не удалось открыть файл журнала!");
    }
}

Logger::~Logger() {
    if (log_file_.is_open()) {
        log_file_.close();
    }
}

void Logger::Log(const std::string& message, Level level) {
    std::lock_guard<std::mutex> lock(log_mutex_); //Безопасно берем ресурсы
    if (level < log_level_) {
        return;
    }

    log_file_ << "[" << GetCurrentTime() << "] "
            << "[" << LevelToString(level) << "] "
            << message << "\n";
}

void Logger::SetLogLevel(Level level) {
    std::lock_guard<std::mutex> lock(log_mutex_);
    log_level_ = level;
}

std::string Logger::GetCurrentTime() const {
    //Переводы из одного формата в другой
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::tm tm_info = *std::localtime(&time);
    std::ostringstream oss;
    oss << std::put_time(&tm_info, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::string Logger::LevelToString(Level level) const {
    switch (level) {
        case Level::DEBUG: return "DEBUG";
        case Level::INFO: return "INFO";
        case Level::ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}
