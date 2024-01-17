#include "Game_test.h"

using namespace Chess_API;
using namespace std;

// Tests creating a basic game object and ensuring there isn't any memory leaks issues - returns true if successful
bool test_create_game() {
    try {
        shared_ptr<Player> player1(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::WHITE));
        shared_ptr<Player> player2(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::BLACK));
        Game * new_game = new Game(player1, player2);
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
            shared_ptr<Player> player1(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::WHITE));
            shared_ptr<Player> player2(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::BLACK));
            Game * game = new Game(player1, player2);
            game_array[i] = game;
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

    shared_ptr<Player> player1(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::WHITE));
    shared_ptr<Player> player2(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::BLACK));
    Game new_game(player1, player2);

    game_piece piece = new_game.get_location(make_pair(x, y));

    // Ensuring this location is an invalid piece - aka 'empty'
    if (piece.type != GAME_PIECE_TYPE::NOTYPE || piece.color != GAME_PIECE_COLOR::NOCOLOR) {
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
    
    shared_ptr<Player> player1(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::WHITE));
    shared_ptr<Player> player2(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::BLACK));
    Game new_game(player1, player2);

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

    // Casting a random piece type
    GAME_PIECE_TYPE type = static_cast<GAME_PIECE_TYPE>(rand() % GAME_PIECE_TYPE::TYPEMAX + 1);
    GAME_PIECE_COLOR color = GAME_PIECE_COLOR::BLACK;

    shared_ptr<Player> player1(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::WHITE));
    shared_ptr<Player> player2(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::BLACK));
    Game new_game(player1, player2);

    // Adding the piece
    new_game.add_piece(type, color, make_pair(x, y));

    // Grabbing the piece to ensure persistence
    game_piece piece = new_game.get_location(make_pair(x, y));

    // Ensuring the piece persisted
    if (piece.color == GAME_PIECE_COLOR::NOCOLOR && piece.type == GAME_PIECE_TYPE::NOTYPE) {
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

    // Casting a random piece type
    GAME_PIECE_TYPE type1 = static_cast<GAME_PIECE_TYPE>(rand() % GAME_PIECE_TYPE::TYPEMAX + 1);
    GAME_PIECE_COLOR color1 = GAME_PIECE_COLOR::BLACK;

    GAME_PIECE_TYPE type2 = static_cast<GAME_PIECE_TYPE>(rand() % GAME_PIECE_TYPE::TYPEMAX + 1);
    GAME_PIECE_COLOR color2 = GAME_PIECE_COLOR::BLACK;

    // Ensuring the attributes are different for testing changes
    while (true) {
        bool types_differ = false;
        if (type2 == type1) {
            type2 = static_cast<GAME_PIECE_TYPE>(rand() % GAME_PIECE_TYPE::TYPEMAX + 1);
        } else {
            types_differ = true;
        }

        bool colors_differ = false;
        if (color2 == color1) {
            color2 = static_cast<GAME_PIECE_COLOR>(rand() % GAME_PIECE_COLOR::COLORMAX + 1);
        } else {
            colors_differ = true;
        }

        if (colors_differ && types_differ) {
            break;
        }
    }

    shared_ptr<Player> player1(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::WHITE));
    shared_ptr<Player> player2(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::BLACK));
    Game new_game(player1, player2);

    // Adding the piece
    new_game.add_piece(type1, color1, make_pair(x, y));

    // Grabbing the piece for testing changes made to it
    game_piece piece1 = new_game.get_location(make_pair(x, y));

    // Applying changes to what should be a copy of the piece grabbed
    piece1.color = color2;
    piece1.type = type2;

    // Grabbing the piece again to ensure that the changes did not persist to the original
    game_piece piece2 = new_game.get_location(make_pair(x, y));
    
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
    GAME_PIECE_TYPE type1 = static_cast<GAME_PIECE_TYPE>(rand() % GAME_PIECE_TYPE::TYPEMAX + 1);
    GAME_PIECE_COLOR color1 = GAME_PIECE_COLOR::BLACK;

    GAME_PIECE_TYPE type2 = static_cast<GAME_PIECE_TYPE>(rand() % GAME_PIECE_TYPE::TYPEMAX + 1);
    GAME_PIECE_COLOR color2 = GAME_PIECE_COLOR::BLACK;

    shared_ptr<Player> player1(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::WHITE));
    shared_ptr<Player> player2(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::BLACK));
    Game new_game(player1, player2);

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
        GAME_PIECE_COLOR color;
        GAME_PIECE_TYPE type;

        location_info(pair<int, int> location_in, GAME_PIECE_COLOR color_in, GAME_PIECE_TYPE type_in) :
                    location(location_in), color(color_in), type(type_in) {}
    };

    vector<location_info> location_info_vec;
    
    shared_ptr<Player> player1(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::WHITE));
    shared_ptr<Player> player2(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::BLACK));
    Game new_game(player1, player2);

    // Fetching random components for pieces and placing them on the board
    try {
        for (int i = 0; i < number_of_pieces; ++i) {
            int x = rand() % DEFAULT_CHESS_BOARD_SIZE;
            int y = rand() % DEFAULT_CHESS_BOARD_SIZE;

            // Casting a random piece type
            GAME_PIECE_TYPE type = static_cast<GAME_PIECE_TYPE>(rand() % GAME_PIECE_TYPE::TYPEMAX + 1);
            GAME_PIECE_COLOR color = GAME_PIECE_COLOR::BLACK;

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
        game_piece piece;
        for (int i = 0; i < location_info_vec.size(); ++i) {
            piece = new_game.get_location(location_info_vec[i].location);

            // Ensuring the piece persisted
            if (piece.color == GAME_PIECE_COLOR::NOCOLOR && piece.type == GAME_PIECE_TYPE::NOTYPE) {
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

    // Casting a random piece type
    GAME_PIECE_TYPE type = static_cast<GAME_PIECE_TYPE>(rand() % GAME_PIECE_TYPE::TYPEMAX + 1);
    GAME_PIECE_COLOR color = GAME_PIECE_COLOR::BLACK;

    shared_ptr<Player> player1(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::WHITE));
    shared_ptr<Player> player2(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::BLACK));
    Game new_game(player1, player2);

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

    // Casting a random piece type
    GAME_PIECE_TYPE type = static_cast<GAME_PIECE_TYPE>(rand() % GAME_PIECE_TYPE::TYPEMAX + 1);
    GAME_PIECE_COLOR color = GAME_PIECE_COLOR::BLACK;

    shared_ptr<Player> player1(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::WHITE));
    shared_ptr<Player> player2(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::BLACK));
    Game new_game(player1, player2);

    new_game.add_piece(type, color, make_pair(x, y));

    game_piece piece = new_game.get_location(make_pair(x, y));

    // Validating the piece information before continuing
    if (piece.color != color || piece.type != type) {
        return false;
    }

    // Removing the piece
    new_game.remove_piece(make_pair(x, y));

    piece = new_game.get_location(make_pair(x, y));

    // Validating the piece is no longer at that location
    if (piece.color != GAME_PIECE_COLOR::NOCOLOR || piece.type != GAME_PIECE_TYPE::NOTYPE) {
        return false;
    }

    // As long as the piece isn't there it should have been removed correctly
    return true;
}

