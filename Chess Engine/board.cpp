#include <iostream>
#include "board.h"

using namespace std;

//Helper functions for converting between representing row and columns to uint64_t number vise versa
uint64_t Board:: find_bit_position(int row, int column){
    /*

    Finds the position of the coordinates in a 64 grid and converts it to a number in a 64 bit number

    Input: 
        int row: Row in a chess board;
        int column: column in a chess board;

    Returns
        uint64_t number which correlates to the row and column input of th user
    
    */
    int bit_position = row * 8 + column;
    uint64_t binary_rep_position = (uint64_t)1 << bit_position;
    return binary_rep_position;
}

vector <int> Board::find_coordinates(int64_t piece){
    /*
    Produces a vector which stores the row and column where the piece is located 
    by taking in a uint64_t number.

    Input:
        Piece value
    
    Output:
        Vector of ints
        [row, colum]    
    */

    vector <int> piece_position_array;
    //Going through the whole board
    for(int bit = 0; bit < 64; bit++){
        // AND bitwise operation, check else move bit by one to left
        if(piece & (1ULL << bit)){
            int column = (bit% 8);
            int row = int(bit/8);
            piece_position_array.push_back(row);
            piece_position_array.push_back(column);
        }
    }
    return piece_position_array;
}


//Helper function for finding which piece clicked
uint64_t* Board:: find_type(int row, int column){
    /*
    Finding the type of piece at the coords column and row and returning the state

    Input:
        int row: The row on a chess board
        int column: The column on a chess board
    
    Return:
        A pointer of the type of piece clicked
            eg white_pawn_state
    
    */

    //Making sure the correct column and row selected
    if(column > 7|| column < 0 || row > 7 || row <0){
        return nullptr;
    }

    //Must find the value 0-63
    uint64_t bit_position = find_bit_position(row, column);

    //Going through all piece positions
    //White pieces
    if(bit_position & (this->white_bishop_state)){
        return &this->white_bishop_state;
    }
    else if(bit_position & (this->white_horse_state)){
        return &this->white_horse_state;
    }
    else if(bit_position & (this->white_king_state)){
        return &this->white_king_state;
    }
    else if(bit_position & (this->white_pawn_state)){
        return &this->white_pawn_state;
    }
    else if(bit_position & (this->white_queen_state)){
        return &this->white_queen_state;
    }
    else if(bit_position & (this->white_rook_state)){
        return &this->white_rook_state;
    }


    //Black pieces
    else if(bit_position & (this->black_bishop_state)){
        return &this->black_bishop_state;
    }
    else if(bit_position & (this->black_horse_state)){
        return &this->black_horse_state;
    }
    else if(bit_position & (this->black_king_state)){
        return &this->black_king_state;
    }
    else if(bit_position & (this->black_pawn_state)){
        return &this->black_pawn_state;
    }
    else if(bit_position & (this->black_queen_state)){
        return &this->black_queen_state;
    }
    else if(bit_position & (this->black_rook_state)){
        return &this->black_rook_state;
    }

    //If not selected
    return nullptr;

}


//Helper function for generating moves
vector<uint64_t> Board:: movement_check(uint64_t piece_state, int row, int column, vector<int> row_column_array){
    /*
    A helper function used in find_legal_move for long range pieces 
    
    Input:
        uint64_t piece_state: The selected piece eg white pawns
        int row: Row in a chess board
        int column: column in a chess board
        vector<int row_column_array>: Is a vector ({row, column}) which either contains values 1, -1, 0.  
            This determines the direction, where 1 means to go in the positive direction of either row or column

    Return:
        {legal_moves, capture_moves}
        Legal_moves is a uint64_t number with one in a bit position where there is a legal move
        capture_moves is a uint64_t number with one in a bit position where there is a capturable move
    
    */
    uint64_t friendly_pieces, enemy_pieces, move;

    if(piece_state & (this->white_board_state())){
        friendly_pieces = this->white_board_state();
        enemy_pieces = this->black_board_state();
    }else{
        enemy_pieces = this->white_board_state();
        friendly_pieces = this->black_board_state(); 
    }

    uint64_t legal_moves = 0;
    uint64_t capture_moves = 0;
    int delta_row = row_column_array[0];
    int delta_column = row_column_array[1];

    int next_row = row + delta_row;
    int next_column = column + delta_column;

    while(next_row >=0 && next_row < 8 && next_column >=0 && next_column<8){
        move = this->find_bit_position(next_row, next_column);
        
        if(move & friendly_pieces){
            break;
        }

        if(move & enemy_pieces){
            capture_moves = capture_moves | move;
            break;
        }

        legal_moves = legal_moves | move;
        next_row += delta_row;
        next_column += delta_column;
    }
    return {legal_moves, capture_moves};
}


