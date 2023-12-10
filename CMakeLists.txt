cmake_minimum_required(VERSION 3.10)
project(Play_Chess VERSION 0.3)

add_executable(Play_Chess main.cpp)

add_subdirectory(src)

configure_file(Play_Chess_Config.h.in Play_Chess_Config.h)

target_include_directories(Play_Chess PUBLIC 
                            builds
                            include
                            src)

target_link_libraries(Play_Chess PUBLIC Chess_API)