// Tests that removing a location with no piece does nothing
bool test_removing_no_piece() {
    int x = rand() % DEFAULT_CHESS_BOARD_SIZE;
    int y = rand() % DEFAULT_CHESS_BOARD_SIZE;

    shared_ptr<Player> player1(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::WHITE));
    shared_ptr<Player> player2(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::BLACK));
    Game new_game(player1, player2);

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
    
    shared_ptr<Player> player1(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::WHITE));
    shared_ptr<Player> player2(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::BLACK));
    Game new_game(player1, player2);

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

// Tests the assignment operator for deep copying the game object
bool test_assignment_operator() {
    int x = rand() % DEFAULT_CHESS_BOARD_SIZE;
    int y = rand() % DEFAULT_CHESS_BOARD_SIZE;

    // Casting a random piece type
    GAME_PIECE_TYPE type = static_cast<GAME_PIECE_TYPE>(rand() % GAME_PIECE_TYPE::TYPEMAX + 1);
    GAME_PIECE_COLOR color = GAME_PIECE_COLOR::BLACK;

    shared_ptr<Player> player1(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::WHITE));
    shared_ptr<Player> player2(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::BLACK));
    Game new_game(player1, player2);

    new_game.add_piece(type, color, make_pair(x, y));

    Game copy_game = new_game;

    game_piece piece = copy_game.get_location(make_pair(x, y));

    // The piece should have persisted to the copy
    if (piece.color == color && piece.type == type) {
        return true;
    } else {
        return false;
    }
}

