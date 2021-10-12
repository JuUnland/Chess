#pragma once
#include "Piece.h"

class King :
    public Piece
{
public:
    King(Team team, std::pair<int, int> pos, SDL_Handler* handler);

    // calls type and color to debug
    void sayMyName();

    // calculates the possible moves
    void calcPossibleMoves(Piece* field[8][8], bool checkCheck);

    // controls whether this piece is checked or not
    void setCheck(Piece* field[8][8], int x, int y);

    // get check
    bool getCheck() { return m_check; };

private:

    // true, if the king is on any of the other teams movelist's
    bool m_check;
};

