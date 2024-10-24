#include <iostream>
#include "game.h"

int main()
{
    Game g;

    int lines = 0;
    int columns = 0;
    int enterLine = 0;
    char enterColumn = 0;
    char restartGame = 'Y';

    g.initializeGame(lines, columns);

    // Main Game Loop
    while (toupper(restartGame) == 'Y')
    {
        g.displayBoard(lines, columns);
        g.userPosition(enterLine, enterColumn, lines, columns);

        // Check only after reveal
        if (g.getRevealed(enterLine - 1, enterColumn - 'A') == 'X' && g.hasHitBomb(enterLine, enterColumn))
        {
            std::cout << std::endl;
            std::cout << "You hit a bomb!" << std::endl;
            std::cout << "------------ GAME OVER ------------";
            std::cout << std::endl;
            std::cout << "Restart? (Y/N): ";
            std::cin >> restartGame;

            if (toupper(restartGame) == 'Y')
            {
                std::cout << std::endl;
                g.initializeGame(lines, columns);
                continue;
            }
            else
            {
                break;
            }

            while (toupper(restartGame) != 'N')
            {
                std::cout << "Error. Yes or No ?: ";
                std::cin >> restartGame;
            }
        }

        // Check for victory
        if (g.checkWin())
        {
            std::cout << "Congratulations! You've won!" << std::endl;
            std::cout << "Restart? (Y/N): ";
            std::cin >> restartGame;

            if (toupper(restartGame) == 'Y')
            {
                std::cout << std::endl;
                g.initializeGame(lines, columns);
                continue;
            }
            else
            {
                break;
            }
        }
    }

    if (toupper(restartGame) == 'N')
    {
        std::cout << std::endl;
        std::cout << "Thank you for playing!";
        std::cout << std::endl;
    }
    return 0;
}