// Tests the removal of a copied piece from the copy game - ensures persistence on the original
bool test_assignment_operator_piece_removal() {
    int x = rand() % DEFAULT_CHESS_BOARD_SIZE;
    int y = rand() % DEFAULT_CHESS_BOARD_SIZE;

    // Casting a random piece type
    GAME_PIECE_TYPE type = static_cast<GAME_PIECE_TYPE>(rand() % GAME_PIECE_TYPE::TYPEMAX + 1);
    GAME_PIECE_COLOR color = GAME_PIECE_COLOR::BLACK;

    shared_ptr<Player> player1(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::WHITE));
    shared_ptr<Player> player2(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::BLACK));
    Game new_game(player1, player2);

    new_game.add_piece(type, color, make_pair(x, y));

    Game copy_game = new_game;

    // Removing copied piece
    copy_game.remove_piece(make_pair(x, y));

    game_piece piece = new_game.get_location(make_pair(x, y));

    if (piece.color == color && piece.type == type) {
        return true;
    } else {
        return false;
    }
}

// Tests the copy constructor to ensure deep coping inside of functions
bool test_copy_constructor() {
    int x = rand() % DEFAULT_CHESS_BOARD_SIZE;
    int y = rand() % DEFAULT_CHESS_BOARD_SIZE;

    // Casting a random piece type
    GAME_PIECE_TYPE type = static_cast<GAME_PIECE_TYPE>(rand() % GAME_PIECE_TYPE::TYPEMAX + 1);
    GAME_PIECE_COLOR color = GAME_PIECE_COLOR::BLACK;

    shared_ptr<Player> player1(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::WHITE));
    shared_ptr<Player> player2(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::BLACK));
    Game new_game(player1, player2);

    new_game.add_piece(type, color, make_pair(x, y));

    // Helper function to copy over the game data and determine if it is a deep copy or not
    auto helper_func = [type, color, x, y](Game game_copy) {
        game_piece piece = game_copy.get_location(make_pair(x, y));

        if (piece.color == color && piece.type == type) {
            return true;
        } else {
            return false;
        }
    };

    if (helper_func(new_game)) {
        return true;
    } else {
        return false;
    }
}

// Tests removal of a piece from a copied version - ensures persistence on the original
bool test_copy_constructor_removal() {
    int x = rand() % DEFAULT_CHESS_BOARD_SIZE;
    int y = rand() % DEFAULT_CHESS_BOARD_SIZE;

    // Casting a random piece type and color
    GAME_PIECE_TYPE type = static_cast<GAME_PIECE_TYPE>(rand() % GAME_PIECE_TYPE::TYPEMAX + 1);
    GAME_PIECE_COLOR color = GAME_PIECE_COLOR::BLACK;

    shared_ptr<Player> player1(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::WHITE));
    shared_ptr<Player> player2(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::BLACK));
    Game new_game(player1, player2);

    new_game.add_piece(type, color, make_pair(x, y));

    // Helper function to copy over the game data
    auto helper_func = [type, color, x, y](Game game_copy) {
        game_copy.remove_piece(make_pair(x, y));

        game_piece piece = game_copy.get_location(make_pair(x, y));

        // Ensuring the copies has been deleted
        if (piece.color == GAME_PIECE_COLOR::NOCOLOR && piece.type == GAME_PIECE_TYPE::NOTYPE) {
            return true;
        } else {
            return false;
        }
    };

    if (helper_func(new_game)) {
        game_piece piece = new_game.get_location(make_pair(x, y));
        // Ensuring persistence in the original copy
        if (piece.color == color && piece.type == type) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

// Tests that is_valid_move returns false if there is no starting piece
bool test_is_valid_move_start_piece() {
    int x = rand() % DEFAULT_CHESS_BOARD_SIZE;
    int y = rand() % DEFAULT_CHESS_BOARD_SIZE;

    shared_ptr<Player> player1(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::WHITE));
    shared_ptr<Player> player2(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::BLACK));
    Game new_game(player1, player2);

    if (new_game.is_valid_move(make_pair(x, y), make_pair(x, y))) {
        return false;
    } else {
        return true;
    }
}

