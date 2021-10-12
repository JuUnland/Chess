#include "Game.h"
#include <iostream>

Game::Game(SDL_Handler* handler)
       :pl1(new Pawn(Piece::WHITE, std::pair<int, int>(0, 1), handler)),
        pl2(new Pawn(Piece::WHITE, std::pair<int, int>(1, 1), handler)),
        pl3(new Pawn(Piece::WHITE, std::pair<int, int>(2, 1), handler)),
        pl4(new Pawn(Piece::WHITE, std::pair<int, int>(3, 1), handler)),
        pl5(new Pawn(Piece::WHITE, std::pair<int, int>(4, 1), handler)),
        pl6(new Pawn(Piece::WHITE, std::pair<int, int>(5, 1), handler)),
        pl7(new Pawn(Piece::WHITE, std::pair<int, int>(6, 1), handler)),
        pl8(new Pawn(Piece::WHITE, std::pair<int, int>(7, 1), handler)),
        pb1(new Pawn(Piece::BLACK, std::pair<int, int>(0, 6), handler)),
        pb2(new Pawn(Piece::BLACK, std::pair<int, int>(1, 6), handler)),
        pb3(new Pawn(Piece::BLACK, std::pair<int, int>(2, 6), handler)),
        pb4(new Pawn(Piece::BLACK, std::pair<int, int>(3, 6), handler)),
        pb5(new Pawn(Piece::BLACK, std::pair<int, int>(4, 6), handler)),
        pb6(new Pawn(Piece::BLACK, std::pair<int, int>(5, 6), handler)),
        pb7(new Pawn(Piece::BLACK, std::pair<int, int>(6, 6), handler)),
        pb8(new Pawn(Piece::BLACK, std::pair<int, int>(7, 6), handler)),
        rb1(new Rook(Piece::BLACK, std::pair<int, int>(0, 7), handler)),
        rb2(new Rook(Piece::BLACK, std::pair<int, int>(7, 7), handler)),
        rl1(new Rook(Piece::WHITE, std::pair<int, int>(0, 0), handler)),
        rl2(new Rook(Piece::WHITE, std::pair<int, int>(7, 0), handler)),
        nb1(new Knight(Piece::BLACK, std::pair<int, int>(1, 7), handler)),
        nb2(new Knight(Piece::BLACK, std::pair<int, int>(6, 7), handler)),
        nl1(new Knight(Piece::WHITE, std::pair<int, int>(1, 0), handler)),
        nl2(new Knight(Piece::WHITE, std::pair<int, int>(6, 0), handler)),
        bb1(new Bishop(Piece::BLACK, std::pair<int, int>(2, 7), handler)),
        bb2(new Bishop(Piece::BLACK, std::pair<int, int>(5, 7), handler)),
        bl1(new Bishop(Piece::WHITE, std::pair<int, int>(2, 0), handler)),
        bl2(new Bishop(Piece::WHITE, std::pair<int, int>(5, 0), handler)),
        kb1(new King(Piece::BLACK, std::pair<int, int>(4, 7), handler)),
        kl1(new King(Piece::WHITE, std::pair<int, int>(4, 0), handler)),
        qb1(new Queen(Piece::BLACK, std::pair<int, int>(3, 7), handler)),
        ql1(new Queen(Piece::WHITE, std::pair<int, int>(3, 0), handler)),
        m_turn(Piece::WHITE),
        m_handler(handler),
        m_checkEnPassant(true)
{
    m_field[0][7] = rb1;
    m_field[7][7] = rb2;
    m_field[0][0] = rl1;
    m_field[7][0] = rl2;

    m_field[1][7] = nb1;
    m_field[6][7] = nb2;
    m_field[1][0] = nl1;
    m_field[6][0] = nl2;

    m_field[2][7] = bb1;
    m_field[5][7] = bb2;
    m_field[2][0] = bl1;
    m_field[5][0] = bl2;

    m_field[4][7] = kb1;
    m_field[4][0] = kl1;

    m_field[3][7] = qb1;
    m_field[3][0] = ql1;

    m_field[0][1] = pl1;
    m_field[1][1] = pl2;
    m_field[2][1] = pl3;
    m_field[3][1] = pl4;
    m_field[4][1] = pl5;
    m_field[5][1] = pl6;
    m_field[6][1] = pl7;
    m_field[7][1] = pl8;

    m_field[0][6] = pb1;
    m_field[1][6] = pb2;
    m_field[2][6] = pb3;
    m_field[3][6] = pb4;
    m_field[4][6] = pb5;
    m_field[5][6] = pb6;
    m_field[6][6] = pb7;
    m_field[7][6] = pb8;

    for (int i = 2; i < 6; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            m_field[j][i] = nullptr;
        }
    }

    calcAllMoves();
}
 

