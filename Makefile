CC = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread
LDFLAGS = -L$(BUILD_DIR) -Wl,-rpath,$(shell pwd)/build
LIB_NAME = liblogger.so
TARGET = logger_app
SRC_DIR = src
BUILD_DIR = build

SRCS = $(SRC_DIR)/logger.cpp
OBJS = $(BUILD_DIR)/logger.o
MAIN_SRC = $(SRC_DIR)/main.cpp

# Главная цель, собирающая и библиотеку, и приложение
all: $(LIB_NAME) $(TARGET)

# Сборка динамической библиотеки
$(LIB_NAME): $(OBJS)
	$(CC) -shared -o $(BUILD_DIR)/$(LIB_NAME) $(OBJS)

# Сборка объекта для библиотеки
$(BUILD_DIR)/logger.o: $(SRC_DIR)/logger.cpp $(SRC_DIR)/logger.hpp
	mkdir -p $(BUILD_DIR)
	$(CC) $(CXXFLAGS) -fPIC -c $(SRC_DIR)/logger.cpp -o $(BUILD_DIR)/logger.o

# Сборка тестового приложения
$(TARGET): $(MAIN_SRC) $(LIB_NAME)
	$(CC) $(CXXFLAGS) $(MAIN_SRC) -o $(BUILD_DIR)/$(TARGET) $(LDFLAGS) -llogger

# Цель для запуска тестов в файл log.txt c уровнем логирования INFO
run:
	./$(BUILD_DIR)/logger_app log.txt INFO

# Очистка каталога от временных файлов
clean:
	rm -rf $(BUILD_DIR)
