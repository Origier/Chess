#include "Game_test.h"

using namespace Chess_API;
using namespace std;

// Tests creating a basic game object and ensuring there isn't any memory leaks issues - returns true if successful
bool test_create_game() {
    try {
        Game * new_game = new Game;
        delete new_game;
        return true;
    } catch (exception& e) {
        return false;
    }
}

// Tests creating a random number of games between 100 - 200 to ensure the behavior is stable for memory
bool test_creating_many_games() {
    int number_of_games = rand() % 101 + 100;

    try {
        Game ** game_array = new Game * [number_of_games];
        // Pushing all of the games onto the stack
        for (int i = 0; i < number_of_games; ++i) {
            game_array[i] = new Game;
        }

        // Deleting all of the games from the stack
        for (int i = 0; i < number_of_games; ++i) {
            delete game_array[i];
        }

        // Finally deallocating the full array
        delete[] game_array;

        // Assuming no issues to this point then the test has passed
        return true;

    } catch (exception& e) {
        return false;
    }
}

// Tests fetching a random location on the board and asserting that the location is empty
bool test_getting_rand_location_board_empty() {
    int x = rand() % DEFAULT_CHESS_BOARD_SIZE;
    int y = rand() % DEFAULT_CHESS_BOARD_SIZE;

    Game new_game;

    Game::game_piece piece = new_game.get_location(make_pair(x, y));

    // Ensuring this location is an invalid piece - aka 'empty'
    if (piece.type != Game::GAME_PIECE_TYPE::NOTYPE || piece.color != Game::GAME_PIECE_COLOR::NOCOLOR) {
        return false;
    } else {
        return true;
    }
}

// Tests that fetching items outside of the bounds will throw an error
bool test_no_access_to_out_of_bounds() {
    // Case where both are above bounds
    int x1 = DEFAULT_CHESS_BOARD_SIZE + 1;
    int y1 = DEFAULT_CHESS_BOARD_SIZE + 1;

    // Case where both are below bounds
    int x2 = -1;
    int y2 = -1;

    // Case where only x is above bounds
    int x3 = DEFAULT_CHESS_BOARD_SIZE + 1;
    int y3 = rand() % DEFAULT_CHESS_BOARD_SIZE;

    // Case where only y is above bounds
    int x4 = rand() % DEFAULT_CHESS_BOARD_SIZE;
    int y4 = DEFAULT_CHESS_BOARD_SIZE + 1;

    // Case where only x is below bounds
    int x5 = -1;
    int y5 = rand() % DEFAULT_CHESS_BOARD_SIZE;

    // Case where only y is below bounds
    int x6 = rand() % DEFAULT_CHESS_BOARD_SIZE;
    int y6 = -1;
    
    Game new_game;

    try {
        new_game.get_location(make_pair(x1, y1));
        // If the above did not throw an error then we need to abort and return a false
        return false;
    } catch (runtime_error r) {
        // All good - this means the code is working correctly
    }

    try {
        new_game.get_location(make_pair(x2, y2));
        // If the above did not throw an error then we need to abort and return a false
        return false;
    } catch (runtime_error r) {
        // All good - this means the code is working correctly
    }

    try {
        new_game.get_location(make_pair(x3, y3));
        // If the above did not throw an error then we need to abort and return a false
        return false;
    } catch (runtime_error r) {
        // All good - this means the code is working correctly
    }

    try {
        new_game.get_location(make_pair(x4, y4));
        // If the above did not throw an error then we need to abort and return a false
        return false;
    } catch (runtime_error r) {
        // All good - this means the code is working correctly
    }

    try {
        new_game.get_location(make_pair(x5, y5));
        // If the above did not throw an error then we need to abort and return a false
        return false;
    } catch (runtime_error r) {
        // All good - this means the code is working correctly
    }

    try {
        new_game.get_location(make_pair(x6, y6));
        // If the above did not throw an error then we need to abort and return a false
        return false;
    } catch (runtime_error r) {
        // All good - this means the code is working correctly
    }

    // If we made it here then all worked as intended
    return true;
}

