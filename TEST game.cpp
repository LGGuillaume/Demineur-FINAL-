#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <ctime>
#include "game.h"

Game::~Game() // Destructor
{
    for (int i = 0; i < lines; ++i)
    {
        delete[] board[i];
        delete[] revealed[i];
    }
    delete[] board;
    delete[] revealed;
}

void Game::initializeGame(int& lines_, int& columns_) // Main Game
{
    int level = 0;

    std::cout << "--- Welcome to Minesweeper ---" << std::endl;
    std::cout << std::endl;
    std::cout << "Choose your difficulty: 1.EASY (10 BOMBS) | 2.NORMAL (20 BOMBS) | 3.HARD (30 BOMBS)" << std::endl;
    std::cout << "Which one do you pick: ";
    std::cin >> level;

    switch (level)
    {
    case 1:
        lines_ = 5;
        columns_ = 10;
        numBombs = 10;

        break;
    case 2:
        lines_ = 10;
        columns_ = 10;
        numBombs = 20;
        break;
    case 3:
        lines_ = 10;
        columns_ = 15;
        numBombs = 1;
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

    // First board with empty cells
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

    // Second board
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

void Game::displayBoard(int lines_, int columns_) // Function that display the board
{
    std::cout << "  ";
    for (char letter = 'A'; letter < 'A' + columns_; ++letter)
    {
        std::cout << std::setw(3) << letter;
    }
    std::cout << std::endl;

    for (int i = 0; i < lines_; ++i)
    {
        std::cout << std::setw(2) << (i + 1) << "|";
        for (int j = 0; j < columns_; ++j)
        {
            if (revealed[i][j] == 'X')  // Revealed cell
            {
                std::cout << " " << board[i][j] << " ";
            }
            else if (revealed[i][j] == 'F')  // Cell is flagged
            {
                std::cout << " F ";
            }
            else  // Cell is not revealed
            {
                std::cout << std::setw(3) << "[ ]";
            }
        }
        std::cout << std::endl;
    }
}

void Game::userPosition(int& enterLine, char& enterColumn, int lines_, int columns_) // Main Function Loop
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

        enterColumn = toupper(userInput[0]);
        enterLine = std::stoi(userInput.substr(1));

        int columnIndex = enterColumn - 'A';
        int lineIndex = enterLine - 1;

        if (enterLine >= 1 && enterLine <= lines_ && columnIndex >= 0 && columnIndex < columns_)
        {
            char action;
            std::cout << "Flag(F) or Reveal(R)? ";
            std::cin >> action;
            action = toupper(action);

            if (action == 'R')
            {
                // If the cell is flagged, the player cannot reveal it
                if (revealed[lineIndex][columnIndex] == 'F')
                {
                    std::cout << "You cannot reveal a flagged position. Remove the flag first." << std::endl;
                    continue;
                }

                revealed[lineIndex][columnIndex] = 'X';

                
                if (board[lineIndex][columnIndex] == ' ')
                {
                    floodfill(lineIndex, columnIndex);
                }

                if (board[lineIndex][columnIndex] == 'B')
                {
                    break;
                }

                std::cout << "You revealed: " << enterColumn << enterLine << std::endl;
            }
            else if (action == 'F')
            {
                flag(enterLine, enterColumn);
            }
            else
            {
                std::cout << std::endl;
                std::cout << "Hmm, ok I'll ask again.." << std::endl;
                continue;
            }
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

void Game::placeBombs() // Function that place bombs
{
    int placedBombs = 0;
    srand(time(NULL));

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

bool Game::hasHitBomb(int enterLine, char enterColumn) // Function that return true if the revealed cell is a bomb
{
    int columnIndex = enterColumn - 'A';
    int lineIndex = enterLine - 1;

    if (revealed[lineIndex][columnIndex] == 'F') // If the cell is a flag, it will not count as 'hasHitBomb'
    {
        return false;
    }
    return board[lineIndex][columnIndex] == 'B'; // Checking if there's a bomb
}

int Game::adjacentBombs() // Function to check the adjacent cells
{
    int totalBombCount = 0;

    for (int lines_ = 0; lines_ < lines; ++lines_)
    {
        for (int columns_ = 0; columns_ < columns; ++columns_)
        {
            if (board[lines_][columns_] == 'B') // If the cell is a bomb, skip checking adjacent cells
            {
                continue;
            }
            int bombCount = 0;

            // Checking adjacent cells
            for (int i = -1; i <= 1; ++i)
            {
                for (int j = -1; j <= 1; ++j)
                {
                    int adjLines = lines_ + i;
                    int adjColumns = columns_ + j;

                    // Checking if the cells are in the board
                    if (adjLines >= 0 && adjLines < lines && adjColumns >= 0 && adjColumns < columns)
                    {
                        if (board[adjLines][adjColumns] == 'B') // If the adjacent cell is a bomb, it adds +1 to count the number of bomb
                        {
                            ++bombCount;
                        }
                    }
                }
            }

            if (bombCount > 0) // Store the number of adjacent bombs
            {
                board[lines_][columns_] = '0' + bombCount;  // Convert bomb count to character
            }

            totalBombCount += bombCount;
        }
    }

    return totalBombCount;
}
void Game::flag(int enterLine, char enterColumn) // Function to put a flag
{
    int columnIndex = enterColumn - 'A';
    int lineIndex = enterLine - 1;

    if (revealed[lineIndex][columnIndex] == 'F')
    {
        revealed[lineIndex][columnIndex] = ' ';  // Put the cell back to unrevealed state
    }
    else if (revealed[lineIndex][columnIndex] == ' ')  // Only if the cell is not revealed
    {
        revealed[lineIndex][columnIndex] = 'F';  // Put a flag
    }
}

bool Game::checkWin() // Function for win condition
{
    int revealedCount = 0;
    int flaggedBombs = 0;

    for (int i = 0; i < lines; ++i)
    {
        for (int j = 0; j < columns; ++j)
        {
            if (revealed[i][j] == 'X' && revealed[i][j] != 'B') // If the cell is empty and not a bomb
            {                                                   // It keeps revealing
                ++revealedCount;
            }
            if (revealed[i][j] == 'F' && board[i][j] == 'B')    // If the revealed cell is a flag and a bomb
            {                                                   // It adds +1 to count the flagged bomb
                ++flaggedBombs;
            }
        }
    }

    int totalNoBomb = (lines * columns) - numBombs;
    if (revealedCount == totalNoBomb && flaggedBombs == numBombs) // If the number of reveal is equal to the total of empty cells
    {                                                             // And the number of flagged bombs is equal to the number of bomb
        return true;                                              // The player win
    }

    return false;
}

char Game::getRevealed(int lines, int columns) // Function for main.cpp
{                                            // Used to get revealed[i][j]'s board
    return revealed[lines][columns];
}

void Game::floodfill(int lines_, int columns_) // Floodfill Function
{
    if (lines_ < 0 || lines_ >= lines || columns_ < 0 || columns_ >= columns)
    {
        return;
    }

    //int bombesAdj = adjacentBombs(); TESTING WITHOUT IT
    if (board[lines_][columns_] != 'B')
    {
        // Révélez les cases adjacentes
        for (int i = lines - 1; i < lines + 1; ++i)
        {
            for (int j = columns - 1; j < columns + 1; ++j)
            {
                floodfill(lines + i, columns + j);
            }
        }
    }
}