// Tests that is_valid_move returns false if there is a piece on the end position of the same color
bool test_is_valid_move_end_piece() {
    int x_start = rand() % DEFAULT_CHESS_BOARD_SIZE;
    int y_start = rand() % DEFAULT_CHESS_BOARD_SIZE;

    int x_end = rand() % DEFAULT_CHESS_BOARD_SIZE;
    int y_end = rand() % DEFAULT_CHESS_BOARD_SIZE;

    // Casting a random piece type
    GAME_PIECE_TYPE type = static_cast<GAME_PIECE_TYPE>(rand() % GAME_PIECE_TYPE::TYPEMAX + 1);
    GAME_PIECE_COLOR color = GAME_PIECE_COLOR::BLACK;

    shared_ptr<Player> player1(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::WHITE));
    shared_ptr<Player> player2(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::BLACK));
    Game new_game(player1, player2);

    new_game.add_piece(type, color, make_pair(x_start, y_start));
    new_game.add_piece(type, color, make_pair(x_end, y_end));

    if (new_game.is_valid_move(make_pair(x_start, y_start), make_pair(x_end, y_end))) {
        return false;
    } else {
        return true;
    }
}

// Tests legal moves for a Queen - the queen acts as a valid test for all unrestricted pieces
bool test_is_legal_move_queen() {
    // Hardcoded for discrete testing purposes
    int x_start = 4;
    int y_start = 4;
    
    // Moving one space diagonally
    int x_end1 = 5;
    int y_end1 = 5;

    int x_end2 = 3;
    int y_end2 = 5;

    int x_end3 = 5;
    int y_end3 = 3;
    
    int x_end4 = 3;
    int y_end4 = 3;

    // Moving one space vertically
    int x_end5 = 5;
    int y_end5 = 4;

    int x_end6 = 3;
    int y_end6 = 4;
    
    // Moving one space horizontally
    int x_end7 = 4;
    int y_end7 = 5;

    int x_end8 = 4;
    int y_end8 = 3;

    // Moving many spaces diagonally
    int x_end9 = 7;
    int y_end9 = 7;

    int x_end10 = 1;
    int y_end10 = 7;

    int x_end11 = 7;
    int y_end11 = 1;
    
    int x_end12 = 1;
    int y_end12 = 1;

    // Moving many spaces vertically
    int x_end13 = 7;
    int y_end13 = 4;

    int x_end14 = 1;
    int y_end14 = 4;

    // Moving many spaces horizontally
    int x_end15 = 4;
    int y_end15 = 7;

    int x_end16 = 4;
    int y_end16 = 1;

    shared_ptr<Player> player1(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::WHITE));
    shared_ptr<Player> player2(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::BLACK));
    Game new_game(player1, player2);

    new_game.add_piece(GAME_PIECE_TYPE::QUEEN, GAME_PIECE_COLOR::WHITE, make_pair(x_start, y_start));

    if(!new_game.is_legal_move(make_pair(x_start, y_start), make_pair(x_end1, y_end1))) {
        return false;
    }

    if(!new_game.is_legal_move(make_pair(x_start, y_start), make_pair(x_end2, y_end2))) {
        return false;
    }

    if(!new_game.is_legal_move(make_pair(x_start, y_start), make_pair(x_end3, y_end3))) {
        return false;
    }

    if(!new_game.is_legal_move(make_pair(x_start, y_start), make_pair(x_end4, y_end4))) {
        return false;
    }

    if(!new_game.is_legal_move(make_pair(x_start, y_start), make_pair(x_end5, y_end5))) {
        return false;
    }

    if(!new_game.is_legal_move(make_pair(x_start, y_start), make_pair(x_end6, y_end6))) {
        return false;
    }

    if(!new_game.is_legal_move(make_pair(x_start, y_start), make_pair(x_end7, y_end7))) {
        return false;
    }

    if(!new_game.is_legal_move(make_pair(x_start, y_start), make_pair(x_end8, y_end8))) {
        return false;
    }

    if(!new_game.is_legal_move(make_pair(x_start, y_start), make_pair(x_end9, y_end9))) {
        return false;
    }

    if(!new_game.is_legal_move(make_pair(x_start, y_start), make_pair(x_end10, y_end10))) {
        return false;
    }

    if(!new_game.is_legal_move(make_pair(x_start, y_start), make_pair(x_end11, y_end11))) {
        return false;
    }

    if(!new_game.is_legal_move(make_pair(x_start, y_start), make_pair(x_end12, y_end12))) {
        return false;
    }

    if(!new_game.is_legal_move(make_pair(x_start, y_start), make_pair(x_end13, y_end13))) {
        return false;
    }

    if(!new_game.is_legal_move(make_pair(x_start, y_start), make_pair(x_end14, y_end14))) {
        return false;
    }

    if(!new_game.is_legal_move(make_pair(x_start, y_start), make_pair(x_end15, y_end15))) {
        return false;
    }

    if(!new_game.is_legal_move(make_pair(x_start, y_start), make_pair(x_end16, y_end16))) {
        return false;
    }

    // After testing each hardcoded positions - return true if all passed
    return true;
}

