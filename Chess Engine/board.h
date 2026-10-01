#include <iostream>

using namespace std;

class Board{
    public:
        //White Pieces Starting Position
        uint64_t white_pawn_state   = 0b00000000'00000000'00000000'00000000'00000000'00000000'11111111'00000000;
        uint64_t white_rook_state   = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'10000001;
        uint64_t white_horse_state  = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'01000010;
        uint64_t white_bishop_state = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00100100;
        uint64_t white_king_state   = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00010000;
        uint64_t white_queen_state  = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00001000;

        //Black Pieces Starting Position
        uint64_t black_pawn_state   = 0b00000000'11111111'00000000'00000000'00000000'00000000'00000000'00000000;
        uint64_t black_rook_state   = 0b10000001'00000000'00000000'00000000'00000000'00000000'00000000'00000000;
        uint64_t black_horse_state  = 0b01000010'00000000'00000000'00000000'00000000'00000000'00000000'00000000;
        uint64_t black_bishop_state = 0b00100100'00000000'00000000'00000000'00000000'00000000'00000000'00000000;
        uint64_t black_king_state   = 0b00010000'00000000'00000000'00000000'00000000'00000000'00000000'00000000;
        uint64_t black_queen_state  = 0b00001000'00000000'00000000'00000000'00000000'00000000'00000000'00000000;


        //Tracks Who's turn it is 
        bool white_move = true;

        //Tracking bools for castling
        bool white_rook_a = true;
        bool white_rook_h = true;
        bool black_rook_a = true;
        bool black_rook_h = true;
        bool white_king_first_move = true;
        bool black_king_first_move = true;

        uint64_t intial_wr_state_a   = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'00000001;
        uint64_t intial_wr_state_h   = 0b00000000'00000000'00000000'00000000'00000000'00000000'00000000'10000000;
        uint64_t intial_br_state_a   = 0b00000001'00000000'00000000'00000000'00000000'00000000'00000000'00000000;
        uint64_t intial_br_state_h   = 0b10000000'00000000'00000000'00000000'00000000'00000000'00000000'00000000;


        bool castle_white_h = false;
        bool castle_white_a = false;
        bool castle_black_h = false;
        bool castle_black_a = false;


        struct Undo{
            uint64_t saved_initial_state;
            uint64_t saved_destination_state;

            uint64_t* initial_piece;
            uint64_t* destination_piece;


            uint64_t saved_white_rook_state;
            uint64_t saved_black_rook_state;
            bool saved_white_king_first_move;
            bool saved_black_king_first_move;

            bool saved_white_rook_a;
            bool saved_white_rook_h;
            bool saved_black_rook_a;
            bool saved_black_rook_h;
        };





        //Finding the bit position in 64 bits number
        uint64_t find_bit_position(int row, int column);

        //Finding the row and column coordinates
        vector <int> find_coordinates(int64_t peice);

        //Returns the state of what type the clicked piece is
        uint64_t *find_type(int row, int column);

        //Returns big number where there is a one for the possible legal moves
        //returns Legal moves and capture moves in a vector
        vector <uint64_t> generate_legal_moves(int row, int column);

        
        // Makes a move and updates the state of the peice
        void make_move(int destination_row, int destination_column, int intial_row, int intial_column);


        //Returns the state of the whole board
        uint64_t board_state();
        uint64_t white_board_state();
        uint64_t black_board_state();


        //Helper function
        vector<uint64_t> movement_check(uint64_t piece_state, int row, int column, vector<int> row_column_array);
        

        //Checks if a king piece is under check in row and column
        bool king_check(uint64_t colour);

        bool castling_check(bool white_king, bool king_side);

        void update_castling_var(uint64_t *intial_state_piece, uint64_t destination_pos);
        void make_castle_move(int destination_row, int destination_column, int intial_row, int intial_column, uint64_t* initial_piece_type, uint64_t piece_clicked);


        //Checkmate and stalemate checks
        bool can_make_move(int destination_row, int destination_column, int intial_row, int intial_column, bool white);
        void undo_move(const Undo& undo);

        bool check_legal_moves(string colour);
        bool is_checkmate(string colour);
        bool is_stalemate(string colour);


        //Promoting Pawn
        void pawn_promote(char selected_piece);
        void can_promote(int row, int col);

        int promoting_row = 0;
        int promoting_col = 0;

        bool white_promotion = false;
        bool black_promotion = false;

        //Board evaluation
        int evaluate_board(string colour);
        int horse_evaluation (uint64_t piece);
};