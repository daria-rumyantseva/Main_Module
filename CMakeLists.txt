
cmake_minimum_required(VERSION 3.31.3)

project(MainServer)

include_directories(${CMAKE_SOURCE_DIR}/lib/include)
include_directories(${CMAKE_SOURCE_DIR}/)
link_directories(${CMAKE_SOURCE_DIR}/lib)

add_executable(main_server 
  server.cpp
)