// Tests adding a random piece to the board and ensuring the piece remains persistent on the board
bool test_adding_piece_to_board() {
    int x = rand() % DEFAULT_CHESS_BOARD_SIZE;
    int y = rand() % DEFAULT_CHESS_BOARD_SIZE;

    // Casting a random piece type and color
    Game::GAME_PIECE_TYPE type = static_cast<Game::GAME_PIECE_TYPE>(rand() % Game::GAME_PIECE_TYPE::TYPEMAX + 1);
    Game::GAME_PIECE_COLOR color = static_cast<Game::GAME_PIECE_COLOR>(rand() % Game::GAME_PIECE_COLOR::COLORMAX + 1);

    Game new_game;

    // Adding the piece
    new_game.add_piece(type, color, make_pair(x, y));

    // Grabbing the piece to ensure persistence
    Game::game_piece piece = new_game.get_location(make_pair(x, y));

    // Ensuring the piece persisted
    if (piece.color == Game::GAME_PIECE_COLOR::NOCOLOR && piece.type == Game::GAME_PIECE_TYPE::NOTYPE) {
        return false;
    }

    // Validating the piece information
    if (piece.color == color && piece.type == type) {
        return true;
    } else {
        return false;
    }
}

// Tests the constness of game_pieces passed from get_location - no details modified should persist on the game board
// Essentially this ensures that the piece passed is a copy rather than a reference
bool test_constness_get_location() {
    int x = rand() % DEFAULT_CHESS_BOARD_SIZE;
    int y = rand() % DEFAULT_CHESS_BOARD_SIZE;

    // Casting a random piece type and color
    Game::GAME_PIECE_TYPE type1 = static_cast<Game::GAME_PIECE_TYPE>(rand() % Game::GAME_PIECE_TYPE::TYPEMAX + 1);
    Game::GAME_PIECE_COLOR color1 = static_cast<Game::GAME_PIECE_COLOR>(rand() % Game::GAME_PIECE_COLOR::COLORMAX + 1);

    Game::GAME_PIECE_TYPE type2 = static_cast<Game::GAME_PIECE_TYPE>(rand() % Game::GAME_PIECE_TYPE::TYPEMAX + 1);
    Game::GAME_PIECE_COLOR color2 = static_cast<Game::GAME_PIECE_COLOR>(rand() % Game::GAME_PIECE_COLOR::COLORMAX + 1);

    // Ensuring the attributes are different for testing changes
    while (true) {
        bool types_differ = false;
        if (type2 == type1) {
            type2 = static_cast<Game::GAME_PIECE_TYPE>(rand() % Game::GAME_PIECE_TYPE::TYPEMAX + 1);
        } else {
            types_differ = true;
        }

        bool colors_differ = false;
        if (color2 == color1) {
            color2 = static_cast<Game::GAME_PIECE_COLOR>(rand() % Game::GAME_PIECE_COLOR::COLORMAX + 1);
        } else {
            colors_differ = true;
        }

        if (colors_differ && types_differ) {
            break;
        }
    }

    Game new_game;

    // Adding the piece
    new_game.add_piece(type1, color1, make_pair(x, y));

    // Grabbing the piece for testing changes made to it
    Game::game_piece piece1 = new_game.get_location(make_pair(x, y));

    // Applying changes to what should be a copy of the piece grabbed
    piece1.color = color2;
    piece1.type = type2;

    // Grabbing the piece again to ensure that the changes did not persist to the original
    Game::game_piece piece2 = new_game.get_location(make_pair(x, y));
    
    if (piece2.color == color2) {
        return false;
    }

    if (piece2.type == type2) {
        return false;
    }

    // Assuming we make it here - the changes did not persist on the original and therefore the pieces are safe from external changes
    return true;
}

