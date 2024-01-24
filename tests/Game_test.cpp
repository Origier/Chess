#include "Game_test.h"

using namespace Chess_API;
using namespace std;

// Helper test function - determines if a move is valid and then returns useful information regarding the validity
std::pair<bool, std::string> check_move_error_codes(Game& game_in, const std::pair<int, int> start_pos, const std::pair<int, int> end_pos) {
    Game::MOVE_ERROR_CODE error_code = game_in.is_valid_move(start_pos, end_pos);
    if (error_code == Game::MOVE_ERROR_CODE::VALID_MOVE) {
        return std::make_pair(true, "Valid move");
    } else {
        if (error_code == Game::MOVE_ERROR_CODE::BLOCKED_MOVE) {
            return std::make_pair(false, "There is a piece of the same color in the way");
        }

        if (error_code == Game::MOVE_ERROR_CODE::CHECK_MOVE) {
            return std::make_pair(false, "That move will put the current player in check");
        }

        if (error_code == Game::MOVE_ERROR_CODE::ILLEGAL_MOVE) {
            return std::make_pair(false, "That piece isn't allowed to make that move");
        }

        if (error_code == Game::MOVE_ERROR_CODE::NO_PIECE) {
            return std::make_pair(false, "There is no piece at the starting position");
        }

        if (error_code == Game::MOVE_ERROR_CODE::OUT_OF_BOUNDS_MOVE) {
            return std::make_pair(false, "That move is outside the bounds of the board");
        }

        if (error_code == Game::MOVE_ERROR_CODE::WRONG_PLAYER) {
            return std::make_pair(false, "That isn't the current players piece");
        }
    }
}

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

// Tests all moves for pawns
bool test_pawn_valid_moves() {
    shared_ptr<Player> player1(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::WHITE));
    shared_ptr<Player> player2(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::BLACK));
    Game new_game(player1, player2);
    new_game.setup_default_board_state();

    // Test D2 - D3 - simple case single move
    std::pair<int, int> player_move_start = std::make_pair(1, 3);
    std::pair<int, int> player_move_end = std::make_pair(2, 3);
    
    std::pair<bool, std::string> error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The first pawn move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }

    // Test D3 - D4 - single move after a first move
    player_move_start = std::make_pair(2, 3);
    player_move_end = std::make_pair(3, 3);
    
    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The second pawn move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }

    // Test C2 - C4 - double move on first move
    player_move_start = std::make_pair(1, 2);
    player_move_end = std::make_pair(3, 2);
    
    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The third pawn move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }

    // Test C4 - C5 - single move after a double move - setup for en passant test
    player_move_start = std::make_pair(3, 2);
    player_move_end = std::make_pair(4, 2);
    
    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The fourth pawn move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }

    // Setup for capture tests - tests swapping current player to ensure both colors may play pawns
    new_game.swap_current_player();

    // D7 - D5 - setup for en passant from white
    player_move_start = std::make_pair(6, 3);
    player_move_end = std::make_pair(4, 3);
    
    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The fifth pawn move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }

    new_game.swap_current_player();

    // Test C5 - D6 - en passant capture on D5
    player_move_start = std::make_pair(4, 2);
    player_move_end = std::make_pair(5, 3);
    
    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The sixth pawn move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }

    // Test D6 - E7 - normal pawn capture
    player_move_start = std::make_pair(5, 3);
    player_move_end = std::make_pair(6, 4);
    
    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The seventh pawn move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }
    
    // Full test complete - return true for passing all seven tested moves
    return true;
}

