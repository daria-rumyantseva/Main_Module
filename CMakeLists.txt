cmake_minimum_required(VERSION 3.31.3)

# Укажите имя проекта
project(MainServer)

# Установите стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Указываем пути для поиска заголовочных файлов и бинарных файлов
set(OpenSSL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/include/openssl")
set(OpenSSL_LIBRARIES "${CMAKE_SOURCE_DIR}/lib/lib")

# Указываем путь для jwt-cpp
set(jwt-cpp_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/include/jwt-cpp")

# Указываем пути для pqxx и libpq++
set(PQXX_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/include/pqxx")
set(PQXX_LIBRARIES "${CMAKE_SOURCE_DIR}/lib/lib/pqxx.lib")
set(LIBPQ_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/include/libpq")
set(LIBPQ_LIBRARIES "${CMAKE_SOURCE_DIR}/lib/lib/libpq.lib")

# Указываем пути для sqlpp
set(SQLPP_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/include/sqlpp")
set(SQLPP_LIBRARIES "${CMAKE_SOURCE_DIR}/lib/lib/sqlpp.lib")

# Указываем директории для поиска заголовочных файлов
include_directories(
  ${OpenSSL_INCLUDE_DIR}
  ${jwt-cpp_INCLUDE_DIR}
  ${PQXX_INCLUDE_DIR}
  ${LIBPQ_INCLUDE_DIR}
  ${SQLPP_INCLUDE_DIR}
)

# Укажите исходные файлы проекта
add_executable(main_server 
  server.cpp
)

# Привязываем библиотеки
target_link_libraries(main_server PRIVATE
  ${OpenSSL_LIBRARIES}/libcrypto.lib
  ${OpenSSL_LIBRARIES}/libssl.lib  # Если требуется, добавьте ssl библиотеку
  ${jwt-cpp_LIBRARIES}
  ${PQXX_LIBRARIES}
  ${LIBPQ_LIBRARIES}
  ${SQLPP_LIBRARIES}
)
