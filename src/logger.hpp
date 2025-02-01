#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <chrono>
#include <ctime>
#include <fstream>
#include <mutex>
#include <sstream>
#include <string>

class Logger {
public:
    enum class Level { DEBUG, INFO, ERROR };

    explicit Logger(const std::string& filename, Level level = Level::INFO);
    ~Logger();

    void Log(const std::string& message, Level level);
    void SetLogLevel(Level level);

private:
    std::ofstream log_file_;
    Level log_level_;
    std::mutex log_mutex_;

    std::string GetCurrentTime() const;
    std::string LevelToString(Level level) const;
};

#endif // LOGGER_HPP