// Tests all moves for pawns that would be invalid
bool test_pawn_invalid_moves() {
    shared_ptr<Player> player1(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::WHITE));
    shared_ptr<Player> player2(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::BLACK));
    Game new_game(player1, player2);
    new_game.setup_default_board_state();

    // Test D2 - D3 - setup to attempt moving backwards
    std::pair<int, int> player_move_start = std::make_pair(1, 3);
    std::pair<int, int> player_move_end = std::make_pair(2, 3);
    
    std::pair<bool, std::string> error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The first pawn move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }

    // Testing moving backwards
    player_move_start = std::make_pair(2, 3);
    player_move_end = std::make_pair(1, 3);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != false) {
        false;
    }

    // Testing a double move after the first move
    player_move_start = std::make_pair(2, 3);
    player_move_end = std::make_pair(4, 3);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != false) {
        false;
    }

    // Testing a diagonal move without any unit to capture and no valid en passant location
    player_move_start = std::make_pair(2, 3);
    player_move_end = std::make_pair(3, 4);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != false) {
        false;
    }

    // Switching to the other player to test a couple more moves
    new_game.swap_current_player();

    player_move_start = std::make_pair(6, 7);
    player_move_end = std::make_pair(4, 7);
    
    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The second pawn move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }

    // Testing moving backwards for this player
    player_move_start = std::make_pair(4, 7);
    player_move_end = std::make_pair(5, 7);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != false) {
        false;
    }

    // Testing moving a second double move after the first
    player_move_start = std::make_pair(4, 7);
    player_move_end = std::make_pair(2, 7);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != false) {
        false;
    }

    // Testing moving out of bounds
    player_move_start = std::make_pair(4, 7);
    player_move_end = std::make_pair(5, 8);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != false) {
        false;
    }

    // Setup for final test
    player_move_start = std::make_pair(4, 7);
    player_move_end = std::make_pair(3, 7);
    
    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The third pawn move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }

    player_move_start = std::make_pair(3, 7);
    player_move_end = std::make_pair(2, 7);
    
    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The third pawn move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }

    // Testing moving forward onto a piece
    player_move_start = std::make_pair(2, 7);
    player_move_end = std::make_pair(1, 7);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != false) {
        false;
    }

    return true;
}

// Tests all moves for kings
bool test_king_valid_moves() {
    shared_ptr<Player> player1(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::WHITE));
    shared_ptr<Player> player2(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::BLACK));
    Game new_game(player1, player2);
    new_game.setup_default_board_state();

    // Moving other pieces out of the way for the king
    // D2 - D4
    std::pair<int, int> player_move_start = std::make_pair(1, 3);
    std::pair<int, int> player_move_end = std::make_pair(3, 3);

    std::pair<bool, std::string> error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The first move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }

    // C1 - F4
    player_move_start = std::make_pair(0, 2);
    player_move_end = std::make_pair(3, 5);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The second move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }

    // B1 - C3
    player_move_start = std::make_pair(0, 1);
    player_move_end = std::make_pair(2, 2);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The third move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }

    // D1 - D2
    player_move_start = std::make_pair(0, 3);
    player_move_end = std::make_pair(1, 3);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The fourth move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }


    // Testing Castle move to the left
    player_move_start = std::make_pair(0, 4);
    player_move_end = std::make_pair(0, 2);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The fifth move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }


    // Swapping players to test the other side
    new_game.swap_current_player();

    // E7 - E5
    player_move_start = std::make_pair(6, 4);
    player_move_end = std::make_pair(4, 4);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The sixth move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }

    // F8 - C5
    player_move_start = std::make_pair(7, 5);
    player_move_end = std::make_pair(4, 2);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The seventh move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }

    // G8 - H6
    player_move_start = std::make_pair(7, 6);
    player_move_end = std::make_pair(5, 7);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The eigth move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }

    // Testing Castle move to the right
    player_move_start = std::make_pair(7, 4);
    player_move_end = std::make_pair(7, 6);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The nineth move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }

    // Setup for further moves
    // G7 - G5
    player_move_start = std::make_pair(6, 6);
    player_move_end = std::make_pair(4, 6);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The tenth move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }

    // Test single verticle move negative x
    player_move_start = std::make_pair(7, 6);
    player_move_end = std::make_pair(6, 6);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The eleventh move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }

    // Test single diagonal move 
    player_move_start = std::make_pair(6, 6);
    player_move_end = std::make_pair(5, 5);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The twelth move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }

    // Test single horizontal move 
    player_move_start = std::make_pair(5, 5);
    player_move_end = std::make_pair(5, 4);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The thirteenth move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }

    // Test single horizontal move back 
    player_move_start = std::make_pair(5, 4);
    player_move_end = std::make_pair(5, 5);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The fourteenth move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }

    // Test single reverse diagonal move 
    player_move_start = std::make_pair(5, 5);
    player_move_end = std::make_pair(6, 6);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The fifteenth move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }

    // Test single verticle move positive x
    player_move_start = std::make_pair(6, 6);
    player_move_end = std::make_pair(7, 6);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The sixteenth move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }

    // Having tested both castles and a move in each direction and back this concludes testing valid moves for the king
    return true;
}