// Tests adding a random piece to a spot where a piece is already placed - ensuring an error is thrown
bool test_adding_piece_to_same_spot() {
    int x = rand() % DEFAULT_CHESS_BOARD_SIZE;
    int y = rand() % DEFAULT_CHESS_BOARD_SIZE;

    // Casting a random piece type and color
    Game::GAME_PIECE_TYPE type1 = static_cast<Game::GAME_PIECE_TYPE>(rand() % Game::GAME_PIECE_TYPE::TYPEMAX + 1);
    Game::GAME_PIECE_COLOR color1 = static_cast<Game::GAME_PIECE_COLOR>(rand() % Game::GAME_PIECE_COLOR::COLORMAX + 1);

    Game::GAME_PIECE_TYPE type2 = static_cast<Game::GAME_PIECE_TYPE>(rand() % Game::GAME_PIECE_TYPE::TYPEMAX + 1);
    Game::GAME_PIECE_COLOR color2 = static_cast<Game::GAME_PIECE_COLOR>(rand() % Game::GAME_PIECE_COLOR::COLORMAX + 1);

    Game new_game;

    // Adding the piece
    new_game.add_piece(type1, color1, make_pair(x, y));

    try {
        new_game.add_piece(type2, color2, make_pair(x, y));
        // Shouldn't reach here - if it does then there is a problem
        return false;
    } catch (runtime_error e) {
        // This means the code is running as expected
        return true;
    }
}

// Tests adding a random number of pieces to the board (10 - 20) and ensuring no issues with deallocating and placement
bool test_adding_many_pieces() {
    int number_of_pieces = rand() % 11 + 10;

    // Helper struct for later reference
    struct location_info {
        pair<int, int> location;
        Game::GAME_PIECE_COLOR color;
        Game::GAME_PIECE_TYPE type;

        location_info(pair<int, int> location_in, Game::GAME_PIECE_COLOR color_in, Game::GAME_PIECE_TYPE type_in) :
                    location(location_in), color(color_in), type(type_in) {}
    };

    vector<location_info> location_info_vec;
    
    Game new_game;

    // Fetching random components for pieces and placing them on the board
    try {
        for (int i = 0; i < number_of_pieces; ++i) {
            int x = rand() % DEFAULT_CHESS_BOARD_SIZE;
            int y = rand() % DEFAULT_CHESS_BOARD_SIZE;

            // Casting a random piece type and color
            Game::GAME_PIECE_TYPE type = static_cast<Game::GAME_PIECE_TYPE>(rand() % Game::GAME_PIECE_TYPE::TYPEMAX + 1);
            Game::GAME_PIECE_COLOR color = static_cast<Game::GAME_PIECE_COLOR>(rand() % Game::GAME_PIECE_COLOR::COLORMAX + 1);

            // Infinite loop try / catch statement to bruteforce a random placement of pieces that don't collide with each other
            while (true) {
                try {
                    new_game.add_piece(type, color, make_pair(x, y));

                    // Adding to the location info vector for confirmation later
                    location_info_vec.push_back(location_info(make_pair(x, y), color, type));

                    break; // As long as we got here then this shouldn't be on top of another piece - good to get out of the loop
                } catch (runtime_error e) {
                    // Getting here means we attempted to place on top of another piece - fetch a new random location
                    x = rand() % DEFAULT_CHESS_BOARD_SIZE;
                    y = rand() % DEFAULT_CHESS_BOARD_SIZE;
                }
            }
        }
    
        // If we make it here then all pieces should have been added fine
        // Now validate that every location in the location_info_vec lines up with an item in the board
        Game::game_piece piece;
        for (int i = 0; i < location_info_vec.size(); ++i) {
            piece = new_game.get_location(location_info_vec[i].location);

            // Ensuring the piece persisted
            if (piece.color == Game::GAME_PIECE_COLOR::NOCOLOR && piece.type == Game::GAME_PIECE_TYPE::NOTYPE) {
                return false;
            }

            // Ensuring the color is correct
            if (piece.color != location_info_vec[i].color) {
                return false;
            }

            // Ensuring the type is correct
            if (piece.type != location_info_vec[i].type) {
                return false;
            }
        }

        // If we get through the whole vector and each piece has persisted then there are no issues
        return true;

    } catch (...) {
        return false;
    }
}

