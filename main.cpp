#include "Chess.h"
#include "Play_Chess_Config.h"

#include <iostream>

#ifdef RUN_TESTS
#   include "Chess_test.h"
#   include "Game_test.h"
#endif

using namespace Chess_API;

int main(int argc, const char **argv) {
#ifdef RUN_TESTS
    int errors = 0;
    int total_errors = 0;
    std::cout << "CHESS IN TEST MODE:" << std::endl;
    std::cout << "Running Chess Object Tests:" << std::endl;
    errors += run_chess_tests();
    if (errors == 0) {
        std::cout << "All tests passed on Chess Object." << std::endl;
    } else {
        std::string test_form = errors == 1 ? "test" : "tests";
        std::cout << errors << " " + test_form + " failed on Chess Object." << std::endl;
    }

    total_errors += errors;
    errors = 0;

    std::cout << std::endl;
    std::cout << "Running Game Object Tests:" << std::endl;
    errors += run_game_tests();
    if (errors == 0) {
        std::cout << "All tests passed on Game Object." << std::endl;
    } else {
        std::string test_form = errors == 1 ? "test" : "tests";
        std::cout << errors << " " + test_form + " failed on Game Object." << std::endl;
    }

    total_errors += errors;
    errors = 0;
    
    std::cout << std::endl;
    std::cout << "Overall:" << std::endl;
    if (total_errors == 0) {
        std::cout << "All tests passed." << std::endl;
    } else {
        std::string test_form = total_errors == 1 ? "test" : "tests";
        std::cout << total_errors << " " + test_form + " failed." << std::endl;
    }

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