Game::~Game()
{
}


Piece* Game::getFieldPos(int row, int col)
{
    return m_field[row][col];
}


void Game::move(Piece* start, std::tuple<int, int, Piece::MoveType> move)
{
    if (m_checkEnPassant)
    {
        disableEnPassant();
    }
    else
    {
        m_checkEnPassant = true;
    }

    switch (std::get<2>(move))
    {
        case Piece::NORMAL:
            normal(start->getPos().first, start->getPos().second, std::get<0>(move), std::get<1>(move));
            break;
        case Piece::CASTLE:
            castles(start->getPos().first, start->getPos().second, std::get<0>(move), std::get<1>(move));
            break;
        case Piece::ENPASSANT:
            enPassant(start->getPos().first, start->getPos().second, std::get<0>(move), std::get<1>(move));
            break;
        case Piece::NEWPIECE:
            exchange(start->getPos().first, start->getPos().second, std::get<0>(move), std::get<1>(move));
            break;
        default:
            break;
    }

    gameState();
}


void Game::normal(int xStart, int yStart, int xEnd, int yEnd)
{
    m_field[xEnd][yEnd] = getFieldPos(xStart, yStart);
    m_field[xEnd][yEnd]->m_hasMoved = true;
    m_field[xStart][yStart] = nullptr;
    m_handler->undoPieceRender(xStart, yStart);
    m_field[xEnd][yEnd]->setPosition(std::pair<int, int>(xEnd, yEnd));
    if (m_field[xEnd][yEnd] != nullptr)
    {
        m_handler->undoPieceRender(xEnd, yEnd);
    }
    m_field[xEnd][yEnd]->render();

    if (m_field[xEnd][yEnd]->getType() == Piece::PAWN)
    {
        if (abs(yEnd - yStart) == 2)
        {
            if (xEnd - 1 >= 0)
            {
                if (m_field[xEnd - 1][yEnd] != nullptr)
                {
                    if (m_field[xEnd - 1][yEnd]->getType() == Piece::PAWN)
                    {
                        Pawn* pwn = static_cast<Pawn*>(m_field[xEnd - 1][yEnd]);
                        pwn->setEnPassant(std::pair<bool, int>(true, -1));
                        m_checkEnPassant = false;
                    }
                }
            }

            if (xEnd + 1 <= 7)
            {
                if (m_field[xEnd + 1][yEnd] != nullptr)
                {
                    if (m_field[xEnd + 1][yEnd]->getType() == Piece::PAWN)
                    {
                        Pawn* pwn = static_cast<Pawn*>(m_field[xEnd + 1][yEnd]);
                        pwn->setEnPassant(std::pair<bool, int>(true, 1));
                        m_checkEnPassant = false;
                    }
                }
            }
        }
    }
}


void Game::enPassant(int xStart, int yStart, int xEnd, int yEnd)
{
    Pawn* pawn_start = static_cast<Pawn*>(m_field[xStart][yStart]);
    m_field[xEnd][yEnd - pawn_start->m_dy] = nullptr;
    m_field[xEnd][yEnd] = getFieldPos(xStart, yStart);
    m_field[xEnd][yEnd]->m_hasMoved = true;
    m_field[xStart][yStart] = nullptr;
    m_handler->undoPieceRender(xStart, yStart);
    m_handler->undoPieceRender(xEnd, yEnd - pawn_start->m_dy);
    m_field[xEnd][yEnd]->setPosition(std::pair<int, int>(xEnd, yEnd));
    m_field[xEnd][yEnd]->render();
}


