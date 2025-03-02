#include "logger.hpp"
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

struct LogMessage {
    std::string text;
    Logger::Level level;
};

class LogManager {
public:
    LogManager(Logger& logger) : logger_(logger), exit_flag_(false) {
        worker_thread_ = std::thread(&LogManager::ProcessQueue, this); //Инициализация треда на метод ProcessQueue
    }

    ~LogManager() {
        {
            std::lock_guard<std::mutex> lock(queue_mutex_); //Безопасно забираем ресуры для удаления
            exit_flag_ = true;
        }
        cv_.notify_one(); //Уведомление следюущего треда
        if (worker_thread_.joinable()) {
            worker_thread_.join(); //Корректное завершение
        }
    }

    void Enqueue(const LogMessage& msg) {
        {
            std::lock_guard<std::mutex> lock(queue_mutex_); //Безопасно берем ресурсы
            log_queue_.push(msg); //Забираем сообщение в очередь
        }
        cv_.notify_one(); //Уведомление следюущего треда
    }

    void ChangeLevel(Logger::Level lvl) {
        logger_.SetLogLevel(lvl);
    }

private:
    void ProcessQueue() {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        while (!exit_flag_ || !log_queue_.empty()) {
            cv_.wait(lock, [&] { return exit_flag_ || !log_queue_.empty(); }); //Ждем пока работает приложение или пока очередь пуста
            while (!log_queue_.empty()) {
                LogMessage msg = log_queue_.front();
                log_queue_.pop();
                lock.unlock();
                logger_.Log(msg.text, msg.level); //Потокобезопасно передаем сообщение в Logger
                lock.lock();
            }
        }
    }

    Logger& logger_;
    std::queue<LogMessage> log_queue_;
    std::mutex queue_mutex_;
    std::condition_variable cv_;
    bool exit_flag_;
    std::thread worker_thread_;
};

Logger::Level ParseLogLevel(const std::string& level_str) {
    if (level_str == "DEBUG") {
        return Logger::Level::DEBUG;
    }
    if (level_str == "INFO") {
        return Logger::Level::INFO;
    }
    if (level_str == "ERROR") {
        return Logger::Level::ERROR;
    }
    throw std::invalid_argument("Ошибка: некорректный уровень логирования!");
}

int InputHandler(LogManager& log_manager, Logger::Level& default_level) {
    std::string input;
    while (true) {
        std::getline(std::cin, input);
        if (input == "exit") {
            return 0;
        }
        Logger::Level msg_level = default_level;
        std::size_t pos = input.find_last_of(' ');
        std::string last_word;
        if (pos != std::string::npos) {
            last_word = input.substr(pos + 1);
            try {
                msg_level = ParseLogLevel(last_word); //Парсим слово в уровень лога или же ловим ошибку
                input = input.substr(0, pos);
            } catch (const std::exception& exc) {
                std::cerr << exc.what() << "\n";
                return 1;
            }
        }
        if (input == "ChangeImportanceLevel") {
            log_manager.ChangeLevel(msg_level); //Меняем минимальный уровень важности
        } else if (input == "ChangeDefaultLevel") {
            default_level = msg_level; //Меняем уровень важности по умолчанию
        }
        else {
            log_manager.Enqueue({input, msg_level}); //Записываем сообщение
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Использование: " << argv[0] << " <лог-файл> <уровень>" << "\n";
        std::cerr << "Уровни: DEBUG, INFO, ERROR" << "\n";
        return 1;
    }

    std::string log_file = argv[1];
    Logger::Level default_level;

    try {
        default_level = ParseLogLevel(argv[2]);
    } catch (const std::exception& exc) {
        std::cerr << exc.what() << "\n";
        return 1;
    }

    Logger logger(log_file, default_level);
    LogManager log_manager(logger);

    std::cout << "Введите сообщение (<текст> [DEBUG/INFO/ERROR]).\n"
                 "Введите 'exit' для выхода.\n"
                 "ChangeImportanceLevel DEBUG/INFO/ERROR для изменения мин уровня важности записываемых сообщений.\n"
                 "ChangeDefaultLevel DEBUG/INFO/ERROR для изменения уровня важности по умолчанию.\n";

    return InputHandler(log_manager, default_level);
}
