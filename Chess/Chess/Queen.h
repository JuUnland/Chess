#pragma once
#include "Piece.h"
class Queen :
    public Piece
{
public:
    Queen(Team team, std::pair<int, int> pos, SDL_Handler* handler);

    // prints name of piece
    void sayMyName();

    // calculates the possible moves
    void calcPossibleMoves(Piece* field[8][8], bool checkCheck);
};