void Game::exchange(int xStart, int yStart, int xEnd, int yEnd)
{
    SDL_Texture* text_rook = m_handler->loadImage("../res/Chess_rlt60.png");
    SDL_Texture* text_knight = m_handler->loadImage("../res/Chess_nlt60.png");
    SDL_Texture* text_bishop = m_handler->loadImage("../res/Chess_blt60.png");
    SDL_Texture* text_queen = m_handler->loadImage("../res/Chess_qlt60.png");
    int y_draw = 0;
    Piece::Team team = Piece::WHITE;

    if (m_field[xStart][yStart]->getTeam() == Piece::BLACK)
    {
        text_rook = m_handler->loadImage("../res/Chess_rdt60.png");
        text_knight = m_handler->loadImage("../res/Chess_ndt60.png");
        text_bishop = m_handler->loadImage("../res/Chess_bdt60.png");
        text_queen = m_handler->loadImage("../res/Chess_qdt60.png");
        y_draw = 3 * m_handler->SCREEN_HEIGHT / 4;
        team = Piece::BLACK;
    }

    SDL_SetRenderDrawColor(m_handler->m_renderer, 155, 103, 60, 255);
    SDL_Rect rectangle = {0,
                          y_draw,
                          m_handler->SCREEN_WIDTH / 4,
                          m_handler->SCREEN_HEIGHT / 4 };
    SDL_RenderFillRect(m_handler->m_renderer, &rectangle);
    SDL_Rect src = { 0, 0, 60, 60 };
    m_handler->DrawRectangle(src, rectangle, text_rook);

    SDL_SetRenderDrawColor(m_handler->m_renderer, 255, 255, 255, 255);
    rectangle.x = m_handler->SCREEN_WIDTH / 4;
    SDL_RenderFillRect(m_handler->m_renderer, &rectangle);
    m_handler->DrawRectangle(src, rectangle, text_knight);

    SDL_SetRenderDrawColor(m_handler->m_renderer, 155, 103, 60, 255);
    rectangle.x = 2 * m_handler->SCREEN_WIDTH / 4;
    SDL_RenderFillRect(m_handler->m_renderer, &rectangle);
    m_handler->DrawRectangle(src, rectangle, text_bishop);

    SDL_SetRenderDrawColor(m_handler->m_renderer, 255, 255, 255, 255);
    rectangle.x = 3 * m_handler->SCREEN_WIDTH / 4;
    SDL_RenderFillRect(m_handler->m_renderer, &rectangle);
    m_handler->DrawRectangle(src, rectangle, text_queen);

    bool quit = false;
    int x = -1;
    int y = -1;

    Piece* clickedOn = nullptr;

    std::cout << m_handler;
    
    while (quit == false)
    {
        while (SDL_PollEvent(&m_handler->m_event))
        {
            if (m_handler->m_event.type == SDL_QUIT)
            {
                quit = true;
            }

            if (m_handler->m_event.type == SDL_MOUSEBUTTONDOWN)
            {
                x = m_handler->m_event.button.x / 160;
                y = m_handler->m_event.button.y / 160;
                
                if (y >= y_draw / 160 && y < y_draw / 160 + 1)
                {
                    if (x < m_handler->SCREEN_WIDTH / 640)
                    {
                        clickedOn = new Rook(team ,std::pair<int, int>(xEnd, yEnd), m_handler);
                    }
                    else if (x < 2 * m_handler->SCREEN_WIDTH / 640)
                    {
                        clickedOn = new Knight(team ,std::pair<int, int>(xEnd, yEnd), m_handler);
                    }
                    else if (x < 3 * m_handler->SCREEN_WIDTH / 640)
                    {
                        clickedOn = new Bishop(team ,std::pair<int, int>(xEnd, yEnd), m_handler);
                    }
                    else if (x <= 4 * m_handler->SCREEN_WIDTH / 640)
                    {
                        clickedOn = new Queen(team ,std::pair<int, int>(xEnd, yEnd), m_handler);
                    }
                    std::cout << x << " " << m_handler->SCREEN_WIDTH / 640 << std::endl;
                }
            }
            
            if (m_handler->m_event.type == SDL_MOUSEBUTTONUP && clickedOn != nullptr)
            {
                quit = true;
            }
        }
    }

    m_field[xEnd][yEnd] = clickedOn;
    m_field[xStart][yStart] = nullptr;
    m_handler->undoPieceRender(xStart, yStart);
    m_handler->renderBackground();
    
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (m_field[i][j] != nullptr)
            {
                m_field[i][j]->render();
            }
        }
    }
    
    SDL_DestroyTexture(text_rook);
    SDL_DestroyTexture(text_bishop);
    SDL_DestroyTexture(text_knight);
    SDL_DestroyTexture(text_queen);
}


void Game::castles(int xStart, int yStart, int xEnd, int yEnd)
{
    if (xEnd == 0)
    {
        m_field[2][yEnd] = m_field[4][yEnd];
        m_field[3][yEnd] = m_field[0][yEnd];
        m_field[2][yEnd]->m_hasMoved = true;
        m_field[3][yEnd]->m_hasMoved = true;
        m_field[2][yEnd]->setPosition(std::pair<int, int>(2, yEnd));
        m_field[3][yEnd]->setPosition(std::pair<int, int>(3, yEnd));
        m_field[4][yEnd] = nullptr;
        m_field[0][yEnd] = nullptr;
        m_handler->undoPieceRender(4, yEnd);
        m_handler->undoPieceRender(0, yEnd);
        m_field[2][yEnd]->render();
        m_field[3][yEnd]->render();
    }
    else
    {
        m_field[6][yEnd] = m_field[4][yEnd];
        m_field[5][yEnd] = m_field[7][yEnd];
        m_field[6][yEnd]->m_hasMoved = true;
        m_field[5][yEnd]->m_hasMoved = true;
        m_field[6][yEnd]->setPosition(std::pair<int, int>(6, yEnd));
        m_field[5][yEnd]->setPosition(std::pair<int, int>(5, yEnd));
        m_field[4][yEnd] = nullptr;
        m_field[7][yEnd] = nullptr;
        m_handler->undoPieceRender(4, yEnd);
        m_handler->undoPieceRender(7, yEnd);
        m_field[6][yEnd]->render();
        m_field[5][yEnd]->render();
    }
}

