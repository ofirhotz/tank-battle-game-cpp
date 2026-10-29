#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <string.h>
#include "io_utils.h"
#include "gameConfig.h"
#include "point.h"
#include "theTankBattle.h"
using namespace std;

int main(int argc, char* argv[])
{
    theTankBattle game;
    GameMode gameMode = REGULAR;
    bool silent = false;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-load") == 0)
        {
            gameMode = LOAD;
        }
        else if (strcmp(argv[i], "-save") == 0)
        {
            gameMode = SAVE;
        }
        else if (strcmp(argv[i], "-silent") == 0)
        {
            silent = true;
        }
    }
    if (silent && (gameMode != LOAD))
    {
        silent = false;
    }
    game.setGameMode(gameMode);
    game.setSilent(silent);
    char userInput = ' ';
    srand((unsigned int)(time(NULL)));

    while (userInput != '9') {
        // only clear when showing menu
        IO_Utils::clear_screen();
        game.displayMenu();
        if (!silent) {
            cout << "Press a key to choose an option..." << endl;
        }
        if (gameMode != REGULAR) {
            userInput = '1';
        }
        // if userInput == '9', leave that value to exit
        if (userInput != '9') {
            userInput = _getch(); // waits for keypress
        }

        switch (userInput) {
        case '1':
            IO_Utils::clear_screen();
            if (!game.init())
            {
                userInput = '9';
                break;
            }
            game.run();
            break;
        case '8':
            IO_Utils::clear_screen();
            game.displayInstructions();
            system("pause"); // pause so user can read
            break;
        case '9':
            IO_Utils::clear_screen();
            game.exitGame();
            break;
        default:
            if (!silent) {
                cout << "Invalid input! Please select a valid option." << endl;
                system("pause");
            }
        }
    }

    return 0;
}
