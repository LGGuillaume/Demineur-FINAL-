#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <ctime>
#include "game.h"

Game::~Game()
{
    for (int i = 0; i < lines; ++i)
    {
        delete[] board[i];
    }
    delete[] board;
}

void Game::initializeGame(int& lines_, int& columns_)
{
    int level = 0;

    std::cout << "--- Welcome to Minesweeper ---" << std::endl;
    std::cout << std::endl;
    std::cout << "Choose your difficulty: 1.EASY | 2.NORMAL | 3.HARD" << std::endl;
    std::cout << "Which one do you pick: ";
    std::cin >> level;

    switch (level)
    {
    case 1:
        lines_ = 10;
        columns_ = 10;
        numBombs = 25;
        break;
    case 2:
        lines_ = 20;
        columns_ = 15;
        numBombs = 40;
        break;
    case 3:
        lines_ = 25;
        columns_ = 20;
        numBombs = 50;
        break;
    default:
        std::cout << std::endl << "--- BUG ---" << std::endl;
        exit(1);
    }

    lines = lines_;
    columns = columns_;
    board = new char* [lines];
    for (int i = 0; i < lines; ++i)
    {
        board[i] = new char[columns];
    }

    // initialise le plateau avec des cases vides
    for (int i = 0; i < lines; ++i)
    {
        for (int j = 0; j < columns; ++j)
        {
            board[i][j] = ' ';
        }
    }

    placeBombs();
    adjacentBombs();

    std::cout << std::endl;

    revealed = new char* [lines];
    for (int i = 0; i < lines; ++i)
    {
        revealed[i] = new char[columns];
        for (int j = 0; j < columns; ++j)
        {
            revealed[i][j] = ' ';
        }
    }
}


void Game::displayBoard(int lines_, int columns_)
{
    std::cout << " ";
    for (char letter = 'A'; letter < 'A' + columns_; ++letter)
    {
        std::cout << std::setw(3) << letter;
    }
    std::cout << std::endl;

    for (int i = 0; i < lines_; ++i)
    {
        std::cout << std::setw(2) << (i + 1);
        for (int j = 0; j < columns_; ++j)
        {
            if (revealed[i][j] == 'X')
            {
                std::cout << std::setw(3) << board[i][j];
            }
            else
            {
                std::cout << std::setw(3) << "[ ]";
            }
        }
        std::cout << std::endl;
    }
}

void Game::userPosition(int& enterLine, char& enterColumn, int lines_, int columns_)
{
    std::string userInput;
    while (true)
    {
        std::cout << std::endl;
        std::cout << "Please, enter your position [EX: A1; B2]: ";
        std::cin >> userInput;

        if (userInput.length() < 2 || !isalpha(userInput[0]) || !isdigit(userInput[1]))
        {
            std::cout << "Invalid format! Please enter a valid position like A1 or B2." << std::endl;
            continue;
        }

        enterColumn = toupper(userInput[0]);  // input[0] = la lettre
        enterLine = std::stoi(userInput.substr(1));  // convertit le reste (donc après input[0]) en entier (ex : A10 -> 10)

        int columnIndex = enterColumn - 'A';
        if (enterLine >= 1 && enterLine <= lines_ && columnIndex >= 0 && columnIndex < columns_)
        {
            revealed[enterLine - 1][columnIndex] = 'X';

            if (board[enterLine - 1][columnIndex] == 'B')
            {
                std::cout << std::endl;
                std::cout << "You hit a bomb!" << std::endl;
                std::cout << std::endl;
                std::cout << "------------------------- GAME OVER -------------------------";
                std::cout << std::endl;
                break;
            }

            std::cout << "You entered: " << enterColumn << enterLine << std::endl;
            std::cout << std::endl;
            break;
        }
        else
        {
            std::cout << std::endl;
            std::cout << "Invalid position! Please enter a valid position." << std::endl;
        }
    }
}

void Game::placeBombs()
{
    srand(time(NULL));
    int placedBombs = 0;

    while (placedBombs < numBombs)
    {
        int randomLine = rand() % lines;
        int randomColumn = rand() % columns;

        if (board[randomLine][randomColumn] != 'B')
        {
            board[randomLine][randomColumn] = 'B';
            ++placedBombs;
        }
    }
}

bool Game::hasHitBomb(int enterLine, char enterColumn)
{
    int columnIndex = enterColumn - 'A';
    return board[enterLine - 1][columnIndex] == 'B'; // vérifie si c'est une bombe
}

void Game::adjacentBombs()
{
    for (int lines_ = 0; lines_ < lines; ++lines_)
    {
        for (int columns_ = 0; columns_ < columns; ++columns_)
        {
            if (board[lines_][columns_] == 'B')
            {
                continue;
            }
            int bombCount = 0;

            // là on vérifie les cases adjacentes
            for (int i = -1; i <= 1; ++i)
            {
                for (int j = -1; j <= 1; ++j)
                {
                    int adjLines = lines_ + i;
                    int adjColumns = columns_ + j;

                    // check si les cases sont dans le plateau
                    if (adjLines >= 0 && adjLines < lines && adjColumns >= 0 && adjColumns < columns)
                    {
                        if (board[adjLines][adjColumns] == 'B')
                        {
                            ++bombCount;
                        }
                    }
                }
            }
            if (bombCount > 0) // stock le nombre de bombe
            {
                board[lines_][columns_] = '0' + bombCount;  // convertit le nombre de bombe en caractère
            }
        }
    }
}