// Tests adding a random piece to an invalid location on the board
bool test_adding_piece_to_invalid_location() {
    // Case where both are above bounds
    int x1 = DEFAULT_CHESS_BOARD_SIZE + 1;
    int y1 = DEFAULT_CHESS_BOARD_SIZE + 1;

    // Case where both are below bounds
    int x2 = -1;
    int y2 = -1;

    // Case where only x is above bounds
    int x3 = DEFAULT_CHESS_BOARD_SIZE + 1;
    int y3 = rand() % DEFAULT_CHESS_BOARD_SIZE;

    // Case where only y is above bounds
    int x4 = rand() % DEFAULT_CHESS_BOARD_SIZE;
    int y4 = DEFAULT_CHESS_BOARD_SIZE + 1;

    // Case where only x is below bounds
    int x5 = -1;
    int y5 = rand() % DEFAULT_CHESS_BOARD_SIZE;

    // Case where only y is below bounds
    int x6 = rand() % DEFAULT_CHESS_BOARD_SIZE;
    int y6 = -1;

    // Casting a random piece type and color
    Game::GAME_PIECE_TYPE type = static_cast<Game::GAME_PIECE_TYPE>(rand() % Game::GAME_PIECE_TYPE::TYPEMAX + 1);
    Game::GAME_PIECE_COLOR color = static_cast<Game::GAME_PIECE_COLOR>(rand() % Game::GAME_PIECE_COLOR::COLORMAX + 1);

    Game new_game;

    try {
        new_game.add_piece(type, color, make_pair(x1, y1));
        // If the above did not throw an error then we need to abort and return a false
        return false;
    } catch (runtime_error r) {
        // All good - this means the code is working correctly
    }

    try {
        new_game.add_piece(type, color, make_pair(x2, y2));
        // If the above did not throw an error then we need to abort and return a false
        return false;
    } catch (runtime_error r) {
        // All good - this means the code is working correctly
    }

    try {
        new_game.add_piece(type, color, make_pair(x3, y3));
        // If the above did not throw an error then we need to abort and return a false
        return false;
    } catch (runtime_error r) {
        // All good - this means the code is working correctly
    }

    try {
        new_game.add_piece(type, color, make_pair(x4, y4));
        // If the above did not throw an error then we need to abort and return a false
        return false;
    } catch (runtime_error r) {
        // All good - this means the code is working correctly
    }

    try {
        new_game.add_piece(type, color, make_pair(x5, y5));
        // If the above did not throw an error then we need to abort and return a false
        return false;
    } catch (runtime_error r) {
        // All good - this means the code is working correctly
    }

    try {
        new_game.add_piece(type, color, make_pair(x6, y6));
        // If the above did not throw an error then we need to abort and return a false
        return false;
    } catch (runtime_error r) {
        // All good - this means the code is working correctly
    }

    // If we made it here then all worked as intended
    return true;

}

