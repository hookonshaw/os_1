cmake_minimum_required(VERSION 3.10)
project(DivisionProject)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включение директорий с исходными файлами
include_directories(include)

# Общие исходные файлы для обоих программ
set(COMMON_SOURCES src/division_os.cpp)

# Создание исполняемого файла для родительского процесса
add_executable(division_parent src/division_parent.cpp ${COMMON_SOURCES})

# Создание исполняемого файла для дочернего процесса
add_executable(division_child src/division_child.cpp ${COMMON_SOURCES})

# Установка свойств для компиляции
set_target_properties(division_parent division_child PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# Для Linux/Unix систем добавляем необходимые библиотеки
if(UNIX)
    find_package(Threads REQUIRED)
    target_link_libraries(division_parent Threads::Threads)
    target_link_libraries(division_child Threads::Threads)
endif()