// Tests illegal moves for a Queen - the queen acts as a valid test for all unrestricted pieces
bool test_is_illegal_move_queen() {
    // Hardcoded for discrete testing purposes
    int x_start = 0;
    int y_start = 0;
    
    // Moving one space higher than diagonal
    int x_end1 = 6;
    int y_end1 = 5;

    // Moving very far horizontal with slight vertical
    int x_end2 = 1;
    int y_end2 = 7;

    // Move like a knight
    int x_end3 = 2;
    int y_end3 = 1;
    
    shared_ptr<Player> player1(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::WHITE));
    shared_ptr<Player> player2(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::BLACK));
    Game new_game(player1, player2);

    new_game.add_piece(GAME_PIECE_TYPE::QUEEN, GAME_PIECE_COLOR::WHITE, make_pair(x_start, y_start));

    if (new_game.is_legal_move(make_pair(x_start, y_start), make_pair(x_end1, y_end1))) {
        return false;
    }

    if (new_game.is_legal_move(make_pair(x_start, y_start), make_pair(x_end2, y_end2))) {
        return false;
    }

    if (new_game.is_legal_move(make_pair(x_start, y_start), make_pair(x_end3, y_end3))) {
        return false;
    }

    // After testing each hardcoded positions - return true if all passed
    return true;
}