// Tests all moves for kings that would be invalid
bool test_king_invalid_moves() {
    shared_ptr<Player> player1(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::WHITE));
    shared_ptr<Player> player2(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::BLACK));
    Game new_game(player1, player2);
    new_game.setup_default_board_state();

    // Testing if the king can move onto another piece
    std::pair<int, int> player_move_start = std::make_pair(0, 4);
    std::pair<int, int> player_move_end = std::make_pair(1, 4);

    std::pair<bool, std::string> error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != false) {
        return false;
    }

    // Moving pieces out of the way
    // D2 - D4
    player_move_start = std::make_pair(1, 3);
    player_move_end = std::make_pair(3, 3);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The first move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }

    // C1 - F4
    player_move_start = std::make_pair(0, 2);
    player_move_end = std::make_pair(3, 5);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The second move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }


    // B1 - C3
    player_move_start = std::make_pair(0, 1);
    player_move_end = std::make_pair(2, 2);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The third move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }

    // D1 - D2
    player_move_start = std::make_pair(0, 3);
    player_move_end = std::make_pair(1, 3);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The fourth move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }


    // Testing moving the king once before attempting to castle
    player_move_start = std::make_pair(0, 4);
    player_move_end = std::make_pair(0, 3);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The fifth move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }
    
    player_move_start = std::make_pair(0, 3);
    player_move_end = std::make_pair(0, 4);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The sixth move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }

    player_move_start = std::make_pair(0, 4);
    player_move_end = std::make_pair(0, 2);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != false) {
        return false;
    }


    // Swapping players to test the other side
    new_game.swap_current_player();

    // E7 - E5
    player_move_start = std::make_pair(6, 4);
    player_move_end = std::make_pair(4, 4);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The seventh move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }

    // F8 - C5
    player_move_start = std::make_pair(7, 5);
    player_move_end = std::make_pair(4, 2);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The eigth move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }

    // G8 - H6
    player_move_start = std::make_pair(7, 6);
    player_move_end = std::make_pair(5, 7);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The nineth move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }


    // Testing moving the rook once before attempting to castle
    player_move_start = std::make_pair(7, 7);
    player_move_end = std::make_pair(7, 6);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The tenth move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }

    player_move_start = std::make_pair(7, 6);
    player_move_end = std::make_pair(7, 7);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The eleventh move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }

    player_move_start = std::make_pair(7, 4);
    player_move_end = std::make_pair(7, 6);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != false) {
        return false;
    }

    // Testing that the king cannot more than one space in any direction
    player_move_start = std::make_pair(7, 4);
    player_move_end = std::make_pair(5, 4);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != false) {
        return false;
    }

    return true;
}

// Tests all moves for the knight - acts as test completeness for restricted pieces
bool test_knight_valid_moves() {
    shared_ptr<Player> player1(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::WHITE));
    shared_ptr<Player> player2(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::BLACK));
    Game new_game(player1, player2);
    new_game.setup_default_board_state();

    // Test first style move B1 - C3 while jumping over allies
    std::pair<int, int> player_move_start = std::make_pair(0, 1);
    std::pair<int, int> player_move_end = std::make_pair(2, 2);

    std::pair<bool, std::string> error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The first move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    } 

    // Test second style move C3 - E4
    player_move_start = std::make_pair(2, 2);
    player_move_end = std::make_pair(3, 4);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The second move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    } 

    // E4 - D6
    player_move_start = std::make_pair(3, 4);
    player_move_end = std::make_pair(5, 3);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The third move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    } 

    // Test Capture - leaping over enemy units
    player_move_start = std::make_pair(5, 3);
    player_move_end = std::make_pair(7, 2);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The forth move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    } 

    return true;
}

// Tests all moves for the knight that would be invalid
bool test_knight_invalid_moves() {
    shared_ptr<Player> player1(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::WHITE));
    shared_ptr<Player> player2(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::BLACK));
    Game new_game(player1, player2);
    new_game.setup_default_board_state();

    // Test landing on an ally
    std::pair<int, int> player_move_start = std::make_pair(0, 1);
    std::pair<int, int> player_move_end = std::make_pair(1, 3);

    std::pair<bool, std::string> error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if(error_values.first != false) {
        return false;
    }

    // Test jumping out of bounds
    player_move_start = std::make_pair(0, 1);
    player_move_end = std::make_pair(1, -1);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if(error_values.first != false) {
        return false;
    }

    // Test a non-knight move
    player_move_start = std::make_pair(0, 1);
    player_move_end = std::make_pair(4, 1);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if(error_values.first != false) {
        return false;
    }

    return true;
}

