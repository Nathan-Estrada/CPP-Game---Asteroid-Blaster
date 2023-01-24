#include <iostream>
#include "Game.h"
using namespace std;

int main()
{
    //Initialize randomness
    std::srand(static_cast<unsigned>(time(NULL)));

    //Initialize game engine
    Game game;

    //Game loop
    game.runGame();

    //End of application
    return 0;
}

