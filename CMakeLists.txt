cmake_minimum_required(VERSION 3.5)
project(CVRP)

set(CMAKE_CXX_STANDARD 11)

# Algunos flags para pasar al compilador (gnu++11 en vez de c++11 para que sea cross-plat)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -ggdb3 -g -O0")

# Leemos todos los archivos fuentes en ./src
file(GLOB SOURCE_FILES src/*)

add_executable(tp ${SOURCE_FILES})