// Tests all moves for the queen - acts as test completeness for unrestricted pieces
bool test_queen_valid_moves() {
    shared_ptr<Player> player1(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::WHITE));
    shared_ptr<Player> player2(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::BLACK));
    Game new_game(player1, player2);
    new_game.setup_default_board_state();

    // Setup for the queen to move
    std::pair<int, int> player_move_start = std::make_pair(1, 3);
    std::pair<int, int> player_move_end = std::make_pair(2, 3);

    std::pair<bool, std::string> error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The first move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    } 
    
    // Test single move verticle
    player_move_start = std::make_pair(0, 3);
    player_move_end = std::make_pair(1, 3);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The second move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    } 

    // Test multi-move diagonal
    player_move_start = std::make_pair(1, 3);
    player_move_end = std::make_pair(5, 7);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The third move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    } 

    // Test single-move diagonal / capture
    player_move_start = std::make_pair(5, 7);
    player_move_end = std::make_pair(6, 6);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The forth move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }

    // Test multi-move verticle
    player_move_start = std::make_pair(6, 6);
    player_move_end = std::make_pair(2, 6);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The fifth move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }

    // Test multi-move horizontal
    player_move_start = std::make_pair(2, 6);
    player_move_end = std::make_pair(2, 0);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The sixth move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }

    // Test single-move horizontal
    player_move_start = std::make_pair(2, 0);
    player_move_end = std::make_pair(2, 1);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The seventh move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }

    // Test multi-move verticle / capture
    player_move_start = std::make_pair(2, 1);
    player_move_end = std::make_pair(6, 1);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The eighth move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }

    // Test single-move horizontal / capture
    player_move_start = std::make_pair(6, 1);
    player_move_end = std::make_pair(6, 2);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != true) {
        throw std::runtime_error("The eighth move generated an unexpected error code: " + error_values.second);
    } else {
        try {
            new_game.play_move(player_move_start, player_move_end);
        } catch (exception e) {
            throw std::runtime_error(e.what());
        }
    }

    return true;
}

// Tests all moves for the queen that would be invalid
bool test_queen_invalid_moves() {
    shared_ptr<Player> player1(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::WHITE));
    shared_ptr<Player> player2(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::BLACK));
    Game new_game(player1, player2);
    new_game.setup_default_board_state();

    // Test the queen moving into an ally space
    std::pair<int, int> player_move_start = std::make_pair(0, 3);
    std::pair<int, int> player_move_end = std::make_pair(1, 3);

    std::pair<bool, std::string> error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != false) {
        return false;
    }

    // Test the queen jumping over the ally
    player_move_start = std::make_pair(0, 3);
    player_move_end = std::make_pair(2, 3);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != false) {
        return false;
    }

    // Test the queen jumping going out of bounds
    player_move_start = std::make_pair(0, 3);
    player_move_end = std::make_pair(-1, 3);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != false) {
        return false;
    }

    // Test the queen making an erroneous move 
    player_move_start = std::make_pair(0, 3);
    player_move_end = std::make_pair(2, 4);

    error_values = check_move_error_codes(new_game, player_move_start, player_move_end);
    if (error_values.first != false) {
        return false;
    }

    return true;
}

// Use only when messing with the display settings - not an important unit test
void test_display() {
    shared_ptr<Player> player1(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::WHITE));
    shared_ptr<Player> player2(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::BLACK));
    Game new_game(player1, player2);
    new_game.setup_default_board_state();
    new_game.show_board();
}