// Tests removing a random piece from a location
bool test_removing_piece() {
    int x = rand() % DEFAULT_CHESS_BOARD_SIZE;
    int y = rand() % DEFAULT_CHESS_BOARD_SIZE;

    // Casting a random piece type and color
    Game::GAME_PIECE_TYPE type = static_cast<Game::GAME_PIECE_TYPE>(rand() % Game::GAME_PIECE_TYPE::TYPEMAX + 1);
    Game::GAME_PIECE_COLOR color = static_cast<Game::GAME_PIECE_COLOR>(rand() % Game::GAME_PIECE_COLOR::COLORMAX + 1);

    Game new_game;

    new_game.add_piece(type, color, make_pair(x, y));

    Game::game_piece piece = new_game.get_location(make_pair(x, y));

    // Validating the piece information before continuing
    if (piece.color != color || piece.type != type) {
        return false;
    }

    // Removing the piece
    new_game.remove_piece(make_pair(x, y));

    piece = new_game.get_location(make_pair(x, y));

    // Validating the piece is no longer at that location
    if (piece.color != Game::GAME_PIECE_COLOR::NOCOLOR || piece.type != Game::GAME_PIECE_TYPE::NOTYPE) {
        return false;
    }

    // As long as the piece isn't there it should have been removed correctly
    return true;
}

// Tests that removing a location with no piece does nothing
bool test_removing_no_piece() {
    int x = rand() % DEFAULT_CHESS_BOARD_SIZE;
    int y = rand() % DEFAULT_CHESS_BOARD_SIZE;

    Game new_game;

    try {
        new_game.remove_piece(make_pair(x, y));
        return true;
    } catch (...) {
        return false;
    }
    
}

// Tests that removing locations out-of-bounds will throw errors
bool test_removing_invalid_locations() {
    // Case where both are above bounds
    int x1 = DEFAULT_CHESS_BOARD_SIZE + 1;
    int y1 = DEFAULT_CHESS_BOARD_SIZE + 1;

    // Case where both are below bounds
    int x2 = -1;
    int y2 = -1;

    // Case where only x is above bounds
    int x3 = DEFAULT_CHESS_BOARD_SIZE + 1;
    int y3 = rand() % DEFAULT_CHESS_BOARD_SIZE;

    // Case where only y is above bounds
    int x4 = rand() % DEFAULT_CHESS_BOARD_SIZE;
    int y4 = DEFAULT_CHESS_BOARD_SIZE + 1;

    // Case where only x is below bounds
    int x5 = -1;
    int y5 = rand() % DEFAULT_CHESS_BOARD_SIZE;

    // Case where only y is below bounds
    int x6 = rand() % DEFAULT_CHESS_BOARD_SIZE;
    int y6 = -1;
    
    Game new_game;

    try {
        new_game.remove_piece(make_pair(x1, y1));
        // If the above did not throw an error then we need to abort and return a false
        return false;
    } catch (runtime_error r) {
        // All good - this means the code is working correctly
    }

    try {
        new_game.remove_piece(make_pair(x2, y2));
        // If the above did not throw an error then we need to abort and return a false
        return false;
    } catch (runtime_error r) {
        // All good - this means the code is working correctly
    }

    try {
        new_game.remove_piece(make_pair(x3, y3));
        // If the above did not throw an error then we need to abort and return a false
        return false;
    } catch (runtime_error r) {
        // All good - this means the code is working correctly
    }

    try {
        new_game.remove_piece(make_pair(x4, y4));
        // If the above did not throw an error then we need to abort and return a false
        return false;
    } catch (runtime_error r) {
        // All good - this means the code is working correctly
    }

    try {
        new_game.remove_piece(make_pair(x5, y5));
        // If the above did not throw an error then we need to abort and return a false
        return false;
    } catch (runtime_error r) {
        // All good - this means the code is working correctly
    }

    try {
        new_game.remove_piece(make_pair(x6, y6));
        // If the above did not throw an error then we need to abort and return a false
        return false;
    } catch (runtime_error r) {
        // All good - this means the code is working correctly
    }

    // If we made it here then all worked as intended
    return true;
}