//Pawn Promotion Logic
void Board:: pawn_promote(char selected_piece){
    /*
    Depending on the input the pawn can promote into it 
    given a pawn is where it can promote
    */
    uint64_t *family_of_piece;
    uint64_t desintation_pos;
    int row = this->promoting_row;
    int column = this->promoting_col;


    if(white_promotion){
        if(selected_piece == 'Q'){
            family_of_piece = &this->white_queen_state;
        }
        else if (selected_piece == 'R'){
            family_of_piece = &this->white_rook_state;
        }
        else if (selected_piece == 'B'){
            family_of_piece = &this->white_bishop_state;
        }
        else if (selected_piece == 'K'){
            family_of_piece = &this->white_horse_state;
        }
        
        desintation_pos = find_bit_position(row, column);

        //First need to remove the pawn from pawn state
        this->white_pawn_state &= ~desintation_pos;
        *family_of_piece |= desintation_pos;
        white_promotion = false;
    }

    if(black_promotion){
        if(selected_piece == 'Q'){
            family_of_piece = &this->black_queen_state;
        }
        else if (selected_piece == 'R'){
            family_of_piece = &this->black_rook_state;
        }
        else if (selected_piece == 'B'){
            family_of_piece = &this->black_bishop_state;
        }
        else if (selected_piece == 'K'){
            family_of_piece = &this->black_horse_state;
        }
        
        desintation_pos = find_bit_position(row, column);

        //First need to remove the pawn from pawn state
        this->black_pawn_state &= ~desintation_pos;
        *family_of_piece |= desintation_pos;
        black_promotion = false;
    }
}

void Board:: can_promote(int row, int column){
    /*
    Returns the row and column of a pawn that needs to be promoted.
    */
    if(row== 7){
        if(this->white_pawn_state & find_bit_position(row,column)){
            this->promoting_row = row;
            this->promoting_col = column;
            this->white_promotion = true;
        }
    }

    if(row== 0){
        if(this->black_pawn_state & find_bit_position(row,column)){
            this->promoting_row = row;
            this->promoting_col = column;
            this->black_promotion = true;
        }
    }
}



//Board Evaluation
int Board:: evaluate_board(string colour){
    int pawn_value = 1;
    int horse_value = 3;
    int bishop_value = 3;
    int rook_value = 5;
    int queen_value = 9;

    int black_value = pawn_value * (__builtin_popcountll(this->black_pawn_state)) + 
                    (horse_value * __builtin_popcountll(this->black_horse_state)) + 
                    (bishop_value * __builtin_popcountll(this->black_bishop_state)) + 
                    (rook_value *__builtin_popcountll(this->black_rook_state)) +
                    (queen_value *__builtin_popcountll(this->black_queen_state)); 

    int white_value = pawn_value * (__builtin_popcountll(this->white_pawn_state)) + 
                    (horse_value * __builtin_popcountll(this->white_horse_state)) + 
                    (bishop_value * __builtin_popcountll(this->white_bishop_state)) + 
                    (rook_value *__builtin_popcountll(this->white_rook_state)) +
                    (queen_value *__builtin_popcountll(this->white_queen_state)); 

    
    return white_value - black_value;
}

int Board:: horse_evaluation(uint64_t piece){
    int horse_value = 3;
    

    constexpr double scalar_adjust[64] = {
        -0.5, -0.4, -0.4, -0.4,  -0.4,  -0.4, -0.4, -0.5, // Bottom row
        -0.4, -0.2,  0.0,  0.0,   0.0,   0.0, -0.2, -0.4,  
        -0.4,  0.0,  0.1,  0.2,   0.2,   0.1,  0.0, -0.4,   
        -0.4,  0.0,  0.2,  0.25,  0.25,  0.2,  0.0, -0.4,   
        -0.4,  0.0,  0.2,  0.25,  0.25,  0.2,  0.0, -0.4,  
        -0.4,  0.0,  0.1,  0.2,   0.2,   0.1,  0.0, -0.4,   
        -0.4, -0.2,  0.0,  0.0,   0.0,   0.0, -0.2, -0.4,   
        -0.5, -0.4, -0.4, -0.4,  -0.4,  -0.4, -0.4, -0.5,   // Top row
    };

}
