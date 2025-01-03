cmake_minimum_required(VERSION 3.10)

# Укажите имя проекта
project(MainServer)

# Установите стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Для postgreSQL
find_package(sqlpp11 CONFIG REQUIRED) 
include_directories(${sqlpp11_INCLUDE_DIRS}) 
add_definitions(${sqlpp11_DEFINITIONS}) 
target_link_libraries(your_project ${sqlpp11_LIBRARIES})

# Укажите исходные файлы проекта
add_executable(main_server main.cpp)
