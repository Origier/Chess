add_library(Chess_API Chess.cpp Game.cpp Human_Player.cpp Computer_Player.cpp)

target_include_directories(Chess_API PUBLIC ../include)