main: main.cpp board.cpp board.h
	g++ main.cpp board.cpp Moves.cpp Castling.cpp GameState.cpp -o main -std=c++17 -I/opt/homebrew/include -L/opt/homebrew/lib -lsfml-graphics -lsfml-window -lsfml-system

run: main
	./main

clean:
	rm -f main