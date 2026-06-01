cmake_minimum_required(VERSION 3.16)
project(MIEMsense LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MINGW)
    add_link_options(-static-libgcc -static-libstdc++ -static)
endif()

enable_testing()

add_subdirectory(src)
add_subdirectory(tests)
