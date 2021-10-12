#pragma once
#include "Piece.h"
#include "King.h"
class Bishop :
    public Piece
{
public:
    //Constructor
    Bishop(Team team, std::pair<int, int> pos, SDL_Handler* handler);

    // used to debug some stuff
    void sayMyName();

    // calculates the possible moves, 
    void calcPossibleMoves(Piece* field[8][8], bool checkCheck);
};

