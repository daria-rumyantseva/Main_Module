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

# Указываем пути для libpq++, pqxx и sqlpp
set(PQXX_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/include/pqxx")
set(PQXX_LIBRARIES "${CMAKE_SOURCE_DIR}/lib/libpqxx.a") # Замените на соответствующую библиотеку
set(SQLPP_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/include/sqlpp")
set(SQLPP_LIBRARIES "${CMAKE_SOURCE_DIR}/lib/libsqlpp.a") # Замените на соответствующую библиотеку

# Указываем директории для поиска заголовочных файлов
include_directories(
  ${OpenSSL_INCLUDE_DIR}
  ${jwt-cpp_INCLUDE_DIR}
  ${PQXX_INCLUDE_DIR}
  ${SQLPP_INCLUDE_DIR}
)

# Укажите исходные файлы проекта
add_executable(main_server 
  server.cpp
)

# Привязываем библиотеки
target_link_libraries(main_server PRIVATE
  ${OpenSSL_LIBRARIES}/libcrypto.lib
  ${OpenSSL_LIBRARIES}/libssl.lib # Добавьте, если требуется
  ${jwt-cpp_LIBRARIES}
  ${PQXX_LIBRARIES}
  ${SQLPP_LIBRARIES}
)
