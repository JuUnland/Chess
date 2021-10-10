#pragma once
#include "Piece.h"
#include "King.h"
class Bishop :
    public Piece
{
public:
    Bishop(Team team, std::pair<int, int> pos, SDL_Handler* handler);

    void sayMyName();

    // calculates the possible moves
    void calcPossibleMoves(Piece* field[8][8], bool checkCheck);
};

