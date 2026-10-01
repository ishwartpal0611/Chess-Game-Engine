#include <SFML/Graphics.hpp>
#include <optional>
#include <string.h>
#include <iostream>


//Including things from other files
#include "board.h"

using namespace sf;
using namespace std;


void print_position_terminal(int64_t piece){
    /*
    This function prints the position of a 
    chess piece type into the terminal.

    input:
        int64_t piece
    output:
        printing in terminal
    */

    //Going through the whole board
    for(int bit = 0; bit < 64; bit++){
        // AND bitwise operation, check else move bit by one to left
        if(piece & (1ULL << bit)){
            char file = (bit% 8)+'a';
            int rank = int(bit/8 + 1);
            cout<<"Pawn is at position: "<< file <<rank<<endl;
            cout<<"Row: "<< bit/8 <<" Column: "<<(bit% 8)<<"\n" << endl;
        }
    }
}

vector <int> generate_piece_position_array(int64_t piece){
    /*
    Produces a vector which stores the column and rows, for the piece

    Input:
        Piece value
    
    Output:
        Vector
        [column, row....]    
    */

    vector <int> piece_position_array;
    //Going through the whole board
    for(int bit = 0; bit < 64; bit++){
        // AND bitwise operation, check else move bit by one to left
        if(piece & (1ULL << bit)){
            char column = (bit% 8);
            int row = int(bit/8);
            piece_position_array.push_back(column);
            piece_position_array.push_back(row);
        }
    }
    return piece_position_array;
}

//Returns a vector of sprites of the pieces in their respective coordinates
vector <Sprite> position_sprite( vector<int> &position_vector, Sprite sprite_template){
    /*
    Takes the position of the piece one the board from generate_piece_position_array() function.
    For column and row coordinates, calulates and saves location which would be used to display.

    Input:
        Position Vector: [column, row...]
        Sprite template:
    
    Output:
        Vector where the sprite is located.
    */

    vector <Sprite> image_vector;

    for(int i = 0; i + 1< position_vector.size() ; i+= 2){
        int x = position_vector[i] * 100;
        int y = position_vector[i + 1] * 100;

        Sprite piece(sprite_template);
        piece.setScale({0.19,0.19});
        piece.setPosition({ x + 18.f, -y + 700.f});

        image_vector.push_back(piece);
    }
    return image_vector;
}


class Piece{
    public:
        uint64_t piece_state;
        string file_path;

        Texture piece_image;

        //Constructor
        Piece(uint64_t piece_state, string file_path);


        //Method
        vector <Sprite>  display_sprite();
};

//Constructor Function
Piece:: Piece(uint64_t piece_state, string file_path){
    this->piece_state = piece_state;
    this->file_path = file_path;
}

//Applies the image to the sprite
//Displays the sprites in their proper location 
vector <Sprite>  Piece:: display_sprite(){
    if (!this->piece_image.loadFromFile(this->file_path)){
        cout<< "Can't open the image"<< endl;
    }
    //Creating Sprite
    Sprite piece_sprite(piece_image);
    piece_sprite.setScale({0.2,0.2});

    //Displaying the piece using piece state
    vector <Sprite> piece_image_vector;
    vector<int>piece_position_vector= generate_piece_position_array(this->piece_state);
    return piece_image_vector = position_sprite(piece_position_vector, piece_sprite);

}




void draw_winner_banner(RenderWindow& window, const sf::Font& font, const string& message){
    RectangleShape title_card(Vector2f(400.f, 100.f));
    title_card.setOutlineColor(Color::White);
    title_card.setOutlineThickness(3.f);
    title_card.setOrigin({200.f, 50.f});
    title_card.setPosition({400.f, 400.f});

    // Text
    Text text(font, message, 30);
    text.setFillColor(sf::Color(0, 0, 0));

    // Center the text within the banner
    FloatRect text_bounds = text.getLocalBounds();
    text.setOrigin({text_bounds.position.x + text_bounds.size.x / 2.f, text_bounds.position.y + text_bounds.size.y / 2.f});
    text.setPosition({400.f, 400.f});

    window.draw(title_card);
    window.draw(text);
}


bool game_won = false;
bool game_drawn = false;
string message;

