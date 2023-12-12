#include "Chess.h"
#include "Play_Chess_Config.h"

#include <iostream>

#ifdef RUN_TESTS
#   include "Chess_test.h"
#endif

using namespace Chess_API;

int main(int argc, const char **argv) {
#ifdef RUN_TESTS
    std::cout << "Running in test mode..." << std::endl;
    run_chess_tests();
    return 0;
#else
    if (argc == 1) {
        std::cout << "Chess version " << Play_Chess_VERSION_MAJOR << "." << Play_Chess_VERSION_MINOR << std::endl;
        std::cout << "Usage: Chess {Play} -> Launches the chess game... (More options to come...)" << std::endl;
    } else {
        std::string input = argv[1];

        std::transform(input.cbegin(), input.cend(), input.begin(),
        [](const char c){return tolower(c);});

        if (input == "play") {
            Chess new_game;
            std::cout << "New chess game made successfully!" << std::endl;
        }
    }

    return 0;
#endif
}