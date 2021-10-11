#include "Rook.h"
#include <iostream>

Rook::Rook(Team team, std::pair<int, int> pos, SDL_Handler* handler)
	:Piece(team, pos, handler, ROOK)
{
	std::string filename;
	if (team == BLACK)
	{
		filename = "Bilder\\Chess_rdt60.png";
	}
	else
	{
		filename = "Bilder\\Chess_rlt60.png";
	}
	m_handler = handler;
	m_texture = handler->loadImage(filename);

	render();
}

void Rook::sayMyName()
{
	if (m_team == BLACK)
	{
		std::cout << "BLACK ROOK" << std::endl;
	}
	else
	{
		std::cout << "WHITE ROOK" << std::endl;
	}
}

void Rook::calcPossibleMoves(Piece* field[8][8], bool checkCheck)
{
	std::vector<std::tuple <int, int, Piece::MoveType>> moves;

	int dx_copy;
	int dy_copy;
	for (int dx = -1; dx <= 1; dx ++)
	{
		for (int dy = -1; dy <= 1; dy ++)
		{
			if (dy == 0 || dx == 0)
			{
				dx_copy = dx;
				dy_copy = dy;
				while (field[m_pos.first + dx_copy][m_pos.second + dy_copy] == nullptr
					&& (m_pos.first + dx_copy >= 0 && m_pos.first + dx_copy <= 7 && m_pos.second + dy_copy >= 0 && m_pos.second + dy_copy <= 7))
				{
					moves = pushMove(moves,
									 std::tuple<int, int, Piece::MoveType>(m_pos.first + dx_copy, m_pos.second + dy_copy, Piece::NORMAL),
									 getOwnKing(field),
									 field,
									 checkCheck);

					if (dx_copy < 0)
					{
						dx_copy -= 1;
					}
					else if (dx_copy > 0)
					{
						dx_copy += 1;
					}
					if (dy_copy < 0)
					{
						dy_copy -= 1;
					}
					else if (dy_copy > 0)
					{
						dy_copy += 1;
					}
				}
				if (field[m_pos.first + dx_copy][m_pos.second + dy_copy] != nullptr
					&& (m_pos.first + dx_copy >= 0 && m_pos.first + dx_copy <= 7 && m_pos.second + dy_copy >= 0 && m_pos.second + dy_copy <= 7))
				{
					if (field[m_pos.first + dx_copy][m_pos.second + dy_copy]->getTeam() != m_team)
					{
						moves = pushMove(moves,
										 std::tuple<int, int, Piece::MoveType>(m_pos.first + dx_copy, m_pos.second + dy_copy, Piece::NORMAL),
										 getOwnKing(field),
										 field,
										 checkCheck);
					}
				}
			}
		}
	}

	m_possibleMoves = moves;
}


Rook::~Rook()
{
}