void Game::gameState()
{
    bool lost = true;
    King* pivot = kb1;

    if (m_turn == Piece::BLACK)
    {
        pivot = kl1;
    }

    pivot->setCheck(m_field, kl1->getPos().first, kl1->getPos().second);
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (m_field[i][j] != nullptr)
            {
                if (m_field[i][j]->getTeam() != m_turn)
                {
                    m_field[i][j]->calcPossibleMoves(m_field, true);
                    if (!m_field[i][j]->getPossibleMoves().empty())
                    {
                        lost = false;
                    }
                }
            }
        }
    }

    if (pivot->getCheck() && lost)
    {
        if (m_turn == Piece::BLACK)
        {
            std::cout << "Black wins!";
        }
        else
        {
            std::cout << "White wins!";
        }
    }
    else if (lost)
    {
        std::cout << "Remis!";
    }
    else
    {
        if (m_turn == Piece::BLACK)
        {
            m_turn = Piece::WHITE;
        }
        else
        {
            m_turn = Piece::BLACK;
        }
    }

}


void Game::disableEnPassant()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (m_field[i][j] != nullptr)
            {
                if (m_field[i][j]->getType() == Piece::PAWN)
                {
                    Pawn* pwn = static_cast<Pawn*>(m_field[i][j]);
                    pwn->setEnPassant(std::pair<bool, int>(false, 0));
                }
            }
        }
    }
}


void Game::renderPossibleMoves(Piece* piece)
{
    piece->calcPossibleMoves(m_field, true);
    std::vector<std::tuple<int, int, Piece::MoveType>> possible = piece->getPossibleMoves();
    SDL_Rect rectangle;
    for (const auto& value : possible) {
        if ((std::get<0>(value) % 2 == 0 && std::get<1>(value) % 2 == 1) || (std::get<0>(value) % 2 == 1 && std::get<1>(value) % 2 == 0))
        {
            SDL_SetRenderDrawColor(m_handler->m_renderer, 0, 134, 139, 255);
        }
        else
        {
            SDL_SetRenderDrawColor(m_handler->m_renderer, 164, 211, 238, 255);
        }
        rectangle = { std::get<0>(value) * m_handler->SCREEN_WIDTH / 8,
                      std::get<1>(value)* m_handler->SCREEN_HEIGHT / 8,
                      m_handler->SCREEN_WIDTH / 8,
                      m_handler->SCREEN_HEIGHT / 8 };
        SDL_RenderFillRect(m_handler->m_renderer, &rectangle);

        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (m_field[i][j] != nullptr)
                {
                    m_field[i][j]->render();
                }
            }
        }
    } 
}

void Game::undoRenderPossibleMoves(Piece* piece)
{
    std::vector<std::tuple<int, int, Piece::MoveType>> possible = piece->getPossibleMoves();
    for (const auto& value : possible) {
        if ((std::get<0>(value) % 2 == 0 && std::get<1>(value) % 2 == 1) || (std::get<0>(value) % 2 == 1 && std::get<1>(value) % 2 == 0))
        {
            SDL_SetRenderDrawColor(m_handler->m_renderer, 155, 103, 60, 255);
        }
        else
        {
            SDL_SetRenderDrawColor(m_handler->m_renderer, 255, 255, 255, 255);
        }
        SDL_Rect rectangle = { std::get<0>(value) * m_handler->SCREEN_WIDTH / 8,
                                  std::get<1>(value) * m_handler->SCREEN_HEIGHT / 8,
                                  m_handler->SCREEN_WIDTH / 8,
                                  m_handler->SCREEN_HEIGHT / 8 };
        SDL_RenderFillRect(m_handler->m_renderer, &rectangle);

        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (m_field[i][j] != nullptr)
                {
                    m_field[i][j]->render();
                }
            }
        }
    }
}

void Game::calcAllMoves()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (m_field[i][j] != nullptr)
            {
                m_field[i][j]->calcPossibleMoves(m_field, true);
            }
        }
    }
}

bool Game::isValidMove(int x, int y, Piece* piece)
{
    std::vector<std::tuple<int, int, Piece::MoveType>> list = piece->getPossibleMoves();
    for (const auto& value : list) {
        if (std::get<0>(value) == x && std::get<1>(value) == y)
        {
            return true;
        }
    }
    return false;
}
