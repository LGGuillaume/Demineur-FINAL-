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

    // Initialisation du jeu et calcul des bombes adjacentes
    g.initializeGame(lines, columns);
    g.adjacentBombs();

    while (toupper(continueGame) == 'Y' && toupper(restartGame) == 'Y')
    {
        g.displayBoard(lines, columns);  // Afficher le plateau

        // Demander la position de l'utilisateur
        g.userPosition(enterLine, enterColumn, lines, columns);

        // Vérifier si l'utilisateur a touché une bombe
        if (g.hasHitBomb(enterLine, enterColumn))
        {
            std::cout << std::endl;
            std::cout << "Restart ? (Y/N): ";
            std::cin >> restartGame;

            if (toupper(restartGame) == 'Y')
            {
                std::cout << std::endl;
                g.initializeGame(lines, columns); // Réinitialiser le jeu
                continue;
            }
            else
            {
                break;
            }
        }

        g.displayBoard(lines, columns); // Afficher à nouveau le plateau après le mouvement

        // Demander à l'utilisateur s'il souhaite continuer à jouer
        std::cout << std::endl;
        std::cout << "Do you want to continue? (Y/N): ";
        std::cin >> continueGame;

        // Validation de l'entrée utilisateur
        while (toupper(continueGame) != 'Y' && toupper(continueGame) != 'N')
        {
            std::cout << "Error. Yes or No ?: ";
            std::cin >> continueGame;
        }
    }

    // Message de fin de jeu
    if (toupper(continueGame) == 'N' || toupper(restartGame) == 'N')
    {
        std::cout << std::endl;
        std::cout << "Thank you for playing!";
        std::cout << std::endl;
    }

    return 0;
}
