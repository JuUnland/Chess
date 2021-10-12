#pragma once
#include "Piece.h"

class Pawn :
    public Piece
{
public:
    // constructor
    Pawn(Team team, std::pair<int, int> pos, SDL_Handler* handler);

    // prints name of piece
    void sayMyName();

    // calculates possible moves
    void calcPossibleMoves(Piece* field[8][8], bool checkCheck);

    // direction the pawn moves
    int m_dy;

    // getter m_enPassant
    std::pair<bool, int> getEnPassant() { return m_enPassant; };

    // set m_enPassant
    void setEnPassant(std::pair<bool, int> modifi) { m_enPassant = modifi; };

private:
    // if true, en passant is possible in the int direction
    std::pair<bool, int> m_enPassant;
};