// Use only when messing with play functions - useful for simulating games without needing the higher level API
void test_simulating_play() {
    shared_ptr<Player> player1(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::WHITE));
    shared_ptr<Player> player2(new Human_Player(DEFAULT_HUMAN_NAME, GAME_PIECE_COLOR::BLACK));
    Game new_game(player1, player2);
    new_game.setup_default_board_state();
    new_game.show_board();

    // Player 1's turn
    std::pair<int, int> player_1_move_start = std::make_pair(1, 3);
    std::pair<int, int> player_1_move_end = std::make_pair(3, 3);

    // Player 2's turn
    std::pair<int, int> player_2_move_start = std::make_pair(6, 4);
    std::pair<int, int> player_2_move_end = std::make_pair(4, 4);

    // D2 - D4
    if (check_move_error_codes(new_game, player_1_move_start, player_1_move_end).first) {
        new_game.play_move(player_1_move_start, player_1_move_end);
    }    
    new_game.show_board();

    new_game.swap_current_player();

    // E7 - E5
    if (check_move_error_codes(new_game, player_2_move_start, player_2_move_end).first) {
        new_game.play_move(player_2_move_start, player_2_move_end);
    }    
    new_game.show_board();



    new_game.swap_current_player();

    player_1_move_start = std::make_pair(3, 3);
    player_1_move_end = std::make_pair(4, 4);

    // Player 1 capture from D4 - E5
    if (check_move_error_codes(new_game, player_1_move_start, player_1_move_end).first) {
        new_game.play_move(player_1_move_start, player_1_move_end);
    }    
    new_game.show_board();



    new_game.swap_current_player();

    player_2_move_start = std::make_pair(7, 4);
    player_2_move_end = std::make_pair(6, 4);

    // E8 - E7
    if (check_move_error_codes(new_game, player_2_move_start, player_2_move_end).first) {
        new_game.play_move(player_2_move_start, player_2_move_end);
    }    
    new_game.show_board();



    new_game.swap_current_player();

    player_1_move_start = std::make_pair(0, 3);
    player_1_move_end = std::make_pair(1, 3);

    // Player 1 capture from D1 - D2
    if (check_move_error_codes(new_game, player_1_move_start, player_1_move_end).first) {
        new_game.play_move(player_1_move_start, player_1_move_end);
    }    
    new_game.show_board();



    new_game.swap_current_player();

    player_2_move_start = std::make_pair(6, 3);
    player_2_move_end = std::make_pair(4, 3);

    // D7 - D5
    if (check_move_error_codes(new_game, player_2_move_start, player_2_move_end).first) {
        new_game.play_move(player_2_move_start, player_2_move_end);
    }    
    new_game.show_board();



    new_game.swap_current_player();

    player_1_move_start = std::make_pair(4, 4);
    player_1_move_end = std::make_pair(5, 3);

    // Player 1 en passant capture E5 - D6, capturing D5
    if (check_move_error_codes(new_game, player_1_move_start, player_1_move_end).first) {
        new_game.play_move(player_1_move_start, player_1_move_end);
    }    
    new_game.show_board();



    new_game.swap_current_player();

    player_2_move_start = std::make_pair(7, 3);
    player_2_move_end = std::make_pair(5, 3);

    // Player 2 capture D8 - D6
    if (check_move_error_codes(new_game, player_2_move_start, player_2_move_end).first) {
        
        new_game.play_move(player_2_move_start, player_2_move_end);
    }    
    new_game.show_board();



    new_game.swap_current_player();

    player_1_move_start = std::make_pair(1, 3);
    player_1_move_end = std::make_pair(3, 1);

    // D2 - B4
    if (check_move_error_codes(new_game, player_1_move_start, player_1_move_end).first) {
        new_game.play_move(player_1_move_start, player_1_move_end);
    }    
    new_game.show_board();


    new_game.swap_current_player();

    player_2_move_start = std::make_pair(5, 3);
    player_2_move_end = std::make_pair(3, 1);

    // Player 2 capture D6 - B4
    if (check_move_error_codes(new_game, player_2_move_start, player_2_move_end).first) {
        
        new_game.play_move(player_2_move_start, player_2_move_end);
    }    
    new_game.show_board();


    new_game.swap_current_player();

    player_1_move_start = std::make_pair(0, 1);
    player_1_move_end = std::make_pair(2, 2);

    // B1 - C3
    if (check_move_error_codes(new_game, player_1_move_start, player_1_move_end).first) {
        new_game.play_move(player_1_move_start, player_1_move_end);
    }    
    new_game.show_board();


    new_game.swap_current_player();

    player_2_move_start = std::make_pair(7, 2);
    player_2_move_end = std::make_pair(4, 5);

    // Player 2 capture C8 - F5
    if (check_move_error_codes(new_game, player_2_move_start, player_2_move_end).first) {
        
        new_game.play_move(player_2_move_start, player_2_move_end);
    }    
    new_game.show_board();


    new_game.swap_current_player();

    player_1_move_start = std::make_pair(0, 2);
    player_1_move_end = std::make_pair(3, 5);

    // C1 - F4
    if (check_move_error_codes(new_game, player_1_move_start, player_1_move_end).first) {
        new_game.play_move(player_1_move_start, player_1_move_end);
    }    
    new_game.show_board();


    new_game.swap_current_player();

    player_2_move_start = std::make_pair(6, 2);
    player_2_move_end = std::make_pair(4, 2);

    // Player 2 capture C7 - C5
    if (check_move_error_codes(new_game, player_2_move_start, player_2_move_end).first) {
        
        new_game.play_move(player_2_move_start, player_2_move_end);
    }    
    new_game.show_board();


    new_game.swap_current_player();

    player_1_move_start = std::make_pair(0, 4);
    player_1_move_end = std::make_pair(0, 2);

    // Player 1 Castle left - E1 - C1
    if (check_move_error_codes(new_game, player_1_move_start, player_1_move_end).first) {
        new_game.play_move(player_1_move_start, player_1_move_end);
    }    
    new_game.show_board();


    new_game.swap_current_player();

    player_2_move_start = std::make_pair(6, 5);
    player_2_move_end = std::make_pair(4, 5);

    // Player 2 capture C7 - C5
    if (check_move_error_codes(new_game, player_2_move_start, player_2_move_end).first) {
        
        new_game.play_move(player_2_move_start, player_2_move_end);
    }    
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


    // Testing the pawns moveset to ensure they can make valid moves
    try {
        if (!test_pawn_valid_moves()) {
            cout << "   ERROR: The pawn was unable to utilize some of its valid moves" << endl;
            ++errors;
        }
    } catch(exception e) {
        cout << "   ERROR: test_pawn_valid_moves threw an error: " << e.what() << endl;
        ++errors;
    }


    // Testing the pawns moveset for invalid moves to ensure they cannot to illegal movess
    try {
        if (!test_pawn_invalid_moves()) {
            cout << "   ERROR: The pawn was able to perform illegal moves" << endl;
            ++errors;
        }
    } catch(exception e) {
        cout << "   ERROR: test_pawn_invalid_moves threw an error: " << e.what() << endl;
        ++errors;
    }


    // Testing the kings moveset for valid moves
    try {
        if (!test_king_valid_moves()) {
            cout << "   ERROR: The king was unable to utilize some of its valid moves" << endl;
            ++errors;
        }
    } catch(exception e) {
        cout << "   ERROR: test_king_valid_moves threw an error: " << e.what() << endl;
        ++errors;
    }


    // Testing the kings moveset for invalid moves to ensure they cannot to illegal movess
    try {
        if (!test_king_invalid_moves()) {
            cout << "   ERROR: The king was able to perform illegal moves" << endl;
            ++errors;
        }
    } catch(exception e) {
        cout << "   ERROR: test_king_invalid_moves threw an error: " << e.what() << endl;
        ++errors;
    }


    // Testing the knights moveset for valid moves - acts as test completeness for restricted pieces
    try {
        if (!test_knight_valid_moves()) {
            cout << "   ERROR: The knight was unable to utilize some of its valid moves" << endl;
            ++errors;
        }
    } catch(exception e) {
        cout << "   ERROR: test_knight_valid_moves threw an error: " << e.what() << endl;
        ++errors;
    }


    // Testing the knights moveset for invalid moves - acts as test completeness for restricted pieces
    try {
        if (!test_knight_invalid_moves()) {
            cout << "   ERROR: The knight was able to perform illegal moves" << endl;
            ++errors;
        }
    } catch(exception e) {
        cout << "   ERROR: test_knight_invalid_moves threw an error: " << e.what() << endl;
        ++errors;
    }


    // Testing the queens moveset for valid moves - acts as test completeness for unrestricted pieces
    try {
        if (!test_queen_valid_moves()) {
            cout << "   ERROR: The queen was unable to utilize some of its moveset" << endl;
            ++errors;
        }
    } catch(exception e) {
        cout << "   ERROR: test_queen_valid_moves threw an error: " << e.what() << endl;
        ++errors;
    }


    // Testing the queens moveset for invalid moves - acts as test completeness for unrestricted pieces
    try {
        if (!test_queen_valid_moves()) {
            cout << "   ERROR: The queen was able to perform illegal moves" << endl;
            ++errors;
        }
    } catch(exception e) {
        cout << "   ERROR: test_queen_invalid_moves threw an error: " << e.what() << endl;
        ++errors;
    }


    // Temporary test to simulate play
    // test_simulating_play();
    test_display();


    return errors;
}