int main() {
    int x_width_display = 800;
    int y_length_display = 800;

    //Colour Presets
    //Chess.com
    Color light_square_chess_com = Color(235, 236, 211);
    Color dark_square_chess_com = Color(122, 148, 90);

    //Rendering the Window
    RenderWindow window(
        VideoMode({800u, 800u}),
        "Chess Board"
    );

    //Setting the font
    sf::Font font;
    if(!font.openFromFile("Fonts/Inter/InterRegular.ttf")){
        cout << "Failed to load font" << endl;
    }


    //Creating a Board 
    Board board;


    //Images for Chess
    //-----------------White Pawn-----------------
    Piece white_pawn(board.white_pawn_state, "Images/White_Pawn/White_pawn.png");
    vector <Sprite> white_pawns_image_vector = white_pawn.display_sprite();

    //-----------------White Rook-----------------
    Piece white_rook(board.white_rook_state, "Images/White_Rook/white_rook_3.png");
    vector <Sprite> white_rook_image_vector = white_rook.display_sprite();
    
    //-----------------White bishops-----------------
    Piece white_bishop(board.white_bishop_state, "Images/White_Bishop/white_bishop_3.png");
    vector <Sprite> white_bishop_image_vector = white_bishop.display_sprite();

    //-----------------White Horse-----------------
    Piece white_horse(board.white_horse_state, "Images/White_Horse/white_horse.png");
    vector <Sprite> white_horse_image_vector = white_horse.display_sprite();

    //-----------------White King-----------------
    Piece white_king(board.white_king_state, "Images/White_King/white_king_2.png");
    vector <Sprite> white_king_image_vector = white_king.display_sprite();

    //-----------------White Queen-----------------
    Piece white_queen(board.white_queen_state, "Images/White_Queen/white_queen_2.png");
    vector <Sprite> white_queen_image_vector = white_queen.display_sprite();




    //-----------------Black Pawn-----------------
    Piece black_pawn(board.black_pawn_state, "Images/Black_Pawn/Black_pawn.png");
    vector <Sprite> black_pawns_image_vector = black_pawn.display_sprite();

    //-----------------Black Rook-----------------
    Piece black_rook(board.black_rook_state, "Images/Black_Rook/Black_rook_1.png");
    vector <Sprite> black_rook_image_vector = black_rook.display_sprite();
        
    //-----------------Black bishop-----------------
    Piece black_bishop(board.black_bishop_state, "Images/Black_Bishop/black_bishop.png");
    vector <Sprite> black_bishop_image_vector = black_bishop.display_sprite();

    //-----------------Black Horse-----------------
    Piece black_horse(board.black_horse_state, "Images/Black_Horse/black_horse.png");
    vector <Sprite> black_horse_image_vector = black_horse.display_sprite();

    //-----------------Black King-----------------
    Piece black_king(board.black_king_state, "Images/Black_King/black_king_1.png");
    vector <Sprite> black_king_image_vector = black_king.display_sprite();

    //-----------------Black Queen-----------------
    Piece black_queen(board.black_queen_state, "Images/Black_Queen/black_queen_2.png");
    vector <Sprite> black_queen_image_vector = black_queen.display_sprite();


    //Remembering how many clicks
    int mouse_clicked = 0;
    bool piece_clicked = false;
    bool wait_move = true;
    uint64_t possible_moves = 0;
    uint64_t destination = 0 ;
    uint64_t piece_being_moved = 0 ;
    vector<uint64_t> moves;

    //First coords
    int intial_column = -1;
    int intial_row = -1;


    //Keep looping while window is open
    while (window.isOpen()) {

        while (const optional event = window.pollEvent()) {
            if (event->is<Event::Closed>()) {
                window.close();
            }
            
            //Check if a key is pressed
            if (const auto* key = event->getIf<Event::KeyPressed>()){
                //If the key is Escape close window
                if(key->code == Keyboard::Key::Escape){
                    window.close();
                }
            }




            //Check if the mouse is clicked
            if(const auto mouse = event->getIf<Event::MouseButtonPressed>()){
                mouse_clicked += 1;

                //Produces eg Position Clicked : c2
                Vector2i mouse_position = Mouse::getPosition(window);
                char rank = mouse_position.x/100 + 'a'; 
                int row = (y_length_display - mouse_position.y)/100 + 1;
                //cout<< "Position Clicked : "<<rank<<row<<endl;



                //Produces eg (1, 2)
                int clicked_row = 7 - (mouse_position.y)/100;
                int clicked_column = int(mouse_position.x/100 );
                //cout<< "("<< clicked_row <<", "<< clicked_column<<")"<< endl;
                cout<< clicked_row * 8 + clicked_column<< endl;

                //First click selecting piece
                if(!piece_clicked){
                    if(board.find_type(clicked_row, clicked_column) != nullptr){
                        intial_column = clicked_column;
                        intial_row = clicked_row;

                        moves = board.generate_legal_moves(clicked_row, clicked_column);
                        possible_moves = moves[0] | moves[1];

                        piece_clicked = true;
                    }
                }else{
                    //Second click moving/ capturing
                    if(piece_clicked && (clicked_row != intial_row || clicked_column != intial_column)){

                        destination = board.find_bit_position(clicked_row, clicked_column);
                        
                        if(possible_moves & destination){
                            //Make a function to move the piece
                            board.make_move(clicked_row, clicked_column ,intial_row, intial_column);
                        }
                        piece_clicked = false;
                    }

                    vector<string> colours = {"WHITE", "BLACK"};
                    
                    for(string colour : colours){
                        if(board.is_checkmate(colour)){
                            cout<<colour<<"King is check mated"<< endl;
                        }

                        if(board.is_stalemate(colour)){
                            cout<<colour<<"King is in check"<< endl;
                        }
                    }
                }
   
            }
        }

        window.clear(); 
        // White Pieces
        white_pawn.piece_state = board.white_pawn_state;
        white_pawns_image_vector = white_pawn.display_sprite();

        white_horse.piece_state = board.white_horse_state;
        white_horse_image_vector = white_horse.display_sprite();

        white_rook.piece_state = board.white_rook_state;
        white_rook_image_vector = white_rook.display_sprite();

        white_bishop.piece_state = board.white_bishop_state;
        white_bishop_image_vector = white_bishop.display_sprite();

        white_queen.piece_state = board.white_queen_state;
        white_queen_image_vector = white_queen.display_sprite();

        white_king.piece_state = board.white_king_state;
        white_king_image_vector = white_king.display_sprite();


        //Black Pieces
        black_pawn.piece_state = board.black_pawn_state;
        black_pawns_image_vector = black_pawn.display_sprite();

        black_horse.piece_state = board.black_horse_state;
        black_horse_image_vector = black_horse.display_sprite();

        black_rook.piece_state = board.black_rook_state;
        black_rook_image_vector = black_rook.display_sprite();

        black_bishop.piece_state = board.black_bishop_state;
        black_bishop_image_vector = black_bishop.display_sprite();

        black_queen.piece_state = board.black_queen_state;
        black_queen_image_vector = black_queen.display_sprite();

        black_king.piece_state = board.black_king_state;
        black_king_image_vector = black_king.display_sprite();


        // Drawing the board
        for(int j = 0; j < 8; j += 1){
            //Row 0, 2, 4, 6
            if(j%2 == 0){
                for (int i = 0; i < 8; i += 1){
                    //Cream Color for every even column
                    if(i%2 == 0){
                        RectangleShape cream_square(Vector2f(100.f,100.f));
                        //Green Colour
                        cream_square.setFillColor(light_square_chess_com);
                        // Setting Position
                        cream_square.setPosition({i * 100.f, j *100.f});
                        window.draw(cream_square);
                    }else{
                        //Green Color for every odd column
                        RectangleShape green_square(Vector2f(100.f,100.f));
                        //Green Colour
                        green_square.setFillColor(dark_square_chess_com);
                        // Setting Position
                        green_square.setPosition({i * 100.f, j *100.f});
                        window.draw(green_square);
                    }
                }
            }else{
                //Row 1, 3, 5, 7
                for (int i = 0; i < 8; i += 1){
                    if(i%2 != 0){
                        RectangleShape cream_square(Vector2f(100.f,100.f));
                        //Green Colour
                        cream_square.setFillColor(light_square_chess_com);
                        // Setting Position
                        cream_square.setPosition({i * 100.f, j *100.f});
                        window.draw(cream_square);
                    }else{
                        RectangleShape green_square(Vector2f(100.f,100.f));
                        //Green Colour
                        green_square.setFillColor(dark_square_chess_com);
                        // Setting Position
                        green_square.setPosition({i * 100.f, j *100.f});
                        window.draw(green_square);
                    }
                }
            }   
        }



        vector<vector <Sprite> >all_pieces_image_vector = {
            white_pawns_image_vector,   black_pawns_image_vector,
            black_rook_image_vector,    white_rook_image_vector, 
            white_bishop_image_vector,  black_bishop_image_vector, 
            white_horse_image_vector,   black_horse_image_vector, 
            white_king_image_vector,    black_king_image_vector, 
            white_queen_image_vector,   black_queen_image_vector
        };

        for(vector<Sprite> &individual_piece_image_vector : all_pieces_image_vector){
            for( Sprite & element : individual_piece_image_vector ){
                window.draw(element);
            }  
        }

        
        //Checking if pawn can promote





        //Checking if the game has been won
        vector<string> colours = {"WHITE", "BLACK"};
        if(!game_drawn && !game_won){
            for(string colour : colours){
                if(board.is_checkmate(colour)){
                    if(colour == "WHITE"){
                        message = "Black Wins!!";
                        game_won = true;
                        cout<<"Black Wins!!"<< endl;
                    }else{
                        message = "White Wins!!";
                        game_won = true;

                        cout<<"White Wins!!"<< endl;
                    }
                }

                if(board.is_stalemate(colour)){
                    message = "Stalemate Draw";
                    game_drawn = true;
                    cout<<"Stalemate Draw"<< endl;
                }
            }
        }        


        if(game_drawn || game_won){
            draw_winner_banner(window, font, message);
        }

        window.display();


    }

    return 0;
}

