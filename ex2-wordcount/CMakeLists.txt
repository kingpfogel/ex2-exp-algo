cmake_minimum_required(VERSION 3.17)
project(ex2_wordcount CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(ex2_wordcount "develop/wordcount/main.cpp")

find_package(OpenMP REQUIRED)
target_link_libraries(ex2_wordcount PRIVATE OpenMP::OpenMP_CXX)