// Use only when messing with the display settings - not an important unit test
void test_display() {
    shared_ptr<Player> player1(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::WHITE));
    shared_ptr<Player> player2(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::BLACK));
    Game new_game(player1, player2);
    new_game.show_board();
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
    } catch(exception e) {
        cout << "   ERROR: test_create_game threw an error: " << e.what() << endl;
        ++errors;
    }

    
    // Testing stability with many instantiations
    try {
        if(!test_creating_many_games()) {
            cout << "   ERROR: An error occured while attempting to create many game objects" << endl;
            ++errors;
        }
    // Case where deallocation was a problem
    } catch(exception e) {
        cout << "   ERROR: test_creating_many_games threw an error: " << e.what() << endl;
        ++errors;
    }
    

    // Testing getting a simple location
    try {
        if (!test_getting_rand_location_board_empty()) {
            cout << "   ERROR: The empty board is not full of invalid pieces" << endl;
            ++errors;
        }
    } catch(exception e) {
        cout << "   ERROR: test_getting_rand_location_board_empty threw an error: " << e.what() << endl;
        ++errors;
    }


    // Testing that you cannot select outside the bounds
    try {
        if (!test_no_access_to_out_of_bounds()) {
            cout << "   ERROR: The game allowed a selection out-of-bounds" << endl;
            ++errors;
        }
    } catch(exception e) {
        cout << "   ERROR: test_no_access_to_out_of_bounds threw an error: " << e.what() << endl;
        ++errors;
    }

    
    // Testing adding a random piece to the board
    try {
        if (!test_adding_piece_to_board()) {
            cout << "   ERROR: The board was unable to successfully add a new random piece" << endl;
            ++errors;
        }
    } catch(exception e) {
        cout << "   ERROR: test_adding_piece_to_board threw an error: " << e.what() << endl;
        ++errors;
    }


    // Testing modifying a piece grabbed - ensuring constness from and external source
    try {
        if (!test_constness_get_location()) {
            cout << "   ERROR: The game allowed pieces to be modified externally" << endl;
            ++errors;
        }
    } catch(exception e) {
        cout << "   ERROR: test_constness_get_location threw an error: " << e.what() << endl;
        ++errors;
    }


    // Testing adding a random piece to the board
    try {
        if (!test_adding_piece_to_same_spot()) {
            cout << "   ERROR: The board allowed a piece to be placed on top of another piece" << endl;
            ++errors;
        }
    } catch(exception e) {
        cout << "   ERROR: test_adding_piece_to_same_spot threw an error: " << e.what() << endl;
        ++errors;
    }


    // Testing adding many pieces to the board
    try {
        if (!test_adding_many_pieces()) {
            cout << "   ERROR: An issue occured while attempting to add many pieces to the game board" << endl;
            ++errors;
        }
    } catch(exception e) {
        cout << "   ERROR: test_adding_many_pieces threw an error: " << e.what() << endl;
        ++errors;
    }


    // Testing adding pieces to invalid locations on the board
    try {
        if (!test_adding_piece_to_invalid_location()) {
            cout << "   ERROR: The board allowed a piece to be created outside the board bounds" << endl;
            ++errors;
        }
    } catch(exception e) {
        cout << "   ERROR: test_adding_piece_to_invalid_location threw an error: " << e.what() << endl;
        ++errors;
    }


    // Testing removing pieces from the game board
    try {
        if (!test_removing_piece()) {
            cout << "   ERROR: The board did not successfully remove the piece" << endl;
            ++errors;
        }
    } catch(exception e) {
        cout << "   ERROR: test_removing_piece threw an error: " << e.what() << endl;
        ++errors;
    }


    // Testing removing nothing from the game board
    try {
        if (!test_removing_no_piece()) {
            cout << "   ERROR: The game had an issue attempting to remove nothing from the board" << endl;
            ++errors;
        }
    } catch(exception e) {
        cout << "   ERROR: test_removing_no_piece threw an error: " << e.what() << endl;
        ++errors;
    }
    

    // Testing removing invalid locations from the board
    try {
        if (!test_removing_invalid_locations()) {
            cout << "   ERROR: The game allowed removal of locations outside the bounds" << endl;
            ++errors;
        }
    } catch(exception e) {
        cout << "   ERROR: test_removing_invalid_locations threw an error: " << e.what() << endl;
        ++errors;
    }


    // Testing the assignment operator to ensure deep copying
    try {
        if (!test_assignment_operator() ) {
            cout << "   ERROR: The game assignment operator did not assign deep data" << endl;
            ++errors;
        }
    } catch(exception e) {
        cout << "   ERROR: test_assignment_operator threw an error: " << e.what() << endl;
        ++errors;
    }


    // Testing the assignment operator to ensure separate data - removal on one copy does not effect the original
    try {
        if (!test_assignment_operator_piece_removal()) {
            cout << "   ERROR: The game assignment operator does not separate the data" << endl;
            ++errors;
        }
    } catch(exception e) {
        cout << "   ERROR: test_assignment_operator_piece_removal threw an error: " << e.what() << endl;
        ++errors;
    }


    // Testing the copy constructor to ensure deep copying
    try {
        if (!test_copy_constructor()) {
            cout << "   ERROR: The game copy constructor does not perform a deep copy correctly" << endl;
            ++errors;
        }
    } catch(exception e) {
        cout << "   ERROR: test_copy_constructor threw an error: " << e.what() << endl;
        ++errors;
    }

    

    // Testing the copy constructor to ensure separation of data
    try {
        if (!test_copy_constructor_removal()) {
            cout << "   ERROR: The game copy constructor does not partion separate memory for game copies" << endl;
            ++errors;
        }
    } catch(exception e) {
        cout << "   ERROR: test_copy_constructor_removal threw an error: " << e.what() << endl;
        ++errors;
    }


    // Testing is_valid_move for starting piece required
    try {
        if (!test_is_valid_move_start_piece()) {
            cout << "   ERROR: The game does not require a starting piece for a move to be valid" << endl;
            ++errors;
        }
    } catch(exception e) {
        cout << "   ERROR: test_is_valid_move_start_piece threw an error: " << e.what() << endl;
        ++errors;
    }


    // Testing is_valid_move for ending piece being not the same color
    try {
        if (!test_is_valid_move_end_piece()) {
            cout << "   ERROR: The game does not require an ending piece to be a different color for a valid move" << endl;
            ++errors;
        }
    } catch(exception e) {
        cout << "   ERROR: test_is_valid_move_end_piece threw an error: " << e.what() << endl;
        ++errors;
    }


    // Testing is_legal_move for legal queen moves
    try {
        if (!test_is_legal_move_queen()) {
            cout << "   ERROR: The game does not correctly register correct moves for the queen" << endl;
            ++errors;
        }
    } catch(exception e) {
        cout << "   ERROR: test_is_legal_move_queen threw an error: " << e.what() << endl;
        ++errors;
    }

    
    // Testing is_legal_move for illegal queen moves
    try {
        if (!test_is_illegal_move_queen()) {
            cout << "   ERROR: The game does not correctly register incorrect moves for the queen" << endl;
            ++errors;
        }
    } catch(exception e) {
        cout << "   ERROR: test_is_illegal_move_queen threw an error: " << e.what() << endl;
        ++errors;
    }

    // Temporary test to see the display
    test_display();


    return errors;
}