#include <iostream>
#include "game.h"

int main() {
    Game g;

    int lines = 0;
    int columns = 0;
    int enterLine = 0;
    char enterColumn = 0;
    char continueGame = 'Y';
    char restartGame = 'Y';

    g.initializeGame(lines, columns);
    g.adjacentBombs();

    while (toupper(continueGame) == 'Y' && toupper(restartGame) == 'Y')
    {
        g.displayBoard(lines, columns);

        g.userPosition(enterLine, enterColumn, lines, columns);

        if (g.hasHitBomb(enterLine, enterColumn))
        {
            std::cout << std::endl;
            std::cout << "Restart ? (Y/N): ";
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

        g.displayBoard(lines, columns);

        std::cout << std::endl;
        std::cout << "Do you want to continue? (Y/N): ";
        std::cout << std::endl;
        std::cin >> continueGame;

        while (toupper(continueGame) != 'Y' && toupper(continueGame) != 'N')
        {
            std::cout << "Error. Yes or No ?: ";
            std::cin >> continueGame;
        }
    }

    if (toupper(continueGame) == 'N' || toupper(restartGame) == 'N')
    {
        std::cout << std::endl;
        std::cout << "Thank you for playing!";
        std::cout << std::endl;
    }

    return 0;
}
