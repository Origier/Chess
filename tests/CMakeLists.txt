add_library(tests_lib Chess_test.cpp Game_test.cpp)

target_include_directories(tests_lib PUBLIC ../include ../src)