// Executes all of the unit tests for the Chess object - if any fail it will return an integer to describe the number that failed
int run_game_tests() {
    int errors = 0;

    // Testing instantiation
    try {
        if(!test_create_game()) {
            cout << "   ERROR: An error occured while attempting to create a new game object" << endl;
            ++errors;
        }
    // Case where deallocation was a problem
    } catch(...) {
        cout << "   ERROR: An error occured while the game object was exiting scope" << endl;
        ++errors;
    }


    // Testing stability with many instantiations
    try {
        if(!test_creating_many_games()) {
            cout << "   ERROR: An error occured while attempting to create many game objects" << endl;
            ++errors;
        }
    // Case where deallocation was a problem
    } catch(...) {
        cout << "   ERROR: An unknown error occured while attempting to create many game objects" << endl;
        ++errors;
    }


    // Testing getting a simple location
    try {
        if (!test_getting_rand_location_board_empty()) {
            cout << "   ERROR: The empty board is not full of invalid pieces" << endl;
            ++errors;
        }
    } catch(...) {
        cout << "   ERROR: Something went wrong when attempting to grab a random location on the board" << endl;
        ++errors;
    }


    // Testing that you cannot select outside the bounds
    try {
        if (!test_no_access_to_out_of_bounds()) {
            cout << "   ERROR: The game allowed a selection out-of-bounds" << endl;
            ++errors;
        }
    } catch(...) {
        cout << "   ERROR: Something went wrong when attempting to test bound integrity" << endl;
        ++errors;
    }


    // Testing adding a random piece to the board
    try {
        if (!test_adding_piece_to_board()) {
            cout << "   ERROR: The board was unable to successfully add a new random piece" << endl;
            ++errors;
        }
    } catch(...) {
        cout << "   ERROR: Something went wrong when attempting add the random piece to the board" << endl;
        ++errors;
    }


    // Testing modifying a piece grabbed - ensuring constness from and external source
    try {
        if (!test_constness_get_location()) {
            cout << "   ERROR: The game allowed pieces to be modified externally" << endl;
            ++errors;
        }
    } catch(...) {
        cout << "   ERROR: Something went wrong when attempting to test the constness of game pieces" << endl;
        ++errors;
    }


    // Testing adding a random piece to the board
    try {
        if (!test_adding_piece_to_same_spot()) {
            cout << "   ERROR: The board allowed a piece to be placed on top of another piece" << endl;
            ++errors;
        }
    } catch(...) {
        cout << "   ERROR: Something went wrong when attempting add the piece on top of another piece" << endl;
        ++errors;
    }


    // Testing adding many pieces to the board
    try {
        if (!test_adding_many_pieces()) {
            cout << "   ERROR: An issue occured while attempting to add many pieces to the game board" << endl;
            ++errors;
        }
    } catch(...) {
        cout << "   ERROR: Something unknown went wrong when attempting add many pieces to the board" << endl;
        ++errors;
    }


    // Testing adding pieces to invalid locations on the board
    try {
        if (!test_adding_piece_to_invalid_location()) {
            cout << "   ERROR: The board allowed a piece to be created outside the board bounds" << endl;
            ++errors;
        }
    } catch(...) {
        cout << "   ERROR: Something unknown went wrong when testing the board integrity" << endl;
        ++errors;
    }


    // Testing removing pieces from the game board
    try {
        if (!test_removing_piece()) {
            cout << "   ERROR: The board did not successfully remove the piece" << endl;
            ++errors;
        }
    } catch(...) {
        cout << "   ERROR: Something unknown went wrong when attempting to remove a game piece" << endl;
        ++errors;
    }


    // Testing removing nothing from the game board
    try {
        if (!test_removing_no_piece()) {
            cout << "   ERROR: The game had an issue attempting to remove nothing from the board" << endl;
            ++errors;
        }
    } catch(...) {
        cout << "   ERROR: Something unknown went wrong when attempting to remove nothing" << endl;
        ++errors;
    }
    

    // Testing removing invalid locations from the board
    try {
        if (!test_removing_invalid_locations()) {
            cout << "   ERROR: The game allowed removal of locations outside the bounds" << endl;
            ++errors;
        }
    } catch(...) {
        cout << "   ERROR: Something unknown went wrong when attempting to test removal of invalid locations" << endl;
        ++errors;
    }

    return errors;
}